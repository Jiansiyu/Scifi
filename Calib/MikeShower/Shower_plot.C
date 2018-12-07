#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TString.h>
#include <TPolyLine3D.h>





using namespace std;





  Double_t xmin[24]={-1.21,-1.10,-1.00,-0.90,-0.80,-0.70,-0.60,-0.50,-0.40,-0.30,-0.20,-0.09,0.00,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.0,1.11};
  Double_t xmax[24]={-1.10,-1.00,-0.90,-0.80,-0.70,-0.60,-0.50,-0.40,-0.30,-0.20,-0.09, 0.00,0.10, 0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.01,1.11,1.21};
  Double_t ps_ymin[2] = {-0.35,0.0};
  Double_t ps_ymax[2] = {0.0,0.35};


  Double_t Sh_xmin[15] = {-1.14,-0.99,-0.83,-0.68,-0.53,-0.38,-0.22,-0.07,0.08,0.22,0.38,0.53,0.68,0.84,0.99};
  Double_t Sh_xmax[15] = {-0.99,-0.83,-0.68,-0.53,-0.38,-0.22,-0.07,0.08,0.22,0.38,0.53,0.68,0.84,0.99,1.14};
  Double_t Sh_ymin[5] =  { -0.38,-0.22,-0.07,0.07,0.23 };
  Double_t Sh_ymax[5] =  { -0.22,-0.07,0.07,0.23,0.38  };



