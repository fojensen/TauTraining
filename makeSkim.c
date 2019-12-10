#include <TCut.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

void makeSkim(const TString fileTag)
{
   const TCut recocut = "decayModeFindingNewDMs>0.5 && pt>=20. && pt<220. && TMath::Abs(eta)<3.";
   const TCut gencut = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   const TCut cuts = recocut && gencut;
 
   char inFile[100];
   sprintf(inFile, "./outputData/%s.root", fileTag.Data());
   TFile * f = TFile::Open(inFile);
   TTree * t = (TTree*)f->Get("tauAnalyzer/tree");
   std::cout << "entries in input tree: " << t->GetEntries() << std::endl;
    
   char outFile[100];
   sprintf(outFile, "./outputData/skim_%s.root", fileTag.Data());
   TFile * fnew = new TFile(outFile, "RECREATE");
   TTree *t_slim = t->CopyTree(cuts);
   std::cout << "entries in output tree: " << t_slim->GetEntries() << std::endl;
   f->Close();
   t_slim->Write("skimmedTree");
   fnew->Close();
}

