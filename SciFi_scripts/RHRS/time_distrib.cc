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


void time_distrib(Int_t runnum=-1){


  if ( runnum == -1){

    char answer[100]  = "filler";

    std::cout << ".x time_distrib.cc(#run)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << " " << std::endl;
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

  //  Double_t time[n];
  Double_t ph[nch][n]; // pulse height
  //  Double_t raw_sum[nch];
     
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  char inputfilename[500] = {0};
  sprintf(inputfilename,"/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_%d.root",run);
  
  cout << " root file opened" << endl;

  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("T");

  Int_t ent = t1->GetEntries();

  char name[500];

  Int_t xbin = n;
  Double_t xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  
  
  for (Int_t chan = 0; chan<nch; chan++){
    
    
    if( chan< 10){
      sprintf(name,"R.sf.a_raw0%d",chan);
    }
    else{
      sprintf(name,"R.sf.a_raw%d",chan);
    }
    
    t1->SetBranchAddress(name,&ph[chan]);
    
  }


  cout << " root branches set" << endl;

  
  TCanvas* ctime[6];
  char ctitle[500];
  //  char cname[500];


  for(Int_t j = 0; j<6; j++){
    sprintf(ctitle,"ctimes_%d",j);
    ctime[j] = new TCanvas(ctitle,ctitle,1200,1200);
    ctime[j]->Divide(4,3); 

    gPad->SetLogy();
    // cped[j].SetTitle(ctitle);
  }
 

  cout << "canvases  set" << endl;

  TCanvas* c_all_time = new TCanvas("All channel peak times","All channel peak times",1200,1200);
  gPad->SetLogy();


  // creat histograms for all channels

  TH1F* tt1[nch];

  for(Int_t z=0; z<nch; z++){

    tt1[z] = new TH1F(Form("tt1_%d",z),Form("Time peak distrib %d",z),30,0,150);
    tt1[z]->GetXaxis()->SetTitle("Time (ns)");
  }

  TH1F* all_time_hist = new TH1F("All channel peak times","All channel peak times",30,0,140);
  all_time_hist->GetXaxis()->SetTitle("Time (ns)");


  
  Double_t peak = -100.0;
  Int_t temp = 0;
  

  bool threshold = true;
  
  for(Int_t i=0 ; i<ent ; i++){
    
    // peak = -200;
    t1->GetEntry(i);
    cout << "Event number: " << i << endl;
    
    for(Int_t k=0 ; k<nch ; k++){
      peak = -200;
      temp = -10;
      threshold = true;
      
      for(Int_t j=0 ; j<n ; j++){
			
	if(ph[k][j] > 500 && ph[k][j] > peak){
	  //	  threshold = false;
	  peak = ph[k][j];
	  temp = ((j) * 4)+2;	

	}


      }

      tt1[k]->Fill(temp);



    }
         
  }


  
  // creat histograms for all channels
  
  cout << " Ok until here" << endl;

  Int_t c_select = 0;
  Int_t c_remainder = 1;

  for(Int_t k=0 ; k<nch ; k++){

    c_select = Int_t( floor((k / 12.0)));
    c_remainder = Int_t( (k%12) +1);

    ctime[c_select]->cd(c_remainder);
    cout << " c_select : c_remain = " << c_select << " : " << c_remainder << endl;

    //    gPad->SetLogy();

    // for(Int_t i=0 ; i<ent ; i++){

    //   tt1[k]->Fill(peak_t[k][i]);
    //   all_time_hist->Fill(peak_t[k][i]);
    // }

    tt1[k]->Draw();

  }  

  c_all_time->cd(0);
  all_time_hist->Draw();

  char pdfname[50];
  
  sprintf(pdfname,"plots/time/Rsf_time_distrib_%d.pdf(",runnum);
  ctime[0]->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/time/Rsf_time_distrib_%d.pdf",runnum);
  ctime[1]->Print(pdfname,"pdf");
  ctime[2]->Print(pdfname,"pdf");
  ctime[3]->Print(pdfname,"pdf");
  ctime[4]->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/time/Rsf_time_distrib_%d.pdf)",runnum);
  ctime[5]->Print(pdfname,"pdf");


  c_all_time->Print("Rsf_time_distrib.pdf)","pdf");


  // calculate average peak and mode times for each file 


  // Double_t peak_time_av[nch];

  Int_t peak_time_mode[nch];
  // Double_t sum_times = 0;

  // place-holder for number of bin with largest signal
  Int_t Peak_bin = -1;

  for(Int_t k=0 ; k<nch ; k++){

  Int_t Peak_bin = -1;
  //   sum_times = 0;
  //   for(Int_t i=0 ; i<ent ; i++){
  //     sum_times += peak_t[k][i];
  //   }

  //   peak_time_av[k] = sum_times/Double_t(ent);

    
    peak_time_mode[k] = (tt1[k]->GetMaximumBin() * 4) + 2;
    
  }

  

  char timefname[500];
  sprintf(timefname,"./ped_integrals/time_peaks_%d.dat",runnum);


  ofstream* ofs = new ofstream(timefname);
    
    
  for(Int_t k=0; k<nch; k++){
    cout << k << "  " << peak_time_mode[k] << std::endl;
    
    *ofs  << k << " " << peak_time_mode[k]  << endl;;
  }
  

  ofs->close();
       

}




			     
 
 
			     
