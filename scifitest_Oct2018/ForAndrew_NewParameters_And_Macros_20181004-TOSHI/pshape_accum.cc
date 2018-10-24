/*
  pshape.cc
  
  Toshiyuki Gogami, November 27, 2017
*/

void pshape_accum(int runnum=4649, 
		  int channel=0,
		  double maxev=100.0)
{
  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  int run = runnum;
  int ch  = channel;
  int printflag = 0; // Do you want to print?
  
  const int nch = 64;
  //const int n   = 74;
  const int n   = 24;
  //const int n   = 25;

  int evID;
  int nn[nch];
  double time[nch][n];
  double ph[nch][n];
  
  char inputfilename[500];
  sprintf(inputfilename,"./rootfiles2/acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("tree");
  t1->SetBranchAddress("evID",  &evID);
  //t1->SetBranchAddress("nent",  &nn);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("ph",    &ph);
  double ent = t1->GetEntries();
  ent = 1000;
  //ent = 50;
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
  //TCanvas* c0 = new TCanvas("c0","c0");
  //c0->Divide(8,4);
  //c0->cd();
  int thflag[nch];
  double th = 600.0;
  for(int i=0 ; i<nite ; i++){
    for(int j=0 ; j<nch ; j++){
      thflag[j] = 0;
    }
    
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      for(int k=0 ; k<n ; k++){
	htmp[j]->SetBinContent(k+1,ph[j][k]);
	if(ph[j][k]>th) thflag[j] = 1;
	//htmp[j]->SetBinError(k+1,0.0);
      }
      if(thflag[j]==1 && printflag==1){
      	char pname[500];
      	TCanvas* cc = new TCanvas("cc","cc");
      	sprintf(pname,"pshape_%d_ch%d_ev%d.eps",run,j,i);
      	htmp[j]->SetMaximum(5000.0);
      	htmp[j]->GetYaxis()->SetRangeUser(140.0,1400.0);
      	//htmp[j]->GetYaxis()->SetRangeUser(140.0,400.0);
      	htmp[j]->Draw();
      	cc->Print(pname);
      	cc->Clear();
      }
      haccum[j]->Add(htmp[j]);
    }
  }
  
  // ===================================== //
  // ====== Pedestal pramameters ========= //
  // ===================================== //
  //char pedfname[500];
  //sprintf(pedfname,"./ped/pedestal_%d.dat",run);
  //ofstream* ofs = new ofstream(pedfname,"w");
  for(int i=0 ; i<nch ; i++){
    haccum[i]->Scale(1./nite);
    char hname[500], fname[500];
    sprintf(hname,"haccum_%d",i);
    sprintf(fname,"func1_%d",i);
    haccum[i]->Fit(fname,"Nq","",0.0,xmax);
    //c0->cd(i+1);
    //haccum[i]->Draw();
    //func1[i]->Draw("same");
//    cout << i << ": " << func1[i]->GetParameter(0) << " "
//	 << func1[i]->GetParError(0) << endl;;
//    *ofs  << i << ": " << func1[i]->GetParameter(0) << " "
//	  << func1[i]->GetParError(0) << endl;;
  }
  //ofs->close();

  TLine l (0.0,0.0,1600.0,0.0);
  TCanvas* c1 = new TCanvas("c1","c1");
  double offset = func1[ch]->GetParameter(0);
  double maxfill = maxev;
  TH1F* h2[200];
  if (maxfill>ent) maxfill = ent;
  for(int i=0 ; i<maxfill ; i++){
    t1->GetEntry(i);
    char hname[500];
    sprintf(hname,"h2_%d",i);
    h2[i] = (TH1F*)h->Clone(hname);
    for(int j=0 ; j<n ; j++){
      //h->SetBinContent(j+1,ph[ch][j]);
      h2[i]->SetBinContent(j+1,
		       ph[ch][j]-offset);
      //cout << j+1 << " " << ph[ch][j]-offset << endl;
    }
    h2[i]->SetMinimum(-100);
    //h->SetMinimum(250.);
    h2[i]->SetMaximum(2000);
    
    //cout << ch << ": "  << i+1<< endl;
    if(i==0) h2[i]->Draw();
    h2[i]->Draw("same");
    //c1->Update();
    //c1->WaitPrimitive();
    //c1->Clear();
  }
  c1->Update();
  //c1->WaitPrimitive();
  
  cout << endl;
  cout << " File:  " << f1->GetName() << endl;
  cout << " Width: " << n*4.0  << " ns"<< endl;
  cout << " Ch:    " << ch << endl;
  cout << endl;
  
  
}

