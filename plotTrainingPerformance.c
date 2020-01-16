#include <TPaveText.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <iostream>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TChain.h>

/*TGraphErrors * runPointROC(const TString tagger)
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
//   c_sig->Add("./outputData/skim_TTTo2L2Nu.root");
   c_sig->Add("./outputData/skim_GluGluHToTauTau.root");
   c_sig->Add("./outputData/skim_VBFHToTauTau.root");
   const TCut sigcut = "drmin_tau_tau<0.4";

   TChain *c_bkg =  new TChain("skimmedTree");
   c_bkg->Add("./outputData/skim_QCD_Flat.root");
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
*/
TGraphErrors * plotROC(const TCut cut, const TCut tag)
{
   TFile * f = TFile::Open("./TMVA.root");
   TTree * tree = (TTree*)f->Get("dataset/TestTree");
   const TCut sigcut = "classID==0";
   const TCut bkgcut = "classID==1";
   
   TH1D * h_sig = new TH1D("h_sig", "", 100, -1., 1.);
   TH1D * h_bkg = (TH1D*)h_sig->Clone("h_bkg");
   const double n_sig = tree->Project("h_sig", "BDT", cut && sigcut);
   const double n_bkg = tree->Project("h_bkg", "BDT", cut && bkgcut);

   TGraphErrors * g_roc = new TGraphErrors(100);
   for (int i = 0; i < 100; ++i) {
      const double eff_sig = h_sig->Integral(i, 101)/n_sig;
      const double eff_bkg = h_bkg->Integral(i, 101)/n_bkg;
      g_roc->SetPoint(i, eff_sig, eff_bkg);
      g_roc->SetPointError(i, 0., 0.);
   }
   g_roc->SetName("");
   g_roc->SetTitle(";signal efficiency;background efficiency");

   //TCanvas * c = new TCanvas("c_"+tag, tag, 400, 400);
   g_roc->SetMarkerStyle(20);
   //g_roc->Draw("APE");
   //g_roc->GetXaxis()->SetRangeUser(0., 1.);
   //g_roc->Draw("APE");
   //c->Update();   
   //g_roc->SetMaximum(1.);
   //g_roc->SetMinimum(1e-4);
   //c->SetLogy();
   return g_roc;
}

