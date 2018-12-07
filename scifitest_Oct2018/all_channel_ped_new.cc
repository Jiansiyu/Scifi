void all_channel_ped_new(Int_t runnum=-1, int pflag = 0, int ref_run = 5049, Int_t Hallflag = 0){

  Int_t right_or_left, run_number;    
  //  cout << "enter run_number: ";
  //  cin >> run_number;

  run_number = runnum;

  TChain *T = new TChain("T");
//  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("./raw_test_Rootfiles/acsis_%d.root",run_number));

  T->Add(Form("/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_%d.root",run_number));



  //  sprintf(inputfilename,"./raw_test_Rootfiles/acsis_%d.root",run);

  const int nch = 64;
  

  std::cout << T->GetEntries() << std::endl;
  
  Double_t min, max, temp;
  Double_t ped_val,ped_wid;
  Int_t max_bin;
  Int_t i, ii;
  Double_t ped[nch],ped_width[nch];
  Double_t gain[nch];

  TCut tritype = "";//"((DR.evtypebits>>1)&1)";

  TCanvas* cped[6];
  char ctitle[500];
  //  char cname[500];


  for(Int_t j = 0; j<6; j++){
    sprintf(ctitle,"cpeds_%d",j);
    cped[j] = new TCanvas(ctitle,ctitle,1200,1200);
    cped[j]->Divide(4,3); 
    // cped[j].SetTitle(ctitle);
    std::cout << "~~~~~~~~~~ " << j  << std::endl;
  }
 
  // TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  // TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  // TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  // TCanvas *c2 = new TCanvas("c2","c2",1200,1200);


  TH1F *tt1 = new TH1F("tt1",Form("Ped%d",1),400,100,500);

  //  c1->Divide(4,3); 


  TF1* func1[nch];
  TF1* func2[nch];

  char fname[500];

  Int_t c_select = 0;
  Int_t c_remainder = 1;

  for(Int_t k=0; k<nch; k++){

    tt1->Reset(); /// fix 

    c_select = Int_t( floor((k / 12.0)));
    c_remainder = Int_t( (k%12) +1);

    cout << "c_select = " << c_select << endl;
    cout << "c_remainder = " << c_remainder << endl;


    cped[c_select]->cd(c_remainder);

//     //  TH1F *tt1 = new TH1F("tt1",Form("Ped%d",i+1),300,300,630);

    cout << " check 1 " << endl;

    if(k<10){
      T->Draw(Form("R.sf.a_raw0%d>>tt1",k));
    }
    else{
      T->Draw(Form("R.sf.a_raw%d>>tt1",k));
    }
    max_bin = tt1->GetMaximumBin();

    std::cout << " ~~~~~~~~~~ " << max_bin << " ~~~~~~~~~~~~" << std::endl;

    min = tt1->GetBinCenter(max_bin) - 40;
    max = tt1->GetBinCenter(max_bin) + 40;


    sprintf(fname,"func1_%d",k);
    func1[k] = new TF1(fname,"gaus");

    tt1->Fit(fname,"NQ","",min,max);
    min = func1[k]->GetParameter(1) - 2*func1[k]->GetParameter(2);
    max = func1[k]->GetParameter(1) + 2*func1[k]->GetParameter(2);

    std::cout << "++++++++++++++++ " <<  func1[k]->GetParameter(1)  << "++++++++++++++" << std::endl;
    

    sprintf(fname,"func2_%d",k);
    func2[k] = new TF1(fname,"gaus");


    tt1->Fit(fname,"Q","",min,max);

//    delete tt1;
    ped_val = func2[k]->GetParameter(1);
    ped_wid = func2[k]->GetParameter(2);    

    std::cout << "---------------- " <<  ped_val  << "--------------" << std::endl;
 

    ped[k] = ped_val * 55;
    ped_width[k] = ped_wid; 

    // std::cout << k << "  " << ped[k] << std::endl;

  }



  // Read pedestal values into file

  char pedfname[500];
  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",run_number);
  //  sprintf(pedfname,"./ped/pedestal_%d_testtesttest.dat",run);
  ofstream* ofs = new ofstream(pedfname);
    
    
  for(Int_t k=0; k<nch; k++){
    std::cout << k << "  " << ped[k] << std::endl;

    *ofs  << k << " " << ped[k] << " " <<  ped_width[k]  << endl;;
  }
  std::cout << " CCCCCCCCCCCCCCCCCCCCCCCCCC CHECK CCCCCCCCCCC " << std::endl;

  ofs->close();

 }

