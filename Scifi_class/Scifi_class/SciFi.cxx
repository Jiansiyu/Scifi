///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Scifi                                                                     //
//                                                                           //
// Class for Scifi detector read out by 4 fadc250 modules wiht 16 channels   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//#include "TriFadcCherenkov.h"
#include "SciFi.h"
#include "THaEvData.h"
#include "THaDetMap.h"
#include "VarDef.h"
#include "VarType.h"
#include "THaTrack.h"
#include "TClonesArray.h"
#include "TDatime.h"
#include "TMath.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;
using namespace Decoder;
//_____________________________________________________________________________
SciFi::SciFi( const char* name, const char* description, THaApparatus* apparatus ): THaNonTrackingDetector(name,description,apparatus), fPed(0), fGain(0), fA(0), fA_p(0), fA_c(0),fPeak(0),fT_FADC(0),fT_FADC_c(0), foverflow(0), funderflow(0),fpedq(0),fNhits(0)
{
  // Constructor
  fFADC=NULL;
}

//_____________________________________________________________________________
SciFi::SciFi(): THaNonTrackingDetector(), fPed(0), fGain(0), fA(0), fA_p(0), fA_c(0),fPeak(0), foverflow(0), funderflow(0),fpedq(0),fNhits(0)
{
  // Default constructor (for ROOT I/O)
}

//_____________________________________________________________________________
Int_t SciFi::ReadDatabase( const TDatime& date )
{
  // Read parameters from the database.
  // 'date' contains the date/time of the run being analyzed.

  const char* const here = "ReadDatabase";

  // Read database

  FILE* file = OpenFile( date );
  if( !file ) return kFileError;

  // Read fOrigin and fSize (required!)
  Int_t err = ReadGeometry( file, date, true );
  if( err ) {
    fclose(file);
    return err;
  }

  vector<Int_t> detmap;
  Int_t nelem;
  Double_t angle = 0.0;

  // Read configuration parameters
  DBRequest config_request[] = {
    { "detmap",  &detmap,  kIntV },
    { "nch",    &nelem,   kInt },
    { "angle",   &angle,   kDouble, 0, 1 },
    { 0 }
  };
  err = LoadDB( file, date, config_request, fPrefix );

  // Sanity checks
  if( !err && nelem <= 0 ) {
    Error( Here(here), "Invalid number of PMTs: %d. Must be > 0. "
	   "Fix database.", nelem );
    err = kInitError;
  }

  // Reinitialization only possible for same basic configuration
  if( !err ) {
    if( fIsInit && nelem != fNelem ) {
      Error( Here(here), "Cannot re-initalize with different number of PMTs. "
	     "(was: %d, now: %d). Detector not re-initialized.", fNelem, nelem );
      err = kInitError;
    } else
      fNelem = nelem;
  }
 
  //  UInt_t flags = THaDetMap::kFillLogicalChannel | THaDetMap::kFillModel;

  UInt_t flags = THaDetMap::kFillModel;

  // potential change depending on how database is structured 
  // what is difference between channel and logical channel number

  if( !err && FillDetMap(detmap, flags, here) <= 0 ) {
    err = kInitError;  // Error already printed by FillDetMap
  }


  // Change:: Don't think this is needed

  // if( !err && (nelem = fDetMap->GetTotNumChan()) != 2*fNelem ) {
  //   Error( Here(here), "Number of detector map channels (%d) "
  // 	   "inconsistent with 2*number of PMTs (%d)", nelem, 2*fNelem );
  //   err = kInitError;
  // }

  if( err ) {
    fclose(file);
    return err;
  }

  DefineAxes( angle*TMath::DegToRad() );

  // Dimension arrays
  //FIXME: use a structure!
  UInt_t nval = fNelem;
  if( !fIsInit ) {
    // Calibration data
    // fOff  = new Float_t[ nval ];
    fPed  = new Float_t[ nval ];
    fGain = new Float_t[ nval ];

    // Per-event data
    // fT    = new Float_t[ nval ];
    // fT_c  = new Float_t[ nval ];
    fA    = new Float_t[ nval ];
    fA_p  = new Float_t[ nval ];
    fA_c  = new Float_t[ nval ];

    fPeak      = new Float_t[ nval ];
    fT_FADC    = new Float_t[ nval ];
    fT_FADC_c  = new Float_t[ nval ];
    foverflow  = new Int_t[ nval ]; 
    funderflow = new Int_t[ nval ];
    fpedq      = new Int_t[ nval ];
    fNhits     = new Int_t[ nval ];

    fIsInit = true;
  }

  // Read calibration parameters

  // Set DEFAULT values here
  // TDC resolution (s/channel)
  //  fTdc2T = 0.1e-9;      // seconds/channel

  // Default TDC offsets (0), ADC pedestals (0) and ADC gains (1)
  //  memset( fOff, 0, nval*sizeof(fOff[0]) );
  memset( fPed, 0, nval*sizeof(fPed[0]) );

  fNPED = 1; //number of samples included in FADC pedestal sum
  fNSA = 1;  //number of integration samples after threshold crossing
  fNSB = 1;  //number of integration samples before threshold crossing
  fWin = 1;  //total number of sample in FADC window
  fTFlag = 1;  //Threshold On: 1, Off: 0

  for( UInt_t i=0; i<nval; ++i ) { fGain[i] = 1.0; }

  DBRequest calib_request[] = {
    // { "tdc.offsets",      fOff,         kFloat, nval, 1 },
    { "pedestals",    fPed,         kFloat, nval, 1 },
    { "gains",        fGain,        kFloat, nval, 1 },
    //    { "tdc.res",          &fTdc2T,      kDouble },
    { "NPED",             &fNPED,        kInt},
    { "NSA",              &fNSA,         kInt},
    { "NSB",              &fNSB,         kInt},
    { "Win",              &fWin,         kInt},
    { "TFlag",            &fTFlag,       kInt},
    { 0 }
  };
  err = LoadDB( file, date, calib_request, fPrefix );
  fclose(file);
  if( err )
    return err;

  return kOK;
}

