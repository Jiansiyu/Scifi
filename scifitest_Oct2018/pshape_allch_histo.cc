/*
  pshape.cc
  
  Toshiyuki Gogami, November 27, 2017
*/
void pshape_allch_histo(Int_t runnum=-1, Int_t cflag = 0, Double_t cut = 150, Int_t chan = 0, Double_t peak_time = 50){


  if ( runnum == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x pshape_allch_histo(#run, #cflag = 0, #cut(def=150), #peak_time(def=530))" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << " Goes though all channels for #run, and produces histograms of: " << "\n" << "    c1: hits for each channel" << "\n" << "    c2: x-distribution of hits" << "\n" << "    c3: y-distribution of hits" << "\n" << "\n" << " If cflag = 0 then macro defines 'hit' by pulse in channel having integral above #cut in region around #peak_time. " << "\n" << "If cflag = 1 then macro cuts events based on npe of event being higher than a threshold" << std::endl;
	  return 1;
	}
	else{
	  std::cout << "Do you want description of macro? (y/n)" << std::endl;
	  std::cin >> answer;
	}
	}
    } 


  // ====================================== //
  // ========= General conditions ========= //
  // ====================================== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  //int inte = 1;
  int run = runnum;
  //int ch  = channel;
  
  const int nch = 64;
  //const int n   = 74;
  const int n   = 24;
  //const int n   = 400;

  int evID;
  int nn[nch];
  double time[nch];
  double ph[nch][n];
  Int_t nscifi[nch];
  Double_t npe[nch];
    
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //
  char inputfilename[500];
  sprintf(inputfilename,"./rootfiles2/acsis_%d.root",run);
  TFile* f1 = new TFile(inputfilename);
  TTree* t1 = (TTree*)f1->Get("tree");

  sprintf(inputfilename,"./rootfiles2/charge_acsis_%d_2.root",run);
  TFile* f2 = new TFile(inputfilename);
  TTree* t2 = (TTree*)f2->Get("tree");


  //  t1->SetBranchAddress("evID",  &evID);
  //t1->SetBranchAddress("nent",  &nn);
  t1->SetBranchAddress("time",  &time);
  t1->SetBranchAddress("ph",    &ph);
  t1->SetBranchAddress("nscifi", &nscifi);
  t2->SetBranchAddress("npe", &npe);
  
  // state npe threshold
  Double_t npe_th = 8.0;

  double ent = t1->GetEntries();
 // ------ The number of events analyzed ------


  int xbin = n;
  //double xmin = 0.0, xmax = 800.0;
  double xmin = 0.0, xmax = n*4.0; // 4 ns per ch (250 MHz)
  TH1F* h = new TH1F("h","",xbin,xmin,xmax);
  TH1F* haccum[nch];
  TH1F* htmp[nch];
  TF1* func1[nch];

  // histogram tobe filled with integrals around peak time
  char htitle[500];
  sprintf(htitle,"Integral around %f ns",peak_time);

  TH1* hint = new TH1F("hint",htitle,64,-100,2500);

  TH1* hchan[9];

  



  // added lines to get hitograms vs x and y 


  double phi = 1.0; // phi = 1.0 mm
  double step = 3.79;
  double center = step * (32.-1.) / 2.0; // 121.28/2 mm
  double length = center*2.0; // mm
  length = 121.28;
  // ========================================== //
  // ======= Pedestal position evaluation ===== //
  // ========================================== //
  int nite = 100; // the number of events accumulated
  for(int i=0 ; i<nch ; i++){
    char hname[500];
    sprintf(hname,"haccum_%d",i);
    haccum[i] = new TH1F(hname,"",xbin,xmin,xmax);
    sprintf(hname,"htemp_%d",i);
    htmp[i] = (TH1F*)haccum[i]->Clone(hname);
    htmp[i]->GetYaxis()->SetRangeUser(-50.,0.);
    char fname[500];
    sprintf(fname,"func1_%d",i);
    func1[i]  = new TF1(fname,"[0]",0.0,xmax);
    func1[i]->SetParameter(0, 250.0);
  }
  //TCanvas* c0 = new TCanvas("c0","c0");
  //c0->Divide(8,4);
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
    char fname[500];
    //sprintf(hname,"haccum_%d",i);
    sprintf(fname,"func1_%d",i);
    haccum[i]->Fit(fname,"Nq","",0.0,xmax);
    //c0->cd(i+1);
    //haccum[i]->Draw();
    //func1[i]->Draw("same");
    cout << i << ": " << func1[i]->GetParameter(0) << " "
	 << func1[i]->GetParError(0) << endl;;
  }

  
  
  TCanvas* c1 = new TCanvas("c1","c1");
  //c1->Divide(6,6);
  TCanvas* c2 = new TCanvas("c2","c2");
  //c2->Divide(6,6);
  TCanvas* c3 = new TCanvas("c3","c3");

  TCanvas* c4 = new TCanvas("c4","c4");

  TCanvas* c5 = new TCanvas("c5","c5");

  // add condition for updating canvas
  bool condition = false;
  
  // add counter of no of events
  Int_t no_of_events[nch] = {0};


  TH1* chan_hist = new TH1F("chan_hits","hits in channels",nch,0,nch);
  chan_hist->GetXaxis()->SetTitle("Channel number");

  TH1* pix_hist = new TH1F("chan_hits (pixel)","hits in pixels",nch,0,nch);
  pix_hist->GetXaxis()->SetTitle("Pixel number");
  
  TH1* hist_x = new TH1F("x-hits","X distributiton",nch/2,0,nch/2);
  TH1* hist_y = new TH1F("y-hist","Y distributiton",nch/2,0,nch/2);
  hist_x->GetXaxis()->SetTitle("X");
  hist_y->GetXaxis()->SetTitle("Y");  
  
  // count total hits for each channel
  Int_t total[nch] = {0};

  Int_t hitfiber = -2;
  
  char pname[500];
  double ymax = 100.;
  double ymin = -20.;
  double offset;// = func1[ch]->GetParameter(0);
  
  Double_t small_integral = 0;




  for(int i=0 ; i<ent ; i++){
    t1->GetEntry(i);
    t2->GetEntry(i);
    condition = false;

    
    // for(int k=0 ; k<nch ; k++){

    //   char hname[500]
    // 	if( k == channel){
	  
    // 	  for(n = 0; n <4; n++){
	    
	    
	    
    // 	    hchan[i]->Clone("hint");
	    
    // 	    sprintf(hname,"hchan_%d",i);
	    

    // 	  }
    // 	}
    // }

    for(int k=0 ; k<nch ; k++){
      offset = func1[k]->GetParameter(0);
      condition = false;
      hitfiber = nscifi[k];
      small_integral = 0;
      //    std::cout << "Channel: "<< k << ", hitfiber = nscif[k] = " <<  nscifi[k] << std::endl;

      for(int j=0 ; j<n ; j++){
	h->SetBinContent(j+1,ph[k][j]-offset);
	//h->SetBinContent(j+1, ph[ch][j]-offset);
	//	htmp[k]->SetBinContent(j+1, ph[k][j]-offset);

	//
	// if( ((j*4.) -530.)*( (j*4.) -530.) < 300){
	//   small_integral =+ ph[k][j]-offset;

	// }

       
	// if( ph[k][j]-offset > 80){
	//   condition = true;
	// }
      }

      small_integral = h->Integral( (peak_time/4)-5, (peak_time/4) +5);



      hint->Fill(small_integral);



      // if( small_integral > cut){
      // 	condition = true;
      // }

      // condition based off cuts on integral around peak
      if(cflag == 0){
	if( small_integral > cut){
	  condition = true;
	  total[k]++;
	  std::cout << "this conidtion" << std::endl;
	}
      }

      // condition based on cut on npe: number of photoelectrons
      else if(cflag != 0){
	if(npe[k]>npe_th){
	  condition = true;
	  
	  total[k]++;
	}
      }
      else{
	std::cout << "incorrect flag, cflag must equal 0 (integral-based cut) or 1 (npe based cut) " << std::endl;
      }

      //      std::cout << " small integral is: " << small_integral << std::endl;


      if(condition){
	
	chan_hist->Fill(k);
	pix_hist->Fill(nscifi[k]);
	
	// condition on nscifi rather than k
	
	if(hitfiber<32){

	  // hist_x->Fill(hitfiber*length/32.);
	  hist_x->Fill(hitfiber*1.);
	  std::cout << " x-hit " << std::endl;

	  std::cout << " fill with value of hitfiber*length/32. = " << hitfiber*length/32. << std::endl;

	  std::cout << "hitfiber = " << hitfiber << ", length = " << length << ", nscifi[ch] " << nscifi[k] << ", Channel is  " << k << ", npe = " << npe[k] << ::endl;

	 

	}
	if(hitfiber<64 && hitfiber>31){
	  std::cout << " y-hit " << std::endl;

	  std::cout << "hitfiber = " << hitfiber << ", length = " << length << ", nscifi[ch] " << nscifi[k] << ", Channel is  " << k  << ", npe = " << npe[k] << ::endl;

	  // hist_y->Fill((hitfiber-32)*(length/32));
	  hist_y->Fill((hitfiber-32)*(1));

	 

	} 


	
      }
      ymin = -50.;
      ymax = 250.;
      htmp[k]->SetMinimum(ymin);
      htmp[k]->SetMaximum(ymax);
    }
    //h->SetMinimum(250.);
    //TLine l(0.0,ymin,xmax,ymax);
    //l.Draw("same");
    //cout << ch << ": "  << i+1<< endl;
    //h->Draw();


    //cin >> temp;
    
    sprintf(pname,"pshape_r%d-%d_00-31.png",run,i+1);
    //c1->Print(pname);
    sprintf(pname,"pshape_r%d-%d_32-63.png",run,i+1);
    //c2->Print(pname);
    
    //    cout << i << endl;
  }




  c1->cd(0);
  chan_hist->Draw();

  c2->cd(0);
  pix_hist->Draw();

  c3->cd(0);
  hist_x->Draw();

  c4->cd(0);
  hist_y->Draw();


  c5->cd(0);
  c5->SetLogy();
  hint->Draw();
  
  // bool to say fiber is x or y
  Bool_t x = false;

  for(Int_t k = 0; k<nch; k++){


    if(nscifi[k]<32){
      x = true;
    }
    else{
      x = false;
    }
    
    if (x){
      
      if(total[k] > 100){
	std::cout << "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << "total hits in channel[" << k << "] are " << total[k] << " with fiber number = " <<  nscifi[k] << " and x = " << nscifi[k] + 1   << " (1-32),   (" <<  (nscifi[k])*0.15 << " inches)"<<  "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << std::endl;
      }
      
    }
    else if (!x){
      if(total[k] > 100){
	std::cout << "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << "total hits in channel[" << k << "] are " << total[k] << " with fiber number = " <<  nscifi[k] << " and y = " << nscifi[k] - 32 + 1 << " (1-32),   (" <<  (nscifi[k] - 32)*0.15 << " inches)"  <<  "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << std::endl;
      }
      
      
    }
    
    
  else{
    std::cout << "total[k] = " << total[k] << std::endl;
  }
    

    
  }
  
}
