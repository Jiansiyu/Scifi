void raster_calibration(){
  
  gStyle->SetOptStat(0);
  
  TChain *t = new TChain("T");
  t->Add("/work/halla/e08014/disk1/dien/E08014/replay_me/Rootfiles_raster_cal/e08014_3634.root");


  //If beam is off, BPMs will go to large (negative) number   
  //...Can add a beam current cut, if we know the variable...
  TCut beamcut = "TMath::Abs(urb.BPMA.x)<100";   //GMp beam current cut -> N.evleft.current_d3 > 60 .
  
  //Draw Raster Currents 
  //and X/Y Positions at BPM's and Targets
  TH1F *h1 = new TH1F("h1","Raster X;current (x) [ADC Channel]",1000,1000,4500);
  TH1F *h2 = new TH1F("h2","Raster Y;current (y) [ADC Channel]",1000,1000,4500);
 
  TH1F *h3 = new TH1F("h3","BPM-A X Position;Position (m)",400,-.02,.02);
  TH1F *h4 = new TH1F("h4","BPM-A Y Position;Position (m)",400,-.02,.02);
  TH1F *h5 = new TH1F("h5","BPM-B X Position;Position (m)",400,-.02,.02);
  TH1F *h6 = new TH1F("h6","BPM-B Y Position;Position (m)",400,-.02,.02);
  TH1F *h7 = new TH1F("h7","Target X-Position from BPMs;Position (m)",400,-.02,.02);
  TH1F *h8 = new TH1F("h8","Target Y-Position from BPMs;Position (m)",400,-.02,.02);


  //Plot 1: Raster Currents
  TCanvas *c1 = new TCanvas("c1");
  c1->Divide(2,1);

  c1->cd(1);
  T->Draw("rb.Raster.rawcur.x>>h1",beamcut);
  Double_t rax=h1->GetMean();
  Double_t drax=h1->GetRMS();
  
  c1->cd(2);
  T->Draw("rb.Raster.rawcur.y>>h2",beamcut);
  Double_t ray=h2->GetMean();
  Double_t dray=h2->GetRMS();


  //Plot 2: BPM Positions
  TCanvas *c2 = new TCanvas("c2");
  c2->Divide(2,3);
 
  c2->cd(1);
  T->Draw("urb.BPMA.x>>h3",beamcut);//can use either beam class here
  Double_t bax=h3->GetMean();       //...provided the variables are written out
  Double_t dbax=h3->GetRMS();
  
  c2->cd(2);
  T->Draw("urb.BPMA.y>>h4",beamcut);//``
  Double_t bay=h4->GetMean();
  Double_t dbay=h4->GetRMS();
  
  c2->cd(3);
  T->Draw("urb.BPMB.x>>h5",beamcut);//``
  Double_t bbx=h5->GetMean();
  Double_t dbbx=h5->GetRMS();
  
  c2->cd(4);
  T->Draw("urb.BPMB.y>>h6",beamcut);//``
  Double_t bby=h6->GetMean();
  Double_t dbby=h6->GetRMS();

  //We also need the BPM positions projected to the target.
  //This is stored in the unrastered beam class in variables like 
  //urb.x and urb.y

  c2->cd(5);
  T->Draw("urb.x>>h7",beamcut);    //need to calculate from BPMs
  Double_t tax=h7->GetMean();
  Double_t dtax=h7->GetRMS();
  
  c2->cd(6);
  T->Draw("urb.y>>h8",beamcut);   //need to calculate from BPMs
  Double_t tay=h8->GetMean();
  Double_t dtay=h8->GetRMS();


  //Set Raster correlation factors
  Double_t kappax=1.0;//Horrizontal beam direction
  Double_t kappay=1.0;//Vertical beam direction

  //
  //Calculate calibration coeffs.
  //
  Double_t bpma_ofx= bax-rax*dbax/drax*kappax ; 
  Double_t bpma_slx= dbax/drax*kappax ; 
  Double_t bpma_ofy= bay-ray*dbay/dray*kappay; 
  Double_t bpma_sly= dbay/dray*kappay; 
  
  Double_t bpmb_ofx= bbx-rax*dbbx/drax*kappax ; 
  Double_t bpmb_slx= dbbx/drax*kappax ; 
  Double_t bpmb_ofy= bby-ray*dbby/dray*kappay; 
  Double_t bpmb_sly= dbby/dray*kappay; 
  
  Double_t tar_ofx= tax-rax*dtax/drax*kappax ; 
  Double_t tar_slx= dtax/drax*kappax ; 
  Double_t tar_ofy= tay-ray*dtay/dray*kappay; 
  Double_t tar_sly= dtay/dray*kappay; 
  
  cout<<endl<<" Raster Constants: (please modify database accordingly)"<<endl;
  cout<<bpma_ofx<<" "<<bpma_ofy<<" "<<bpma_slx<<" "<<bpma_sly<<" 0. 0."<<endl;
  cout<<bpmb_ofx<<" "<<bpmb_ofy<<" "<<bpmb_slx<<" "<<bpmb_sly<<" 0. 0."<<endl;
  cout<<tar_ofx<<" "<<tar_ofy<<" "<<tar_slx<<" "<<tar_sly<<" 0. 0."<<endl<<endl;


  c1->Print("raster_calibration.pdf[");
  c1->Print("raster_calibration.pdf");
  c2->Print("raster_calibration.pdf");
  c2->Print("raster_calibration.pdf]");
}