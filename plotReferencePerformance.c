#include <TFile.h>
#include <TPaveText.h>
#include <TH1D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <iostream>
#include <TAxis.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TTree.h>

TGraphAsymmErrors * runPointEff(TTree * tree, TH1D * h, const TString var, const TString tag, const TCut cut_baseline)
{
   std::cout << "runPointEff()" << std::endl;

   TH1D * h_num = (TH1D*)h->Clone("h_num_"+tag);
   TH1D * h_denom = (TH1D*)h->Clone("h_denom_"+tag);

   const double n_denom = tree->Project(h_denom->GetName(), var, cut_baseline);
   const double n_num = tree->Project(h_num->GetName(), var, cut_baseline && TCut("iso_run2017v2[3]>0.5"));
   
   std::cout << "inclusive efficiency: " << n_num/n_denom << std::endl;

   TGraphAsymmErrors * g = new TGraphAsymmErrors(h_num, h_denom);
   g->SetName("g_"+tag);
   char title[100];
   sprintf(title, ";%s;efficiency", var.Data());
   g->SetTitle(title);
   return g;
}

void plotEff(TH1D * h, const TString tauVar, const TCut tauCut, const TString savetag)
{
   std::cout << "plotEff()" << std::endl;

   const TCut cutSig = tauCut && TCut("drmin_tau_tau<0.4");
   const TCut cutBkg = tauCut && TCut("drmin_tau_tau>=0.4");

   TFile * f_sig = TFile::Open("./outputData/skim_VBFHToTauTau.root");
   TTree * t_sig = (TTree*)f_sig->Get("skimmedTree");
   TGraphAsymmErrors * g_sig = runPointEff(t_sig, h, tauVar, "sig_Phase2", cutSig);

   TFile * f_bkg = TFile::Open("./outputData/skim_QCD_Flat.root");
   TTree * t_bkg = (TTree*)f_bkg->Get("skimmedTree");
   TGraphAsymmErrors * g_bkg = runPointEff(t_bkg, h, tauVar, "bkg_Phase2", cutBkg);

   TFile * f_sig_Run2 = TFile::Open("./outputData/skim_VBFHToTauTau_Run2.root");
   TTree * t_sig_Run2 = (TTree*)f_sig_Run2->Get("skimmedTree");
   TGraphAsymmErrors * g_sig_Run2 = runPointEff(t_sig_Run2, h, tauVar, "sig_Run2", cutSig);

   TFile * f_bkg_Run2 = TFile::Open("./outputData/skim_QCD_Flat_Run2.root");
   TTree * t_bkg_Run2 = (TTree*)f_bkg_Run2->Get("skimmedTree");
   TGraphAsymmErrors * g_bkg_Run2 = runPointEff(t_bkg_Run2, h, tauVar, "bkg_Run2", cutBkg);

   TCanvas * canvas = new TCanvas("c_"+savetag, savetag, 400, 400);
   g_sig->SetMarkerStyle(20);
   g_sig->SetLineColor(3);
   g_sig->SetMarkerColor(3);
   g_sig->Draw("APE");

   g_bkg->SetMarkerStyle(20);
   g_bkg->SetLineColor(4);
   g_bkg->SetMarkerColor(4);
   g_bkg->Draw("PE, SAME");

   g_sig_Run2->SetMarkerStyle(20);
   g_sig_Run2->SetLineColor(6);
   g_sig_Run2->SetMarkerColor(6);
   g_sig_Run2->Draw("PE, SAME");

   g_bkg_Run2->SetMarkerStyle(20);
   g_bkg_Run2->SetLineColor(7);
   g_bkg_Run2->SetMarkerColor(7);
   g_bkg_Run2->Draw("PE, SAME");

   TLegend * l = new TLegend(0.25, 0.2, 0.7, 0.3);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   l->AddEntry(g_sig, "VBF H; Phase2", "P");
   l->AddEntry(g_bkg, "QCD Flat; Phase2", "P");
   l->AddEntry(g_sig_Run2, "VBF H; Run2", "P");
   l->AddEntry(g_bkg_Run2, "QCD Flat; Run2", "P");
   l->Draw();

   g_sig->SetMaximum(1.);
   g_sig->SetMinimum(0.0001);
   canvas->SetLogy();
  
   l->Draw();

   canvas->SaveAs("./plots/"+savetag+".pdf");

}

