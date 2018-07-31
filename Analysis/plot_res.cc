#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TROOT.h" //gROOT
#include "TLine.h"
#include "TStyle.h" //gStyle
#include <vector>

using namespace std;
int main(){
  
  TApplication *app = new TApplication("app",0,0);
  TCanvas *c1 = new TCanvas();
  TF1 *fit = new TF1("fit","[0]*x",0,4096);


  vector<int> DAC;
  vector<int> pulse_amp;

  for(int i = 0 ; i < 400 ; ++i){
    DAC.push_back(i*10);
    pulse_amp.push_back(i);
  }


  TH1F * h1 = new TH1F("h1","h1",4096,0,4096);
  h1->Sumw2();
  TAxis *xaxis = h1->GetXaxis();
  gStyle->SetOptStat(0);
  for(int i = 0 ; i < (int)DAC.size() ; ++i){
    
    int ibin = xaxis->FindBin( DAC.at(i) );
    h1->SetBinContent( ibin , pulse_amp.at(i) );
    h1->SetBinError( ibin, 1e-5);
  }
  h1->SetMarkerStyle(22);
  h1->SetMarkerSize(1.2);
  h1->SetMarkerColor(2);
  h1->SetXTitle("");
  h1->Fit(fit);
  h1->SetTitle("DAC Linearity");
  h1->SetTitleSize(28);
  fit->SetLineColor(1);
  
  //Ratio plot
  TH1F * hratio = new TH1F("hratio","hratio",4096,0,4096);
  hratio->Sumw2();
  xaxis = hratio->GetXaxis();
  gStyle->SetOptStat(0);
  double p0 = fit->GetParameter(0);
  float residual;
  for(int i = 0 ; i < (int)DAC.size() ; ++i){
    //cout << pulse_amp.at(i) << "," << p0*DAC.at(i) << endl;
    if( DAC.at(i) == 0 ) continue;
    residual = (pulse_amp.at(i) - p0*DAC.at(i))/(p0*DAC.at(i));
    //cout << residual << endl;
    int ibin = xaxis->FindBin( DAC.at(i) );
    hratio->SetBinContent( ibin , residual );
    hratio->SetBinError( ibin, 1e-5);
  }
  hratio->SetMarkerStyle(22);
  hratio->SetMarkerSize(1.2);
  hratio->SetMarkerColor(2);

  
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad
  h1->Draw();               // Draw h1
  fit->Draw("same");
  
  // Do not draw the Y axis label on the upper plot and redraw a small
  // axis instead, in order to avoid the first label (0) to be clipped.
  //h1->GetYaxis()->SetLabelSize(0.);
  h1->SetYTitle("pulse_amp(mV)");
  TAxis *axis = h1->GetYaxis();
  axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  axis->SetLabelSize(15);
  axis->SetTitleSize(0.06);
  axis->SetTitleOffset(0.7);
  axis->Draw();

  //h1->SetXTitle("DAC");
  //h1->GetXaxis()->SetTitleOffset(4);
  axis = h1->GetXaxis();
  axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  axis->SetLabelSize(0);
  axis->Draw();

  // lower plot will be in pad
  c1->cd();          // Go back to the main canvas before defining pad2

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
  pad2->SetTopMargin(0.1);
  pad2->SetBottomMargin(0.2);
  pad2->SetGridx(); // vertical grid
  pad2->Draw();
  pad2->cd();       // pad2 becomes the current pad
  hratio->SetTitle("");
  //hratio->SetXTitle("DAC");
  //hratio->SetYTitle("ratio");
  
  hratio->SetMaximum(0.1);
  hratio->SetMinimum(-0.1); 
  //hratio->Draw();
  
  hratio->GetYaxis()->SetTitle("#frac{Amp - Fit}{Fit}");
  hratio->GetYaxis()->SetTitleOffset(0.35);
  hratio->GetYaxis()->SetLabelFont(43); 
  hratio->GetYaxis()->SetLabelSize(15);

  axis = hratio->GetYaxis();
  axis->SetNdivisions(404);
  axis->Draw();

  axis = hratio->GetXaxis();
  axis->SetTitle("DAC");
  axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  axis->SetLabelSize(15);
  
  

  hratio->GetXaxis()->SetTitleSize(0.12);
  hratio->GetXaxis()->SetTitleOffset(0.76);
  hratio->GetYaxis()->SetTitleSize(0.12);
  //axis->Draw();

  
  hratio->Draw();
  //Add TLine to show mean value
  TLine *Gline = new TLine(0,0,4096,0);
  Gline->SetLineColor(1);
  Gline->SetLineWidth(4.8);
  Gline->SetLineStyle(7);
  Gline->Draw();  
  c1->Update();
  getchar();
  c1->SaveAs("DAC_Linearity.png");
  
}
