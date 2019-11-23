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
//#include "TRandom3.h"

class TauAnalyzer : public edm::EDAnalyzer {
public:
	explicit TauAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);

   edm::EDGetTokenT<std::vector<pat::Tau>> tauToken_;
   edm::EDGetTokenT<std::vector<reco::GenJet>> genJetToken_; 
   edm::EDGetTokenT<pat::CompositeCandidateCollection> genVisTauToken_;
  
   TTree * tree;
   
   float pt;
   float eta;
   float chargedIsoPtSum;
   float neutralIsoPtSum;
   float puCorrPtSum;
   float photonPtSumOutsideSignalCone; 
   int decayMode;
   int isolationGammaCands_size;
   int signalGammaCands_size;
   float hcalEnergy;
   float ecalEnergy;
   float leadTrack_pt;
   float dxy;
   float dxy_Sig; 
   float ip3d;
   float ip3d_Sig;
   bool hasSecondaryVertex;
   float mass;
   float phi;
   float leadChargedHadrCand_dxy;
   float leadChargedHadrCand_dxysig;
   float leadNeutralCand_pt;
   float decayModeFinding;
   TString labels_run2017v2[8];
   TString labels_deepTau2017v2p1[9];
   float iso_run2017v2[8];
   float iso_deepTau2017v2p1[9];
   float drmin_jet;
   float drmin_tau_e;
   float drmin_tau_mu;
   float drmin_tau_tau;
   float flightLengthSig;
   float flightLength;
   
   //float var1, var2;
   //TRandom3 *r;
   //float varmean;
};

