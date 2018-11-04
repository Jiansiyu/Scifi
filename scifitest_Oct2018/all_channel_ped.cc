/*
  ped.cc
  
  create pedestal data 
  Toshiyuki Gogami, November 27, 2017
*/

//void ped(int runnum=5008, int channel=31){
void all_channel_ped(int runnum=-1, int pflag = 0, int ref_run = 5049){
  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //



  if ( runnum == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x all_channel_ped.cc(#run, #pflag(def = 0), #ref_run(def=5049))" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "\n" << "Script for a specific run (#runnum) that produces a file with the pedestals for each channel -> ped_integrals/pedestal_#run.dat"<<  "\n" << "\n" << "If pflag is set to 0 then The RMS values of the pedestals for each channel are plotted (c2) as well as the difference in pedestal compared to a reference pedestal (#ref_run) (c1)" << "\n" << "\n" << "For fuller description read description of single_ch_amp.cc, this macro uses the same method of hisogramming the integrals of raw signals (no pedestal cuts) and then fitting this histogram and taking the mean as the pedestal, and the RMS as the width" << std::endl;
	return 1;
	}
	else{
	  std::cout << "Do you want description of macro? (y/n)" << std::endl;
	  std::cin >> answer;
	}
	}
    } 



  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);


  int run = runnum;

  
  const int nch = 64; // the number of total channel 
  //const int n   = 74;
  const int n   = 400; // the number of data samples

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
  //  ent = 1000;
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
  TH1F* hintegrals[nch];
  TH1F* h_RMScheck;
  TH1F* h_pedcomp;

  TF1* func1[nch];
  TF1* func2[nch];
  double nite = 100;
  double nacc[nch];
  bool ok[nch];

  Double_t int_ch = -100;


  // Form a series of histograms and fitting functions eor each channel

  //params for histogram of signal amplitudes

  Double_t ixmin = 3000*(n/24), ixmax = 8000*(n/24);


  


  for(int i=0 ; i<nch ; i++){
    nacc[i] = 0.0;
    char hname[500];
    sprintf(hname,"haccum_%d",i);
    haccum[i] = new TH1F(hname,"",xbin,xmin,xmax);
    sprintf(hname,"htemp_%d",i);
    htmp[i] = (TH1F*)haccum[i]->Clone(hname);
    char fname[500];
    sprintf(fname,"func1_%d",i);
    func1[i]  = new TF1(fname,"gaus",ixmin,ixmax);
    
    char ftitle[500];
    sprintf(fname,"Integrals_%d",i);
    sprintf(ftitle,"Integrals (channel_number_%d)",i);
    hintegrals[i] = new TH1F(fname,ftitle,ixmax-ixmin,ixmin,ixmax);
    hintegrals[i]->GetXaxis()->SetTitle("Integrated amplitude");


  }
  //TCanvas* c0 = new TCanvas("c0","c0");
  //c0->Divide(8,4);
  //c0->cd();
  char pedfname[500];
  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",run);
  //  sprintf(pedfname,"./ped/pedestal_%d_testtesttest.dat",run);
  ofstream* ofs = new ofstream(pedfname);

  
  // *ofs  << "Channel_number "<< "||| " << "Pedestal value" << "||| "
  //	<< "RMS of ped " << endl;;




  for(int i=0 ; i<ent  ; i++){
    t1->GetEntry(i);


    
    for(int j=0 ; j<nch ; j++){

      haccum[j]->Reset("ICESM");
      ok[j] = true;
      for(int k=0 ; k<n ; k++){
	htmp[j]->SetBinContent(k+1,ph[j][k]);
	//	if(ph[j][k]>300.0){
	//ok[j] = false;
	//}       option to have cut on large signals (to not put in pedestal
	//htmp[j]->SetBinError(k+1,0.0);
      }
      //      if(ok[j]==true){
      haccum[j]->Add(htmp[j]);
      nacc[j] = nacc[j]+1.0;
	//    }


      int_ch = haccum[j]->Integral(xmin,xmax);
      hintegrals[j]->Fill(int_ch); 

    }

  }
  
  // ===================================== //
  // ====== Pedestal pramameters ========= //
  // ===================================== //
  // char pedfname[500];
  //  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",run);
  //  sprintf(pedfname,"./ped/pedestal_%d_testtesttest.dat",run);
  //  ofstream* ofs = new ofstream(pedfname);


  // crearte hist of RMS for all channels
  char ftitle[500];
  sprintf(ftitle,"RMS comparison of all channels (run%d)",run);
  h_RMScheck =  new TH1F("h_RMScheck",ftitle,64,0,64);
  h_RMScheck->GetXaxis()->SetTitle("Channel number");
  h_RMScheck->GetYaxis()->SetTitle("RMS");

  // create hist of reference pedestals - pedestal for all channels

  sprintf(ftitle,"Pedestals for run %d compared to run %d",run, ref_run);
  h_pedcomp =  new TH1F("h_pedcomp",ftitle,64,0,64);
  h_pedcomp->GetXaxis()->SetTitle("Channel number");
  h_pedcomp->GetYaxis()->SetTitle("ref ped - new ped");


  // read in chosen run pedestals (to compare against new


  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",ref_run);
  ifstream* ifs = new ifstream(pedfname);

  // rename for later cout statement
  sprintf(pedfname,"./ped_integrals/pedestal_%d.dat",run);

  // values to store info from new ped
  Double_t holder = -200;
  Double_t iped[nch] = {0};
  Double_t iRMS[nch] = {0};


  /*  Int_t counter = 0;

  while(counter < nch){
    //haccum[i]->Scale(1./nite);
    //    haccum[i]->Scale(1./nacc[i]);
  *ifs >> holder >> iped[counter] >> iRMS[counter];
  counter++;

  }
  */


  for(int i=0 ; i<nch ; i++){
    //haccum[i]->Scale(1./nite);
    //    haccum[i]->Scale(1./nacc[i]);


    char hname[500], fname[500];
    //    sprintf(hname,"haccum_%d",i);
    sprintf(fname,"func1_%d",i);
    hintegrals[i]->Fit(fname,"0R+");
    Double_t mean_1 = func1[i]->GetParameter(1);
    Double_t sd_1 = func1[i]->GetParameter(2);

    // define second function based on first fit parameters
    sprintf(fname,"func2_%d",i);
    func2[i] =  new TF1(fname, "gaus", (mean_1-2*sd_1), mean_1 + 2*sd_1);

    hintegrals[i]->Fit(fname,"0R+");
    
    Double_t mean_2 = func2[i]->GetParameter(1);
    Double_t sd_2 = func2[i]->GetParameter(2);

  
    
    //c0->cd(i+1);
    //haccum[i]->Draw();
    //func1[i]->Draw("same");
//    cout << i << ": " << func1[i]->GetParameter(0) << " "
//	 << func1[i]->GetParError(0) << endl;;
    *ofs  << i << " " << mean_2 << " "
	  << sd_2  << endl;;


    *ifs >> holder >> iped[i] >> iRMS[i];
  
    // add lines to round sd_2 to 4 decimal places (to match ped file)
  
    h_RMScheck->SetBinContent(i+1,roundf(sd_2*100)/100 );
    h_pedcomp->SetBinContent(i+1, roundf(iped[i]*100)/100 -  roundf(mean_2*100)/100 );


  }
  ofs->close();



  if( pflag == 0){


    TCanvas* c1 = new TCanvas("c1","c1");
    TCanvas* c2 = new TCanvas("c2","c2");
    //  TCanvas* c3 = new TCanvas("c3","c3");
    
    c1->cd();
    h_pedcomp->Draw();  
    
    c2->cd();
    h_RMScheck->Draw();

  }

  //  c3->cd(1);
  //  h_pedcomp->Draw();


  // c1->Divide(2,1);

  // c1->cd(1);
  // hintegrals[10]->Draw();

  // c1->cd(2);
  // func2[10]->Draw();
  

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
  cout << " -->    pedestal data: " << pedfname << endl;
  
  
}

