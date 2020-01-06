#include <TCut.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

void makeSkim(const TString fileTag, const TCut genTauCut, const int cap=0)
{
   std::cout << "beginning " << fileTag << std::endl;
 
   char inFile[100];
   sprintf(inFile, "./outputData/%s.root", fileTag.Data());
   TFile * f = TFile::Open(inFile);
   if (!f) {
     std::cout << "file not found" << std::endl;
     return;
   }
   
   TTree * tEvent = (TTree*)f->Get("eventAnalyzer/tree");
   const double nEvent = tEvent->GetEntries();
   std::cout << "# of mc events: " << nEvent << std::endl; 
   const double nEvent2 = tEvent->GetEntries(genTauCut);
   std::cout << nEvent2 << " events survive the genTauCut" << std::endl;
   std::cout << "eff: " << nEvent2/nEvent << std::endl;

   TTree * tTau = (TTree*)f->Get("tauAnalyzer/tree");
   std::cout << tTau->GetEntries(genTauCut) << " reconstructed taus" << std::endl;

   std::cout << "now skimming the files..." << std::endl;    
   const TCut recocut = "decayModeFindingNewDMs>0.5 && pt>=20. && pt<220. && TMath::Abs(eta)<3.";
   const TCut gencut = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   const TCut cuts = recocut && gencut && genTauCut;

   char outFile[100];
   sprintf(outFile, "./outputData/skim_%s.root", fileTag.Data());
   TFile * fnew = new TFile(outFile, "RECREATE");
   TTree *t_slim = tTau->CopyTree(cuts);
   const int nSlim = t_slim->GetEntries();
   std::cout << "entries in output tree after skimming: " << nSlim << std::endl;  
   if (cap!=0 && nSlim>cap) {
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
   makeSkim("WToLNu_2J", "nTaus_gen==1", 100000);
   makeSkim("DYToLL-M-50_2J", "nTaus_gen==2", 100000);
   makeSkim("TTTo2L2Nu", "nTaus_gen==2", 100000);
   makeSkim("GluGluHToTauTau", "nTaus_gen==2", 100000);
   makeSkim("VBFHToTauTau", "nTaus_gen==2", 100000);
   makeSkim("QCD_Flat", "nTaus_gen==0", 1000000);
   makeSkim("VBFHToTauTau_Run2", "nTaus_gen==2", 100000);
   makeSkim("QCD_Flat_Run2", "nTaus_gen==0", 1000000);
}

