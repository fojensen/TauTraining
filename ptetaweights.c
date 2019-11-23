#include <TFile.h>
#include <iostream>
#include <TCut.h>
#include <TH1D.h>
#include <TChain.h>
#include <TH2D.h>

void ptetaweights(const TString sigfile, const TString bkgfile, const TString savetag)
{
   TH1D * h_pt = new TH1D("h_pt", ";p_{T} [GeV];", 20, 20., 220.);
   h_pt->Sumw2();
   TH1D * h_eta = new TH1D("h_eta", ";#eta;", 12, -3., 3.);
   h_eta->Sumw2();
   TH2D * h_pteta = new TH2D("h_pteta", ";|#eta|;p_{T} [GeV]", 3, 0., 3., 5, 20., 220.);
   h_pteta->Sumw2();

   TH1D * h_pt_sig = (TH1D*)h_pt->Clone("h_pt_sig");
   TH1D * h_eta_sig = (TH1D*)h_eta->Clone("h_eta_sig");
   TH2D * h_pteta_sig = (TH2D*)h_pteta->Clone("h_pteta_sig");

   TChain * c_sig = new TChain("skimmedTree");
   c_sig->Add(sigfile);
   const int n_sig = c_sig->GetEntries();
   c_sig->Project("h_pt_sig", "pt");
   c_sig->Project("h_eta_sig", "eta");
   c_sig->Project("h_pteta_sig", "pt:std::abs(eta)");   
   h_pt_sig->Scale(1./n_sig);
   h_eta_sig->Scale(1./n_sig);
   h_pteta_sig->Scale(1./n_sig);

   TH1D * h_pt_bkg = (TH1D*)h_pt->Clone("h_pt_bkg");
   TH1D * h_eta_bkg = (TH1D*)h_eta->Clone("h_eta_bkg");
   TH2D * h_pteta_bkg = (TH2D*)h_pteta->Clone("h_pteta_bkg");
  
   TChain * c_bkg = new TChain("skimmedTree");
   c_bkg->Add(bkgfile);
   const int n_bkg = c_bkg->GetEntries();
   c_bkg->Project("h_pt_bkg", "pt");
   c_bkg->Project("h_eta_bkg", "eta");
   c_bkg->Project("h_pteta_bkg", "pt:std::abs(eta)");
   h_pt_bkg->Scale(1./n_bkg);
   h_eta_bkg->Scale(1./n_bkg);
   h_pteta_bkg->Scale(1./n_bkg); 

   TH1D * w_pt = (TH1D*)h_pt_sig->Clone("w_pt");
   w_pt->Divide(h_pt_bkg);

   TH1D * w_eta = (TH1D*)h_eta_sig->Clone("w_eta");
   w_eta->Divide(h_eta_bkg);

   TH2D * w_pteta = (TH2D*)h_pteta_sig->Clone("w_pteta");
   w_pteta->Divide(h_pteta_bkg);

   char outfile[100];
   sprintf(outfile, "./ptetaweights.%s.root", savetag.Data());
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

