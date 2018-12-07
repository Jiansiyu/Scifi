#include <iostream>
#include <iomanip>
#include <fstream>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TString.h>
#include <TPolyLine3D.h>


using namespace std;


void Test(){

  int run_number;
  cout <<"Enter Run Number : "<<endl;
  cin >> run_number;

  char arm[2];
  if (run_number<90000){
    sprintf(arm,"L");
  }
  else {
    sprintf(arm,"R");
  }

  //    TString Base_Path = "/volatile/halla/triton/Tritium_Rootfiles";
  //    const TString Base_Path = "/chafs1/work1/tritium/M_rootfiles";
    const TString Base_Path = "/chafs1/work1/tritium/pass1";
  //  const TString Base_Path = "/mss/halla/triton/prod/pass1_calibration/kin15";
  std::ostringstream step ;

  step << Base_Path << "/tritium_" << run_number;
  TString convert = step.str().c_str();
  TString rootfile = convert + ".root";



  TChain *T = new TChain("T");
    Long_t j=0;
  while  (! gSystem ->AccessPathName(rootfile)) {
    T->Add(rootfile);                                                               
    cout << "Added"<<rootfile<<endl;
      j++;
        rootfile = convert + "_"+ j + ".root";
    //    rootfile = convert + "_"+ ".root";
  }


  //  const int N = 100000;
  // double xTr[N];
  // double yTr[N];
  // double Cher[10];
  Double_t Prl1_x_pos=0,Prl2_x_pos=0/*,Cher_x_pos=0*/,s2_x_pos=0,vdc_x=0;
  Double_t Prl1_y_pos=0,Prl2_y_pos=0/*,Cher_y_pos=0*/,s2_y_pos=0,vdc_y=0,tr_n=0,tr_p=0;
  Double_t Cher_x_pos[100];
  Double_t Cher_y_pos[100];
 
  //  Double_t s2_x_pos[100],s2_y_pos[100];
  // T->SetBranchAddress("L.tr.x",xTr);
  // T->SetBranchAddress("L.tr.y",yTr);

  /*  
   T->SetBranchStatus("*",0);

   T->SetBranchStatus("L.tr.d_x",1);
   T->SetBranchStatus("L.tr.d_y",1);
   T->SetBranchStatus("L.cer.trx",1);
   T->SetBranchStatus("L.cer.try",1);
   T->SetBranchStatus("L.s2.trx",1);
   T->SetBranchStatus("L.s2.try",1);
   T->SetBranchStatus("L.prl1.trx",1);
   T->SetBranchStatus("L.prl1.try",1);
   T->SetBranchStatus("L.prl2.trx",1);
   T->SetBranchStatus("L.prl2.try",1);
   //T->SetBranchStatus("L.tr.n",1);
   //T->SetBranchStatus("L.tr.p",1);
   */
 
  //   T->SetBranchAddress("L.tr.d_x",&vdc_x);
  // T->SetBranchAddress("L.tr.d_y",&vdc_y);
  /*   
   T->SetBranchAddress("L.cer.trx",&Cher_x_pos);
   T->SetBranchAddress("L.cer.try",&Cher_y_pos);
   T->SetBranchAddress("L.s2.trx",&s2_x_pos);
   T->SetBranchAddress("L.s2.try",&s2_y_pos);
      T->SetBranchAddress("L.prl1.trx",&Prl1_x_pos);
   T->SetBranchAddress("L.prl1.try",&Prl1_y_pos);
    T->SetBranchAddress("L.prl2.trx",&Prl2_x_pos);
   T->SetBranchAddress("L.prl2.try",&Prl2_y_pos);
  */
   /* 
  // T->SetBranchAddress("L.tr.n",&tr_n);
   //   T->SetBranchAddress("L.tr.p",&tr_p);
   */
  TCanvas *c1 = new TCanvas();
  //TPolyLine3D *l1 = new TPolyLine3d(3);
  //  l1 -> SetPoint(
  TH3F* h0,*h1,*h2,*h3,*h4;
    h0 = new TH3F("track0","tracks0",100,-0.8,5,100,-5.,5.,100,-5.,5.);
    h1 = new TH3F("track1","tracks1",100,-0.8,5,100,-5.,5.,100,-5.,5.);
    h2 = new TH3F("track2","tracks2",100,-0.8,5,100,-5.,5.,100,-5.,5.);
    h3 = new TH3F("track3","tracks3",100,-0.8,5,100,-5.,5.,100,-5.,5.);
    h4 = new TH3F("track4","tracks4",100,-0.8,5,100,-5.,5.,100,-5.,5.);
    // h5 = new TH3F("track5","tracks5",100,-0.8,5,100,-3,3,100,-3.,3.);

    // T->Draw("L.prl1.e:L.prl2.e>>h1(5000,0,5000,5000,0,5000)","L.cer.asum_c >2000.");

  // double  xpos[N];
  //double ypos[N];
  // double ch[10];
  int EntryNo = T->GetEntries();
   cout <<"EntryNo"<<"\t"<<EntryNo<<endl;

   double cher_x[100];
   double cher_y[100];

   /*
  for (Int_t i=0; i<1000; i++){
    T->GetEntry(i);
    cout <<i<<"\t"<<"Prl1_y_pos"<<Prl1_y_pos<<"\t"<<"Prl1_x_pos"<<"\t"<<Prl1_x_pos<<endl;
    //h0->Fill(0.0,vdc_y,vdc_x);
    // h0->Draw();
    //    h1 -> Fill(1.8,Cher_y_pos,Cher_x_pos);
    //h1->Draw();
    //  }
    //  for (int i=0; i<20000; i++){
    //    T->GetEntry(i);
    //h2 -> Fill(3.17,s2_y_pos,s2_x_pos);
    //h3 -> Fill(4.58,Prl2_y_pos,Prl2_x_pos);
  
    // h1->Draw();
    //h2->Draw("same");
    // h3->Draw("same");
     }
   */
  //h5->Draw("same");
  // h1->Draw();
  //h3->Draw("same");
  // c1 -> Update();
  /*
  for(int i=0; i<100; i++){
cout <<"x_pos:"<<"\t"<<cher_x[i] << "\t"<< "y_pos:"<<"\t"<< cher_y[i]<<endl;
}
  */



}
