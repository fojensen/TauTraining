#include <TCut.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TFile.h>
#include <iostream>
#include <TH1D.h>
#include <TChain.h>
#include <TH2D.h>

void addWeights()
{
   std::cout << "addWeights" << std::endl;
   TFile * f_w = TFile::Open("./ptetaWeights.root");
   TH1D * w_pt = (TH1D*)f_w->Get("w_pt");
   TH1D * w_eta = (TH1D*)f_w->Get("w_eta");
   TH2D * w_pteta = (TH2D*)f_w->Get("w_pteta");
   w_pt->SetDirectory(0);
   w_eta->SetDirectory(0);
   w_pteta->SetDirectory(0);
   f_w->Close();
   TH1D * w_pt_temp = (TH1D*)w_pt->Clone("w_pt_temp");
   TH1D * w_eta_temp = (TH1D*)w_eta->Clone("w_eta_temp");
   TH2D * w_pteta_temp = (TH2D*)w_pteta->Clone("w_pteta_temp");

   TFile * f = TFile::Open("./outputData/skim_QCD_Flat.root", "UPDATE");
   TTree * t = (TTree*)f->Get("skimmedTree");
   
   float ptWeight = 0.;
   TBranch *b_ptWeight;
   b_ptWeight = t->Branch("ptWeight", &ptWeight, "ptWeight/F");

   float etaWeight = 0.;
   TBranch *b_etaWeight;
   b_etaWeight = t->Branch("etaWeight", &etaWeight, "etaWeight/F");

   float ptetaWeight = 0.;
   TBranch *b_ptetaWeight;
   b_ptetaWeight = t->Branch("ptetaWeight", &ptetaWeight, "ptetaWeight/F");

   float pt = 0.; t->SetBranchAddress("pt", &pt);
   float eta = 0.; t->SetBranchAddress("eta", &eta);

   for (int i = 0; i < t->GetEntries(); ++i) {
      t->GetEntry(i);
      ptWeight = w_pt->GetBinContent(w_pt_temp->Fill(pt));
      b_ptWeight->Fill();
      etaWeight = w_eta->GetBinContent(w_eta_temp->Fill(TMath::Abs(eta)));
      b_etaWeight->Fill();
      const int bin = w_pteta_temp->Fill(TMath::Abs(eta), pt);
      int binx = 0;
      int biny = 0;
      int binz = 0;
      w_pteta_temp->GetBinXYZ(bin, binx, biny, binz);
      ptetaWeight = w_pteta->GetBinContent(binx, biny);
      b_ptetaWeight->Fill();
   }

   t->Write("skimmedTree", TObject::kOverwrite);
   f->Close();
}

TCanvas * drawWeights(TH1D *h_sig, TH1D *h_bkg, TH1D *h_weight, const TString var)
{
   std::cout << "drawWeights" << std::endl;
   TCanvas * canvas = new TCanvas("canvas_"+var, "drawWeights", 800, 400);
   canvas->Divide(2, 1);

   TPad * p1 = (TPad*)canvas->cd(1);
   h_sig->SetLineWidth(2);
   h_sig->SetLineColor(8);
   h_bkg->SetLineWidth(2);
   h_bkg->SetLineColor(9);
   h_sig->SetStats(0);
   h_sig->DrawNormalized("HIST, E");
   h_bkg->DrawNormalized("HIST, E, SAME");
   h_sig->SetMaximum(1.);
   h_sig->SetMinimum(1.e-4);
   p1->SetLogy();

   TLegend * l = new TLegend(0.6, 0.8, 0.875, 0.875);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   l->AddEntry(h_sig, "#tau_{h}", "L");
   l->AddEntry(h_bkg, "jet", "L");
   l->Draw();

   canvas->cd(2);
   h_weight->SetMarkerStyle(20);
   h_weight->SetStats(0);
   h_weight->Draw("PE");
   
   char buffer[100];
   sprintf(buffer, "./plots/weights.%s.pdf", var.Data());
   canvas->SaveAs(buffer);

   for (int i = 1; i <= h_weight->GetNbinsX(); ++i) {
      const double n = h_sig->GetBinContent(i);
      if (n) {
         const double f = h_weight->GetBinError(i)/h_weight->GetBinContent(i);
         if (f>0.05) {
            std::cout << "bin " <<  i << " fractional error weight exceeds 5%" << std::endl;
            std::cout << "   low edge: " << h_weight->GetBinLowEdge(i) << std::endl;
         }
      } else {
         std::cout << "bin " <<  i << " has no signal entries!" << std::endl;
         std::cout << "   low edge: " << h_weight->GetBinLowEdge(i) << std::endl;
      }
   }

   return canvas;
}

