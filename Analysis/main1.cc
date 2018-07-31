#include "makePlots.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include "TFile.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include <typeinfo>
#include "TH2Poly.h"
using namespace std;
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
    chain1->Add("Module_1_RUN_050518_0444.root");
    makePlots M(chain1);
    b=M.Loop1(chip,channel,SCA);
    delete chain1;
    cout << b << endl;
    return b;
}

float Subtract_mean(int file_number1,int file_number2,vector<float> b,int chip,int channel,int time_stamp){
    float a;
    TChain *chain2 = new TChain("HGCBD");
    chain2->Add(Form("Module_1_RUN_050518_%02d%02d.root",file_number1,file_number2));
    makePlots N(chain2);
    a=N.Loop5(b,chip,channel,time_stamp);
    delete chain2;
    cout << a << endl;
    //delete chain2;
    return a;
}
float Subtract_error(int file_number1,int file_number2,vector<float> b,int chip,int channel,int time_stamp){
    float a;
    TChain *chain4 = new TChain("HGCBD");
    chain4->Add(Form("Module_1_RUN_050518_%02d%02d.root",file_number1,file_number2));
    makePlots N(chain4);
    a=N.Loop6(b,chip,channel,time_stamp);
    delete chain4;
    cout << a << endl;
    //delete chain2;
    return a;
}

float Getdac(int file_number1,int file_number2){
    float c;
    TChain *chain3 = new TChain("HGCBD");
    chain3->Add(Form("Module_1_RUN_050518_%02d%02d.root",file_number1,file_number2));
    makePlots O(chain3);
    c=O.Getdac();
    delete chain3;
    cout << c << endl;
    //delete chain2;
    return c;
}



