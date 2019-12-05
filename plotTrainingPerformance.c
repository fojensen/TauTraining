#include <TPaveText.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>

void plotROC()
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
   TPaveText *pt = new TPaveText(.85, .9, .975, .975, "NDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   char buffer2[100];
   sprintf(buffer2, "auc: %.3f", auc);
   pt->AddText(buffer2);
   pt->Draw();

   c->SaveAs("./plots/roc.pdf");
}

void plotMVA()
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

TGraphAsymmErrors* plotEff_runPoint(const TCut wp, const TCut sigbkg, const TString name, const TString var)
{
   TFile * f = TFile::Open("./TMVA.root");
   TTree * t = (TTree*)f->Get("dataset/TestTree");

   int ntemp;
   TString title;
   TString var_;
   if (var=="pt") {
      ntemp = 9;
      title = ";p_{T} [GeV];efficiency";
      var_ = "pt";
   } else if (var=="eta") {
       ntemp = 6;
       title = ";|#eta|;efficiency";
       var_ = "TMath_Abs_eta_";
   } else {
      return 0;
   }
   const int n = ntemp;
   double x[n+1];
   if (var=="pt") {
      x[0] = 20.;
      x[1] = 25.;
      x[2] = 35.;
      x[3] = 50.;
      x[4] = 70.;
      x[5] = 95.;
      x[6] = 125.;
      x[7] = 160.;
      x[8] = 200.;
      x[9] = 245.;
   }
   if (var=="eta") {
      x[0] = 0.;
      x[1] = 0.5;
      x[2] = 1.;
      x[3] = 1.5;
      x[4] = 2.;
      x[5] = 2.5;
      x[6] = 3.;
   }

   const TString name_num = "h_num_"+name+"_"+var;
   TH1D * h_num = new TH1D(name_num, ";;", n, x);

   const TString name_den = "h_den_"+name+"_"+var;
   TH1D * h_den = (TH1D*)h_num->Clone(name_den);
 
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
   sprintf(wp90, "BDT>=%f", getWP(0.865891));
   const TCut wp_loose = TCut(wp90);

   char wp40[100];
   sprintf(wp40, "BDT>=%f", getWP(0.307306));
   const TCut wp_tight = TCut(wp40);

   TGraphAsymmErrors * g_sig_loose = plotEff_runPoint(wp_loose, sig, "g_sig_loose", var);
   g_sig_loose->SetMarkerStyle(7);
   g_sig_loose->SetMarkerColor(6);
   g_sig_loose->SetLineColor(6);
   
   TGraphAsymmErrors * g_sig_tight = plotEff_runPoint(wp_tight, sig, "g_sig_tight", var);
   g_sig_tight->SetMarkerStyle(7);
   g_sig_tight->SetMarkerColor(7);
   g_sig_tight->SetLineColor(7);
   
   TGraphAsymmErrors * g_bkg_loose = plotEff_runPoint(wp_loose, bkg, "g_bkg_loose", var);
   g_bkg_loose->SetMarkerStyle(7);
   g_bkg_loose->SetMarkerColor(8);
   g_bkg_loose->SetLineColor(8);
   
   TGraphAsymmErrors * g_bkg_tight = plotEff_runPoint(wp_tight, bkg, "g_bkg_tight", var);
   g_bkg_tight->SetMarkerStyle(7);
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
   l->AddEntry(g_sig_loose, "sig 87%", "P");
   l->AddEntry(g_sig_tight, "sig 31%", "P");
   l->AddEntry(g_bkg_loose, "bkg loose", "P");
   l->AddEntry(g_bkg_tight, "bkg tight", "P");
   l->Draw();
   
   c->SaveAs("./plots/eff."+var+".pdf");
}

void plotTrainingPerformance()
{
   plotMVA();
   plotROC();
   plotEff("pt");
   plotEff("eta");
}

