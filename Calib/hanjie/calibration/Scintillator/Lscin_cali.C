#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cassert>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"

using namespace std;
void Lscin_cali()
{
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));

  cout << T->GetEntries() << endl;
  
  Double_t min, max, temp;
  Double_t ped_val,ped_wid;
  Int_t max_bin;
  Int_t i, ii;
  Double_t ped[16],ped_width[16];
  Double_t gain[16];
  ofstream myfile;
  myfile.open("Lscin_ped.txt");//,fstream::app);
  myfile << setiosflags(ios::left) << setw(2) << "#" << "   "; 
  myfile << setiosflags(ios::left) << setw(5) << "Ped" << "   ";
  myfile << setiosflags(ios::left) << setw(8) << "ped width" << "   "<<endl;


  TCut tritype = "";//"((DR.evtypebits>>1)&1)";
 
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(4,4); 
  for(i=0;i<16;i++){
    c1->cd(i+1);
    TH1F *tt1 = new TH1F("tt1",Form("FADC_Ls2_la%d",i+1),500,4000,8000);
    T->Draw(Form("L.s2.la[%d]>>tt1",i,i),tritype);
    tt1->SetXTitle(Form("L.s2.la[%d]",i));
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 40;
    max = tt1->GetBinCenter(max_bin) + 40;
    tt1->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 2*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 2*gaus->GetParameter(2);

    tt1->Fit("gaus","Q","",min,max);

//    delete tt1;
    ped_val = gaus->GetParameter(1);
    ped_wid = gaus->GetParameter(2);    

    gPad->SetLogy();

    ped[i] = ped_val;
    ped_width[i] = ped_wid; 

//    delete tt1;
 //w   cout << "Lcer" << i << endl;
    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(2) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << ped_wid <<endl;
  } 

  Double_t lfgain[16];

  myfile << setiosflags(ios::left) << setw(2) << "#" << "   ";
  myfile << setiosflags(ios::left) << setw(5) << "gain" << endl;

  Double_t fscale=100;
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  c2->Divide(4,4);
  for(i=0;i<16;i++){
    c2->cd(i+1);
    TH1F *tt2 = new TH1F("tt2",Form("Ls2_lap_%d",i+1),300,-200,1000);
    T->Draw(Form("L.s2.la_p[%d]>>tt2",i,i),tritype);
    tt2->SetXTitle(Form("L.s2.la_p[%d]",i));
    tt2->GetXaxis()->SetRangeUser(0,190);
    min_bin = tt2->GetMinimumBin();
    min_bin = tt2->GetXaxis()->GetBinCenter(min_bin);
    tt2->GetXaxis()->SetRangeUser(-200,1000);

    TF1 *f1 = new TF1("f1", "landau",min_bin, min_bin+3.5*fscale);
    tt2->Fit("f1", "Rq");

    min = f1->GetParameter(1) - 1.5*f1->GetParameter(2);
    max = f1->GetParameter(1) + 1.5*f1->GetParameter(2);

    tt2->Fit("landau","Q","",min,max);
    lfgain[i]=300*1.0/landau->GetParameter(1);


    gPad->SetLogy();
//    delete tt2;
    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(5) << lfgain[i] <<endl;
  }

  TCanvas *c3 = new TCanvas("c3","c3",1200,1200);
  c3->Divide(4,4);
  for(i=0;i<16;i++){
    c3->cd(i+1);
    TH1F *tt3 = new TH1F("tt3",Form("FADC_Ls2_ra%d",i+1),500,4000,8000);
    T->Draw(Form("L.s2.ra[%d]>>tt3",i,i),tritype);
    tt3->SetXTitle(Form("L.s2.ra[%d]",i));
    max_bin = tt3->GetMaximumBin();
    min = tt3->GetBinCenter(max_bin) - 40;
    max = tt3->GetBinCenter(max_bin) + 40;
    tt3->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 2*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 2*gaus->GetParameter(2);

    tt3->Fit("gaus","Q","",min,max);

//    delete tt1;
    ped_val = gaus->GetParameter(1);
    ped_wid = gaus->GetParameter(2);

    gPad->SetLogy();

    ped[i] = ped_val;
    ped_width[i] = ped_wid;

//    delete tt3;
    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(5) << setiosflags(ios::fixed) << setprecision(2) << ped_val << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(1) << ped_wid <<endl;
  }

  Double_t rfgain[16];

  myfile << setiosflags(ios::left) << setw(2) << "#" << "   ";
  myfile << setiosflags(ios::left) << setw(5) << "gain" << endl;

  fscale=100;
  TCanvas *c4 = new TCanvas("c4","c4",1200,1200);
  c4->Divide(4,4);
  for(i=0;i<16;i++){
    c4->cd(i+1);
    TH1F *tt4 = new TH1F("tt4",Form("Ls2_rap_%d",i+1),300,-200,1000);
    T->Draw(Form("L.s2.ra_p[%d]>>tt4",i,i),tritype);
    tt4->SetXTitle(Form("L.s2.ra_p[%d]",i));
    tt4->GetXaxis()->SetRangeUser(0,190);
    min_bin = tt4->GetMinimumBin();
    min_bin = tt4->GetXaxis()->GetBinCenter(min_bin);
    tt4->GetXaxis()->SetRangeUser(-200,1000);

    TF1 *f1 = new TF1("f1", "landau",min_bin, min_bin+3.5*fscale);
    tt4->Fit("f1", "Rq");

    min = f1->GetParameter(1) - 1.5*f1->GetParameter(2);
    max = f1->GetParameter(1) + 1.5*f1->GetParameter(2);

    tt4->Fit("landau","Q","",min,max);
    rfgain[i]=300*1.0/landau->GetParameter(1);


    gPad->SetLogy();

//    delete tt4;
    myfile << setiosflags(ios::left) << setw(2) << setiosflags(ios::fixed) << setprecision(1) << i << "   ";
    myfile << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(5) << rfgain[i] <<endl;
  }



  c1->Print("Lscin.pdf[");
  c1->Print("Lscin.pdf");
  c2->Print("Lscin.pdf");
  c3->Print("Lscin.pdf");
  c4->Print("Lscin.pdf");
  c4->Print("Lscin.pdf]");
  myfile.close();

}

