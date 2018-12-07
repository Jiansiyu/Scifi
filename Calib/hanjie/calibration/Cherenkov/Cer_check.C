void Cer_check()
{
  Int_t run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
//  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));

  cout << T->GetEntries() << endl;

/*  
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(5,2); 
  TH2F *h1[10],*hh1[10];
  for(int ii=0;ii<10;ii++)
   {  c1->cd(ii+1); 
      const char* htitle = Form("h1_%d",ii);
      h1[ii] = new TH2F(htitle,htitle,10,50,150,10,200,400);
      T->Draw(Form("R.cer.a_c[%d]:FbusR.cer.a_c[%d]>>h1_%d",ii,ii,ii),Form("R.cer.noverflow[%d]==0",ii));

      TProfile *prof = h1[ii]->ProfileX();
      prof->Fit("pol1","Q");
      Double_t p0;
      p0=pol1->GetParameter(1);
      cout<<"!!!:   "<<p0<<endl;
   }
  c1->Draw();
*/

  TCanvas *c2 = new TCanvas("c1","c1",1200,1200);
  TH2F *h1 = new TH2F("h1","cer_asum without cut",1000,0,10000,2000,0,21000);
  TH2F *h2 = new TH2F("h2","cer_ausm with overflow cut",1000,0,10000,2000,0,21000);
  TH2F *h3 = new TH2F("h3","cer_asum with tr.n=1 & trigger cut",1000,0,10000,2000,0,21000);
  TH2F *h4 = new TH2F("h4","cer_asum with tr.n=1 & trigger & overflow cut",1000,0,10000,2000,0,21000);


  TCut noverflow = "R.cer.noverflow[0]==0 && R.cer.noverflow[1]==0 &&R.cer.noverflow[2]==0 && R.cer.noverflow[3]==0 && R.cer.noverflow[4]==0 && R.cer.noverflow[5]==0 && R.cer.noverflow[6]==0 && R.cer.noverflow[7]==0 && R.cer.noverflow[8]==0 && R.cer.noverflow[9]==0";
  TCut c1 = "R.tr.n==1 && (DR.evtypebits>>5)&1";

  c2->Divide(2,2);
  c2->cd(1);
  T->Draw("R.cer.asum_c:FbusR.cer.asum_c>>h1","","colz");

  c2->cd(2);
  T->Draw("R.cer.asum_c:FbusR.cer.asum_c>>h2",noverflow,"colz");

  c2->cd(3);
  T->Draw("R.cer.asum_c:FbusR.cer.asum_c>>h3",c1,"colz");

  c2->cd(4);
  T->Draw("R.cer.asum_c:FbusR.cer.asum_c>>h4",c1+noverflow,"colz");



  c2->Draw();
  Int_t n1 = T->GetEntries();
  Int_t n2 = T->GetEntries(noverflow); 
  Int_t n3 = T->GetEntries(c1);
  Int_t n4 = T->GetEntries(c1+noverflow);


  cout<<n1<<"   "<<n2<<"   "<<n3<<"  "<<n4<<endl;
 
  c2->Print("Rcer_compare.pdf");

}
