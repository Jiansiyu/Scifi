/*

  channel_ped_track.cc

  John Williamson, October 18th, 2018

*/

// function to check if file exists (defined at bottom)

// look at pshape for how to read tree
// need to use getentry(i) etc



bool fexists(const char *filename); 


void channel_ped_track(Int_t run = -1, Int_t channum = 10, Double_t events = 10001, Int_t m_flag = 1){


  if ( run == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x channel_ped_track.cc(#run, #channum(def=0), #events(def=10000), m_flag )" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "\n" <<"  Macro goes through chosen run and channel and caluclates pedestal for every 1000 bins, it then plots the event number against pedestal. " << "\n" << " The number of events gone through is controlled by input parameter #events."  << "\n" << "if m_flag = 0 (true) then only middle 5 bins of each pulse are used to find integrals/ pedestals" <<  std::endl;
	return 1;
	}
	else{
	  std::cout << "Do you want description of macro? (y/n)" << std::endl;
	  std::cin >> answer;
	}
	}
  } 


  // ================================ //
  // ===== General conditions ======= //
  // ================================ //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  

  //=====================================================//
  //====== Define Pedestal variables and histograms =====//
  //=====================================================//

  const int nch = 64;


  Double_t pedestals[nch];
  Double_t RMS[nch];
  Double_t chan[nch];
  Double_t temp;
    
  Double_t ped_tot[nch];
  Double_t RMS_tot[nch];
  
  Double_t ped_mean[nch];
  Double_t RMS_mean[nch];

  

  // TH1F* h_peds[nch];
  TH1F* h_ped_m[nch];
  TH1F* h_raw[nch];
  TH2F* h_raw2D[nch];
  //  TH1F* h_peds[nch];
  
  TH1F* h_ave[nch];

  TF1* func1[nch];
  


  TGraph* gr1_m[nch];
  TGraph* gr1_m2[nch];

  // check if run exists

  char fname[500];
  sprintf(fname,"./rootfiles2/acsis_%d.root",run);    

  if (! fexists(fname) ){
    
    cout << "Root file: " << fname << " could'nt be found!" << std::endl;

  }

  const int n = 24; // number of data samples
  double ph[nch][n]; // amplitude in each sample
  

  TFile* f1 = new TFile(fname);
  TTree* t1 = (TTree*)f1->Get("tree");
  double ent = t1->GetEntries();
  //  ent = 10001;
  ent = events;

  //set up tree branches (to read in)

  double intc_raw[nch];     // Integrated raw charge
  int evID = 0;         // The number of event


  t1->SetBranchAddress("intc_raw",  &intc_raw);
  t1->SetBranchAddress("evID",  &evID);


  for(int i=0 ; i<nch ; i++){ //JW if condition to get 00, 01, 02 etc (match with 1st root file
    char name[500];

    if( i< 10){
      sprintf(name,"ph[%d]",i);
    }
    else{
      sprintf(name,"ph[%d]",i);
    }
    t1->SetBranchAddress("ph",&ph);
  }



  // decide what number of entries to average over for (say 100)
  Int_t period = 1000;


  
  Int_t g_points = Int_t(ent/period); 

  if (int(ent)%int(period) == 0){
    g_points = g_points -1; // points with X thousand were producing one more graph point than needed (not sure hwy they didn't fill)
  }


  TF1* funcMult[g_points];
  TH1F* h_fit[g_points];


  std::cout << " g_points =  " << g_points << std::endl;

  //  TGraph* gr1[64];

  double xmin = 0;
  double xmax = ent;


  Int_t bin_1 = 9; // bin numbers if  time cut if flagged
  Int_t bin_2 = 14;

  Double_t time_1 = 4 * bin_1; // 4ns bins
  Double_t time_2 = 4 * bin_2; 


  for (Int_t j=0; j<nch; j++){
    
    char hname[500];
    char htitle[500];
    sprintf(hname,"Pedestals_%d",j);
    sprintf(htitle,"Pedestals over time (channel_number_%d)",j);
    h_raw[j] = new TH1F(hname, htitle, ent, xmin, xmax);
    h_raw[j]->GetXaxis()->SetTitle("Event Number");
    h_raw[j]->GetYaxis()->SetTitle("Pedestal");
      
    sprintf(hname,"Pedestals_2D%d",j);
    h_raw2D[j] = new TH2F(hname, htitle, ent, xmin, xmax, 1000, 6000, 7000);
    h_raw2D[j]->GetXaxis()->SetTitle("Event Number");
    h_raw2D[j]->GetYaxis()->SetTitle("Pedestal");

    sprintf(hname,"raw_distrib_%d",j);
    sprintf(htitle,"Pedestals (channel_number_%d)",j);
    h_ped_m[j] = new TH1F(hname, htitle, ent, 6000, 7000);
    h_ped_m[j]->GetXaxis()->SetTitle("Pedestal");
    //    h_ped_m[j]->GetYaxis()->SetTitle("Pedestal");



    sprintf(hname,"Pedestals_averages_%d",j);
    sprintf(htitle,"Pedestals average (channel_number_%d)",j);
    h_ave[j] = new TH1F(hname, htitle, ent, xmin, xmax);
    h_ave[j]->GetXaxis()->SetTitle("Event Number");
    h_ave[j]->GetYaxis()->SetTitle("Pedestal");

    // sprintf(hname,"Pedestals_averages_%d",j);
    // sprintf(htitle,"Pedestals average (channel_number_%d)",j);
    // h_int[j] = new TH1F(hname, htitle, ent, xmin, xmax);
    // h_int[j]->GetXaxis()->SetTitle("Event Number");
    // h_int[j]->GetXaxis()->SetTitle("Pedestal");

    gr1_m[j] = new TGraph(g_points);

    gr1_m[j]->SetTitle("Average every 1000 events with fit");
    
    if( m_flag == 0){

      char htitle[500];
      sprintf(htitle,"Average of every 1000 events (channel_number_%d), w/ time cut %.2f-%.2f ns",j,time_1, time_2);
      gr1_m[j]->SetTitle(htitle);

    }


    gr1_m[j]->GetXaxis()->SetTitle("Event number");
    gr1_m[j]->GetYaxis()->SetTitle("Pedestal");
    gr1_m[j]->SetMaximum(7000);
    gr1_m[j]->SetMinimum(6000);
    gr1_m[j]->GetXaxis()->SetLimits(0,6000);

    char fname[500];
    sprintf(fname,"func1_%d",j);
    func1[j]  = new TF1(fname,"[0]+[1]*x",0,xmax);
    func1[j]->SetParameter(0, 6500.0);
    func1[j]->SetParameter(1, 0.0);

 }

  // set-up multiple fitting functions

  for(Int_t i = 0; i<g_points; i++){
    char fname[500];
    sprintf(fname,"fucM_%d",i);
    funcMult[i] = new TF1(fname,"[0]+[1]*x", i*period,(i+1) * period);
    funcMult[i]->SetParameter(0, 6500.0);
    funcMult[i]->SetParameter(1, 0.0);

    char hname[500];
    char htitle[500];
    sprintf(hname,"Pedestals_fit%d",i);
    sprintf(htitle,"Pedestals (channel_number_%d)",i);
    h_fit[i] = new TH1F(hname, htitle, ent, xmin, xmax);
    h_fit[i]->GetXaxis()->SetTitle("Event Number");
    h_fit[i]->GetYaxis()->SetTitle("Pedestal");

  }


  // loop fills histograms and graphs

  double integrals = 0;;
  double tot_int[nch];
  Int_t counter = 0;

  Double_t int_ave[g_points];

  Double_t mid_int = 0;


  for(Int_t i = 0; i< ent; i++){
    
    
    t1->GetEntry(i);
    
    // go over all channels and ill in relevant histogram
    
    
    for (Int_t j=0; j<nch; j++){

      // add -up middle bin as smaller integral
      
      mid_int = 0;

      for(Int_t k = 9; k<15; k++){

	mid_int += ph[j][k];
	
      }
      
      if( m_flag == 0 ){

	h_raw[j]->SetBinContent(i+1,mid_int);
	h_raw2D[j]->Fill(i+1,mid_int,1);
	h_ped_m[j]->Fill(mid_int);

	char htitle[500];
	sprintf(htitle,"Pedestals over time (channel_number_%d), w/ time cut %.2f-%.2f ns",j,time_1, time_2);
	h_raw[j]->SetTitle(htitle);
	h_raw[j]->GetYaxis()->SetTitle("Pedestal");	

	// change limits 
	h_raw2D[j]->GetYaxis()->SetLimits(6000/(24/5), 7000/(24/5));
	h_raw2D[j]->SetTitle(htitle);

	sprintf(htitle,"Pedestals (channel_number_%d), w/ time cut %.2f-%.2f ns",j,time_1, time_2);
	h_ped_m[j]->GetXaxis()->SetLimits(6000/(24/5), 7000/(24/5));
	h_ped_m[j]->SetTitle(htitle);

      }
      else{
	h_raw[j]->SetBinContent(i+1,intc_raw[j]);
	h_raw2D[j]->Fill(i+1,intc_raw[j],1);
	h_ped_m[j]->Fill(intc_raw[j]);
      }

      h_ave[j]->Add(h_raw[j]);
      // FILL GRAPHS HERE 
      // can use setpoint to do this

      //      gr1_m[j]->SetPoint 

    
      integrals = h_ave[j]->Integral(xmin+ (i%period)*period,xmax);

      //      tot_int[j] += intc_raw[j];

      if( m_flag == 0 ){
	    
	tot_int[j] += mid_int;

      }
      else{
	tot_int[j] += intc_raw[j];

      }

      if(i>2 && j==channum && i%period == 0){
	counter++;
	
	std::cout << "Condition passed for event:  " << evID << std::endl ; 
	std::cout << " SetPoint(i/period-1, i+1, integrals/period) -> " << i/period << " -- " << i << " -- " << integrals/period << std::endl; 
	

	std::cout << " Raw int example : " << intc_raw[j] << std::endl;

	std::cout << " simpler attempt at ped average : " << tot_int[j]/period << std::endl;

	gr1_m[j]->SetPoint(counter-1, i+1, tot_int[j]/period);

	std::cout << " SetPoint(counter, i+1, tot_int[j]/period) -> " << counter << " -- " << i+1 << " -- " << tot_int[j]/period << std::endl; 
      	
	h_ave[j]->Reset("ICES");
	//	h_raw[j]->Reset("ICES");

	int_ave[counter-1] = tot_int[j]/period;

	tot_int[j] = 0;

      }


    }
    
  }


  // find the average of the averages

  


  Double_t int_ave_ave = 0;
  
  for(Int_t i=0; i<g_points; i++){
    int_ave_ave += int_ave[i];
  }

  int_ave_ave = int_ave_ave/g_points;

  cout<< "int_ave_ave is : " << int_ave_ave << std::endl;

  for (Int_t j=0; j<nch; j++){
    gr1_m[j]->SetMaximum(int_ave_ave+0.05*int_ave_ave);
    gr1_m[j]->SetMinimum(int_ave_ave-0.05*int_ave_ave);
  }
  

  // fittting histo of verage pedestals

  for (Int_t j=0; j<nch; j++){

    char fname[500];
    sprintf(fname,"func1_%d",j);
    gr1_m[j]->Fit(fname,"Nq","",0.0,xmax);
  }

  // fit histo of all integrals (m-m-m-m-multifit)

  for(Int_t i = 0; i<g_points; i++){
     char fname[500];
     sprintf(fname,"fucM_%d",i);
    h_raw[channum]->Fit(fname,"Nq+","", i*period,(i+1) * period);

    std::cout << i +1 <<":sub-range fit formula: [0] + [1]*x" << "\n" << "[0] = " << funcMult[i]->GetParameter(0) << "\n" << "[1] = " << funcMult[i]->GetParameter(1) << std::endl;

    (TVirtualFitter::GetFitter())->GetConfidenceIntervals(h_fit[i]);

  }

  TCanvas* c1 = new TCanvas("c1","c1");
  
  // c1->Divide(2,1);
  c1->cd(1);

  h_raw[channum]->SetMarkerStyle(24);
  h_raw[channum]->Draw("p SAME");   

   for(Int_t i = 0; i<g_points; i++){
     funcMult[i]->SetLineColor(kRed);
     funcMult[i]->Draw("SAME");     
     h_fit[i]->SetFillColor(kBlue);
     h_fit[i]->Draw("e3 SAME");
     // funcMult[i]->SetLineColor(kRed);
     // funcMult[i]->Draw("SAME");     
   }
   //  h_raw[channum]->Draw("p SAME");   
  
  TCanvas* c2 = new TCanvas("c2","c2");

  c2->cd(1);

  h_raw2D[channum]->SetMarkerStyle(24);
  h_raw2D[channum]->Draw("p SAME");   
  

  TCanvas* c3 = new TCanvas("c3","c3");


  

  TProfile* profy = h_raw2D[channum]->ProfileY();
  profy->SetTitle("Pedestals vs event nmber");
  profy->GetXaxis()->SetTitle("Pedestal");
  profy->GetYaxis()->SetTitle("Average event number");
  
  if( m_flag == 0 ){
    char htitle[500];
    sprintf(htitle,"Pedestals vs event number (channel_number_%d), w/ time cut %.2f-%.2f ns",channum,time_1, time_2);
    profy->SetTitle(htitle);
  }

  TH1D* h_peds = profy->ProjectionX();

  // c1->Divide(2,1);
  c3->cd(1);

  gr1_m[channum]->SetMarkerStyle(24);
  func1[channum]->SetLineColor(kBlue);
  gr1_m[channum]->Draw("AP");
  func1[channum]->Draw("SAME");


  TCanvas* c4 = new TCanvas("c4","c4");

  profy->Draw();


  TCanvas* c5 = new TCanvas("c5","c5");

  
  h_ped_m[channum]->Draw("p");

  c1->Update();
  c2->Update();
  c3->Update();



  std::cout << "Full range fit formula: [0] + [1]*x" << "\n" << "[0] = " << func1[channum]->GetParameter(0) << "+/-" << func1[channum]->GetParError(0) << "\n" << "[1] = " << func1[channum]->GetParameter(1) << "\n" << "Chi-square of fit is: " << func1[channum]->GetChisquare() << std::endl;

  std::cout << "RMS of histogram is: "  <<  h_raw[channum]->GetRMS(1) << " or " <<  h_raw[channum]->GetRMS(2) << std::endl;

  std::cout << "Stddev of histogram is: "  <<  h_raw[channum]->GetStdDev(1) << " or " <<  h_raw[channum]->GetStdDev(2) << std::endl;


  std::cout << "RMS of 2D histogram is: "  <<  h_raw2D[channum]->GetRMS(1) << " or " <<  h_raw2D[channum]->GetRMS(2) << std::endl;


  std::cout << "RMS of histogram of pedestals is: " <<  h_ped_m[channum]->GetRMS() << std::endl;

  
  // c2->Divide(2,1);
  // c2->cd(1);

  // h_ped_m[channum]->SetMarkerStyle(24);
  // h_ped_m[channum]->Draw();  
  // gr1_m[channum]->SetMarkerStyle(24);
  // gr1_m[channum]->Draw("AP"); 

  // c2->cd(2);

  // h_ped_m2[channum]->SetMarkerStyle(24);
  // h_ped_m2[channum]->Draw();  
  // gr1_m2[channum]->SetMarkerStyle(24);
  // gr1_m2[channum]->Draw();



 // for (Int_t j=0; j<nch; j++){
 //   delete gr1_m[j];
 //   delete gr1_m2[j];

 // }


  // gr1[0]->Draw("*,SAME");
  // gr1[0]->SetMarkerStyle(24);
  // gr1[0]->SetMarkerSize(1.5);
  // gr1[0]->SetMarkerColor(1);


}

bool fexists(const char *filename){
  std::ifstream ifile(filename);
  return (bool)ifile;
}

  



  //  std:: cout << "Size of array of graphs = " << (sizeof(gr1))/(sizeof(TGraph)) << std::endl;

  // for (Int_t j=0; j<nch; j++){

  //   char gname[500];
  //   //      char gtitle[500];
  //   sprintf(gname,"Pedestals_chan_%d",j);
  //   //      sprintf(gtitle,"Pedestals (channel_number_%d)",j);
    
  //   std::cout << " j*no_runs = " << j*no_runs << std::endl;

  //   // gr1[j] = new TGraph(no_runs,chan[j*no_runs],pedestals[j*no_runs]); 
  //   // gr1[j] = new TGraph(no_runs,chan[0+j*no_runs],pedestals[0+j*no_runs]); 

  //   // std::cout << gname << " was created" << std::endl;
  //   // gr1[j]->SetName(gname);
  // }

