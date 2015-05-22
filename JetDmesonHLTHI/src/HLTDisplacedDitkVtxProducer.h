#ifndef HLTDisplacedDitkVtxProducer_h
#define HLTDisplacedDitkVtxProducer_h


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

class HLTDisplacedDitkVtxProducer : public edm::EDProducer {
 public:
  explicit HLTDisplacedDitkVtxProducer(const edm::ParameterSet&);
  ~HLTDisplacedDitkVtxProducer();
  
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