TauAnalyzer::TauAnalyzer(const edm::ParameterSet& iConfig)
{
   tauToken_ = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   genJetToken_ = consumes<std::vector<reco::GenJet>>(iConfig.getParameter<edm::InputTag>("genJetCollection"));
   genVisTauToken_ = consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>("genVisTauCollection"));

   //varmean = iConfig.getParameter<double>("varmean");

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
 
   tree->Branch("pt", &pt, "pt/F");
   tree->Branch("eta", &eta, "eta/F");
   tree->Branch("phi", &phi, "phi/F");
   tree->Branch("mass", &mass, "mass/F");   
   tree->Branch("decayModeFinding", &decayModeFinding, "decayModeFinding/F");
   tree->Branch("decayMode", &decayMode, "decayMode/I");  
   tree->Branch("chargedIsoPtSum", &chargedIsoPtSum, "chargedIsoPtSum/F");
   tree->Branch("neutralIsoPtSum", &neutralIsoPtSum, "neutralIsoPtSum/F");
   tree->Branch("puCorrPtSum", &puCorrPtSum, "puCorrPtSum/F");
   tree->Branch("photonPtSumOutsideSignalCone", &photonPtSumOutsideSignalCone, "photonPtSumOutsideSignalCone/F");
   tree->Branch("signalGammaCands_size", &signalGammaCands_size, "signalGammaCands_size/I");
   tree->Branch("isolationGammaCands_size", &isolationGammaCands_size, "isolationGammaCands_size/I");
   tree->Branch("dxy", &dxy, "dxy/F");
   tree->Branch("dxy_Sig", &dxy_Sig, "dxy_Sig/F");
   tree->Branch("ip3d", &ip3d, "ip3d/F");
   tree->Branch("ip3d_Sig", &ip3d_Sig, "ip3d_Sig/F");
   tree->Branch("hasSecondaryVertex", &hasSecondaryVertex, "hasSecondaryVertex/O");
   tree->Branch("hcalEnergy", &hcalEnergy, "hcalEnergy/F");
   tree->Branch("ecalEnergy", &ecalEnergy, "ecalEnergy/F");
   tree->Branch("iso_run2017v2", iso_run2017v2, "iso_run2017v2[8]/F");
   tree->Branch("iso_deepTau2017v2p1", iso_deepTau2017v2p1, "iso_deepTau2017v2p1[9]/F");
   tree->Branch("flightLengthSig", &flightLengthSig, "flightLengthSig/F");
   tree->Branch("flightLength", &flightLength, "flightLength/F");
   tree->Branch("leadChargedHadrCand_dxy", &leadChargedHadrCand_dxy, "leadChargedHadrCand_dxy/F");
   tree->Branch("leadChargedHadrCand_dxysig", &leadChargedHadrCand_dxysig, "leadChargedHadrCand_dxysig/F");
   tree->Branch("drmin_jet", &drmin_jet, "drmin_jet/F");   
   tree->Branch("drmin_tau_e", &drmin_tau_e, "drmin_tau_e/F");
   tree->Branch("drmin_tau_mu", &drmin_tau_mu, "drmin_tau_mu/F");
   tree->Branch("drmin_tau_tau", &drmin_tau_tau, "drmin_tau_tau/F");

   //r = new TRandom3();
   //tree->Branch("var1", &var1, "var1/F");
   //tree->Branch("var2", &var2, "var2/F");
   
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
   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);

   edm::Handle<std::vector<reco::GenJet>> genJets;
   iEvent.getByToken(genJetToken_, genJets);

   edm::Handle<pat::CompositeCandidateCollection> genVisTaus;
   iEvent.getByToken(genVisTauToken_, genVisTaus);

   for (auto i = taus->begin(); i != taus->end(); ++i) {
 
      drmin_jet = 9.;
      for (auto j = genJets->begin(); j != genJets->end(); ++j) {
         if (reco::deltaR(*i, *j)<drmin_jet) {
            drmin_jet = reco::deltaR(*i, *j);
         }
      }
      if (drmin_jet>=0.4) continue;
 
      pt = i->pt();
      eta = i->eta();
      decayModeFinding = i->tauID("decayModeFinding");
      //if (pt<20. || pt>=220. || std::abs(eta)>=3 || !decayModeFinding) continue;
 
      drmin_tau_e = drmin_tau_mu = drmin_tau_tau = 9.;
      for (auto j = genVisTaus->begin(); j != genVisTaus->end(); ++j) {
         const int id = std::abs(j->pdgId());
         const float dr = reco::deltaR(*i, *j);
         if (id==11 && dr<drmin_tau_e) drmin_tau_e = dr;
         if (id==13 && dr<drmin_tau_mu) drmin_tau_mu = dr;
         if (id==15 && dr<drmin_tau_tau) drmin_tau_tau = dr;
      }
      //if (drmin_tau_e<0.4 || drmin_tau_mu<0.4) continue;

      leadChargedHadrCand_dxy = leadChargedHadrCand_dxysig = -9.;
      if (i->leadChargedHadrCand().isNonnull()) {
         if (i->leadChargedHadrCand()->bestTrack()) {
            leadChargedHadrCand_dxy = i->leadChargedHadrCand()->bestTrack()->dxy();
            leadChargedHadrCand_dxysig = i->leadChargedHadrCand()->bestTrack()->dxyError();
            leadChargedHadrCand_dxysig = leadChargedHadrCand_dxy / leadChargedHadrCand_dxysig;
         }
      }

      phi = i->phi();
      mass = i->mass();
      chargedIsoPtSum = i->tauID("chargedIsoPtSum");
      neutralIsoPtSum = i->tauID("neutralIsoPtSum");
      puCorrPtSum = i->tauID("puCorrPtSum"); 
      photonPtSumOutsideSignalCone = i->tauID("photonPtSumOutsideSignalCone");
      decayMode = i->decayMode();
      signalGammaCands_size = i->signalGammaCands().size();
      isolationGammaCands_size = i->isolationGammaCands().size();
      dxy = i->dxy();
      dxy_Sig = i->dxy_Sig();
      ip3d = i->ip3d();
      ip3d_Sig = i->ip3d_Sig();
      ecalEnergy = i->ecalEnergy();
      hcalEnergy = i->hcalEnergy();
      flightLength = sqrt(i->flightLength().Mag2());
      flightLengthSig = i->flightLengthSig();

      hasSecondaryVertex = i->hasSecondaryVertex();

      //veto_e = i->tauID("againstElectronVLooseMVA6");
      //veto_mu = i->tauID("againstMuonLoose3");

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

 //     var1 = r->Gaus(varmean, 1.);
   //   var2 = r->Gaus(varmean, 2.);
  
      tree->Fill();
   }
   
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauAnalyzer);
