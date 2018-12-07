void Cer_check_1()
{
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));

  cout << T->GetEntries() << endl;

  TCut noverflow = "R.cer.noverflow[0]==0 && R.cer.noverflow[1]==0 &&R.cer.noverflow[2]==0 && R.cer.noverflow[3]==0 && R.cer.noverflow[4]==0 && R.cer.noverflow[5]==0 && R.cer.noverflow[6]==0 && R.cer.noverflow[7]==0 && R.cer.noverflow[8]==0 && R.cer.noverflow[9]==0";

  TCut good_ele = "R.cer.asum_c>2000 && R.tr.n==1 && (DR.evtypebits>>5)&1";

  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
//  c1->Divide(2,1);
  TH1F *pad[10];
//  c1->cd(1);
  for(int ii=0;ii<10;ii++)
   {
      TString title=Form("pad%d",ii);
      pad[ii] = new TH1F(title,"Cer overflow for each paddle",13,-1,12);
      if(ii==0){ 
         T->Draw(Form("R.cer.noverflow[%d]*1>>pad%d",ii,ii),Form("R.cer.noverflow[%d]>0",ii));
         pad[0]->SetMaximum(2000); 
       }
      else 
         T->Draw(Form("R.cer.noverflow[%d]*(%d+1)>>pad%d",ii,ii,ii),Form("R.cer.noverflow[%d]>0",ii),"same");
   }
/*
  c1->cd(2);
  TH1F *s0[2];
  s0[0]=new TH1F("s0A","s0A",4,1,3);
  s0[1]=new TH1F("s0B","s0B",4,1,3);
  T->Draw("R.s0.loverflow>>s0A","R.s0.loverflow>0");
  T->Draw("R.s0.roverflow*2>>s0B","R.s0.roverflow>0","same");
  s0[0]->SetMaximum(5000);

  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
  TH1F *shh1 = new TH1F("shh1","shower for overflow events",800,-100,5000);
  TH1F *shh2 = new TH1F("shh2","shower for overflow events with good eletron cut",800,-100,5000);
  TH1F *shh3 = new TH1F("shh3","shower for non-overflow events",800,-100,5000);
  TH1F *shh4 = new TH1F("shh4","shower for non-overflow events with good electron cut",800,-100,5000);

  c2->Divide(2,2);
  c2->cd(1);
  T->Draw("R.sh.e+R.ps.e>>shh3",noverflow,"");
  c2->cd(2);
  T->Draw("R.sh.e+R.ps.e>>shh4",noverflow+good_ele);
  c2->cd(3);
  T->Draw("R.sh.e+R.ps.e>>shh1",!noverflow,"");
  gPad->SetLogy();
  c2->cd(4);
  T->Draw("R.sh.e+R.ps.e>>shh2",!noverflow+good_ele);

  gPad->SetLogy();
//  shh2->SetLineColor(2);

  TCanvas *c3 = new TCanvas("c3","c3",1200,1200);
  TH1F *ep1 = new TH1F("ep1","E/p for non-overflow events",100,0,2);
  TH1F *ep2 = new TH1F("ep2","E/p for overflow events",100,0,2);

  c3->Divide(2,1);
  c3->cd(1);
  T->Draw("(R.sh.e+R.ps.e)/(R.tr.p[0]*1000)>>ep1",noverflow,"");
  c3->cd(2);
  T->Draw("(R.sh.e+R.ps.e)/(R.tr.p[0]*1000)>>ep2",!noverflow,"");


  TCanvas *c4 = new TCanvas("c4","c4",1200,1200);
  TH2F *CervsS0 = new TH2F("CervsS0","cer overflow vs s0 overflow",20,-1,2,20,-1,2);
  TH2F *CervsS0cut = new TH2F("CervsS0cut","cer overflow vs s0 overflow with good electron cut",20,-1,2,20,-1,2);


  Int_t nceroverflow=0,ns0overflow=0;
  Int_t cers0=0,s0nocer=0,cernos0=0;


  Double_t cerover[10],s0lover,s0rover;
  T->SetBranchAddress("R.cer.noverflow",cerover);
  T->SetBranchAddress("R.s0.loverflow",&s0lover);
  T->SetBranchAddress("R.s0.roverflow",&s0rover);
  Int_t nentries = T->GetEntries();*/
/*  for(int ii=0;ii<nentries;ii++){
      T->GetEntry(ii);
      int jj=0;
      while(jj<10){
          if(cerover[jj]==1)nceroverflow=1;
          jj++;
       }
      if(s0lover==1 || s0rover==1)ns0overflow=1;
     
      if(nceroverflow==1 && ns0overflow==1)cers0++;
      if(nceroverflow==0 && ns0overflow==1)s0nocer++;
      if(nceroverflow==1 && ns0overflow==0)cernos0++;
 
      nceroverflow=0;
      ns0overflow=0;
   }

  cout<<cers0<<"  "<<s0nocer<<"  "<<cernos0<<endl;
*/
/*
  c4->Divide(2,1);
  c4->cd(1);
 
  TCut nonoverflow = "R.cer.noverflow[0]==0 && R.cer.noverflow[1]==0 &&R.cer.noverflow[2]==0 && R.cer.noverflow[3]==0 && R.cer.noverflow[4]==0 && R.cer.noverflow[5]==0 && R.cer.noverflow[6]==0 && R.cer.noverflow[7]==0 && R.cer.noverflow[8]==0 && R.cer.noverflow[9]==0 && R.s0.loverflow==0 && R.s0.roverflow==0";
  
 
  T->Draw("R.cer.noverflow[0]+R.cer.noverflow[1]+R.cer.noverflow[2]+R.cer.noverflow[3]+R.cer.noverflow[4]+R.cer.noverflow[5]+R.cer.noverflow[6]+R.cer.noverflow[7]+R.cer.noverflow[8]+R.cer.noverflow[9]:(R.s0.loverflow+R.s0.roverflow)>>CervsS0",!nonoverflow,"lego");

  c4->cd(2);

  T->Draw("R.cer.noverflow[0]+R.cer.noverflow[1]+R.cer.noverflow[2]+R.cer.noverflow[3]+R.cer.noverflow[4]+R.cer.noverflow[5]+R.cer.noverflow[6]+R.cer.noverflow[7]+R.cer.noverflow[8]+R.cer.noverflow[9]:(R.s0.loverflow+R.s0.roverflow)>>CervsS0cut",!nonoverflow+good_ele,"lego");



  c1->Print("Check_Cer.pdf[");
  c1->Print("Check_Cer.pdf");
  c2->Print("Check_Cer.pdf");
  c3->Print("Check_Cer.pdf");
  c4->Print("Check_Cer.pdf");
  c4->Print("Check_Cer.pdf]");
*/


}
