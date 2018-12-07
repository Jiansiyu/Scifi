void Ls0_cali()
{
  Int_t right_or_left, run_number;    
  cout << "enter run_number: ";
  cin >> run_number;
  TChain *T = new TChain("T");
  T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
//  T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));

  cout << T->GetEntries() << endl;
  
  Double_t min, max, temp;
  Double_t ped_val,ped_wid;
  Int_t max_bin;
  Double_t lfgain;
  ofstream myfile;
  myfile.open("L_s0_ped.txt");//,fstream::app);

  TCut tritype = "";//"((DR.evtypebits>>1)&1)";
 
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(2,1);
    c1->cd(1);
    TH1F *tt1 = new TH1F("tt1","FADC L.s0.la",400,4000,7000);
    T->Draw("L.s0.la>>tt1",tritype);
    tt1->SetXTitle("L.s0.la");
    tt1->GetXaxis()->SetRangeUser(5000,5800);
    max_bin = tt1->GetMaximumBin();
    min = tt1->GetBinCenter(max_bin) - 30;
    max = tt1->GetBinCenter(max_bin) + 30;
    tt1->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 1.5*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 1.5*gaus->GetParameter(2);

    tt1->Fit("gaus","Q","",min,max);
    ped_val = gaus->GetParameter(1);
    ped_wid = gaus->GetParameter(2);    
    tt1->GetXaxis()->SetRangeUser(4000,7000);

    //gPad->SetLogy();
    myfile<<"s0la"<<endl;
    myfile<<"ped: "<<ped_val <<endl;
    myfile<<"ped wid: "<< ped_wid <<endl;
  

  Double_t fscale=100;
    c1->cd(2);
    TH1F *tt2 = new TH1F("tt2","FADC Ls0.la_p",300,-200,1000);
    T->Draw("L.s0.la_p>>tt2",tritype);
    tt2->SetXTitle("L.s0.la_p");
    max_bin = tt2->GetMaximumBin();
    min = tt2->GetBinCenter(max_bin) - 30;
    max = tt2->GetBinCenter(max_bin) + 50;
    tt2->Fit("landau","NQ","",min,max);
    min = landau->GetParameter(1) - 1.5*landau->GetParameter(2);
    max = landau->GetParameter(1) + 1.5*landau->GetParameter(2);
    tt2->Fit("landau","Q","",min,max);

    lfgain=400*1.0/landau->GetParameter(1);
    //gPad->SetLogy();

    myfile<<"gain: "<<lfgain<<endl;
 
  TCanvas *c2 = new TCanvas("c2","c2",1200,1200);
    c2->Divide(2,1);
    c2->cd(1);
    TH1F *tt3 = new TH1F("tt3","FbusL.s0_la",180,200,2000);
    T->Draw("FbusL.s0.la>>tt3",tritype);
    tt3->SetXTitle("FbusL.s0.la");
    max_bin = tt3->GetMaximumBin();
    min = tt3->GetBinCenter(max_bin) - 30;
    max = tt3->GetBinCenter(max_bin) + 30;
    tt3->Fit("landau","NQ","",min,max);
    min = landau->GetParameter(1) - 1.5*landau->GetParameter(2);
    max = landau->GetParameter(1) + 1.5*landau->GetParameter(2);
    tt3->Fit("landau","Q","",min,max);

    Double_t dpk = landau->GetParameter(1);
    c2->cd(2);
    TH1F *tt7 = new TH1F("tt7","FbusL.s0_la",100,400,700);
    T->Draw("FbusL.s0.la>>tt7",tritype);
    tt7->SetXTitle("FbusL.s0.la");
    max_bin = tt7->GetMaximumBin();
    min = tt7->GetBinCenter(max_bin) - 20;
    max = tt7->GetBinCenter(max_bin) + 20;
    tt7->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 2*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 2*gaus->GetParameter(2);

    tt7->Fit("gaus","Q","",min,max);

    ped_val = gaus->GetParameter(1);
    ped_wid = gaus->GetParameter(2);

    //gPad->SetLogy();
//    tt3->Draw();
    Double_t gain=250*1.0/(dpk-ped_val);
    TLine *l1 = new TLine(ped_val,0,ped_val,tt3->GetMaximum(););
    l1->SetLineColor(2);
//    l1->Draw();

 //w   cout << "Lcer" << i << endl;
    myfile <<"Fbus.la: " << ped_val << "   ";
    myfile << gain<<endl;


  TCanvas *c3 = new TCanvas("c3","c3",1200,1200);
  c3->Divide(2,1);
    c3->cd(1);
    TH1F *tt4 = new TH1F("tt4","FADC L.s0.ra",600,3000,6000);
    T->Draw("L.s0.ra>>tt4",tritype);
    tt4->SetXTitle("L.s0.ra");
    tt4->GetXaxis()->SetRangeUser(4000,4600);
    max_bin = tt4->GetMaximumBin();
    min = tt4->GetBinCenter(max_bin) - 20;
    max = tt4->GetBinCenter(max_bin) + 20;
    tt4->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 1*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 1*gaus->GetParameter(2);

    tt4->Fit("gaus","Q","",min,max);

    ped_val = gaus->GetParameter(1);
    ped_wid = gaus->GetParameter(2);

    tt4->GetXaxis()->SetRangeUser(3000,6000);
   // gPad->SetLogy();
    myfile<<"s0ra"<<endl;
    myfile<<"ped: "<<ped_val <<endl;
    myfile<<"ped wid: "<< ped_wid <<endl;

    c3->cd(2);
    TH1F *tt5 = new TH1F("tt5","FADC L.s0.ra_p",300,-200,1000);
    T->Draw("L.s0.ra_p>>tt5",tritype);
    tt5->SetXTitle("L.s0.ra_p");
    max_bin = tt5->GetMaximumBin();
    min = tt5->GetBinCenter(max_bin) - 10;
    max = tt5->GetBinCenter(max_bin) + 30;
    tt5->Fit("landau","NQ","",min,max);
    min = landau->GetParameter(1) - 1.5*landau->GetParameter(2);
    max = landau->GetParameter(1) + 1.5*landau->GetParameter(2);
    tt5->Fit("landau","Q","",min,max);

    lfgain=400*1.0/landau->GetParameter(1);
   // gPad->SetLogy();

    myfile<<"gain: "<<lfgain<<endl;

  TCanvas *c4 = new TCanvas("c4","c4",1200,1200);
    c4->Divide(2,1);
    c4->cd(1);
    TH1F *tt6 = new TH1F("tt6","FBusLs0_ra",180,200,2000);
    T->Draw("FbusL.s0.ra>>tt6",tritype);
    tt6->SetXTitle("FbusL.s0.ra");
    max_bin = tt6->GetMaximumBin();
    min = tt6->GetBinCenter(max_bin) - 25;
    max = tt6->GetBinCenter(max_bin) + 25;
    tt6->Fit("landau","NQ","",min,max);
    min = landau->GetParameter(1) - 1*landau->GetParameter(2);
    max = landau->GetParameter(1) + 1*landau->GetParameter(2);
    tt6->Fit("landau","Q","",min,max);

    dpk = landau->GetParameter(1);
    c4->cd(2);
    TH1F *tt8 = new TH1F("tt8","FBusLs0_ra",100,300,600);
    T->Draw("FbusL.s0.ra>>tt8",tritype);
    max_bin = tt8->GetMaximumBin();
    min = tt8->GetBinCenter(max_bin) - 10;
    max = tt8->GetBinCenter(max_bin) + 10;
    tt8->Fit("gaus","NQ","",min,max);
    min = gaus->GetParameter(1) - 1*gaus->GetParameter(2);
    max = gaus->GetParameter(1) + 1*gaus->GetParameter(2);

    tt8->Fit("gaus","Q","",min,max);

    ped_val = gaus->GetParameter(1);
    ped_wid = gaus->GetParameter(2);

    //gPad->SetLog();

//    tt6->Draw();
    gain=250*1.0/(dpk-ped_val);
    TLine *l2 = new TLine(ped_val,0,ped_val,tt6->GetMaximum(););
    l2->SetLineColor(2);
//    l2->Draw();

    myfile <<"Fbus.ra: " << ped_val << "   ";
    myfile << gain<<endl;
  

  c1->Print("Ls0.pdf[");
  c1->Print("Ls0.pdf");
  c2->Print("Ls0.pdf");
  c3->Print("Ls0.pdf");
  c4->Print("Ls0.pdf");
  c4->Print("Ls0.pdf]");
  myfile.close();

}

