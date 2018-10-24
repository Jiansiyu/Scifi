/*
  occupancy.cc
  
  Toshiyuki Gogami, July 9, 2018
*/

void occupancy(int runnum=46224625){
  // ================================ //
  // ===== General conditions ======= //
  // ================================ //
  //runnum = 46224625; // cosmic ray
  //runnum = 4624; // cosmic ray
  //runnum = 4625; // cosmic ray
  //runnum = 4622; // cosmic ray
  //runnum = 4628; // Sr-90 (pos1)
  //runnum = 46314632; // Sr-90 (pos2) Good to see 
  //runnum = 46364638; // Sr-90 (pos3)
  //runnum = 46394641; // Sr-90 (pos4)
  gStyle->SetOptStat(0);
  int pflag = 0; // print histograms out? (1: yes)
  
  
  // ================================ //
  // ====== Open a ROOT file ======== //
  // ================================ //
  char fname[500];
  //sprintf(fname,"./rootfiles2/charge_acsis_%d_2.root",runnum);
  sprintf(fname,"./rootfiles2/acsis_%d_2.root",runnum);
  TFile* f1 = new TFile(fname);
  TTree* t1 = (TTree*)f1->Get("tree");
  
  const int nch = 64;
  double time[nch];
  double npe[nch];
  int nscifi[nch];
  t1->SetBranchAddress("time",&time);
  //t1->SetBranchAddress("intc",&intc);
  t1->SetBranchAddress("npe",&npe);
  t1->SetBranchAddress("nscifi",&nscifi);
  double ent = t1->GetEntries();
  
  // ================================ //
  // ===== Create a histogram ======= //
  // ================================ //
  int nbin = 32, min = 0.0, max = 32.0;
  TH1F* h1 = new TH1F("h1","",nbin,min,max);
  TH1F* h2 = (TH1F*)h1->Clone("h2");
  TH2F* hist = new TH2F("hist","",nbin,min,max,nbin,min,max);
  
  //double npe_th = 0.3;
  double npe_th = 2.5;
  int hitfiber = 0;
  bool hit1 = false;
  bool hit2 = false;
  const int maxmulti = 10;
  int hitf1[maxmulti], nhit1=0;
  int hitf2[maxmulti], nhit2=0;
		       
  for(int i=0 ; i<ent ; i++){
    // ----- Initialization ----- //
    for(int j=0 ; j<nch ; j++){
      time[j] = -2222.0;
      npe[j]  = -2222.0;
      nscifi[j] = -2;
    }
    hitfiber = -2;
    hit1 = false;
    hit2 = false;
    for(int j=0 ; j<maxmulti ; j++){
      hitf1[j] = -2;
      hitf2[j] = -2;
    }
    nhit1 = 0;
    nhit2 = 0;
    
    // ---- Data filling ----- //
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      if(npe[j]>npe_th){
	hitfiber = nscifi[j];
	if(hitfiber<32){
	  hit1 = true;
	  h1->Fill(hitfiber);
	  hitf1[nhit1] = hitfiber;
	  nhit1++;
	}
	else {
	  hit2 = true;
	  h2->Fill(hitfiber-32);
	  hitf2[nhit2] = hitfiber-32;
	  //cout << j<< " " << hitfiber << " " << npe[j] << endl;
	  nhit2++;
	}
      }
    }
    
    if(hit1==true && hit2==true){
      for(int j=0 ; j<nhit1 ; j++){
	for(int k=0 ; k<nhit2 ; k++){
	  //hist->Fill(hitf1[j],hitf2[k]);
	  hist->Fill(hitf2[k],hitf1[j]);
	}
      }
    }
    
  }
  
  // ================================= //
  // ===== Histogram options ========= //
  // ================================= //
  h1->GetXaxis()->SetTitle("N_{y}");
  h1->GetYaxis()->SetTitle("Counts");
  h1->SetMinimum(0);
  h2->GetXaxis()->SetTitle("N_{x}");
  h2->GetYaxis()->SetTitle("Counts");
  h2->SetMinimum(0);
  
  
  // ==================================== //
  // ========== Draw histograms ========= //
  // ==================================== //
  TCanvas* c1 = new TCanvas("c1","c1");
  h1->Draw();
  TCanvas* c2 = new TCanvas("c2","c2");
  h2->Draw();
  
  TCanvas* c3 = new TCanvas("c3","c3",600,600);
  hist->Draw("colz");
  
  
  // ================= //
  // ===== Print ===== //
  // ================= //
  //char pname[500];
  //sprintf(pname,"hitpat_r%d_0-31.png",runnum);
  //c1->Print(pname);
  //sprintf(pname,"hitpat_r%d_32-63.png",runnum);
  //c2->Print(pname);
  //sprintf(pname,"hitpat_r%d_2D.png",runnum);
  //c3->Print(pname); 
  
}
