using namespace std;
void LcerPMT3(int run_number)
{
     ofstream myfile;
     myfile.open("LHRS_PMT3.txt",fstream::app);

     TChain *T=new TChain("T");
//     TString File=Form("/chafs1/work1/tritium/pass1/tritium_%d.root",run_number);
     TString File=Form("/chafs1/work1/tritium/Rootfiles/tritium_%d.root",run_number);

     if (!gSystem->AccessPathName(File)){
//     T->Add(Form("/chafs1/work1/tritium/pass1/tritium_%d*.root",run_number));
     T->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",run_number));
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
 
  TCanvas *c1 = new TCanvas("c1","c1",1200,1200);
  TH1F *hcer3=new TH1F("hcer3","hcer3",300,-150,800);
  T->Draw("L.cer.a_c[3]>>hcer3");//,"(DL.evtypebits>>2)&1");
  Int_t  max_bin = hcer3->GetMaximumBin();
  Double_t  min = hcer3->GetBinCenter(max_bin) - 40;
  Double_t  max = hcer3->GetBinCenter(max_bin) + 40;
  TF1 *f1=new TF1("f1","gaus",min,max);
  hcer3->Fit("f1","NQR");
  min = f1->GetParameter(1) - 2*f1->GetParameter(2);
  max = f1->GetParameter(1) + 2*f1->GetParameter(2);

  TF1 *f2=new TF1("f2","gaus",min,max);
  hcer3->Fit("f2","NQR");

  Double_t  ped_val = f2->GetParameter(1);
  Double_t  ped_wid = f2->GetParameter(2);    

  myfile <<run_number<<"  "<<ped_val<<"  "<<ped_wid<<endl;
  myfile.close();
  delete c1;
  exit(0);
}

