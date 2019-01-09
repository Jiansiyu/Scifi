#include "mapping.hh"


const Int_t nch = 64;

void mapping( Double_t nscifi[]){
  
  char chmapdata[500];
  sprintf(chmapdata,"SciFi_chmap.dat");
  // ifstream* ifs = new ifstream("SciFi_chmap.dat");
  ifstream* ifs = new ifstream(chmapdata);
  cout << " _________________________________________" << endl;
  cout << " Channel map, " << chmapdata << " is used." << endl;
  cout << " _________________________________________" << endl;


  for(Int_t i=0 ; i<nch ; i++){
    Int_t ttemp, ttemp2;
    char ttempc[500];
    *ifs >> ttemp >> ttempc >> ttemp2 >> nscifi[i];
    nscifi[i] = nscifi[i] - 1; // Subtraction by 1 because # starts from 1
  }
  ifs->close();

}


