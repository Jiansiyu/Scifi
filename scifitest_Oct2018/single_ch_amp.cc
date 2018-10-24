/*
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
void  single_ch_amp(Int_t runnum=-1, Int_t channum = 0, Double_t ped_cut = 6500, Double_t tcut_low = 0, Double_t tcut_high = 100, Bool_t tcut_outside = true){



  if ( runnum == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x single_ch_amp.cc(#run, #channum(def=0), #ped_cut(def=6500.) , #tcut_low(def=0.), #tcut_high(def=100.), #tcut_outside(def=true)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "Script for a specific run (#run) and channel (#channel) that produces histograms of the integrals of event signals, and a peak time distribution"<<  "\n" << "\n" << "1: For histograms of integrals: " << "\n" << "\n" << "1.1: Get raw integrals (no pedestals) for every event and store in histogram. Also apply time cut to signals (tcut_low < t < tcut_high if tcut_outside = true, and t < tcut_low || t > tcut_high if tcut_outside = false) and store resulting histograms in seperate histogram" << "\n" << "1.2: Make crude Gaussian over whole range of uncut histogram " << "\n" << "1.3: Use first Gaus fit parameters (mean and RMS) to produce better fit. The mean of this fit is 'the pedestal' of the channel, and the RMS its width" << "\n" << "\n" << "c1: Plot of raw amplitude integrals:" << "\n" << "    first panel: raw amplitude" << "\n" <<  "    second panel: raw amplitude with first/poor fit" << "\n" <<  "    third panel: raw amplitude with second/good fit" << "\n" << "c2: Plot of better fit" << "\n" << "c3: Plot of raw amplitude integrals with time cuts" << "\n" << "\n" << "2: For time peak histograms" << "\n" << "\n" <<"2.1: Whilst doing step 1.1 also add the peak time for each event to a histogram, and if integral of signal is above threshold (#ped_cut) add to second histogram" << "\n" << "\n" << "c4: Plot of peak times for events:" << "\n" << "    first panel: all events" << "\n" << "    second panel: events with integral above #ped_cut" << std::endl;
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
  Int_t n = 24;
  //const Int_t n   = 400;
  
       
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //

  Int_t evID;
  Int_t nn;
  Double_t time[nch];
  Double_t intc[nch];
  Double_t ph[nch][n];
     
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
  t1->SetBranchAddress("intc",  &intc);
  Double_t ent = t1->GetEntries();
  //  ent = 30; // ------ The number of evnets analyzed ------


  Int_t xbin = n;
  //Double_t xmin = 0.0, xmax = 800.0;
  Double_t xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  TH1F* h = new TH1F("h","",xbin,xmin,xmax);
  TH1F* haccum;
  TH1F* htmp;
  TH1F* htmp_tc;
  TH1F* hscale;
  TH1F* integrals;
  TH1F* htime;
  TH1F* htime_c;
  TH1F* integrals_tc; // histo of integrals with time cuts

  //  TH1F* pedestals;
  //  TF1* func1;

  // ========================================== //
  // ======= Pedestal position evaluation ===== //
  // ========================================== //
  //  Int_t nite = 100; // the number of events accumulated
  
  char htitle[500]; 
  char hname[500];
  sprintf(hname,"haccum_%d",channum);
  sprintf(htitle,"Raw signals (channel_number_%d)",channum);
  haccum = new TH1F(hname,htitle,xbin,xmin,xmax);
  sprintf(hname,"htemp_%d",channum);
  htmp = (TH1F*)haccum->Clone(hname);
  char fname[500];
  sprintf(fname,"func1_%d",channum);
  //  func1 = new TF1(fname,"[0]",0.0,xmax);
  //  func1->SetParameter(0, 250.0);

  // JW: create histogram of pulse (time v amp) with time cuts
  sprintf(hname,"htemp_tc_%d",channum);
  htmp_tc = (TH1F*)haccum->Clone(hname);


  
  
  //TCanvas* c0 = new TCanvas("c0","c0");
  //c0->Divide(8,4);
  //c0->cd();
  //htmp = (TH1F*)haccum[0]->Clone("htmp");
  Double_t int_ch = -100;
  Double_t int_ch_tc = -200;

  // JW: define integrals histogram 
  Double_t ixmin = 4000, ixmax = 8000;

  Double_t ixmin_tc = 0, ixmax_tc = 8000;

  //  Double_t iymin = 0, iymax = 1000000;
  sprintf(hname,"Integrals_%d",channum);
  sprintf(htitle,"Integrals (channel_number_%d)",channum);
  integrals = new TH1F(hname,htitle,ixmax-ixmin,ixmin,ixmax);
  integrals->GetXaxis()->SetTitle("Integrated amplitude");

  sprintf(hname,"Integrals_tc_%d",channum);
  sprintf(htitle,"Integrals (channel_number_%d) w/time_cut (%.0f - %.0f ns)",channum, tcut_low, tcut_high);
  integrals_tc = new TH1F(hname,htitle,700,ixmin_tc,ixmax_tc);
  integrals_tc->GetXaxis()->SetTitle("Integrated amplitude");
 

  sprintf(hname,"Time_%d",channum);
  sprintf(htitle,"Time (channel_number_%d)",channum);
  htime = new TH1F(hname,htitle,xbin,xmin,xmax);
  htime->GetXaxis()->SetTitle("Peak time distrib (ns)");

  sprintf(hname,"Time_w_cut_%d",channum);
  sprintf(htitle,"Time_w_cut_%.2f (channel_number_%d)",ped_cut,channum);
  htime_c = new TH1F(hname,htitle,xbin,xmin,xmax);
  htime_c->GetXaxis()->SetTitle("Peak time distrib");


  sprintf(hname,"hscale_%d",channum);
  hscale = (TH1F*)haccum->Clone(hname);



  Double_t bin_int = 0.;

  Double_t cut = ped_cut;

  // convert input time cuts to histogram bins (bins of 4ns)

  Int_t low_bin = floor(tcut_low/4.);

  Int_t high_bin = ceil(tcut_high/4.);

  std::cout << " Low bin is " << low_bin << " and high bin is " << high_bin << std::endl;

  
  for(Int_t i=0 ; i<ent ; i++){

    for(int j=0 ; j<nch ; j++){
      time[j]  = -2222.0;
    }

    t1->GetEntry(i);
    bin_int = 0;
    htime->Fill(time[channum]);


    for(Int_t k=0 ; k<n ; k++){
      htmp->SetBinContent(k+1,ph[channum][k]);
      

      if(tcut_outside){ // If you want outside of defined range cut

	if( k> low_bin && k < high_bin){
	  
	  htmp_tc->SetBinContent(k+1,ph[channum][k]);
	}
      }
      else{ // if you want inside of defined range cut

	if( k<low_bin ||  k>high_bin){

	 htmp_tc->SetBinContent(k+1,ph[channum][k]);
	} 
	
      }  
	
      
      
      //htmp[j]->SetBinError(k+1,0.0);
      //bin_int += ph[channum][k];
    }
    //    htmp->Fit(fname,"Nq","",0.0,xmax);
    haccum->Add(htmp);
    hscale->Add(htmp);
    int_ch = htmp->Integral(xmin,xmax);
    int_ch_tc = htmp_tc->Integral(xmin,xmax);
    integrals->Fill(int_ch);
    integrals_tc->Fill(int_ch_tc);



    if(int_ch>cut){ // added to stop pedestals dominating
      htime_c->Fill(time[channum]);
    }

  //  haccum->Fit(fname,"Nq","",0.0,xmax);
  //  std::cout << channum << ": " << func1->GetParameter(0) << " " << func1->GetParError(0) << std::endl;
  }
  

  hscale->Scale(1./ent);
  sprintf(htitle,"Raw signals scaled (channel_number_%d)",channum);
  hscale->SetTitle(htitle);



  // Perfrom intial and improved Gaussian fit for histo of raw integrals
  TF1 *g1 = new TF1("g1","gaus",ixmin,ixmax);
  integrals->Fit(g1,"0R+");

  Double_t mean_1 = g1->GetParameter(1);
  Double_t sd_1 = g1->GetParameter(2);


  TF1 *g2 = new TF1("g2","gaus", (mean_1-2*sd_1), mean_1 + 2*sd_1);
  integrals->Fit(g2,"0R+");
  
  Double_t mean_2 = g2->GetParameter(1);
  Double_t sd_2 = g2->GetParameter(2);

  // Perfrom same procedure but for hist of raw integrals with time cut

  TF1 *g1_tc = new TF1("g1_tc","gaus",ixmin_tc,ixmax_tc);
  integrals_tc->Fit(g1_tc,"0R+");

  Double_t mean_1_tc = g1_tc->GetParameter(1);
  Double_t sd_1_tc = g1_tc->GetParameter(2);


  TF1 *g2_tc = new TF1("g2_tc","gaus", (mean_1_tc-2*sd_1_tc), mean_1_tc + 2*sd_1_tc);
  integrals_tc->Fit(g2_tc,"0R+");
  
  Double_t mean_2_tc = g2_tc->GetParameter(1);
  Double_t sd_2_tc = g2_tc->GetParameter(2);






  //  std::cout << channum << ": " << func1->GetParameter(0) << " " << func1->GetParError(0) << std::endl;

  
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->Divide(3,1);

  TCanvas* c2 = new TCanvas("c2","c2");
  TCanvas* c3 = new TCanvas("c3","c3");

  
  TCanvas* c4 = new TCanvas("c4","c4");
  c4->Divide(2,1);

  //c2->Divide(6,6);
  char pname[500];
  Double_t ymax = 1000.;
  Double_t ymin = -200.;
  
  hscale->SetMinimum(ymin);
  hscale->SetMaximum(ymax);
  hscale->GetXaxis()->SetTitle("Time (ns)");
  hscale->GetYaxis()->SetTitle("Channel number");

  integrals->GetXaxis()->SetRangeUser(mean_2-10*sd_2, mean_2+10*sd_2);
  //  integrals_tc->GetXaxis()->SetRangeUser(mean_2_tc10*sd_2_tc, mean_2_tc+10*sd_2__tc);
  integrals_tc->GetXaxis()->SetRangeUser(mean_2_tc+30, (mean_2_tc+10*sd_2_tc) + 1000);

  //  integrals_tc->SetBins(30);


  //  Int_t event_1 =   intc = htmp->Integral(xmin,xmax);
  c1->cd(1);
  integrals->Draw();


  c1->cd(2);
  integrals->Draw();
  g1->Draw("same");

  c1->cd(3);
  integrals->Draw();
  g2->Draw("same");

  c2->cd(0);
  integrals->Draw();
  g2->Draw("same");


  c3->cd(0);
  integrals_tc->Draw();
  //  g2_tc->Draw("same");

  c4->cd(1);
  htime->Draw();
  c4->cd(2);
  htime_c->Draw();
  
  // JW getting fit paraeters on hisots

  c1->cd(1);
  c1->BuildLegend(0.15, 0.7, 0.4, 0.9, "Gaus fit statisitcs");
 
  /* mycanvas->Modified(); mycanvas->Update(); // make sure itâs really (re)drawn
 TPaveStats st = ((TPaveStats)(gr1->GetListOfFunctions()->FindObject(âstatsâ)));
 if (st) {
  st->SetTextColor(gr1->GetLineColor());
 st->SetX1NDC(0.64); st->SetX2NDC(0.99);
 st->SetY1NDC(0.4); st->SetY2NDC(0.6);
 }
 st = ((TPaveStats*)(gr2->GetListOfFunctions()->FindObject(âstatsâ)));
 if (st) {
  st->SetTextColor(gr2->GetLineColor());
 st->SetX1NDC(0.64); st->SetX2NDC(0.99);
 st->SetY1NDC(0.15); st->SetY2NDC(0.35);
 }
 mycanvas->Modified(); mycanvas->Update(); // make sure itâs really (re)drawn

  */



  std::cout << "First gaussian with mean: " << mean_1 << " and standard deviation " << sd_1 << std::endl;

  std::cout << "Second gaussian with mean: " << mean_2 << " and standard deviation " << sd_2 << std::endl;
 

  std::cout << "\n" << "\n" << "For integrals with time cuts: " << std::endl;

   std::cout << "First gaussian with mean: " << mean_1_tc << " and standard deviation " << sd_1_tc << std::endl;

  std::cout << "Second gaussian with mean: " << mean_2_tc << " and standard deviation " << sd_2_tc << std::endl;
 

 
}



