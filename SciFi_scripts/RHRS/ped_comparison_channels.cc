/*

  ped_comparison_channels.cc

  John Williamson, December 7th, 2018

*/

// function to check if file exists (defined at bottom)
bool fexists(const char *filename); 

// find mean of pedestals
Double_t Mean_Ped(Double_t arr[], Int_t size);

using namespace std;


void ped_comparison_channels(Int_t runnum){

  const Int_t nch = 64; // no of channels (64 for SciFi)

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
  }


  Double_t mean_ped = Mean_Ped(ped, nch);

  // create plot to display compare pedestals to average (of pedestals of all channels)

  TH1D* h_peds = new TH1D("Pedestals compared to mean", "Pedestals compared to mean", nch, 0, nch);

  // filling histogram with pedestal values
  for (Int_t j=0; j<nch; j++){
    h_peds->SetBinContent(j,ped[j]);
  }
  
  TLine *line = new TLine(0,mean_ped,nch,mean_ped);

  
  TCanvas *cped = new TCanvas("c1","c1",1200,1200);
  
  cped->cd(0);


  h_peds->GetXaxis()->SetTitle("channel number");

  h_peds->GetYaxis()->SetTitle("Amplitude");
  h_peds->GetYaxis()->SetRangeUser(1600,2500);
  
  h_peds->Draw();
  line->Draw("SAME");

  char pdfname [50];

  sprintf(pdfname,"plots/pedestals/Pedestal_comparison_channels_%d.pdf",runnum);
  cped->Print(pdfname,"pdf");

  
  cout << "mean is " << mean_ped << endl;

}




bool fexists(const char *filename){
  std::ifstream ifile(filename);
  return (bool)ifile;
}


Double_t Mean_Ped(Double_t arr[], Int_t size){
  Int_t i, sum = 0;
  Double_t avg;

  for (i = 0; i<size; ++i){
    sum += arr[i];
  }
  
  avg = Double_t(sum) / size;

  return avg;
}


