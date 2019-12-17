import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.register('globalTag',
   '93X_upgrade2023_realistic_v5',
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.string,
   "globalTag"
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = options.globalTag

process.genVisTaus = cms.EDProducer("GenVisTauProducer",
   genParticleCollection = cms.InputTag("prunedGenParticles")
)

process.goodVertices = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"),
   filter = cms.bool(False)
)

process.eventAnalyzer = cms.EDAnalyzer("EventAnalyzer",
   tauCollection = cms.InputTag("slimmedTaus"),
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   vertexCollection = cms.InputTag("goodVertices")
)

updatedTauName = "slimmedTausNewID"
import RecoTauTag.RecoTau.tools.runTauIdMVA as tauIdConfig
tauIdEmbedder = tauIdConfig.TauIDEmbedder(
   process,
   cms,
   debug = False,
   updatedTauName = updatedTauName,
   toKeep = ["2017v2", "deepTau2017v2p1"])
tauIdEmbedder.runTauID()

process.tauAnalyzer = cms.EDAnalyzer("TauAnalyzer",
   tauCollection = cms.InputTag(updatedTauName),
   genJetCollection = cms.InputTag("slimmedGenJets"),
   genVisTauCollection = cms.InputTag("genVisTaus:genVisTaus"),
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   vertexCollection = cms.InputTag("goodVertices")
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
      "/store/mc/PhaseIISpr18AODMiniAOD/GluGluHToTauTau_M125_14TeV_powheg_pythia8/MINIAODSIM/PU200_93X_upgrade2023_realistic_v5-v1/20000/02AB313D-AA45-E811-A1B6-7CD30AB15C58.root"
      #"/store/mc/PhaseIISpr18AODMiniAOD/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/MINIAODSIM/PU200_93X_upgrade2023_realistic_v5-v1/90000/FA267931-2E44-E811-B0F9-C4346BC80410.root"
      #"/store/mc/PhaseIISpr18AODMiniAOD/WToLNu_2J_14TeV-madgraphMLM-pythia8/MINIAODSIM/PU200_93X_upgrade2023_realistic_v5-v1/90000/BA580DFF-2049-E811-B211-1CC1DE1CF44E.root"
   )
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("output.root")
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(100)
)

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True),
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
   src = cms.InputTag("prunedGenParticles"),
   printP4 = cms.untracked.bool(False),
   printPtEtaPhi = cms.untracked.bool(False),
   printVertex = cms.untracked.bool(False),
   printStatus = cms.untracked.bool(False),
   printIndex = cms.untracked.bool(False),
   #status = cms.untracked.vint32(3),
)

process.mypath = cms.Sequence(
   process.goodVertices
   * process.eventAnalyzer
   #* process.printTree
   * process.genVisTaus
   * process.rerunMvaIsolationSequence
   * getattr(process,updatedTauName)
   * process.tauAnalyzer
)

process.p = cms.Path(process.mypath)

