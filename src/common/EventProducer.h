#ifndef SRC_COMMON_EVENTPRODUCER_H_
#define SRC_COMMON_EVENTPRODUCER_H_

#include "../common/common.h"

#include "NativeWrapper.h"

#include <orbsvcs/Event_Utilities.h>
#include <orbsvcs/RtecEventChannelAdminS.h>

namespace corbabinding {

template<typename T>
class EventProducer {
public:
	EventProducer(RtecEventComm::EventSourceID sourceId, RtecEventComm::EventType type) :
		mLog(getLog("EventProducer")), mSourceId(sourceId), mType(type), mSuplierPersonality(this, false) {}

	virtual ~EventProducer() {
		disconnect();
	}

	void connect(NativeWrapper & wrapper) {
		mLog.v() << "EventProducer::connect(NativeWrapper & wrapper)" << done;
		RtecEventChannelAdmin::SupplierAdmin_var supplierAdmin = wrapper.getEventService()->for_suppliers();
		mConsumerProxy = supplierAdmin->obtain_push_consumer();
		RtecEventComm::PushSupplier_var supplier = this->mSuplierPersonality._this();
		ACE_SupplierQOS_Factory publications;
		publications.insert(mSourceId, mType, 0, 1);
		mConsumerProxy->connect_push_supplier(supplier.in(), publications.get_SupplierQOS());
	}

	void disconnect() {
		if (mConsumerProxy.in()) {
			mConsumerProxy->disconnect_push_consumer();
		}
	}

	void disconnect_push_supplier(void) {
		mConsumerProxy = RtecEventChannelAdmin::ProxyPushConsumer::_nil();
	}

	void push(const T & event) {
		if (CORBA::is_nil(mConsumerProxy.in())) {
		mLog.w() << "consumer proxy is null" << done;
			return;
		}
		RtecEventComm::EventSet eventSet(1);
		eventSet.length(1);
		RtecEventComm::Event &e = eventSet[0];
		e.header.source = mSourceId;
		e.header.type = mType;
		e.data.any_value <<= event;
		mConsumerProxy->push(eventSet);
		return;
	}

private:
	testutil::Log mLog;
	RtecEventComm::EventSourceID mSourceId;
	RtecEventComm::EventType mType;
	POA_RtecEventComm::PushSupplier_tie<EventProducer> mSuplierPersonality;
	RtecEventChannelAdmin::ProxyPushConsumer_var mConsumerProxy;
};

} /* namespace corbabinding */

#endif /* SRC_COMMON_EVENTPRODUCER_H_ */
