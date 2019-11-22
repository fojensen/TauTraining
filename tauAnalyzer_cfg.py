import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v18'
#process.load('Configuration.StandardSequences.Services_cff')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_cff')
#process.load('Geometry.CaloEventSetup.CaloTopology_cfi')

process.genVisTaus = cms.EDProducer("GenVisTauProducer",
   genParticleCollection = cms.InputTag("prunedGenParticles")
)

#process.goodVertices = cms.EDFilter("VertexSelector",
#   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
#   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"),
#   filter = cms.bool(False)
#)

updatedTauName = "slimmedTausNewID" #name of pat::Tau collection with new tau-Ids
import RecoTauTag.RecoTau.tools.runTauIdMVA as tauIdConfig
tauIdEmbedder = tauIdConfig.TauIDEmbedder(process, cms, debug = False,
                    updatedTauName = updatedTauName,
                    toKeep = ["2017v2", "deepTau2017v2p1"])
tauIdEmbedder.runTauID()

process.tauAnalyzer = cms.EDAnalyzer("TauAnalyzer",
   genJetCollection = cms.InputTag("slimmedGenJets"),
   tauCollection = cms.InputTag(updatedTauName),
   genVisTauCollection = cms.InputTag("genVisTaus:genVisTaus")
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring("/store/mc/PhaseIISpr18AODMiniAOD/GluGluHToTauTau_M125_14TeV_powheg_pythia8/MINIAODSIM/PU200_93X_upgrade2023_realistic_v5-v1/20000/02AB313D-AA45-E811-A1B6-7CD30AB15C58.root")
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("output.root")
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
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

