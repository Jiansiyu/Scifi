     TCanvas *c4 = new TCanvas("c4");
     TH1F *hp7=new TH1F("hp7","Lrb.BPMB.y",1000,-8,10);
     T2->Draw("1000*Lrb.BPMB.y>>hp7");
     hp7->SetLineColor(4);
     hp7->Scale(nT1/nT2);

     TH1F *hp8=new TH1F("hp8","Lrb.BPMB.y",1000,-8,10);
     hp8->SetLineColor(2);
     T1->Draw("1000*Lrb.BPMB.y>>hp8","","same");

