/*
  "showcharge_time.cc"
  This macro shows charge (integraed area of pulse) 
  versus time.

  ** time: time at peak
  ** wtime: time weighted by pulse height
  
  (APEX SciFi test at EEL122)
  Toshiyuki Gogami,  October 11, 2018
*/


void showcharge_time(int runnum = 5041){
  // ================================ //
  // ===== General conditions ======= //
  // ================================ //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  //bool pflag = true;  // print histograms out? (true: yes)
  bool pflag = true; // print histograms out? (true: yes)
  
  
  // =================================== //
  // ====== Opening a ROOT file ======== //
  // =================================== //
  char fname[500];
  sprintf(fname,"./rootfiles2/charge_acsis_%d.root",runnum);
  //  sprintf(fname,"./charge_acsis_%d.root",runnum);
  TFile* f1 = new TFile(fname);
  TTree* t1 = (TTree*)f1->Get("tree");
  
  const int nch = 64;
  double time[nch];
  double wtime[nch];
  double intc[nch];
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("wtime", &wtime);
  t1->SetBranchAddress("intc",  &intc);
  double ent = t1->GetEntries();
  
  
  // ================================== //
  // ===== Creating a histogram ======= //
  // ================================== //
  TH2F* h_TvsA[nch]; // 2D histograms between Charge and Time
  TH1F* h_time[nch]; // Time histograms
  char hname[500];
  double xmin = -200, xmax = 1500.0; int xbin = (int)xmax-(int)xmin;
  int ybin = 24;  double ymin = 0,    ymax = 96.0 ;
  for(int i=0; i<nch; i++){
    sprintf(hname, "h_TvsA%d",i);
    h_TvsA[i] = new TH2F(hname,"",xbin,xmin,xmax,ybin,ymin,ymax);
    h_TvsA[i]->GetXaxis()->SetTitle("Pulse integration (ch)");
    h_TvsA[i]->GetYaxis()->SetTitle("Time (ns)");
    sprintf(hname, "h_time%d",i);
    h_time[i] = new TH1F(hname,"",ybin,ymin,ymax);
    h_time[i]->GetXaxis()->SetTitle("Time (ns)");
  }

  
  // ============================================ //
  // ===== Main loop for filling with data ====== //
  // ============================================ //
  for(int i=0 ; i<ent ; i++){
    // ----- Initialization ----- //
    for(int j=0 ; j<nch ; j++){
      time[j]  = -2222.0;
      wtime[j] = -2222.0;
      intc[j]  = -2222.0;
    }
    t1->GetEntry(i);
    
    for(int j=0 ; j<nch ; j++){
      if(intc[j]>200.0){ // Filling the histogram with data
      //if(intc[j]>-200.0){ // use this if you want to include pedestals
	
	//h_time[j] ->Fill(wtime[j]);        // weighted time
	//h_TvsA[j]->Fill(intc[j],wtime[j]); // charge vs. weighted time
	h_time[j] -> Fill(time[j]);          // time
	h_TvsA[j] -> Fill(intc[j],time[j]);  // charge vs. weighted time
	
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
    c1->cd(i+1); h_time[i]->Draw();
    c3->cd(i+1); h_TvsA[i]->Draw("col");
    c2->cd(i+1); h_time[i+32]->Draw();
    c4->cd(i+1); h_TvsA[i+32]->Draw("col");
  }
  //for(int i=32 ; i<64 ; i++){
  //  c2->cd(i+1-32); h_time[i]->Draw();
  //  c4->cd(i+1-32); h_TvsA[i]->Draw("col");
  //}

  
  // ============================ //
  // ========= Printing ========= //
  // ============================ //
  if(pflag==true){
    char pname[500];
    sprintf(pname,"time_scifi_r%d_0-31.png",runnum); // original 
    //sprintf(pname,"Im_Andreeeeeeeeeeeew_r%d_0-31.png",runnum);
    c1->Print(pname);
    sprintf(pname,"time_scifi_r%d_32-63.png",runnum); // original
    //sprintf(pname,"Im_Andreeeeeeeeeeeew_r%d_32-63.png",runnum);
    c2->Print(pname);
    sprintf(pname,"charge-time_scifi_r%d_0-31.png",runnum); // original
    //sprintf(pname,"Im_Joooooooooooooohn_r%d_0-31.png",runnum);
    c3->Print(pname);
    sprintf(pname,"charge-time_scifi_r%d_32-63.png",runnum); // original
    //sprintf(pname,"Im_Joooooooooooooohn_r%d_32-64.png",runnum);
    c4->Print(pname);
  }

  
  // ============================================================ //
  // ======= Creating a new ROOT file which can be used  ======== //
  // ======= to check each histogram                   ========== //
  // ============================================================ //
  char newfname[500];
  sprintf(newfname,"rootfiles2/histcheck_charge-time_%d.root",runnum);
  TFile* newroot = new TFile(newfname,"recreate");
  TObjArray h(0);
  h.Add(c1);
  h.Add(c2);
  h.Add(c3);
  h.Add(c4);
  for(int i=0 ; i<nch ; i++){
    h.Add(h_time[i]);
    h.Add(h_TvsA[i]);
  }
  h.Write();
  newroot->Close();


  cout << endl;
  cout << " ROOT file " << f1->GetName() << " was analyzed." << endl;
  cout << " A new ROOT file to check each histogram has been output to: " << endl;
  cout << " --> " << newfname << endl;
  cout << endl;
  
}
