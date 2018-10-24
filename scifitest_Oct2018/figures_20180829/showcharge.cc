/*
  "showcharge.cc"
  Showing integrated charge spectra over 
  a time range you specified in the macro. 

  Toshiyuki Gogami, November 27, 2017
*/

void showcharge(int runnum = 5009,
		int pflag = 0,
		int outflag = 0)
{
  const int nch = 64; // The number of channels
  pflag = 0;      // Prit flag (you want to print? yes:1) 
  
  // ================================== //
  // ===== General conditions ========= //
  // ================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  // ================================== //
  // ====== Open ROOT file ============ //
  // ================================== //
  char ifilename[500];
  int run = runnum;
  //sprintf(ifilename,"./rootfiles2/acsis_%d.root",run);
  sprintf(ifilename,"./rootfiles2/charge_acsis_%d.root",run);
  TFile* f1 = new TFile(ifilename);
  TTree* t1 = (TTree*)f1->Get("tree");
  double ent = t1->GetEntries();
  double intc[nch];
  //double nent[nch];
  t1->SetBranchAddress("intc", &intc);
  //t1->SetBranchAddress("nent", &nent);
  
  cout << endl;
  cout << " " << f1->GetName() << " is being analyzed..." << endl;
  
  
  TH1F* hist1[nch];
  int xbin = 200;
  //double xmin = 4000.0, xmax = 9000;
  //double xmin = 0.0, xmax = 6000; // after pepestal subtraction
  double xmin = -1000.0, xmax = 9000; // after pepestal subtraction
  for(int i=0 ; i<nch ; i++){
    char hname[500];
    sprintf(hname,"hist1_%d",i);
    hist1[i] = new TH1F(hname,"",xbin,xmin,xmax);
    hist1[i]->GetXaxis()->SetRangeUser(-500.0, 7000.0);
  }

  for(int i=0 ; i<ent ; i++){
    for(int j=0 ; j<nch ; j++){
      intc[j] = -2222.0;
    }
    
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      hist1[j] -> Fill(intc[j]); 
    }
  }
  
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->Divide(4,4);
  for(int i=0 ; i<16 ; i++){
    c1->cd(i+1);
    gPad->SetLogy(1);
    hist1[i]->Draw();
  }
  TCanvas* c2 = new TCanvas("c2","c2");
  c2->Divide(4,4);
  for(int i=0 ; i<16 ; i++){
    c2->cd(i+1);
    gPad->SetLogy(1);
    hist1[i+16]->Draw();
  }
  TCanvas* c3 = new TCanvas("c3","c3");
  c3->Divide(4,4);
  for(int i=0 ; i<16 ; i++){
    c3->cd(i+1);
    gPad->SetLogy(1);
    hist1[i+32]->Draw();
  }
  TCanvas* c4 = new TCanvas("c4","c4");
  c4->Divide(4,4);
  for(int i=0 ; i<16 ; i++){
    c4->cd(i+1);
    gPad->SetLogy(1);
    hist1[i+48]->Draw();
  }
  
  // ================== //
  // ===== Print ====== //
  // ================== //
  if(pflag==1){
    char pname[500];
    sprintf(pname,"charge_acsis_run%d_1.png",run);
    c1->Print(pname);
    sprintf(pname,"charge_acsis_run%d_2.png",run);
    c2->Print(pname);
    sprintf(pname,"charge_acsis_run%d_3.png",run);
    c3->Print(pname);
    sprintf(pname,"charge_acsis_run%d_4.png",run);
    c4->Print(pname);
  }
  
  if(outflag==1){
    char outfname[500];
    sprintf(outfname,"intcHist_acsis_%d.root",run);
    TFile* fnew = new TFile(outfname,"recreate");
    TObjArray histlist;
    for(int i=0 ; i<nch ; i++){
      histlist.Add(hist1[i]);
    }
    histlist.Write();
    fnew->Close();
    cout << " Histograms of integrated charge were saved in " 
	 << outfname << endl;
  }
  
  
}
