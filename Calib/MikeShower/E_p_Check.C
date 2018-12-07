#include "../TRI_Main.h" //Used for th RunNoChain to Tchain all of the TTrees together     
#include "../TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together 





void E_p_Check(TString filename=""){


  if(filename==""){ 
cout<<"Input file name:   "; 
cin>>filename;
}
  filename = "/adaqfs/home/a-onl/tritium_work/Runlist/" + filename;
  ifstream file(filename.Data());
  if(!file.good()){cout << filename.Data() << "does not exist! "<<endl; exit(1);}
  TString content;
  TString Target,Kin,Run_String;
  const char* kin;
  for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {
    cout<<"!!!:  "<<content<<endl;
    if(ii==0)Target = content;
    if(ii==1){
      kin = content.Data();
      Kin = Form("Kin%s", kin);
    }
    if(ii==2)Run_String = content;
  }
  file.close();



  ///////////////////////////////////////////////////                                                                                                                             
  ///Call tri_tools functions and combine root files                                                                                                                              
  const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
  TChain* T =(TChain*) gGetTree(RunNoChain, "T");






}
