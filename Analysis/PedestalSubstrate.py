

#===========The parameters that we set for the starting============#
NSCA=13
Full_Gaussian1 = TF1("fit1","gaus",130,170,300)
Full_Gaussian2 = TF1("fit1","gaus",130,170,300)
#============Get the TCahin that we will use later=================#

TChain fchain1("HGCBD")
fchain1.Add("Module_1_RUN_210418_0602.root")
TChain fchain2("HGCBD")
fchain2.Add("Module_1_RUN_210418_0607.root")

#==================make sure the charge run and pedestal run for both (first)===================#
fChain1.SetBranchAddress("hits",&HITS1)
fChain1.GetEntry(0)
vec_inj_ch1.assign(HITS1.inj_ch.begin(),HITS1.inj_ch.begin()+HITS1.inj_ch.size());
if(vec_inj_ch1.size() == 0):
    print 'This run has no injection channel! Pedestal run!'

if(vec_inj_ch1.size() != 0):
    print 'This run has injection channel! Charge run!'
    print 'number of injection channel:'+vec_inj_ch1.size()
    print  'injection channel:'+vec_inj_ch1.at(0)

fChain2.SetBranchAddress("hits",&HITS2)
fChain2.GetEntry(0)
vec_inj_ch2.assign(HITS2.inj_ch.begin(),HITS2.inj_ch.begin()+HITS2.inj_ch.size());
if(vec_inj_ch2.size() == 0):
print 'This run has no injection channel! Pedestal run!'

if(vec_inj_ch2.size() != 0):
print 'This run has injection channel! Charge run!'
print 'number of injection channel:'+vec_inj_ch2.size()
print  'injection channel:'+vec_inj_ch2.at(0)
#=================First for the analysis of the pedestal run and substrate the mean of the gaussian=============#

c1 = TCanvas("c1", "c1",0,0,1000,1000)
nevents1= fChain1.GetEntries()
nhits1 = HITS1.hit_num
h_Pedestal = TFile("Pedestal","",100,150,250)

for ev in range(nevents1):
    fChain1.GetEntry(ev)
    for(int i = 0 ; i < NSCA ; ++i) TS[i] = HITS.rollposition[i];
        for hit in range(nhits):
            H=HITS1.Hits.at(nhits1)
            if(!H.CCorNC) continue
            if(H.chip = 0 and H.ch == 20):
                if(TS[i]<9):
                    h_Pedestal.Fill(H.SCA_hg[0])

h_Pedestal.Fit(Full_Gaussian1,"R")
Ped_Chi=Full_Gaussian1.GetChisquare()
Ped_NDF=Full_Gaussian1.GetNDF()
Ped_Degree_part1=Ped_Chi/Ped_NDF
Ped_Mean=Full_Gaussian1.GetParameter(1)
Ped_Error=Full_Gaussian1.GetParameter(2)

#=======================================For the injection channel analysis=============================#
c2 = TCanvas("c1", "c1",0,0,1000,1000)
nevents2= fChain2.GetEntries()
nhits2 = HITS2.hit_num
h_Charge = TFile("Charge","",100,150,250)

for ev in range(nevents2):
    fChain2.GetEntry(ev)
    for(int i = 0 ; i < NSCA ; ++i) TS[i] = HITS.rollposition[i];
        for hit in range(nhits):
            H=HITS2.Hits.at(nhits2)
            if(!H.CCorNC) continue
            if(H.chip = 0 and H.ch == 20):
                if(TS[i]<9):
                    h_Charge.Fill(H.SCA_hg[0]-Ped_Mean)

h_Charge.Fit(Full_Gaussian2,"R")
Char_Chi=Full_Gaussian2.GetChisquare()
Char_NDF=Full_Gaussian2.GetNDF()
Char_Degree_part2=Char_Chi/Char_NDF
Char_Mean=Full_Gaussian2.GetParameter(1)
Char_Error=Full_Gaussian2.GetParameter(2)

leg = TLegend(0.1,0.4,0.5,0.8)
leg.SetFillColor(0)
leg.SetFillStyle(0)
leg.SetTextSize(0.05)
leg.SetTextFont(22)
leg.SetBorderSize(0)

h_Charge.Draw()
leg.Draw()
c2.Print("HG_Chip0_SCA0_channel20.pdf")
