#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"

using namespace std;

//Double_t xmin[17]={100., 0.78, 0.65, 0.49, 0.33, 0.20, 0.05, -0.09, -0.24, -0.38, -0.53, -0.68, -0.83, -0.97, -1.11, -1.3, -100.};
//Double_t xcen[17]={100., 0.82,0.675, 0.53, 0.385,0.24, 0.095,-0.05, -0.195,-0.34, -0.485,-0.63, -0.775,-0.92, -1.065,-1.21,-100.};
//Double_t xmax[17]={100., 0.87, 0.72, 0.57, 0.43, 0.29, 0.14, -0.01, -0.16, -0.30, -0.45, -0.60, -0.75, -0.89, -1.06, -1.2, -100.};


//const Double_t PS_ROW[24]={-1.1615,-1.0605,-0.9595,-0.8585,-0.7575,-0.6565,-0.5555,-0.4545,-0.3535,-0.2525,-0.1515,-0.0505,0.0505,0.1515,0.2525,0.3535,0.4545,0.5555,0.6565,0.7575,0.8585,0.9595,1.0605,1.1615};
//const Double_t PS_COL[2] = {-0.175,0.175};
//const Double_t SH_ROW[15]={-1.06755,-0.91505,-0.76255,-0.61005,-0.45755,-0.30505,-0.15255,-5e-05,0.15245,0.30495,0.45745,0.60995,0.76245,0.91495,1.06745};
//const Double_t SH_COL[5]={-0.30505,-0.15255,-5e-05,0.15245,0.30495};

Double_t xmin[24]={-1.21,-1.10,-1.00,-0.90,-0.80,-0.70,-0.60,-0.50,-0.40,-0.30,-0.20,-0.09,0.00,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.0,1.11};
Double_t xmax[24]={-1.10,-1.00,-0.90,-0.80,-0.70,-0.60,-0.50,-0.40,-0.30,-0.20,-0.09, 0.00,0.10, 0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.01,1.11,1.21};
Double_t ps_ymin[2] = {-0.35,0.0};
Double_t ps_ymax[2] = {0.0,0.35};


Double_t Sh_xmin[15] = {-1.14,-0.99,-0.83,-0.68,-0.53,-0.38,-0.22,-0.07,0.08,0.22,0.38,0.53,0.68,0.84,0.99};
Double_t Sh_xmax[15] = {-0.99,-0.83,-0.68,-0.53,-0.38,-0.22,-0.07,0.08,0.22,0.38,0.53,0.68,0.84,0.99,1.14};
Double_t Sh_ymin[5] =  { -0.38,-0.22,-0.07,0.07,0.23 };
Double_t Sh_ymax[5] =  { -0.22,-0.07,0.07,0.23,0.38  };


Double_t ymin_l=0.04, ymax_l=0.12;
Double_t ymin_r=-0.16, ymax_r=-0.06;
Double_t ymin = -0.16, ymax = 0.12;

