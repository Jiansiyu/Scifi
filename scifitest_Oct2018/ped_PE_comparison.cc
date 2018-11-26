/*

  ped_PE_compariosn.cc

  John Williamson, October 18th, 2018

*/

// function to check if file exists (defined at bottom)
bool fexists(const char *filename); 


void ped_PE_comparison(Int_t run_f = -1, Int_t run_l =  5049, Int_t channum = 0){


  if ( run_f == -1 ){

    char answer[100]  = "filler";

    std::cout << ".x ped_PE_comparison.cc(#run_f, #run_l(def=5049), #channum)" << std::endl;
    std::cout << "Do you want description of macro? (y/n)" << std::endl;
    std::cin >> answer;

    
    while(true){
      
      if(answer[0] == 'n'){
	  return 1;
      }
      else if(answer[0] == 'y'){
	  
	std::cout << "\n" <<"  Macro goes through pedestals for each channel ./ped_integrals/pedestal_%d.dat,  and compares them over the range defined by run_f (first run) and run_l (last run). The range of runs is then from run_f to run_l." << "\n" << "It then produces histograms" <<  std::endl;
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
  Int_t  no_runs = run_l - run_f;

  Double_t pedestals[nch][no_runs];
  Double_t RMS[nch][no_runs];
  Double_t chan[nch][no_runs];
  Double_t temp;
    
  Double_t ped_tot[nch];
  Double_t RMS_tot[nch];
  
  Double_t ped_mean[nch];
  Double_t RMS_mean[nch];

  Bool_t zero_ped[nch][no_runs];

  TH1F* h_peds[nch];
  TH1F* h_ped_m[nch];
  TH1F* h_ped_m2[nch];


  TGraph* gr1_m[nch];
  TGraph* gr1_m2[nch];


  Double_t xmin = run_f;
  Double_t xmax = run_l;  



  Int_t no_of_runs = 0;


  // loop to find number of runs that exist (aren't empty)
 
  for(Int_t i = 0; i< no_runs; i++){

    char fname[500];
    sprintf(fname,"./ped_integrals/pedestal_%d.dat",i+run_f);
    
    
    if ( fexists(fname) ){
      
      no_of_runs++;
    }
    
  }

  //  TGraph* gr1[64];

  for (Int_t j=0; j<nch; j++){
    
    char hname[500];
    char htitle[500];
    sprintf(hname,"Pedestals_chan_%d",j);
    sprintf(htitle,"Pedestals (channel_number_%d)",j);
    h_peds[j] = new TH1F(hname, htitle, no_runs, xmin, xmax);
    h_peds[j]->GetXaxis()->SetTitle("Run number");
      
    sprintf(hname,"Pedestals_chan_%d - mean",j);
    sprintf(htitle,"Pedestals - mean (channel_number_%d)",j);
    h_ped_m[j] = new TH1F(hname, htitle, no_runs, xmin, xmax);
    h_ped_m[j]->GetXaxis()->SetTitle("Run number");


    sprintf(hname,"Percentage Pedestals_chan_%d - mean.",j);
    sprintf(htitle,"Percentage, Pedestals - mean (channel_number_%d)",j);
    h_ped_m2[j] = new TH1F(hname, htitle, no_runs, xmin, xmax);
    h_ped_m2[j]->GetXaxis()->SetTitle("Run number");

    // for(Int_t i= 0; i<no_runs; i++){

    //   pedestals[j][i] = 0;
    //   RMS[j][i] = 0;
    //   chan[j][i] = 0;
    // }

    ped_tot[j] = 0;
    //    ped_mean[j] = 0;

    RMS_tot[j] = 0;
    //    RMS_mean[j] = 0;
 }


  // for(Int_t i = 0; i < nch; i++){


  //   gr1_m[i] = new TGraph(no_of_runs);
  //   gr1_m2[i] = new TGraph(no_of_runs);

  //   gr1_m[i]->GetXaxis()->SetTitle("Run number");
  //   gr1_m[i]->GetYaxis()->SetTitle("Raw difference");
  //   gr1_m[i]->SetMaximum(200);
  //   gr1_m[i]->SetMinimum(-200);

  //   gr1_m2[i]->GetXaxis()->SetTitle("Run number");
  //   gr1_m2[i]->GetYaxis()->SetTitle("Percentage difference");;
  //   gr1_m2[i]->SetMaximum(3);
  //   gr1_m2[i]->SetMinimum(-3);


  // }


  // loop fills histograms and graphs


  // keep track of number of zero_peds 
  Int_t zero_peds_no = 0;

  for(Int_t i = 0; i< no_runs; i++){


  char fname[500];
  sprintf(fname,"./ped_integrals/pedestal_%d.dat",i+run_f);
  

  if ( fexists(fname) ){

    std::cout <<  fname << " exists " << std::endl; 

    std::ifstream* ifs = new ifstream(fname);


    
    // go over all channels and ill in relevant histogram


    for (Int_t j=0; j<nch; j++){
      
      *ifs >> chan[j][i] >> pedestals[j][i] >> RMS[j][i];


      //      h_peds[j]->SetBinContent(i+1,pedestals[j][i]);

      //      RMS_tot[j] += RMS[j][i];
      ped_tot[j] += pedestals[j][i];

      // FILL GRAPHS HERE 
      // can use setpoint to do this

      zero_ped[j][i] = false;

      if (pedestals[j][i] == 0){

	//	zero_peds_no++;
	zero_ped[j][i] = true;
	
	
	// only counting for displayed channel
	if (j == channum){
	  zero_peds_no++;
	}


      }
      else{

      RMS_tot[j] += RMS[j][i];
      
      }

     

    }
  
    
  }

  }  

  /// calculate mean of pedestals and prodcue histograms relative to this

  //  std::cout << " No of runs are " << no_of_runs << std::endl;



  for(Int_t i = 0; i < nch; i++){


    gr1_m[i] = new TGraph(no_of_runs -  zero_peds_no);
    gr1_m2[i] = new TGraph(no_of_runs - zero_peds_no);

    gr1_m[i]->GetXaxis()->SetTitle("Run number");
    gr1_m[i]->GetYaxis()->SetTitle("Raw difference");
    gr1_m[i]->SetMaximum(200);
    gr1_m[i]->SetMinimum(-200);

    gr1_m2[i]->GetXaxis()->SetTitle("Run number");
    gr1_m2[i]->GetYaxis()->SetTitle("Percentage difference");;
    gr1_m2[i]->SetMaximum(3);
    gr1_m2[i]->SetMinimum(-3);


  }




  for (Int_t j=0; j<nch; j++){

    ped_mean[j] = ped_tot[j]/ double(no_of_runs - zero_peds_no);
    RMS_mean[j] = RMS_tot[j]/ double(no_of_runs - zero_peds_no);
  }

  Int_t counter = 0;

  for (Int_t j=0; j<nch; j++){
    counter = 0;

    for(Int_t i = 0; i< no_runs; i++){

	char fname[500];
	sprintf(fname,"./ped_integrals/pedestal_%d.dat",i+run_f);
	if ( fexists(fname) ){
	  
	  
	  if(zero_ped[j][i] == false){

	    h_peds[j]->SetBinContent(i+1,pedestals[j][i]);

	    h_ped_m[j]->SetBinContent(i+1, pedestals[j][i] - ped_mean[j]);
	    h_ped_m2[j]->SetBinContent(i+1, (100 * (pedestals[j][i] - ped_mean[j])/ ped_mean[j])  );
	  
	    gr1_m[j]->SetPoint(counter, i + run_f,  pedestals[j][i] - ped_mean[j]);
	    gr1_m2[j]->SetPoint(counter, i + run_f,(100 * (pedestals[j][i] - ped_mean[j])/ ped_mean[j]) );
	    
	    counter++;
	  }

	}
	
    }
  }
  
  
  // TGraph* gr1_m[nch];
  // TGraph* gr1_m2[nch];

  Int_t test = 1;
  Double_t y =1;
  Double_t x =1;

  Double_t y1 =1;
  Double_t x1 =1;







  for (Int_t j=0; j<nch; j++){

  //   gr1_m[j] = new TGraph(h_ped_m[j]); 
  //   gr1_m[j]->GetXaxis()->SetTitle("Run number");
  //   gr1_m[j]->GetYaxis()->SetTitle("Raw difference");

  //   gr1_m2[j] = new TGraph(h_ped_m2[j]); 
  //   gr1_m2[j]->GetXaxis()->SetTitle("Run number");
  //   gr1_m2[j]->GetYaxis()->SetTitle("Percentage difference");;


      for(Int_t i = 0; i< no_runs; i++){
	
	// y = 1;
	// x = 1;
	// gr1_m[j]->GetPoint(i,x,y);

	// if (y == 0){
	//   gr1_m[j]->RemovePoint(i);
	// }

	// y = 1;
	// gr1_m2[j]->GetPoint(i,x,y);
	
	// if (y == 0 && j == channum){
	  
	//   gr1_m2[j]->GetPoint(i+1,x1,y1);
	  
	//   std::cout << "One point on from this is at " << x1 << ", " << y1 <<std::endl;
	    
	//   std::cout << " this condition is passed! For run: " << i + run_f << " and co-ords: x = " << x << ", y = " << y << "\n" << std::endl;
	//   gr1_m2[j]->RemovePoint(i);
	// }
	// else if (j == channum){
	//   std::cout << " this condition is not passed! For run: " << i + run_f << " and co-ords: x = " << x << ", y = " << y <<std::endl;
	// }


	  
      }
      
      h_ped_m[j]->Clear();
      h_ped_m2[j]->Clear(); 
  
  }




  TCanvas* c1 = new TCanvas("c1","c1");
  
  c1->cd();

  h_peds[channum]->SetMarkerStyle(24);
  h_peds[channum]->Draw("P");  

  c1->Update();

  TCanvas* c2 = new TCanvas("c2","c2");
  
  c2->Divide(2,1);
  c2->cd(1);

  h_ped_m[channum]->SetMarkerStyle(24);
  h_ped_m[channum]->Draw();  
  gr1_m[channum]->SetMarkerStyle(24);
  gr1_m[channum]->Draw("AP"); 

  c2->cd(2);

  h_ped_m2[channum]->SetMarkerStyle(24);
  h_ped_m2[channum]->Draw();  
  gr1_m2[channum]->SetMarkerStyle(24);
  gr1_m2[channum]->Draw();



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

