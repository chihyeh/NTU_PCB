import ROOT
import sys
from ROOT import TFile, TH1F, gDirectory, TCanvas, TPad, TProfile,TGraph, TGraphAsymmErrors,TF1
from ROOT import TH1D, TH1, TH1I
from ROOT import gStyle
from ROOT import gROOT
from ROOT import TStyle
from ROOT import TLegend
from ROOT import TMath
from ROOT import TPaveText
from ROOT import TLatex
from array import array
#------------------------------Start to Analyze-------------------------#


for chip in range(0,4):
    for SCA in range (0,14):
        for channel in range (0,32,2):
            c = TCanvas("c1", "c1",0,0,500,500)
            gStyle.SetOptStat(1111)
            gStyle.SetTitleSize(0.05,"XY")
            gStyle.SetTitleFont(62,"XY")
            gStyle.SetLegendFont(62)
            
            f1 = ROOT.TFile.Open("/Users/ms08962476/NTU/RPI_analysis/Analysis/HG_Chip"+str(chip)+"_SCA"+str(SCA)+"_channel"+str(channel)+"_inj20.root", 'r')
            h1 = f1.Get("h")

            g1_part = TF1("fit","gaus",150+(h1.FindFirstBinAbove()-1),150+(h1.FindLastBinAbove()))
            g2_full = TF1("fit1","gaus",150,250)
            g1_part.SetLineColor(4)
            g2_full.SetLineColor(3)
            h1.Fit(g1_part,"R")
            h1.Fit(g2_full,"R+")

            Chi1=g1_part.GetChisquare()
            NDF1=g1_part.GetNDF()
            Number1_part=Chi1/NDF1
            Chi2=g2_full.GetChisquare()
            NDF2=g2_full.GetChisquare()
            Number2_full=Chi2/NDF2
            Mean_part=g1_part.GetParameter(1)
            Error_part=g1_part.GetParameter(2)
            Mean_full=g2_full.GetParameter(1)
            Error_full=g2_full.GetParameter(2)

            leg = TLegend(0.1,0.4,0.5,0.8)
            leg.SetFillColor(0)
            leg.SetFillStyle(0)
            leg.SetTextSize(0.05)
            leg.SetTextFont(22)
            leg.SetBorderSize(0)
            leg.AddEntry(g2_full,"Full range-Mean:"+str(Mean_full),"l")
            leg.AddEntry(g1_part,"From first to last bin-Mean:"+str(Mean_part),"l")
            leg.AddEntry(g2_full,"Full range-Error:"+str(Error_full),"l")
            leg.AddEntry(g1_part,"From first to last bin-Error:"+str(Error_part),"l")
            leg.AddEntry(g2_full,"Full range-#frac{#chi^{2}}{NDF}:"+str(Number2_full),"l")
            leg.AddEntry(g1_part,"From first to last bin-#frac{#chi^{2}}{NDF}:"+str(Number1_part),"l")

            leg.Draw()

            c.Print("HG_Chip"+str(chip)+"_SCA"+str(SCA)+"_channel"+str(channel)+"_inj20_fit.pdf")


