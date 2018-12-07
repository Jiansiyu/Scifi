void plot_bpm()
{
     TFile *f1 = new TFile("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_2060.root");
     TFile *f2 = new TFile("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_2101.root");

     TTree *T1 = (TTree*)f1->Get("T");
     TTree *T2 = (TTree*)f2->Get("T");

     TCut trip = "evLeftdnew_r>100";

     TCanvas *c1 = new TCanvas("c1");
     c1->Divide(2,2);
     c1->cd(1);
     TH1F *hp1=new TH1F("hp1","Lrb.BPMA.x",1000,-1,1);
     T2->Draw("1000*Lrb.BPMA.x>>hp1",trip);
     hp1->SetLineColor(4);
     Double_t nT1=T1->GetEntries(trip);
     Double_t nT2=T2->GetEntries(trip);
     hp1->Scale(nT1/nT2);
     hp1->SetStats(0);

     TH1F *hp2=new TH1F("hp2","Lrb.BPMA.x",1000,-1,1);
     hp2->SetLineColor(2);
     T1->Draw("1000*Lrb.BPMA.x>>hp2",trip,"same");

    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(32);
    t1->SetTextColor(4);
    t1->SetTextSize(0.06);
    t1->DrawLatex(0.15, 0.6, "run 2101");
    t1->SetTextColor(2);
    t1->SetTextSize(0.06);
    t1->DrawLatex(0.15, 0.7, "run 2073");

//     TCanvas *c2 = new TCanvas("c2");
     c1->cd(2);
     TH1F *hp3=new TH1F("hp3","Lrb.BPMB.x",1000,-3,2);
     T2->Draw("1000*Lrb.BPMB.x>>hp3",trip);
     hp3->SetLineColor(4);
     hp3->Scale(nT1/nT2);
     hp3->SetStats(0);

     TH1F *hp4=new TH1F("hp4","Lrb.BPMB.x",1000,-3,2);
     hp4->SetLineColor(2);
     T1->Draw("1000*Lrb.BPMB.x>>hp4",trip,"same");


    // TCanvas *c3 = new TCanvas("c3");
     c1->cd(3);
     TH1F *hp5=new TH1F("hp5","Lrb.BPMA.y",1000,-2,3);
     T2->Draw("1000*Lrb.BPMA.y>>hp5",trip);
     hp5->SetLineColor(4);
     hp5->Scale(nT1/nT2);
     hp5->SetStats(0);

     TH1F *hp6=new TH1F("hp6","Lrb.BPMA.y",1000,-2,3);
     hp6->SetLineColor(2);
     T1->Draw("1000*Lrb.BPMA.y>>hp6",trip,"same");

     //TCanvas *c4 = new TCanvas("c4");
     c1->cd(4);
     TH1F *hp7=new TH1F("hp7","Lrb.BPMB.y",1000,-2,2);
     T2->Draw("1000*Lrb.BPMB.y>>hp7",trip);
     hp7->SetLineColor(4);
     hp7->Scale(nT1/nT2);
     hp7->SetStats(0);

     TH1F *hp8=new TH1F("hp8","Lrb.BPMB.y",1000,-2,2);
     hp8->SetLineColor(2);
     T1->Draw("1000*Lrb.BPMB.y>>hp8",trip,"same");


}
