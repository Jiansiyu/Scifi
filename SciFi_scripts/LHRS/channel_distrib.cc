/*
  channel_distrib.cc

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

/*  const Int_t nch = 64;
  //const Int_t n   = 74;
  const Int_t n   = 24;
  //const Int_t n   = 400;
  */

void channel_distrib(Int_t runnum=-1){


  if ( runnum == -1){

    char answer[100]  = "filler";

    std::cout << ".x channel_distrib.cc(#run)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "Plots integrals of pulses in all channels (over the range 32-64 ns, and prints the results in Rsf_Ped_(32-64_ns)_distrib.pdf" << "\n" << "Also plots distribution of pulse height (Fadc channel in each 4ns window) with same cut around 32-64 ns and prints this in Rsf_chan_distrib.pdf" << std::endl;
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
      sprintf(name,"Ndata.L.sf.a_raw0%d",chan); // remove .2
    }
    else{
      sprintf(name,"Ndata.L.sf.a_raw%d",chan); // remove .2
    }
    t1->SetBranchAddress(name,&nn);   
    
    if( chan < 10){
      sprintf(name,"L.sf.a_raw0%d",chan);
      cout << " L.sf.a_raw0" << chan << endl;

    }
    else{
      sprintf(name,"L.sf.a_raw%d",chan);
    }
    
    t1->SetBranchAddress(name,&ph[chan]);


    //    t1->SetBranchAddress("L.sf.a_raw_sum",&raw_sum[nch]);
    
  }


  
  TCanvas* cchan[6];
  char ctitle[500];
  //  char cname[500];

  TCanvas* cintegrals[6];


  for(Int_t j = 0; j<6; j++){
    sprintf(ctitle,"cchans_%d",j);
    cchan[j] = new TCanvas(ctitle,ctitle,1200,1200);
    cchan[j]->Divide(4,3); 

    gPad->SetLogy();

    sprintf(ctitle,"cintegrals_%d",j);
    cintegrals[j] = new TCanvas(ctitle,ctitle,1200,1200);
    cintegrals[j]->Divide(4,3); 

    // cped[j].SetTitle(ctitle);
  }
 
  TCanvas* c_all_chan = new TCanvas("All channel peak times","All channel peak times",1200,1200);
  gPad->SetLogy();

  
  Double_t peak = -100.0;
  //  Int_t peak_t[nch][ent];
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
  
  TH1F* integrals[nch];

  for(Int_t z=0; z<nch; z++){

    tt1[z] = new TH1F(Form("tt1_%d",z),Form("Fadc channel distributions %d",z),400,0,4000);
    tt1[z]->GetXaxis()->SetTitle("Channel number" );

    integrals[z] =  new TH1F(Form("Integrals_%d",z),Form("Integral (32-64 ns) channel %d",z),5000,1500,7000);
    integrals[z]->GetXaxis()->SetTitle("Integral" );

  }
  
  TH1F* all_chan_hist = new TH1F("All fibres fadc channel numbers","All fibres fadc channel numbers",30,0,140);
  all_chan_hist->GetXaxis()->SetTitle("amp. in 4 ns bin");

  
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

  for(Int_t k=0 ; k<nch ; k++){
  
      c_select = Int_t( floor((k / 12.0)));
      c_remainder = Int_t( (k%12) +1);
      
      cchan[c_select]->cd(c_remainder);
      gPad->SetLogy();

      cout << " c_select : c_remain = " << c_select << " : " << c_remainder << endl;
      
      tt1[k]->Draw();


      cintegrals[c_select]->cd(c_remainder);
      
      gPad->SetLogy();
      integrals[k]->Draw();
      


  }

  c_all_chan->cd(0);
  all_chan_hist->Draw();

  
  char pdfname [50];
  

  sprintf(pdfname,"plots/pedestals/Rsf_chan_distrib_%d.pdf(",runnum);
  cchan[0]->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/pedestals/Rsf_chan_distrib_%d.pdf",runnum);
  cchan[1]->Print(pdfname,"pdf");
  cchan[2]->Print(pdfname,"pdf");
  cchan[3]->Print(pdfname,"pdf");
  cchan[4]->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/pedestals/Rsf_chan_distrib_%d.pdf)",runnum);
  cchan[5]->Print(pdfname,"pdf");
  //  c_all_chan->Print("Rsf_chan_distrib.pdf)","pdf");



  sprintf(pdfname,"plots/pedestals/Rsf_Ped_(32-64_ns)_distrib_%d.pdf(",runnum);
  cintegrals[0]->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/pedestals/Rsf_Ped_(32-64_ns)_distrib_%d.pdf",runnum);
  cintegrals[1]->Print(pdfname,"pdf");
  cintegrals[2]->Print(pdfname,"pdf");
  cintegrals[3]->Print(pdfname,"pdf");
  cintegrals[4]->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/pedestals/Rsf_Ped_(32-64_ns)_distrib_%d.pdf)",runnum);
  cintegrals[5]->Print(pdfname,"pdf");
  


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




			     
 
 
			     
