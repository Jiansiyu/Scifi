
#include <stdio.h>

void L_s2_timing(){
//int main(){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);

  const Float_t ctos = 0.5E-9; //0.5ns per TDC channel  


  //Initialize Histograms
  TH2 *hleft[16];
  TH2 *hright[16];
  TH2 *hdiff[16];
  TProfile *pdiff[16];
  TH1 *hsum[16];
    
    Int_t s2l_low=1700;
    Int_t s2l_high=1800;
    Int_t s2r_low=1650;
    Int_t s2r_high=1800;
    Int_t s2l_bin=(s2l_high-s2l_low);
    Int_t s2r_bin=(s2r_high-s2r_low)
    ;
    Int_t s2diff_low=s2l_low-s2r_high;//s2 diff: Left-Right
    Int_t s2diff_high=s2l_high-s2r_low;
    Int_t s2diff_bin=s2diff_high-s2diff_low;
    Int_t s2sum_high=s2l_high+s2r_high;
    Int_t s2sum_low=s2l_low+s2r_high;
    Int_t s2sum_bin=s2sum_high-s2sum_low;
    

  for(Int_t i=0;i<16;i++){
    
      hleft[i] = new TH2F(Form("hleft[%d]",i),Form("S2 Left PMT TDC vs Y_s2: Paddle %d",i+1),300,-0.3,0.3,s2l_bin,s2l_low,s2l_high);
    hleft[i]->GetXaxis()->SetTitle("Y_s2 [m]"); hleft[i]->GetXaxis()->CenterTitle();
    hleft[i]->GetYaxis()->SetTitle("TDC Channel"); hleft[i]->GetYaxis()->CenterTitle();
    
    hright[i] = new TH2F(Form("hright[%d]",i),Form("S2 Right PMT TDC vs. Y_s2: Paddle %d",i+1),300,-0.3,0.3,s2r_bin,s2r_low,s2r_high);
    hright[i]->GetXaxis()->SetTitle("Y_s2 [m]"); hright[i]->GetXaxis()->CenterTitle();
    hright[i]->GetYaxis()->SetTitle("TDC Channel"); hright[i]->GetYaxis()->CenterTitle();

    hdiff[i] = new TH2F(Form("hdiff[%d]",i),Form("S2 Left - Right vs Y_s2: Paddle %d",i+1),300,-0.3,0.3,s2diff_bin,s2diff_low,s2diff_high);
    hdiff[i]->GetXaxis()->SetTitle("Y_s2 [m]"); hdiff[i]->GetXaxis()->CenterTitle();
    hdiff[i]->GetYaxis()->SetTitle("TDC Channel"); hdiff[i]->GetYaxis()->CenterTitle();

    if(i==0) pdiff[i] = new TProfile(Form("pdiff[%d]",i),Form("S2 Left - Right vs Y_s2: Paddle %d",i+1),6,-0.3,0.3,s2diff_low,s2diff_high);
    else pdiff[i] = new TProfile(Form("pdiff[%d]",i),Form("S2 Left - Right vs. Track Projection Y: Paddle %d",i+1),90,-0.3,0.3,s2diff_low,s2diff_high);
    pdiff[i]->GetXaxis()->SetTitle("Y_s2 [m]"); pdiff[i]->GetXaxis()->CenterTitle();
    pdiff[i]->GetYaxis()->SetTitle("TDC Channel"); pdiff[i]->GetYaxis()->CenterTitle();
    pdiff[i]->SetLineColor(kBlack);pdiff[i]->SetMarkerColor(kBlack);

    //hsum[i] = new TH2F(Form("hsum[%d]",i),Form("S2 Left + Right vs. Track Projection Y: Paddle %d",i+1),300,-0.3,0.3,10000,0,10000);
    hsum[i] = new TH1F(Form("hsum[%d]",i),Form("S2 Left + Right vs. Track Projection Y: Paddle %d",i+1),200,2800,3000);
    hsum[i]->GetXaxis()->SetTitle("Y_s2 [m]"); hsum[i]->GetXaxis()->CenterTitle();
    hsum[i]->GetYaxis()->SetTitle("TDC Channel"); hsum[i]->GetYaxis()->CenterTitle();

  }

  TChain * t = new TChain("T");

  t->Add("/Users/tong/Tritium_analysis/Rootfiles/tritium_811*.root");
 // t->Add("../Ar_replay/gmp_13997*.root");
    
    
  //Define Variables
  Double_t L_tr_n,L_cer_asum_c,L_ps_e,L_sh_e;
  Double_t L_tr_p[100],L_s2_try[100];
  Double_t L_s2_lt[16],L_s2_rt[16];
  Double_t L_s2_nthit;
  Double_t L_s2_t_pads[16];
  Double_t evtypebits;
  Double_t L_s0_trpath[100],L_s2_trpath[100];
  Double_t L_s0_lt[10],L_s0_rt[10];
  Double_t L_s0_nthit;

    
    
    Double_t L=1.7;
    Double_t l=0.432;
    Double_t c0=1.21577e+08;
    Double_t c_s2=1.26193e+08;
    Double_t elec_v=3.00e+08;
    Float_t  tof;//in second
    Float_t  TOF;//in Channel
    Float_t  SUM;
    
    
  
    //Double_t Retiming[100];
    //t->SetBranchStatus("th.Retiming_L",1);
    
    
    
    
    
    
    
  //Define Branch Status/Addresses
  t->SetBranchStatus("*",0);
  t->SetBranchStatus("L.tr.n",1);
  t->SetBranchStatus("L.tr.p",1);
  t->SetBranchStatus("L.cer.asum_c",1);
  t->SetBranchStatus("L.prl1.e",1);
  t->SetBranchStatus("L.prl2.e",1);
  t->SetBranchStatus("L.s2.lt",1);
  t->SetBranchStatus("L.s2.rt",1);
  t->SetBranchStatus("L.s2.try",1);
  t->SetBranchStatus("L.s2.nthit",1);
  t->SetBranchStatus("L.s2.t_pads",1);
  t->SetBranchStatus("L.s0.trpath",1);
  t->SetBranchStatus("L.s2.trpath",1);
  t->SetBranchStatus("L.s0.lt",1);
  t->SetBranchStatus("L.s0.rt",1);
  t->SetBranchStatus("L.s0.nthit",1);
    
  t->SetBranchStatus("DL.evtypebits",1);

  t->SetBranchAddress("L.tr.n",&L_tr_n);
  t->SetBranchAddress("L.tr.p",L_tr_p);
  t->SetBranchAddress("L.cer.asum_c",&L_cer_asum_c);
  t->SetBranchAddress("L.prl1.e",&L_ps_e);
  t->SetBranchAddress("L.prl2.e",&L_sh_e);
  t->SetBranchAddress("L.s2.lt",L_s2_lt);
  t->SetBranchAddress("L.s2.rt",L_s2_rt);
  t->SetBranchAddress("L.s2.try",L_s2_try);
  t->SetBranchAddress("L.s2.nthit",&L_s2_nthit);
  t->SetBranchAddress("L.s2.t_pads",L_s2_t_pads);
  t->SetBranchAddress("L.s0.trpath",L_s0_trpath);
  t->SetBranchAddress("L.s2.trpath",L_s2_trpath);
  t->SetBranchAddress("L.s0.lt",L_s0_lt);
  t->SetBranchAddress("L.s0.rt",L_s0_rt);
  t->SetBranchAddress("L.s0.nthit",&L_s0_nthit);
  t->SetBranchAddress("DL.evtypebits",&evtypebits);
  //t->SetBranchAddress("DR.Retiming_L",Retiming);
    
  Int_t nentries = t->GetEntries();
  cout<<"Total Number of Events = "<<nentries<<endl;
  //Loop over events
   
  for(Int_t i=0;i<nentries;i++){
    
 //******************print how many events processed for every 100000 events*********//
   if(i%100000==0) cout << " events processed = " << i << endl;
 //**********************************************************************************//
      
      t->GetEntry(i);
      Int_t x=evtypebits;
      //For pads other than pad 0
    for(Int_t j=1;j<16;j++){
        if(L_tr_n==1 && L_cer_asum_c>2500&&L_s2_t_pads[0]==j&&(x>>1)&1&&L_s0_nthit==1&&(L_ps_e+L_sh_e)/L_tr_p[0]/1000>0.7)
    {
        tof=(L_s2_trpath[0]-L_s0_trpath[0])/elec_v;
        TOF=tof/ctos;
        SUM=(L_s0_lt[0]+L_s0_rt[0])-(L_s2_rt[j]+L_s2_lt[j])+(L/c0/ctos)-(l/c_s2/ctos)+3018.26-2*TOF;
        
        hleft[j]->Fill(L_s2_try[0],L_s2_lt[j]);
        hright[j]->Fill(L_s2_try[0],L_s2_rt[j]);
        hdiff[j]->Fill(L_s2_try[0],L_s2_lt[j]-L_s2_rt[j]);
        pdiff[j]->Fill(L_s2_try[0],L_s2_lt[j]-L_s2_rt[j]);
        hsum[j]->Fill(SUM);
	} } }
   
  //Make Plots
  
  TCanvas *c1[4],*c2[7],*c4[7];
  Int_t scin_count,pad_count;

  Int_t counter(16);
  Float_t pad[16],intercept[16],slope[16],slope_err[16];
  Float_t sum[16];
  

  for(Int_t i=0;i<4;i++){
    c1[i]= new TCanvas(Form("c1[%d]",i));
    c1[i]->Divide(2,4);
    pad_count=0;
    
    for(Int_t j=0;j<4;j++){
      scin_count = 4*i +j;
      pad_count++; c1[i]->cd(pad_count);
      hleft[scin_count]->Draw("col");
      pad_count++; c1[i]->cd(pad_count);
      hright[scin_count]->Draw("col");
    }
    
    if(i==0){
     c1[i]->Print("s2_timing.pdf[");
     c1[i]->Print("s2_timing.pdf");
    }
    else{
      c1[i]->Print("s2_timing.pdf");
    }
  }

  for(Int_t i=0;i<7;i++){
    c2[i]= new TCanvas(Form("c2[%d]",i));
    c2[i]->Divide(1,2);
    pad_count=0;
    for(Int_t j=0;j<2;j++){
      scin_count = 2*i + j+1;
      pad_count++; c2[i]->cd(pad_count);
    
        pdiff[scin_count]->Draw();
        pdiff[scin_count]->Fit("pol1","R","",-0.125,0.125);
        c2[i]->Update();
        
         intercept[scin_count] = pdiff[scin_count]->GetFunction("pol1")->GetParameter(0);
       
      c2[i]->Modified();
    }
      c2[i]->Print("s2_timing.pdf");
  }
   

  for(Int_t i=0;i<7;i++){
    c4[i]= new TCanvas(Form("c4[%d]",i));
    c4[i]->Divide(1,2);
    pad_count=0;
    for(Int_t j=0;j<2;j++){
      scin_count = 2*i + j+1;
      pad_count++;
      c4[i]->cd(pad_count);
      hsum[scin_count]->Draw();
      hsum[scin_count]->Fit("gaus");
      double xx=hsum[scin_count]->GetFunction("gaus")->GetParameter(2);
      double yy=hsum[scin_count]->GetFunction("gaus")->GetParameter(1);
      hsum[scin_count]->Fit("gaus","R","",yy-xx,yy+xx);
      c4[i]->Modified();
      sum[scin_count]=hsum[scin_count]->GetFunction("gaus")->GetParameter(1);
      cout<<"mean value="<<sum[scin_count]<<" haha"<<endl;
    }
    if(i==6){
      c4[i]->Print("s2_timing.pdf");
      c4[i]->Print("s2_timing.pdf]");
    }
    else c4[i]->Print("s2_timing.pdf");
  }

   cout<<"------------------------------------------------------\n";
    
   
    Double_t left[16],right[16];
    
    for (Int_t ii=1;ii<15;ii++)
    {right[ii]=(sum[ii]+intercept[ii])/2.0;
     left[ii]=(sum[ii]-intercept[ii])/2.0;
        cout<<"s2_left["<<ii<<"]="<<left[ii]<<";"<<endl;
        cout<<"s2_right["<<ii<<"]="<<right[ii]<<";"<<endl;
        
    }
    ofstream oofile("DB_test.txt");
    
    for (Int_t iii=1;iii<15;iii++)
        
    { oofile<<-right[iii]+3000.0<<"  ";
    }
    oofile<<endl;
    
    for (Int_t jjj=1;jjj<15;jjj++)
        
    { oofile<<-left[jjj]+3000.0<<"  ";
    }
    oofile<<endl;
    
    oofile.close();
  
}

