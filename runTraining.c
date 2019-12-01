#include <TFile.h>
#include <TTree.h>
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"

void runTraining()
{
   TFile* outputFile = TFile::Open("TMVA.GluGluHToTauTau_PU200.root", "RECREATE");
   const TString theJobName = "MVAAnalysis";
   //const TString theOption_1 = "";
   const TString theOption_1 = "V=True:Color=True:Transformations=I;D;P;U;G;D:Silent=False:DrawProgressBar=True:AnalysisType=Classification";
   TMVA::Factory *factory = new TMVA::Factory(theJobName, outputFile, theOption_1);

   TFile *input_sig = TFile::Open("./mcsamples/skim_GluGluHToTauTau_PU200.root");
   TFile *input_bkg = TFile::Open("./mcsamples/skim_QCD_Flat_Pt-15to7000_PU200.root");

   TMVA::DataLoader loader("dataset");
 
   // Run 1
   loader.AddVariable("chargedIsoPtSum");
   loader.AddVariable("neutralIsoPtSum");
   loader.AddVariable("decayMode");
   //loader.AddVariable("TMath::Sqrt(TMath::Abs(leadChargedHadrCand_dxy))");
   //loader.AddVariable("TMath::Abs(leadChargedHadrCand_dxysig");
   loader.AddVariable("hasSecondaryVertex");
   loader.AddVariable("hasSecondaryVertex? TMath::Sqrt(TMath::Abs(flightLength)) : -0.25");
   loader.AddVariable("hasSecondaryVertex? TMath::Abs(flightLengthSig): -0.25");
   //loader.AddVariable("pt");
   loader.AddSpectator("pt");
   loader.AddVariable("TMath::Abs(eta)");
   loader.AddVariable("puCorrPtSum");

   // Run 2
   loader.AddVariable("photonPtSumOutsideSignalCone"); 
   loader.AddVariable("TMath::Sqrt(TMath::Abs(ip3d))");
   loader.AddVariable("TMath::Abs(ip3d_Sig)");
   loader.AddVariable("signalGammaCands_size");
   loader.AddVariable("isolationGammaCands_size"); 
   loader.AddVariable("signalGammaCands_size? sigCands_dr : -0.25");
   loader.AddVariable("signalGammaCands_size? sigCands_deta : -0.25");
   loader.AddVariable("signalGammaCands_size? sigCands_dphi: -0.25");
   loader.AddVariable("isolationGammaCands_size? isoCands_dr: -0.25");
   loader.AddVariable("isolationGammaCands_size? isoCands_deta: -0.25");
   loader.AddVariable("isolationGammaCands_size? isoCands_dphi: -0.25");
  
   loader.AddSignalTree((TTree*)input_sig->Get("skimmedTree"));
   loader.AddBackgroundTree((TTree*)input_bkg->Get("skimmedTree"));

   loader.SetWeightExpression("ptweight", "Background");

   //const TString splitOpt = "";
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

