#include "GetFile.h"
void Check_tdc()
{
  int arm=-1; //LHRS: arm=0; RHRS: arm=1;
  TString kin;
  TChain* T=GetTree(arm,kin);

  if(arm==-1){cout<<"Don't know which arm"<<endl;exit(0);}
  int LHRS=0,RHRS=0;
  if(arm==0)LHRS=1;
  if(arm==1)RHRS=1;

  ofstream myfile;
  myfile.open("Rtdc_check.txt",fstream::app);


     TCut trigger2,Ep,TRK;
     if(LHRS){
          trigger2 = "(DL.evtypebits>>2)&1";
          Ep = "(L.prl1.e+L.prl2.e)/(1000*L.gold.p)>0.8";
          TRK = "L.tr.n==1";
      }

     if(RHRS){
          trigger2 = "(DR.evtypebits>>5)&1";
          Ep = "(R.sh.e+R.ps.e)/(1000*R.gold.p)>0.8";
          TRK = "R.tr.n==1";
      }

     TFile *f1;
     if(LHRS) f1=new TFile("./results/LChecktdc_plots.root","RECREATE");
     if(RHRS) f1=new TFile("./results/RChecktdc_plots.root","RECREATE");

     TH1F *htdc[10];
     TH1F *htdc_near[10];
     Double_t pk1[10],pk2[10];
     Double_t pk1_c[10],pk2_c[10];
     Double_t ratio[10],ratio1[10];
     Int_t binx_min=0,binx_max=0;
     Int_t max_bin1=0,max_bin2=0;

     TCut Acer5 = "R.cer.a_c[5]>1100";

     TCanvas *c1 = new TCanvas("c1");
     TCanvas *c2 = new TCanvas("c2");
     c1->Divide(5,2);
     c2->Divide(5,2);
     for(int ii=0;ii<10;ii++){
	 c1->cd(ii+1);
         htdc[ii] = new TH1F(Form("htdc%d",ii),Form("cer.t[%d]",ii),300,1700,2000);
	 T->Draw(Form("R.cer.t[%d]>>htdc%d",ii,ii),TRK+trigger2+Ep);
         TLine *l1 = new TLine(190,0,190,htdc[ii]->GetMaximum());
	 l1->Draw();

         pk1[ii]=htdc[ii]->Integral(170,184);
         pk2[ii]=htdc[ii]->Integral(200,220);
	 ratio[ii]=pk1[ii]/pk2[ii];

         c2->cd(ii+1);
         htdc_near[ii] = new TH1F(Form("htdc_near%d",ii),Form("cer.t[%d](cer.a_c[5]>1100)",ii),300,1700,2000);
         T->Draw(Form("R.cer.t[%d]>>htdc_near%d",ii,ii),TRK+trigger2+Ep+Acer5);
         TLine *l2 = new TLine(190,0,190,htdc_near[ii]->GetMaximum());
         l2->Draw();

         pk1_c[ii]=htdc_near[ii]->Integral(170,184);
         pk2_c[ii]=htdc_near[ii]->Integral(200,220);
         ratio1[ii]=pk1_c[ii]/pk2_c[ii];

      }

     for(int jj=0;jj<10;jj++){
         cout<<jj<<"  "<<ratio[jj]<<endl;
	 myfile<<jj<<"  "<<ratio[jj]<<endl;
     }

         cout<<"-----------PMT 5 main fired"<<endl;
         myfile<<"-----------PMT 5 main fired"<<endl;


     for(int jj=0;jj<10;jj++){
         cout<<jj<<"  "<<ratio1[jj]<<endl;
         myfile<<jj<<"  "<<ratio1[jj]<<endl;
     }


     f1->Write(); 


}
