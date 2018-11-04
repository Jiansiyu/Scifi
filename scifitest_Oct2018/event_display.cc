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

void event_display(Int_t runnum=-1, int ref_run = 5049){



  if ( runnum == -1){

    char answer[100]  = "filler";

    std::cout << ".x event_display.cc(#run)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "\n" <<  "Event display: " << "\n" << "Goes through each event and displays 2D histogram showing which channels 'fired', this is defined as the pulse in said channel being apreciably above its pedestal." << std::endl;
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
  
  const Int_t nch = 64;
  //const Int_t n   = 74;
  const Int_t n = 24;
  //const Int_t n   = 400;
  
       
  // ==================================== //
  // ==== Define channel variables ====== //
  // ==================================== //

  Int_t evID;
  Int_t nn[nch];
  Double_t time[nch][n];
  Double_t ph[nch][n];
  Double_t int_ch[nch];
  int nscifi[nch];  
   
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  char inputfilename[500] = {0};
  sprintf(inputfilename,"./rootfiles2/acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("tree");
  t1->SetBranchAddress("evID",  &evID);
  //t1->SetBranchAddress("nent",  &nn);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("ph",    &ph);
  t1->SetBranchAddress("nscifi" ,   &nscifi );
  
  Double_t ent = t1->GetEntries();

  //===========================//
  //==== Set-up histograms=====//
  //===========================//

  double xmin = 0.0, xmax = (double)n*4.0;

  double xbin = nch;

  TH1F* h = new TH1F("h","",
		     xbin,xmin,xmax);
  h->GetXaxis()->SetTitle("(ns)");
  TH1F* htmp[nch];        // histogram for each pulse 
  TH1F* haccum[nch];      // histogram   
  TH1F* hintegrals[nch];
  


  for(int i=0 ; i<nch ; i++){
   
    char hname[500];
    sprintf(hname,"haccum_%d",i);
    haccum[i] = new TH1F(hname,"",xbin,xmin,xmax);
    sprintf(hname,"htemp_%d",i);
    htmp[i] = (TH1F*)haccum[i]->Clone(hname);
  
    
  }

  //===============================//
  //==== read in pedestal data ====//
  //===============================//


  Double_t holder = -200;
  Double_t iped[nch]; // this will take in the ped values for each channel
  Double_t iRMS[nch];


  char pedfname[500];
  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",run);
  ifstream* ifs = new ifstream(pedfname);

   for(int j=0 ; j<nch ; j++){
     
     iped[j] = 0;

     *ifs >> holder >> iped[j] >> iRMS[j];

     

   }

  //=================================//
  //==== create empty 2D display ====//
  //=================================//


  TH2F* hemp = new TH2F("hemp","",100,-80.0,80.0,100,-80.0,80.);
  
  TCanvas* c1 = new TCanvas("c1","c1",700,700);
  const int nfiber = 64;
  hemp->Draw();
  TBox* b[nfiber];
  double phi = 1.0; // phi = 1.0 mm
  double step = 3.79;
  double center = step * (32.-1.) / 2.0; // 121.28/2 mm
  double length = center*2.0; // mm
  for(int i=0 ; i<nfiber ; i++){
    if(i<32){
      b[i] = new TBox(-center-phi/2.0 + step*i,
		      -length/2.0,
		      -center+phi/2.0 + step*i,
		      length/2.0);
      b[i]->SetFillStyle(1001);
      b[i]->SetFillColor(1);
      b[i]->Draw("same");
    }
    else if(i<64){
      b[i] = new TBox(-length/2.0,
		      -center-phi/2.0 + step*(i-32),
		      length/2.0,
		      -center+phi/2.0 + step*(i-32));
      b[i]->SetFillStyle(1001);
      b[i]->SetFillColor(1);
      b[i]->Draw("same");
    }
  }


   //===================================================//
   //==== compare to event  integrals to peds ==========//
   //=========== and start 2D display ==================//    
   //===================================================//

  //  double npeth = 2.0;
  bool hit=false;
  bool hitfront = false;
  bool hitrear  = false;
  int nmulti=0;
  const int maxmulti = 50;
  int hitfibernum[maxmulti];
  

  
  for(int i=0 ; i<ent  ; i++){

    // intialize variables for each event
    for(int j=0; j<nch ; j++){
      nscifi[j] = -2222.0;
    }


    hit      = false;
    hitfront = false;
    hitrear  = false;
    nmulti = 0;

    t1->GetEntry(i);
 
    for(int j=0 ; j<nch ; j++){

      htmp[j]->Reset("ICESM");
      //      nscifi[j] = -2222.0;



      int_ch[j] = 0;

      for(int k=0 ; k<n ; k++){
	htmp[j]->SetBinContent(k+1,ph[j][k]);
	//	if(ph[j][k]>300.0){
	//ok[j] = false;
	//}       option to have cut on large signals (to not put in pedestal
	//htmp[j]->SetBinError(k+1,0.0);
      }


      int_ch[j] = htmp[j]->Integral(xmin,xmax);


      if( int_ch[j] > (iped[i] + 500) && nmulti<maxmulti){

	hit=true;
	//int nfi = nscifi[j];
	b[nscifi[j]]->SetFillColor(6);
	b[nscifi[j]]->SetLineColor(6);
	b[nscifi[j]]->SetLineWidth(2);
	if(nscifi[j]<32) hitfront = true;
	else if(nscifi[j]<64) hitrear = true;
	cout << " #ev=" << i << ", #ch=" << j  << ". Integral =  " << int_ch[j] << ", #SciFi=" << nscifi[j] << endl;
	hitfibernum[nmulti] = nscifi[j];
	nmulti++;
	b[nscifi[j]]->Draw();
	//c1->Update();
      }
    }
    

    if(hit==true){
      c1->Update();
      if(hitrear==true && hitfront==true){
	cout << " --> " << nmulti << " hit(s) + criss cross hit!!" << endl;
	//c1->WaitPrimitive();
      }
      else{
	cout << " --> " << nmulti << " hit(s)." << endl;
      }
      cout << endl;
      c1->WaitPrimitive();
      for(int j=0 ; j<nmulti ; j++){
	b[hitfibernum[j]]->SetFillColor(1);
	b[hitfibernum[j]]->SetLineColor(1);
	b[hitfibernum[j]]->SetLineWidth(1);
	b[hitfibernum[j]]->Draw();
      }
      c1->Update();




    }
  }

}
  







                                 

