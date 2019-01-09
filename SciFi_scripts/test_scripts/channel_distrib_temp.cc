/*
  time_distrib.cc

  John Williamson, October 14th, 2018

  based on:
  pshape.cc
  
  Toshiyuki Gogami, November 27, 2017
*/



#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TF1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>



using namespace std;

/*  const Int_t nch = 64;
  //const Int_t n   = 74;
  const Int_t n   = 24;
  //const Int_t n   = 400;
  */

void channel_distrib_temp(Int_t runnum=-1){


  if ( runnum == -1){

    char answer[100]  = "filler";

    std::cout << ".x pshape_singlech.cc(#run, #channum(def=0), #minimum_integral(def=200), first_event_number(def=0) " << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "Pulse shape check: " << "\n" << "Single channel: Displays event-by-event the pulse shapes (time (ns) vs amplitude). Only shows events with integral above threshhold (defined by minimum_integral)." << "\n" << "Events have been pedestal subtracted based on ped.cc pedestals (~200 per time bin)" << "\n" << "To cycle through event click on canvas and press enter, to quite macro close canvas and ctrl-c." << "\n" << "Final parameter, first_event_number, controls the event at which the macro starts" << std::endl;
	  return 1;
	}
	else{
	  std::cout << "Do you want description of macro? (y/n)" << std::endl;
	  std::cin >> answer;
	}
	}
  } 



  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);


  Int_t run = runnum;  
  Int_t nch = 64;

  const Int_t n = 35;

  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  Int_t evID;
  Int_t nn[n]; // size of pules
  //  Double_t time[n];
  Double_t ph[nch][n]; // pulse height
  Double_t raw_sum[nch];
     
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  char inputfilename[500] = {0};
  sprintf(inputfilename,"/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("T");
  
  Int_t ent = t1->GetEntries();
  cout << ent << endl;
  char name[500];

  Int_t xbin = n;
  Double_t xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  
  
  for (Int_t chan = 0; chan<nch; chan++){
    
    if( chan <10){
      sprintf(name,"Ndata.R.sf.a_raw0%d",chan); // remove .2
    }
    else{
      sprintf(name,"Ndata.R.sf.a_raw%d",chan); // remove .2
    }
    //  t1->SetBranchAddress(name,&nn);   
    
    if( chan< 10){
      sprintf(name,"R.sf.a_raw0%d",chan);
    }
    else{
      sprintf(name,"R.sf.a_raw%d",chan);
    }
    
    t1->SetBranchAddress(name,&ph[chan]);
    

    //  t1->SetBranchAddress("R.sf.a_raw_sum",&raw_sum[nch]);
    
  }
  cout << "Branch Adresses Set" << endl;

  
  TCanvas* cchan[6];
  char ctitle[500];
  //  char cname[500];


  for(Int_t j = 0; j<6; j++){
    sprintf(ctitle,"cchans_%d",j);
    cchan[j] = new TCanvas(ctitle,ctitle,1200,1200);
    cchan[j]->Divide(4,3); 

    gPad->SetLogy();
    // cped[j].SetTitle(ctitle);
  }
 
  TCanvas* c_all_chan = new TCanvas("All channel peak times","All channel peak times",1200,1200);
  gPad->SetLogy();

  cout << "after gpad " << endl;
  
  Double_t peak = -100.0;
  // Int_t peak_t[nch][ent];
  Int_t temp = 0;
  


  
  // for(Int_t i=0 ; i<ent ; i++){
    
  //   // peak = -200;
  //   t1->GetEntry(i);
    
  //   for(Int_t k=0 ; k<nch ; k++){
  //     peak = -200;
  //     temp = -10;
  //     for(Int_t j=0 ; j<n ; j++){
			
  // 	if(ph[k][j] > peak){
  // 	  peak = ph[k][j];
  // 	  temp = ((j) * 4)+2;	
  // 	}
  //     }
      
  //     peak_t[k][i] = temp;

  //     //     cout << " peak_t[k][i] = " <<  peak_t[k][i] << endl;

  //   }
         
  // }

  
  // creat histograms for all channels

  TH1F* tt1[nch];

  cout << " reched here " << endl;

  for(Int_t z=0; z<nch; z++){

    tt1[z] = new TH1F(Form("tt1_%d",z),Form("Fadc channel distributions %d",z),400,0,4000);
    tt1[z]->GetXaxis()->SetTitle("Channel numer" );
  }

  TH1F* all_chan_hist = new TH1F("All fibres fadc channel numbers","All fibres fadc channel numbers",30,0,140);
  all_chan_hist->GetXaxis()->SetTitle("Channel number");

  
  cout << " Ok until here" << endl;

  Int_t c_select = 0;
  Int_t c_remainder = 1;


  for(Int_t i=0 ; i<ent ; i++){
    t1->GetEntry(i);

    for(Int_t k=0 ; k<nch ; k++){

      //    gPad->SetLogy();
      
      for(Int_t j=0 ; j<n ; j++){
	
	tt1[k]->Fill(ph[k][j]);
	all_chan_hist->Fill(ph[k][j]);
      }
      
      
      
    }  
  }

  for(Int_t k=0 ; k<nch ; k++){
  
      c_select = Int_t( floor((k / 12.0)));
      c_remainder = Int_t( (k%12) +1);
      
      cchan[c_select]->cd(c_remainder);
      gPad->SetLogy();

      cout << " c_select : c_remain = " << c_select << " : " << c_remainder << endl;
      
      tt1[k]->Draw();
  }

  c_all_chan->cd(0);
  all_chan_hist->Draw();


  
  cchan[0]->Print("Rsf_chan_distrib.pdf(","pdf");
  cchan[1]->Print("Rsf_chan_distrib.pdf","pdf");
  cchan[2]->Print("Rsf_chan_distrib.pdf","pdf");
  cchan[3]->Print("Rsf_chan_distrib.pdf","pdf");
  cchan[4]->Print("Rsf_chan_distrib.pdf","pdf");
  cchan[5]->Print("Rsf_chan_distrib.pdf","pdf");
  c_all_chan->Print("Rsf_chan_distrib.pdf)","pdf");


  // calculate average peak and mode times for each file 


  // Double_t peak_time_av[nch];

  // Int_t peak_time_mode[nch];
  // Double_t sum_times = 0;

  // for(Int_t k=0 ; k<nch ; k++){
  //   sum_times = 0;
  //   for(Int_t i=0 ; i<ent ; i++){
  //     sum_times += peak_t[k][i];
  //   }

  //   peak_time_av[k] = sum_times/Double_t(ent);
  //   //    peak_time_mode[nch] = tt1[k]->GetMaximumBin();
    
  // }

  



  // char timefname[500];
  // sprintf(timefname,"./ped_integrals/time_peaks_%d.dat",runnum);


  // ofstream* ofs = new ofstream(timefname);
    
    
  // for(Int_t k=0; k<nch; k++){
  //   cout << k << "  " << peak_time_av[k] << std::endl;
    
  //   *ofs  << k << " " << peak_time_av[k]  << endl;;
  // }


  // ofs->close();
       

}




			     
 
 
			     
