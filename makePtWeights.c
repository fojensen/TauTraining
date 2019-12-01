#include <TLegend.h>
#include <TCanvas.h>
#include <TFile.h>
#include <iostream>
#include <TH1D.h>
#include <TChain.h>
#include <TH2D.h>

void makePtWeights(const TString sigTag, const TString bkgTag, const TString saveTag)
{
   const int n = 9;
   const double x[n+1] = {20., 25., 35., 50., 70., 95., 125., 160., 200., 245.};
   TH1D * h_pt = new TH1D("h_pt", ";p_{T} [GeV];#tau_{h} candidates / bin", n, x);
   h_pt->Sumw2();
   TH1D * h_eta = new TH1D("h_eta", ";#eta;# of #tau_{h} candidates / bin", 12, -3., 3.);
   h_eta->Sumw2();
   TH2D * h_pteta = new TH2D("h_pteta", ";|#eta|;p_{T} [GeV]", 3, 0., 3., 5, 20., 245.);
   h_pteta->Sumw2();

   TH1D * h_pt_sig = (TH1D*)h_pt->Clone("h_pt_sig");
   TH1D * h_eta_sig = (TH1D*)h_eta->Clone("h_eta_sig");
   TH2D * h_pteta_sig = (TH2D*)h_pteta->Clone("h_pteta_sig");

   TChain * c_sig = new TChain("skimmedTree");
   char infile_sig[100];
   sprintf(infile_sig, "./outputData/skim_%s.root", sigTag.Data());
   c_sig->Add(infile_sig);
   const int n_sig = c_sig->GetEntries();
   std::cout << "number of signal entries: " << n_sig << std::endl;
   c_sig->Project("h_pt_sig", "pt");
   c_sig->Project("h_eta_sig", "eta");
   c_sig->Project("h_pteta_sig", "pt:std::abs(eta)");

   TH1D * h_pt_bkg = (TH1D*)h_pt->Clone("h_pt_bkg");
   TH1D * h_eta_bkg = (TH1D*)h_eta->Clone("h_eta_bkg");
   TH2D * h_pteta_bkg = (TH2D*)h_pteta->Clone("h_pteta_bkg");
  
   TChain * c_bkg = new TChain("skimmedTree");
   char infile_bkg[100];
   sprintf(infile_bkg, "./outputData/skim_%s.root", bkgTag.Data());
   c_bkg->Add(infile_bkg);

   const int n_bkg = c_bkg->GetEntries();
   std::cout << "number of background entries: " << n_bkg << std::endl;

   c_bkg->Project("h_pt_bkg", "pt");
   //c_bkg->Project("h_pt_bkg", "pt", "ptweight * (1>0)"); // test weights

   c_bkg->Project("h_eta_bkg", "eta");
   c_bkg->Project("h_pteta_bkg", "pt:std::abs(eta)");

   TH1D * w_pt = (TH1D*)h_pt_sig->Clone("w_pt");
   w_pt->Divide(h_pt_bkg);
   w_pt->Scale(n_bkg/n_sig); // preserve QCD normalization
   w_pt->GetYaxis()->SetTitle("#tau_{h} / jet");
   
   for (int i = 0; i < w_pt->GetNbinsX()+1; ++i) {
      std::cout << "bin " << i << std::endl;
      std::cout << " " << h_pt_bkg->GetBinContent(i) << " / " << h_pt_sig->GetBinContent(i) << std::endl;
      std::cout << " = " << w_pt->GetBinContent(i) << " +- " << w_pt->GetBinError(i) << std::endl;
      if (w_pt->GetBinContent(i)) {
         std::cout << " fractional error: " << w_pt->GetBinError(i)/w_pt->GetBinContent(i);
      }
      std::cout << std::endl;
   }

   TH1D * w_eta = (TH1D*)h_eta_sig->Clone("w_eta");
   w_eta->Divide(h_eta_bkg);
   w_eta->Scale(n_bkg/n_sig); // preserve QCD normalization

   TH2D * w_pteta = (TH2D*)h_pteta_sig->Clone("w_pteta");
   w_pteta->Divide(h_pteta_bkg);
   w_pteta->Scale(n_bkg/n_sig); // preserve QCD normalization

   TCanvas * canvas = new TCanvas("canvas", "", 800, 400);
   canvas->Divide(2, 1);
   TPad * p1 = (TPad*)canvas->cd(1);
   h_pt_sig->SetLineWidth(2);
   h_pt_sig->SetLineColor(8);
   h_pt_bkg->SetLineWidth(2);
   h_pt_bkg->SetLineColor(9);
   h_pt_sig->SetStats(0);
   h_pt_sig->DrawNormalized("HIST, E");
   h_pt_bkg->DrawNormalized("HIST, E, SAME");
   h_pt_sig->SetMaximum(1.);
   h_pt_sig->SetMinimum(1.e-4);
   p1->SetLogy();

   TLegend * l = new TLegend(0.6, 0.8, 0.875, 0.875);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   l->AddEntry(h_pt_sig, "#tau_{h}", "L");
   l->AddEntry(h_pt_bkg, "jet", "L");
   l->Draw();

   canvas->cd(2);
   w_pt->SetMarkerStyle(20);
   w_pt->SetStats(0);
   w_pt->Draw("PE");

   const TString buffer = "./plots/ptWeights."+saveTag+".pdf";
   //const TString buffer = "./plots/ptweights."+saveTag+".closure.pdf"; // test weights
   canvas->SaveAs(buffer);
   
   char outfile[100];
   sprintf(outfile, "./ptWeights.%s.root", saveTag.Data());
   TFile * f = new TFile(outfile, "RECREATE");
   h_pt_sig->Write();
   h_eta_sig->Write();
   h_pteta_sig->Write();
   h_pt_bkg->Write();
   h_eta_bkg->Write();
   h_pteta_bkg->Write();
   w_pt->Write();
   w_eta->Write();
   w_pteta->Write(); 
   f->Close();
}

