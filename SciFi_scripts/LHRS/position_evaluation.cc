//#include "mapping.hh"

bool fexists(const char *filename); 

void position_evaluation(Int_t runnum=-1){ 


  if ( runnum == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x position_evaluation.cc(#runnumber )" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;
    
    while(true){
      
      if(answer[0] == 'n'){
  	  return 1;
      }
      else if(answer[0] == 'y'){
	  
  	std::cout << "Loops over all channels and  " <<"\n" <<  std::endl;
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


  int run = runnum;

  
  const int nch = 64; // number of channels
  const int n   = 35; // length of pulse window (n*4ns)

  Double_t ph[nch][n]; // pulse height
    
  // ========================== //
  // ==== Open ROOT file ====== //
  // ========================== //
  char ifilename[500];

  sprintf(ifilename,"/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_%d.root",run);
 
  TFile* f1 = new TFile(ifilename);
  TTree* t1 = (TTree*)f1->Get("T");

  std::cout << " achieved this step! " << std::endl;


  // creating array of mapping values from order of output in analyzer to 

  //  Double_t nscifi[nch] = {0};

  //  mapping(nscifi);
  
 

  Double_t ent = t1->GetEntries();
 // // ------ The number of events analyzed ------

  char name[500];

  // set tree branch address to the 64 raw outputs from teh SciFi detector

  for (Int_t chan = 0; chan<nch; chan++){
        
    if( chan < 10){
      sprintf(name,"L.sf.a_raw0%d",chan);
    }
    else{
      sprintf(name,"L.sf.a_raw%d",chan);
    }
    
    t1->SetBranchAddress(name,&ph[chan]);
  }


  //////////////////////////////////////
  //////// Read in pedestal values /////
  //////////////////////////////////////

  // This is done so pulses in each channel can be compared to respective pedestals for that channel


  Double_t ped[nch];
  Double_t RMS[nch];
  Double_t holder = 0;

  char fname[500];
  sprintf(fname,"./ped_integrals/pedestal_%d.dat",runnum);
  

  cout << fname << endl;

  // check if pedestal file exists
  if ( ! fexists(fname)){
    cout << " Error, pedestal file not found " << endl;
    return 1;
  }
 
  

  // create placeholder values in case pedestal file is not read properly
  for (Int_t j=0; j<nch; j++){
    
    ped[j] = -2000;
    RMS[j] = -2000;
  }



  // ifstream for reading pedestal data
  std::ifstream* ifs = new ifstream(fname);

  for (Int_t j=0; j<nch; j++){
        
    
    *ifs >> holder >> ped[j] >> RMS[j];

    // stops zero pedestal (failed fitting) from making whole end plot look strange
    if( ped[j] == 0){
      ped[j] = ped[j-1]*10;
      cout << "channel " << j << " has pedestal of 0, using value of pedestal for previous channel: " << ped[j-1]*10 << endl;
    }

  }




  // setting up histograms for x and y positons of 'hits'

  TH1* hist_x = new TH1F("x-hits","X distributiton",nch/2,0,nch/2);
  TH1* hist_y = new TH1F("y-hist","Y distributiton",nch/2,0,nch/2);

  Int_t total = 0;

  for( Int_t i = 0; i<ent; i++){
    
    t1->GetEntry(i);

    for( Int_t j = 0; j<nch; j++){
      
      total = 0;   // resetting total for each channel
      for( Int_t k = 8; k<15; k++){ 
	// only including interval around where peaks should be (32-64 ns)
	total += ph[j][k];
      }

      // if total in event is above a certain threshold then record as 'hit' 
      
      
      if(total > (ped[j] + 0.1*(ped[j])) ){
	
	//	cout << "Passed, j = " << j << " and total = " << total << endl;
	
	if(j < 32){ // x 'hit'
	  hist_x->Fill(j);
	}
	else if( j >= 32){// y 'hit'
	  hist_y->Fill(j-32);
	}
	  
      }	 
      else{
	//	cout << "failed, j= " << j << " and total = " << total << endl;
      }
    }
  }


  
  TCanvas* c1 = new TCanvas("c1","c1");
  
  TCanvas* c2 = new TCanvas("c2","c2");
  

  c1->cd(0);
  hist_x->Draw();

  c2->cd(0);
  hist_y->Draw();
	










 //  // added lines to get hitograms vs x and y 

 //  double phi = 1.0; // phi = 1.0 mm
 //  double step = 3.79;
 //  double center = step * (32.-1.) / 2.0; // 121.28/2 mm
 //  double length = center*2.0; // mm
 //  length = 121.28;
 //  // ========================================== //
 //  // ======= Pedestal position evaluation ===== //
 //  // ========================================== //


 //  int nite = 100; // the number of events accumulated

 //  for(int i=0 ; i<nch ; i++){
 //    char hname[500];
 //    sprintf(hname,"haccum_%d",i);
 //    haccum[i] = new TH1F(hname,"",xbin,xmin,xmax);
 //    sprintf(hname,"htemp_%d",i);
 //    htmp[i] = (TH1F*)haccum[i]->Clone(hname);
 //    htmp[i]->GetYaxis()->SetRangeUser(-50.,0.);
 //    char fname[500];
 //    sprintf(fname,"func1_%d",i);
 //    func1[i]  = new TF1(fname,"[0]",0.0,xmax);
 //    func1[i]->SetParameter(0, 250.0);
 //    func1[i]->SetParLimits(0,100.0,420.0);
 //  }

 //  //TCanvas* c0 = new TCanvas("c0","c0");
 //  //c0->Divide(8,4);
 //  //c0->cd();
 //  //htmp = (TH1F*)haccum[0]->Clone("htmp");

 // // std::cout << " created first histograms  " << std::endl;
 // //  for(int i=0 ; i<nite ; i++){
 // //    t1->GetEntry(i);
 // //    for(int j=0 ; j<nch ; j++){
 // //      for(int k=0 ; k<n ; k++){
 // // 	htmp[j]->SetBinContent(k+1,ph[j][k]);
 // // 	//htmp[j]->SetBinError(k+1,0.0);
 // //      }
 // //      haccum[j]->Add(htmp[j]);
 // //    }
 // //  }
 // //  std::cout << " created first histograms  " << std::endl;
 

 //  // for(int i=0 ; i<nch ; i++){
 //  //   haccum[i]->Scale(1./nite);
 //  //   char fname[500];
 //  //   //sprintf(hname,"haccum_%d",i);
 //  //   sprintf(fname,"func1_%d",i);
 //  //   haccum[i]->Fit(fname,"Nq","",0.0,xmax);
 //  //   //c0->cd(i+1);
 //  //   //haccum[i]->Draw();
 //  //   //func1[i]->Draw("same");
 //  //   cout << i << ": " << func1[i]->GetParameter(0) << " "
 //  // 	 << func1[i]->GetParError(0) << endl;;
 //  // }
  
 //  // ======================================== //
 //  // === alternative way to get pedestal ==== //
 //  // ======================================== //

 //  char chmapdata[500];
 //  char peddata[500];
 //  sprintf(chmapdata,"ped_integrals/pedestal_%d.dat",111890);
 //  ifstream* dragon_ped = new ifstream(chmapdata);
 //  double pedch[nch];
 //  double RMSch[nch];
 //  double ttemp2;
 //  for(int i=0 ; i<nch ; i++){
 //    *dragon_ped >> ttemp2 >> pedch[i] >> RMSch[i];
 //    pedch[i] = pedch[i]/55.0;
 //    //cout << pedch[i] << endl;
 //    //pedch[i] = 0.0;
 //  }
 //  dragon_ped->close();
 //  std::cout << "I read the ped dat file!" << std::endl;

  
 //   std::cout << " created first histograms  " << std::endl;
 //  TCanvas* c1 = new TCanvas("c1","c1");
 //  //c1->Divide(6,6);
 //  TCanvas* c2 = new TCanvas("c2","c2");
 //  //c2->Divide(6,6);
 //  TCanvas* c3 = new TCanvas("c3","c3");

 //  TCanvas* c4 = new TCanvas("c4","c4");

 //  TCanvas* c5 = new TCanvas("c5","c5");

 //  TCanvas* c6 = new TCanvas("c6","c6");


 //  std::cout << " created first canvases " << std::endl;

 //  // add condition for updating canvas
 //  bool condition = false;
  
 //  // add counter of no of events
 //  Int_t no_of_events[nch] = {0};


 //  TH1* chan_hist = new TH1F("chan_hits","hits in fadc channels",nch,0,nch);
 //  chan_hist->GetXaxis()->SetTitle("Channel number");

 //  TH1* pix_hist = new TH1F("chan_hits (pixel)","hits in fibres (0-63)",nch,0,nch);
 //  pix_hist->GetXaxis()->SetTitle("Fibre number");
  
 //  TH1* hist_x = new TH1F("x-hits","X distributiton",nch/2,0,nch/2);
 //  TH1* hist_y = new TH1F("y-hist","Y distributiton",nch/2,0,nch/2);
 //  hist_x->GetXaxis()->SetTitle("X");
 //  hist_y->GetXaxis()->SetTitle("Y");  
  
 //  // count total hits for each channel
 //  Int_t total[nch] = {0};

 //  Int_t hitfiber = -2;
  
 //  char pname[500];
 //  double ymax = 100.;
 //  double ymin = -20.;
 //  double offset[nch] = {0};// = func1[ch]->GetParameter(0);
  
 //  Double_t small_integral = 0;

 //  Bool_t initialised = false;

 //  Int_t c6_hit = 0;
 //  Int_t overall_hit = 0;

 //  Int_t x_y_conv = 0;
 //  char x_or_y = {0};
   

 //  for(int i=0 ; i<ent ; i++){
 //    t1->GetEntry(i);
 //    // t2->GetEntry(i);
 //    condition = false;

    
 //    for(int k=0 ; k<nch ; k++){

 //      cout << " nch = << " << k << endl;

 //      char hname[500];
 //    	if( nscifi[k] + 1 == fibre && initialised == false){
	  
 //    	  for(Int_t z = 0; z <4; z++){
	    	 
 // 	    if ( nscifi[k] <= 31 && nscifi[k] >= 0 ){
 // 	      x_y_conv = 0;
 // 	      x_or_y = 'x';
 // 	    }
 // 	    else if ( nscifi[k] >= 32 && nscifi[k] <= 63){
 // 	      x_y_conv = 32;
 // 	      x_or_y = 'y';
 // 	    }
	      


 //    	    sprintf(hname,"hfibre_%d | %c", nscifi[k] + n - 4 - x_y_conv + 1,  x_or_y );
 //    	    hchan[z] = (TH1F*)hint->Clone(hname);
 // 	    hchan[z]->Reset("ICESM");
 // 	    hchan[z]->SetTitle(hname);	    
	    	    
 // 	    sprintf(hname,"hfibre_%d | %c", nscifi[k] + n  + 1 - x_y_conv + 1,  x_or_y);
 //   	    hchan[z+5] = (TH1F*)hint->Clone(hname);
 // 	    hchan[z+5]->Reset("ICESM");
 // 	    hchan[z+5]->SetTitle(hname);
 //    	  }
	  
 // 	  sprintf(hname,"hfibre_%d | %c", nscifi[k]  - x_y_conv + 1, x_or_y);
 // 	  hchan[4] = (TH1F*)hint->Clone(hname);
 // 	  hchan[4]->Reset("ICESM");
 // 	  hchan[4]->SetTitle(hname);

 // 	  // deal with actual channel plot here
 // 	  initialised = true;

 //    	}
	
	
	
 //    }

 //    for(int k=0 ; k<nch ; k++){
 //      offset[k] = func1[k]->GetParameter(0);
 //      condition = false;
 //      hitfiber = nscifi[k];
 //      small_integral = 0;
 //      //    std::cout << "Channel: "<< k << ", hitfiber = nscif[k] = " <<  nscifi[k] << std::endl;
 
 //      for(int j=0 ; j<n ; j++){


 // 	//	h->SetBinContent(j+1,ph[k][j]-offset[k]);
 // 	h->SetBinContent(j+1,ph[k][j]-pedch[k]);

       

 // 	//h->SetBinContent(j+1, ph[ch][j]-offset);
 // 	//	htmp[k]->SetBinContent(j+1, ph[k][j]-offset);

 // 	//
 // 	// if( ((j*4.) -530.)*( (j*4.) -530.) < 300){
 // 	//   small_integral =+ ph[k][j]-offset;

 // 	// }

       
 // 	// if( ph[k][j]-offset > 80){
 // 	//   condition = true;
 // 	// }
 //      }

 //      small_integral = (h->Integral( (peak_time/4)-5, (peak_time/4) +5));


 //      Int_t diff =  fibre - (hitfiber + 1 );

 //      // JW: line above as hitfiber goes from 0-63, 
      
 //      //      std::cout << "diff = " << diff << std::endl;

 //      if( (diff)*(diff) < 17){
	
 //      	// Int_t diff =  nscifi[chan] -hitfiber; // 
 // 	hchan[4 - diff]->Fill(small_integral);
 // 	//	hchan[4 + diff]->Fill(100* diff);

 // 	// 	std::cout << "Codition passes: small int = " << small_integral << "\n" << " and 4 + diff = " << diff + 4 << std::endl;


 // 	//	c6_hit++;
 //      }

 //      //      overall_hit++;

 //      hint->Fill(small_integral);
	  
	  
	  
 //      // if( small_integral > cut){
 //      // 	condition = true;
 //      // }

 //      // condition based off cuts on integral around peak
 //      if(cflag == 0){
 // 	if( small_integral > cut){
 // 	  condition = true;
 // 	  total[k]++;
 // 	  //	  std::cout << "this conidtion" << std::endl;
 // 	}
 //      }

 //      // condition based on cut on npe: number of photoelectrons
 //      else if(cflag != 0){
 // 	// if(npe[k]>npe_th){
 // 	//   condition = true;
	  
 // 	//   total[k]++;
 // 	// }
 //      }
 //      else{
 // 	std::cout << "incorrect flag, cflag must equal 0 (integral-based cut) or 1 (npe based cut) " << std::endl;
 //      }

 //      //      std::cout << " small integral is: " << small_integral << std::endl;


 //      if(condition){
	
 // 	chan_hist->Fill(k);
 // 	pix_hist->Fill(nscifi[k]+ 1);
	
 // 	// condition on nscifi rather than k
	
 // 	if(hitfiber<32){

 // 	  // hist_x->Fill(hitfiber*length/32.);
 // 	  hist_x->Fill(hitfiber*1.+1);
 // 	  //	  std::cout << " x-hit " << std::endl;

 // 	  //	  std::cout << " fill with value of hitfiber*length/32. = " << hitfiber*length/32. << std::endl;

 // 	  //	  std::cout << "hitfiber = " << hitfiber << ", length = " << length << ", nscifi[ch] " << nscifi[k] << ", Channel is  " << k << ", npe = " << "blah"  << std::endl;

	 

 // 	}
 // 	if(hitfiber<64 && hitfiber>31){
 // 	  //	  std::cout << " y-hit " << std::endl;

 // 	  //	  std::cout << "hitfiber = " << hitfiber << ", length = " << length << ", nscifi[ch] " << nscifi[k] << ", Channel is  " << k  << ", npe = " << "blah" << ::endl;

 // 	  // hist_y->Fill((hitfiber-32)*(length/32));
 // 	  hist_y->Fill((hitfiber-32)*(1)+1);

	 

 // 	} 

 // 	if( (diff)*(diff) < 17){
 // 	  c6_hit++;
 //      //      overall_hit++;
 // 	}
 // 	else{
 // 	  overall_hit++;
 // 	}
	
 //      }
 //      ymin = -50.;
 //      ymax = 250.;
 //      htmp[k]->SetMinimum(ymin);
 //      htmp[k]->SetMaximum(ymax);
 //    }
 //    //h->SetMinimum(250.);
 //    //TLine l(0.0,ymin,xmax,ymax);
 //    //l.Draw("same");
 //    //cout << ch << ": "  << i+1<< endl;
 //    //h->Draw();


 //    //cin >> temp;
    
 //    sprintf(pname,"pshape_r%d-%d_00-31.png",run,i+1);
 //    //c1->Print(pname);
 //    sprintf(pname,"pshape_r%d-%d_32-63.png",run,i+1);
 //    //c2->Print(pname);
    
 //    //    cout << i << endl;
 //  }




 //  c1->cd(0);
 //  chan_hist->Draw();

 //  c2->cd(0);
 //  pix_hist->Draw();

 //  c3->cd(0);
 //  hist_x->Draw();

 //  c4->cd(0);
 //  hist_y->Draw();


 //  c5->cd(0);
 //  c5->SetLogy();
 //  hint->Draw();
  
 //  c6->Divide(3,3);
 //  c6->SetLogy();
  
 //  // c6->cd(0);
 //  // hchan[0]->Draw();
 //  // c6->cd(1);
 //  // hchan[1]->Draw();

 //  //  TPad *p1 = c1->cd(1); p1->SetLogy(); 

 //  for(Int_t m = 0; m<9; m++){
 //    //  	c6->cd(m+1);
 // 	pads[m] = c6->cd(m+1);
 // 	pads[m]->SetLogy();

 //  	hchan[m]->Draw();



   // for(Int_t n = 0; n<3; n++){
  //     for(Int_t m = 0; m<3; m++){
  // 	c6->cd(n*3 + m);
  // 	hchan[n*3 + m]->Draw();
  //     }
  // }


  




  // bool to say fiber is x or y
  // Bool_t x = false;

  // for(Int_t k = 0; k<nch; k++){


  //   if(nscifi[k]<32){
  //     x = true;
  //   }
  //   else{
  //     x = false;
  //   }
    
  //   if (x){
      
  //     if(total[k] > 100){
  // 	//	std::cout << "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << "total hits in channel[" << k << "] are " << total[k] << " with fibre number = " <<  nscifi[k] + 1 << " and x = " << nscifi[k] + 1   << " (1-32),   (" <<  (nscifi[k])*0.15 << " inches)" << " and offset = " << offset[k] <<  "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << std::endl;
  //     }
      
  //   }
  //   else if (!x){
  //     if(total[k] > 100){
  // 	//	std::cout << "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << "total hits in channel[" << k << "] are " << total[k] << " with fibre number = " <<  nscifi[k] + 1 << " and y = " << nscifi[k] - 32 + 1 << " (1-32),   (" <<  (nscifi[k] - 32)*0.15 << " inches)"  << " and offset = " << offset[k] << "\n" << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n" << std::endl;
  //     }
      
      
  //   }
    
    

  //   std::cout << "For channel " << k << " with total[k] = " << total[k] <<  ", fibre number = " <<  nscifi[k] + 1 << " with offset[k] = " << offset[k] << std::endl;



        
  // }

  //   std::cout << "ratio of c6 hits to overall = " << Double_t(c6_hit)/overall_hit << std::endl; 

  
}


bool fexists(const char *filename){
  std::ifstream ifile(filename);
  return (bool)ifile;
}