//_____________________________________________________________________________
Int_t SciFi::DefineVariables( EMode mode )
{
  // Initialize global variables

  if( mode == kDefine && fIsSetup ) return kOK;
  fIsSetup = ( mode == kDefine );

  RVarDef vars[] = {
    // { "nthit",  "Number of Left paddles TDC times",  "fNThit" },
    // { "nahit",  "Number of Right paddles TDC times", "fNAhit" },
    // { "t",      "TDC values",                        "fT" },
    // { "t_c",    "Corrected TDC values",              "fT_c" },
    { "a",      "ADC values",                        "fA" },
    { "a_p",    "Ped-subtracted ADC values ",        "fA_p" },
    { "a_c",    "Corrected ADC values",              "fA_c" },
    { "peak",   "FADC ADC peak values",              "fPeak" },
    { "t_fadc", "FADC TDC values",                   "fT_FADC" },
    { "tc_fadc", "FADC corrected TDC values",        "fT_FADC_c" },
    { "asum_p", "Sum of ADC minus pedestal values",  "fASUM_p" },
    { "asum_c", "Sum of corrected ADC amplitudes",   "fASUM_c" },
    { "trx",    "x-position of track in det plane",  "fTrackProj.THaTrackProj.fX" },
    { "try",    "y-position of track in det plane",  "fTrackProj.THaTrackProj.fY" },
    { "trpath", "TRCS pathlen of track to det plane","fTrackProj.THaTrackProj.fPathl" },
    { "noverflow",  "overflow bit of FADC pulse",    "foverflow" },
    { "nunderflow",  "underflow bit of FADC pulse",  "funderflow" },
    { "nbadped",  "pedestal quality bit of FADC pulse",   "fpedq" },
    { "nhits",  "Number of hits for each PMT",       "fNhits" },
    { 0 }
  };
  return DefineVarsFromList( vars, mode );
}

