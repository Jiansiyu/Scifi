///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// SBSHCal                                                               //
//                                                                           //
// Shower counter class, describing a generic segmented shower detector      //
// (preshower or shower).                                                    //
// Currently, only the "main" cluster, i.e. cluster with the largest energy  //
// deposition is considered. Units of measurements are MeV for energy of     //
// shower and centimeters for coordinates.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "SBSHCal.h"

//#include "THaBBe.h"
//#include "THaGlobals.h"

#include "THaEvData.h"
//#include "THaCodaDecoder.h"
#include "Fadc250Module.h"
#include "THaDetMap.h"
#include "VarDef.h"
#include "VarType.h"
#include "THaTrack.h"
#include "TClonesArray.h"
#include "TDatime.h"
#include "TMath.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#define CLUSTER_BLOCK_RADIUS 1
#define MAX_FADC_SAMPLES 400 // Max number of samples allowed

using namespace std;

ClassImp(SBSHCal)

//_____________________________________________________________________________
SBSHCal::SBSHCal( const char* name, const char* description,
                         THaApparatus* apparatus ) :
THaPidDetector(name,description,apparatus), fNChan(NULL)
{
    // Constructor.
    fCoarseProcessed = 0;
    fFineProcessed = 0;
}

//_____________________________________________________________________________
Int_t SBSHCal::ReadDatabase( const TDatime& date )
{
  // Read database for HCal detector.
  // This function is called by THaDetectorBase::Init() once at the
  // beginning of the analysis.
  // 'date' contains the date/time of the run being analyzed.
  // Most of this was borrowed from THaShower as a starting point

  // We can use this name here for logs
  static const char* const here = "ReadDatabase()";

  // Read database

  FILE* file = OpenFile( date );
  if( !file ) return kFileError;

  // Read fOrigin and fSize (required!)
  Int_t err = ReadGeometry( file, date, true );
  if( err ) {
    fclose(file);
    return err;
  }

  vector<Int_t> detmap, chanmap;
  vector<Double_t> xy, dxy;
  Int_t ncols, nrows;
  Double_t angle = 0.0;

  // Read mapping/geometry/configuration parameters
  DBRequest config_request[] = {
    { "detmap",       &detmap,  kIntV },
    { "chanmap",      &chanmap, kIntV,    0, 1 },
    { "ncols",        &ncols,   kInt },
    { "nrows",        &nrows,   kInt },
    { "angle",        &angle,   kDouble,  0, 1 },
    { "xy",           &xy,      kDoubleV, 2 },  // center pos of block 1
    { "dxdy",         &dxy,     kDoubleV, 2 },  // dx and dy block spacings
    { "emin",         &fEmin,   kDouble },
    { 0 }
  };
  err = LoadDB( file, date, config_request, fPrefix );

  // Sanity checks
  if( !err && (nrows <= 0 || ncols <= 0) ) {
    Error( Here(here), "Illegal number of rows or columns: %d %d. Must be > 0. "
	   "Fix database.", nrows, ncols );
    err = kInitError;
  }

  Int_t nelem = ncols * nrows; 
  Int_t nclbl = TMath::Min( 3, nrows ) * TMath::Min( 3, ncols );

  // Reinitialization only possible for same basic configuration
  if( !err ) {
    if( fIsInit && (nelem != fNelem || nrows != fNrows || ncols != fNcols ) ) {
      Error( Here(here), "Cannot re-initalize with different number of blocks or "
          "blocks per cluster (was: %d, now: %d). Detector not re-initialized.",
          fNelem, nelem );
      err = kInitError;
    } else {
      fNelem = nelem;
      fNrows = nrows;
      fNcols = ncols;
      fNclublk = nclbl;
    }
  }

  if( !err ) {
    // Clear out the old channel map before reading a new one
    fChanMap.clear();
    if( FillDetMap(detmap, 0, here) <= 0 ) {
      err = kInitError;  // Error already printed by FillDetMap
    } else if( (nelem = fDetMap->GetTotNumChan()) != fNelem ) {
      Error( Here(here), "Number of detector map channels (%d) "
          "inconsistent with number of blocks (%d)", nelem, fNelem );
      err = kInitError;
    }
  }
  if( !err ) {
    if( !chanmap.empty() ) {
      // If a map is found in the database, ensure it has the correct size
      Int_t cmapsize = chanmap.size();
      if( cmapsize != fNelem ) {
        Error( Here(here), "Channel map size (%d) and number of detector "
            "channels (%d) must be equal. Fix database.", cmapsize, fNelem );
        err = kInitError;
      }
    }
    if( !err ) {
      // Set up the new channel map
      Int_t nmodules = fDetMap->GetSize();
      assert( nmodules > 0 );
      fChanMap.resize(nmodules);
      for( Int_t i=0, k=0; i < nmodules && !err; i++ ) {
        THaDetMap::Module* module = fDetMap->GetModule(i);
        Int_t nchan = module->hi - module->lo + 1;
        if( nchan > 0 ) {
          fChanMap.at(i).resize(nchan);
          for( Int_t j=0; j<nchan; ++j ) {
            assert( k < fNelem );
            fChanMap.at(i).at(j) = chanmap.empty() ? k+1 : chanmap[k];
            ++k;
          }
        } else {
          Error( Here(here), "No channels defined for module %d.", i);
          fChanMap.clear();
          err = kInitError;
        }
      }
    }
  }

  // Read calibration parameters
  // Default ADC pedestals (0) and ADC gains (1)
  fPed.resize(fNelem);
  fGain.resize(fNelem);
  ResetVector(fPed,Float_t(0.0));
  ResetVector(fGain,Float_t(1.0));

  // Read ADC pedestals and gains (in order of logical channel number)
  DBRequest calib_request[] = {
    { "pedestals",   &fPed,   kFloatV, UInt_t(fNelem), 1 },
    { "gains",       &fGain,  kFloatV, UInt_t(fNelem), 1 },
    { 0 }
  };
  err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;

  DefineAxes( angle*TMath::DegToRad() );

  // Dimension arrays
  if( !fIsInit ) {
    // Compute block positions and creates blocks array
    fBlkGrid.clear();
    fBlkGrid.resize(fNrows);
    for (int i=0;i<fNrows;i++) fBlkGrid[i].resize(fNcols);
    //2//fClusters = new SBSHCalCluster*[fMaxNClust];
    fBlocks.clear();
    fBlocks.resize(fNelem);
    fASamples.resize(fNelem);
    fASamplesPed.resize(fNelem);
    fASamplesCal.resize(fNelem);
    fNumSamples.resize(fNelem);
    for(Int_t i = 0; i < fNelem; i++) {
      // We'll resize the vector now to make sure the data are contained
      // in a contigous part of memory (needed by THaOutput when writing
      // to the ROOT file)
      fASamples[i].resize(MAX_FADC_SAMPLES);
      fASamplesPed[i].resize(MAX_FADC_SAMPLES);
      fASamplesCal[i].resize(MAX_FADC_SAMPLES);
    }
    fBsum.resize(fNelem); // MAPC
    // Yup, hard-coded in because it's only a test
    // TODO: Fix me, don't hard code it in
    fMaxNClust = 9;
    fE = new Float_t[UInt_t(fMaxNClust)];
    fE_c = new Float_t[UInt_t(fMaxNClust)];
    fX = new Float_t[UInt_t(fMaxNClust)];
    fY = new Float_t[UInt_t(fMaxNClust)];
    fMult = new Int_t[UInt_t(fMaxNClust)];

    fNblk = new Int_t[UInt_t(fNclublk)];
    fEblk = new Float_t[UInt_t(fNclublk)];

    for( int r=0; r<nrows; r++ ) {
      for( int c=0; c<ncols; c++ ) {
        int k = nrows*c + r;
        Float_t x = xy[0] + r*dxy[0];
        Float_t y = xy[1] + r*dxy[1];
        SBSShowerBlock* block = 
          new SBSShowerBlock(x,y,fPed[k],fGain[k],r,c);
        fBlocks[k]=block;
        fBlkGrid[r][c]=fBlocks[k];
      }
    }
    fIsInit = true;
  }

  return kOK;
}

