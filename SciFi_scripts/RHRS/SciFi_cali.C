#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <iomanip>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"

using namespace std;
void SciFi_cali()
{
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
//  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
  T->Add(Form("/adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_%d.root",run_number));

  cout << T->GetEntries() << endl;
  
  Double_t min, max, temp;
  Double_t ped_val,ped_wid;
  Int_t max_bin;
  Int_t i, ii;
  Double_t ped[10],ped_width[10];
  Double_t gain[10];
  ofstream myfile;
  myfile.open("R_SciFi_ped.txt");//,fstream::app);
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "gain" << "   "<<endl;


  TCut tritype = "";//"((DR.evtypebits>>1)&1)"; // trigger cut

  
  Int_t nch = 64;

  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(5,2); 

  char fname[500];
  sprintf(fname,"func1");
  TF1* func1 = new TF1(fname,"gaus");
  TH1F *tt1 = new TH1F("tt1","FADC_RSciFi_%d",1000,8000,12000);

  for(i=0;i<nch;i++){
    c1->cd(i+1);
    tt1->Reset();
    T->Draw(Form("R.sf.A[%d]>>tt1",i),tritype);
    tt1->SetXTitle(Form("R.sf.A[%d]",i));
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 40;
    max = tt1->GetBinCenter(max_bin) + 40;
    tt1->Fit(fname,"NQ","",min,max);
    min = func1->GetParameter(1) - 3*func1->GetParameter(2);
    max = func1->GetParameter(1) + 3*func1->GetParameter(2);

    tt1->Fit(fname,"Q","",min,max);

//    delete tt1;
    ped_val = func1->GetParameter(1);
    ped_wid = func1->GetParameter(2);    

    gPad->SetLogy();

    ped[i] = ped_val;
    ped_width[i] = ped_wid; 

    // similar method as previosu to get pedestal (double fitting of amplitude)


 //w   cout << "Lcer" << i << endl;
    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(2) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << ped_wid <<endl;
  } 

  Double_t lfgain[10];

  myfile << setiosflags(ios::left) << setw(2) << "#" << "   ";
  myfile << setiosflags(ios::left) << setw(5) << "gain" << endl;

  Double_t fscale=100;
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(5,2);
  Int_t min_bin = -1;
  TH1F *tt2 = new TH1F("tt2","RSciFi_p",300,-100,1000);


  for(i=0;i<10;i++){
    c2->cd(i+1);
    T->Draw(Form("R.sf.A_p[%d]>>tt2",i),tritype);
    tt2->SetXTitle(Form("R.sf.A_p[%d]",i));
    tt2->GetXaxis()->SetRangeUser(0,110);
    min_bin = tt2->GetMinimumBin();
    min_bin = tt2->GetXaxis()->GetBinCenter(min_bin);
    tt2->GetXaxis()->SetRangeUser(-100,1000);

    TF1 *f1 = new TF1("f1", "gaus",min_bin, min_bin+4*fscale);
    tt2->Fit("f1", "Rq");

    min = f1->GetParameter(1) - 1*f1->GetParameter(2);
    max = f1->GetParameter(1) + 1*f1->GetParameter(2);

    tt2->Fit(func1,"Q","",min,max);
    lfgain[i]=300*1.0/func1->GetParameter(1);


    gPad->SetLogy();

    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(5) << lfgain[i] <<endl;
  }

  TCut peakcut="";
 
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   ";
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "gain" << "   "<<endl;

 
  Double_t scale =100;
  TCanvas *c3 = new TCanvas("c3","c3",1200,1200);
  c3->Divide(5,2);
  TH1F *tt3 = new TH1F("tt3","Rcer_%d",600,200,2000);

  for(i=0;i<10;i++){
    c3->cd(i+1);
    T->Draw(Form("FbusR.sf.A[%d]>>tt3",i),tritype);
    tt3->SetXTitle(Form("FbusR.sf.A[%d]",i));
    max_bin = tt3->GetMaximumBin();
    min = tt3->GetBinCenter(max_bin) - 20;
    max = tt3->GetBinCenter(max_bin) + 20;
    tt3->Fit(fname,"NQ","",min,max);
    min = func1->GetParameter(1) - 3*func1->GetParameter(2);
    max = func1->GetParameter(1) + 3*func1->GetParameter(2);

    tt3->Fit(fname,"Q","",min,max);

//    delete tt1;
    ped_val = func1->GetParameter(1);
    ped_wid = func1->GetParameter(2);

    gPad->SetLogy();

    ped[i] = ped_val;
    ped_width[i] = ped_wid;

    tt3->GetXaxis()->SetRangeUser(ped[i],ped[i]+scale*1);
    min_bin = tt3->GetMinimumBin();
    min_bin = tt3->GetXaxis()->GetBinCenter(min_bin);
    tt3->GetXaxis()->SetRangeUser(ped[i]-1*scale,ped[i]+5*scale);

    TF1 *f1;
    if(i==0)
       f1 = new TF1("f1", "gaus",min_bin+10, min_bin+1*scale);
    else f1 = new TF1("f1", "gaus",min_bin, min_bin+1*scale);
    tt3->Fit("f1", "Rq");

    min = f1->GetParameter(1) - 1*f1->GetParameter(2);
    max = f1->GetParameter(1) + 1*f1->GetParameter(2);

    tt3->Fit(func1,"Q","",min,max);
    gain[i]=100*1.0/(func1->GetParameter(1)-ped[i]);

    TLine *l1 = new TLine(ped[i],0,ped[i],tt3->GetMaximum());
    l1->SetLineColor(2);
    l1->Draw();

 //w   cout << "Lcer" << i << endl;
    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(2) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(5) << gain[i]<<endl;

  }

  c1->Print("Rcer.pdf[");
  c1->Print("Rcer.pdf");
  c2->Print("Rcer.pdf");
  c3->Print("Rcer.pdf");
  c3->Print("Rcer.pdf]");
  myfile.close();

}

