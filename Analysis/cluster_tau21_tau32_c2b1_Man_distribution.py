import ROOT
import sys
import math
from ROOT import TFile, TH1F, gDirectory, TCanvas, TPad, TProfile,TGraph, TGraphAsymmErrors,TGraphErrors
from ROOT import TH1D, TH1, TH1I, TH2Poly
from ROOT import gStyle
from ROOT import gROOT
from ROOT import TStyle
from ROOT import TLegend
from ROOT import TMath
from ROOT import TPaveText
from ROOT import TLatex
from array import array
#---------------------------------------------Setting the parameters in
#---------------------------------------------setting the hisotgram in and normalize
f1=ROOT.TFile.Open("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_SCA0_subtractmean_inj20_TH2Poly_1.root ",'r')
f2=ROOT.TFile.Open("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_SCA0_subtractmean_inj20_TH2Poly_2.root ",'r')

h1 = f1.Get("NoName")
h2 = f2.Get("NoName")


c = TCanvas("c1", "c1",0,0,500,500)
        
c.Draw("colztext>>h1")
c.Draw("colztext>>h1+")
c.Print("Try.pdf")




