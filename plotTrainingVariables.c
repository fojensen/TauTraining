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

void runPoint(TH1D * h, const TString var, const bool dolog=false)
{
   TH1D * h_sig = (TH1D*)h->Clone(TString(h->GetName())+"_sig");
   TH1D * h_bkg = (TH1D*)h->Clone(TString(h->GetName())+"_bkg");

   TChain c_sig("skimmedTree");
   c_sig.Add("./outputData/skim_GluGluHToTauTau_PU200.root");

   TChain c_bkg("skimmedTree");
   c_bkg.Add("./outputData/skim_QCD_Flat_Pt-15to7000_PU200.root");
 
   const int n_sig = c_sig.Project(h_sig->GetName(), var);
   const int n_bkg = c_bkg.Project(h_bkg->GetName(), var);

   addOverflow(h_sig);
   addOverflow(h_bkg);
   addUnderflow(h_sig);
   addUnderflow(h_bkg);

   h_sig->Scale(1./n_sig);
   h_bkg->Scale(1./n_bkg);
   
   h_sig->SetLineWidth(2);
   h_sig->SetLineColor(8);
   h_sig->SetStats(0);
   h_bkg->SetLineWidth(2);
   h_bkg->SetLineColor(9);

   TCanvas * c = new TCanvas("c_"+var, var, 400, 400);
   h_sig->Draw("HIST, E");
   h_bkg->Draw("HIST, E, SAME");
   
   const double max = TMath::Max(h_sig->GetMaximum(), h_bkg->GetMaximum());
   h_sig->SetMinimum(0.);
   h_sig->SetMaximum(1.1*max);
   if (dolog) {
      h_sig->SetMaximum(1.);
      h_sig->SetMinimum(1.e-4);
      c->SetLogy();
   }

   TLegend * l = new TLegend(0.5, 0.8, 0.875, 0.875);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   l->AddEntry(h_sig, "#tau_{h}", "L");
   l->AddEntry(h_bkg, "jet", "L");
   l->Draw();

   char hname[100];
   sprintf(hname, "./plots/dists_%s.pdf", h->GetName());
   c->SaveAs(hname);
}

void plotTrainingVariables()
{
   TH1D h_chargedIsoPtSum("h_chargedIsoPtSum", ";chargedIsoPtSum;#tau_{h} candidates / 1 GeV", 40, 0., 40.);
   runPoint(&h_chargedIsoPtSum, "chargedIsoPtSum", true);

   TH1D h_neutralIsoPtSum("h_neutralIsoPtSum", ";neutralIsoPtSum;#tau_{h} candidates / 1 GeV", 40, 0., 40.);
   runPoint(&h_neutralIsoPtSum, "neutralIsoPtSum", true);

   TH1D h_decayMode("h_decayMode", ";decayMode;# of #tau_{h} / bin",  11, -0.5, 10.5);
   runPoint(&h_decayMode, "decayMode", false);

   TH1D h_leadChargedHadrCand_dxy("h_leadChargedHadrCand_dxy", ";TMath::Sqrt(TMath::Abs(leadChargedHadrCand_dxy));#tau_{h} candidates / 0.01", 40, 0., 0.2);
   runPoint(&h_leadChargedHadrCand_dxy, "TMath::Sqrt(TMath::Abs(leadChargedHadrCand_dxy))", false);

   TH1D h_leadChargedHadrCand_dxysig("h_leadChargedHadrCand_dxysig", ";TMath::Abs(leadChargedHadrCand_dxysig);#tau_{h} candidates / 0.25", 40, 0., 10.);
   runPoint(&h_leadChargedHadrCand_dxysig, "TMath::Abs(leadChargedHadrCand_dxysig)", false);

   TH1D h_hasSecondaryVertex("h_hasSecondaryVertex", ";hasSecondaryVertex;#tau_{h} candidates / bin", 2, -0.5, 1.5);
   runPoint(&h_hasSecondaryVertex, "hasSecondaryVertex", false);

   TH1D h_flightLength("h_flightLength", ";TMath::Sqrt(TMath::Abs(flightLength));#tau_{h} candidates / 0.0875", 40, -0.5, 3.);
   runPoint(&h_flightLength, "hasSecondaryVertex? TMath::Sqrt(TMath::Abs(flightLength)) : -0.25", true);

   TH1D h_flightLengthSig("h_flightLengthSig", ";TMath::Abs(flightLengthSig);#tau_{h} candidates / 0.2625", 40, -0.5, 10.);
   runPoint(&h_flightLengthSig, "hasSecondaryVertex? TMath::Abs(flightLengthSig): -0.25", true);

   TH1D h_puCorrPtSum("h_puCorrPtSum", ";puCorrPtSum;#tau_{h} candidates / 6.25 GeV", 40, 0., 250.);
   runPoint(&h_puCorrPtSum, "puCorrPtSum", true);
 
   TH1D h_pt("h_pt", ";pt;#tau_{h} candidates / 5.625 GeV", 40, 20., 245.);
   runPoint(&h_pt, "pt", true);

   TH1D h_eta("h_eta", ";TMath::Abs(eta);#tau_{h} candidates / 0.075", 40, 0., 3.);
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

   TH1D h_sigCands_dr("h_sigCands_dr", ";sigCands_dr;#tau_{h} candidates / 0.0325", 40, -0.5, 0.8);
   runPoint(&h_sigCands_dr, "signalGammaCands_size? sigCands_dr : -0.25", false);

   TH1D h_sigCands_deta("h_sigCands_deta", ";sigCands_deta;#tau_{h} candidates / 0.0325", 40, -0.5, 0.8);
   runPoint(&h_sigCands_deta, "signalGammaCands_size? sigCands_deta : -0.25", false);

   TH1D h_sigCands_dphi("h_sigCands_dphi", ";sigCands_dphi;#tau_{h} candidates / 0.0325", 40, -0.5, 0.8);
   runPoint(&h_sigCands_dphi, "signalGammaCands_size? sigCands_dphi : -0.25", false);

   TH1D h_isoCands_dr("h_isoCands_dr", ";isoCands_dr;#tau_{h} candidates / 0.0325", 40, -0.5, 0.8);
   runPoint(&h_isoCands_dr, "isolationGammaCands_size? isoCands_dr : -0.25", false);

   TH1D h_isoCands_deta("h_isoCands_deta", ";isoCands_deta;#tau_{h} candidates / 0.0325", 40, -0.5, 0.8);
   runPoint(&h_isoCands_deta, "isolationGammaCands_size? isoCands_deta : -0.25", false);

   TH1D h_isoCands_dphi("h_isoCands_dphi", ";isoCands_dphi;#tau_{h} candidates / 0.0325", 40, -0.5, 0.8);
   runPoint(&h_isoCands_dphi, "isolationGammaCands_size? isoCands_dphi : -0.25", false);
}

