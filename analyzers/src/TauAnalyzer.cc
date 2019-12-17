// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TTree.h>
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class TauAnalyzer : public edm::EDAnalyzer {
public:
	explicit TauAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);

   edm::EDGetTokenT<std::vector<pat::Tau>> tauToken_;
   edm::EDGetTokenT<std::vector<reco::GenJet>> genJetToken_; 
   edm::EDGetTokenT<pat::CompositeCandidateCollection> genVisTauToken_;
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
   edm::EDGetTokenT<std::vector<reco::Vertex>> vertexToken_;
   TTree * tree;
 
   float recTauGJangleDiff;
   float recTauGJangleMeasured;
   float chargedIsoPtSum;
   float neutralIsoPtSum;
   int decayMode;
   float dxy;
   float dxy_Sig;
   float decayModeFinding;
   float decayModeFindingNewDMs;
   float leadChargedHadrCand_dxy;
   float leadChargedHadrCand_dxysig;  
   bool hasSecondaryVertex;
   float flightLength;
   float flightLengthSig; 
   float puCorrPtSum;
   float pt;
   float eta;
   float photonPtSumOutsideSignalCone; 
   float ip3d;
   float ip3d_Sig;
   int isolationGammaCands_size;
   int signalGammaCands_size;
   float sigCands_dr;
   float sigCands_deta;
   float sigCands_dphi;
   float isoCands_dr;
   float isoCands_deta;
   float isoCands_dphi;

   float ecalEnergy;
   float hcalEnergy;
   float leadingTrackNormChi2;

   TString labels_run2017v2[8];
   TString labels_deepTau2017v2p1[9];
   float iso_run2017v2[8];
   float iso_deepTau2017v2p1[9];
   float drmin_jet;
   float drmin_tau_e;
   float drmin_tau_mu;
   float drmin_tau_tau;
   float drmin_e;
   float drmin_mu;
   float drmin_tau;
   float drmin_b;

   int nTaus_gen, nVertices;
};

