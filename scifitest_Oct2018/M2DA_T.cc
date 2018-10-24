/*
  "showtime.cc"
  Showing time at the peak for each channel
  (APEX SciFi test at EEL122)

  Toshiyuki Gogami, July 9, 2018
*/


void M2DA_T(int runnum=5049){
  // ================================ //
  // ===== General conditions ======= //
  // ================================ //
  gStyle->SetOptStat(0);
  int pflag = 1; // print histograms out? (1: yes)
  
  
  // ================================ //
  // ====== Open a ROOT file ======== //
  // ================================ //
  char fname[500];
  sprintf(fname,"./rootfiles2/charge_acsis_%d.root",runnum);
  TFile* f1 = new TFile(fname);
  TTree* t1 = (TTree*)f1->Get("tree");
  
  const int nch = 64;
  double time[nch];
  double intc[nch];
  t1->SetBranchAddress("time",&time);
  t1->SetBranchAddress("intc",&intc);
  double ent = t1->GetEntries();
  
  
  // ================================ //
  // ===== Create a histogram ======= //
  // ================================ //
  TH1F* htime[nch];
  double xmin = 0, xmax = 296;
  int xbin = 74; // the number of data for each channel
  char hname[500];
  for(int i=0 ; i<nch ; i++){
    sprintf(hname,"h%d",i);
    htime[i] = new TH1F(hname,"",xbin,xmin,xmax);
    htime[i]->GetXaxis()->SetTitle("(ns)");
  }

  TH2F* AvsT[nch];
  double ymin = 0, ymax = 296;
  int ybin = 74;
  for(int i=0; i<nch; i++){
    sprintf(hname, "AvsT%d",i);
    AvsT[i] = new TH2F(hname,"",xbin,xmin,xmax,ybin,ymin,ymax);
    AvsT[i]->GetYaxis()->SetTitle("(Volts)");
    AvsT[i]->GetXaxis()->SetTitle("(ns)");
  }
  
  // ============================================ //
  // ===== Main loop for filling with data ====== //
  // ============================================ //
  for(int i=0 ; i<ent ; i++){
    // ----- Initialization ----- //
    for(int j=0 ; j<nch ; j++){
      time[j] = -2222.0;
      intc[j] = -2222.0;
    }
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      if(intc[j]>200.0){
	htime[j]->Fill(time[j]); // Fill the hist with data
	AvsT[j]->Fill(time[j],intc[i]);
      }
    }
  }
  
  // ============================================== //
  // ======== Draw histograms on canvases ========= //
  // ============================================== //
  TCanvas* c1 = new TCanvas("c1","c1");c1->Divide(6,6);
  TCanvas* c2 = new TCanvas("c2","c2");c2->Divide(6,6);
  TCanvas* c3 = new TCanvas("c3","c3");c3->Divide(6,6);
  TCanvas* c4 = new TCanvas("c4","c4");c4->Divide(6,6);
  
  for(int i=0 ; i<32 ; i++){
    c1->cd(i+1);
    htime[i]->Draw();  
  }
  for(int i=32 ; i<64 ; i++){
    c2->cd(i+1-32);
    htime[i]->Draw();
  }
  for(int i=0; i<32; i++){
    c3->cd(i+1);
    AvsT[i]->Draw();
  }
  for(int i=32; i<64; i++){
    c4->cd(i+1-32);
    AvsT[i]->Draw();
  }

  // ========================= //
  // ========= Print ========= //
  // ========================= //
  if(pflag==1){
    char pname[500];
    sprintf(pname,"time_scifi_r%d_0-31.png",runnum);
    c1->Print(pname);
    sprintf(pname,"time_scifi_r%d_32-63.png",runnum);
    c2->Print(pname);
    
  }

  
}
