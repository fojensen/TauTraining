import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '93X_upgrade2023_realistic_v5'

process.genVisTaus = cms.EDProducer("GenVisTauProducer",
   genParticleCollection = cms.InputTag("prunedGenParticles")
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
   genVisTauCollection = cms.InputTag("genVisTaus:genVisTaus")
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
      "/store/mc/PhaseIISpr18AODMiniAOD/GluGluHToTauTau_M125_14TeV_powheg_pythia8/MINIAODSIM/PU200_93X_upgrade2023_realistic_v5-v1/20000/02AB313D-AA45-E811-A1B6-7CD30AB15C58.root"
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

process.mypath = cms.Sequence(
   process.genVisTaus
   * process.rerunMvaIsolationSequence
   * getattr(process,updatedTauName)
   * process.tauAnalyzer
)

process.p = cms.Path(process.mypath)

