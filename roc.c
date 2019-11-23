//#include <TLine.h>
#include <TCanvas.h>
#include <iostream>
#include <TAxis.h>
#include <TCut.h>
#include <TGraphErrors.h>
#include <TChain.h>
#include <TLegend.h>

TGraphErrors * runPoint(const TString bkgfile, const TString tagger)
{
   int nwp_ = 1;
   if (tagger=="run2017v2") {
      nwp_ = 8;
   } else if (tagger=="deepTau2017v2p1") {
      nwp_ = 9;
   }
   const int nwp = nwp_;

   TChain *c_sig = new TChain("tauAnalyzer/tree");
   c_sig->Add("./mcsamples/GluGluHToTauTau_PU200.root");

   TChain *c_bkg =  new TChain("tauAnalyzer/tree");
   c_bkg->Add(bkgfile);

   double x[nwp], y[nwp];
   double xerr[nwp], yerr[nwp];

   const TCut cut_reco = "pt>=20. && pt<220. && std::abs(eta)<3. && decayModeFinding>0.5";
   const TCut cut_gen = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   const TCut cut_baseline = cut_reco&&cut_gen;
   const TCut cut_sig = "drmin_tau_tau<0.4";

   for (int i = 0; i < nwp; ++i) {

      char cut[100];
      if (tagger=="run2017v2") {
         sprintf(cut, "iso_run2017v2[%d]>0.5", i);
      } else if (tagger=="deepTau2017v2p1") {
         sprintf(cut, "iso_deepTau2017v2p1[%d]>0.5", i);
      }
      const TCut cut_iso = TCut(cut);

      const TCut cut_num_sig = cut_iso && cut_sig && cut_baseline;
      const double num_sig = c_sig->GetEntries(cut_num_sig);

      const TCut cut_denom_sig = cut_sig && cut_baseline;
      const double denom_sig = c_sig->GetEntries(cut_denom_sig);

      TCut cut_num_bkg = cut_iso && cut_baseline;
      const double num_bkg = c_bkg->GetEntries(cut_num_bkg);

      TCut cut_denom_bkg = cut_baseline;
      const double denom_bkg = c_bkg->GetEntries(cut_denom_bkg);

      x[i] = num_sig/denom_sig;
      xerr[i] = x[i]*(1.-x[i])/denom_sig;
      xerr[i] = sqrt(xerr[i]);
      y[i] = num_bkg/denom_bkg;
      yerr[i] = y[i]*(1.-y[i])/denom_bkg;
      yerr[i] = sqrt(yerr[i]);
      
      std::cout << "x: " << x[i] << " +- " << xerr[i] << std::endl;
      std::cout << "y: " << y[i] << " +- " << yerr[i] << std::endl;
   }

   TGraphErrors * g = new TGraphErrors(nwp, x, y, xerr, yerr);
   char title[100];
   sprintf(title, "g_%s", tagger.Data());
   g->SetName(title);
   g->SetTitle(";signal efficiency;background efficiency");
  
   return g; 
}

void roc()
{
   TGraphErrors * g_run2017v2 = runPoint("./mcsamples/QCD_PU200.root", "run2017v2");
   TGraphErrors * g_deepTau2017v2p1 = runPoint("./mcsamples/QCD_PU200.root", "deepTau2017v2p1");

   TCanvas * c = new TCanvas("c", "", 400, 400);

   g_run2017v2->SetMarkerStyle(20);
   g_run2017v2->SetMarkerColor(6);
   g_run2017v2->Draw("APE");
   g_run2017v2->SetMinimum(1.e-4);
   g_run2017v2->SetMaximum(1.);
   g_run2017v2->GetXaxis()->SetRangeUser(0., 1.);
   g_run2017v2->Draw("APE");
   c->Update();

   g_deepTau2017v2p1->SetMarkerStyle(20);
   g_deepTau2017v2p1->SetMarkerColor(6);
   g_deepTau2017v2p1->SetLineColor(6);
   g_deepTau2017v2p1->Draw("PE, SAME");

   TLegend * l = new TLegend(0.2, 0.6, 0.6, 0.875);
   l->SetBorderSize(0);
   l->AddEntry(g_run2017v2, "run2017v2", "P");
   l->AddEntry(g_deepTau2017v2p1, "deepTau2017v2p1", "P");
   l->Draw();
   
   c->SetLogy();
}