//_____________________________________________________________________________
Int_t SBSHCal::DefineVariables( EMode mode )
{
  // Initialize global variables

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );
  cout << "--> " << mode << endl;
  
  // Register variables in global list
  RVarDef vars[] = {
    { "nhit",   "Number of hits",                     "fNhits" },
    { "asum_p", "Sum of ped-subtracted ADCs",         "fAsum_p" },
    { "asum_c", "Sum of calibrated ADCs",             "fAsum_c" },
    { "nclust", "Number of clusters",                 "fNclust" },
    { "e",      "Energy (MeV) of largest cluster",    "fE" },
    { "e_c",    "Corrected Energy (MeV) of largest cluster",    "fE_c" },
    { "x",      "x-position (m) of largest cluster", "fX" },
    { "y",      "y-position (m) of largest cluster", "fY" },
    //{ "targ.x", "x-position (m) of largest cluster in target coords", "fXtarg" },
    //{ "targ.y", "y-position (m) of largest cluster in target coords", "fYtarg" },
    //{ "targ.z", "z-position (m) of largest cluster in target coords", "fZtarg" },
    { "mult",   "Multiplicity of largest cluster",    "fMult" },
    { "nblk",   "Numbers of blocks in main cluster",  "fNblk" },
    { "eblk",   "Energies of blocks in main cluster", "fEblk" },
    //     { "trx",    "track x-position in det plane",      "fTRX" },
    //     { "try",    "track y-position in det plane",      "fTRY" },
    {"blksum", "Integrated raw pulses for each channel", "fBsum" }, // MAPC
    //{"a_wave", "Raw data for each channel", "fAwave" }, // MAPC
    { 0 }
  };
  Int_t err = DefineVarsFromList(vars, mode);
  if( err != kOK )
    return err;

  // Now register 
  std::vector<VarDef> vars2;
  for(Int_t m = 0; m < fNelem; m++) {
    VarDef v;
    char *name   =  new char[128];
    char *name_p = new char[128];
    char *name_c = new char[128];
    sprintf(name,"a.raw%.2d",m);
    sprintf(name_p,"a_p.m%d",m);
    sprintf(name_c,"a_c.m%d",m);
    char *desc = new char[256];
    sprintf(desc,"Raw ADC samples for Module %d",m);
    v.name = name;
    v.desc = "Raw ADC samples";
    v.type = kDouble;
    v.size = MAX_FADC_SAMPLES;
    v.loc  = &(fASamples[m].data()[0]); //JW: location of data
    v.count = &fNumSamples[m];
    vars2.push_back(v);
    v.name = name_p;
    v.desc = "Pedestal subtracted ADC samples";
    v.loc = &(fASamplesPed[m].data()[0]);
    vars2.push_back(v);
    v.name = name_c;
    v.desc = "Pedestal subtracted calibrated ADC samples";
    v.loc = &(fASamplesCal[m].data()[0]);
    vars2.push_back(v);
  }
  vars2.push_back(VarDef());
  return DefineVarsFromList( vars2.data(), mode );
}

