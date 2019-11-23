#include <TFile.h>
#include <TTree.h>
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include <math.h>

void runTraining()
{
   TFile* outputFile = TFile::Open("TMVA.root", "RECREATE");
   const TString theJobName = "MVAnalysis";
   const TString theOption_1 = "";
   TMVA::Factory *factory = new TMVA::Factory(theJobName, outputFile, theOption_1);

   TFile *input_sig = TFile::Open("./mcsamples/skim_GluGluHToTauTau_PU200.root");
   TFile *input_bkg = TFile::Open("./mcsamples/skim_QCD_Flat_Pt-15to7000_PU200.root");

   TMVA::DataLoader loader("dataset");
   
   loader.AddVariable("chargedIsoPtSum");
   loader.AddVariable("neutralIsoPtSum");
   loader.AddVariable("leadChargedHadrCand_dxy");
   loader.AddVariable("leadChargedHadrCand_dxysig");
   loader.AddVariable("decayMode");
   //loader.AddVariable("hasSecondaryVertex");
   loader.AddVariable("hasSecondaryVertex ? flightLengthSig : -2.");
   loader.AddVariable("hasSecondaryVertex ? flightLength : -2.");
   //loader.AddVariable("flightLengthSig")
   //loader.AddVariable("flightLength");
   loader.AddVariable("puCorrPtSum");
   loader.AddVariable("math::log(pt)");
   loader.AddVariable("std::abs(eta)");

   loader.AddSignalTree((TTree*)input_sig->Get("skimmedTree"));
   loader.AddBackgroundTree((TTree*)input_bkg->Get("skimmedTree"));

   //loader.SetWeightExpression("ptweight", "Background");

   const TString splitOpt = "";
   loader.PrepareTrainingAndTestTree("", "", splitOpt);
 
   const TString methodTitle = "BDT";
   const TString theOption_2 = "";
   factory->BookMethod(&loader, TMVA::Types::kBDT, methodTitle, theOption_2);
  
   factory->TrainAllMethods();  
   factory->TestAllMethods();
   factory->EvaluateAllMethods();
  
   outputFile->Close();

   delete factory;
}
