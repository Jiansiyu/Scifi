/*
  plotnpe.cc
  
  Toshiyuki Gogami, December 4, 2017
*/

void plotnpe(){
  // ================================ //
  // ===== General conditions ======= //
  // ================================ //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);


  // ================================ //
  // ===== Read input file ========== //
  // ================================ //
  TH1F*h1 = new TH1F("h1","",35,0.0,7.0);
  ifstream* ifs = new ifstream("npe_th0.3.dat");
  const int nch = 64;
  double x[nch];
  double y[nch];
  for(int i=0 ; i<nch ; i++){
    *ifs >> x[i] >> y[i];
    if(y[i]<0.1 || (i==14 || i==18 || i==20 || i==24 || i==25 || 
		    i==55 || i==56 || i==60 || i==63)){
      y[i] = -2222.0;
    }
    else{
      h1->Fill(y[i]);
    }
    
  }

  TH2F* hemp = new TH2F("hemp","",100,-2.0,33.0,100,0.0,7.0);
  hemp->GetXaxis()->SetTitle("Channel number");
  hemp->GetYaxis()->SetTitle("The number of photoelectrons");


  // ============================= //
  // ====== Create graph ========= //
  // ============================= //
  TGraph* gr1 = new TGraph(nch,x,y);
  gr1->SetName("gr1");


  // ============================== //
  // ========= Draw graph ========= //
  // ============================== //
  TCanvas* c1 = new TCanvas("c1","c1");
  hemp->Draw();
  gr1->Draw("*");
  gr1->SetMarkerStyle(24);
  gr1->SetMarkerSize(1.5);
  gr1->SetMarkerColor(1);
  //gr-1>SetLineWidth(2);

  TCanvas* c2 = new TCanvas("c2","c2");
  h1->GetXaxis()->SetTitle("NPE");
  //h1->GetYaxis()->SetTitle("Counts / 0.1 NPE");
  h1->GetYaxis()->SetTitle("Counts / 0.2 NPE");
  h1->SetFillStyle(1001);
  h1->SetFillColor(9);
  h1->Draw();

  double mean  = h1->GetMean();
  double stdev = h1->GetStdDev();
  cout << mean << " "  << stdev << endl;

  //c1->Print("NPE_vs_channel.png");
  //c2->Print("NPE_stdev.png");

  
}