//_____________________________________________________________________________
SBSHCal::~SBSHCal()
{
    // Destructor. Removes internal arrays and global variables.

    if( fIsSetup )
        RemoveVariables();
    if( fIsInit )
        DeleteArrays();
}

//_____________________________________________________________________________
void SBSHCal::DeleteArrays()
{
    // Delete member arrays. Internal function used by destructor.

    fChanMap.clear();
    delete [] fNChan; fNChan = 0;
    delete [] fNblk;    fNblk    = 0;
    delete [] fEblk;    fEblk    = 0;

    // Clear vectors
    for(size_t i = 0; i < fBlocks.size(); i++) {
      if (fBlocks[i])
        delete fBlocks[i];
    }
    fBlocks.clear();
    for(size_t r = 0; r < fBlkGrid.size(); r++) {
      for(size_t c = 0; c < fBlkGrid[r].size(); c++) {
        if(fBlkGrid[r][c])
          delete fBlkGrid[r][c];
      }
      fBlkGrid[r].clear();
    }
    fBlkGrid.clear();
    //2//delete [] fClusters; fClusters = 0;
    delete [] fX; fX = 0;
    delete [] fY; fY = 0;
    //delete [] fXtarg; fXtarg = 0;
    //delete [] fYtarg; fYtarg = 0;
    //delete [] fZtarg; fZtarg = 0;
    delete [] fE; fE = 0;
    delete [] fE_c; fE_c = 0;
    delete [] fMult; fMult = 0;
}

