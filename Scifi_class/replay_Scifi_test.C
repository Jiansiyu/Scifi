/*
  A replay sample from Marco. 
  This works for fADC data of HRS-AC test @ESB.
  (2017 November)
  
  Trying to apply this code to fADC data 
  of APEX SS test @EEL122 (T. Gogami, Movember 24, 2017). 
*/

#include "TSystem.h"
#include "TList.h"
#include "THaEvent.h"
#include "THaRun.h"
#include "THaAnalyzer.h"
#include "THaApparatus.h"

//#include "SBSGEMStand.h"
//#include "SBSBigBite.h"
// R__LOAD_LIBRARY(libsbs)
 R__LOAD_LIBRARY(Scifi_class/libSciFi)


#include "Scifi_class/SciFi.h"
#include "Scifi_class/THaHRS.h" // some kind of arm...

 void replay_Scifi_test(Int_t runnum = -1, Int_t lastEvent = 10000, Bool_t mflag = 0){


  if ( runnum == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x replay_Scifi_test( #runnum, #lastEvent(def=10000), #mflag(0)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << " Replay script for lab bench set-up in EEL 122 for testing of SciFi Detector.  #runnum is the choice of run number. #lastevent is the last event o be replayhed. #mflag is a flag controlling if the data is raw mode (0) or pulse mode (1)." << std::endl;
	  return 1;
	}
	else{
	  std::cout << "Do you want description of macro? (y/n)" << std::endl;
	  std::cin >> answer;
	}
	}
    }


  //  gSystem->Load("libsbs.so");
  gSystem->Load("Scifi_class/libSciFi.so");
  
  THaHRS *RHRS = new THaHRS("R","Right arm RHRS");

  // so RHRS doesn't try and define cherenkov, s1, s2 etc
  RHRS->AutoStandardDetectors(kFALSE);
  

  SciFi *sf = new SciFi("sf","SciFi detector");

  RHRS->AddDetector(sf);

  //
  //  Steering script for Hall A analyzer demo
  //

  // Set up the equipment to be analyzed.

  // add the two spectrometers with the "standard" configuration
  // (VDC planes, S1, and S2)
  // Collect information about a easily modified random set of channels
  // (see DB_DIR/*/db_D.dat)
  /*
     THaApparatus* DECDAT = new THaDecData("D","Misc. Decoder Data");
     gHaApps->Add( DECDAT );
     */


  // Set up the analyzer - we use the standard one,
  // but this could be an experiment-specific one as well.
  // The Analyzer controls the reading of the data, executes
  // tests/cuts, loops over Apparatus's and PhysicsModules,
  // and executes the output routines.
  THaAnalyzer* analyzer = new THaAnalyzer;

  gHaApps->Add(RHRS);

  // A simple event class to be output to the resulting tree.
  // Creating your own descendant of THaEvent is one way of
  // defining and controlling the output.
  THaEvent* event = new THaEvent;

  // Define the run(s) that we want to analyze.
  // We just set up one, but this could be many.
  //  THaRun* run = new THaRun( "prod12_4100V_TrigRate25_4.dat" );
  //THaRun* run = new THaRun(TString::Format("data/fadcACSIS_%d.dat.0",runnum) );
  //THaRun* run = new THaRun(TString::Format("data/FADC_%d.dat.0",runnum) );
  //THaRun* run = new THaRun(TString::Format("data/FADC_%d.dat",runnum) );


  THaRun* run = new THaRun(TString::Format("data/SciFi_%d.dat",runnum) );
  run->SetLastEvent(lastEvent);

  run->SetDataRequired(0);
  run->SetDate(TDatime());

  analyzer->SetVerbosity(0);



  if(mflag == 0){
    analyzer->SetOdefFile("output_SciFi_test_raw.def");
  }
  else if(mflag == 1){
    analyzer->SetOdefFile("output_SciFi_test_pulse.def");
  }
  else{
    std::cout << "Error: mflag not set to 0 (raw mode) or 1 (pulse mode)" << std::endl;
    
    std::cout << "Defaulting to raw mode" << std::endl;
    analyzer->SetOdefFile("output_SciFi_test_raw.def");
  }


  // Define the analysis parameters
  analyzer->SetEvent( event );

  analyzer->SetOutFile( TString::Format("rootfiles/bunk_fadcACSIS_%d.root",runnum));
  // File to record cuts accounting information
  analyzer->SetSummaryFile("SciFi_test.log"); // optional

  //analyzer->SetCompressionLevel(0); // turn off compression
  analyzer->Process(run);     // start the actual analysis
}
