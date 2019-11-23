#include <TLegend.h>
#include <TChain.h>
#include <TH1D.h>
#include <TCut.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>

void runPoint_eta(const bool isSignal, const TString infile, const TString tag)
{
   const TCut cuts_wp[2] = {"iso_run2017v2[1]>0.5", "iso_run2017v2[7]>0.5"};
   TCut cuts_reco = "decayModeFinding>0.5 && pt>=20. && pt<220.";
   TCut cuts_gen = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   TCut cuts_baseline = cuts_reco && cuts_gen;
   if (isSignal) cuts_baseline = TCut("drmin_tau_tau<0.4");
 
   TH1D * h = new TH1D("h_eta_"+tag, ";|#eta|;", 10, 0., 4.);
   TH1D *h_num[2], *h_denom[2];
   for (int i = 0; i < 2; ++i) {
      const TString tag2 = tag + "_eta_" + TString::Itoa(i, 10);
      h_num[i] = (TH1D*)h->Clone("h_num_"+tag2);
      h_denom[i] = (TH1D*)h->Clone("h_denom_"+tag2);
   }

   TChain * chain = new TChain("tauAnalyzer/tree");
   chain->Add(infile);
   TGraphAsymmErrors * g[2];
   for (int i = 0; i < 2; ++i) {
      const TCut cuts_denom = cuts_baseline;
      const TCut cuts_num = cuts_wp[i] && cuts_baseline;
      chain->Project(h_denom[i]->GetName(), "std::abs(eta)", cuts_denom);
      chain->Project(h_num[i]->GetName(), "std::abs(eta)", cuts_num);
      g[i] = new TGraphAsymmErrors(h_num[i], h_denom[i]);
   }

   g[0]->SetMarkerStyle(20);
   g[0]->SetMarkerColor(8);
   g[0]->SetLineColor(8);

   g[1]->SetMarkerStyle(20);
   g[1]->SetMarkerColor(9);
   g[1]->SetLineColor(9);
   
   TCanvas * canvas = new TCanvas("canvas_eta_"+tag, tag, 400, 400);
   if (isSignal) {
      g[0]->SetTitle(tag+";#eta;efficiency");
   } else {
      g[0]->SetTitle(tag+";#eta;mistag rate");
   }
   g[0]->Draw("APE");
   //if (isSignal) {
   //   g[0][0]->SetMinimum(0.);
    //  g[0][0]->SetMaximum(1.);
   //} else {
      g[0]->SetMinimum(0.0001);
      g[0]->SetMaximum(1.);
   g[1]->Draw("PE, SAME");
      canvas->SetLogy();
  // }
//   }

   TLegend * l = new TLegend(0.25, 0.175, 0.5, 0.3);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   l->AddEntry(g[0], "loose wp", "P");
   l->AddEntry(g[1], "tight wp", "P");
   l->Draw();

   canvas->SaveAs("./plots/effeta."+tag+".pdf");
}


void runPoint_pt(const bool isSignal, const TString infile, const TString tag)
{
   const TCut cuts_wp[2] = {"iso_run2017v2[1]>0.5", "iso_run2017v2[7]>0.5"};
   const TCut cuts_eta[2] = {"std::abs(eta)<1.", "std::abs(eta)>=1. && std::abs(eta)<3."};   
   TCut cuts_reco = "decayModeFinding>0.5 && std::abs(eta)<3.";
   TCut cuts_gen = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   TCut cuts_baseline = cuts_reco && cuts_gen;
   if (isSignal) cuts_baseline = TCut("drmin_tau_tau<0.4");
 
   TH1D * h = new TH1D("h_pt_"+tag, ";p_{T} [GeV];", 10, 0., 300.);
   TH1D *h_num[2][2], *h_denom[2][2];
   for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
         const TString tag2 = tag + "_pt_" + TString::Itoa(i, 10) + TString::Itoa(j, 10);
         h_num[i][j] = (TH1D*)h->Clone("h_num_"+tag2);
         h_denom[i][j] = (TH1D*)h->Clone("h_denom_"+tag2);
      }
   }

   TChain * chain = new TChain("tauAnalyzer/tree");
   chain->Add(infile);
   TGraphAsymmErrors * g[2][2];
   for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
         const TCut cuts_denom = cuts_baseline && cuts_eta[j];
         const TCut cuts_num = cuts_denom && cuts_wp[i];    
         chain->Project(h_denom[i][j]->GetName(), "pt", cuts_denom);
         chain->Project(h_num[i][j]->GetName(), "pt", cuts_num);
         g[i][j] = new TGraphAsymmErrors(h_num[i][j], h_denom[i][j]);
      }
   }

   g[0][0]->SetMarkerStyle(33);
   g[0][0]->SetMarkerColor(6);
   g[0][0]->SetLineColor(6);

   g[0][1]->SetMarkerStyle(33);
   g[0][1]->SetMarkerColor(7);
   g[0][1]->SetLineColor(7);