//_____________________________________________________________________________
inline
void SBSHCal::ClearEvent()
{
  // Reset all local data to prepare for next event.
  ResetVector(fNumSamples,0);
  ResetVector(fASamples,0.0);
  ResetVector(fASamplesPed,0.0);
  ResetVector(fASamplesCal,0.0);

    fCoarseProcessed = 0;
    fFineProcessed = 0;

    const int lsh = fNelem*sizeof(Float_t);
    const int lshh = fMaxNClust*sizeof(Float_t);
    const int lsc = fNclublk*sizeof(Float_t);
    const int lsi = fNclublk*sizeof(Int_t);
    const int lsj = fMaxNClust*sizeof(Int_t);

    fNhits = 0;
    memset( fE, 0, lshh );
    memset( fE_c, 0, lshh );
    memset( fX, 0, lshh );
    memset( fY, 0, lshh );
    //memset( fXtarg, 0, lshh );
    //memset( fYtarg, 0, lshh );
    //memset( fZtarg, 0, lshh );
    memset( fMult, 0, lsj );
    fAsum_p = 0.0;
    fAsum_c = 0.0;
    fNclust = 0;
    memset( fNblk, 0, lsi );
    memset( fEblk, 0, lsc );
    fTRX = 0.0;
    fTRY = 0.0;
  
    for(size_t i=0; i<fBsum.size(); i++) fBsum[i] = 0.0; // MAPC

    //2//for (int i=0;i<fNelem;i++) 
        //2//fBlocks[i]->ClearEvent();

}

