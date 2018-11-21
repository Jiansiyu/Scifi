/*
  getSPE.cc
  
  Toshiyuki Gogami, December 1, 2017.
*/


double fitf0(double* x, double* par){
  double g_ped = par[0] * exp(-0.5 * pow((x[0]-par[1])/par[2],2.0) );
  return g_ped;
}
double fitf1(double* x, double* par){
  double g_ped = par[0] * exp(-0.5 * pow((x[0]-par[1])/par[2],2.0) );
  double p1 = par[3] * TMath::Poisson(x[0]-par[4], par[5]);
  return g_ped + p1;
}
double fitf3(double* x, double* par){
  double pedcut = par[8];
  double g1;// = par[0] * exp(-0.5 * pow((x[0]-par[1])/par[2],2.0) );
  double l1 = 0.0;
  if(pedcut<x[0]){
    g1 = par[0] * exp(-0.5 * pow((x[0]-par[1])/par[2],2.0) );
    l1 = par[3] + par[4]*x[0];
  }
  else l1 = 0.0;
  double g_ped = par[5] * exp(-0.5 * pow((x[0]-par[6])/par[7],2.0) );
  return g1 + l1 + g_ped;
}
double fitf(double* x, double* par){
  double g_ped = par[0] * exp(-0.5 * pow((x[0]-par[1])/par[2],2.0) );
  double g1    = par[3] * exp(-0.5 * pow((x[0]-par[4])/par[5],2.0) );
  return g_ped + g1;
}


void getSPE_allch(int run  = 5049, Int_t channel = -1){


  // ============================== //
  // ======= Open ROOT file ======= //
  // ============================== //
  Int_t n = 24; // fac window


  char inputfilename[500];
  sprintf(inputfilename,"./singlePE/intcHist_acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  
  //=======================================//
  //===== Set-up output fit variables =====//
  //=======================================//

  Int_t nch = 64;


  Double_t pedmean[nch];
  Double_t pedmean_er[nch];
  Double_t pedsigma[nch];
  Double_t pedsigma_er[nch];
  Double_t spemean[nch];
  Double_t spemean_er[nch];


  for(Int_t ch = 0; ch<nch; ch++){


  
    if(ch<0 || ch>63) ch = 0;
 

    char hname[500];
    sprintf(hname, "hist1_%d", ch);
    TH1F* h1 = (TH1F*)f1->Get(hname);
    
    double xmin = h1->GetXaxis()->GetXmin();
    double xmax = h1->GetXaxis()->GetXmax();
    int    xbin = h1->GetXaxis()->GetNbins();
    double binreso = (xmax-xmin)/(double)xbin;
    cout << " Binning resolution = " << binreso << endl;
    h1->GetXaxis()->SetTitle("ch");
    char xtitle[500];
    sprintf(xtitle,"Counts / %d ch",(int)binreso);
    h1->GetYaxis()->SetTitle(xtitle);
    
    
    // ================================ //
    // ===== Draw histogram =========== //
    // ================================ //
    /*  TCanvas* c1 = new TCanvas("c1","c1");
	gPad->SetLogy(1);
	h1->Draw(); */
    
    
    // ============================== //
    // ===== Fit to pedestal ======== //
    // ============================== //
    TF1* func1 = new TF1("func1",fitf,
			 -1000.0, 2000.0, 
			 3);
    func1->SetParameter(0, 1000.0);
    func1->SetParameter(1, 0.0);
    func1->SetParameter(2, 55.0);
    func1->SetParLimits(2, -300.0, 300.0);
    func1->SetParLimits(1, -300.0, 300.0);
    
    //func1->SetParameter(3, 800.0);
    //func1->SetParameter(4, 350.0);
    //func1->SetParameter(5, 50.0);
    
    h1->Fit("func1","N","",-1000.0, 1000.0); // N here means do not draw
    func1->SetNpx(1000);
    //func1->Draw("same");
    pedmean[ch]    = func1->GetParameter(1);
    pedmean_er[ch]  = func1->GetParError(1);
    pedsigma[ch]    = func1->GetParameter(2); 
    pedsigma_er[ch] = func1->GetParError(2); 
    double pedmin, pedmax;
    pedmin = pedmean[ch] - 3.0*pedsigma[ch];
    pedmax = pedmean[ch] + 3.0*pedsigma[ch];
    //cout << pedsigma << " " << pedmax << endl;
  
    TF1* func2 = new TF1("func2",fitf3,
		       -1000.0, 300.0, 
		       9); 
    func2->SetNpx(1000);
    func2->SetParameter(0, 50.0); func2->SetParLimits(0, 0.00, 400.0);
    func2->SetParameter(1, 500.0); func2->SetParLimits(1, 200.0, 700.0);
    //func2->SetParameter(2, 80.0); func2->SetParLimits(2, 75.0, 300.0);
    func2->SetParameter(2, 180.0); func2->SetParLimits(2, 75.0, 300.0);
    func2->SetParameter(3, 50.0);
    func2->SetParameter(4, -0.05); func2->SetParLimits(4,-0.08,0.0);
    func2->FixParameter(5, func1->GetParameter(0));
    func2->SetParameter(6, func1->GetParameter(1));
    func2->SetParameter(7, func1->GetParameter(2));
    func2->FixParameter(8, pedmax);

    pedsigma[ch]    = func2->GetParameter(7); 
    pedsigma_er[ch] = func2->GetParError(7); 

    
    h1->Fit("func2","","",-1000.0, 3000.0); // took out NB in second param
    //    func2->Draw("same");

    spemean[ch] = func2->GetParameter(1);
    spemean_er[ch] = func2->GetParError(1);

    TCanvas* c1 =  new TCanvas("c1","c1");

    if (ch == channel){

      c1->cd(0);
      h1->Draw();
      func2->Draw("same");
      c1->WaitPrimitive();
    }
      



    
    // TCanvas* c1 = new TCanvas("c1","c1");
    // h1->Draw();
    // func2->Draw("same");

    // cout << endl;
    // cout << " " 
    // 	 << run << " " 
    // 	 << anach << " " 
    // 	 << pedmean << " " 
    // 	 << pedmean_er << " "
    // 	 << pedsigma << " " 
    //    << pedsigma_er << " "
    // 	 << spemean << " " 
    // 	 << spemean_er << endl;
    // cout << endl;

  }


  // ========================================== //
  // ===== Send fit reults to output file ===== //
  // ========================================== //



  char pedfname[500];
  sprintf(pedfname,"./singlePE/spe_%d.dat",run);
  //  sprintf(pedfname,"./ped/pedestal_%d_testtesttest.dat",run);
  ofstream* ofs = new ofstream(pedfname);
  for(int i=0 ; i<nch ; i++){
    //haccum[i]->Scale(1./nite);
    *ofs  << run  << " " << i << " "
	  << pedmean[i] << " " << pedmean_er[i] << " " << pedsigma[i] << " " << pedsigma_er[i] << " " << spemean[i] << " " << spemean_er[i] << endl;;
    }
  ofs->close();





}
