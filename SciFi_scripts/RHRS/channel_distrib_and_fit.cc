/*
  channel_distrib_and_fit.cc

  John Williamson, December 12th, 2018

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

void channel_distrib_and_fit(Int_t runnum=-1){



  if ( runnum == -1){

    char answer[100]  = "filler";

    std::cout << ".x channel_distrib_and_fit.cc(#run) " << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	std::cout << "Finds the pedestals of all 64 channels by plotting the integrals for events (over the range 32-64 ns) and then using a Gaussian to fit this distribution. A second Gaussian is used to obtain a more accurate fit and thus pedestal value" << "\n" << "The pedestal integrals are saved in ped_integrals/pedestal_(#run).dat"  << std::endl;
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

  char name[500];

  Int_t xbin = n;
  Double_t xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  
  
  for (Int_t chan = 0; chan<nch; chan++){
    
    if( chan < 10){
      sprintf(name,"Ndata.R.sf.a_raw0%d",chan); // remove .2
    }
    else{
      sprintf(name,"Ndata.R.sf.a_raw%d",chan); // remove .2
    }
    t1->SetBranchAddress(name,&nn);   
    
    if( chan < 10){
      sprintf(name,"R.sf.a_raw0%d",chan);
      cout << " R.sf.a_raw0" << chan << endl;

    }
    else{
      sprintf(name,"R.sf.a_raw%d",chan);
    }
    
    t1->SetBranchAddress(name,&ph[chan]);


    //    t1->SetBranchAddress("R.sf.a_raw_sum",&raw_sum[nch]);
    
  }


  // create canvases to plot pedestal and channel distribtuions

  
  TCanvas* cchan[6];
  char ctitle[500];
  //  char cname[500];

  TCanvas* cintegrals[6];


  for(Int_t j = 0; j<6; j++){
    sprintf(ctitle,"cchans_%d",j);
    cchan[j] = new TCanvas(ctitle,ctitle,1200,1200);
    cchan[j]->Divide(4,3); 

    //    gPad->SetLogy();

    sprintf(ctitle,"cintegrals_%d",j);
    cintegrals[j] = new TCanvas(ctitle,ctitle,1200,1200);
    cintegrals[j]->Divide(4,3); 

    // cped[j].SetTitle(ctitle);
  }
 


  TH1F* tt1[nch];
  
  TH1F* integrals[nch];

  for(Int_t z=0; z<nch; z++){

    tt1[z] = new TH1F(Form("tt1_%d",z),Form("Fadc channel distributions %d",z),400,0,4000);
    tt1[z]->GetXaxis()->SetTitle("Channel number" );

    integrals[z] =  new TH1F(Form("Integrals_%d",z),Form("Integral (32-64 ns) channel %d",z),5000,1500,3000);
    integrals[z]->GetXaxis()->SetTitle("Integral" );

  }
  
  TH1F* all_chan_hist = new TH1F("All fibres fadc channel numbers","All fibres fadc channel numbers",30,0,140);
  all_chan_hist->GetXaxis()->SetTitle("Channel number");

  
  cout << " Ok until here" << endl;

  Int_t c_select = 0;
  Int_t c_remainder = 1;

  Double_t hist_int = 0;

  Double_t total = 0;

  for(Int_t i = 0 ; i<ent ; i++){
    t1->GetEntry(i);
    total = 0;

    for(Int_t k=0 ; k<nch ; k++){
      
      total = 0;
      //    gPad->SetLogy();
      
      for(Int_t z = 8 ; z<15 ; z++){
	
	tt1[k]->Fill(ph[k][z]);
	all_chan_hist->Fill(ph[k][z]);

	total += ph[k][z];
      }


      //      cout << "total = " << total << endl;

      integrals[k]->Fill(total);
      
      if( total > 50000){
	cout << "channel = " << k << ", event = " << i << ", with total " << total << endl;
      }
      

    }  
  }


  // set-up function to fit pedestals with

  char fname[500];

  TF1* func1[nch];  // functions for fitting overall peds
  TF1* func2[nch];

  
  TF1* func1_c[nch]; // functions for fitting ditribution of channels (should be 300)
  TF1* func2_c[nch];

  Double_t min, max, temp;
  Double_t ped_val,ped_wid;

  Int_t max_bin;
  Int_t i, ii;
  Double_t ped[nch],ped_width[nch];
  Double_t gain[nch];


  for(Int_t k=0 ; k<nch ; k++){
  
      c_select = Int_t( floor((k / 12.0)));
      c_remainder = Int_t( (k%12) +1);
      
      cchan[c_select]->cd(c_remainder);
      //      gPad->SetLogy();

      cout << " c_select : c_remain = " << c_select << " : " << c_remainder << endl;
      
      tt1[k]->Draw();


      cintegrals[c_select]->cd(c_remainder);
      
      gPad->SetLogy();
      integrals[k]->Draw();      

      max_bin = integrals[k]->GetMaximumBin();


      min = integrals[k]->GetBinCenter(max_bin) - 50;
      max = integrals[k]->GetBinCenter(max_bin) + 50;
    

      sprintf(fname,"func1_%d",k);
      func1[k] = new TF1(fname,"gaus");

      integrals[k]->Fit(fname,"NQ","",min,max);
      
      
      sprintf(fname,"func2_%d",k);
      func2[k] = new TF1(fname,"gaus");

      
      integrals[k]->Fit(fname,"Q","",min,max);
      
      //    delete tt1;
      ped_val = func2[k]->GetParameter(1);
      ped_wid = func2[k]->GetParameter(2);    
      
      std::cout << "---------------- " <<  ped_val  << "--------------" << std::endl;
      
      ped[k] = ped_val;
      ped_width[k] = ped_wid; 
  }

  
  // cintegrals[0]->Print("Rsf_plots_with_PE.pdf(","pdf");
  // cintegrals[1]->Print("Rsf_plots_with_PE.pdf","pdf");
  // cintegrals[2]->Print("Rsf_plots_with_PE.pdf","pdf");
  // cintegrals[3]->Print("Rsf_plots_with_PE.pdf","pdf");
  // cintegrals[4]->Print("Rsf_plots_with_PE.pdf","pdf");
  // cintegrals[5]->Print("Rsf_plots_with_PE.pdf)","pdf");
  
  
  char pedfname[500];
  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",runnum);
  //  sprintf(pedfname,"./ped/pedestal_%d_testtesttest.dat",run);
  ofstream* ofs = new ofstream(pedfname);
  
    
  for(Int_t k=0; k<nch; k++){
    std::cout << k << "  " << ped[k] << " " <<  ped_width[k] << std::endl;
    
    *ofs  << k << " " << ped[k] << " " <<  ped_width[k]  << endl;;
  }
  

  ofs->close();


}




			     
 
 
			     
