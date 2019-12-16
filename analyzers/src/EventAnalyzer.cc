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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class EventAnalyzer : public edm::EDAnalyzer {
public:
	explicit EventAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);

   edm::EDGetTokenT<std::vector<pat::Tau>> tauToken_;
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
   edm::EDGetTokenT<std::vector<reco::Vertex>> vertexToken_;
   TTree * tree;
   int nElectrons_gen, nMuons_gen, nTaus_gen;
   int nTaus;
   int nVertices;
   int nQuarks, nGluons;
   int nQuarks23, nGluons23;
};

EventAnalyzer::EventAnalyzer(const edm::ParameterSet& iConfig)
{
   tauToken_ = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   vertexToken_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("nElectrons_gen", &nElectrons_gen, "nElectrons_gen/I");
   tree->Branch("nMuons_gen", &nMuons_gen, "nMuons_gen/I");
   tree->Branch("nTaus_gen", &nTaus_gen, "nTaus_gen/I");
   tree->Branch("nTaus", &nTaus, "nTaus/I");
   tree->Branch("nVertices", &nVertices, "nVertices/I");
   tree->Branch("nQuarks23", &nQuarks23, "nQuarks23/I");
   tree->Branch("nGluons23", &nGluons23, "nGluons23/I");
   tree->Branch("nQuarks", &nQuarks, "nQuarks/I");
   tree->Branch("nGluons", &nGluons, "nGluons/I");
}

void EventAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   nElectrons_gen = nMuons_gen = nTaus_gen = 0;
   nQuarks23 = nGluons23 = 0;
   nQuarks = nGluons = 0;
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      const int id = std::abs(i->pdgId());
      if (i->isLastCopy()) {
         if (id==11) ++nElectrons_gen;
         if (id==13) ++nMuons_gen;
         if (id==15) ++nTaus_gen;
         if (i->status()==23) {
            if (id<=5) ++nQuarks23; 
            if (id==21) ++nGluons23;
         } else {
            if (id<=5) ++nQuarks;
            if (id==21) ++nGluons;
         }
      }
      
   }

   edm::Handle<std::vector<reco::Vertex>> vertices;
   iEvent.getByToken(vertexToken_, vertices);
   nVertices = vertices->size();

   nTaus = 0;
   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);
   for (auto i = taus->begin(); i != taus->end(); ++i) {
      if (i->pt()>=20. && i->pt()<220. && std::abs(i->eta())<3. && i->tauID("decayModeFindingNewDMs")) {
         ++nTaus;
      }
   }

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(EventAnalyzer);

