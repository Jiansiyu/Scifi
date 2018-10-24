/*
  npe.cc
  
  Toshiyuki Gogami, December 4, 2017
*/

void npe(int maxev = 1000000,
	 int run = 4628,
	 int print = 0){
  // ============================================= //
  // ============ General conditions ============= //
  // ============================================= //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  // ==================================== //
  // ======== Open ROOT file ============ //
  // ==================================== //
  char fname[500];
  sprintf(fname,"./rootfiles2/charge_acsis_%d_2.root",run);
  const int nch = 64; // The number of channel of FADC250
  const int n = 74;   // The number of data (4 ns per one data point)
  TFile* f1 = new TFile(fname);
  TTree* t1 = (TTree*)f1->Get("tree");
  double intc[nch];
  int nscifi[nch];
  double npe[nch];
  int evID, runID;
  double ent = t1->GetEntries();
  if(maxev<ent) ent = maxev;
  t1->SetBranchAddress("runID"  ,   &runID  );
  t1->SetBranchAddress("evID"   ,   &evID   );
  t1->SetBranchAddress("npe"    ,   &npe    );
  t1->SetBranchAddress("nscifi" ,   &nscifi );
  
  
  //TH2F* hemp = new TH2F("hemp","",100,-80.0,80.0,100,-80.0,80.);
  //hemp->GetXaxis()->SetTitle("[cm]");
  //hemp->GetYaxis()->SetTitle("[cm]");

  ifstream* ifs = new ifstream("./singlePE/spe_46224624.dat");
  double temp, pedmean[nch], pedwidth_npe[nch], spe[nch];
  double npeth[nch];
  for(int i=0 ; i<nch ; i++){
    *ifs >> temp >> temp >> pedmean[i] >> temp >> pedwidth_npe[i] >> temp >> spe[i] >> temp;
    //cout << pedmean[i] << " " << pedwidth_npe[i] << " " << spe[i] << endl;
    pedwidth_npe[i] = pedwidth_npe[i] / (spe[i]-pedmean[i]);
    //cout << pedwidth_npe[i] << endl;
    npeth[i] = fabs(pedwidth_npe[i]) * 3.0;
    //cout << npeth[i] << endl;
    npeth[i] = 0.3;
  }
  
  
  // ========================================== //
  // ======== Start event display  ============ //
  // ========================================== //
  
  const int maxmulti = 20;
  int hitfibernum[maxmulti];
  TH1F* hnpe[nch];

  double xmin = -5.0, xmax = 50.0;
  int xbin = 100;
  for(int i=0 ; i<nch ; i++){
    char hname[500];
    sprintf(hname,"hnpe%d",i);
    hnpe[i] = new TH1F(hname,"",xbin,xmin,xmax);
    hnpe[i]->GetXaxis()->SetRangeUser(-3.0,25.0);
  }
  
  for(int i=0 ; i<ent ; i++){
    // --- initialization ---
    runID = 0;
    evID  = 0;
    for(int j=0; j<nch ; j++){
      npe[j]    = -2222.0;
      nscifi[j] = -2222.0;
    }
    
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      if(npe[j]>-200.0 && npe[j]>npeth[j]){
	hnpe[j]->Fill(npe[j]);
      }
    }
  }

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->Divide(6,6);
  double npe_mean[nch];
  for(int i=0 ; i<32 ; i++){
    c1->cd(i+1);gPad->SetLogy(1);
    npe_mean[i] = 0.0;
    npe_mean[i] = hnpe[i]->GetMean();
    hnpe[i]->Draw();
    cout << i << " " << npe_mean[i] << endl;
  }
  TCanvas* c2 = new TCanvas("c2","c2");
  c2->Divide(6,6);
  for(int i=32 ; i<64 ; i++){
    c2->cd(i+1-32);gPad->SetLogy(1);
    npe_mean[i] = 0.0;
    npe_mean[i] = hnpe[i]->GetMean();
    hnpe[i]->Draw();
    cout << i << " " << npe_mean[i] << endl;
  }
  
  // ==== print =====
  //c1->Print("npe_th0.3_r46224624_1.png");
  //c2->Print("npe_th0.3_r46224624_2.png");

}
  
