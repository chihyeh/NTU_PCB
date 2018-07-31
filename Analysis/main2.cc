#include "makePlots.h"
#include <fstream>
#include <iostream>

int main(){
  TChain *chain1 = new TChain("HGCBD");
  chain1->Add("Module_1_RUN_210418_0602.root");
  TChain *chain2 = new TChain("HGCBD");
  chain2->Add("Module_1_RUN_210418_0607.root");
for (int chip=0 ; chip<1 ; chip++){
    for (int channel=0 ; channel < 64 ; channel+=2){
        for ( int SCA=0 ; SCA < 13 ; SCA++ ){
          cout << "chip :"<<chip<<"channel :"<<channel<<"SCA :"<<SCA;
          //makePlots M(chain1);
          //makePlots N(chain2);
          makePlots O(chain2);
          O.Loop2(chip,channel,SCA);
          //N.Loop3(M.Loop1(chip,channel,SCA),chip,channel,SCA);
            continue;
  //M.calib();
  
  return(0);
        }}}}

