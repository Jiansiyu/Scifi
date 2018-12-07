#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2F.h>

//91327 or 91326  //LHRS 2024 or 2023??

using namespace std;

void ADC_check(){

  int run_number;
  cout <<"enter run number"<<endl;
  cin >> run_number;


  //  TString Base_Path = "/chafs1/work1/tritium/Rootfiles"; 
  TString Base_Path = "/volatile/halla/triton/Tritium_Rootfiles";
  //     TString Base_Path = "/chafs1/work1/tritium/rootfiles_1.6_root6";

  TChain *T = new TChain("T");
  TChain *evLeft = new TChain("evLeft");
  std::ostringstream step ;

  step << Base_Path << "/tritium_" << run_number;
  TString convert = step.str().c_str();
  TString rootfile = convert + ".root";

  Long_t q=0;
  while  (! gSystem ->AccessPathName(rootfile)) {
    T->Add(rootfile);
    evLeft->Add(rootfile);
    //TSLeft1->Add(rootfile);                                                                                                                                                                                                         
    cout << "T Branch Added"<<rootfile<<endl;
    q++;
    rootfile = convert + "_"+ q + ".root";
  }


  TH2F *h2,*h3,*h4,*h5,*h6,*h7,*h8,*h9;
  double ADC[48],C_ADC[10],s0_ADC[2],prl2_ADC[48];

  h2 = new TH2F("prl1","prl1",163503,0,163503,2000,0,2000);
  h7 = new TH2F("cherenkov","Cherenkov",163503,0,163503,2000,0,2000);
  h8 = new TH2F("s0","s0",163503,0,163503,2000,0,2000);
  h9 = new TH2F("prl2","prl2",163503,0,163503,2000,0,2000);
  h3 = new TH2F("prl1","prl1",70,0,70,2000,0,2000);
  h4 = new TH2F("cherenkov","Cherenkov",70,0,70,2000,0,2000);
  h5 = new TH2F("s0","s0",70,0,70,2000,0,2000);
  h6 = new TH2F("prl2","prl2",70,0,70,2000,0,2000);

  double evLeftLclock;
  double Lclock;
  double prl1[48],prl2[48],cerenkov[10],s0[2];
  T->SetBranchAddress("L.prl1.a",prl1);
  T->SetBranchAddress("L.prl2.a",prl2);
  T->SetBranchAddress("FbusL.cer.a",cerenkov);
  T->SetBranchAddress("LeftLclock",&Lclock);
  T->SetBranchAddress("FbusL.s0.la",s0);

  evLeft -> SetBranchAddress("evLeftLclock",&evLeftLclock);
 
  double Left_PrevClk=0;
  double Left_Time=0;
  double clk_rate = 103700;
  TCanvas *c1 = new TCanvas();
  c1 -> Divide(2,2);
  TCanvas *c2 = new TCanvas();
  c2 -> Divide(2,2);
 int nEvents =  T->GetEntries();


  for (int i=0;i<nEvents; i++){
    T-> GetEntry(i);
    //    evLeft->GetEntry(i);
    ADC[0] = prl1[0];
    C_ADC[0] = cerenkov[0];
    s0_ADC[0] = s0[0];
    prl2_ADC[0] = prl2[0];
    h2 -> Fill(i,ADC[0]);
    c1 -> cd(1);
    h2 -> GetXaxis()->SetTitle("Entry");
    h2 -> GetYaxis()->SetTitle("ADC");
    h2 -> Draw();
    h9 -> Fill(i,prl2_ADC[0]);
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
    //    h3 -> Fill(evLeftLclock,ADC[0]);

    Left_Time += ((Lclock) - Left_PrevClk) / (clk_rate);
    //cout << "time" << Uc_Time<<endl;                                                                                                                                                                                                     
  h3 -> Fill(+Left_Time/60.0,ADC[0]);
  //  TCanvas *c2 = new TCanvas();
  c2 -> cd(1);
  h3 -> GetXaxis()->SetTitle("Entry");
  h3 -> GetYaxis()->SetTitle("minutes");
  h3 -> Draw();
  h6 -> Fill(+Left_Time/60.0,prl2_ADC[0]);
  c2 -> cd(2);
  h6 -> GetXaxis()->SetTitle("Entry");
  h6 -> GetYaxis()->SetTitle("minutes");
  h6 ->Draw();
  h4 -> Fill(+Left_Time/60.0,C_ADC[0]);  
  c2->cd(3);
  h4 -> GetXaxis()->SetTitle("Entry");
  h4 -> GetYaxis()->SetTitle("minutes");
  h4->Draw();
  h5 -> Fill(+Left_Time/60.0,s0_ADC[0]);
  c2 -> cd(4);
  h5 -> GetXaxis()->SetTitle("Entry");
  h5 -> GetYaxis()->SetTitle("minutes");
  h5->Draw();
  Left_PrevClk = Lclock ;



  }

  cout << "Lefttime" << Left_Time<<endl;

  double Uc_PrevClk = 0;
  double Uc_Time    = 0;
  //  double clk_rate = 103700; // 1.037 kHz clock 

  int evEntry = evLeft ->GetEntries();
  for (Int_t i =0;i<evEntry;i++){
    evLeft -> GetEntry(i);
    Uc_Time += ((evLeftLclock) - Uc_PrevClk) / (clk_rate);
    //cout << "time" << Uc_Time<<endl;
    Uc_PrevClk = evLeftLclock ;
  }
  cout << "time" << Uc_Time<<endl;


}
