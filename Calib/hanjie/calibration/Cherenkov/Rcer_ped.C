void Rcer_ped()
{
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
//  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));
  T->Add(Form("/chafs2/work1/triton/hanjie/Rootfiles/tritium_%d*.root",run_number));

  cout << T->GetEntries() << endl;
  
  Double_t min, max, temp;
  Double_t ped_val,ped_wid;
  Int_t max_bin;
  Int_t i, ii;
  Double_t ped[10],ped_width[10];
  Double_t gain[10];

  TCut tritype = "";//"((DR.evtypebits>>1)&1)";
 
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(5,2); 
  for(i=0;i<10;i++){
    c1->cd(i+1);
    TH1F *tt1 = new TH1F("tt1",Form("Ped%d",i+1),300,5500,7500);
    T->Draw(Form("R.cer.ped[%d]>>tt1",i));
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

    ped[i] = ped_val;
    ped_width[i] = ped_wid; 

    cout<<i<<" "<<ped[i]<<endl;

  } 



}

