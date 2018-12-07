void plot_ped()
{
  Int_t right_or_left, run_number;
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
//  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));
  T->Add(Form("/chafs2/work1/triton/hanjie/Rootfiles/tritium_%d*.root",run_number));

  TCanvas *c1= new TCanvas("c1");
  c1->Divide(2,5);
  TH1F *hcer[10];
  Double_t nped[10]={6843.71,6871.62,6672.99,6982,6407.2,6503.83,6504.08,7187.54,7088.7,5988.15};
  for(int ii=0;ii<10;ii++)
   {
         c1->cd(ii+1);
         hcer[ii] = new TH1F(Form("hcer%d",ii),Form("cer.ped[%d]",ii),200,-300,300);
         T->Draw(Form("R.cer.ped[%d]-%f>>hcer%d",ii,nped[ii],ii));
         gPad->SetLogy();
	

   }

  c1->Print("Rcer_ped.pdf");




}
