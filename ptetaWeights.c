#include <TCut.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TFile.h>
#include <iostream>
#include <TH1F.h>
#include <TChain.h>
#include <TH2F.h>

void addWeights(const TString weightFile, const TString bkgTag)
{
   TFile * f_w = TFile::Open(weightFile);
   TH1F * w_pt = (TH1F*)f_w->Get("w_pt");
   TH1F * w_eta = (TH1F*)f_w->Get("w_eta");
   TH2F * w_pteta = (TH2F*)f_w->Get("w_pteta");
   w_pt->SetDirectory(0);
   w_eta->SetDirectory(0);
   w_pteta->SetDirectory(0);
   f_w->Close();
   TH1F * w_pt_temp = (TH1F*)w_pt->Clone("w_pt_temp");
   TH1F * w_eta_temp = (TH1F*)w_eta->Clone("w_eta_temp");
   TH2F * w_pteta_temp = (TH2F*)w_pteta->Clone("w_pteta_temp");

   const TString bkgFile = "./outputData/skim_"+bkgTag+".root";
   TFile * f = TFile::Open(bkgFile, "UPDATE");
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
      std::cout << bin << std::endl;
      int binx = 0;
      int biny = 0;
      int binz = 0;
      w_pteta_temp->GetBinXYZ(bin, binx, biny, binz);
      std::cout << binx << " " << biny << std::endl;
      ptetaWeight = w_pteta->GetBinContent(binx, biny);
      std::cout << ptetaWeight << " ptetaWeight " << std::endl;
      b_ptetaWeight->Fill();
   }

   t->Write("skimmedTree", TObject::kOverwrite);
   f->Close();
}

TCanvas * drawWeights(TH1F * h_sig, TH1F *h_bkg, TH1F * h_weight, const TString sigTag, const TString bkgTag, const TString var)
{
   const TString canvasName = "canvas_"+sigTag+"_"+bkgTag+"_"+var;
   TCanvas * canvas = new TCanvas(canvasName, canvasName, 800, 400);
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

   const TString saveTag = "./plots/ptWeights_"+sigTag+"_"+bkgTag+".pdf";
   canvas->SaveAs(saveTag);

   std::cout << "stats for " << var << std::endl;
   for (int i = 0; i < h_weight->GetNbinsX()+1; ++i) {
      std::cout << "bin " << i << std::endl;
      std::cout << " " << h_bkg->GetBinContent(i) << " / " << h_sig->GetBinContent(i) << std::endl;
      std::cout << " = " << h_weight->GetBinContent(i) << " +- " << h_weight->GetBinError(i) << std::endl;
      if (h_weight->GetBinContent(i)) {
         std::cout << " fractional error: " << h_weight->GetBinError(i)/h_weight->GetBinContent(i);
      }
      std::cout << std::endl;
   }
 
   return canvas;
}

TCanvas * draw2DWeights(TH2F *h)
{
   TCanvas * canvas = new TCanvas("canvas", "pteta", 400, 400);
   canvas->SetLogy();
   h->SetMarkerSize(2);
   h->Draw("COLZ, TEXT, E");
   h->SetStats(0);
   canvas->SaveAs("./plots/ptetaWeights.pdf");

   for (int i = 1; i < h->GetNbinsX()+1; ++i) {
      for (int j = 1; j < h->GetNbinsY()+1; ++j) {
         std::cout << "bin " << i << ", " << j << std::endl;
         std::cout << " = " << h->GetBinContent(i, j) << " +- " << h->GetBinError(i, j) << std::endl;
         if (h->GetBinContent(i, j)) {
            std::cout << " fractional error: " << h->GetBinError(i, j)/h->GetBinContent(i, j);
       }
         std::cout << std::endl;
      }
   }
   return canvas;
}

