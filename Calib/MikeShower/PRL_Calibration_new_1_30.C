#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include  <stdio.h>
#include  <stdlib.h>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"

using namespace std;

//Double_t xmin[17]={100., 0.78, 0.65, 0.49, 0.33, 0.20, 0.05, -0.09, -0.24, -0.38, -0.53, -0.68, -0.83, -0.97, -1.11, -1.3, -100.};
//Double_t xcen[17]={100., 0.82,0.675, 0.53, 0.385,0.24, 0.095,-0.05, -0.195,-0.34, -0.485,-0.63, -0.775,-0.92, -1.065,-1.21,-100.};
//Double_t xmax[17]={100., 0.87, 0.72, 0.57, 0.43, 0.29, 0.14, -0.01, -0.16, -0.30, -0.45, -0.60, -0.75, -0.89, -1.06, -1.2, -100.};



//Double_t xmin[24]={1.11, 1.01,0.91,0.81,0.71,0.61,0.50,0.40,0.30,0.20,0.10,0.0,-0.10,-0.20,-0.30,-0.40,-0.50,-0.61,-0.71,-0.81,-0.91,-1.01,-1.11,-1.212};
//Double_t xcen[]={100.,1.09,0.99,0.89,0.79,0.68,0.5-100.};
//Double_t xmax[24]={1.14,1.04,0.94,0.84,0.74,0.64,0.54,0.44,0.33,0.23,0.13,0.03,-0.07,-0.17,-0.27,-0.37,-0.47,-0.58,-0.68,-0.78,-0.88,-0.98,-1.08,-1.181};

//Double_t xmin[24]={-1.212,-1.11,-1.01,-0.91,-0.81.-0.71.-0.61,-0.50,-0.40,-0.30,-0.20,-0.10,0.0,0.10,0.20,0.30,0.40,0.50,0.61,0.71,0.81,0.91,1.01,1.11};
//Double_t xmax[24]={-1.181,-1.08,-0.98,-0.88,-0.78,-0.68,-0.58,-0.47,-0.37,-0.27,-0.17,-0.07,0.03,0.13,0.23,0.33,0.44,0.54,0.64,0.74,0.84,0.94,1.04,1.14};

Double_t xmin[24]={-1.25,-1.15,-1.05,-0.95,-0.85.-0.75.-0.65,-0.54,-0.45,-0.35,-0.25,-0.15,-0.04,0.05,0.16,0.26,0.36,0.46,0.56,0.66,0.76,0.86,0.96,1.06};
Double_t xmax[24]={-1.21,-1.10,-1.00,-0.90,-0.80,-0.70,-0.60,-0.49,-0.50,-0.30,-0.20,-0.10,0.005,0.11,0.21,0.31,0.41,0.51,0.61,0.71,0.81,0.91,1.01,1.11};


Double_t Sh_xmin[15] = {-1.13,-0.98,-0.83,-0.68,-0.52,-0.37,-0.22,-0.065,0.08,0.23,0.39,0.54,0.69,0.85,1.04};
Double_t Sh_xmax[15] = {-1.05,-0.90,-0.75,-0.59,-0.44,-0.29,-0.14,0.011,0.16,0.32,0.47,0.62,0.77,0.92,1.07};
Double_t Sh_ymin[5] = {-0.18,-0.028,0.12};
Double_t Sh_ymax[5] = {-0.11,0.04,0.19};


//Double_t Sh_xmin[15] = {1.04,0.85,0.69,0.54,0.39,0.23,0.08,-0.065,-0.22,-0.37,-0.52,-0.68,-0.83,-0.98,-1.13};
//Double_t Sh_xmax[15] = {1.07,0.92,0.77,0.62,0.47,0.32,0.16,0.011,-0.14,-0.29,-0.44,-0.59,-0.75,-0.90,-1.05};
//Double_t Sh_ymin[5] = {/*0.23,0.43,*/0.27,0.12,-0.028,-0.18,-0.33};
//Double_t Sh_ymax[5] = {/*0.31,0.50,*/0.35,0.19,0.04,-0.11,-0.26};
//Double_t Sh_ymin[5] = {0.12,-0.028,-0.18};
//Double_t Sh_ymax[5] = {0.19,0.04,-0.11};