//_____________________________________________________________________________
Int_t SBSHCal::Decode( const THaEvData& evdata )
{
  // Decode data and store into the following local data structure:
  //
  // fNhits           - Number of hits on HCal
  // fASamples[][]    - 2D Array of ADC samples/values for each block
  // fASamplesPed[][] - 2D Array of ped subtracted fASamples[][]
  // fASamplesCal[][] - 2D array of ped subtracted and calibrated fASamples[][]
  //
  // (The following are presently now being used)
  // fAsum_p          -  Sum of shower blocks ADC minus pedestal values;
  // fAsum_c          -  Sum of shower blocks corrected ADC values;
  //
  // fBsum[]            - fASamples integrated for each block // MAPC

  // Clear last event
  ClearEvent();
  static const char* const here = "Decode()";
  
  // Loop over all modules defined for HCal
  for( UShort_t i = 0; i < fDetMap->GetSize(); i++ ) {
    THaDetMap::Module *d = fDetMap->GetModule( i );
    Bool_t isADC = false;
    //Bool_t isTDC = false;

    // Get a pointer to the abstract module, and we'll proceed
    // depending on the type of module we encounter
    Decoder::Module *m = evdata.GetModule(d->crate,d->slot);

    // Try to see if it's an FADC
    Decoder::Fadc250Module *fadc = dynamic_cast<Decoder::Fadc250Module*>(m);
    if(fadc != NULL) isADC = true;

// MAPC
//if(d->slot == 17 || d->slot == 18) isADC=true;
//Int_t chan = evdata.GetNextChan( d->crate, d->slot, 0 );
//cout << isADC << " " << d->crate << " " << d->slot << " " << endl;//<< fadc->GetNumFadcSamples(0,0) << endl;
//cout << "--> " << evdata.GetNumChan( d->crate, d->slot ) << endl;

    // Loop over all channels that have a hit.
    for( Int_t j = 0; j < evdata.GetNumChan( d->crate, d->slot ); j++) {
      // Increment the number of hits
      fNhits++;

      // Get the next available channel, but 
      Int_t chan = evdata.GetNextChan( d->crate, d->slot, j );
      // Skip channels that do not belong to our detector
      if( chan > d->hi || chan < d->lo ) continue;

      // Get the block that corresponds to this module and channel
      Int_t jchan = (d->reverse) ? d->hi - chan : chan-d->lo;
      Int_t k = fChanMap[i][jchan] - 1;


      // Is this module an FADC?
      if(isADC) {
        // Get the mode, nu ber of events and number of samples from
        // the event data.
        //   (jc2) We can probably later specify just a fixed number of samples
        //   (perhaps in the DB itself) if this proves too slow.
        Int_t mode, num_events, num_samples;
        Bool_t raw_mode = kFALSE;
	//cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
        mode = fadc->GetFadcMode();
//cout << mode << endl;
        raw_mode = (mode == 1) || (mode == 8) || (mode == 10);
        num_events = fadc->GetNumFadcEvents(chan);

        // In raw mode, raw samples are read out for each hit
        if( raw_mode) {
          num_samples = fadc->GetNumFadcSamples(chan,0);
          if(num_samples > MAX_FADC_SAMPLES || num_samples < 0) {
            Error( Here(here),
                "Too manu samples in fADC: %d out of %d MAX",num_samples,
                MAX_FADC_SAMPLES);
          } else {
            fNumSamples[k] = num_samples;
            std::vector<UInt_t> samples = fadc->GetPulseSamplesVector(chan);
            for(Int_t s = 0; s < num_samples; s++) {
//cout << samples[s] << endl;
              fASamples[k][s] = samples[s];
              fASamplesPed[k][s] = fASamples[k][s]-fPed[k];
              fASamplesCal[k][s] = fASamplesPed[k][s]*fGain[k];
              fBsum[k] += samples[s];
            }
	  }
        }
      }
    }
  }

  return fNhits;
}