//_____________________________________________________________________________
SciFi::~SciFi()
{
  // Destructor. Remove variables from global list.

  if( fIsSetup )
    RemoveVariables();
  if( fIsInit )
    DeleteArrays();
  fFADC=NULL;
}

//_____________________________________________________________________________
void SciFi::DeleteArrays()
{
  // Delete member arrays. Internal function used by destructor.

  delete [] fA_c;    fA_c    = NULL;
  delete [] fA_p;    fA_p    = NULL;
  delete [] fA;      fA      = NULL;
  // delete [] fT_c;    fT_c    = NULL;
  // delete [] fT;      fT      = NULL;

  delete [] fGain;   fGain   = NULL;
  delete [] fPed;    fPed    = NULL;
  // delete [] fOff;    fOff    = NULL;

  delete [] fPeak;      fPeak      = NULL;
  delete [] fT_FADC;    fT_FADC    = NULL;
  delete [] fT_FADC_c;  fT_FADC_c  = NULL;
  delete [] foverflow;  foverflow  = NULL;
  delete [] funderflow; funderflow = NULL;
  delete [] fpedq;      fpedq      = NULL;
  delete [] fNhits;     fNhits     = NULL;
}

//_____________________________________________________________________________
void SciFi::Clear( Option_t* opt )
{
  // Clear event data
  THaNonTrackingDetector::Clear(opt);
  // fNThit = fNAhit = 0;
  assert(fIsInit);
  for( Int_t i=0; i<fNelem; ++i ) {
    // fT[i] = fT_c[i] = 0.0;
    fA[i] = fA_p[i] = fA_c[i] = 0.0;
    fPeak[i]=0.0;
    fT_FADC[i]=0.0;
    fT_FADC_c[i]=0.0;
  }
  fASUM_p = fASUM_c = 0.0;

  if( !strchr(opt,'I') ) {
    memset( foverflow, 0, fNelem*sizeof(foverflow[0]) );
    memset( funderflow, 0, fNelem*sizeof(funderflow[0]) );
    memset( fpedq, 0, fNelem*sizeof(fpedq[0]) );
    memset( fNhits, 0, fNelem*sizeof(fNhits[0]) );
  }
}

