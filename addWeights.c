#include <iostream>
#include <TChain.h>
#include <TH1D.h>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>

void addWeights()
{
   TFile * f_w = TFile::Open("ptetaweights.PU200.root");
   TH1D * h = (TH1D*)f_w->Get("w_pt");
   TH1D * h_ptweights = (TH1D*)h->Clone("h_ptweights");
   TH1D * h_ptweights_temp = (TH1D*)h->Clone("h_ptweights_temp");
   h->SetDirectory(0);
   h_ptweights->SetDirectory(0);
   h_ptweights_temp->SetDirectory(0);
   f_w->Close();

   TFile * f = TFile::Open("./mcsamples/QCD_PU200.skim.root", "UPDATE");
   TTree * t = (TTree*)f->Get("skimmedTree");
   
   float ptweight = 0.;
   TBranch *b_ptweight;
   b_ptweight = t->Branch("ptweight", &ptweight, "ptweight/F");

   float pt = 0.;
   t->SetBranchAddress("pt", &pt);

   for (int i = 0; i < t->GetEntries(); ++i) {
      t->GetEntry(i);
      ptweight = h_ptweights->GetBinContent(h_ptweights_temp->Fill(pt));
      b_ptweight->Fill();
   }

   t->Write("skimmedTree", TObject::kOverwrite);
   f->Close();
}