Double_t ymin_l=0.04, ymax_l=0.12;
Double_t ymin_r=-0.16, ymax_r=-0.06;
Double_t ymin = -0.16, ymax = 0.12;

//void PRL_Calibration(int runNo) { 
void PRL_Calibration_new() { 
  //gROOT->LoadMacro("WIJ.C+");
  //WIJ();

  int runNo;
  cout<<"run number"<<endl;
  cin >> runNo;
	// which arm?
	char arm[2]; 
	if(runNo < 90000) {
		//LHRS
		sprintf(arm,"L");
	} else {
		//RHRS
		sprintf(arm, "R");
		const int N = 48;
		const int Y = 75;
	}

	Double_t ped_ps[N]={0};
	Double_t ped_sh[Y]={0};

	//	TString RootFile=Form("/chafs1/work1/tritium/rootfiles_1.6_root6/Rootfiles/tritium_%d.root",runNo);
		TString RootFile=Form("/chafs1/work1/tritium/Rootfiles/tritium_%d.root",runNo);
	//	TString RootFile=Form("/lustre/expphy/volatile/halla/triton/Tritium_Rootfiles",runNo);
	TChain *T=new TChain("T");
	T->Add(RootFile);
	// fit the pedestals

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


	Int_t EventNo = T->GetEntries();
			   
	Double_t A[93];
	Double_t S0[93];
	Double_t S[93][93];
	Int_t block_number;

	// initialize matrix
	for(Int_t i=0; i<(93); i++) {
	
		S0[i]=0.0;
		A[i]=0.0;

		for(Int_t j=0;j<(93);j++) {

			S[i][j]=0;

		}
	}

	// loop the events

	for (Int_t i_evt=0; i_evt<EventNo;i_evt++) {  
	
		T->GetEntry(i_evt);
		//		if ((scer>2000)&&(e1+e2)/p[0]/1000>0.8&&(n_tr==1)&&(chi2[0]/ndof[0]<40.)&&((y[0]+4.57*ph[0])>-0.16)&&((y[0]+4.57*ph[0])<0.12)&& (block >15 && block<59 && block!=29 ))
		//if ((scer>2000)&&(e1+e2)/p[0]/1000>0.7&&(n_tr==1)&&(chi2[0]/ndof[0]<40.)&&((y[0]+4.57*ph[0])>-0.16)&&((y[0]+4.57*ph[0])<0.12))
		if ((scer>2000)&&(e1+e2)/p[0]/1000>0.7&&(n_tr==1)&&(chi2[0]/ndof[0]<40.)){
	 		
			good_el++;
			Double_t sum_tmp=0;
			Double_t Ai[93]={0};
			/*
			for (Int_t j=0; j<(48+75); j++) {
			  //Ai[j]=0.;
			  Ai[j]=0.;
			}
			*/
			Int_t nn = x_to_n(tr_x_ps[0]);
			//Int_t nn = x_to_n(x[0]+3.5*th[0]);  //check for each event
			//Int_t nx = x_to_Xpos(x[0]+4.57*th[0]);  //check for each event
			//Int_t ny = x_to_Ypos(x[0]+4.57*ph[0]);  //check for each event
			//			Int_t row = x_to_Xpos(x[0]+3.64*th[0]);  //check for each event
			//	Int_t column = x_to_Ypos(x[0]+3.64*ph[0]);  //check for each event
			    

			//			Int_t sh_block = (column+1)*15 + row;
			//			cout << "sh_block" <<sh_block<<endl;
			
			if (nn>=1) {  
			
			  //for (Int_t j=nn-1; j<=nn+1; j++) { 
			  for (Int_t j=nn-1; j<=nn+1; j++) { 
			    // if (row>=1 && column>0 && column<5){

			      //  for (Int_t i=sh_block; i<=sh_block+1; i++){
			      //	(blk < 59 && blk > 15) && (blk != 29 && blk != 30 && blk!= 44 && blk != 45 )  // exclude the edges
					if(ps[j]-ped_ps[j]>0) {Ai[j]=(ps[j]-ped_ps[j])/1000.;}
					//		cout <<"ps[j]"<<"\t"<<ps[j]<<"\t"<<"ped"<<ped_ps[j] <<"\t"<<"Ai"<<"\t"<<Ai[j]<<endl;
					if((ps[j+24]-ped_ps[j+24])>0) {Ai[j+24]=(ps[j+24]-ped_ps[j+24])/1000.;}                                                                                                                                                                           
					//			  } commented 10:15
			  //	}
					
					//if (row>=1 && column>0 && column<5){
					//for (Int_t i=sh_block; i<=sh_block+1; i++){
			  //			Int_t row = x_to_Xpos(x[0]+3.64*th[0]);  //check for each event
			  //	Int_t column = y_to_Ypos(x[0]+3.64*ph[0]);  //check for each event

			  Int_t row = x_to_Xpos(tr_x_sh[0]);
			  Int_t column = y_to_Ypos(tr_y_sh[0]);
			Int_t sh_block = (column+1)*15 + row; 
			//if (row>=1 && column>0 &&  column<5 && (sh_block < 59 && sh_block > 15) && (sh_block != 29 && sh_block != 30 && sh_block!= 44 && sh_block != 45 ))*/{  // exclude the edges
			      //			      cout << "Column:"<<"\t"<<column<<"\t"<<"shower block:"<<"\t"<<sh_block<<"\t" <<"nn"<<nn<<endl;
			      if (sh_block>=16){
					if (column == 0){
					  for (Int_t i=sh_block-1; i<=sh_block+1; i++){
					    if((sh[i]-ped_sh[sh_block])>0){ Ai[j+24+(48-(j+24))+(i-15)]=(sh[i]-ped_sh[i])/1000.;}
					 //	cout <<"j"<<"\t"<<j<<"\t"<<"i"<<"\t"<<i<<endl;
					    if(sh[i+15]-ped_sh[i+15]>0){ Ai[j+24+(48-(j+24))+(i-15)+15]=(sh[i+15]-ped_sh[i+15])/1000.;}
					//cout << "column"<<column<<"\t"<< "j"<<j<<"\t" <<"i"<<i<<"\t" <<"Ai" <<Ai[j+i];
					//cout <<"**************************************************************************************"<<endl;
					// if(sh[sh_block-15]-ped_sh[sh_block-15]>0){ Ai[j+sh_block-15]=(sh[sh_block-15]-ped_sh[sh_block-15])/1000.;}
					// if(sh[sh_block-1+15]-ped_sh[sh_block-1+15]>0){ Ai[j+sh_block-1+15]=(sh[sh_block-1+15]-ped_sh[sh_block-1+15])/1000.;}
					  }
					}



					if (column == 1){
					  for (Int_t i=sh_block-1; i<=sh_block+1; i++){
					 if((sh[i]-ped_sh[i])>0){ Ai[j+(i-15)]=(sh[i]-ped_sh[i])/1000.;}
					 // 	cout <<"i"<<"\t"<<i<<endl;
					 if(sh[i+15]-ped_sh[i+15]>0){ Ai[j+24+(48-(j+24))+(i-15)+15]=(sh[i+15]-ped_sh[i+15])/1000.;}
					 //if(sh[sh_block-15]-ped_sh[sh_block-15]>0){ Ai[j+24+(48-(j+24))+(sh_block-15)-15]=(sh[sh_block-15]-ped_sh[sh_block-15])/1000.;}
					     // if(sh[sh_block-1+15]-ped_sh[sh_block-1+15]>0){ Ai[j+(48-(j+24))+(sh_block-15)-1+15]=(sh[sh_block-1+15]-ped_sh[sh_block-1+15])/1000.;}
					//cout <<"column"<<column<<"\t" <<"j"<<j<<"\t" <<"sh_block"<<sh_block<<"\t" <<"Ai" <<Ai[j+sh_block];
					//cout <<"**************************************************************************************"<<endl;
					}
					}


					if (column == 2){
					  for (Int_t i=sh_block; i<=sh_block+1; i++){
					 if((sh[i]-ped_sh[i])>0){ Ai[j+24+(48-(j+24))+(i-15)]=(sh[i]-ped_sh[i])/1000.;}
					 //	cout <<"j"<<"\t"<<j<<"\t"<<"i"<<"\t"<<i<<endl;
					 //if(sh[i+15]-ped_sh[i+15]>0){ Ai[j+i+15]=(sh[i+15]-ped_sh[i+15])/1000.;}
					 if(sh[i-15]-ped_sh[i-15]>0){ Ai[j+24+(48-(j+24))+(i-15)-15]=(sh[i-15]-ped_sh[i-15])/1000.;}
					// if(sh[i-1+15]-ped_sh[i-1+15]>0){ Ai[j+i-1+15]=(sh[i-1+15]-ped_sh[i-1+15])/1000.;}
			
					//cout << "column"<<column<<"\t"<< "j"<<j<<"\t" <<"i"<<i<<"\t" <<"Ai" <<Ai[j+i];
					//cout <<"**************************************************************************************"<<endl;
		}
					}
			      }
			   
			  } // add 10:15
			}

			//} was connected to if(scer)
			  //}

					/*
					if((sh[sh_block]-ped_sh[sh_block])>0){ Ai[j+(sh_block)]=(sh[sh_block]-ped_sh[sh_block])/1000.;}
	   				cout <<"j"<<"\t"<<j<<"\t"<<"sh_block"<<"\t"<<sh_block<<endl;
					if(sh[sh_block+15]-ped_sh[sh_block+15]>0){ Ai[j+sh_block+15]=(sh[sh_block+15]-ped_sh[sh_block+15])/1000.;}
				        if(sh[sh_block-15]-ped_sh[sh_block-15]>0){ Ai[j+sh_block-15]=(sh[sh_block-15]-ped_sh[sh_block-15])/1000.;}
				        if(sh[sh_block-1+15]-ped_sh[sh_block-1+15]>0){ Ai[j+sh_block-1+15]=(sh[sh_block-1+15]-ped_sh[sh_block-1+15])/1000.;}
					*/
					/*
					if((sh[j]-ped_sh[j])>0){ Ai[j+48]=(sh[j]-ped_sh[j])/1000.;}
	   				if(sh[j+14]-ped_sh[j+15]>0){ Ai[j+48+15]=(sh[j+15]-ped_sh[j+15])/1000.;}
					if(sh[j+29]-ped_sh[j+30]>0){ Ai[j+48+30]=(sh[j+30]-ped_sh[j+30])/1000.;}
					if(sh[j+45]-ped_sh[j+45]>0){ Ai[j+48+45]=(sh[j+45]-ped_sh[j+45])/1000.;}
					if(sh[j+60]-ped_sh[j+60]>0){ Ai[j+48+45]=(sh[j+60]-ped_sh[j+60])/1000.;}
					*/
					//	sum_tmp+=(Ai[j]+Ai[j+sh_block]); 
				  

			//}
			/*	  

			for (Int_t i=0;i<(N+Y); i++){
                          cout <<"i"<<i<<"\t" <<"Ai"<<"\t"<<Ai[i]<<endl;
                        }
			*/

	

			for (Int_t i=0; i<(93); i++) {
				S0[i] += p[0]*Ai[i];
				//cout <<"Ai"<<"\t"<<Ai[i]<<endl;
	  			for (Int_t j=0; j<(93); j++) { 
					S[i][j] += Ai[i]*Ai[j]; 
		       			//cout <<"S[i][j]"<<S[i][j]<<endl;
				} 
			}  

			if ( (i_evt % 1000) == 0 ) {    
				cout<<i_evt<<"/"<<EventNo<<" events are analyzed, selected "<<good_el<<" electrons "<<endl;
	    			if (nn>=0 && (sh_block < 59 && sh_block > 15) && (sh_block != 29 && sh_block != 30 && sh_block!= 44 && sh_block != 45 ) ) { 
				  /*
					Double_t e11=Ai[nn]+Ai[nn+17]+Ai[nn-1]+Ai[nn-1+17]+Ai[nn+1]+Ai[nn+1+17];
	    				Double_t e12=Ai[nn]+Ai[nn+17]+Ai[nn-1]+Ai[nn-1+17]+Ai[nn+1]+Ai[nn+1+17]+Ai[nn+34]+
						Ai[nn+17+34]+Ai[nn-1+34]+Ai[nn-1+17+34]+Ai[nn+1+34]+Ai[nn+1+17+34];
				  */
				  cout <<"shower Block"<<"\t"<<sh_block<<endl;
				  /*
			   	   Double_t e11=Ai[nn]+Ai[nn+24]+Ai[nn-1]+Ai[nn-1+24]+Ai[nn+1]+Ai[nn+1+24];    //preshower
				   Double_t e12=Ai[nn]+Ai[nn+24]+Ai[nn-1]+Ai[nn-1+24]+Ai[nn+1]+Ai[nn+1+24]+Ai[j+24+(48-(j+24))+(sh_block-15)]+
				    Ai[j+24+(48-(j+24))+(sh_block-15)+15]+Ai[j+24+(48-(j+24))+(sh_block-15)-15]+Ai[j+24+(48-(j+24))+(sh_block-15)-1+15];//  +Ai[nn+1+34]+Ai[nn+1+17+34];
				  */	
					for (int i=0; i<(93); i++) { 
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

	TMatrixD AA(93,93);
	TMatrixD S_mat(93,1);

	for(int i=0;i<93;i++) { 	
		S_mat(i,0)=S0[i];
		cout <<"S0i"<<"\t"<<S0[i]<<endl;
		for(int j=0;j<93;j++) { 
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
	
	ofstream ooo("matrix_1.txt");

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
	  for (int j_sig=0; j_sig<(93); j_sig++) { 
			ooo<<setw(14)<<Coef(j_sig,0);
				if(j_sig==33) {
					ooo<<"########"<<endl;
				}
			ooo<<endl;
		} 
	}

	ooo.close();
	
}



int x_to_n(double x_tmp) {
	int nn=-99;
  	for (int i=0; i<23; i++) {
    		if ( (xmin[i]<x_tmp) && (x_tmp<xmax[i]) ) {
      			nn=i;

      			break;
   		}
	}

	//	cout << "nn" << "\t"<<nn <<endl;
	return nn;
}






int x_to_Xpos(double x_tmp1){

  int row = 0;
  for (int i=0;i<15; i++){
    if ( (Sh_xmin[i]<x_tmp1) && (x_tmp1<Sh_xmax[i]) ) {
      row =i;
      break;
    }
    //    cout << "row"<<"\t"<<row<<endl;
  }
  // cout << "row"<<"\t"<<row<<endl;
  return row;
}


int y_to_Ypos(double x_tmp2){

  int column = 0;
  for (int i=0;i<3; i++){
    if ( (Sh_ymin[i]<x_tmp2) && (x_tmp2<Sh_ymax[i]) ) {
      column =i;
      break;
    }
    //    cout  << "coumn"<<"\t"<<column<<endl;
  }
  //cout  << "coumn"<<"\t"<<column<<endl;
  return column;
}
