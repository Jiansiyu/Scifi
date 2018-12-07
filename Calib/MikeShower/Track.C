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


void Track(){

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
  Double_t Prl1_x_pos[50],Prl2_x_pos[50],Cher_x_pos[50],s2_x_pos[50],vdc_x[50];
  Double_t Prl1_y_pos[50],Prl2_y_pos[50],Cher_y_pos[50],s2_y_pos[50],vdc_y[50];
  Double_t tr_n,tr_p[50];
  // T->SetBranchAddress("L.tr.x",xTr);
  // T->SetBranchAddress("L.tr.y",yTr);
  
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
   T->SetBranchStatus("L.tr.p",1);

   T->SetBranchAddress("L.tr.d_x",vdc_x);
   T->SetBranchAddress("L.tr.d_y",vdc_y);
   T->SetBranchAddress("L.cer.trx",Cher_x_pos);
   T->SetBranchAddress("L.cer.try",Cher_y_pos);
   T->SetBranchAddress("L.s2.trx",s2_x_pos);
   T->SetBranchAddress("L.s2.try",s2_y_pos);
   T->SetBranchAddress("L.prl1.trx",Prl1_x_pos);
   T->SetBranchAddress("L.prl1.try",Prl1_y_pos);
   T->SetBranchAddress("L.prl2.trx",Prl2_x_pos);
   T->SetBranchAddress("L.prl2.try",Prl2_y_pos);
   // T->SetBranchAddress("L.tr.n",&tr_n);
   T->SetBranchAddress("L.tr.p",tr_p);

  TCanvas *c1 = new TCanvas();
  //TPolyLine3D *l1 = new TPolyLine3d(3);
  //  l1 -> SetPoint(
  TH3F* h0,*h1,*h2,*h3,*h4;
    h0 = new TH3F("track0","tracks0",100,-0.8,5,100,-3,3,100,-3.,3.);
    h1 = new TH3F("track1","tracks1",100,-0.8,5,100,-3,3,100,-3.,3.);
    h2 = new TH3F("track2","tracks2",100,-0.8,5,100,-3,3,100,-3.,3.);
    h3 = new TH3F("track3","tracks3",100,-0.8,5,100,-3,3,100,-3.,3.);
    h4 = new TH3F("track4","tracks4",100,-0.8,5,100,-3,3,100,-3.,3.);
    // h5 = new TH3F("track5","tracks5",100,-0.8,5,100,-3,3,100,-3.,3.);



  // double  xpos[N];
  //double ypos[N];
  // double ch[10];
    int EntryNo = T->GetEntries();
    cout <<"EntryNo"<<"\t"<<EntryNo<<endl;

    double cher_x[100];
    double cher_y[100];

  // cout <<  T->GetEntry(0)<<endl;
  //  cout << x_pos<<endl;
    Int_t counts=0;

    for (int i=0;i<=EntryNo;i++){
   // for (int i=0; i<1000; i++){
    T->GetEntry(i);
           if (tr_p[0]>0.){
	     counts++;
    //    cout <<"entry"<<i<<endl;
    //    cout <<"vdc_x"<<"\t"<<"vdc_y"<<"\t"<<vdc_y<<endl; 
    //  cher_x[i] = x_pos[0];
    // cher_y[i] = y_pos[0];
        h0 -> Fill(0.0,vdc_y[0],vdc_x[0]);
	h1 -> Fill(1.8,Cher_y_pos[0],Cher_x_pos[0]);
	h2 -> Fill(3.17,s2_y_pos[0],s2_x_pos[0]);
	h3 -> Fill(4.58,Prl1_y_pos[0],Prl1_x_pos[0]);
     	h4 -> Fill(4.77,Prl2_y_pos[0],Prl2_x_pos[0]);
    // }
    // h5-> Fill(0.0,vdc_y,vdc_x);
         h0->Draw();
      /*
      h0 ->Draw();
       h0 ->GetXaxis()->SetTitle("Z");
       h0 ->GetZaxis()->SetTitle("X");
       h0 ->GetYaxis()->SetTitle("Y");
      */
         h1->SetMarkerColor(2);
	 h1->Draw("same");
	 h2 ->Draw("same");
	 h3 ->Draw("same"); 
	 h3->SetMarkerColor(7);
      	 h4 ->Draw("same");
	   }

	   // cout <<"counts"<<counts<<endl;
      // h5 ->Draw("same");
      // h0->SetMarkerColor(2);
      //      h3->SetMarkerColor(7);
    
      //      h3->SetMarkerColor(7);
      // h4 ->SetMarkerColor(3);
      //  c1->Update()
      //   cout << i<<"\t"<<"x_pos:"<<"\t"<<Cher_x_pos<<"\t"<<Prl1_x_pos << "\t"<< "y_pos:"<<"\t"<< Cher_y_pos<<"\t"<<Prl1_y_pos<<endl;
      //	 Cher_x_pos=0;
      //	 Cher_y_pos=0;
      //	 Prl1_x_pos=0;
      //	 Prl1_y_pos=0;
  }
   cout <<"counts"<<counts<<endl;

}
