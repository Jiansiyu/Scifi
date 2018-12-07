void plot_PMT3()
{
     TString file1;
     file1="LHRS_PMT3.txt";

     Double_t word[3];
     string line;
     ifstream infile1;
     infile1.open(file1);

     Double_t nrun[2478],pval[2478],pwid[2478];
     int ntotal=0;
     while(getline(infile1,line)){
           istringstream str(line);
           str>>word[0]>>word[1];
           if(word[0]==-1||word[1]==-1||word[0]>100000)continue;
           nrun[ntotal]=word[0];
           pval[ntotal]=word[1];
           str>>pwid[ntotal];
           ntotal++;
     }
     infile1.close();

     TCanvas *c1=new TCanvas("c1","c1",1500,1500);
     TGraph *gcer3 = new TGraph();
     Int_t jj=0;
     for(int ii=0;ii<ntotal;ii++){
         gcer3->SetPoint(ii,nrun[ii],pwid[ii]);
      }

     gcer3->SetMarkerStyle(7);
     gcer3->SetMarkerColor(2);
     gcer3->Draw("AP");
     gcer3->SetTitle("PMT3 ped width;run_number;");

}
