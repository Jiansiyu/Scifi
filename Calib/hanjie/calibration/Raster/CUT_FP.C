void Cut_FP()

{ cout<<"Please enter a run number"<<endl;
  //Int_t runNo;
  //cin>>runNo; 
  TChain *T=new TChain("T");
  T->Add("/chafs2/work1/triton/hanjie/Rootfiles/tritium_843*.root");
  TH2F *FP_x=new TH2F("FP_x","FP #theta VS FP x(CK,Trk1,T1,E/P)",100,-1,1,100,-0.2,0.2);
  TH2F *FP_y= new TH2F ("FP_y","FP #phi VS FP y(CK,Trk1,T1,E/P)",100,-0.1,0.1,100,-0.1,0.1);
  TH2F *FP_y2= new TH2F ("FP_y2","FP #phi VS FP y(after FP cut)",100,-0.1,0.1,100,-0.1,0.1);

  FP_x->SetMarkerSize(3);
  FP_y->SetMarkerSize(3);
  FP_y2->SetMarkerSize(3);


 TCanvas *c1=new TCanvas("c1","c1",3600,1200);
 c1->Divide(3,1);
c1->cd(1);
  T->Draw("L.tr.th:L.tr.x>>FP_x",cut1,"");
c1->cd(2);
  T->Draw("L.tr.y:L.tr.ph>>FP_y",cut1,"");
c1->Update();


c1->cd(1);
        TCutG* cutg;
        cutg = (TCutG*)(TVirtualPad::Pad()->WaitPrimitive("CUTG", "CutG"));
        c1->Update();
        cutg->SetName("cut_FP"); 
        cutg->SetVarX("L.tr.x");
        cutg->SetVarY("L.tr.th");
        cutg->SetLineColor(kMagenta);
        cutg->SetLineWidth(5);
        cutg->Draw("PL");
        c1->Update();

c1->cd(3);
 
        TCut DrawCut2=cut1+TCut("cut_FP");
         (TCutG*)gROOT->FindObject("cut_FP");       
        T->Draw("L.tr.y:L.tr.ph>>FP_y2",DrawCut2, " ");
        c1->Update();






}

