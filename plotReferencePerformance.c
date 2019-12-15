#include <TPaveText.h>
#include <TH1D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <iostream>
#include <TAxis.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TChain.h>
#include <TLegend.h>

void plotEff(const TString var, const TString tagger)
{
   TChain * c_sig = new TChain("skimmedTree");
   c_sig->Add("./outputData/skim_WToLNu_2J.root");
   c_sig->Add("./outputData/skim_DYToLL-M-50_2J.root");
   c_sig->Add("./outputData/skim_TTTo2L2Nu.root");
   c_sig->Add("./outputData/skim_GluGluHToTauTau.root");
   c_sig->Add("./outputData/skim_VBFHToTauTau.root");
   const TCut sigcut = "drmin_tau_tau<0.4";

   TChain * c_bkg = new TChain("skimmedTree");
   c_bkg->Add("./outputData/skim_QCD_Flat_Pt-15to7000.root");
   const TCut bkgcut = "drmin_tau_tau>=0.4";

   TCut wp90, wp40;
   if (tagger=="run2017v2") {
      wp90 = "iso_run2017v2[1]>0.5";
      wp40 = "iso_run2017v2[5]>0.5";
   } else if (tagger=="deepTau2017v2p1") {
      wp90 = "iso_deepTau2017v2p1[1]>0.5";
      wp40 = "iso_deepTau2017v2p1[5]>0.5";
   } else {
      return;
   }

   const TString tag = "_"+var+"_"+tagger;

   TH1D * h;
   TString var_;
   if (var=="pt") {
      h = new TH1D("h"+tag, ";p_{T} [GeV];#tau_{h} candidates / bin", 20, 20., 220.);
      var_ = "pt";
   } else if (var=="eta") {
      h = new TH1D("h"+tag, ";|#eta|;#tau_{h} candidates / 0.25", 12, 0., 3.);
      var_ = "TMath::Abs(eta)";
   } else {
      return;
   }

   TH1D * h_sig_num90 = (TH1D*)h->Clone("h_sig_num90"+tag);
   TH1D * h_sig_num40 = (TH1D*)h->Clone("h_sig_num40"+tag);
   TH1D * h_sig_denom = (TH1D*)h->Clone("h_sig_denom"+tag);
   TH1D * h_bkg_num90 = (TH1D*)h->Clone("h_bkg_num90"+tag);
   TH1D * h_bkg_num40 = (TH1D*)h->Clone("h_bkg_num40"+tag);
   TH1D * h_bkg_denom = (TH1D*)h->Clone("h_bkg_denom"+tag);

   c_sig->Project(h_sig_denom->GetName(), var_, sigcut);
   c_sig->Project(h_sig_num90->GetName(), var_, sigcut && wp90);
   c_sig->Project(h_sig_num40->GetName(), var_, sigcut && wp40);

   c_bkg->Project(h_bkg_denom->GetName(), var_, bkgcut);
   c_bkg->Project(h_bkg_num90->GetName(), var_, bkgcut && wp90);
   c_bkg->Project(h_bkg_num40->GetName(), var_, bkgcut && wp40);

   TGraphAsymmErrors * g_sig90 = new TGraphAsymmErrors(h_sig_num90, h_sig_denom);
   TGraphAsymmErrors * g_sig40 = new TGraphAsymmErrors(h_sig_num40, h_sig_denom);
   TGraphAsymmErrors * g_bkg90 = new TGraphAsymmErrors(h_bkg_num90, h_bkg_denom);
   TGraphAsymmErrors * g_bkg40 = new TGraphAsymmErrors(h_bkg_num40, h_bkg_denom);

   g_sig90->SetMarkerStyle(20);
   g_sig90->SetMarkerColor(6);
   g_sig90->SetLineColor(6);

   g_sig40->SetMarkerStyle(20);
   g_sig40->SetMarkerColor(7);
   g_sig40->SetLineColor(7);

   g_bkg90->SetMarkerStyle(20);
   g_bkg90->SetMarkerColor(8);
   g_bkg90->SetLineColor(8);

   g_bkg40->SetMarkerStyle(20);
   g_bkg40->SetMarkerColor(9);
   g_bkg40->SetLineColor(9);

   TCanvas * canvas = new TCanvas("canvas_"+tag, "plotEff", 400, 400);
   g_sig90->Draw("APE");
   g_sig90->SetMinimum(0.0001);
   g_sig90->SetMaximum(1.);
   char buffertitle[100];
   sprintf(buffertitle, ";%s;tagging efficiency", h->GetXaxis()->GetTitle());
   g_sig90->SetTitle(buffertitle);
   canvas->SetLogy();
   g_sig40->Draw("PE, SAME");
   g_bkg40->Draw("PE, SAME");
   g_bkg90->Draw("PE, SAME");

   TLegend * l = new TLegend(0.225, 0.175, 0.825, 0.3);
   l->SetNColumns(4);
   l->SetBorderSize(0);
   l->AddEntry(g_sig40, "sig loose", "P");
   l->AddEntry(g_sig90, "sig tight", "P");
   l->AddEntry(g_bkg40, "bkg tight", "P");
   l->AddEntry(g_bkg90, "bkg loose", "P");
   l->Draw();

   TPaveText *pt = new TPaveText(.7, .9, .9, .975, "NDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->AddText(tagger);
   pt->Draw();

   char buffer[100];
   sprintf(buffer, "./plots/refperf.%s.%s.pdf", tagger.Data(), var.Data());
   canvas->SaveAs(buffer);
}

TGraphErrors * runPointROC(const TString tagger)
{
   std::cout << "Running " << tagger << std::endl;
   int nwp_ = 1;
   if (tagger=="run2017v2") {
      nwp_ = 8;
   } else if (tagger=="deepTau2017v2p1") {
      nwp_ = 9;
   }
   const int nwp = nwp_;

   TChain *c_sig = new TChain("skimmedTree");
   c_sig->Add("./outputData/skim_WToLNu_2J.root");
   c_sig->Add("./outputData/skim_DYToLL-M-50_2J.root");
   c_sig->Add("./outputData/skim_TTTo2L2Nu.root");
   c_sig->Add("./outputData/skim_GluGluHToTauTau.root");
   c_sig->Add("./outputData/skim_VBFHToTauTau.root");
   const TCut sigcut = "drmin_tau_tau<0.4";

   TChain *c_bkg =  new TChain("skimmedTree");
   c_bkg->Add("./outputData/skim_QCD_Flat_Pt-15to7000.root");
   const TCut bkgcut = "drmin_tau_tau>=0.4";

   double x[nwp], y[nwp];
   double xerr[nwp], yerr[nwp];

   for (int i = 0; i < nwp; ++i) {

      char cut[100];
      if (tagger=="run2017v2") {
         sprintf(cut, "iso_run2017v2[%d]>0.5", i);
      } else if (tagger=="deepTau2017v2p1") {
         sprintf(cut, "iso_deepTau2017v2p1[%d]>0.5", i);
      }

      const double num_sig = c_sig->GetEntries(TCut(cut) && sigcut);
      const double denom_sig = c_sig->GetEntries(sigcut);

      const double num_bkg = c_bkg->GetEntries(TCut(cut) && bkgcut);
      const double denom_bkg = c_bkg->GetEntries(bkgcut);

      x[i] = num_sig/denom_sig;
      xerr[i] = x[i]*(1.-x[i])/denom_sig;
      xerr[i] = sqrt(xerr[i]);
      y[i] = num_bkg/denom_bkg;
      yerr[i] = y[i]*(1.-y[i])/denom_bkg;
      yerr[i] = sqrt(yerr[i]);
      
      std::cout << "wp " << i << std::endl;
      std::cout << "   x: " << x[i] << " +- " << xerr[i] << std::endl;
      std::cout << "   y: " << y[i] << " +- " << yerr[i] << std::endl;
   }

   TGraphErrors * g = new TGraphErrors(nwp, x, y, xerr, yerr);
   char title[100];
   sprintf(title, "g_%s", tagger.Data());
   g->SetName(title);
   g->SetTitle(";signal efficiency;background efficiency");
  
   return g; 
}

void plotROC()
{
   TGraphErrors * g_run2017v2 = runPointROC("run2017v2");
   TGraphErrors * g_deepTau2017v2p1 = runPointROC("deepTau2017v2p1");
   
   TCanvas * c = new TCanvas("c", "plotROC", 400, 400);

   g_run2017v2->SetMarkerStyle(20);
   g_run2017v2->SetMarkerColor(6);
   g_run2017v2->SetLineColor(6);
   g_run2017v2->Draw("APE");
   g_run2017v2->SetMinimum(1.e-4);
   g_run2017v2->SetMaximum(1.);
   g_run2017v2->GetXaxis()->SetRangeUser(0., 1.);
   g_run2017v2->Draw("APE");
   c->Update();

   g_deepTau2017v2p1->SetMarkerStyle(20);
   g_deepTau2017v2p1->SetMarkerColor(7);
   g_deepTau2017v2p1->SetLineColor(7);
   g_deepTau2017v2p1->Draw("PE, SAME");

   TLegend *l = new TLegend(0.25, 0.7, 0.6, 0.875);
   l->SetBorderSize(0);
   l->AddEntry(g_run2017v2, "run2017v2", "P");
   l->AddEntry(g_deepTau2017v2p1, "deepTau2017v2p1", "P");
   l->Draw();
   
   c->SetLogy();

   c->SaveAs("./plots/referenceroc.pdf");
}

void plotReferencePerformance()
{
   plotROC();
   plotEff("pt", "run2017v2");
   plotEff("eta", "run2017v2");
   plotEff("pt", "deepTau2017v2p1");
   plotEff("eta", "deepTau2017v2p1");
}

