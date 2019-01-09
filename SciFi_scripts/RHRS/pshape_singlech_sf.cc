/*
  pshape_singlech_sf.cc

  John Williamson, October 14th, 2018

*/

#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TF1.h>
#include <TH2.h>
#include <TCanvas.h>
//#include <iostream>


// using namesace std;

/*  const Int_t nch = 64;
  //const Int_t n   = 74;
  const Int_t n   = 24;
  //const Int_t n   = 400;
  */

void pshape_singlech_sf(Int_t runnum=-1, Int_t chan = 0, Int_t min_int = 11000){


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
	  
	std::cout << "Pulse shape check: " << "\n" << "Single channel: Displays event-by-event the pulse shapes (time (ns) vs amplitude). Only shows events with integral above threshhold (defined by minimum_integral)." << "To cycle through event click on canvas and press enter, to quite macro close canvas and ctrl-c." << std::endl;
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

  
       
  std::cout << " Min int = " << min_int << std::endl;

  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  Int_t evID;
  Int_t nn[n]; // size of pules
  //  Double_t time[n];
  Double_t ph[n]; // pulse height
  Double_t raw_sum[nch];
     
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  char inputfilename[500] = {0};
  sprintf(inputfilename,"/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("T");

  Double_t ent = t1->GetEntries();

  char name[500];

  Int_t xbin = n;
  Double_t xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  
  
  if( chan <10){
    sprintf(name,"Ndata.R.sf.a_raw0%d",chan); // remove .2
  }
  else{
    sprintf(name,"Ndata.R.sf.a_raw%d",chan); // remove .2
  }
  t1->SetBranchAddress(name,&nn);   
  
  if( chan< 10){
    sprintf(name,"R.sf.a_raw0%d",chan);
  }
  else{
    sprintf(name,"R.sf.a_raw%d",chan);
  }
  
  t1->SetBranchAddress(name,&ph);


  t1->SetBranchAddress("R.sf.a_raw_sum",&raw_sum);

  //  TH1F* h = new TH1F("h","",xbin,xmin,xmax);
  //  TH1F* haccum;
  //TH1F* htmp;
  //  TF1* func1;

  char htitle[500]; 
  char hname[500];

  sprintf(htitle,"channel_number_%d",chan);
  sprintf(hname,"htemp_%d",chan);
  TH1F* htmp = new TH1F(hname,htitle,xbin,xmin,xmax);


  
  TCanvas* c1 = new TCanvas("c1","c1");
  //c1->Divide(6,6);
  
  //c2->Divide(6,6);
  char pname[500];
  Double_t ymax = 4000.;
  Double_t ymin = 0.;
  
  Double_t intc = -100; // integrated charge
  
  
  htmp->GetXaxis()->SetTitle("Time (ns)");
  htmp->GetYaxis()->SetTitle("Channel number");
  
  Double_t peak = -100.0;
  Int_t peak_t = -5;
  Double_t chan_cor = -100.0; // value in channel - offset
  
  //  Int_t event_1 = evNo;
  
  for(Int_t i=evNo ; i<ent ; i++){
    
    peak = -200;
    t1->GetEntry(i);
    
    for(Int_t j=0 ; j<n ; j++){
      
      chan_cor = ph[j]; // JW: usually has offset
      htmp->SetBinContent(j+1, chan_cor);
      
      if(chan_cor > peak){
	peak = chan_cor;
	peak_t = ((j) * 4)+2;	
      }
    }
    
    
    intc = htmp->Integral( 8, 15 );
    
    //    min_int = 15*(n/24.);

    //    min_int = 70;
    Int_t minimum_int  = 300;

    std::cout << " integral managed, intc = " << intc <<  ", min_int = " << min_int  << std::endl;
    
    
    c1->cd(0);
    htmp->Draw();
    
    htmp->SetMinimum(ymin);
    htmp->SetMaximum(ymax);
    
    minimum_int  = 11000;

    if ( intc > min_int){

      
      std::cout << "event # " << i << ", integral above ped :" << intc << " and raw sum = " << raw_sum[chan] << ", peak time (ns): " << peak_t << std::endl;
      
      c1->Update();
      c1->WaitPrimitive();
    }
    c1->Clear();
    //cin >> temp;
    
    
    
    sprintf(pname,"pshape_r%d-%d_00-31.png",run,i+1);
    //c1->Print(pname);
    sprintf(pname,"pshape_r%d-%d_32-63.png",run,i+1);
    //c2->Print(pname);
    
  }

}			     
			     


			     
 
 
			     
