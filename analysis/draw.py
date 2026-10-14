#based on https://git.km3net.de/vpestel/obelix-investigation/-/blob/main/energy/nb_triggered_pmts.py?ref_type=heads#L16
import numpy as np
import matplotlib.pyplot as plt
import km3io as kio
import awkward as ak
import pandas as pd

import sys
import ROOT

import math

ROOT.gSystem.Load("libKM3NeTROOT")
ROOT.gROOT.SetBatch(False)


from km3io.definitions import fitparameters as kfit
from km3io.tools import (
    to_num,
    cached_property,
    unfold_indices,
    unique,
    uniquecount,
    fitinf,
    count_nested,
    mask,
    best_track,
    get_w2list_param,
    get_multiplicity,
    has_jmuon,
    has_jshower,
    has_aashower,
    has_dusjshower,
    best_jmuon,
    best_jshower,
    best_aashower,
    best_dusjshower,
    is_cc,
    is_bit_set,
    is_3dshower,
    is_mxshower,
    is_3dmuon,
    is_nanobeacon,
    TimeConverter,
    phi,
    theta,
    zenith,
    azimuth,
    angle,
    angle_between,
    magnitude,
    unit_vector,
)

colours = [ROOT.kBlue, ROOT.kRed, ROOT.kGreen, ROOT.kBlack]

def get_livetime(filename):
  head = ROOT.TFile(filename).Get("Head") 
  livetime = head.mc_livetime()
  return livetime

filenames = [ 'mupage.run_0023.std.root', 'mupage.run_0023.root' ]

f = ROOT.TFile("out.root", "RECREATE")

for filename in filenames:
  nweird = 0
  input_file = kio.OfflineReader(filename)
  livetime = get_livetime(filename)
  f.cd()

  h   = ROOT.TH1F(filename,"",20,-10,10)
  hm1 = ROOT.TH1F(filename+"m1","",20,-10,10)
  hmu = ROOT.TH1F(filename+"mu","",20,-10,10)
 
  nev = 0
  totw = 0
  nev1 = 0
  totw1 = 0
  for evt in input_file:
    if len(evt.mc_trks.E) > 1: 
        nev = nev + 1
        if len(evt.w) == 0: 
            weight = 1
        else:
            weight = evt.w[0]
        if len(evt.mc_trks.E) == 2: 
          hm1.Fill(math.log10(evt.mc_trks.E[0]), weight) #weight written
          nev1 = nev1 + 1
          totw1 += weight
        h.Fill(math.log10(evt.mc_trks.E[0]), weight)
        for idx, E in enumerate(evt.mc_trks.E): 
          if idx != 0: hmu.Fill(math.log10(E), weight)
        totw += weight
    #else: print(evt.fUniqueID)
  h.Scale(1./livetime)
  hm1.Scale(1./livetime)
  hmu.Scale(1./livetime)
  f.cd()
  h.Write()
  hm1.Write()
  hmu.Write()
  print("ev: ", nev, "totweight", totw, "livetime", livetime, "livetime rescaled to weight", livetime/totw*nev, "ev m=1:", nev1, "weight m=1", totw1)
f.Write()
f.Close()

f = ROOT.TFile("out.root")

c1 = ROOT.TCanvas()
c1.SetLogy()

leg = ROOT.TLegend(0.1,0.7,0.48,0.9)
for i,filename in enumerate(filenames):
  h = f.Get(filename)
  hm1 = f.Get(filename+"m1")
  hmu = f.Get(filename+"mu")

  if filename == filenames[0]: 
      h.Draw()
      h.SetLineColor(colours[i])
      h.SetMarkerStyle(20)
      h.SetMarkerColor(colours[i])
      h.GetXaxis().SetTitle("E_bundle[GeV]")
      h.GetYaxis().SetTitle("Nevents")
      leg.AddEntry(h,filename+" all, bundle energy","lp")
      hm1.Draw("same")
      hm1.SetLineColor(colours[i])
      hm1.SetMarkerStyle(21)
      hm1.SetMarkerColor(colours[i])
      leg.AddEntry(hm1,filename+" m1","lp")
      hmu.Draw("same")
      hmu.SetLineColor(colours[i])
      hmu.SetMarkerStyle(22)
      hmu.SetMarkerColor(colours[i])
      leg.AddEntry(hmu,filename+" all, mu energy","lp")
  else:  
      h.Draw("same")
      h.SetLineColor(colours[i])
      h.SetMarkerStyle(20)
      h.SetMarkerColor(colours[i])
      leg.AddEntry(h,filename+" all","lp")
      hm1.SetLineColor(colours[i])
      hm1.SetMarkerStyle(21)
      hm1.SetMarkerColor(colours[i])
      hm1.Draw("same")
      leg.AddEntry(hm1,filename+" m1","lp")
      hmu.Draw("same")
      hmu.SetLineColor(colours[i])
      hmu.SetMarkerStyle(22)
      hmu.SetMarkerColor(colours[i])
      leg.AddEntry(hmu,filename+" all, mu energy","lp")
 
leg.Draw()
c1.SaveAs("out.png")
