#include <iostream>
#include <TLegend.h>
#include <TH1D.h>
#include <TChain.h>
#include <TCanvas.h>

double addOverflow(TH1D *h)
{
   const int n = h->GetNbinsX();
   const double y = h->GetBinContent(n+1);
   if (y) {
      std::cout << h->GetName() << " has overflow" << std::endl;
   }
   h->SetBinContent(n+1, 0);
   h->SetBinError(n+1, 0.);
   h->AddBinContent(n, y);
   return y;
}

double addUnderflow(TH1D *h)
{
   const double y = h->GetBinContent(0);
   if (y) {
      std::cout << h->GetName() << " has underflow" << std::endl;
   }
   h->SetBinContent(0, 0);
   h->SetBinError(0, 0.);
   h->AddBinContent(1, y);
   return y;
}

void runPoint(TH1D * h, const TString var, const bool dolog=false)
{
   TH1D * h_sig = (TH1D*)h->Clone("h_sig_"+var);
   TH1D * h_bkg = (TH1D*)h->Clone("h_bkg_"+var);

   TChain c_sig("skimmedTree");
   c_sig.Add("./mcsamples/skim_GluGluHToTauTau_PU200.root");

   TChain c_bkg("skimmedTree");
   c_bkg.Add("./mcsamples/skim_QCD_Flat_Pt-15to7000_PU200.root");
 
   const int n_sig = c_sig.Project(h_sig->GetName(), var);
   const int n_bkg = c_bkg.Project(h_bkg->GetName(), var);

   addOverflow(h_sig);
   addOverflow(h_bkg);
   addUnderflow(h_sig);
   addUnderflow(h_bkg);

   h_sig->Scale(1./n_sig);
   h_bkg->Scale(1./n_bkg);
   
   h_sig->SetLineWidth(2);
   h_sig->SetLineColor(4);
   h_sig->SetStats(0);
   h_bkg->SetLineWidth(2);
   h_bkg->SetLineColor(2);

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

void makeVarPlots()
{
   TH1D h_chargedIsoPtSum("h_chargedIsoPtSum", ";chargedIsoPtSum [GeV];# of #tau_{h} / 1 GeV", 40, 0., 40.);
   runPoint(&h_chargedIsoPtSum, "chargedIsoPtSum", true);

   TH1D h_neutralIsoPtSum("h_neutralIsoPtSum", ";neutralIsoPtSum [GeV];# of #tau_{h} / 1 GeV", 40, 0., 40.);
   runPoint(&h_neutralIsoPtSum, "neutralIsoPtSum", true);

   TH1D h_leadChargedHadrCand_dxy("h_leadChargedHadrCand_dxy", ";leadChargedHadrCand_dxy;# of #tau_{h} / 0.05", 40, -1., 1.);
   runPoint(&h_leadChargedHadrCand_dxy, "leadChargedHadrCand_dxy", false);

   TH1D h_leadChargedHadrCand_dxysig("h_leadChargedHadrCand_dxysig", ";leadChargedHadrCand_dxysig;# of #tau_{h} / 0.05", 40, -1., 1.);
   runPoint(&h_leadChargedHadrCand_dxysig, "leadChargedHadrCand_dxysig", false);

   TH1D h_decayMode("h_decayMode", ";decayMode;# of #tau_{h} / bin",  11, -0.5, 10.5);
   runPoint(&h_decayMode, "decayMode", false);

   TH1D h_hasSecondaryVertex("h_hasSecondaryVertex", ";hasSecondaryVertex;# of #tau_{h} / bin", 2, -0.5, 1.5);
   runPoint(&h_hasSecondaryVertex, "hasSecondaryVertex", false);

   TH1D h_flightLength("h_flightLength", ";flightLength [cm];# of #tau_{h} / 0.075", 40, -1., 3.);
   runPoint(&h_flightLength, "hasSecondaryVertex? flightLength : -2.", true);

   TH1D h_flightLengthSig("h_flightLengthSig", ";flightLengthSig;# of #tau_{h} / ", 40, -5., 10.);
   runPoint(&h_flightLengthSig, "hasSecondaryVertex? flightLengthSig : -2.", true);

   TH1D h_puCorrPtSum("h_puCorrPtSum", ";puCorrPtSum;# of #tau_{h} / 1", 40, 0., 40.);
   runPoint(&h_puCorrPtSum, "puCorrPtSum", true);
 
   TH1D h_pt("h_pt", ";log p_{T};", 40, 20., 220.);
   runPoint(&h_pt, "math::log(pt)", true);

   TH1D h_eta("h_eta", ";|#eta|;", 40, 0., 3.);
   runPoint(&h_eta, "std::abs(eta)", false);
}