int main()
{
    cout << "Start" << endl;
    gStyle->SetOptFit();
    vector<float> vector_file_number1={1,2,3,4,5,6,7,8,9,10,11,12};
    vector<float> vector_difference;
    for (int chip=0 ; chip<1 ; chip++)
    {
        for( int channel=16 ; channel<24 ; channel+=2 )
        {
            for ( int SCA=1 ; SCA<13 ; SCA++)
            {
                vector_difference.push_back(Pedestal_mean(chip,channel,0)-Pedestal_mean(chip,channel,SCA));
            }
            TGraph *gr;
            TCanvas *c1 = new TCanvas("c4","c4",0,0,500,500);
            TF1 *Full_poly= new TF1("f1","pol1",0,13);
            gr = new TGraph(12,&vector_file_number1[0],&vector_difference[0]);
            gr->Fit(Full_poly,"R");
            gr->SetMarkerStyle(20);
            gr->SetLineColor(2);
            gr->SetMarkerSize(0.8);
            gr->SetMarkerColor(1);
            gr->Draw("AP E");
            gr->SetTitle("<SCAX> - <SCA0>");
            gr->GetXaxis()->SetTitle("X");
            gr->GetYaxis()->SetTitle("ADC");
            gr->GetYaxis()->SetRangeUser(-20,20);
            c1->Print(Form("HG_Chip%d_channel%d_Subtract_mean_Stability.pdf",chip,channel));
            cout << "end" << endl;
            delete gr;
            c1->Clear();
            Full_poly->Clear();
            vector_difference.clear();
        }
    }
}
/*{
    vector<int> channel={8,14,16,18,20,24,60};
    TMultiGraph *Sum_gr = new TMultiGraph();
    TLegend *leg = new TLegend(0.4,0.7,0.9,1);
    for ( int eve=0 ; eve < 1001 ; eve+=500 )
    {
        for( int chip=0 ; chip < 1 ; chip++)
        {
            for(int dac=400 ; dac < 3400 ; dac+=400)
            {
                TGraph *h1;
                TCanvas *c1 = new TCanvas("c4","c4",0,0,500,500);
                cout << "=========================================================================="<< endl;
                cout << "eve:" << eve << endl;
                cout << "dac:" << dac << endl;
                cout << "chip:" << chip << endl;
                cout << "=========================================================================="<< endl;

                TFile* f1 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[0],dac));
                h1=(TGraph*)f1->Get("Graph");
                h1->SetMarkerColor(2);
                h1->SetMarkerStyle(22);
                h1->SetMarkerSize(0.6);
                c1->cd();
                h1->Draw("AP");
                leg->AddEntry(h1,Form("channel:%d",channel[0]));
                Sum_gr->Add(h1,"AP");
                cout << "aaa" << endl;

                TFile* f2 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[1],dac));
                h2=(TGraph*)f2->Get("Graph");
                h2->SetMarkerColor(2);
                h2->SetMarkerStyle(22);
                h2->SetMarkerSize(0.8);
                //h2->Draw("AP");
                //leg->AddEntry(h2,Form("channel:%d",channel[1]));
                //Sum_gr->Add(h2,"AP");
                cout << "bbb" << endl;
                
                TFile* f3 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[2],dac));
                h3=(TGraph*)f3->Get("Graph");
                h3->SetMarkerColor(3);
                h3->SetMarkerStyle(22);
                h3->SetMarkerSize(0.8);
                //h3->Draw("AP");
                //leg->AddEntry(h3,Form("channel:%d",channel[2]));
                //Sum_gr->Add(h3,"AP");
                cout << "ccc" << endl;
                
                TFile* f4 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[3],dac));
                h4=(TGraph*)f4->Get("Graph");
                h4->SetMarkerColor(4);
                h4->SetMarkerStyle(22);
                h4->SetMarkerSize(0.8);
                //h4->Draw("AP");
                //leg->AddEntry(h4,Form("channel:%d",channel[3]));
                //Sum_gr->Add(h4,"AP");
                cout << "ddd" << endl;
                
                TFile* f5 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[4],dac));
                h5=(TGraph*)f5->Get("Graph");
                h5->SetMarkerColor(5);
                h5->SetMarkerStyle(22);
                h5->SetMarkerSize(0.8);
                //h5->Draw("AP");
                //leg->AddEntry(h5,Form("channel:%d",channel[4]));
                //Sum_gr->Add(h5,"AP");
                cout << "eee" << endl;
                
                TFile* f6 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[5],dac));
                h6=(TGraph*)f6->Get("Graph");
                h6->SetMarkerColor(6);
                h6->SetMarkerStyle(22);
                h6->SetMarkerSize(0.8);
                //h6->Draw("AP");
               // leg->AddEntry(h6,Form("channel:%d",channel[5]));
                //Sum_gr->Add(h6,"AP");
                cout << "hhh" << endl;
                
                TFile* f7 = TFile::Open(Form("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_evt_%d_chip_%d_channel_%d_dac_%d.root",eve,chip,channel[6],dac));
                h7=(TGraph*)f7->Get("Graph");
                h7->SetMarkerColor(6);
                h7->SetMarkerStyle(22);
                h7->SetMarkerSize(0.8);
                //h6->Draw("AP");
                // leg->AddEntry(h6,Form("channel:%d",channel[5]));
                //Sum_gr->Add(h6,"AP");
                cout << "mmm" << endl;

                leg->Draw();
                cout << "iii" << endl;
                Sum_gr->Draw();
                cout << "jjj" << endl;
                leg->SetFillColor(0);
                leg->SetFillStyle(0);
                leg->SetTextSize(0.05);
                leg->SetBorderSize(0);
                leg->SetTextFont(22);
                cout << "kkk" << endl;
                c1->Print(Form("HG_evt_%d_chip_%d_dac_%d.pdf",eve,chip,dac));
                cout << "lll" << endl;
            
                leg->Clear();
                Sum_gr->Clear();
    //==============================//
                delete h1;
                delete h2;
                delete h3;
                delete h4;
                delete h5;
                delete h6;
                delete h7;
    //==============================//
                delete c1;
    //==============================//
                delete f1;
                delete f2;
                delete f3;
                delete f4;
                delete f5;
                delete f6;
                delete f7;
    //==============================//

            }
            
        }
    }
}*/


