/*
  ped.cc
  
  create pedestal data 
  Toshiyuki Gogami, November 27, 2017
*/

//void ped(int runnum=5008, int channel=31){
void ped(int runnum=5042){
  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  int channel = 31; // no use
  int run = runnum;
  int ch  = channel;
  
  const int nch = 64; // the number of total channel 
  //const int n   = 74;
  const int n   = 24; // the number of data samples

  int evID;
  int nn[nch];
  double time[nch][n];
  double ph[nch][n];
  
  char inputfilename[500];
  sprintf(inputfilename,"./rootfiles2/JW_acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("tree");
  t1->SetBranchAddress("evID",  &evID);
  //t1->SetBranchAddress("nent",  &nn);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("ph",    &ph);
  double ent = t1->GetEntries();
  ent = 1000;
  //ent = 1;


  int xbin = n;
  //double xmin = 0.0, xmax = 800.0;
  double xmin = 0.0, xmax = (double)n*4.0;
  //double xmin = 0.0, xmax = 400.0;
  TH1F* h = new TH1F("h","",
		     xbin,xmin,xmax);
  h->GetXaxis()->SetTitle("(ns)");
  TH1F* haccum[nch];
  TH1F* htmp[nch];
  TF1* func1[nch];
  double nite = 100;
  double nacc[nch];
  bool ok[nch];
  for(int i=0 ; i<nch ; i++){
    nacc[i] = 0.0;
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
  //TCanvas* c0 = new TCanvas("c0","c0");
  //c0->Divide(8,4);
  //c0->cd();
  for(int i=0 ; i<nite ; i++){
    t1->GetEntry(i);
    
    for(int j=0 ; j<nch ; j++){
      ok[j] = true;
      for(int k=0 ; k<n ; k++){
	htmp[j]->SetBinContent(k+1,ph[j][k]);
	if(ph[j][k]>300.0){
	  ok[j] = false;
	}
	//htmp[j]->SetBinError(k+1,0.0);
      }
      if(ok[j]==true){
	haccum[j]->Add(htmp[j]);
	nacc[j] = nacc[j]+1.0;
      }
    }
  }
  
  // ===================================== //
  // ====== Pedestal pramameters ========= //
  // ===================================== //
  char pedfname[500];
  sprintf(pedfname,"./ped/pedestal_%d.dat",run);
  //  sprintf(pedfname,"./ped/pedestal_%d_testtesttest.dat",run);
  ofstream* ofs = new ofstream(pedfname);
  for(int i=0 ; i<nch ; i++){
    //haccum[i]->Scale(1./nite);
    haccum[i]->Scale(1./nacc[i]);
    char hname[500], fname[500];
    sprintf(hname,"haccum_%d",i);
    sprintf(fname,"func1_%d",i);
    haccum[i]->Fit(fname,"Nq","",0.0,xmax);
    //c0->cd(i+1);
    //haccum[i]->Draw();
    //func1[i]->Draw("same");
//    cout << i << ": " << func1[i]->GetParameter(0) << " "
//	 << func1[i]->GetParError(0) << endl;;
    *ofs  << i << " " << func1[i]->GetParameter(0) << " "
	  << func1[i]->GetParError(0) << endl;;
  }
  ofs->close();

//  TLine l (0.0,0.0,1600.0,0.0);
//  TCanvas* c1 = new TCanvas("c1","c1");
//  double offset = func1[ch]->GetParameter(0);
//  double maxfill = 100;
//  TH1F* h2[200];
//  if (maxfill>ent) maxfill = ent;
//  for(int i=0 ; i<maxfill ; i++){
//    t1->GetEntry(i);
//    char hname[500];
//    sprintf(hname,"h2_%d",i);
//    h2[i] = (TH1F*)h->Clone(hname);
//    for(int j=0 ; j<n ; j++){
//      //h->SetBinContent(j+1,ph[ch][j]);
//      h2[i]->SetBinContent(j+1,
//		       ph[ch][j]-offset);
//      //cout << j+1 << " " << ph[ch][j]-offset << endl;
//    }
//    h2[i]->SetMinimum(-20);
//    //h->SetMinimum(250.);
//    h2[i]->SetMaximum(1000);
//    
//    //cout << ch << ": "  << i+1<< endl;
//    if(i==0) h2[i]->Draw();
//    h2[i]->Draw("same");
//    //c1->Update();
//    //c1->WaitPrimitive();
//    //c1->Clear();
//  }
//  c1->Update();
//  //c1->WaitPrimitive();
//  
//  cout << endl;
//  cout << " File:  " << f1->GetName() << endl;
//  cout << " Width: " << n*4.0  << " ns"<< endl;
//  cout << " Ch:    " << ch << endl;
//  cout << endl;
  
  cout << endl;
  cout << " File:  " << f1->GetName() << endl;
  cout << " -->    dedestal data: " << pedfname << endl;
  
  
}