//void PRL_Calibration(int runNo) { 
void Shower_cuts() { 
  //gROOT->LoadMacro("WIJ.C+");
  //WIJ();
  //    int runNo;
   int runNo[6];
    cout<<"First run number"<<endl;
    cin >> runNo[0];
   cout<<"Second run number"<<endl;
    cin >> runNo[1];
    cout<<"Third run number"<<endl;
    cin >> runNo[2];
    cout<<"Fourth run number"<<endl;
    cin >> runNo[3];
    cout<<"Fith run number"<<endl;
    cin >> runNo[4];
    //cout<<"Sixth run number"<<endl;
    //cin >> runNo[5];



     //cout <<"1"<<runNo[0]<<"\t"<<"2"<<runNo[1]<<endl;
  
	// which arm?
	char arm[2]; 
	if(runNo[0] < 90000) {
		//LHRS
		sprintf(arm,"L");
	} else {
		//RHRS
		sprintf(arm, "R");
		const int N = 48;
		const int Y = 75;
	}

	Double_t ped_ps[N]={0};
	Double_t sigma_ps[N] ={0};
	Double_t ped_sh[Y]={0};
	Double_t sigma_sh[Y]={0};



	//	TString RootFile=Form("/lustre/expphy/volatile/halla/triton/Tritium_Rootfiles%d.root",runNo);
	//	TString Base_Path = "/chafs1/work1/tritium/Rootfiles";
	//      TString Base_Path = "/volatile/halla/triton/Tritium_Rootfiles/";
	        TString Base_Path = "/chafs1/work1/tritium/M_rootfiles";
		 
		 //  TString RootFile=Form("/volatile/halla/triton/Tritium_Rootfiles/tritium_%i.root",runNo);
	

		TChain *T=new TChain("T");
		//             	T->Add(RootFile);
	// fit the pedestals
						
	for (int k=0; k<6;k++){
	  //	  TChain *T = new TChain("T");
	std::ostringstream step ;
	//      	for (int k=0; k<2;k++){
	
	step << Base_Path << "/tritium_" << runNo[k];
	TString convert = step.str().c_str();
	TString rootfile = convert + ".root";


	Long_t q=0;
	while  (! gSystem ->AccessPathName(rootfile)) {
	  T->Add(rootfile);
	  //TSLeft1->Add(rootfile);                                                                                                                                                               
	  cout << "T Branch Added"<<rootfile<<endl;
	  q++;
	  rootfile = convert + "_"+ q + ".root";
	}

	}
	
	ifstream preShower_ped;
	preShower_ped.open("ps_ped.txt");
	for (Int_t i =0;i<48;i++){
	  preShower_ped>>ped_ps[i];
	}

	ifstream Shower_ped;
	Shower_ped.open("sh_ped.txt");
	for (Int_t i =0;i<75;i++){
	  Shower_ped>>ped_sh[i];
	}


				
	/*
	TCanvas *c1=new TCanvas("c1","For fitting the pedestals",1200,1200);
	c1->SetLogy();
	c1->SetTickx();
	c1->SetGridx();
	c1->Divide(1,2);

	TString ped_name1=Form("%sHRS_SHOWER_Ped_%d.pdf[",arm,runNo);
	TString ped_name2=Form("%sHRS_SHOWER_Ped_%d.pdf",arm,runNo);
	TString ped_name3=Form("%sHRS_SHOWER_Ped_%d.pdf]",arm,runNo);

	for (Int_t i=0;i<48;i++){

		TH1F *ps_ped= new TH1F("ps_ped",Form("Pedstal for PreShower PMT %d",i),600,200,800);
		//	TH1F *sh_ped= new TH1F("sh_ped",Form("Pedstal for Shower PMT %d",i),600,200,800);

		//PRL1	
		c1->cd(1);
		T->Draw(Form("%s.ps.a[%d]>>ps_ped",arm,i),Form("(D%s.evtypebits>>4)&1",arm));

		Int_t binmax=ps_ped->GetMaximumBin();
		Double_t x1=ps_ped->GetXaxis()->GetBinCenter(binmax);

		TH1D *htmp = (TH1D*)ps_ped->Clone("htmp");
		htmp->GetXaxis()->SetRange(binmax-20, binmax+20);
		Double_t ped_fit_range = 2.5*htmp->GetRMS();

		ps_ped->Fit("gaus", "R", "", x1-ped_fit_range, x1+ped_fit_range);
		ps_ped->Fit("gaus", "R", "", gaus->GetParameter(1)-2.*gaus->GetParameter(2), gaus->GetParameter(1)+2.*gaus->GetParameter(2));
		delete htmp;

		ped_ps[i]=gaus->GetParameter(1);
		sigma_ps[i] = gaus->GetParameter(2);
		gPad->SetTickx();
		gPad->SetLogy();
		gPad->SetGridx();
	}
	for (Int_t i=0; i<75;i++){
		//PRL2
	  TH1F *sh_ped= new TH1F("sh_ped",Form("Pedstal for Shower PMT %d",i),600,200,800);
		c1->cd(2);
		T->Draw(Form("%s.sh.a[%d]>>sh_ped",arm,i),Form("(D%s.evtypebits>>4)&1",arm));
	
		Int_t binmax2=sh_ped->GetMaximumBin();
		Double_t x2=sh_ped->GetXaxis()->GetBinCenter(binmax2);

		TH1D *htmp2 = (TH1D*)sh_ped->Clone("htmp2");
		htmp2->GetXaxis()->SetRange(binmax2-20, binmax2+20);
		Double_t ped_fit_range2 = 2.5*htmp2->GetRMS();

		sh_ped->Fit("gaus", "R", "", x2-ped_fit_range2, x2+ped_fit_range2);
		sh_ped->Fit("gaus", "R", "", gaus->GetParameter(1)-2.*gaus->GetParameter(2), gaus->GetParameter(1)+2.*gaus->GetParameter(2));
		delete htmp2;

		ped_sh[i]=gaus->GetParameter(1);
		sigma_sh[i] = gaus->GetParameter(2);
		gPad->SetTickx();
		gPad->SetLogy();
		gPad->SetGridx();


		if(i==0) {
			c1->SaveAs(ped_name1);
			c1->SaveAs(ped_name2);
		}

		if(i==33) {
			c1->SaveAs(ped_name2);
			c1->SaveAs(ped_name3);
		}
	
		else c1->SaveAs(ped_name2);
		delete ps_ped;
		delete sh_ped;
	}

	*/
	// Perpare the Mathrix
	//1) extract the variable we need
	Double_t x[100];
	Double_t th[100];
	Double_t y[100];
	Double_t ph[100];
	Double_t p[100];
	Double_t chi2[100];
	Double_t ndof[100];
	Double_t scer[100];
	Double_t ps[48];
	Double_t sh[75];
	Double_t tr_x_ps[100];
	Double_t tr_y_ps[100];
	Double_t tr_x_sh[100];
	Double_t tr_y_sh[100];
       	Double_t e1,e2;
	Double_t n_tr;
        Double_t Rtr_vz[100];
	Int_t good_el=0;

	T->SetBranchStatus("*",0);  

	T->SetBranchStatus(Form("%s.ps.e",arm),1);
	T->SetBranchStatus(Form("%s.sh.e",arm),1);
	T->SetBranchStatus(Form("%s.cer.asum_c",arm),1);
	T->SetBranchStatus(Form("%s.tr.n",arm),1);
	T->SetBranchStatus(Form("%s.tr.p",arm),1);
	T->SetBranchStatus(Form("%s.tr.x",arm),1);
	T->SetBranchStatus(Form("%s.tr.y",arm),1);
	T->SetBranchStatus(Form("%s.tr.ph",arm),1);
	T->SetBranchStatus(Form("%s.tr.th",arm),1);
	T->SetBranchStatus(Form("%s.tr.chi2",arm),1);
	T->SetBranchStatus(Form("%s.tr.ndof",arm),1);
	T->SetBranchStatus(Form("%s.ps.a",arm),1);
	T->SetBranchStatus(Form("%s.sh.a",arm),1);
	T->SetBranchStatus(Form("%s.ps.trx",arm),1);
	T->SetBranchStatus(Form("%s.ps.try",arm),1);
	T->SetBranchStatus(Form("%s.sh.trx",arm),1);
	T->SetBranchStatus(Form("%s.sh.try",arm),1);
	T->SetBranchStatus(Form("%s.tr.vz",arm),1);

	T->SetBranchAddress(Form("%s.ps.e",arm),&e1);
	T->SetBranchAddress(Form("%s.sh.e",arm),&e2);
	T->SetBranchAddress(Form("%s.cer.asum_c",arm),&scer);
	T->SetBranchAddress(Form("%s.tr.n",arm),&n_tr);
	T->SetBranchAddress(Form("%s.tr.p",arm),&p);
	T->SetBranchAddress(Form("%s.tr.x",arm),&x);
	T->SetBranchAddress(Form("%s.tr.y",arm),&y);
	T->SetBranchAddress(Form("%s.tr.ph",arm),&ph);
	T->SetBranchAddress(Form("%s.tr.th",arm),&th);
	T->SetBranchAddress(Form("%s.tr.chi2",arm),&chi2);
	T->SetBranchAddress(Form("%s.tr.ndof",arm),&ndof);
	T->SetBranchAddress(Form("%s.ps.a",arm),&ps);
	T->SetBranchAddress(Form("%s.sh.a",arm),&sh);
	T->SetBranchAddress(Form("%s.ps.trx",arm),&tr_x_ps);
	T->SetBranchAddress(Form("%s.ps.try",arm),&tr_y_ps);
	T->SetBranchAddress(Form("%s.sh.trx",arm),&tr_x_sh);
	T->SetBranchAddress(Form("%s.sh.try",arm),&tr_y_sh);
	T->SetBranchAddress(Form("%s.tr.vz",arm),&Rtr_vz);

	Int_t EventNo = T->GetEntries();
			   
	Double_t A[123];
	Double_t S0[123];
	Double_t S[123][123];
	Int_t block_number;

	// initialize matrix
	for(Int_t i=0; i<(123); i++) {
	
		S0[i]=0.0;
		A[i]=0.0;

		for(Int_t j=0;j<(123);j++) {

			S[i][j]=0;

		}
	}

	// loop the events

	for (Int_t i_evt=0; i_evt<EventNo;i_evt++) {  
	
		T->GetEntry(i_evt);
      			//if ((scer>2000)&&(e1+e2)/p[0]/1000>0.7&&(n_tr==1)&&(chi2[0]/ndof[0]<40.)&&((y[0]+4.57*ph[0])>-0.16)&&((y[0]+4.57*ph[0])<0.12))
		//if ((scer>2000)&&(e1+e2)/p[0]/1000>0.7&&(n_tr==1)&&(chi2[0]/ndof[0]<40.)){
		//if ((scer[0]>2000)&&(e1[0]+e2[0])/p[0]/1000>0.7&&(n_tr[0]=1) && (Rtr_vz[0])> -0.6 && (Rtr_vz[0])< 0.6 && (e1[0]+e2[0])<3250 && e2[0] >1000.0 && e1[0]>300 && (e1[0]+e2[0])<2600 ){
		if ((scer[0]>2000)&&(e1[0]+e2[0])/p[0]/1000>0.8&&(n_tr[0]=1) && (Rtr_vz[0])> -0.8 && (Rtr_vz[0])< 0.8 && e2[0] >1000.0 && e1[0]>300 && (e1[0]+e2[0])<2600){
		  
		//if ((scer>2000)&&(e1+e2)>2900 && (e1+e2)<3500 && e2<3300 && e2>1400 && n_tr==1)	 		
		//	if ((scer>2000)&&(e1+0.9*e2)>1600 /*&& e2>1000*/ && e2<250 && n_tr==1){	 		
			good_el++;
			Int_t ps_block =0;
			Double_t sum_tmp=0;
			Double_t Ai[123]={0};
			
			for (Int_t j=0; j<(123); j++) {
			  //Ai[j]=0.;
			  Ai[j]=0.0;
			  }
			
			Int_t nn = x_to_n(tr_x_ps[0]);
			
	      		Int_t ps_column = psy_to_Ypos(tr_y_ps[0]);
					if (nn !=-99 && ps_column!= -95){
		         ps_block = (ps_column)*24 + nn;
			
			 //cout <<"row"<<"\t"<<nn<<"\t"<<"column"<<"\t"<<ps_column<<"\t"<<"block"<<"\t"<<ps_block<<endl;
					}			
		      


			  //for (Int_t j=nn-1; j<=nn+1; j++) { 
			  //   for (Int_t j=nn-1; j<=nn+1; j++) { 
			  // for (Int_t j=ps_block-1;j<=ps_block+1;j++){
			     			    
			  //if (ps_block <1 && ps_block > -1){
			  if (ps_block==0){
			 for (Int_t j=ps_block;j<=ps_block+1;j++){
			   //  cout <<"1"<<"\t"<<"ps[j]"<<"\t"<<ps[j] <<"\t"<<"j"<<"\t"<<j<<endl;
  if(ps[j]-ped_ps[j]>0) {Ai[j]=(ps[j]-ped_ps[j])/1000.;}
			    if((ps[j+24]-ped_ps[j+24])>0) {Ai[j+24]=(ps[j+24]-ped_ps[j+24])/1000.;}                                                                            }
}
    else {
if (ps_block>46 && ps_block<48){
			 for (Int_t j=ps_block-1;j<=ps_block;j++){
			   //  cout <<"2"<<"\t"<<"ps[j]"<<"\t"<<ps[j] <<"\t"<<"j"<<"\t"<<j<<endl;
  if(ps[j]-ped_ps[j]>0) {Ai[j]=(ps[j]-ped_ps[j])/1000.;}
			    if((ps[j-24]-ped_ps[j-24])>0) {Ai[j-24]=(ps[j-24]-ped_ps[j-24])/1000.;}                                                                            
			    }
}
    }
			  else{
if (ps_block==23){
      //if (ps_block>22 && ps_block<24){
			 for (Int_t j=ps_block-1;j<=ps_block;j++){
			   //cout <<"ps[j]"<<"\t"<<ps[j] <<"\t"<<"j"<<"\t"<<j<<endl;
			    if((ps[j+24]-ped_ps[j+24])>0) {Ai[j+24]=(ps[j+24]-ped_ps[j+24])/1000.;}                                                                            
			    }
}
    }
	
    else{
if ((ps_block>0 && ps_block<22)){
for (Int_t j=ps_block-1;j<=ps_block+1;j++){
  //cout <<"5"<<"\t"<<"ps[j]"<<"\t"<<ps[j] <<"\t"<<"j"<<"\t"<<j<<endl;
  if(ps[j]-ped_ps[j]>0) {Ai[j]=(ps[j]-ped_ps[j])/1000.;}
			    //		cout <<"ps[j]"<<"\t"<<ps[j]<<"\t"<<"ped"<<ped_ps[j] <<"\t"<<"Ai"<<"\t"<<Ai[j]<<endl;
			    if((ps[j+24]-ped_ps[j+24])>0) {Ai[j+24]=(ps[j+24]-ped_ps[j+24])/1000.;}                                                                            
			    }
    }
    }

    else{
if ( ( ps_block>24 && ps_block<47)){
for (Int_t j=ps_block-1;j<=ps_block+1;j++){
  //cout <<"6"<<"\t"<<"ps[j]"<<"\t"<<ps[j] <<"\t"<<"j"<<"\t"<<j<<endl;
  if(ps[j]-ped_ps[j]>0) {Ai[j]=(ps[j]-ped_ps[j])/1000.;}
			    //		cout <<"ps[j]"<<"\t"<<ps[j]<<"\t"<<"ped"<<ped_ps[j] <<"\t"<<"Ai"<<"\t"<<Ai[j]<<endl;
			    if((ps[j-24]-ped_ps[j-24])>0) {Ai[j-24]=(ps[j-24]-ped_ps[j-24])/1000.;}                                                                            
			    }
    }
    }

		 			  
			  
			  Int_t row = x_to_Xpos(tr_x_sh[0]);
			 
			  Int_t column = y_to_Ypos(tr_y_sh[0]);
			  if (row >=0 && column>=0){
			   Int_t sh_block = (column)*15 + row;
			   // Int_t sh_block = (column+1)*15 + row; 
			  }
       

			  //			  else{
			    if (( sh_block >0 && sh_block < 14)){		      
			    for (Int_t i= sh_block;i<=sh_block+1;i++){
			      // cout <<"1"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i+15] -ped_sh[i+15]>0){Ai[i+48 +15] = (sh[i+15] - ped_sh[i+15])/1000.;} //-15 is if we skip column 0 
			    }
  }
			    //     }


			  else{
			   if (sh_block==15 || sh_block==30||sh_block==45){
			  //   if (sh_block==15||sh_block==30||sh_block==45){
			    for (Int_t i= sh_block;i<=sh_block+1;i++){
			      // cout <<"1"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i+15] -ped_sh[i+15]>0){Ai[i+48 +15] = (sh[i+15] - ped_sh[i+15])/1000.;} //-15 is if we skip column 0 
			       if(sh[i-15] -ped_sh[i-15]>0){Ai[i+48 -15] = (sh[i-15] - ped_sh[i-15])/1000.;}
			    }
  }
			  }


			  else {
			    if (sh_block==14||sh_block==29||sh_block==44 ||sh_block==59){
			    //  if (sh_block==29||sh_block==44){
			    for (Int_t i= sh_block-1;i<=sh_block;i++){
			      //cout <<"2"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i+15] -ped_sh[i+15]>0){Ai[i+48+15] = (sh[i+15] - ped_sh[i+15])/1000.;} //-15 is if we skip column 0 
			     
			    }
  }
			  }
			   
			   else{
			     if (sh_block>60 && sh_block<74){
			       // if (sh_block>45 && sh_block<59){
			    for (Int_t i= sh_block-1;i<=sh_block+1;i++){
			      //cout <<"3"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i-15] -ped_sh[i-15]>0){Ai[i+48-15] = (sh[i-15] - ped_sh[i-15])/1000.;} //-15 is if we skip column 0 
			    }
			    }
  }
			   			   
			   
			  else{
			     if (sh_block==74){
			    for (Int_t i= sh_block-1;i<sh_block;i++){
			      //cout <<"3"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i-15] -ped_sh[i-15]>0){Ai[i+48-15] = (sh[i-15] - ped_sh[i-15])/1000.;} //-15 is if we skip column 0 
			    }
			    }
  }
			   
			/*
			  else{
			     // if (sh_block>60 && sh_block<74){
			     if (sh_block==59){
			    for (Int_t i= sh_block-1;i<=sh_block;i++){
			      //cout <<"3"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i-15] -ped_sh[i-15]>0){Ai[i+48-15] = (sh[i-15] - ped_sh[i-15])/1000.;} //-15 is if we skip column 0 
			    }
			    }
  }
			*/


			  else {
			    if (sh_block==60){
			      //if (sh_block==45){
			      for (Int_t i= sh_block;i<=sh_block+1;i++){
			      //cout <<"4"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i-15] -ped_sh[i-15]>0){Ai[i+48-15] = (sh[i-15] - ped_sh[i-15])/1000.;} //-15 is if we skip column 0 
			    }
  }
			  }
			   
			  else {
			    //   if (sh_block>15 && sh_block<59 && ( sh_block!= 29 || sh_block!=30 && sh_block!=44 && sh_block!=45)){ 
			        if (sh_block>15 && sh_block<59 && ( sh_block!= 29 && sh_block!=30 && sh_block!=44 && sh_block!=45)){ 
			    //if (sh_block !=0 && sh_block !=14 && sh_block !=15 && sh_block!=30 && sh_block !=60 && sh_block !=29 && sh_block !=44 && sh_block !=15 && sh_block !=30 && sh_block !=45 && sh_block !=59 && sh_block !=74 ){
			    for (Int_t i= sh_block-1;i<=sh_block+1;i++){
			      //cout <<"5"<<"\t"<<sh_block<<endl;
			      if(sh[i] -ped_sh[i]>0){Ai[i+48] = (sh[i] - ped_sh[i])/1000.;}
			      if(sh[i+15] -ped_sh[i+15]>0){Ai[i+48+15] = (sh[i+15] - ped_sh[i+15])/1000.;} //-15 is if we skip column 0 
			    }
			    }
			   }

				       

				for (Int_t i=0; i<(123); i++) {
				S0[i] += p[0]*Ai[i];
				//cout <<"Ai"<<"\t"<<Ai[i]<<endl;
	  			for (Int_t j=0; j<(123); j++) { 
					S[i][j] += Ai[i]*Ai[j]; 
		       			//cout <<"S[i][j]"<<S[i][j]<<endl;
				} 
			}  

			if ( (i_evt % 1000) == 0 ) {    
				cout<<i_evt<<"/"<<EventNo<<" events are analyzed, selected "<<good_el<<" electrons "<<endl;
	    			if (nn>=0 && (sh_block < 59 && sh_block > 15) && (sh_block != 29 && sh_block != 30 && sh_block!= 44 && sh_block != 45 ) ) { 
				  cout <<"shower Block"<<"\t"<<sh_block<<endl;
		
					for (int i=0; i<(123); i++) { 
						if (S0[i]==0) cout<<"Block "<<i<<" has no event"<<endl; 
					}
 				} 
			} 
		   }	
	} // end good event if statement  
		 // end event for loop

	// to calibrate the matrix

	//	const Int_t nrVar  = 68
	const Int_t nrVar  = (N+Y);

	TMatrixD AA(123,123);
	TMatrixD S_mat(123,1);

	for(int i=0;i<123;i++) { 	
		S_mat(i,0)=S0[i];
		cout <<"S0i"<<"\t"<<S0[i]<<endl;
		for(int j=0;j<123;j++) { 
			AA(i,j)=S[i][j]; 
			//			cout <<("S0[i][j]:")<<i << j<<"\t"<<S[i][j]<<endl;
		} 
	}

	TMatrixD ST(TMatrixD::kTransposed,S_mat);

	TMatrixD A_1 = AA;
	Double_t d;
	A_1.Invert(&d);
	TMatrixD C = (A_1*AA);
	TMatrixD Coef = (A_1*S_mat);


	// output
	
	ofstream ooo("matrix_cut.txt");

	if (ooo.is_open()) { 
		for (int j_sig=0; j_sig<N; j_sig++) { 
			ooo<<setw(14)<<ped_ps[j_sig];
			ooo<<endl;
		} 
	}

	ooo<<"################"<<endl;
	
	if (ooo.is_open()) { 
		for (int j_sig=0; j_sig<Y; j_sig++) { 
			ooo<<setw(14)<<ped_sh[j_sig];
			ooo<<endl;
		} 
	}

	ooo<<"################"<<endl;
	
	if (ooo.is_open()) { 
	  for (int j_sig=0; j_sig<(123); j_sig++) { 
			ooo<<setw(14)<<Coef(j_sig,0);
			if(j_sig==47) {
					ooo<<"########"<<endl;
				}
			ooo<<endl;
		} 
	}

	ooo.close();
	
}



