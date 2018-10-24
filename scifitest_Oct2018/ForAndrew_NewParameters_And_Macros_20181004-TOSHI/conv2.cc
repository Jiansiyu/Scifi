/*
  "conv2.cc"
  Convertion of ch into NPE.
  
  Toshiyuki Gogami, December 2, 2017
*/

void conv2(int runnum = 46224624,
	   int dataflag = 1)
{
  // ================================= //
  // ===== General conditions ======== //
  // ================================= //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  // =============================== //
  // ======== Open ROOT file ======= //
  // =============================== //
  int flag = dataflag;
  int run = runnum;
  char fname[500];
  sprintf(fname,"./rootfiles2/acsis_%d.root",run);
  TFile* f1 = new TFile(fname);
  TTree* t1 = (TTree*)f1->Get("tree");
  
  // ================================================ //
  // ====== Parameters and variable definitions ===== //
  // ================================================ //
  //const int n = 74;     // The number of data samples (250 MHz sampling = 4 ns per ch)
  const int n = 24;     // The number of data samples (250 MHz sampling = 4 ns per ch)
  const int nch = 64;   // The number of channels (16ch * 2)
  int nn[nch];          // The number of data which are really stored 
  //double time[nch][n];  // Time in ns
  double time[nch];     // Time at peaks in ns
  double wtime[nch];    // Weighted time in ns
  double ph[nch][n];    // Pulse height
  int evID = 0;         // The number of event
  int nscifi[nch];      // Fiber number
  int nent[nch];
  double intc[nch];     // Integrated charge
  double npe[nch];      // NPE
  t1->SetBranchAddress("runID", &run);
  t1->SetBranchAddress("evID",  &evID);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("wtime", &wtime);
  t1->SetBranchAddress("nent",   &nent);
  t1->SetBranchAddress("ph",    &ph);
  t1->SetBranchAddress("nscifi", &nscifi);
  //t1->SetBranchAddress("npe",    &npe);
  t1->SetBranchAddress("intc",   &intc);
  t1->SetBranchAddress("nent",   &nent);
  
  
  char outputfilename[500];
  char tempc[500];
  if(flag==0){
    sprintf(outputfilename,"./rootfiles2/acsis_%d_2.root",run);
  }
  else {
    sprintf(outputfilename,"./rootfiles2/charge_acsis_%d_2.root",run);
  }
  TFile* fnew = new TFile(outputfilename,"recreate");
  TTree* tnew = new TTree("tree","APEX SciFi test at EEL122 in 2018");
  tnew->Branch("runID",  &run,  "runID/I");
  tnew->Branch("evID",   &evID, "evID/I");
  //sprintf(tempc,"nn[%d]/I", nch);         tnew->Branch("nent",  &nn,    tempc);
  sprintf(tempc,"nn[%d]/I", nch);         tnew->Branch("nent",  &nent,    tempc);
  if(flag==0){
    //sprintf(tempc,"time[%d][%d]/D", nch, n);tnew->Branch("time",  &time,  tempc);
    sprintf(tempc,"ph[%d][%d]/D", nch, n);  tnew->Branch("ph",    &ph,    tempc);
  }
  sprintf(tempc,"time[%d]/D",   nch);   tnew->Branch("time",  &time,  tempc);
  sprintf(tempc,"wtime[%d]/D",  nch);   tnew->Branch("wtime", &wtime,  tempc);
  sprintf(tempc,"npe[%d]/D",    nch);   tnew->Branch("npe",  &npe,  tempc);
  sprintf(tempc,"nscifi[%d]/I", nch);   tnew->Branch("nscifi",&nscifi,tempc);
  
  // ========================== //
  // ===== ch-->NPE data ====== //
  // ========================== //
  ifstream* ifs = new ifstream("./singlePE/spe_46224624.dat");
  double temp, ped[nch], spe[nch];
  int cha;
  for(int i=0 ; i<nch ; i++){
    *ifs >> temp >> cha >> ped[i] >> temp >> temp >> temp >> spe[i] >> temp; 
    //cout << ped[i] << " " << spe[i] << endl;
  }
  
  double ent = t1->GetEntries();
  
  for(int i=0 ; i<ent ; i++){
    // -------------------------- //
    // ----- initialization ----- //
    // -------------------------- //
    run = -2;
    evID  = -2;
    for(int j=0 ; j<nch ; j++){
      intc[j]   = -2222.0;
      time[j]   = -2222.0;
      wtime[j]  = -2222.0;
      nscifi[j] = -2;
      nn[j]     = -2;
      npe[j]    = -2222.0;
      for(int k=0 ; k<n ; k++){
	ph[j][k] = -2222.0;
      }
    }
    
    
    t1->GetEntry(i);
    for(int j=0 ; j<nch ; j++){
      npe[j] = (intc[j]-ped[j])/(spe[j]-ped[j]);
      ///if(j==22 || j==29 || j==30) npe[j] = -2222.0; // OFF
      //cout << npe[j] << endl;
    }
    tnew->Fill();
  }
  tnew->Write();
  fnew->Close();
  
  
}