//   g[0][2]->SetMarkerStyle(33);
  // g[0][2]->SetMarkerColor(8);
  // g[0][2]->SetLineColor(8);

   g[1][0]->SetMarkerStyle(34);
   g[1][0]->SetMarkerColor(6);
   g[1][0]->SetLineColor(6);
   
   g[1][1]->SetMarkerStyle(34);
   g[1][1]->SetMarkerColor(7);
   g[1][1]->SetLineColor(7);

//   g[1][2]->SetMarkerStyle(34);
 //  g[1][2]->SetMarkerColor(8);
 //  g[1][2]->SetLineColor(8);
   
   TCanvas * canvas = new TCanvas("canvas_pt_"+tag, tag, 400, 400);
   //if (isSignal) {
   //   g[0][0]->SetTitle(tag+";p_{T} [GeV];efficiency");
   //} else {
   //   g[0][0]->SetTitle(tag+";p_{T} [GeV];efficiency");
   //}
   g[0][0]->SetTitle(tag+";signal efficiency;background efficiency");
   g[0][0]->Draw("APE");
   //if (isSignal) {
   //   g[0][0]->SetMinimum(0.);
    //  g[0][0]->SetMaximum(1.);
   //} else {
      g[0][0]->SetMinimum(0.0001);
      g[0][0]->SetMaximum(1.);
      canvas->SetLogy();
  // }
   for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
         if (i==0&&j==0) continue;
         g[i][j]->Draw("PE, SAME");
      }
   }

   TLegend * l1 = new TLegend(0.25, 0.175, 0.8, 0.3);
   l1->SetBorderSize(0);
   l1->SetNColumns(2);
   l1->AddEntry(g[0][0], "loose, |#eta|<1", "P");
   l1->AddEntry(g[0][1], "loose, 1.<|#eta|<3.", "P");
 //  l1->AddEntry(g[0][2], "loose, |#eta|>2.5", "P");
   l1->AddEntry(g[1][0], "tight, |#eta|<1.", "P");
   l1->AddEntry(g[1][1], "tight, 1.<|#eta|<3.", "P");
// /  l1->AddEntry(g[1][2], "tight, |#eta|>2.", "P");
   l1->Draw();

   canvas->SaveAs("./plots/effpt."+tag+".pdf");
}

void eff1d()
{
   runPoint_pt(true, "./mcsamples/GluGluHToTauTau_PU200.root", "HPU200");
   runPoint_eta(true, "./mcsamples/GluGluHToTauTau_PU200.root", "HPU200");

   //runPoint_pt(true, "./mcsamples/GluGluHToTauTau_noPU.root", "HPU0");
   //runPoint_eta(true, "./mcsamples/GluGluHToTauTau_noPU.root", "HPU0");
   
   //runPoint_pt(false, "./mcsamples/QCD_Flat_Pt-15to7000_noPU.root", "QCDPU0");
   //runPoint_eta(false, "./mcsamples/QCD_Flat_Pt-15to7000_noPU.root", "QCDPU0");
   
   runPoint_pt(false, "./mcsamples/QCD_PU200.root", "QCDPU200");
   runPoint_eta(false, "./mcsamples/QCD_PU200.root", "QCDPU200");
}

