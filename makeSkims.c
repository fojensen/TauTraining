#include <TCut.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

void makeSkim(const TString fileTag, const int cap=0)
{
   std::cout << "beginning " << fileTag << std::endl;
   std::cout << fileTag << std::endl;
   const TCut recocut = "decayModeFindingNewDMs>0.5 && pt>=20. && pt<220. && TMath::Abs(eta)<3.";
   const TCut gencut = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   const TCut cuts = recocut && gencut;
 
   char inFile[100];
   sprintf(inFile, "./outputData/%s.root", fileTag.Data());
   TFile * f = TFile::Open(inFile);

   TTree * t = (TTree*)f->Get("tauAnalyzer/tree");
   const double n = t->GetEntries();
   std::cout << "total number of reconstructed taus: " << n << std::endl;
    
   std::cout << "now skimming the files..." << std::endl;

   char outFile[100];
   sprintf(outFile, "./outputData/skim_%s.root", fileTag.Data());
   TFile * fnew = new TFile(outFile, "RECREATE");
   TTree *t_slim = t->CopyTree(cuts);
   const double nSlim = t_slim->GetEntries();;
   std::cout << "entries in output tree, after full skimming requirements: " << nSlim << std::endl;  
   if (cap) {
      TTree *t_extraslim = t_slim->CopyTree("", "", cap);
      const double nExtraSlim =  t_extraslim->GetEntries();
      std::cout << "entries in the capped output tree: " << nExtraSlim << std::endl;
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
   makeSkim("WToLNu_2J", 100000);
   makeSkim("DYToLL-M-50_2J", 100000);
   makeSkim("TTTo2L2Nu", 100000);
   makeSkim("GluGluHToTauTau", 100000);
   makeSkim("VBFHToTauTau", 100000);
   makeSkim("QCD_Flat_Pt-15to7000");
}