TGraphErrors * runPointROC(TTree * sig, TTree * bkg,  const TCut cut_baseline, const TString tag)
{
   std::cout << "runPointROC()" << std::endl;

   TCut sigcut = "drmin_tau_tau<0.4";
   sigcut = sigcut && cut_baseline;

   TCut bkgcut = "drmin_tau_tau>=0.4";
   bkgcut = bkgcut && cut_baseline;

   const int nwp = 8;
   double x[nwp], y[nwp];
   double xerr[nwp], yerr[nwp];

   for (int i = 0; i < nwp; ++i) {
      char cut[100];
      sprintf(cut, "iso_run2017v2[%d]>0.5", i);

      const double num_sig = sig->GetEntries(TCut(cut) && sigcut);
      const double denom_sig = sig->GetEntries(sigcut);

      const double num_bkg = bkg->GetEntries(TCut(cut) && bkgcut);
      const double denom_bkg = bkg->GetEntries(bkgcut);

      if (denom_sig>0.) {
         x[i] = num_sig/denom_sig;
         xerr[i] = x[i]*(1.-x[i])/denom_sig;
         xerr[i] = sqrt(xerr[i]);
      } else {
         x[i] = 0.;
         xerr[i] = 0.;
      }

      if (denom_bkg>0.) {
         y[i] = num_bkg/denom_bkg;
         yerr[i] = y[i]*(1.-y[i])/denom_bkg;
         yerr[i] = sqrt(yerr[i]);
      } else {
         y[i] = 0.;
         yerr[i] = 0.;
      }   
   }

   for (int i = 0; i < nwp; ++i) {
      std::cout << "wp " << i << " xeff: " << x[i] << " yeff: " << y[i] << std::endl;
   }

   TGraphErrors * g = new TGraphErrors(nwp, x, y, xerr, yerr);
   char title[100];
   sprintf(title, "g_%s", tag.Data());
   g->SetName(title);
   g->SetTitle(";signal efficiency;background efficiency");
  
   return g; 
}

void plotROC(const TCut cut_baseline, const TString tag)
{
   std::cout << "plotROC()" << std::endl;

   TFile * f_sigRun2 = TFile::Open("./outputData/skim_VBFHToTauTau_Run2.root");
   TTree * t_sigRun2 = (TTree*)f_sigRun2->Get("skimmedTree");
   TFile * f_bkgRun2 = TFile::Open("./outputData/skim_QCD_Flat_Run2.root");
   TTree * t_bkgRun2 = (TTree*)f_bkgRun2->Get("skimmedTree");
   TGraphErrors * g_roc_Run2 = runPointROC(t_sigRun2, t_bkgRun2, cut_baseline, "Run2");

   TFile * f_sigPhase2 = TFile::Open("./outputData/skim_VBFHToTauTau.root");
   TTree * t_sigPhase2 = (TTree*)f_sigPhase2->Get("skimmedTree");
   TFile * f_bkgPhase2 = TFile::Open("./outputData/skim_QCD_Flat.root");
   TTree * t_bkgPhase2 = (TTree*)f_bkgPhase2->Get("skimmedTree");
   TGraphErrors * g_roc_Phase2 = runPointROC(t_sigPhase2, t_bkgPhase2, cut_baseline, "Phase2");

   TCanvas * c = new TCanvas("c_"+tag, "plotROC_"+tag, 400, 400);

   g_roc_Phase2->SetMarkerStyle(20);
   g_roc_Phase2->SetMarkerColor(6);
   g_roc_Phase2->SetLineColor(6);
   g_roc_Phase2->Draw("APE");
   g_roc_Phase2->SetMinimum(1.e-4);
   g_roc_Phase2->SetMaximum(1.);
   g_roc_Phase2->GetXaxis()->SetRangeUser(0., 1.);
   g_roc_Phase2->Draw("APE");
   c->Update();

   g_roc_Run2->SetMarkerStyle(20);
   g_roc_Run2->SetMarkerColor(8);
   g_roc_Run2->SetLineColor(8);
   g_roc_Run2->Draw("PE, SAME");

   TLegend *l = new TLegend(0.25, 0.7, 0.6, 0.875);
   l->SetBorderSize(0);
   l->AddEntry(g_roc_Phase2, "Phase 2 MC", "P");
   l->AddEntry(g_roc_Run2, "Run 2 MC", "P");
   l->Draw();
   
   c->SetLogy();

   c->SaveAs("./plots/referenceroc."+tag+".pdf");
}

void plotReferencePerformance()
{
   const TCut loweta = "TMath::Abs(eta)<2.3";
   const TCut higheta = "TMath::Abs(eta)>=2.3 && TMath::Abs(eta)<3.";
   const TCut lowpt = "pt>=20. && pt<50.";
   const TCut highpt = "pt>=50. && pt<220.";
   const TCut baseline = "decayModeFinding>0.5 && TMath::Abs(eta)<3. && pt>=20. && pt<220. && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";

   plotROC(loweta && lowpt && baseline, "centraleta_lowpt");
   plotROC(loweta && highpt && baseline, "centraleta_highpt");
   plotROC(higheta && lowpt && baseline, "forwardeta_lowpt");
   plotROC(higheta && highpt && baseline, "forwardeta_highpt");
 
   TH1D * h_pt = new TH1D("h_pt", ";p_{T} [GeV];", 10, 20., 220.);
   plotEff(h_pt, "pt", loweta && baseline, "pt_loweta");
   plotEff(h_pt, "pt", higheta && baseline, "pt_higheta");

   TH1D * h_eta = new TH1D("h_eta", ";|#eta|;", 12, 0., 3.);
   plotEff(h_eta, "TMath::Abs(eta)", lowpt && baseline, "eta_lowpt");
   plotEff(h_eta, "TMath::Abs(eta)", highpt && baseline, "eta_highpt");
}

