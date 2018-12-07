void plot()
{
     TFile *f1 = new TFile("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_2073.root");
     TFile *f2 = new TFile("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_2174.root");

     TTree *T1 = (TTree*)f1->Get("T");
     TTree *T2 = (TTree*)f2->Get("T");

     TFile *fo = new TFile("raster_plots.root","RECREATE");

     TCanvas *c1 = new TCanvas("c1");
     TH2F *hx1=new TH2F("hx1","Lrb.Raster.rawcur.x:Lrb.Raster2.rawcur.x",1000,50000,95000,1000,50000,95000);
     T2->Draw("Lrb.Raster.rawcur.x:Lrb.Raster2.rawcur.x>>hx1");
     hx1->SetStats(0);
     hx1->SetMarkerStyle(3001);
     hx1->SetMarkerColor(4);

     TH2F *hx2=new TH2F("hx2","Lrb.Raster.rawcur.x:Lrb.Raster2.rawcur.x",1000,50000,95000,1000,50000,95000);
     hx2->SetMarkerStyle(3001);
     hx2->SetMarkerColor(2);
     T1->Draw("Lrb.Raster.rawcur.x:Lrb.Raster2.rawcur.x>>hx2","","same");
    
    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(32);
    t1->SetTextSize(0.06);
    t1->SetTextColor(1);
    t1->DrawLatex(0.15, 0.8, "FADC raster x");
    t1->SetTextColor(4);
    t1->SetTextSize(0.06);
    t1->DrawLatex(0.15, 0.6, "run 2101");
    t1->SetTextColor(2);
    t1->SetTextSize(0.06);
    t1->DrawLatex(0.15, 0.7, "run 2073");

     TCanvas *c2 = new TCanvas("c2");
     TH2F *hx3=new TH2F("hx3","FbusLrb.Raster.rawcur.x:FbusLrb.Raster2.rawcur.x",500,3000,6000,500,3000,6000);
     T2->Draw("FbusLrb.Raster.rawcur.x:FbusLrb.Raster2.rawcur.x>>hx3");
     hx3->SetStats(0);
     hx3->SetMarkerStyle(3001);
     hx3->SetMarkerColor(4);

     TH2F *hx4=new TH2F("hx4","FbusLrb.Raster.rawcur.x:FbusLrb.Raster2.rawcur.x",500,3000,6000,500,3000,6000);
     hx4->SetMarkerStyle(3001);
     hx4->SetMarkerColor(2);
     T1->Draw("FbusLrb.Raster.rawcur.x:FbusLrb.Raster2.rawcur.x>>hx4","","same");

    TLatex *t2 = new TLatex();
    t2->SetNDC();
    t2->SetTextFont(32);
    t2->SetTextSize(0.06);
    t2->SetTextColor(1);
    t2->DrawLatex(0.15, 0.8, "FastBus Raster x");
    t2->SetTextColor(4);
    t2->SetTextSize(0.06);
    t2->DrawLatex(0.15, 0.6, "run 2101");

    t2->SetTextColor(2);
    t2->SetTextSize(0.06);
    t2->DrawLatex(0.15, 0.7, "run 2073");

     TCanvas *c3 = new TCanvas("c3");
     TH2F *hy1=new TH2F("hy1","Lrb.Raster.rawcur.y:Lrb.Raster2.rawcur.y",1000,30000,120000,1000,30000,120000);
     T2->Draw("Lrb.Raster.rawcur.y:Lrb.Raster2.rawcur.y>>hy1");
     hy1->SetStats(0);
     hy1->SetMarkerStyle(3001);
     hy1->SetMarkerColor(4);

     TH2F *hy2=new TH2F("hy2","Lrb.Raster.rawcur.y:Lrb.Raster2.rawcur.y",1000,30000,120000,1000,30000,120000);
     hy2->SetMarkerStyle(3001);
     hy2->SetMarkerColor(2);
     T1->Draw("Lrb.Raster.rawcur.y:Lrb.Raster2.rawcur.y>>hy2","","same");

    TLatex *t3 = new TLatex();
    t3->SetNDC();
    t3->SetTextFont(32);
    t3->SetTextSize(0.06);
    t3->SetTextColor(1);
    t3->DrawLatex(0.15, 0.8, "FADC raster y");
    t3->SetTextColor(4);
    t3->SetTextSize(0.06);
    t3->DrawLatex(0.15, 0.6, "run 2101");
    t3->SetTextColor(2);
    t3->SetTextSize(0.06);
    t3->DrawLatex(0.15, 0.7, "run 2073");

     TCanvas *c4 = new TCanvas("c4");
     TH2F *hy3=new TH2F("hy3","FbusLrb.Raster.rawcur.y:FbusLrb.Raster2.rawcur.y",500,1000,8000,500,1000,8000);
     T2->Draw("FbusLrb.Raster.rawcur.y:FbusLrb.Raster2.rawcur.y>>hy3");
     hy3->SetStats(0);
     hy3->SetMarkerStyle(3001);
     hy3->SetMarkerColor(4);

     TH2F *hy4=new TH2F("hy4","FbusLrb.Raster.rawcur.y:FbusLrb.Raster2.rawcur.y",500,1000,8000,500,1000,8000);
     hy4->SetMarkerStyle(3001);
     hy4->SetMarkerColor(2);
     T1->Draw("FbusLrb.Raster.rawcur.y:FbusLrb.Raster2.rawcur.y>>hy4","","same");

    TLatex *t4 = new TLatex();
    t4->SetNDC();
    t4->SetTextFont(32);
    t4->SetTextSize(0.06);
    t4->SetTextColor(1);
    t4->DrawLatex(0.15, 0.8, "FastBus raster y");
    t4->SetTextColor(4);
    t4->SetTextSize(0.06);
    t4->DrawLatex(0.15, 0.6, "run 2101");
    t4->SetTextColor(2);
    t4->SetTextSize(0.06);
    t4->DrawLatex(0.15, 0.7, "run 2073");
    fo->Write();
/*
    c1->Print("Raster_plots.pdf[");
    c1->Print("Raster_plots.pdf");
    c2->Print("Raster_plots.pdf");
    c3->Print("Raster_plots.pdf");
    c4->Print("Raster_plots.pdf");
    c4->Print("Raster_plots.pdf]");
*/

}
