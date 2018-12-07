#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include  <stdio.h>
#include  <stdlib.h>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"
const Double_t PS_ROW[24]={-1.1615,-1.0605,-0.9595,-0.8585,-0.7575,-0.6565,-0.5555,-0.4545,-0.3535,-0.2525,-0.1515,-0.0505,0.0505,0.1515,0.2525,0.3535,0.4545,0.5555,0.6565,0.7575,0.8585,0.9595,1.0605,1.1615};
const Double_t SH_ROW[15]={-1.06755,-0.91505,-0.76255,-0.61005,-0.45755,-0.30505,-0.15255,-5e-05,0.15245,0.30495,0.45745,0.60995,0.76245,0.91495,1.06745};
const Double_t SH_COL[5]={-0.30505,-0.15255,-5e-05,0.15245,0.30495};


//Int_t SH_Row(Double_t TRX);
//Int_t PS_Row(Double_t TRX);
//Int_t SH_Col(Double_t TRY);



void RHRS_Shower_Calibration()
{ 
 // fit the pedstal
Double_t PS_ped[48]={327.342,525.506,448.622,309.219,515.543,623.86,527.058,449.576,660.938,432.01,418.603,530.723,595.848,530.361,540.469,632.16,305.253,323.212,273.47, 338.373,382.769,348.317,435.691,359.230,387.365,345.125,323.464,313.399,430.194,386.985,390.524,404.996,565.326,506.102,566.811,600.717,617.108,592.972,623.825,612.019,579.874,551.490,584.548,533.972,590.690,558.896,567.097,661.603};
Double_t SH_ped[75]={261.177,292.099,295.677,275.008,349.188,343.603,441.674,361.941,322.644,317.207,235.2,301.52,474.26,346.748,399.8,353.355,462.01,359.049,305.008,404.033,409.822,408.965,373.13,461.364,364.776,277.403,378.082,508.873,478.258,501.52,504.717,499.924,505.395,462.045,574.383,519.359,562.734,604.605,510.987,566.312,473.726,508.898,591.895,617.277,547.193,594.994,568.522,529.608,513.08,613.375,659.801,619.165,647.219,562.653,473.815,550.536,493.548,683.755,670.524,617.385,370.643,303.027,251.455,257.958,389.917,437.147,457.793,495.107,283.248,253.145,242.183,249.572, 475.582,356.51, 374.355};

TChain *T=new TChain("T");
//T->Add("/volatile/halla/triton/Tritium_Rootfiles/tritium_90849*.root");
T->Add("/chafs1/work1/tritium/rootfiles_1.6_root6/tritium_93634.root");
TString sh_name1=Form("RHRS_SH_Ped_%d.pdf[",93634);
TString sh_name2=Form("RHRS_SH_Ped_%d.pdf",93634);
TString sh_name3=Form("RHRS_SH_Ped_%d.pdf]",93634);

/*TCanvas *c1=new TCanvas("c1","For fitting the pedstals",1200,1200);
c1->SetLogy();
c1->SetTickx();
c1->SetGridx();


for(Int_t i=0;i<48;i++)
{ TH1F *ps_ped=new TH1F("ps_ped",Form("pedstal for preshower %d",i),600,200,800);
  c1->cd();
  T->Draw(Form("R.ps.a[%d]>>ps_ped",i),"DR.evtypebits>>5&1");
  Int_t maxbin=ps_ped->GetMaximumBin();
  Double_t x0=ps_ped->GetXaxis()->GetBinCenter(maxbin);
  ps_ped->Fit("gaus","","",x0-20,x0+20);
  Double_t x1=gaus->GetParameter(1);
  Double_t x2=gaus->GetParameter(2);
  ps_ped->Fit("gaus","","",x1-2.0*x2,x1+2.0*x2);
  PS_ped[i]=gaus->GetParameter(1);
  if(i==0) {c1->SaveAs(sh_name1);}
  c1->SaveAs(sh_name2);
 delete ps_ped;}


for(Int_t i=0;i<75;i++)
{ TH1F *sh_ped=new TH1F("sh_ped",Form("pedstal for shower %d",i),1200,0,1200);
  c1->cd();
  T->Draw(Form("R.sh.a[%d]>>sh_ped",i),"DR.evtypebits>>5&1");
  Int_t maxbin=sh_ped->GetMaximumBin();
  Double_t x0=sh_ped->GetXaxis()->GetBinCenter(maxbin);
  sh_ped->Fit("gaus","","",x0-20,x0+20);
  Double_t x1=gaus->GetParameter(1);
  Double_t x2=gaus->GetParameter(2);
  sh_ped->Fit("gaus","","",x1-2.0*x2,x1+2.0*x2);
  SH_ped[i]=gaus->GetParameter(1);
  c1->SaveAs(sh_name2);
  if(i==74) {c1->SaveAs(sh_name3);}
 delete sh_ped;}

*/
Double_t SS[123][123]={0};
Double_t S0[123]={0};


Double_t ps_trx[100];
Double_t sh_trx[100];
Double_t sh_try[100];
Double_t e1[100];
Double_t e2[100];
Double_t cer[100];
Double_t trn[100];
Double_t tr_p[100];
Double_t ps_a[48];
Double_t sh_a[75];

    T->SetBranchStatus("*",0);
    T->SetBranchStatus("R.ps.e",1);
    T->SetBranchStatus("R.sh.e",1);
    T->SetBranchStatus("R.cer.asum_c",1);
    T->SetBranchStatus("R.tr.n",1);
    T->SetBranchStatus("R.tr.p",1);
    T->SetBranchStatus("R.ps.trx",1);
    T->SetBranchStatus("R.sh.try",1);
    T->SetBranchStatus("R.sh.trx",1);
    T->SetBranchStatus("R.ps.a",1);
    T->SetBranchStatus("R.sh.a",1);
   // T->SetBranchStatus("R.tr.p",1);

    T->SetBranchAddress("R.ps.e",&e1);
    T->SetBranchAddress("R.sh.e",&e2);
    T->SetBranchAddress("R.ps.trx",&ps_trx);
    T->SetBranchAddress("R.sh.trx",&sh_trx);
    T->SetBranchAddress("R.ps.a",&ps_a);
    T->SetBranchAddress("R.sh.a",&sh_a);
    T->SetBranchAddress("R.sh.try",&sh_try);
    T->SetBranchAddress("R.cer.asum_c",&cer);
    T->SetBranchAddress("R.tr.n",&trn);
    T->SetBranchAddress("R.tr.p",&tr_p);

   Int_t ps_counter[24];
for(Int_t i=0;i<24;i++) {ps_counter[i]=0;}
   Int_t sh_counter[75];

for(Int_t i=0;i<75;i++) {sh_counter[i]=0;}

   Int_t No_Entry=T->GetEntries();
   
   for(Int_t j=0;j<No_Entry;j++)
{  T->GetEntry(j);
 if( j%100000==0) {cout<<j<<" events has been percessed"<<endl;}
 
  if(cer[0]>2000&&((e1[0]+e2[0])/1000.0/tr_p[0]>0.8)&&trn[0]>0)
   
{  
//  cout<<trn[0]<<endl;
  Int_t ps_x=PS_Row(ps_trx[0]);
  if(ps_x>=0) {ps_counter[ps_x]++;}
// cout<<ps_trx[0]<<"  "<<ps_x<<endl;
   Int_t sh_x=SH_Row(sh_trx[0]);
   Int_t sh_y=SH_Col(sh_try[0]);
   if(sh_x>=0&&sh_y>=0)
   {Int_t N_sh=15*sh_y+sh_x;
    sh_counter[N_sh]++; }
 // cout<<"trx="<<sh_trx[0]<<"  and try="<<sh_try[0]<<" and block="<<N_sh<<endl;


  Double_t AA[123];

for(Int_t rt=0;rt<123;rt++)
{AA[rt]=0;}



  // build the cluster for ps
  for(Int_t c_ps=ps_x-1;c_ps<=ps_x+1;c_ps++)
{ if(c_ps>=0&&c_ps<24&&(ps_a[c_ps]-PS_ped[c_ps])>0) {AA[c_ps]=(ps_a[c_ps]-PS_ped[c_ps])/1000.0;}
  if(c_ps>=0&&c_ps<24&&(ps_a[c_ps+24]-PS_ped[c_ps+24])>0) {AA[c_ps+24]=(ps_a[c_ps+24]-PS_ped[c_ps+24])/1000.0;}
}
    
 //build the cluster for sh
  for(Int_t c_sh=N_sh-1;c_sh<N_sh+1;c_sh++)
{ 
 if(c_sh>=0&&c_sh<75&&(sh_a[c_sh]-SH_ped[c_sh])>0) {AA[c_sh+48]=(sh_a[c_sh]-SH_ped[c_sh])/1000.00;}
 if((c_sh-15)>=0&&(c_sh-15)<75&&(sh_a[c_sh-15]-SH_ped[c_sh-15])>0) {AA[c_sh+48-15]=(sh_a[c_sh-15]-SH_ped[c_sh-15])/1000.00;}
 if((c_sh+15)>=0&&(c_sh+15)<75&&(sh_a[c_sh+15]-SH_ped[c_sh+15])>0) {AA[c_sh+48+15]=(sh_a[c_sh+15]-SH_ped[c_sh+15])/1000.00;}
}

for(Int_t ii=0; ii<123;ii++)


{ S0[ii]+=tr_p[0]*AA[ii];
  for(Int_t jj=0;jj<123;jj++)
  {SS[ii][jj]+=AA[ii]*AA[jj];}}
}
 }

  const Int_t nrVar  = 123;
  TMatrixD AAA(123,123);
  TMatrixD S_mat(123,1);
  for(int i=0;i<123;i++)
  { S_mat(i,0)=S0[i];
    for(int j=0;j<123;j++)
    { AAA(i,j)=SS[i][j]; } }

  TMatrixD ST(TMatrixD::kTransposed,S_mat);

  TMatrixD A_1 = AAA;
  Double_t d;
  A_1.Invert(&d);
  TMatrixD C = (A_1*AAA);
  TMatrixD Coef = (A_1*S_mat);


 ofstream ooo("matrix_90849.txt");
  if (ooo.is_open())
  { for (int j_sig=0; j_sig<48; j_sig++)
    { ooo<<setw(14)<<PS_ped[j_sig];
      ooo<<endl;
 } }

      ooo<<"################"<<endl;
 if (ooo.is_open())
  { for (int j_sig=0; j_sig<75; j_sig++)
    { ooo<<setw(14)<<SH_ped[j_sig];
      ooo<<endl;
 } }

      ooo<<"################"<<endl;
 if (ooo.is_open())
  { for (int j_sig=0; j_sig<123; j_sig++)
    { ooo<<setw(14)<<Coef(j_sig,0)<<endl;
     if(j_sig==47) {ooo<<"########"<<endl;}

 } }

  ooo.close();




for(Int_t j=0;j<24;j++)
{ cout<<"for preshower block ["<<j<<"] we get "<<ps_counter[j]<<"events "<<endl;}

for(Int_t j=0;j<75;j++)
{ cout<<"for shower block ["<<j<<"] we get "<<sh_counter[j]<<"events "<<endl;}

}


Int_t PS_Row(Double_t TRX)
{Int_t nn=-1;
const Double_t dd=0.101;
for(Int_t index=0;index<24;index++)
{if((TRX>PS_ROW[index]-dd)&&TRX<PS_ROW[index]+dd))
  {nn=index;}}
return nn;}


Int_t SH_Col(Double_t TRY)
{Int_t nn=-1;
const Double_t dd=0.1525;
for(Int_t index=0;index<5;index++)
{if((TRY>SH_COL[index]-dd)&&TRY<SH_COL[index]+dd))
  {nn=index;}}
return nn;}

Int_t SH_Row(Double_t TRX)
{Int_t nn=-1;
const Double_t dd=0.1525;
for(Int_t index=0;index<15;index++)
{if((TRX>SH_ROW[index]-dd)&&TRX<SH_ROW[index]+dd))
  {nn=index;}}
return nn;}