//_____________________________________________________________________________
Int_t SBSHCal::CoarseProcess(TClonesArray& tracks)
{
    // Reconstruct Clusters in shower detector and copy the data 
    // into the following local data structure:
    //
    // fNclust        -  Number of clusters in shower;
    // fE             -  Energy (in MeV) of the "main" cluster;
    // fX             -  X-coordinate (in cm) of the cluster;
    // fY             -  Y-coordinate (in cm) of the cluster;
    // fMult          -  Number of blocks in the cluster;
    // fNblk[0]...[5] -  Numbers of blocks composing the cluster;
    // fEblk[0]...[5] -  Energies in blocks composing the cluster;
    // fTRX;          -  X-coordinate of track cross point with shower plane
    // fTRY;          -  Y-coordinate of track cross point with shower plane
    //

  /*
    if( fCoarseProcessed ) return 0;

    Int_t col, row;
    Int_t colmax=0, rowmax=0;
    Double_t  energy_prev = 0.0;

# if not defined(_WIN32)//Win32 compiler do not support variable as array size
    Double_t energyDep[fNcols][fNrows];
# else
    Double_t energyDep[100][100];
# endif

    Double_t energyTotal = 0.0;
    SBSBBShowerCluster cluster(9);

    //  for( col = 0; col < fNcols; col++ )
    //     {
    //       for( row = 0; row < fNrows; row++ )
    // 	{
    // 	  energyDep[col][row] = 0.0;
    // 	}
    //     }

    //  cout << "Energy Deposition:" << endl <<"___________________________________________________" << endl;
    for( row = 0; row < fNrows; row++ )
    {
        for( col = 0; col < fNcols; col++ )
        {
            energyDep[col][row] = fA_c[BlockColRowToNumber(col,row)]; 

            //	  cout << energyDep[col][row] << " ";
            if( energyDep[col][row] < 0.0 ) 
                energyDep[col][row] = 0.0;
            energyTotal += energyDep[col][row];
        }
        //      cout << endl;
    }

    for( row = 0; row < fNrows; row++ )
    {
        for( col = 0; col < fNcols; col++ )
        {
            if(energyDep[col][row]>energy_prev)
            {
                energy_prev=energyDep[col][row];
                colmax = col;
                rowmax = row;
            }
        }
    }


    //  cout <<"___________________________________________________" << endl;

    Int_t i, j, k=0;
    Double_t energyClusterTotal = 0.0;
    //  Double_t energyClusterGreatest = 0.0;

    Int_t clusterRow = 0;
    Int_t clusterCol = 0;

    //  for( row = 0; row < fNrows; row++ )
    //     {
    //       for( col = 0; col < fNcols; col++ )
    // 	{
    // 	  energyClusterTotal = 0.0;
    // 	  for( i = row-CLUSTER_BLOCK_RADIUS; i <= row+CLUSTER_BLOCK_RADIUS; i++ )
    // 	    {
    // 	      for( j = col-CLUSTER_BLOCK_RADIUS; j <= col+CLUSTER_BLOCK_RADIUS; j++)
    // 		{
    // 		  if( (i >= 0 && i < fNrows ) && ( j >=0 && j < fNcols ) ){   
    // 		    energyClusterTotal += energyDep[j][i];
    // 		  }
    // 		}
    // 	    }

    // 	  if( energyClusterTotal > energyClusterGreatest )
    // 	    {
    // 	      energyClusterGreatest = energyClusterTotal;
    // 	      clusterRow = row;
    // 	      clusterCol = col;
    // 	    }
    // 	}
    //     }
    energyClusterTotal = 0.0; 
    Int_t
        mnrow=TMath::Max(rowmax-CLUSTER_BLOCK_RADIUS,0),
        mxrow=TMath::Min(rowmax+CLUSTER_BLOCK_RADIUS,fNrows-1),
        mncol=TMath::Max(colmax-CLUSTER_BLOCK_RADIUS,0),
        mxcol=TMath::Min(colmax+CLUSTER_BLOCK_RADIUS,fNcols-1);

    for( i = mnrow; i <= mxrow; i++ )
    {
        for( j = mncol; j <= mxcol; j++)
        {
            energyClusterTotal += energyDep[j][i];
            fEblk[k] = energyDep[j][i];
            k++;
        }
    }

    //  cout <<"___________________________________________________" << endl;

    Double_t energyCluster = energyClusterTotal;
    Double_t X, Y;

    if( energyCluster < 0.0 ) return 0;

    //  cout << "Got a cluster!" << endl;
    X = fBlockX[BlockColRowToNumber(colmax, rowmax)];
    Y = fBlockY[BlockColRowToNumber(colmax, rowmax)];

    Double_t energyX = 0.0;
    Double_t energyY = 0.0;

    Int_t  blockcounter = 0;
    for( i = clusterRow-CLUSTER_BLOCK_RADIUS; i <= clusterRow + CLUSTER_BLOCK_RADIUS; i++ )
    {
        for( j = clusterCol-CLUSTER_BLOCK_RADIUS; j <= clusterCol + CLUSTER_BLOCK_RADIUS; j++ )
        {
            if( (i >= 0 && i < fNrows ) && ( j >=0 && j < fNcols ) )
            {
                energyX += energyDep[j][i]*fBlockX[BlockColRowToNumber(j,i)];
                energyY += energyDep[j][i]*fBlockY[BlockColRowToNumber(j,i)];

                cluster.AddBlock( fBlocks[BlockColRowToNumber(j,i)] );
                blockcounter++;
            }
        }
    }

    //  cout << energyCluster << " " << energyX/energyCluster << " " << energyY/ energyCluster << " " << cluster.GetMult() << endl;

    cluster.SetE( energyCluster );
    //cluster.SetX( energyX/energyCluster );
    cluster.SetX( X+fOrigin.X() );
    cluster.SetY( Y+fOrigin.Y() );
    //cluster.SetY( energyY/energyCluster );

    AddCluster(cluster);  

    //  cout << "Added - we now have " << fNclust << endl;

    fCoarseProcessed = 1;
    */
    return 0;

}

