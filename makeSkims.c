#include <TCut.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

void makeSkim(const TString fileTag, const TCut nTaus_gen, const int cap=0)
{
   std::cout << fileTag << std::endl;
   const TCut recocut = "decayModeFindingNewDMs>0.5 && pt>=20. && pt<220. && TMath::Abs(eta)<3.";
   const TCut gencut = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   const TCut cuts = recocut && gencut && nTaus_gen;
 
   char inFile[100];
   sprintf(inFile, "./outputData/%s.root", fileTag.Data());
   TFile * f = TFile::Open(inFile);

   TTree * t = (TTree*)f->Get("tauAnalyzer/tree");
   const double n = t->GetEntries();
   const double n_gen = t->GetEntries(nTaus_gen);
   std::cout << "entries in input tree: " << n << std::endl;
   std::cout << "entries with nTaus_gen cut: " << n_gen << std::endl;
   if (n) std::cout << "rel: " << n_gen/n << std::endl;
    
   char outFile[100];
   sprintf(outFile, "./outputData/skim_%s.root", fileTag.Data());
   TFile * fnew = new TFile(outFile, "RECREATE");
   TTree *t_slim = t->CopyTree(cuts);
   std::cout << "entries in output tree: " << t_slim->GetEntries() << std::endl;
   if (cap) {
      TTree *t_extraslim = t_slim->CopyTree("", "", cap);
      std::cout << "entries in extraslim output tree: " << t_extraslim->GetEntries() << std::endl;
      t_extraslim->Write("skimmedTree");
      f->Close();
   } else {
      t_slim->Write("skimmedTree");
   }
   fnew->Close();
   std::cout << "" << std::endl;
}

void makeSkims()
{
   makeSkim("WToLNu_2J", "nTaus_gen==1", 100000);
   makeSkim("DYToLL-M-50_2J", "nTaus_gen==2", 100000);
   makeSkim("GluGluHToTauTau", "nTaus_gen==2", 100000);
   makeSkim("VBFHToTauTau", "nTaus_gen==2", 100000);
   makeSkim("QCD_Flat_Pt-15to7000", "nTaus_gen==0");
}