void Shower_plot(){


  int run_number;
  cout <<"Enter Run Number : "<<endl;
  cin >> run_number;

  char arm[2];
  if (run_number<90000){
    sprintf(arm,"L");
    }
    else {
      sprintf(arm,"R");
    }

  const TString Base_Path = "/volatile/halla/triton/Tritium_Rootfiles";

  std::ostringstream step ;

  step << Base_Path << "/tritium_" << run_number;
  TString convert = step.str().c_str();
  TString rootfile = convert + ".root";

  //    TChain* TSRight = new TChain("TSRight");                                                                                                
  TChain *T = new TChain("T");
  Long_t j=0;
  while  (! gSystem ->AccessPathName(rootfile)) {
    T->Add(rootfile);
    //TSRight->Add(rootfile);
    //evRight->Add(rootfile);
    T->Add(rootfile);
    cout << "Added"<<rootfile<<endl;
    j++;
    rootfile = convert + "_"+ j + ".root";
  }


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


  TCanvas *c1 = new TCanvas("c1");
  
  // TBox *box1 = new TBox(-1.21,-0.35,1.21,0.35);
  TBox *box1 = new TBox(-0.35,-1.21,0.35,1.21);
  //box1 -> SetLineColor(2);
  box1 -> SetFillColor(0);
  // box1 -> Draw();
  //TBox *box2 = new TBox(-1.21,-0.35,1.21,0.35);
  TBox *box2 = new TBox(-0.35,-1.21,0.35,1.21);
  box2 -> SetFillStyle(0);
 box2 -> SetLineColor(1);
 box2 -> SetLineWidth(2);

//TBox *box3 = new TBox(-1.14,-0.38+0.35,1.14,0.38+0.35);
 TBox *box3 = new TBox(-0.38,-1.14+0.35,0.38,1.14+0.35);
  //box3 -> SetLineColor(2);
  box3 -> SetFillColor(0);
  // box3 -> Draw();
//TBox *box4 = new TBox(-1.14,-0.38+0.35,1.14,0.38+0.35);
TBox *box4 = new TBox (-0.38,-1.14+0.35,0.38,1.14+0.35);
  //box4 -> SetLineColor(30);
 box4 -> SetFillStyle(0);
 box4 -> SetLineColor(2);
 box4 -> SetLineWidth(2);
  





 //TH2F *h1,*h2; 
  TH3F *h1,*h2;

  //TLine *line[100];
  TView3D *view = new TView3D();
  TPolyLine3D *pl = new TPolyLine3D();
  pl-> SetLineColor(2);

  TPolyMarker3D *pm3d1 = new TPolyMarker3D();
  pm3d1 -> SetMarkerStyle(2);
 // h1 = new TH2F("PS","PreShower",10,-3,3,10,-3,3);
  // h2 = new TH2F("SH","Shower",10,-3,3,10,-3,3);
    h1 = new TH3F("PS","PreShower",100,-3,3,100,-1,1,100,-3,3);
    h2 = new TH3F("SH","Shower",100,-3,3,100,-1,1,100,-3,3);

  int counter =0;

  Int_t EventNo = T->GetEntries();

  //  for (int i = 0; i<EventNo;i++){ // keep it simple for the moment
  for (int i =0; i< 100; i++){
    T->GetEntry(i);

    if ((scer>2000.)&&(e1+e2)/p[0]/1000>0.7&&(n_tr>0)){
      //      counter ++;


	Int_t nn = x_to_n(tr_x_ps[0]);
	Int_t ps_column = psy_to_Ypos(tr_y_ps[0]);
	if (nn !=-99 && ps_column!= -95){
	  ps_block = (ps_column)*24 + nn;
	}
	/*
	if (nn !=-99 && ps_column!= -95){
	h1 -> Fill(tr_x_ps[0],tr_y_ps[0]);
	  h1 ->Draw();
	}
	*/
	Int_t row = x_to_Xpos(tr_x_sh[0]);
	Int_t column = y_to_Ypos(tr_y_sh[0]);
	if (row >=0 && column>=0){
	Int_t sh_block = (column)*15 + row;
	  // Int_t sh_block = (column+1)*15 + row;                                                                                                                                                
	}

if (nn !=-99 && ps_column!= -95 && row >=0 && column >=0){
  // h1 -> Fill(tr_y_ps[0],tr_x_ps[0]);
   // h2 -> Fill(tr_y_sh[0],tr_x_sh[0]+0.35);

     h1 -> Fill(tr_y_ps[0],0,tr_x_ps[0]);
    h2 -> Fill(tr_y_sh[0],0,tr_x_sh[0]+0.35);

  h1 -> GetXaxis()->SetTitle("x");
  h1 -> GetYaxis()->SetTitle("y");
  // h1 -> GetYaxis()->SetTitle("z");
  // h1 -> GetYaxis()->SetTitle("x");

	  h1 ->Draw();
	  h2 -> Draw("same");
	  //	   box1 ->Draw();
	  //  box2 -> Draw();
	  //  box3 ->Draw();
	  //  box4-> Draw();
	  // pl[counter] = new TPolyLine3D(100);
	  pl -> SetPoint(i,tr_y_ps[0],0,tr_x_ps[0]);
	  pm3d1 ->SetPoint(i,tr_y_ps[0],0,tr_x_ps[0]);

	  //TLine *line = new TLine(tr_x_ps[0],tr_y_ps[0],tr_x_sh[0],tr_y_sh[0]+0.31);
	  //	   line[counter] = new TLine(tr_x_ps[0],tr_y_ps[0],tr_x_sh[0],tr_y_sh[0]+0.35);
	  //  line[counter] -> SetLineColor(kRed);
	  //	    line[counter] -> Draw();
	}
 pl->Draw();
 pm3d1 -> Draw();
c1 -> Update();
 counter++;

//if (row >=0 && column>=0){
// h2 -> Fill(tr_x_sh[0],tr_y_sh[0]+0.31);
//  h2 -> Draw("same");

// }

    }
    //    c1 -> Update(); 
 }

  //  c1 -> Update();



}





int x_to_n(double x_tmp) {
  // cout <<"psh x"<< x_tmp<<endl; 
 int nn=-99;
  for (int i=0; i<24; i++) {
    //    cout <<"psh x"<< x_tmp<<endl;  
  if ( (xmin[i]<x_tmp) && (x_tmp<xmax[i]) ) {
      //      if (x_tmp == PS_ROW){                                                                                                                                                                    
      nn=i;

      break;
    }
  }

  //      cout << "nn" << "\t"<<nn <<endl;                                                                                                                                                                   
  return nn;
}

int psy_to_Ypos(double y_tmp1){
  //cout <<"psh y"<< y_tmp1<<endl;
  int ps_column = -95;
  for (int i=0;i<2; i++){
    // cout <<"psh y"<< y_tmp1<<endl;  
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
  // cout <<"sh x"<< x_tmp1<<endl;
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
  //  cout <<"sh y"<< y_tmp2<<endl;
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

