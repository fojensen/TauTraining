#include <TCut.h>
#include <iostream>
#include <TLegend.h>
#include <TH1D.h>
#include <TChain.h>
#include <TCanvas.h>

double addOverflow(TH1D *h)
{
   const int n = h->GetNbinsX();
   const double y = h->GetBinContent(n+1);
   if (y) std::cout << h->GetName() << " has overflow" << std::endl;
   h->SetBinContent(n+1, 0);
   h->SetBinError(n+1, 0.);
   h->AddBinContent(n, y);
   return y;
}

double addUnderflow(TH1D *h)
{
   const double y = h->GetBinContent(0);
   if (y) std::cout << h->GetName() << " has underflow" << std::endl;
   h->SetBinContent(0, 0);
   h->SetBinError(0, 0.);
   h->AddBinContent(1, y);
   return y;
}

void runPoint(TH1D * htemp, const TString var, const bool dolog=false)
{
   const int n = 6;
   TString filetag[n];
   TCut cuts[n];
   filetag[0] = "WToLNu_2J";            cuts[0] = "drmin_tau_tau<0.4";
   filetag[1] = "DYToLL-M-50_2J";       cuts[1] = "drmin_tau_tau<0.4";
   filetag[2] = "TTTo2L2Nu";            cuts[2] = "drmin_tau_tau<0.4";
   filetag[3] = "GluGluHToTauTau";      cuts[3] = "drmin_tau_tau<0.4";
   filetag[4] = "VBFHToTauTau";         cuts[4] = "drmin_tau_tau<0.4";
   filetag[5] = "QCD_Flat_Pt-15to7000"; cuts[5] = "drmin_tau_tau>=0.4";

   TH1D * h[n];
   for (int i = 0; i < n; ++i) {
      const TString hname = "h_"+TString(htemp->GetName())+"_"+TString::Itoa(i, 10);
      h[i] = (TH1D*)htemp->Clone(hname);
      TChain chain("skimmedTree");
      char buffer[100];
      sprintf(buffer, "./outputData/skim_%s.root", filetag[i].Data()); 
      chain.Add(buffer);
      if (chain.GetEntries()) {
         const double nentries = chain.Project(hname, var, cuts[i]);
         std::cout << filetag[i] << " has " << nentries << " entries." << std::endl;
         addOverflow(h[i]);
         addUnderflow(h[i]);
         h[i]->Scale(1./nentries);
         h[i]->SetLineWidth(2);
         h[i]->SetLineColor(3+i);
      }
   }
   h[2]->SetLineColor(2);

   TCanvas * canvas = new TCanvas("canvas_"+var, var, 400, 400);
   double max = 0.;
   for (int i = 0; i < n; ++i) {
      if (h[i]->GetMaximum()>max) max = h[i]->GetMaximum();
      h[i]->Draw("HIST, E, SAME");      
   }
   
   h[0]->SetMinimum(0.);
   h[0]->SetMaximum(1.1*max);
   h[0]->SetStats(0);
   if (dolog) {
      h[0]->SetMaximum(1.);
      h[0]->SetMinimum(1.e-4);
      canvas->SetLogy();
   }

   TLegend * l = new TLegend(0.25, 0.8, 0.875, 0.875);
   l->SetBorderSize(0);
   l->SetNColumns(3);
   for (int i = 0; i < n; ++i) {
      l->AddEntry(h[i], filetag[i], "L");
   }
   l->Draw();

   char hname[100];
   sprintf(hname, "./plots/dists_%s.pdf", htemp->GetName());
   canvas->SaveAs(hname);
}

