void L_s0_timing(){

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);

  const Float_t ctos = 0.5E-9; //0.5ns per TDC channel
  const Float_t ly_s2 = 0.432; //Full length of s2 along y [m]
  const Float_t lx_s0 = 1.70; //Full length of s0 along x [m]
  const Float_t c_s2 = 1.26193e+08; //Speed of light in s2 [m/s]
  const Float_t c_s0 = 1.23858e+08; //Speed of light in s0 [m/s]
  const Float_t elec_v = 3.0E8; //Speed of electron between s0 and s2 [m/s]

  Float_t tof; //in seconds
  Float_t tof_sum; //in channels
 
   
  //Initialize Histograms
  TH2F *h1a = new TH2F("h1a","LHRS Left_s0_TDC vs X: Good Events",500,-1.25,1.25,400,1500,1900);
  h1a->GetXaxis()->SetTitle("X_s0 [m]"); h1a->GetXaxis()->CenterTitle();
  h1a->GetYaxis()->SetTitle("TDC Channel"); h1a->GetYaxis()->CenterTitle();
  
  TH2F *h1b = new TH2F("h1b","LHRS Right_s0_TDC vs Track  X: Good Events",500,-1.25,1.25,400,1500,1900);
  h1b->GetXaxis()->SetTitle("X_s0 [m]"); h1b->GetXaxis()->CenterTitle();
  h1b->GetYaxis()->SetTitle("TDC Channel"); h1b->GetYaxis()->CenterTitle();

  TH2F *h2a = new TH2F("h2a","LHRS S0 : Left_TDC - Right_TDC  vs  X: Good Events",500,-1.25,1.25,180,-60,120);
  h2a->GetXaxis()->SetTitle("X_s0 [m]"); h2a->GetXaxis()->CenterTitle();
  h2a->GetYaxis()->SetTitle("Left_TDC-Right_TDC"); h2a->GetYaxis()->CenterTitle();
  
  TProfile *h2b = new TProfile("h2b","LHRS S0 :Left - Right  vs X: Good Events",100,-1.5,1.5,-60,120);
  h2b->SetLineColor(kRed);h2b->SetMarkerColor(kRed);

 
  TChain * t = new TChain("T");

  t->Add("/Users/tong/Tritium_analysis/Rootfiles/tritium_811*.root");
   

    
  //Define Variables
  Double_t L_tr_n,L_cer_asum_c,L_ps_e,L_sh_e;
  Double_t L_tr_p[100],L_s0_trx[100];
  Double_t L_s0_lt[10],L_s0_rt[10];
  Double_t L_s0_nthit;
  Double_t L_s2_try[100];
  Double_t L_s2_nthit;
  Double_t L_s2_t_pads[16];
  Double_t evtypebits;
  Double_t L_s0_trpath[100],L_s2_trpath[100];
  
  //Define Branch Status/Addresses
  t->SetBranchStatus("*",0);
  t->SetBranchStatus("L.tr.n",1);
  t->SetBranchStatus("L.tr.p",1);
  t->SetBranchStatus("L.cer.asum_c",1);
  t->SetBranchStatus("L.prl1.e",1);
  t->SetBranchStatus("L.prl2.e",1);
  t->SetBranchStatus("L.s0.lt",1);
  t->SetBranchStatus("L.s0.rt",1);
  t->SetBranchStatus("L.s0.trx",1);
  t->SetBranchStatus("L.s0.nthit",1);
  t->SetBranchStatus("L.s2.try",1);
  t->SetBranchStatus("L.s2.nthit",1);
  t->SetBranchStatus("L.s2.t_pads",1);
  t->SetBranchStatus("DL.evtypebits",1);
  t->SetBranchStatus("L.s0.trpath",1);
  t->SetBranchStatus("L.s2.trpath",1);

  t->SetBranchAddress("L.tr.n",&L_tr_n);
  t->SetBranchAddress("L.tr.p",L_tr_p);
  t->SetBranchAddress("L.cer.asum_c",&L_cer_asum_c);
  t->SetBranchAddress("L.prl1.e",&L_ps_e);
  t->SetBranchAddress("L.prl2.e",&L_sh_e);
  t->SetBranchAddress("L.s0.lt",L_s0_lt);
  t->SetBranchAddress("L.s0.rt",L_s0_rt);
  t->SetBranchAddress("L.s0.trx",L_s0_trx);
  t->SetBranchAddress("L.s0.nthit",&L_s0_nthit);
  t->SetBranchAddress("L.s2.try",L_s2_try);
  t->SetBranchAddress("L.s2.nthit",&L_s2_nthit);
  t->SetBranchAddress("L.s2.t_pads",L_s2_t_pads);
  t->SetBranchAddress("DL.evtypebits",&evtypebits);
  t->SetBranchAddress("L.s0.trpath",L_s0_trpath);
  t->SetBranchAddress("L.s2.trpath",L_s2_trpath);

  Int_t nentries = t->GetEntries();

  cout<<"Total Number of Events = "<<nentries<<endl;

  //Loop over events
  for(Int_t i=0;i<nentries;i++){
    
    if(i%100000==0) cout << " events processed = " << i << endl;
    t->GetEntry(i);

   
      if(L_tr_n==1 && L_cer_asum_c>1500 && (L_ps_e+L_sh_e)/(1000.*L_tr_p[0])>0.8 && L_s0_nthit==1 && ((Int_t)evtypebits>>1&1)){
     
      
      h1a->Fill(L_s0_trx[0],L_s0_lt[0]);
      h1b->Fill(L_s0_trx[0],L_s0_rt[0]);
      h2a->Fill(L_s0_trx[0],L_s0_lt[0]-L_s0_rt[0]);
      h2b->Fill(L_s0_trx[0],L_s0_lt[0]-L_s0_rt[0]);
}
}
   
  //Make Plots
  TCanvas *c1 = new TCanvas("c1");
  c1->Divide(1,2);
  c1->cd(1);
  h1a->Draw("col");
  c1->cd(2);
  h1b->Draw("col");
  
  TCanvas *c2 = new TCanvas("c2");
  h2b->Draw();
  h2b->Fit("pol1","R","",-0.75,0.75);
  c2->Update();
  h2a->Draw("col");
  h2b->Draw("same");
  c2->Modified();

  Float_t difference =  h2b->GetFunction("pol1")->GetParameter(0);
  Float_t temp_speed = h2b->GetFunction("pol1")->GetParameter(1);
  Float_t c_speed = abs((2./temp_speed)/ctos);
  
 

  c1->Print("s0_timing.pdf[");
  c1->Print("s0_timing.pdf");
  c2->Print("s0_timing.pdf");
  c2->Print("s0_timing.pdf]");
    
  Float_t corr_left = 1500.00;
  Float_t corr_right = 1500.00+difference;
  
  //Write some info to the screen
  cout<<"\n------------------------------------------------------\n";

  cout<<"Left - Right Intercept [channel]: "<<difference<<endl;
  
  cout<<"Left PMT Correction Factor: "<<corr_left<<endl;
  cout<<"Right PMT Correction Factor: "<<corr_right<<endl<<endl;

  cout<<"Speed of Light in Scintillator = "<<c_speed<<" m/s !!!"<<endl;
  cout<<"------------------------------------------------------\n";


}

