#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class QWEtaGapProducer : public edm::EDProducer {
public:
	explicit QWEtaGapProducer(const edm::ParameterSet&);
	~QWEtaGapProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	edm::InputTag	srcE_;
	edm::InputTag	srcEta_;
	double		min_;
	double		max_;
};

QWEtaGapProducer::QWEtaGapProducer(const edm::ParameterSet& pset) :
	srcE_(pset.getUntrackedParameter<edm::InputTag>("srcE")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta"))
{
	consumes<std::vector<double> >(srcE_);
	consumes<std::vector<double> >(srcEta_);
	min_ = pset.getUntrackedParameter<double>("minE", std::numeric_limits<double>::min());
	max_ = pset.getUntrackedParameter<double>("maxE", std::numeric_limits<double>::max());

	produces<double>("negEta");
	produces<double>("posEta");
}

QWEtaGapProducer::~QWEtaGapProducer() {
	return;
}

void QWEtaGapProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<std::vector<double> > vectE;
	Handle<std::vector<double> > vectEta;
	iEvent.getByLabel(srcE_, vectE);
	iEvent.getByLabel(srcEta_, vectEta);

	std::vector<double> vectE2;
	std::vector<double> vectEta2;

	for ( unsigned int i = 0; i < vectE->size(); i++ ) {
		if ( (*vectE)[i] > min_ and (*vectE)[i] < max_ ) {
			vectE2.push_back( (*vectE)[i] );
			vectEta2.push_back( (*vectEta)[i] );
		}
	}
	auto edge = std::minmax_element( vectE2.begin(), vectE2.end() );
	int idx_neg = edge.first - vectE2.begin();
	int idx_pos = edge.second - vectE2.begin();

	iEvent.put( std::auto_ptr<double>(new double(vectEta2[idx_neg])), "negEta" );
	iEvent.put( std::auto_ptr<double>(new double(vectEta2[idx_pos])), "posEta" );
}

DEFINE_FWK_MODULE(QWEtaGapProducer);