int x_to_n(double x_tmp) {
	int nn=-99;
  	for (int i=0; i<24; i++) {
       		if ( (xmin[i]<x_tmp) && (x_tmp<xmax[i]) ) {
		  //	  if (x_tmp == PS_ROW){
      			nn=i;

      			break;
   		}
	}

	//   	cout << "nn" << "\t"<<nn <<endl;
	return nn;
}


int psy_to_Ypos(double y_tmp1){

  int ps_column = -95;
  for (int i=0;i<2; i++){
        if ( (ps_ymin[i]<y_tmp1) && (y_tmp1<ps_ymax[i]) ) {
    //if (y_tmp1 == PS_COL[i]){
      ps_column =i;
      break;
    }
    
  }
  //     cout  << "column"<<"\t"<<ps_column<<endl;
  return ps_column;
}




int x_to_Xpos(double x_tmp1){

  int row = -98;
  for (int i=0;i<15; i++){
    if ( (Sh_xmin[i]<x_tmp1) && (x_tmp1<Sh_xmax[i]) ) {
    //if (tmp 1 == SH_ROW[i]){
      row =i;
      break;
    }
    //    cout << "row"<<"\t"<<row<<endl;
  }
  //  cout << "row"<<"\t"<<row<<endl;
  return row;
}

int y_to_Ypos(double y_tmp2){

  int column = -97;
  for (int i=0;i<5; i++){
       if ( (Sh_ymin[i]<y_tmp2) && (y_tmp2<Sh_ymax[i]) ) {
	 // if (tmp2 == SH_COL[i]){
      column =i;
      break;
    }
    //    cout  << "coumn"<<"\t"<<column<<endl;
  }
  //    cout  << "coumn"<<"\t"<<column<<endl;
  return column;
}