TString makeWeights(const TString sigTag, const TString bkgTag)
{
   gStyle->SetPaintTextFormat("4.2f");
   const int n_pt = 9;
   const double x_pt[n_pt+1] = {20., 25., 35., 50., 70., 95., 125., 160., 200., 245.};
   TH1F * h_pt = new TH1F("h_pt", ";p_{T} [GeV];#tau_{h} candidates / bin", n_pt, x_pt);
   h_pt->Sumw2();
   
   const int n_eta = 6;
   const double x_eta[n_eta+1] = {0., 0.5, 1., 1.5, 2., 2.5, 3.}; 
   TH1F * h_eta = new TH1F("h_eta", ";|#eta|;#tau_{h} candidates / 0.25", n_eta, x_eta);
   h_eta->Sumw2();
   
   const int n2_pt = 6;
   const double x2_pt[n2_pt+1] = {20., 30., 50., 80., 120., 170., 245.};
   const int n2_eta = 3;
   const double x2_eta[n2_eta+1] = {0., 1., 1.5, 3.};
   TH2F * h_pteta = new TH2F("h_pteta", ";|#eta|;p_{T} [GeV]", n2_eta, x2_eta, n2_pt, x2_pt);
   h_pteta->Sumw2();

   TH1F * h_pt_sig = (TH1F*)h_pt->Clone("h_pt_sig");
   TH1F * h_eta_sig = (TH1F*)h_eta->Clone("h_eta_sig");
   TH2F * h_pteta_sig = (TH2F*)h_pteta->Clone("h_pteta_sig");

   TH1F * h_pt_bkg = (TH1F*)h_pt->Clone("h_pt_bkg");
   TH1F * h_eta_bkg = (TH1F*)h_eta->Clone("h_eta_bkg");
   TH2F * h_pteta_bkg = (TH2F*)h_pteta->Clone("h_pteta_bkg");

   TChain * c_sig = new TChain("skimmedTree");
   char infile_sig[100];
   sprintf(infile_sig, "./outputData/skim_%s.root", sigTag.Data());
   c_sig->Add(infile_sig);
   const int n_sig = c_sig->GetEntries();
   std::cout << "number of signal entries: " << n_sig << std::endl;

   const TCut cut1 = "1>0";
   const TCut cut2 = "1>0";
   const TCut cut3 = "1>0";
   //const TCut cut1 = "ptWeight * (1>0)";
   //const TCut cut2 = "etaWeight * (1>0)";
   //const TCut cut3 = "ptetaWeight * (1>0)";

   c_sig->Project("h_pt_sig", "pt", cut1);
   c_sig->Project("h_eta_sig", "TMath::Abs(eta)", cut2);
   c_sig->Project("h_pteta_sig", "pt:TMath::Abs(eta)", cut3);

   TChain * c_bkg = new TChain("skimmedTree");
   char infile_bkg[100];
   sprintf(infile_bkg, "./outputData/skim_%s.root", bkgTag.Data());
   c_bkg->Add(infile_bkg);
   const int n_bkg = c_bkg->GetEntries();
   std::cout << "number of background entries: " << n_bkg << std::endl;

   c_bkg->Project("h_pt_bkg", "pt");
   //c_bkg->Project("h_pt_bkg", "pt", "ptweight * (1>0)"); // test weights
   c_bkg->Project("h_eta_bkg", "TMath::Abs(eta)");
   c_bkg->Project("h_pteta_bkg", "pt:TMath::Abs(eta)");

   TH1F * w_pt = (TH1F*)h_pt_sig->Clone("w_pt");
   w_pt->Divide(h_pt_bkg);
   w_pt->Scale(n_bkg/n_sig); // preserve QCD normalization
   w_pt->GetYaxis()->SetTitle("#tau_{h} / jet");
   drawWeights(h_pt_sig, h_pt_bkg, w_pt, sigTag, bkgTag, "pt");

   TH1F * w_eta = (TH1F*)h_eta_sig->Clone("w_eta");
   w_eta->Divide(h_eta_bkg);
   w_eta->Scale(n_bkg/n_sig); // preserve QCD normalization
   drawWeights(h_eta_sig, h_eta_bkg, w_eta, sigTag, bkgTag, "eta");

   TH2F * w_pteta = (TH2F*)h_pteta_sig->Clone("w_pteta");
   w_pteta->Divide(h_pteta_bkg);
   w_pteta->Scale(n_bkg/n_sig); // preserve QCD normalization
   draw2DWeights(w_pteta);

   //sprintf(outfile, "./ptWeights.%s.closure.root", saveTag.Data()); // test weights
   const TString outputFile = "./ptetaWeights_"+sigTag+"_"+bkgTag+".root";
   TFile * f = new TFile(outputFile, "RECREATE");
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

   return outputFile;
}

void ptetaWeights(const TString sigTag, const TString bkgTag)
{
   const TString weightFile = makeWeights(sigTag, bkgTag);
   addWeights(weightFile, bkgTag);
}
