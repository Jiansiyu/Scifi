inline TChain *GetTree(int& arm, TString& kin){
  Int_t flag, run_number;
  flag=-1;
  cout << "file or run number? (0=file, 1=runnumber): ";
  cin >> flag;
  if(!(flag==1)&&flag!=0){cout<<"wrong choice!";return 0;}

  int RHRS=0,LHRS=0;
  TChain *T = new TChain("T");
  if(flag==1){
     cout<<"enter the run number: ";
     cin>>run_number;
     TString File=Form("/chafs2/work1/triton/hanjie/Rootfiles/tritium_%d.root",run_number);
//     TString File=Form("/chafs1/work1/tritium/pass1/tritium_%d.root",run_number);
//     TString File=Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d.root",run_number);        
//     TString File=Form("/volatile/halla/triton/Marathon_Rootfiles/pass1/tritium_%d.root",run_number);

     if (!gSystem->AccessPathName(File)){
//     T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",run_number));
//     T->Add(Form("/volatile/halla/triton/Marathon_Rootfiles/pass1/tritium_%d*.root",run_number));
//     T->Add(Form("/chafs1/work1/tritium/pass1/tritium_%d*.root",run_number));
        T->Add(Form("/chafs2/work1/triton/hanjie/Rootfiles/tritium_%d*.root",run_number));

        cout<<run_number<<" has been added to the List"<<endl;
     }
    else {cout<<File<<" can not be found! "<<endl;return 0;}
    if(run_number<20000)LHRS=1;
    else RHRS=1;
    kin += run_number;
  }
   TString filename;
   vector< Int_t> runList;
   Int_t runNo;
   if(flag==0){
      cout<<"Input file name:   "; cin>>filename;
      filename = "./runlist/"+filename;
     
      ifstream file(filename.Data());
      if(!file.is_open()){cout<<"!!! file not found "<<endl;return 0;}

      TString content;
      content.ReadToken(file);
      kin=content;
      for(int ii=0;content.ReadToken(file);ii++)
       {  
          runNo = atoi(content);
          cout<<runNo<<" ";
          if(runNo>0)runList.push_back(runNo);
       }

     file.close();
     cout<<endl;

     for(Int_t index=0;index<runList.size();index++)
      {
//        TString File1=Form("/chafs1/work1/tritium/pass1/tritium_%d.root",runList[index]);
       //TString File1=Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d.root",runList[index]);
       TString File1=Form("/chafs2/work1/triton/hanjie/Rootfiles/tritium_%d.root",runList[index]);
//       TString File1=Form("/volatile/halla/triton/Marathon_Rootfiles/pass1/tritium_%d.root",runList[index]);

        if (!gSystem->AccessPathName(File1))
         {
        //    T->Add(Form("/chafs1/work1/tritium/pass1/tritium_%d*.root",runList[index]));
        //    T->Add(Form("/adaqfs/home/a-onl/tritium_work/MARANAL0/replay/Rootfiles/tritium_%d*.root",runList[index]));
            T->Add(Form("/chafs2/work1/triton/hanjie/Rootfiles/tritium_%d*.root",runList[index]));
        //    T->Add(Form("/volatile/halla/triton/Marathon_Rootfiles/pass1/tritium_%d*.root",runList[index]));
              cout<<runList[index]<<" has been added to the List"<<endl;
         }
        else {cout<<File1<<"  cannot be found!!!"<<endl;return 0;}
      }
    if(runList[0]<20000)LHRS=1;
    else RHRS=1;
  }
  if(LHRS)arm=0;
  else arm=1;

  return T;

}
