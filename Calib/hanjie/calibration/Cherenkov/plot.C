#include "GetFile.h"
void plot()
{
  int arm=-1; //LHRS: arm=0; RHRS: arm=1;
  TString kin;
  TChain* T=GetTree(arm,kin);

  if(arm==-1){cout<<"Don't know which arm"<<endl;exit(0);}
  int LHRS=0,RHRS=0;
  if(arm==0)LHRS=1;
  if(arm==1)RHRS=1;


     cout << T->GetEntries() << endl;
 
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
     if(LHRS) f1=new TFile("Lcer_plots.root","RECREATE");
     if(RHRS) f1=new TFile("Rcer_plots.root","RECREATE"); 

     TH1F *hcer_p[10];
     TH1F *hcer_p_cut[10];
     TH1F *hcer_c[10];
     TH1F *hcer_c_cut[10];
     TH2F *hta[10];
     TH1F *htdc[10];
     TCanvas *c1 = new TCanvas("c1");
     c1->Divide(2,5);
     TCanvas *c2 = new TCanvas("c2");
     c2->Divide(2,5);
     TCanvas *c3 = new TCanvas("c3");
     c3->Divide(2,5);
     TCanvas *c4 = new TCanvas("c4");
     c4->Divide(2,5);
     TCanvas *c5 = new TCanvas("c5");
     c5->Divide(5,2);
     TCanvas *c6 = new TCanvas("c6");
     c6->Divide(5,2);

 
     TCut tcer;
     for(int ii=0;ii<10;ii++)
      {  
         if(LHRS)tcer = Form("L.cer.t[%d]>0",ii);
         if(RHRS)tcer = Form("R.cer.t[%d]>0",ii);
         c1->cd(ii+1);
	 hcer_p[ii] = new TH1F(Form("hcer_p%d",ii),Form("cer.a_p[%d]",ii),150,-200,800);
         if(LHRS)T->Draw(Form("L.cer.a_p[%d]>>hcer_p%d",ii,ii),trigger2);
         if(RHRS)T->Draw(Form("R.cer.a_p[%d]>>hcer_p%d",ii,ii),trigger2);
         gPad->SetLogy();

         c2->cd(ii+1);
         hcer_p_cut[ii] = new TH1F(Form("hcer_p_cut%d",ii),Form("cer.a_p[%d]",ii),200,0,2000);
         if(LHRS)T->Draw(Form("L.cer.a_p[%d]>>hcer_p_cut%d",ii,ii),trigger2+Ep+tcer+TRK);
         if(RHRS)T->Draw(Form("R.cer.a_p[%d]>>hcer_p_cut%d",ii,ii),trigger2+Ep+tcer+TRK);

         c3->cd(ii+1);
         hcer_c[ii] = new TH1F(Form("hcer_c%d",ii),Form("cer.a_c[%d]",ii),200,0,2000);
         if(LHRS)T->Draw(Form("L.cer.a_c[%d]>>hcer_c%d",ii,ii),trigger2);
         if(RHRS)T->Draw(Form("R.cer.a_c[%d]>>hcer_c%d",ii,ii),trigger2);
	 gPad->SetLogy();

         c4->cd(ii+1);
         hcer_p[ii] = new TH1F(Form("hcer_c_cut%d",ii),Form("cer.a_c[%d]",ii),200,0,2000);
         if(LHRS)T->Draw(Form("L.cer.a_c[%d]>>hcer_c_cut%d",ii,ii),trigger2+Ep+tcer+TRK);
         if(RHRS)T->Draw(Form("R.cer.a_c[%d]>>hcer_c_cut%d",ii,ii),trigger2+Ep+tcer+TRK);

         c5->cd(ii+1);
         hta[ii] = new TH2F(Form("hta%d",ii),Form("cer.a_c[%d]:cer.t[%d]",ii,ii),500,1000,2000,800,0,2000);
         if(LHRS)T->Draw(Form("L.cer.a_c[%d]:L.cer.t[%d]>>hta%d",ii,ii,ii),trigger2+Ep+TRK,"COLZ");
         if(RHRS)T->Draw(Form("R.cer.a_c[%d]:R.cer.t[%d]>>hta%d",ii,ii,ii),trigger2+Ep+TRK,"COLZ");
         //gPad->SetLogy();

         c6->cd(ii+1);
         htdc[ii] = new TH1F(Form("htdc%d",ii),Form("cer.t[%d]",ii),500,1500,2500);
         if(LHRS)T->Draw(Form("L.cer.t[%d]>>htdc%d",ii,ii),trigger2+Ep+TRK+tcer);
         if(RHRS)T->Draw(Form("R.cer.t[%d]>>htdc%d",ii,ii),trigger2+Ep+TRK+tcer);


      }

    f1->Write();

    if(LHRS){
         c1->Print("Lcer_plots.pdf[");
         c1->Print("Lcer_plots.pdf");
         c2->Print("Lcer_plots.pdf");
         c3->Print("Lcer_plots.pdf");
         c4->Print("Lcer_plots.pdf");
         c5->Print("Lcer_plots.pdf");
         c6->Print("Lcer_plots.pdf");
         c6->Print("Lcer_plots.pdf]");
     }

    if(RHRS){
         c1->Print("Rcer_plots.pdf[");
         c1->Print("Rcer_plots.pdf");
         c2->Print("Rcer_plots.pdf");
         c3->Print("Rcer_plots.pdf");
         c4->Print("Rcer_plots.pdf");
         c5->Print("Rcer_plots.pdf");
         c6->Print("Rcer_plots.pdf");
         c6->Print("Rcer_plots.pdf]");
     }



}
