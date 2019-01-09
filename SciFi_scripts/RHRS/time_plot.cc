/* time_plot.cc

   John Williamson December 29th, 2018

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

void time_plot(Int_t runnum=-1){

  if ( runnum == -1){

    char answer[100]  = "filler";

    std::cout << ".x time_plot.cc(#run)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "Plots, for x and y, the values of the peak times against fibre number (increasing in distance). " << std::endl;
	  return 1;
	}
	else{
	  std::cout << "Do you want description of macro? (y/n)" << std::endl;
	  std::cin >> answer;
	}
	}
  } 

  // set-up variables for reading in peak times for all channels

  const Int_t nch = 64;
  Double_t chan[nch];
  Double_t time[nch];

  // set-up x and y plots 

  TH1F* peak_hist_x = new TH1F("Peak_times_x","Peak_times(x)",nch/2,0,nch/2);

  peak_hist_x->GetXaxis()->SetTitle("Fibre number");
  peak_hist_x->GetYaxis()->SetTitle("Time (ns)");

  TH1F* peak_hist_y = new TH1F("Peak_times_y","Peak_times(y)",nch/2,0,nch/2);
    
  peak_hist_y->GetXaxis()->SetTitle("Fibre number");
  peak_hist_y->GetYaxis()->SetTitle("Time (ns)");


  // read in peak values from file
  char timefname[500];
  sprintf(timefname,"./ped_integrals/time_peaks_%d.dat",runnum);
  

  ifstream* ifs = new ifstream(timefname);


  for (Int_t j=0; j<nch; j++){

    *ifs >> chan[j] >> time[j];

      if ( j < 32){
	// fill x plot
	peak_hist_x->SetBinContent(j+1, time[j]);
      }

      else if( j >= 32){
	// fill y plot
	peak_hist_y->SetBinContent(j-32+1, time[j]);
      }
  }


  // display results on tcanvases

  TCanvas* c1 = new TCanvas("c1","c1");
  TCanvas* c2 = new TCanvas("c2","c2");

  c1->cd(0);
  peak_hist_x->Draw();

  c2->cd(0);
  peak_hist_y->Draw();


  // save results in pdf

  char pdfname[50];

  sprintf(pdfname,"plots/time/Rsf_time_plot_%d.pdf(",runnum);
  c1->Print(pdfname,"pdf");

  sprintf(pdfname,"plots/time/Rsf_time_plot_%d.pdf)",runnum);
  c2->Print(pdfname,"pdf");

}
    



