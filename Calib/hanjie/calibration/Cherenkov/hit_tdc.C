#include "GetFile.h"
void hit_tdc()
{
  int arm=-1; //LHRS: arm=0; RHRS: arm=1;
  TString kin;
  TChain* T=GetTree(arm,kin);

  if(arm==-1){cout<<"Don't know which arm"<<endl;exit(0);}
  int LHRS=0,RHRS=0;
  if(arm==0)LHRS=1;
  if(arm==1)RHRS=1;

 
 
  T->SetBranchAddress("R.s0.trx",s0x);
  T->SetBranchAddress("R.s2.trx",s2x);
  T->SetBranchAddress("R.cer.trx",cerx);
  T->SetBranchAddress("R.cer.t",tcer);

  T->SetBranchAddress("R.gold.p",&goldp);
  T->SetBranchAddress("R.tr.n",&trn);
  T->SetBranchAddress("DR.evtypebits",&trigger);

  T->SetBranchAddress("R.ps.e",&ps);
  T->SetBranchAddress("R.sh.e",&sh);

  TGraph *gtrk[20];
  TMultiGraph *mg = new TMultiGraph();
  Double_t Ep;
  
//  ofstream myfile;
//  myfile.open("track_x.txt");

  Int_t nentries = T->GetEntries();
  Int_t trigger2;
  Int_t kk=0;
  for(int ii=0;ii<nentries && kk<20;ii++)
   {
      T->GetEntry(ii);
      trigger2=(Int_t)trigger;
      Ep = (ps+sh)/(1000*goldp);
      if(Ep>0.75 && trn==1 && ((trigger2>>5)&1) && tcer[5]>1850 && tcer[5]<1884){         
//        myfile<<s0x[0]<<"  "<<cerx[0]<<"  "<<s2x[0]<<endl;
          gtrk[kk] = new TGraph();
          gtrk[kk] ->SetPoint(0,s0x[0],s0z);
          gtrk[kk] ->SetPoint(1,cerx[0],cerz);
          gtrk[kk] ->SetPoint(2,s2x[0],s2z);
          gtrk[kk]->SetMarkerColor(kk);
          mg->Add(gtrk[kk]);
          kk++;
       }

   }

  mg->Draw("APL");




}
