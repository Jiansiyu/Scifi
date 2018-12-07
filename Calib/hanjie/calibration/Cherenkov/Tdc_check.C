#include "GetFile.h"
void Tdc_check()
{
  int arm=-1; //LHRS: arm=0; RHRS: arm=1;
  TString kin;
  TChain* T=GetTree(arm,kin);

  if(arm==-1){cout<<"Don't know which arm"<<endl;exit(0);}
  int LHRS=0,RHRS=0;
  if(arm==0)LHRS=1;
  if(arm==1)RHRS=1;

  TCut trigger2,Ep,TRK,Scer;
  if(LHRS){
       trigger2 = "(DL.evtypebits>>2)&1";
       Ep = "(L.prl1.e+L.prl2.e)/(1000*L.gold.p)>0.75";
       TRK = "L.tr.n==1";
       Scer = "L.cer.t[5]<1884 && L.cer.t[5]>1850";
   }

  if(RHRS){
       trigger2 = "(DR.evtypebits>>5)&1";
       Ep = "(R.sh.e+R.ps.e)/(1000*R.gold.p)>0.75";
       TRK = "R.tr.n==1";
       Scer = "R.cer.t[5]<1884 && R.cer.t[5]>1850";
   }

  TFile *f1;
  if(LHRS) f1=new TFile("Ltdc_check.root","RECREATE");
  if(RHRS) f1=new TFile("Rtdc_check.root","RECREATE");

  TCanvas *c1 = new TCanvas("c1");
  c1->Divide(3,3);
  TH2F *hcer[9];
  int kk=0;
  for(int ii=0;ii<10;ii++)
   {    c1->cd(kk+1);
	if(ii==5)continue;
        hcer[kk] = new TH2F(Form("hcer%d",ii),Form("cer.a_c[%d]:cer.t[%d]",ii,ii),200,1800,2000,500,0,2000);
        if(LHRS)T->Draw(Form("L.cer.a_c[%d]:L.cer.t[%d]>>hcer%d",ii,ii,ii),trigger2+Ep+TRK+Scer,"COLZ");
        if(RHRS)T->Draw(Form("R.cer.a_c[%d]:R.cer.t[%d]>>hcer%d",ii,ii,ii),trigger2+Ep+TRK+Scer,"COLZ");
        kk++;
   }

  TCanvas *c2 = new TCanvas("c2");
  c2->Divide(3,2);

  c2->cd(1);
  TH1F *hvz = new TH1F("hvz","target vertex z",100,-0.20,0.20);
  if(LHRS)T->Draw("L.tr.vz>>hvz",trigger2+Ep+TRK+Scer);
  if(RHRS)T->Draw("R.tr.vz>>hvz",trigger2+Ep+TRK+Scer);

  c2->cd(2);
  TH1F *htrx = new TH1F("htrx","tr.x",200,-0.8,0.8);
  if(LHRS) T->Draw("L.tr.x>>htrx",TRK+Ep+trigger2+Scer);
  if(RHRS) T->Draw("R.tr.x>>htrx",TRK+Ep+trigger2+Scer);

  c2->cd(3);
  TH1F *hs2trx = new TH1F("hs2trx","s2.trx",300,-1,1);
  if(LHRS)T->Draw("L.s2.trx>>hs2trx",TRK+Ep+trigger2+Scer);
  if(RHRS)T->Draw("R.s2.trx>>hs2trx",TRK+Ep+trigger2+Scer);

  c2->cd(4);
  TH1F *hs0trx = new TH1F("hs0trx","s0.trx",300,-1,1);
  if(LHRS)T->Draw("L.s0.trx>>hs0trx",TRK+Ep+trigger2+Scer);
  if(RHRS)T->Draw("R.s0.trx>>hs0trx",TRK+Ep+trigger2+Scer);

  c2->cd(5);
  TH2F *hcertrx = new TH2F("hcertrx","cer.trx:cer.try",10,-0.3,0.3,300,-1.0,1.0);
  if(LHRS)T->Draw("L.cer.trx:L.cer.try>>hcertrx",TRK+Ep+trigger2+Scer,"COLZ");
  if(RHRS)T->Draw("R.cer.trx:R.cer.try>>hcertrx",TRK+Ep+trigger2+Scer,"COLZ");

  c2->cd(6);
  TH2F *hs2xy = new TH2F("hs2xy","s2.trx:s2.try",10,-0.2,0.2,300,-1.2,1.2);
  if(LHRS)T->Draw("L.s2.trx:L.s2.try>>hs2xy",TRK+Ep+trigger2+Scer,"COLZ");
  if(RHRS)T->Draw("R.s2.trx:R.s2.try>>hs2xy",TRK+Ep+trigger2+Scer,"COLZ");



  f1->Write();

}
