#ifndef HLTDisplacedtktkVtxProducer_h
#define HLTDisplacedtktkVtxProducer_h


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include <vector>

namespace edm {
  class ConfigurationDescriptions;
}

class HLTDisplacedtktkVtxProducer : public edm::EDProducer {
 public:
  explicit HLTDisplacedtktkVtxProducer(const edm::ParameterSet&);
  ~HLTDisplacedtktkVtxProducer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

 private:  

  edm::InputTag src_;
  double maxEta_;
  double mintrackPt_;
  double minPtPair_;
  double minInvMass_;
  double maxInvMass_;
};

#endif