TauAnalyzer::TauAnalyzer(const edm::ParameterSet& iConfig)
{
   tauToken_ = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   genJetToken_ = consumes<std::vector<reco::GenJet>>(iConfig.getParameter<edm::InputTag>("genJetCollection"));
   genVisTauToken_ = consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>("genVisTauCollection"));
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   vertexToken_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");

   tree->Branch("recTauGJangleDiff", &recTauGJangleDiff, "recTauGJangleDiff/F");
   tree->Branch("recTauGJangleMeasured", &recTauGJangleMeasured, "recTauGJangleMeasured/F");
   tree->Branch("pt", &pt, "pt/F");
   tree->Branch("eta", &eta, "eta/F");
   tree->Branch("dxy", &dxy, "dxy/F");
   tree->Branch("dxy_Sig", &dxy_Sig, "dxy_Sig/F");
   tree->Branch("decayMode", &decayMode, "decayMode/I");
   tree->Branch("decayModeFinding", &decayModeFinding, "decayModeFinding/F");
   tree->Branch("decayModeFindingNewDMs", &decayModeFindingNewDMs, "decayModeFindingNewDMs/F");
   tree->Branch("chargedIsoPtSum", &chargedIsoPtSum, "chargedIsoPtSum/F");
   tree->Branch("neutralIsoPtSum", &neutralIsoPtSum, "neutralIsoPtSum/F");
   tree->Branch("puCorrPtSum", &puCorrPtSum, "puCorrPtSum/F");
   tree->Branch("photonPtSumOutsideSignalCone", &photonPtSumOutsideSignalCone, "photonPtSumOutsideSignalCone/F");
   tree->Branch("signalGammaCands_size", &signalGammaCands_size, "signalGammaCands_size/I");
   tree->Branch("isolationGammaCands_size", &isolationGammaCands_size, "isolationGammaCands_size/I");
   tree->Branch("ip3d", &ip3d, "ip3d/F");
   tree->Branch("ip3d_Sig", &ip3d_Sig, "ip3d_Sig/F");
   tree->Branch("hasSecondaryVertex", &hasSecondaryVertex, "hasSecondaryVertex/O");
   tree->Branch("flightLengthSig", &flightLengthSig, "flightLengthSig/F");
   tree->Branch("flightLength", &flightLength, "flightLength/F");
   tree->Branch("leadChargedHadrCand_dxy", &leadChargedHadrCand_dxy, "leadChargedHadrCand_dxy/F");
   tree->Branch("leadChargedHadrCand_dxysig", &leadChargedHadrCand_dxysig, "leadChargedHadrCand_dxysig/F");
   tree->Branch("sigCands_dr", &sigCands_dr, "sigCands_dr/F");
   tree->Branch("sigCands_deta", &sigCands_deta, "sigCands_deta/F");
   tree->Branch("sigCands_dphi", &sigCands_dphi, "sigCands_dphi/F");
   tree->Branch("isoCands_dr", &isoCands_dr, "isoCands_dr/F");
   tree->Branch("isoCands_deta", &isoCands_deta, "isoCands_deta/F");
   tree->Branch("isoCands_dphi", &isoCands_dphi, "isoCands_dphi/F");

   tree->Branch("ecalEnergy", &ecalEnergy, "ecalEnergy/F");
   tree->Branch("hcalEnergy", &hcalEnergy, "hcalEnergy/F");
   tree->Branch("leadingTrackNormChi2", &leadingTrackNormChi2 , "leadingTrackNormChi2/F");

   tree->Branch("iso_run2017v2", iso_run2017v2, "iso_run2017v2[8]/F");
   tree->Branch("iso_deepTau2017v2p1", iso_deepTau2017v2p1, "iso_deepTau2017v2p1[9]/F");
   tree->Branch("drmin_jet", &drmin_jet, "drmin_jet/F");   
   tree->Branch("drmin_tau_e", &drmin_tau_e, "drmin_tau_e/F");
   tree->Branch("drmin_tau_mu", &drmin_tau_mu, "drmin_tau_mu/F");
   tree->Branch("drmin_tau_tau", &drmin_tau_tau, "drmin_tau_tau/F");
   tree->Branch("drmin_e", &drmin_e, "drmin_e/F");
   tree->Branch("drmin_mu", &drmin_mu, "drmin_mu/F");
   tree->Branch("drmin_tau", &drmin_tau, "drmin_tau/F");
   tree->Branch("drmin_b", &drmin_b, "drmin_b/F");

   tree->Branch("nVertices", &nVertices, "nVertices/I");
   tree->Branch("nTaus_gen", &nTaus_gen, "nTaus_gen/I");

   labels_run2017v2[0] = "inclusive";
   labels_run2017v2[1] = "byVVLooseIsolationMVArun2017v2DBoldDMwLT2017";
   labels_run2017v2[2] = "byVLooseIsolationMVArun2017v2DBoldDMwLT2017";
   labels_run2017v2[3] = "byLooseIsolationMVArun2017v2DBoldDMwLT2017";
   labels_run2017v2[4] = "byMediumIsolationMVArun2017v2DBoldDMwLT2017";
   labels_run2017v2[5] = "byTightIsolationMVArun2017v2DBoldDMwLT2017";
   labels_run2017v2[6] = "byVTightIsolationMVArun2017v2DBoldDMwLT2017";
   labels_run2017v2[7] = "byVVTightIsolationMVArun2017v2DBoldDMwLT2017";

   labels_deepTau2017v2p1[0] = "inclusive";
   labels_deepTau2017v2p1[1] = "byVVVLooseDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[2] = "byVVLooseDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[3] = "byVLooseDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[4] = "byLooseDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[5] = "byMediumDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[6] = "byTightDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[7] = "byVTightDeepTau2017v2p1VSjet";
   labels_deepTau2017v2p1[8] = "byVVTightDeepTau2017v2p1VSjet";
}

void TauAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);

   edm::Handle<std::vector<reco::GenJet>> genJets;
   iEvent.getByToken(genJetToken_, genJets);

   edm::Handle<pat::CompositeCandidateCollection> genVisTaus;
   iEvent.getByToken(genVisTauToken_, genVisTaus);

   nTaus_gen = 0;
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->isLastCopy()) {
         if (std::abs(i->pdgId())==15) ++nTaus_gen;
      }
   }

   edm::Handle<std::vector<reco::Vertex>> vertices;
   iEvent.getByToken(vertexToken_, vertices);
   nVertices = vertices->size();

   for (auto i = taus->begin(); i != taus->end(); ++i) {
 
      drmin_jet = 9.;
      for (auto j = genJets->begin(); j != genJets->end(); ++j) {
         if (reco::deltaR(*i, *j)<drmin_jet) {
            drmin_jet = reco::deltaR(*i, *j);
         }
      }
      if (drmin_jet>=0.4) continue;
 
      drmin_tau_e = drmin_tau_mu = drmin_tau_tau = 9.;
      for (auto j = genVisTaus->begin(); j != genVisTaus->end(); ++j) {
         const int id = std::abs(j->pdgId());
         const float dr = reco::deltaR(*i, *j);
         if (id==11 && dr<drmin_tau_e) drmin_tau_e = dr;
         if (id==13 && dr<drmin_tau_mu) drmin_tau_mu = dr;
         if (id==15 && dr<drmin_tau_tau) drmin_tau_tau = dr;
      }
      if (drmin_tau_e<0.4) continue;
      if (drmin_tau_mu<0.4) continue;

      drmin_e = drmin_mu = drmin_tau = 9.;
      drmin_b = 9.;
      for (auto j = genParticles->begin(); j != genParticles->end(); ++j) {
         if (j->isLastCopy()) {
            const int id = std::abs(j->pdgId());
            const float dr = reco::deltaR(*i, *j);
            if (id==5  && dr<drmin_b) drmin_b = dr;
            if (id==11 && dr<drmin_e) drmin_e = dr;
            if (id==13 && dr<drmin_mu) drmin_mu = dr;
            if (id==15 && dr<drmin_tau) drmin_tau = dr;
         }
      }
  
      leadChargedHadrCand_dxy = leadChargedHadrCand_dxysig = -999.;
      if (i->leadChargedHadrCand().isNonnull()) {
         if (i->leadChargedHadrCand()->bestTrack()) {
            leadChargedHadrCand_dxy = i->leadChargedHadrCand()->bestTrack()->dxy();
            const float dxyError = i->leadChargedHadrCand()->bestTrack()->dxyError();
            if (dxyError>0.) {
               leadChargedHadrCand_dxysig = leadChargedHadrCand_dxy / dxyError;
            }
         }
      }
      if (std::isnan(leadChargedHadrCand_dxy)) leadChargedHadrCand_dxy = -998.;
      if (std::isnan(leadChargedHadrCand_dxysig)) leadChargedHadrCand_dxysig = -998.;

      pt = i->pt();
      eta = i->eta();
      decayMode = i->decayMode();
      decayModeFinding = i->tauID("decayModeFinding");
      decayModeFindingNewDMs = i->tauID("decayModeFindingNewDMs");
      chargedIsoPtSum = i->tauID("chargedIsoPtSum");
      neutralIsoPtSum = i->tauID("neutralIsoPtSum");
      puCorrPtSum = i->tauID("puCorrPtSum"); 
      photonPtSumOutsideSignalCone = i->tauID("photonPtSumOutsideSignalCone");
      signalGammaCands_size = i->signalGammaCands().size();
      isolationGammaCands_size = i->isolationGammaCands().size();
      dxy = i->dxy();
      dxy_Sig = i->dxy_Sig();
      ip3d = i->ip3d();
      ip3d_Sig = i->ip3d_Sig();
      flightLength = sqrt(i->flightLength().Mag2());
      flightLengthSig = i->flightLengthSig();
      hasSecondaryVertex = i->hasSecondaryVertex();
      ecalEnergy = i->ecalEnergy();
      hcalEnergy = i->hcalEnergy(); 
      leadingTrackNormChi2 = i->leadingTrackNormChi2();

      if (i->decayMode()==10) {
         // calculate difference between maximally allowed Gottfried-Jackson angle (angle between tau and a1 momentum)
         // and measured Gottfried-Jackson angle from flightlength vector and tau momentum
         // thetaGJmax = arcsin( ( m_tau^2 - m_a1^2) / ( 2 * m_tau * mag(p_a1) ))
         // thetaGJmeasured is simply the angle between the flightLength vector and the tau momentum
         double mTau = 1.77682;
         double mAOne = i->p4().M();
         double flightLengthMag = i->flightLength().Mag2();
         double pAOneMag = i->p();
         double thetaGJmax = TMath::ASin( (TMath::Power(mTau,2) - TMath::Power(mAOne,2)) / ( 2 * mTau * pAOneMag) );
         double thetaGJmeasured = TMath::ACos( ( i->p4().px() * i->flightLength().x() + i->p4().py() * i->flightLength().y() + i->p4().pz() * i->flightLength().z()) / ( pAOneMag * TMath::Sqrt(flightLengthMag)) );
         recTauGJangleMeasured = thetaGJmeasured;
         recTauGJangleDiff = thetaGJmeasured - thetaGJmax;
      } else {
         recTauGJangleMeasured = -999.;
         recTauGJangleDiff = -999.;
      }

      float sigCands_pt = 0.;
      sigCands_dr = sigCands_deta = sigCands_dphi = 0.;
      for (auto j = i->signalGammaCands().begin(); j < i->signalGammaCands().end(); ++j) {
         const float dr = reco::deltaR(*i, **j);
         const float deta = std::abs(i->eta() - (*j)->eta());
         const float dphi = std::abs(reco::deltaPhi(i->phi(), (*j)->phi()));
         const float pt_ = (*j)->pt();
         sigCands_dr += dr * pt_;
         sigCands_deta += deta * pt_;
         sigCands_dphi += dphi * pt_;
         sigCands_pt += pt_;
      }
      if (sigCands_pt>0.) {
         sigCands_dr = sigCands_dr / sigCands_pt;
         sigCands_deta = sigCands_deta / sigCands_pt;
         sigCands_dphi = sigCands_dphi / sigCands_pt;
      } else {
         sigCands_dr = sigCands_deta = sigCands_dphi = -999.;
      }

      float isoCands_pt = 0.;
      isoCands_dr = isoCands_deta = isoCands_dphi = 0.;
      for (auto j = i->isolationGammaCands().begin(); j < i->isolationGammaCands().end(); ++j) {
         const float dr = reco::deltaR(*i, **j);
         const float deta = std::abs(i->eta() - (*j)->eta());
         const float dphi = std::abs(reco::deltaPhi(i->phi(), (*j)->phi()));
         const float pt_ = (*j)->pt();
         isoCands_dr += dr * pt_;
         isoCands_deta += deta * pt_;
         isoCands_dphi += dphi * pt_;
         isoCands_pt += pt_;
      }
      if (isoCands_pt>0.) {
         isoCands_dr = isoCands_dr / isoCands_pt;
         isoCands_deta = isoCands_deta / isoCands_pt;
         isoCands_dphi = isoCands_dphi / isoCands_pt;
      } else {
         isoCands_dr = isoCands_deta = isoCands_dphi = -999.;
      }

      for (int j = 0; j < 8; ++j) {
         if (j==0) {
            iso_run2017v2[j] = 1.;
         } else {
            iso_run2017v2[j] = i->tauID(labels_run2017v2[j]);
         }
      }
      for (int j = 0; j < 9; ++j) {
         if (j==0) {
            iso_deepTau2017v2p1[j] = 1.;
         } else {
            iso_deepTau2017v2p1[j] = i->tauID(labels_deepTau2017v2p1[j]);
         }
      }  
 
      tree->Fill();
   }
   
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauAnalyzer);

