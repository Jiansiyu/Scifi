/*
  evdisp.cc
  
  Toshiyuki Gogami, December 4, 2017
*/

void Toshi_event_dis(int maxev = 50){
  // ============================================= //
  // ============ General conditions ============= //
  // ============================================= //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  char pname[500];
  
  // ==================================== //
  // ======== Open ROOT file ============ //
  // ==================================== //
  const int nch = 64; // The number of channel of FADC250
  const int n = 74;   // The number of data (4 ns per one data point)
  TFile* f1 = new TFile("../rootfiles2/charge_acsis_46224624_2.root");
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
  
  
  TH2F* hemp = new TH2F("hemp","",100,-80.0,80.0,100,-80.0,80.);
  
  
  TCanvas* c1 = new TCanvas("c1","c1",700,700);
  const int nfiber = 64;
  hemp->Draw();
  TBox* b[nfiber];
  double phi = 1.0; // phi = 1.0 mm
  double step = 3.79;
  double center = step * (32.-1.) / 2.0; // 121.28/2 mm
  double length = center*2.0; // mm
  for(int i=0 ; i<nfiber ; i++){
    if(i<32){
      b[i] = new TBox(-center-phi/2.0 + step*i,
		      -length/2.0,
		      -center+phi/2.0 + step*i,
		      length/2.0);
      b[i]->SetFillStyle(1001);
      b[i]->SetFillColor(1);
      b[i]->Draw("same");
    }
    else if(i<64){
      b[i] = new TBox(-length/2.0,
		      -center-phi/2.0 + step*(i-32),
		      length/2.0,
		      -center+phi/2.0 + step*(i-32));
      b[i]->SetFillStyle(1001);
      b[i]->SetFillColor(1);
      b[i]->Draw("same");
    }
  }
  
  //for(int i=0)
  
  
  // ========================================== //
  // ======== Start event display  ============ //
  // ========================================== //
  double npeth = 2.0;
  bool hit=false;
  bool hitfront = false;
  bool hitrear  = false;
  int nmulti=0;
  const int maxmulti = 20;
  int hitfibernum[maxmulti];
  for(int i=0 ; i<ent ; i++){
    // --- initialization ---
    runID = 0;
    evID  = 0;
    for(int j=0; j<nch ; j++){
      npe[j]    = -2222.0;
      nscifi[j] = -2222.0;
    }
    hit      = false;
    hitfront = false;
    hitrear  = false;
    nmulti = 0;
     
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      if(npe[j]>npeth && nmulti<maxmulti){
	hit=true;
	//int nfi = nscifi[j];
	b[nscifi[j]]->SetFillColor(6);
	b[nscifi[j]]->SetLineColor(6);
	b[nscifi[j]]->SetLineWidth(2);
	if(nscifi[j]<32) hitfront = true;
	else if(nscifi[j]<64) hitrear = true;
	cout << " #ev=" << i << ", #ch=" << j << ", npe=" << npe[j] << ", #SciFi=" << nscifi[j] << endl;
	hitfibernum[nmulti] = nscifi[j];
	nmulti++;
	b[nscifi[j]]->Draw();
	//c1->Update();
      }
    }
    if(hit==true){
      c1->Update();
      if(hitrear==true && hitfront==true){
	cout << " --> " << nmulti << " hit(s) + criss cross hit!!" << endl;
	//c1->WaitPrimitive();
      }
      else{
	cout << " --> " << nmulti << " hit(s)." << endl;
      }
      cout << endl;
      c1->WaitPrimitive();
      for(int j=0 ; j<nmulti ; j++){
	b[hitfibernum[j]]->SetFillColor(1);
	b[hitfibernum[j]]->SetLineColor(1);
	b[hitfibernum[j]]->SetLineWidth(1);
	b[hitfibernum[j]]->Draw();
      }
      c1->Update();
      //for(int j=0 ; j<nfiber ; j++){
      //	b[j]->SetFillColor(1);
      //	b[j]->SetLineColor(1);
      //	b[j]->SetLineWidth(1);
      //	b[j]->Draw();
      //}
    }
    //c1->Clear();
  }
  //cout << ent << endl;
}
  