//_____________________________________________________________________________
Int_t SBSHCal::FineProcess(TClonesArray& tracks)
{

    if( fFineProcessed ) return 0;

    /*
    // Fine Shower processing.

    //   cout << endl << fNclust << " clusters " << GetName()  << endl;
    //   for (int i=0;i<fNclust;i++) {
    //     cout << setw(2) << i << setw(7) << setprecision(1) 
    // 	 << fClusters[i]->GetE() << setw(8) << setprecision(3) 
    // 	 << fClusters[i]->GetX() << setw(8) << fClusters[i]->GetY() 
    // 	 << setw(4) << fClusters[i]->GetMult() << endl;
    //   }

    TVector3 clusterpoint;

    for (int i=0;i<fNclust;i++) {
        //    cout << fClusters[i]->GetE() << " " << fClusters[i]->GetX() << " " << fClusters[i]->GetY() <<fClusters[i]->GetMult()  << endl; 
        fE[i] = fClusters[i]->GetE();
        fE_c[i] = fClusters[i]->GetE()*(gconst + gslope*acc_charge);
        fX[i] = fClusters[i]->GetX();
        fY[i] = fClusters[i]->GetY();
        fMult[i] = fClusters[i]->GetMult();

        //clusterpoint.SetXYZ( fX[i], fY[i], fOrigin.Z() );
        //clusterpoint.Transform(fDetToTarg);
        //clusterpoint = clusterpoint + fDetOffset;

        //fXtarg[i] = clusterpoint.X();
        //fYtarg[i] = clusterpoint.Y();
        //fZtarg[i] = clusterpoint.Z();
        //// We want the shower coordinates in target coordinates

    }

    fFineProcessed = 1;
    */
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

/*
void SBSHCal::AddCluster(SBSBBShowerCluster* clus) {
    fClusters[fNclust++]=clus;
}

void SBSHCal::AddCluster(SBSBBShowerCluster& clus) {

    fClusters[fNclust] = new SBSBBShowerCluster(clus.GetNMaxBlocks());
    fClusters[fNclust]->SetE(clus.GetE());
    fClusters[fNclust]->SetX(clus.GetX());
    fClusters[fNclust]->SetY(clus.GetY());
    fClusters[fNclust++]->SetMult(clus.GetMult());
}

void SBSHCal::RemoveCluster(int i) {
    fNclust--;
    for (int j=i;j<fNclust;j++) fClusters[j]=fClusters[j+1];
}
*/

Int_t SBSHCal::BlockColRowToNumber( Int_t col, Int_t row )
{
    return col*fNrows + row;
}

void SBSHCal::LoadMCHitAt( Double_t x, Double_t y, Double_t E )
{  
  /*
    ClearEvent();
    fNclust = 0;
    fClusters[fNclust] = new SBSBBShowerCluster(0);
    fClusters[fNclust]->SetE(E);
    fClusters[fNclust]->SetX(x);
    fClusters[fNclust]->SetY(y);
    fClusters[fNclust]->SetMult(0);   

    fE[fNclust] = fClusters[fNclust]->GetE();
    fX[fNclust] = fClusters[fNclust]->GetX();
    fY[fNclust] = fClusters[fNclust]->GetY();
    fMult[fNclust] = fClusters[fNclust]->GetMult();
    fNclust++;
    */
}


void SBSHCal::SetBlockADCSamples(Int_t block, std::vector<UInt_t> samples)
{
}