//_____________________________________________________________________________
Int_t SciFi::Decode( const THaEvData& evdata )
{
  // Decode Cherenkov data, correct TDC times and ADC amplitudes, and copy
  // the data into the local data members.
  // This implementation assumes that the first half of the detector map
  // entries corresponds to ADCs, and the second half, to TDCs.

  // this next loop should go through 4 entries (4 fadc modules with 16 channels apeice)
  for( Int_t i = 0; i < fDetMap->GetSize(); i++ ) {

    cout << " detmap size is " << fDetMap->GetSize() << endl;
    THaDetMap::Module* d = fDetMap->GetModule( i );
    bool adc = fDetMap->IsADC(d);


    const char* const here = "Decode";
    
    // if (!adc){
    //  Error( Here(here), "Module %d in database is not of type fadc250 "
    // 	   "Fix database", i);
    //  Int_t err = kDecodeErr; // defined somwhere in ThaAnalyzer
    // }

    
    // change: removed if condition for fadc (should be only thing present)
    fFADC = dynamic_cast <Fadc250Module*> (evdata.GetModule(d->crate, d->slot));

    // Loop over all channels that have a hit.
    for( Int_t j = 0; j < evdata.GetNumChan( d->crate, d->slot ); j++) {

      Int_t chan = evdata.GetNextChan( d->crate, d->slot, j );
      if( chan < d->lo || chan > d->hi ) continue;     // Not one of my channels

      // Get the detector channel number, starting at 0
      // removed -1 at end, not sure of purpose
      Int_t k = d->first + ((d->reverse) ? d->hi - chan : chan - d->lo);

#ifdef WITH_DEBUG
      if( k<0 || k>= fNelem ) {
        Warning( Here("Decode()"), "Illegal detector channel: %d, chan = %d, hit no = %d,  i = %d", k, chan, j, i );
        continue;
      }
#endif

      // Get the data. fADCs are assumed to have only single hit (hit=0)
      Int_t data;
      Int_t ftime=0;
      Int_t fpeak=0;
      Float_t tempPed = fPed[k];             // Dont overwrite DB pedestal value!!! -- REM -- 2018-08-21
      // if(adc){
      data = evdata.GetData(kPulseIntegral,d->crate,d->slot,chan,0);
      ftime = evdata.GetData(kPulseTime,d->crate,d->slot,chan,0);
      fpeak = evdata.GetData(kPulsePeak,d->crate,d->slot,chan,0);
      // }
      // else{ 
      // 	fNhits[k]=evdata.GetNumHits(d->crate, d->slot, chan);     
      // 	data = evdata.GetData( d->crate, d->slot, chan, fNhits[k]-1 );
      // }

      // if(adc){
      if(fFADC!=NULL){
	foverflow[k] = fFADC->GetOverflowBit(chan,0);
	funderflow[k] = fFADC->GetUnderflowBit(chan,0);
	fpedq[k] = fFADC->GetPedestalQuality(chan,0);
	//       if(foverflow[k]+funderflow[k]+fpedq[k] != 0) printf("Bad Quality: (over, under, ped)= (%i,%i,%i)\n",foverflow[k],funderflow[k],fpedq[k]);
      }
      if(fpedq[k]==0)
	{
	  if(fTFlag == 1)
	    {
	      tempPed=(fNSA+fNSB)*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;
	    }
	  else
	    {
	      tempPed=fWin*(static_cast<Double_t>(evdata.GetData(kPulsePedestal,d->crate,d->slot,chan,0)))/fNPED;
	    }
	}
	//       else
   //       {
   //         printf("\nWARNING: BAD FADC PEDESTAL\n");
   //       }
      // }
      
      // Copy the data to the local variables.
      // if ( adc ) {
      fA[k]   = data;
      fPeak[k] = static_cast<Float_t>(fpeak);
      fT_FADC[k]=static_cast<Float_t>(ftime);
      fT_FADC_c[k]=fT_FADC[k]*0.0625;
      fA_p[k] = data - tempPed;
      fA_c[k] = fA_p[k] * fGain[k];
      // only add channels with signals to the sums
      if( fA_p[k] > 0.0 )
	fASUM_p += fA_p[k];
      if( fA_c[k] > 0.0 )
	fASUM_c += fA_c[k];
      fNAhit++;
      // } else {
      // 	// fT[k]   = data;
      // 	// fT_c[k] = data - fOff[k];
      // 	fNThit++;
      // }
    }
  }

  if ( fDebug > 3 ) {
    printf("\nSciFi %s:\n",GetPrefix());
    int ncol=2;
    for (int i=0; i<ncol; i++) {
      printf("  ADC  ADC_p  ");
    }
    printf("\n");
 
    for (int i=0; i<(fNelem+ncol-1)/ncol; i++ ) {
      for (int c=0; c<ncol; c++) {
	int ind = c*fNelem/ncol+i;
	if (ind < fNelem) {
	  printf("  %3d  %5.0f  %5.0f  ",ind+1,fA[ind],fA_p[ind]);
	} else {
	  //	  printf("\n");
	  break;
	}
      }
      printf("\n");
    }
  }

  // return fNThit;
  return fNAhit++;
}

//_____________________________________________________________________________
Int_t SciFi::CoarseProcess( TClonesArray& tracks )
{
  // Reconstruct coordinates of where a particle track crosses
  // the Cherenkov plane, and copy the point into the fTrackProj array.

  CalcTrackProj( tracks );

  return 0;
}

//_____________________________________________________________________________
Int_t SciFi::FineProcess( TClonesArray& tracks )
{
  // Fine Cherenkov processing

  // Redo the track matching since tracks might have been thrown out
  // during the FineTracking stage.
  fTrackProj->Clear();
  CalcTrackProj( tracks );

  return 0;
}

//_____________________________________________________________________________
ClassImp(SciFi)
