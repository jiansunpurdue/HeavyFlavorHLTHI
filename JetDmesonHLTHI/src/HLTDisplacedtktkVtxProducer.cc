#include <iostream>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerRefsCollections.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "HeavyFlavorHLTHI/JetDmesonHLTHI/interface/HFMasses.hh"

#include "HLTDisplacedtktkVtxProducer.h"
#include <TLorentzVector.h>

using namespace edm;
using namespace reco;
using namespace std; 
using namespace trigger;
//
// constructors and destructor
//
HLTDisplacedtktkVtxProducer::HLTDisplacedtktkVtxProducer(const edm::ParameterSet& iConfig):	
	src_ (iConfig.getParameter<edm::InputTag>("Src")),
	maxEta_ (iConfig.getParameter<double>("MaxEta")),
	mintrackPt_ (iConfig.getParameter<double>("MinTrackPt")),
	minPtPair_ (iConfig.getParameter<double>("MinPtPair")),
	minInvMass_ (iConfig.getParameter<double>("MinInvMass")),
	maxInvMass_ (iConfig.getParameter<double>("MaxInvMass"))
{
	produces<VertexCollection>();
}


HLTDisplacedtktkVtxProducer::~HLTDisplacedtktkVtxProducer()
{

}

void HLTDisplacedtktkVtxProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	edm::ParameterSetDescription desc;
	desc.add<edm::InputTag>("Src",edm::InputTag("hltFastPixelBLifetimeL3AssociatorHbb"));
	desc.add<double>("MaxEta",2.5);
	desc.add<double>("MinTrackPt",2.0);
	desc.add<double>("MinPtPair",5.0);
	desc.add<double>("MinInvMass",1.55);
	desc.add<double>("MaxInvMass",2.10);
	descriptions.add("hltDisplacedmumumuVtxProducer", desc);
}

// ------------ method called once each job just before starting event loop  ------------
void HLTDisplacedtktkVtxProducer::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void HLTDisplacedtktkVtxProducer::endJob() 
{

}

// ------------ method called on each new Event  ------------
void HLTDisplacedtktkVtxProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

//	edm::Handle <reco::TrackCollection> tracks;
	edm::Handle <edm::View<reco::Track>> tracks;
	iEvent.getByLabel(src_,tracks);

	edm::ESHandle<TransientTrackBuilder> theB;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);

	std::auto_ptr<VertexCollection> vertexCollection(new VertexCollection());

	TLorentzVector ka,pi,trackpair;
	for(unsigned int ipion = 0; ipion < tracks->size(); ipion++)
	{
		reco::TrackBaseRef pionTrackView(tracks,ipion);
		reco::Track track_pion(*pionTrackView);
		if( track_pion.pt() < mintrackPt_ )   continue;

		for(unsigned int ikaon = 0; ikaon < tracks->size(); ikaon++)
		{
			if( ipion == ikaon )  continue;
		    reco::TrackBaseRef kaonTrackView(tracks,ikaon);
		    reco::Track track_kaon(*kaonTrackView);
			if( track_pion.charge() * track_kaon.charge() > 0 )   continue;
			if( track_kaon.pt() < mintrackPt_ )   continue;
			pi.SetPtEtaPhiM(track_pion.pt(), track_pion.eta(), track_pion.phi(), MPION);
			ka.SetPtEtaPhiM(track_kaon.pt(), track_kaon.eta(), track_kaon.phi(), MKAON);
			trackpair = ka + pi;
			if (  trackpair.M() <  minInvMass_ || trackpair.M() >   maxInvMass_) continue;
			if ( trackpair.Pt() < minPtPair_)     continue;
			if ( TMath::Abs( trackpair.Eta() ) > maxEta_ )     continue;


			// do the vertex fit
			vector<TransientTrack> t_tks;
			TransientTrack ttkp1 = (*theB).build(&track_pion);
			TransientTrack ttkp2 = (*theB).build(&track_kaon);
			t_tks.push_back(ttkp1);
			t_tks.push_back(ttkp2);

			KalmanVertexFitter kvf;
			TransientVertex tv = kvf.vertex(t_tks);

			if (!tv.isValid()) continue;

			Vertex vertex = tv;
			vertex.removeTracks();    //by default, the dau tracks are not saved but the track size is saved.
			vertex.add(reco::TrackBaseRef(tracks, ipion));// Need to reset the track size and then add the dau tracks
			vertex.add(reco::TrackBaseRef(tracks, ikaon));
			// put vertex in the event
			vertexCollection->push_back(vertex);

		}

	}


	iEvent.put(vertexCollection);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTDisplacedtktkVtxProducer);
