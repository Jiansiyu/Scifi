#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2F.h>



using namespace std;

void ADC_Right_Check(){

  int run_number;
  cout <<"enter run number"<<endl;
  cin >> run_number;



       TString Base_Path = "/volatile/halla/triton/Tritium_Rootfiles";
  //    TString Base_Path = "/chafs1/work1/tritium/rootfiles_1.6_root6";

  TChain *T = new TChain("T");
  //  TChain *evLeft = new TChain("evLeft");
  std::ostringstream step ;

  step << Base_Path << "/tritium_" << run_number;
  TString convert = step.str().c_str();
  TString rootfile = convert + ".root";

  Long_t q=0;
  while  (! gSystem ->AccessPathName(rootfile)) {
    T->Add(rootfile);
    //evLeft->Add(rootfile);
    //TSLeft1->Add(rootfile);                                                                                                                                                                                                         
    cout << "T Branch Added"<<rootfile<<endl;
    q++;
    rootfile = convert + "_"+ q + ".root";
  }


  TH2F *h2,*h3,*h4,*h5,*h6,*h7,*h8,*h9;
  double ADC[48],C_ADC[10],s0_ADC[2],sh_ADC[75];

  h2 = new TH2F("ps","ps",163503,0,163503,2000,0,2000);
  h7 = new TH2F("cherenkov","Cherenkov",163503,0,163503,2000,0,2000);
  h8 = new TH2F("s0","s0",163503,0,163503,2000,0,2000);
  h9 = new TH2F("sh","sh",163503,0,163503,2000,0,2000);
  h3 = new TH2F("ps","ps",70,0,70,2000,0,2000);
  h4 = new TH2F("cherenkov","Cherenkov",70,0,70,2000,0,2000);
  h5 = new TH2F("s0","s0",70,0,70,2000,0,2000);
  h6 = new TH2F("sh","sh",70,0,70,2000,0,2000);

  double evLeftLclock;
  double Rclock,MLU;
  double ps[48],sh[75],cerenkov[10],s0[2];
  T->SetBranchAddress("R.ps.a",ps);
  T->SetBranchAddress("R.sh.a",sh);
  T->SetBranchAddress("FbusR.cer.a",cerenkov);
  //  T->SetBranchAddress("RightRclock",&Rclock);
T->SetBranchAddress("V1495ClockCount",&MLU);

  T->SetBranchAddress("FbusR.s0.la",s0);

  //  evLeft -> SetBranchAddress("evLeftLclock",&evLeftLclock);
 
  double Right_PrevClk=0;
  double Right_Time=0;
  double clk_rate = 103700;
  TCanvas *c1 = new TCanvas();
  c1 -> Divide(2,2);
  TCanvas *c2 = new TCanvas();
  c2 -> Divide(2,2);
 int nEvents =  T->GetEntries();
 //cout <<nEvents<<endl;

  for (int i=0;i<nEvents; i++){
    T-> GetEntry(i);
    //    evRight->GetEntry(i);
    ADC[0] = ps[0];
    C_ADC[0] = cerenkov[0];
    s0_ADC[0] = s0[0];
    sh_ADC[0] = sh[0];
    h2 -> Fill(i,ADC[0]);
    c1 -> cd(1);
    h2 -> GetXaxis()->SetTitle("Entry");
    h2 -> GetYaxis()->SetTitle("ADC");
    h2 -> Draw();
    h9 -> Fill(i,sh_ADC[0]);
    c1 -> cd(2);
    h9 -> GetXaxis()->SetTitle("Entry");
    h9 -> GetYaxis()->SetTitle("ADC");
    h9->Draw();
    h7 -> Fill(i,C_ADC[0]);
    c1 -> cd(3);
    h7 -> GetXaxis()->SetTitle("Entry");
    h7 -> GetYaxis()->SetTitle("ADC");
    h7 -> Draw();
    h8 -> Fill(i,s0_ADC[0]);
    c1 -> cd(4);
    h8 -> GetXaxis()->SetTitle("Entry");
    h8 -> GetYaxis()->SetTitle("ADC");
    h8 ->Draw();
    //   h2-> Draw();
    //    h3 -> Fill(evRightLclock,ADC[0]);

    Right_Time += ((MLU) - Right_PrevClk) / (clk_rate);
    //cout << "time" << Uc_Time<<endl;                                                                                                                                                                                                     
  h3 -> Fill(Right_Time/60.0,ADC[0]);
  //  TCanvas *c2 = new TCanvas();
  c2 -> cd(1);
  h3 -> GetXaxis()->SetTitle("ADC");
  h3 -> GetYaxis()->SetTitle("minutes");
  h3 -> Draw();
  h6 -> Fill(Right_Time/60.0,sh_ADC[0]);
  c2 -> cd(2);
  h6 -> GetXaxis()->SetTitle("ADC");
  h6 -> GetYaxis()->SetTitle("minutes");
  h6 ->Draw();
  h4 -> Fill(Right_Time/60.0,C_ADC[0]);  
  c2->cd(3);
  h4 -> GetXaxis()->SetTitle("ADC");
  h4 -> GetYaxis()->SetTitle("minutes");
  h4->Draw();
  h5 -> Fill(Right_Time/60.0,s0_ADC[0]);
  c2 -> cd(4);
  h5 -> GetXaxis()->SetTitle("ADC");
  h5 -> GetYaxis()->SetTitle("minutes");
  h5->Draw();
  Right_PrevClk = MLU ;



  }

  cout << "Righttime" << Right_Time<<endl;
  /*
  double Uc_PrevClk = 0;
  double Uc_Time    = 0;
  //  double clk_rate = 103700; // 1.037 kHz clock 

  int evEntry = evRight ->GetEntries();
  for (Int_t i =0;i<evEntry;i++){
    evRight -> GetEntry(i);
    Uc_Time += ((evRightLclock) - Uc_PrevClk) / (clk_rate);
    //cout << "time" << Uc_Time<<endl;
    Uc_PrevClk = evRightLclock ;
  }
  cout << "time" << Uc_Time<<endl;
  */

}
