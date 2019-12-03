#include <TFile.h>
#include <TTree.h>
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"

void runTraining(const TString sigTag)
{
   char outputFile_[100];
   sprintf(outputFile_, "TMVA.%s.root", sigTag.Data());
   TFile* outputFile = TFile::Open(outputFile_, "RECREATE");

   const TString theJobName = "MVAAnalysis";
   //const TString optionTable_1 = 
   //   "V=True:Color=True:Transformations=I:Silent=False:DrawProgressBar=True:AnalysisType=Classification"
   //; // Transformations=?
   const TString optionTable_1 = "";
   TMVA::Factory *factory = new TMVA::Factory(theJobName, outputFile, optionTable_1);

   char sigFile[100];
   sprintf(sigFile, "./outputData/skim_%s.root", sigTag.Data());
   TFile *input_sig = TFile::Open(sigFile);
   TFile *input_bkg = TFile::Open("./outputData/skim_QCD_Flat_Pt-15to7000_PU200.root");

   TMVA::DataLoader loader("dataset");
 
   // Run 1
   loader.AddVariable("chargedIsoPtSum");
   loader.AddVariable("neutralIsoPtSum");
   loader.AddVariable("decayMode");
   loader.AddVariable("TMath::Sqrt(TMath::Abs(leadChargedHadrCand_dxy))");
   loader.AddVariable("TMath::Abs(leadChargedHadrCand_dxysig)");
   loader.AddVariable("hasSecondaryVertex");
   loader.AddVariable("hasSecondaryVertex? TMath::Sqrt(TMath::Abs(flightLength)) : -0.25");
   loader.AddVariable("hasSecondaryVertex? TMath::Abs(flightLengthSig): -0.25");
   loader.AddVariable("puCorrPtSum");

   loader.AddVariable("pt");
   loader.AddVariable("TMath::Abs(eta)");
   loader.AddSpectator("eta");

   // Run 2
   loader.AddVariable("photonPtSumOutsideSignalCone"); 
   loader.AddVariable("TMath::Sqrt(TMath::Abs(ip3d))");
   loader.AddVariable("TMath::Abs(ip3d_Sig)");
   loader.AddVariable("signalGammaCands_size");
   loader.AddVariable("signalGammaCands_size? sigCands_dr : -0.25");
   loader.AddVariable("signalGammaCands_size? sigCands_deta : -0.25");
   loader.AddVariable("signalGammaCands_size? sigCands_dphi: -0.25");
   loader.AddVariable("isolationGammaCands_size");
   loader.AddVariable("isolationGammaCands_size? isoCands_dr: -0.25");
   loader.AddVariable("isolationGammaCands_size? isoCands_deta: -0.25");
   loader.AddVariable("isolationGammaCands_size? isoCands_dphi: -0.25");
  
   loader.AddSignalTree((TTree*)input_sig->Get("skimmedTree"));
   loader.AddBackgroundTree((TTree*)input_bkg->Get("skimmedTree"));

   loader.SetWeightExpression("ptetaWeight", "Background");

   //const TString optionTable_2 = 
   //   "SplitMode=Random:MixMode=SameAsSplitMode:SplitSeed=100:NormMode=EqualNumEvents:nTrain_signal=10000:nTest_signal=10000:nTrain_Background=10000:nTest_Background=10000:V=True:VerboseLevel=Debug"
   //;
   const TString optionTable_2 = "";
   loader.PrepareTrainingAndTestTree("", "", optionTable_2);
 
   const TString methodTitle = "BDT";
   //const TString optionTable_25 = 
   //   "NTrees=800:MaxDepth=3:MinNodeSize=5%:nCuts=20:BoostType=AdaBoost:AdaBoostR2Loss=Quadratic:UseBaggedGrad=False:Shrinkage=1:AdaBoostBeta=0.5:UseRandomisedTrees=False:UseNvars=2:UsePoissonNvars=True:BaggedSampleFraction=0.6:UseYesNoLeaf=True"
   //;
   //const TString optionTable_26 =
   //   "NegWeightTreatment=InverseBoostNegWeights:NodePurityLimit=0.5:SeparationType=GiniIndex:DoBoostMonitor=False:UseFisherCuts=False:MinLinCorrForFisher=0.8:UseExclusiveVars=False:DoPreselection=False:SigToBkgFraction=1"
   //;//RenormByClass=False?
   const TString optionTable_25 = "";
   const TString optionTable_26 = "";
   factory->BookMethod(&loader, TMVA::Types::kBDT, methodTitle, optionTable_25+":"+optionTable_26);
  
   factory->TrainAllMethods();  
   factory->TestAllMethods();
   factory->EvaluateAllMethods();
  
   outputFile->Close();

   delete factory;
}

