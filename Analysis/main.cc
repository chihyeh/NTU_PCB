#include "makePlots.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include "TFile.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include <typeinfo>
#include "TH2Poly.h"

void InitTH2Poly(TH2Poly& poly)
{
    int MAXVERTICES = 6;
    double HexX[MAXVERTICES];
    double HexY[MAXVERTICES];
    int iu,iv,CellXYsize;
    ifstream file("src_txtfile/poly_frame.txt");
    string line;
    
    
    for(int header = 0; header < 4; ++header )     getline(file,line);
    
    while(true){
        getline(file,line);
        if( file.eof() ) break;
        file >> iu >> iv >> CellXYsize;
        for(int i = 0; i < CellXYsize ; ++i){
            getline(file,line);
            file >> HexX[i] >> HexY[i];
        }
        
        /*if(CellXYsize == 4){
         CellXYsize++;
         HexX[4] = HexX[0];
         HexY[4] = HexY[0];}*/
        poly.AddBin(CellXYsize, HexX, HexY);
    }
    file.close();
    
}
map<int,pair < double,double > > CHmap;
void readmap(){
    ifstream file("./src_txtfile/CH_map.txt");
    string line;
    int chip,ch,type,formatCH;
    double posx,posy;
    while(true){
        getline(file,line);
        if( file.eof() ) break;
        file >> chip >> ch >> posx >> posy >> type;
        formatCH = chip*32+ch/2;
        CHmap[formatCH] = make_pair(posx,posy);}
    file.close();
    //Since there is no such pad, assign a unreasonable value
    CHmap[2*32+60/2] = make_pair(1000.,1000.);
    
}
float Pedestal_mean(int chip,int channel,int SCA){
    float b;
    TChain *chain1 = new TChain("HGCBD");
    chain1->Add("Module_1_RUN_210418_0602.root");
    makePlots M(chain1);
    b=M.Loop1(chip,channel,SCA);
    cout << b << endl;
    return b;
}

float Subtract(float b,int chip,int channel,int SCA){
    float a;
    TChain *chain2 = new TChain("HGCBD");
    chain2->Add("Module_1_RUN_210418_0607.root");
    makePlots N(chain2);
    a=N.Loop3(b,chip,channel,SCA);
    cout << a << endl;
    //delete chain2;
    return a;
}

int main(){
    
cout << "Hello" << endl;
    for(int SCA=0; SCA <0 ; SCA++){
        char plot_title1[50];
        readmap();;
        TH2Poly *poly = new TH2Poly;
        InitTH2Poly(*poly);
        
        for(int chip=0 ; chip<1 ; chip++){
            for (int channel=0 ; channel < 4 ; channel+=2){
                cout << "=============================================================" <<endl;
                cout << "chip :" << chip <<endl;
                cout << "channel :" << channel <<endl;
                cout << " SCA :" << SCA << endl;
                cout << "=============================================================" <<endl;
                int forCH = chip*32+channel/2;
                float X = CHmap[forCH].first;
                float Y = CHmap[forCH].second;
                cout << "=============================================================" <<endl;
                cout << "X :" << X <<endl;
                cout << "Y :" << Y <<endl;
                float d=Pedestal_mean(chip,channel,SCA);
                float c=Subtract(d,chip,channel,SCA);
                cout << "Subtract :" << c << endl;
                cout << "=============================================================" <<endl;
                
                cout << "=============================================================" <<endl;
                
                poly->Fill(X,Y,c);
                cout << "=====bbbbbbbbb======" << endl;
            }}
        TCanvas *c4 = new TCanvas("c4","c4",0,0,500,500);
        c4->cd();
        poly->Draw("colztext");
        //   poly->Draw();
        cout << "=====bbcccceeeeeeeecccccbb======" << endl;
        sprintf(plot_title1,"HG_chip0_subtractmean_inj20_Th2Poly");
        cout << "=====bbcccceffffffffecccccbb======" << endl;
        c4->Print(Form("HG_SCA%d_subtractmean_inj20_TH2Poly.pdf",SCA));
        cout << "0000000000000000000000000000000000000000000000" <<endl;
        delete poly;}
    
/*int   forCH = 0;
float X = CHmap[forCH].first;
float Y = CHmap[forCH].second;
    
TH2Poly *poly = new TH2Poly;
InitTH2Poly(*poly);*/
    

/*float d=Pedestal_mean(chip,channel,SCA);
cout <<"d: " <<d << endl;

//float c=Subtract(d,chip,channel,SCA);
//cout <<"c: " <<c << endl;

   // TCanvas *c4 = new TCanvas("c4","c4",0,0,500,500);
//c4->cd();

//TCanvas *c4 = new TCanvas;

    
poly->Fill(X,Y,d);
cout << "=====bbcccccccccccccbb======" << endl;*/
    
//poly->Draw("colztext");
 //   poly->Draw();
//cout << "=====bbcccceeeeeeeecccccbb======" << endl;
//sprintf(plot_title1,"HG_chip0_submean");
//cout << "=====bbcccceffffffffecccccbb======" << endl;
//c4->Print(Form("HG_chip0_submean.pdf"));
//cout << "0000000000000000000000000000000000000000000000" <<endl;

  //makePlots O(chain2);
  //O.Loop2(chip,channel,SCA);
  //M.calib();
  
  return(0);
            }

