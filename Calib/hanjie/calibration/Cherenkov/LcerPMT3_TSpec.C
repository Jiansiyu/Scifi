void LcerPMT3(int run_number)
{
     ofstream myfile;
     myfile.open("LHRS_PMT3.txt",fstream::app);

     TChain *T=new TChain("T");
     TString File=Form("/chafs1/work1/tritium/pass1/tritium_%d.root",run_number);

     if (!gSystem->AccessPathName(File)){
     T->Add(Form("/chafs1/work1/tritium/pass1/tritium_%d*.root",run_number));
     }
     else {
        myfile<<"-1  -1"<<endl;
        exit(0);
     }

     Double_t nentries=T->GetEntries();
     if(nentries<10000){
        myfile<<run_number<<"  "<<"-1"<<endl;
        exit(0);
     }

    int LHRS=0,RHRS=0;
    if(run_number<20000)LHRS=1;
    else RHRS=1;

    TCanvas *c1=new TCanvas("c1","c1",1500,1500);
    TH1F *hcer3=new TH1F("hcer3","hcer3",200,-150,1000);
    if(LHRS)T->Draw("L.cer.a_c[3]>>hcer3","(DL.evtypebits>>2)&1");
    gPad->SetLogy();     

    TSpectrum *ss=new TSpectrum(2);
    Int_t nfound=ss->Search(hcer3,3);
    c1->Update();
    cout<<nfound<<" peaks are found"<<endl;
    Double_t *xpeaks=ss->GetPositionX();
    cout<<xpeaks[0]<<"   "<<xpeaks[1]<<endl;

    



}