void plotTrainingVariables()
{
   TH1D h_chargedIsoPtSum("h_chargedIsoPtSum", ";chargedIsoPtSum;#tau_{h} candidates / 1 GeV", 40, 0., 40.);
   runPoint(&h_chargedIsoPtSum, "chargedIsoPtSum", true);

   TH1D h_neutralIsoPtSum("h_neutralIsoPtSum", ";neutralIsoPtSum;#tau_{h} candidates / 1 GeV", 40, 0., 40.);
   runPoint(&h_neutralIsoPtSum, "neutralIsoPtSum", true);
   
   TH1D h_decayMode("h_decayMode", ";decayMode;# of #tau_{h} / bin", 12, -0.5, 11.5);
   runPoint(&h_decayMode, "decayMode", false);

   TH1D h_leadChargedHadrCand_dxy("h_leadChargedHadrCand_dxy", ";TMath::Sqrt(TMath::Abs(leadChargedHadrCand_dxy));#tau_{h} candidates / 0.01", 40, 0., 0.2);
   runPoint(&h_leadChargedHadrCand_dxy, "TMath::Sqrt(TMath::Abs(leadChargedHadrCand_dxy))", false);

   TH1D h_leadChargedHadrCand_dxysig("h_leadChargedHadrCand_dxysig", ";TMath::Abs(leadChargedHadrCand_dxysig);#tau_{h} candidates / 0.25", 40, 0., 10.);
   runPoint(&h_leadChargedHadrCand_dxysig, "TMath::Abs(leadChargedHadrCand_dxysig)", false);

   TH1D h_hasSecondaryVertex("h_hasSecondaryVertex", ";hasSecondaryVertex;#tau_{h} candidates / bin", 2, -0.5, 1.5);
   runPoint(&h_hasSecondaryVertex, "hasSecondaryVertex", false);

   TH1D h_flightLength("h_flightLength", ";TMath::Sqrt(TMath::Abs(flightLength));#tau_{h} candidates / bin", 40, -0.25, 3.);
   runPoint(&h_flightLength, "hasSecondaryVertex? TMath::Sqrt(TMath::Abs(flightLength)) : -0.1", true);

   TH1D h_flightLengthSig("h_flightLengthSig", ";TMath::Abs(flightLengthSig);#tau_{h} candidates / bin", 40, -0.25, 10.);
   runPoint(&h_flightLengthSig, "hasSecondaryVertex? TMath::Abs(flightLengthSig): -0.1", true);

   TH1D h_puCorrPtSum("h_puCorrPtSum", ";puCorrPtSum;#tau_{h} candidates / 10 GeV", 30, 0., 300.);
   runPoint(&h_puCorrPtSum, "puCorrPtSum", true);
 
   TH1D h_pt("h_pt", ";pt;#tau_{h} candidates / 10 GeV", 20, 20., 220.);
   runPoint(&h_pt, "pt", true);

   TH1D h_eta("h_eta", ";TMath::Abs(eta);#tau_{h} candidates / 0.2", 15, 0., 3.);
   runPoint(&h_eta, "TMath::Abs(eta)", false);

   TH1D h_photonPtSumOutsideSignalCone("h_photonPtSumOutsideSignalCone", ";photonPtSumOutsideSignalCone;#tau_{h} candidates / 0.25 GeV", 40, 0., 10.);
   runPoint(&h_photonPtSumOutsideSignalCone, "photonPtSumOutsideSignalCone", true);

   TH1D h_ip3d("h_ip3d", ";TMath::Sqrt(TMath::Abs(ip3d));#tau_{h} candidates / 0.005", 40, 0., 0.2);
   runPoint(&h_ip3d, "TMath::Sqrt(TMath::Abs(ip3d))", false);

   TH1D h_ip3d_Sig("h_ip3d_Sig", ";TMath::Abs(ip3d_Sig);#tau_{h} candidates / 0.25", 40, 0., 10.);
   runPoint(&h_ip3d_Sig, "TMath::Abs(ip3d_Sig)", false);

   TH1D h_signalGammaCands_size("h_signalGammaCands_size", ";signalGammaCands_size;#tau_{h} candidates / 1", 40, -0.5, 39.5);
   runPoint(&h_signalGammaCands_size, "signalGammaCands_size", true);

   TH1D h_isolationGammaCands_size("h_isolationGammaCands_size", ";isolationGammaCands_size;#tau_{h} candidates / 1", 150, -0.5, 149.5);
   runPoint(&h_isolationGammaCands_size, "isolationGammaCands_size", true);

   TH1D h_sigCands_dr("h_sigCands_dr", ";sigCands_dr;#tau_{h} candidates / 0.02", 20, -0.1, 0.3);
   runPoint(&h_sigCands_dr, "signalGammaCands_size? sigCands_dr : -0.05", false);

   TH1D h_sigCands_deta("h_sigCands_deta", ";sigCands_deta;#tau_{h} candidates / 0.02", 20, -0.1, 0.3);
   runPoint(&h_sigCands_deta, "signalGammaCands_size? sigCands_deta : -0.05", false);

   TH1D h_sigCands_dphi("h_sigCands_dphi", ";sigCands_dphi;#tau_{h} candidates / 0.02", 20, -0.1, 0.3);
   runPoint(&h_sigCands_dphi, "signalGammaCands_size? sigCands_dphi : -0.05", false);

   TH1D h_isoCands_dr("h_isoCands_dr", ";isoCands_dr;#tau_{h} candidates / 0.028", 25, -0.1, 0.6);
   runPoint(&h_isoCands_dr, "isolationGammaCands_size? isoCands_dr : -0.05", false);

   TH1D h_isoCands_deta("h_isoCands_deta", ";isoCands_deta;#tau_{h} candidates / 0.028", 25, -0.1, 0.6);
   runPoint(&h_isoCands_deta, "isolationGammaCands_size? isoCands_deta : -0.05", false);

   TH1D h_isoCands_dphi("h_isoCands_dphi", ";isoCands_dphi;#tau_{h} candidates / 0.0285", 25, -0.1, 0.6);
   runPoint(&h_isoCands_dphi, "isolationGammaCands_size? isoCands_dphi : -0.05", false);

   TH1D h_leadingTrackNormChi2("h_leadingTrackNormChi2", ";leadingTrackNormChi2;", 40, 0., 100.);
   runPoint(&h_leadingTrackNormChi2, "leadingTrackNormChi2", false);

   TH1D h_eRatio("h_eRatio", ";ecalEnergy/(ecalEnergy+hcalEnergy);", 40, 0., 1.);
   runPoint(&h_eRatio, "ecalEnergy/(ecalEnergy+hcalEnergy)", false);
}

