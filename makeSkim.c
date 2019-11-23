#include <TCut.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

void makeSkim(const TString filetag, const bool isSignal)
{
   const TCut recocut = "decayModeFinding>0.5 && pt>=20. && pt<220. && std::abs(eta)<3.";
   const TCut gencut = "drmin_jet<0.4 && drmin_tau_e>=0.4 && drmin_tau_mu>=0.4";
   TCut cuts = recocut && gencut;
   if (isSignal) {
      cuts = cuts && TCut("drmin_tau_tau<0.4");
   }

   char infile[100];
   sprintf(infile, "./mcsamples/%s.root", filetag.Data());
   TFile * f = TFile::Open(infile);
   TTree * t = (TTree*)f->Get("skimmedTree");
   std::cout << "entries in input tree: " << t->GetEntries() << std::endl;
 
   char outfile[100];
   sprintf(outfile, "./mcsamples/skim_%s.root", filetag.Data());
   TFile * fnew = new TFile(outfile, "RECREATE");
   TTree *t_slim = t->CopyTree(cuts);
   std::cout << "entries in output tree: " << t_slim->GetEntries() << std::endl;
   f->Close();
   t_slim->Write("skimmedTree");
   fnew->Close();
}