TCanvas * draw2DWeights(TH2D *h)
{
   std::cout << "draw2DWeights" << std::endl;
   TCanvas * canvas = new TCanvas("canvas", "draw2DWeights", 800, 800);
   canvas->SetLogy();
   h->SetMarkerSize(1);
   h->Draw("COLZ, TEXT, E");
   h->SetStats(0);
   canvas->SaveAs("./plots/weights.pteta.pdf");

   for (int i = 1; i <= h->GetNbinsX(); ++i) {
      for (int j = 1; j <= h->GetNbinsY(); ++j) {
         if (h->GetBinContent(i, j)) {
            const double y = h->GetBinContent(i, j);
            double yerr = h->GetBinError(i, j);
            const double rel = yerr/y;
            std::cout << y << " " << yerr << " " << rel << std::endl;
            if (rel>0.05) {
               std::cout << "large relative error! bin " << i << "," << j << std::endl;
            }
         }
      }
   }
   return canvas;
}

void makeWeights()
{
   std::cout << "makeWeights" << std::endl;
   gStyle->SetPaintTextFormat("4.2f");

   TH1D * h_pt = new TH1D("h_pt", ";p_{T} [GeV];#tau_{h} candidates / 10 GeV", 20, 20., 220.);
   h_pt->Sumw2();
   TH1D * h_eta = new TH1D("h_eta", ";|#eta|;#tau_{h} candidates / 0.1", 30, 0., 3.);
   h_eta->Sumw2();
   
   const int n2_pt = 11;
   const double x2_pt[n2_pt+1] = {20., 30., 40., 50., 60., 70., 80., 90., 100., 120., 140., 220.};
   const int n2_eta = 3;
   const double x2_eta[n2_eta+1] = {0., 1.5, 2.3, 3.};
   TH2D * h_pteta = new TH2D("h_pteta", ";|#eta|;p_{T} [GeV]", n2_eta, x2_eta, n2_pt, x2_pt);
   h_pteta->Sumw2();

   TH1D * h_pt_sig = (TH1D*)h_pt->Clone("h_pt_sig");
   TH1D * h_eta_sig = (TH1D*)h_eta->Clone("h_eta_sig");
   TH2D * h_pteta_sig = (TH2D*)h_pteta->Clone("h_pteta_sig");

   TH1D * h_pt_bkg = (TH1D*)h_pt->Clone("h_pt_bkg");
   TH1D * h_eta_bkg = (TH1D*)h_eta->Clone("h_eta_bkg");
   TH2D * h_pteta_bkg = (TH2D*)h_pteta->Clone("h_pteta_bkg");

   const TCut sigcut = "drmin_tau_tau<0.4";
   const TCut bkgcut = "drmin_tau_tau>=0.4";

   TChain * c_sig = new TChain("skimmedTree");
   c_sig->Add("./outputData/skim_WToLNu_2J.root");
   c_sig->Add("./outputData/skim_DYToLL-M-50_2J.root");
   c_sig->Add("./outputData/skim_TTTo2L2Nu.root");
   c_sig->Add("./outputData/skim_GluGluHToTauTau.root");
   c_sig->Add("./outputData/skim_VBFHToTauTau.root");
   const int n_sig = c_sig->GetEntries(sigcut);
   std::cout << "number of signal entries: " << n_sig << std::endl;

   c_sig->Project("h_pt_sig", "pt", sigcut);
   c_sig->Project("h_eta_sig", "TMath::Abs(eta)", sigcut);
   c_sig->Project("h_pteta_sig", "pt:TMath::Abs(eta)", sigcut);

   TChain * c_bkg = new TChain("skimmedTree");
   c_bkg->Add("./outputData/skim_QCD_Flat.root");
   const int n_bkg = c_bkg->GetEntries(bkgcut);
   std::cout << "number of background entries: " << n_bkg << std::endl;

   c_bkg->Project("h_pt_bkg", "pt", bkgcut);
   c_bkg->Project("h_eta_bkg", "TMath::Abs(eta)", bkgcut);
   c_bkg->Project("h_pteta_bkg", "pt:TMath::Abs(eta)", bkgcut);

   TH1D * w_pt = (TH1D*)h_pt_sig->Clone("w_pt");
   w_pt->Divide(h_pt_bkg);
   w_pt->Scale(n_bkg/n_sig); // preserve QCD normalization
   w_pt->GetYaxis()->SetTitle("#tau_{h} / jet");
   drawWeights(h_pt_sig, h_pt_bkg, w_pt, "pt");

   TH1D * w_eta = (TH1D*)h_eta_sig->Clone("w_eta");
   w_eta->Divide(h_eta_bkg);
   w_eta->Scale(n_bkg/n_sig); // preserve QCD normalization
   drawWeights(h_eta_sig, h_eta_bkg, w_eta, "eta");

   TH2D * w_pteta = (TH2D*)h_pteta_sig->Clone("w_pteta");
   w_pteta->Divide(h_pteta_bkg);
   w_pteta->Scale(n_bkg/n_sig); // preserve QCD normalization
   draw2DWeights(w_pteta);

   TFile * f = new TFile("ptetaWeights.root", "RECREATE");
   h_pt_sig->Write();
   h_pt_bkg->Write();
   w_pt->Write();
   h_eta_sig->Write();
   h_eta_bkg->Write();
   w_eta->Write();
   h_pteta_sig->Write();
   h_pteta_bkg->Write();
   w_pteta->Write(); 
   f->Close();
}

void ptetaWeights()
{
   std::cout << "draw2DWeights" << std::endl;
   makeWeights();
   addWeights();
}

