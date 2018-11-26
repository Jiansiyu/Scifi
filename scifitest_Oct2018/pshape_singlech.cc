/*
  pshape_singlech.cc

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
//#include <iostream>


// using namesace std;

/*  const Int_t nch = 64;
  //const Int_t n   = 74;
  const Int_t n   = 24;
  //const Int_t n   = 400;
  */

void pshape_singlech(Int_t runnum=-1, Int_t channum = 0, Int_t min_int = 200, Int_t evNo = 0 ){




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



  /*, Int_t evNo = 0*/
  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  //Int_t Int_te = 1;
  Int_t run = runnum;
  //Int_t ch  = channel;
  
  Int_t nch = 64;
  //const Int_t n   = 74;
  const Int_t n = 24;
  //const Int_t n   = 400;
  
       
  std::cout << " Min int = " << min_int << std::endl;

  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //
  Int_t John = n+nch;
  Int_t evID;
  Int_t nn;
  Double_t time[n];
  Double_t ph[nch][n];
     
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  char inputfilename[500] = {0};
  sprintf(inputfilename,"./rootfiles2/acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("tree");
  //  t1->SetBranchAddress("evID",  &evID);
  //t1->SetBranchAddress("nent",  &nn);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("ph",    &ph);
  Double_t ent = t1->GetEntries();

  //  ent = 30; // ------ The number of evnets analyzed ------


  Int_t xbin = n;
  //Double_t xmin = 0.0, xmax = 800.0;
  Double_t xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  TH1F* h = new TH1F("h","",xbin,xmin,xmax);
  TH1F* haccum;
  TH1F* htmp;
  TF1* func1;

  // ========================================== //
  // ======= Pedestal position evaluation ===== //
  // ========================================== //
  Int_t nite = 10; // the number of events accumulated
  
  char htitle[500]; 
  char hname[500];
  sprintf(hname,"haccum_%d",channum);
  sprintf(htitle,"channel_number_%d",channum);
  haccum = new TH1F(hname,htitle,xbin,xmin,xmax);
  sprintf(hname,"htemp_%d",channum);
  htmp = (TH1F*)haccum->Clone(hname);
  char fname[500];
  sprintf(fname,"func1_%d",channum);
  func1 = new TF1(fname,"[0]",0.0,xmax);
  func1->SetParameter(0, 200.0);
  

  std::cout << " about to get peds" << std::endl;
  //TCanvas* c0 = new TCanvas("c0","c0");
  //c0->Divide(8,4);
  //c0->cd();
  //htmp = (TH1F*)haccum[0]->Clone("htmp");
  std::cout << " Min int = " << min_int << std::endl;

  for(Int_t i=0 ; i<nite ; i++){
    t1->GetEntry(i);
    for(Int_t k=0 ; k<n ; k++){
      htmp->SetBinContent(k+1,ph[channum][k]);
      //htmp[j]->SetBinError(k+1,0.0);
    }
    haccum->Add(htmp);
  }

  // where mint_int is multiplied/ changed
  std::cout << " Min int = " << min_int << std::endl;
  
  min_int = 20;

  haccum->Scale(1./nite);
  //sprintf(hname,"haccum_%d",i);
  haccum->Fit(fname,"Nq","",0.0,xmax);
  //c0->cd(i+1);
  //haccum[i]->Draw();
  //func1[i]->Draw("same");
  std::cout << channum << ": " << func1->GetParameter(0) << " "
       << func1->GetParError(0) << std::endl;;
  

  
  
  TCanvas* c1 = new TCanvas("c1","c1");
  //c1->Divide(6,6);

  //c2->Divide(6,6);
  char pname[500];
  Double_t ymax = 100.;
  Double_t ymin = -20.;
  Double_t offset;// = func1[ch]->GetParameter(0);

  Double_t intc = -100; // integrated charge
  

  htmp->GetXaxis()->SetTitle("Time (ns)");
  htmp->GetYaxis()->SetTitle("Channel number (minus offest)");
  
  Double_t peak = -100.0;
  Int_t peak_t = -5;
  Double_t chan_cor = -100.0; // value in channel - offset

  //  Int_t event_1 = evNo;

  
  
  for(Int_t i=evNo ; i<ent ; i++){

    peak = -200;
    t1->GetEntry(i);
    offset = func1->GetParameter(0);
    for(Int_t j=0 ; j<n ; j++){
      //h->SetBinContent(j+1,ph[ch][j]);
      //h->SetBinContent(j+1, ph[ch][j]-offset);
      // chan_cor = ph[channum][j]; // JW: usually has offset
      chan_cor = ph[channum][j]-offset; // JW: usually has offset
      htmp->SetBinContent(j+1, chan_cor);
      //      std::cout << " channel  " << j << " = " << ph[channum][j] <<std::endl;
      
      if(chan_cor > peak){
	peak = chan_cor;
	peak_t = ((j) * 4)+2;
      }

    }

    std::cout << " fitted peds" << std::endl;
    


    intc = htmp->Integral( 0, n );
    
    //    min_int = 15*(n/24.);

    //    min_int = 70;
    Int_t minimum_int  = 300;

    std::cout << " integral managed, intc = " << intc <<  ", min_int = " << min_int  << std::endl;


    c1->cd(0);
    htmp->Draw();
  
    htmp->SetMinimum(ymin);
    htmp->SetMaximum(ymax);
  
    //h->SetMinimum(250.);
    //TLine l(0.0,ymin,xmax,ymax);
    //l.Draw("same");
    //std::cout << ch << ": "  << i+1<< std::endl;
    //h->Draw();

    minimum_int  = 300;

    if ( intc > minimum_int){


      std::cout << "event # " << i << ", integral above ped :" << intc << ", peak time (ns): " << peak_t << std::endl;

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
