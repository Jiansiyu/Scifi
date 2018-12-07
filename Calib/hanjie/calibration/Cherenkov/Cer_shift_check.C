#include "GetFile.h"
void Cer_shift_check()
{
  int arm=-1; //LHRS: arm=0; RHRS: arm=1;
  TString kin;
  TChain* T1=GetTree(arm,kin);
  Double_t n1=T1->GetEntries();

  TChain* T2=GetTree(arm,kin);
  Double_t n2=T2->GetEntries();
  cout<<n1<<"   "<<n2<<endl;  


  TH1F *hcer_p1[10];
  TH1F *hcer_p2[10];
  TH1F *hcer_c1[10];
  TH1F *hcer_c2[10];

  TCanvas *c1=new TCanvas("c1","c1",1500,1500);
  c1->Divide(2,5);
  TCanvas *c2=new TCanvas("c2","c2",1500,1500);
  c2->Divide(2,5);
  for(int ii=0;ii<10;ii++){
     c1->cd(ii+1);
     hcer_p1[ii] = new TH1F(Form("hcer_p1%d",ii),Form("cer.a_p[%d]",ii),150,-200,1500);
     T1->Draw(Form("R.cer.a_p[%d]>>hcer_p1%d",ii,ii));
     hcer_p1[ii]->SetLineColor(2);

     hcer_p2[ii] = new TH1F(Form("hcer_p2%d",ii),Form("cer.a_p[%d]",ii),150,-200,1500);
     T2->Draw(Form("R.cer.a_p[%d]>>hcer_p2%d",ii,ii),"","same");
         gPad->SetLogy();
      
     c2->cd(ii+1);
     hcer_c1[ii] = new TH1F(Form("hcer_c1%d",ii),Form("cer.a_c[%d]",ii),150,-200,1500);
     T1->Draw(Form("R.cer.a_c[%d]>>hcer_c1%d",ii,ii));
     hcer_c1[ii]->SetLineColor(2);

     hcer_c2[ii] = new TH1F(Form("hcer_c2%d",ii),Form("cer.a_c[%d]",ii),150,-200,1500);
     T2->Draw(Form("R.cer.a_c[%d]>>hcer_c2%d",ii,ii),"","same");
         gPad->SetLogy();

   }
  TCanvas *c3=new TCanvas("c3","c3",1500,1500);
  TH1F *hcers1=new TH1F("hcers1","hcers1",1000,0,15000);
  TH1F *hcers2=new TH1F("hcers2","hcers2",1000,0,15000);
  T1->Draw("R.cer.asum_c>>hcers1");
  hcers1->SetLineColor(2);
  T2->Draw("R.cer.asum_c>>hcers2","","same");
 
  TCanvas *c4=new TCanvas("c4","c4",1500,1500);
  c4->Divide(2,5);
  TCanvas *c5=new TCanvas("c5","c5",1500,1500);
  c5->Divide(2,5);
  TH1F *hFcer_p1[10];
  TH1F *hFcer_p2[10];
  TH1F *hFcer_c1[10];
  TH1F *hFcer_c2[10];

  for(int ii=0;ii<10;ii++){
     c4->cd(ii+1);
     hFcer_p1[ii] = new TH1F(Form("hFcer_p1%d",ii),Form("Fcer.a_p[%d]",ii),150,-200,1500);
     T1->Draw(Form("FbusR.cer.a_p[%d]>>hFcer_p1%d",ii,ii));
     hFcer_p1[ii]->SetLineColor(2);

     hFcer_p2[ii] = new TH1F(Form("hFcer_p2%d",ii),Form("Fcer.a_p[%d]",ii),150,-200,1500);
     T2->Draw(Form("FbusR.cer.a_p[%d]>>hFcer_p2%d",ii,ii),"","same");
      gPad->SetLogy();


     c5->cd(ii+1);
     hFcer_c1[ii] = new TH1F(Form("hFcer_c1%d",ii),Form("Fcer.a_c[%d]",ii),150,-200,1500);
     T1->Draw(Form("FbusR.cer.a_c[%d]>>hFcer_c1%d",ii,ii));
     hFcer_c1[ii]->SetLineColor(2);

     hFcer_c2[ii] = new TH1F(Form("hFcer_c2%d",ii),Form("Fcer.a_c[%d]",ii),150,-200,1500);
     T2->Draw(Form("FbusR.cer.a_c[%d]>>hFcer_c2%d",ii,ii),"","same");
         gPad->SetLogy();

   }
  TCanvas *c6=new TCanvas("c6","c6",1500,1500);
  TH1F *hFcers1=new TH1F("hFcers1","hFcers1",800,0,3000);
  TH1F *hFcers2=new TH1F("hFcers2","hFcers2",800,0,3000);
  T1->Draw("FbusR.cer.asum_c>>hFcers1");
  hFcers1->SetLineColor(2);
  T2->Draw("FbusR.cer.asum_c>>hFcers2","","same");
 

}