TCanvas * plotROCInclusive()
{
   TFile * f = TFile::Open("./TMVA.root");
   TH1D* h_test = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_effBvsS");
   TH1D* h_train = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_trainingEffBvsS");

   h_test->SetLineWidth(2);
   h_test->SetLineColor(6);
   h_test->SetMarkerColor(6);
   
   h_train->SetLineWidth(2);
   h_train->SetLineColor(7);
   h_train->SetMarkerColor(7);
   
   TCanvas * c = new TCanvas("c_plotROC", "plotROC", 400, 400);

   h_test->SetStats(0);
   h_test->Draw("");
   h_test->SetTitle(";signal efficiency;background efficiency");
   h_train->Draw("SAME");

   c->SetLogy();
   h_test->SetMinimum(1.e-4);
   h_test->SetMaximum(1.);

   TLegend *l = new TLegend(0.25, 0.7, 0.6, 0.875);
   l->AddEntry(h_test, "test", "L");
   l->AddEntry(h_train, "train", "L");
   l->SetBorderSize(0);
   l->Draw();

   TH1D *h_auc = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_rejBvsS");
   const float auc = h_auc->Integral("width");
   TPaveText *pt = new TPaveText(.8, .9, .925, .975, "NDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   char buffer2[100];
   sprintf(buffer2, "auc: %.3f", auc);
   pt->AddText(buffer2);
   pt->Draw();

   /*TGraphErrors * g_run2017v2 = runPointROC("run2017v2");
   TGraphErrors * g_deepTau2017v2p1 = runPointROC("deepTau2017v2p1");
   g_run2017v2->SetMarkerStyle(20);
   g_run2017v2->SetMarkerColor(8);
   g_deepTau2017v2p1->SetMarkerStyle(20);
   g_deepTau2017v2p1->SetMarkerColor(9);
   g_run2017v2->Draw("PE, SAME");
   g_deepTau2017v2p1->Draw("PE, SAME");

   TLegend *l2 = new TLegend(0.5, 0.2, 0.875, 0.3);
   l2->SetBorderSize(0);
   l2->AddEntry(g_run2017v2, "run2017v2", "P");
   l2->AddEntry(g_deepTau2017v2p1, "deepTau2017v2p1", "P");
   l2->Draw();*/

   c->SaveAs("./plots/roc.pdf");
   return c;
}

void plotMVAInclusive()
{
   TFile * f = TFile::Open("./TMVA.root");
   TH1D *h_train_S = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_Train_S");
   TH1D *h_train_B = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_Train_B");
   TH1D *h_test_S = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_S");
   TH1D *h_test_B = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_B");
   
   h_train_S->SetLineWidth(2);
   h_train_S->SetLineStyle(1);;
   h_train_S->SetLineColor(4);
  
   h_test_S->SetLineWidth(2); 
   h_test_S->SetLineStyle(1);
   h_test_S->SetLineColor(5);   

   h_train_B->SetLineWidth(2);
   h_train_B->SetLineStyle(1);
   h_train_B->SetLineColor(6);

   h_test_B->SetLineWidth(2);
   h_test_B->SetLineStyle(1);
   h_test_B->SetLineColor(7);

   TCanvas * c = new TCanvas("c_plotMVA", "plotMVA", 400, 400);
   h_train_S->Draw("HIST, E");
   h_train_S->SetStats(0);
   h_train_S->SetTitle(";;");
   h_train_B->Draw("HIST, E, SAME");
   h_test_S->Draw("HIST, E, SAME");
   h_test_B->Draw("HIST, E, SAME");
   h_train_S->SetMaximum(5.);
   h_train_S->SetMinimum(0.);

   TLegend * l = new TLegend(0.25, 0.65, 0.6, 0.875);
   l->SetBorderSize(0);
   l->AddEntry(h_train_S, "train S", "L");
   l->AddEntry(h_train_B, "train B", "L");
   l->AddEntry(h_test_S, "test S", "L");
   l->AddEntry(h_test_B, "test B", "L");
   l->Draw();
   
   c->SaveAs("./plots/mva.pdf");
}

/*TGraphAsymmErrors* plotEff_runPoint(const TCut wp, const TCut sigbkg, const TString name, const TString var)
{
   TFile * f = TFile::Open("./TMVA.root");
   TTree * t = (TTree*)f->Get("dataset/TestTree");

   TH1D *h_num, *h_den;
   TString var_;
   TString title;
   if (var == "pt") {
      h_num = new TH1D("h_pt_num", ";p_{T} [GeV];efficiency", 20, 20., 220.);
      h_den = (TH1D*)h_num->Clone("h_pt_denom");
      var_ = "pt";
      title = ";p_{T} [GeV];efficiency";
   } else if (var=="eta") {
      h_num = new TH1D("h_eta_num", ";|#eta|;efficiency", 10, 0., 3.);
      h_den = (TH1D*)h_num->Clone("h_eta_den");
      var_ = "TMath::Abs(eta)";
      title = ";|#eta|;efficiency";
   } else {
      return 0;
   }
 
   const double n_den = t->Project(h_den->GetName(), var_, sigbkg);
   const double n_num = t->Project(h_num->GetName(), var_, sigbkg && wp);

   TGraphAsymmErrors * g = new TGraphAsymmErrors(h_num, h_den);
   g->SetTitle(title);
   g->SetName(name+"_"+var);

   std::cout << "graph " << g->GetName() << "; eff: " << n_num/n_den << std::endl;
   return g;
}

float getWP(const float eff)
{
   TFile* f = TFile::Open("./TMVA.root");
   TH1D* h = (TH1D*)f->Get("dataset/Method_BDT/BDT/MVA_BDT_effS");
   const int bin = h->FindLastBinAbove(eff);
   const float wp = h->GetBinLowEdge(bin+1);
   return wp;
}

void plotEff(const TString var)
{
   const TCut sig = "classID==0";
   const TCut bkg = "classID==1";

   char wp90[100];
   sprintf(wp90, "BDT>=%f", getWP(0.837238));
   const TCut wp_loose = TCut(wp90);

   char wp40[100];
   sprintf(wp40, "BDT>=%f", getWP(0.294128));
   const TCut wp_tight = TCut(wp40);

   TGraphAsymmErrors * g_sig_loose = plotEff_runPoint(wp_loose, sig, "g_sig_loose", var);
   g_sig_loose->SetMarkerStyle(20);
   g_sig_loose->SetMarkerColor(6);
   g_sig_loose->SetLineColor(6);
   
   TGraphAsymmErrors * g_sig_tight = plotEff_runPoint(wp_tight, sig, "g_sig_tight", var);
   g_sig_tight->SetMarkerStyle(20);
   g_sig_tight->SetMarkerColor(7);
   g_sig_tight->SetLineColor(7);
   
   TGraphAsymmErrors * g_bkg_loose = plotEff_runPoint(wp_loose, bkg, "g_bkg_loose", var);
   g_bkg_loose->SetMarkerStyle(20);
   g_bkg_loose->SetMarkerColor(8);
   g_bkg_loose->SetLineColor(8);
   
   TGraphAsymmErrors * g_bkg_tight = plotEff_runPoint(wp_tight, bkg, "g_bkg_tight", var);
   g_bkg_tight->SetMarkerStyle(20);
   g_bkg_tight->SetMarkerColor(9);
   g_bkg_tight->SetLineColor(9);

   TCanvas * c = new TCanvas("c_plotEff_"+var, "plotEff", 400, 400);
   g_sig_loose->Draw("APE");
   g_sig_loose->SetMinimum(1.e-4);
   g_sig_loose->SetMaximum(1.);
   c->SetLogy();
   g_sig_tight->Draw("PE, SAME");
   g_bkg_loose->Draw("PE, SAME");
   g_bkg_tight->Draw("PE, SAME");

   TLegend * l = new TLegend(0.225, 0.175, 0.825, 0.3);
   l->SetNColumns(4);   
   l->SetBorderSize(0);
   l->AddEntry(g_sig_loose, "sig 84%", "P");
   l->AddEntry(g_sig_tight, "sig 29%", "P");
   l->AddEntry(g_bkg_loose, "bkg loose", "P");
   l->AddEntry(g_bkg_tight, "bkg tight", "P");
   l->Draw();
   
   c->SaveAs("./plots/eff."+var+".pdf");
}
*/
void plotTrainingPerformance()
{
   plotMVAInclusive();
   plotROCInclusive();

   const TCut loweta = "TMath::Abs(eta)<2.3";
   const TCut higheta = "TMath::Abs(eta)>=2.3 && TMath::Abs(eta)<3.";
   const TCut lowpt = "pt>=20. && pt<50.";
   const TCut highpt = "pt>=50. && pt<220.";
   //const TCut recocut = "decayModeFindingNewDMs>0.5 && pt>=20. && pt<220. && TMath::Abs(eta)<3.";
   //const TCut gencut = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   //const TCut baseline = recocut && gencut;
   const TCut baseline = "1>0";

   TGraphErrors * g_lowetalowpt = plotROC(loweta&&lowpt&&baseline, "lowetalowpt");
   TGraphErrors * g_lowetahighpt = plotROC(loweta&&highpt&&baseline, "lowetahighpt");
   TGraphErrors * g_highetalowpt = plotROC(higheta&&lowpt&&baseline, "highetalowpt");
   TGraphErrors * g_highetahighpt = plotROC(higheta&&highpt&&baseline, "highetahighpt");
   TCanvas * c = new TCanvas("c", "c", 400, 400);

   g_lowetalowpt->SetMarkerColor(6);
   g_lowetalowpt->Draw("APE");
   g_lowetalowpt->SetMinimum(0.0001);
   g_lowetalowpt->SetMaximum(1.);
   g_lowetalowpt->GetXaxis()->SetRangeUser(0., 1.);
   g_lowetalowpt->Draw("APE");
   c->Update();
   c->SetLogy();
   g_lowetahighpt->SetMarkerColor(7);
   g_lowetahighpt->Draw("PE, SAME");
   g_highetalowpt->SetMarkerColor(8);
   g_highetalowpt->Draw("PE, SAME");
   g_highetahighpt->SetMarkerColor(9);
   g_highetahighpt->Draw("PE, SAME");

   TLegend * l = new TLegend(0.4, 0.175, 0.825, 0.3);
   l->SetNColumns(2);
   l->SetBorderSize(0);
   l->AddEntry(g_lowetalowpt, "|#eta|<2.3; p_{T}<50", "P");
   l->AddEntry(g_lowetahighpt, "|#eta|<2.3; p_{T}>50", "P");
   l->AddEntry(g_highetalowpt, "|#eta|>2.3; p_{T}<50", "P");
   l->AddEntry(g_highetahighpt, "|#eta|>2.3; p_{T}>50", "P");
   l->Draw();

   c->SaveAs("./plots/trainingrocs.pdf");

/*   plotEff("pt", loweta && baseline, "pt_loweta");
   plotEff("pt", higheta && baseline, "pt_higheta");
   plotEff("eta", lowpt && baseline, "eta_lowpt");
   plotEff("eta", highpt && baseline, "eta_highpt");
   plotEff("getPU_NumInteractions", loweta && lowpt && baseline, "pu_lowetalowpt");
   plotEff("getPU_NumInteractions", loweta && lowpt && baseline, "pu_lowetalowpt");
   plotEff("getPU_NumInteractions", loweta && lowpt && baseline, "pu_lowetalowpt");
   plotEff("getPU_NumInteractions", loweta && lowpt && baseline, "pu_lowetalowpt");*/    
}

