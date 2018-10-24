/*
  pshape.cc
  
  Toshiyuki Gogami, November 27, 2017
*/

void pshape(int runnum=5035, 
	    int channel=63, 
	    int nite=10){
  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  int run = runnum;
  int ch  = channel;
  
  const int nch = 64;
  //const int n   = 74;
  const int n   = 24;
  //const int n   = 400;

  int evID;
  int nn[nch];
  double time[nch][n];
  double ph[nch][n];
  
  char inputfilename[500];
  sprintf(inputfilename,"./rootfiles2/acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("tree");
  t1->SetBranchAddress("evID",  &evID);
  t1->SetBranchAddress("nent",  &nn);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("ph",    &ph);
  double ent = t1->GetEntries();
  ent = 30;
  //ent = 1;


  int xbin = n;
  //double xmin = 0.0, xmax = 800.0;
  double xmin = 0.0, xmax = n*4.0;
  TH1F* h = new TH1F("h","",
		     xbin,xmin,xmax);
  TH1F* haccum[nch];
  TH1F* htmp[nch];
  TF1* func1[nch];
  //int nite = 100;
  for(int i=0 ; i<nch ; i++){
    char hname[500];
    sprintf(hname,"haccum_%d",i);
    haccum[i] = new TH1F(hname,"",xbin,xmin,xmax);
    sprintf(hname,"htemp_%d",i);
    htmp[i] = (TH1F*)haccum[i]->Clone(hname);
    char fname[500];
    sprintf(fname,"func1_%d",i);
    func1[i]  = new TF1(fname,"[0]",0.0,xmax);
    func1[i]->SetParameter(0, 250.0);
  }
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->Divide(8,4);
  //c0->cd();
  //htmp = (TH1F*)haccum[0]->Clone("htmp");
  for(int i=0 ; i<nite ; i++){
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      for(int k=0 ; k<n ; k++){
	htmp[j]->SetBinContent(k+1,ph[j][k]);
	//htmp[j]->SetBinError(k+1,0.0);
      }
      haccum[j]->Add(htmp[j]);
    }
  }
  for(int i=0 ; i<nch ; i++){
    haccum[i]->Scale(1./nite);
    char hname[500], fname[500];
    sprintf(hname,"haccum_%d",i);
    sprintf(fname,"func1_%d",i);
    haccum[i]->Fit(fname,"Nq","",0.0,xmax);
    c0->cd(i+1);
    //haccum[i]->Draw();
    //func1[i]->Draw("same");
    cout << i << ": " << func1[i]->GetParameter(0) << " "
	 << func1[i]->GetParError(0) << endl;;
  }

  TLine l(0.0,0.0,xmax,0.0);
  
  TCanvas* c1 = new TCanvas("c1","c1");
  double offset = func1[ch]->GetParameter(0);
  for(int i=0 ; i<ent ; i++){
    t1->GetEntry(i);
    for(int j=0 ; j<n ; j++){
      //h->SetBinContent(j+1,ph[ch][j]);
      h->SetBinContent(j+1,
		       ph[ch][j]-offset);
    }
    //h->SetMinimum(250.);
    //h->SetMinimum(-200);
    h->SetMinimum(-1000);
    h->SetMaximum(1000);
    l.Draw("same");
    cout << ch << ": "  << i+1<< endl;
    h->Draw();
    c1->Update();
    c1->WaitPrimitive();
    c1->Clear();
    //cin >> temp;
    
  }
  
  
}

