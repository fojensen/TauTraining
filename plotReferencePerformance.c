#include <TCut.h>
#include <TCanvas.h>
#include <iostream>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TChain.h>
#include <TLegend.h>

void plotEff(const TString var, const TString tagger)
{

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
   c_sig->Add("./outputData/skim_GluGluHToTauTau_PU200.root");

   TChain *c_bkg =  new TChain("skimmedTree");
   c_bkg->Add("./outputData/skim_QCD_Flat_Pt-15to7000_PU200.root");

   double x[nwp], y[nwp];
   double xerr[nwp], yerr[nwp];

   for (int i = 0; i < nwp; ++i) {

      char cut[100];
      if (tagger=="run2017v2") {
         sprintf(cut, "iso_run2017v2[%d]>0.5", i);
      } else if (tagger=="deepTau2017v2p1") {
         sprintf(cut, "iso_deepTau2017v2p1[%d]>0.5", i);
      }

      const double num_sig = c_sig->GetEntries(TCut(cut));
      const double denom_sig = c_sig->GetEntries();

      const double num_bkg = c_bkg->GetEntries(TCut(cut));
      const double denom_bkg = c_bkg->GetEntries();

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

   TCanvas * c = new TCanvas("c", "", 400, 400);

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

   TLegend * l = new TLegend(0.25, 0.65, 0.6, 0.875);
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