/*{
vector<int> channel={8,14,16,18,20,24,60};
vector<int> vector_file_number1={4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
vector<int> vector_file_number2={55,56,57,59,0,2,3,4,6,7,8,10,11,13,14,15,17,19};
for (int channel_run=0 ; channel_run < channel.size()+1 ; channel_run++)
    for(int number=1 ; number < vector_file_number1.size()+1 ; number++)
    {
        {
        TChain *chain8 = new TChain("HGCBD");
        chain8->Add(Form("Module_1_RUN_050518_%02d%02d.root",vector_file_number1[number],vector_file_number2[number]));
        makePlots O(chain8);
        O.Pulseshape(channel[channel_run]);
        delete chain8;
        cout << "finish" << ":" << channel[channel_run] << endl;
        }
    }
return 0;
            }*/
/*{
    
gStyle->SetOptFit();
vector<int> vector_file_number1={4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
vector<int> vector_file_number2={55,56,57,59,0,2,3,4,6,7,8,10,11,13,14,15,17,19};
vector<float> vector_dac;
vector<float> vector_Subtract_mean;
vector<float> vector_Subtract_errorx={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
vector<float> vector_Subtract_errory;
vector<float> vector_Pedestal_mean_SCA_order;

for (int chip=2 ; chip < 4; chip++){
    for (int channel=0 ; channel < 64; channel+=2){
            for (int SCA_use=0 ; SCA_use<13 ; SCA_use++)
            {
                vector_Pedestal_mean_SCA_order.push_back(Pedestal_mean(chip,channel,SCA_use));
            }
            for(int time_stamp=4 ; time_stamp<5 ; time_stamp++){
            for( int number=0 ; number < 18 ; number++ ){
                cout <<"=================================================="<<endl;
                cout <<" chip:" << chip << endl;
                cout <<" channel:" << channel << endl;
                cout <<"=================================================="<<endl;
                cout << "DAQ:" << Getdac(vector_file_number1[number],vector_file_number2[number]) << endl;
                cout << "Subtract_mean:" << Subtract_mean(vector_file_number1[number],vector_file_number2[number],vector_Pedestal_mean_SCA_order,chip,channel,time_stamp) << endl;
                cout << "Subtract_error:" << Subtract_error(vector_file_number1[number],vector_file_number2[number],vector_Pedestal_mean_SCA_order,chip,channel,time_stamp) << endl;
                vector_dac.push_back(Getdac(vector_file_number1[number],vector_file_number2[number]));
                vector_Subtract_mean.push_back(Subtract_mean(vector_file_number1[number],vector_file_number2[number],vector_Pedestal_mean_SCA_order,chip,channel,time_stamp));
                vector_Subtract_errory.push_back(Subtract_error(vector_file_number1[number],vector_file_number2[number],vector_Pedestal_mean_SCA_order,chip,channel,time_stamp));
            }
            TGraphErrors *gr;
            TCanvas *c1 = new TCanvas("c4","c4",0,0,500,500);
            TF1 *Full_poly= new TF1("f1","pol1",1500,4000);
            gr = new TGraphErrors(18,&vector_dac[0],&vector_Subtract_mean[0],&vector_Subtract_errorx[0],&vector_Subtract_errory[0]);
            gr->Fit(Full_poly,"R");
            gr->SetMarkerStyle(20);
            gr->SetLineColor(2);
            gr->SetMarkerSize(0.8);
            gr->SetMarkerColor(1);
            gr->Draw("AP");
            gr->SetTitle(Form("HG_Chip%d_channel%d_Pedestal_TS%d_Subtract_mean_and_RMS.pdf",chip,channel,time_stamp));
            gr->GetXaxis()->SetTitle("dac");
            gr->GetYaxis()->SetTitle("Mean and RMS (PED subtrction)");
            gr->GetYaxis()->SetRangeUser(-30,30);
            c1->Print(Form("HG_Chip%d_channel%d_Pedestal_TS%d_Subtract_mean_and_RMS.pdf",chip,channel,time_stamp));
            cout << "end" << endl;
            delete gr;
            c1->Clear();
            Full_poly->Clear();
            vector_dac.clear();
            vector_Subtract_mean.clear();
            vector_Subtract_errory.clear();
            }}}
    return(0);
}*/
/*for (int chip=0 ; chip < 1; chip++){
    for (int channel=0 ; channel < 2 ; channel+=2){
        for ( int SCA=0 ; SCA <2  ; SCA++){
            for( int number=0 ; number < 18 ; number++ ){
            cout <<"=================================================="<<endl;
            cout <<" chip:" << chip << endl;
            cout <<" channel:" << channel << endl;
            cout <<" SCA:" << SCA << endl;
            cout <<"=================================================="<<endl;
            float d=Pedestal_mean(chip,channel,SCA);
            cout << "DAQ:" << Getdac(vector_file_number1[number],vector_file_number2[number]) << endl;
            cout << "Subtract_mean:" << Subtract_mean(vector_file_number1[number],vector_file_number2[number],d,chip,channel,SCA) << endl;
            cout << "Subtract_error:" << Subtract_error(vector_file_number1[number],vector_file_number2[number],d,chip,channel,SCA) << endl;
            vector_dac.push_back(Getdac(vector_file_number1[number],vector_file_number2[number]));
            vector_Subtract_mean.push_back(Subtract_mean(vector_file_number1[number],vector_file_number2[number],d,chip,channel,SCA));
            vector_Subtract_errory.push_back(Subtract_error(vector_file_number1[number],vector_file_number2[number],d,chip,channel,SCA));
            }
            TGraph *gr;
            TCanvas *c1 = new TCanvas("c4","c4",0,0,500,500);
            TF1 *Full_poly= new TF1("f1","pol1",0,4000);
            gr = new TGraph(18,&vector_dac[0],&vector_Subtract_errory[0]);
            gr->Fit(Full_poly,"R");
            gr->SetMarkerStyle(20);
            gr->SetLineColor(2);
            gr->SetMarkerSize(0.8);
            gr->SetMarkerColor(1);
            gr->Draw("AP");
            gr->SetTitle(Form("Chip%d_channel%d_SCA%d_Subtract_error.pdf",chip,channel,SCA));
            gr->GetXaxis()->SetTitle("dac");
            gr->GetYaxis()->SetTitle("Pedestal_Subtract_error");
            gr->GetYaxis()->SetRangeUser(0,25);
            c1->Print(Form("Chip%d_channel%d_SCA%d_Subtract_error.pdf",chip,channel,SCA));
            cout << "end" << endl;
            delete gr;
            c1->Clear();
            Full_poly->Clear();
            vector_dac.clear();
            vector_Subtract_mean.clear();
            vector_Subtract_errory.clear();
                    }}}
return(0);
}*/
    

    //===================================Subtract TH2Poly study=======================================//
    /*TCanvas *c4 = new TCanvas("c4","c4",0,0,500,500);
    float d=0;
    float c=0;
    cout << "Hello" << endl;
    for (int SCA=0; SCA <13  ; SCA ++){
        char plot_title1[50];
        readmap();
        TH2Poly *poly = new TH2Poly;
        InitTH2Poly(*poly);
        for(int chip=0 ; chip<4; chip++){
            for (int channel=0 ; channel < 66 ; channel+=2){
                if(channel==20){
                    c=-15;
                    int forCH = chip*32+channel/2;
                    float X = CHmap[forCH].first;
                    float Y = CHmap[forCH].second;
                    poly->Fill(X,Y,c);}
                else{
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
                d=Pedestal_mean(chip,channel,SCA);
                c=Subtract(d,chip,channel,SCA);
                cout << "Subtract :" << c << endl;
                cout << "=============================================================" <<endl;
                
                cout << "=============================================================" <<endl;
                
                poly->Fill(X,Y,c);
                cout << "=====bbbbbbbbb======" << endl;
                }}}
        c4->cd();
        gStyle->SetOptStat(0);
        poly->Draw("colztext");
        poly->SetTitle(Form("HG_SCA%d_subtractmean_inj20_TH2Poly",SCA));
        poly->SetXTitle("X position");
        poly->SetYTitle("Y position");
        poly->SetZTitle("Subtract mean");
        poly->GetZaxis()->SetRangeUser(-5,5); //0,15
        //   poly->Draw();
        cout << "=====bbcccceeeeeeeecccccbb======" << endl;
        cout << "=====bbcccceffffffffecccccbb======" << endl;
        c4->Print(Form("HG_SCA%d_subtractmean_inj20_TH2Poly.pdf",SCA));
        cout << "0000000000000000000000000000000000000000000000" <<endl;
        delete poly;
        c4->Clear();}*/
    
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
    //M.calib();*/


