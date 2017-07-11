#ifndef SRC_COMMON_ANYEVENTCONSUMER_H_
#define SRC_COMMON_ANYEVENTCONSUMER_H_

#include "NativeWrapper.h"

#include "../common/common.h"

#include <orbsvcs/Event_Utilities.h>
#include <orbsvcs/RtecEventChannelAdminC.h>
#include <orbsvcs/RtecEventCommS.h>

namespace corbabinding {

class AnyEventConsumer : public POA_RtecEventComm::PushConsumer {
public:
	AnyEventConsumer(CORBA::ULong type): mLog(getLog("AnyEventConsumer")), mType(type), mNativeWrapper(0x0) {}

	virtual ~AnyEventConsumer() {}

	void connect(NativeWrapper * nativeWraper) {
		mLog.v() << "AnyEventConsumer::connect(NativeWrapper & nativeWraper)" << testutil::done;
		mNativeWrapper = nativeWraper;
		ACE_ConsumerQOS_Factory qos;
		qos.start_disjunction_group();
		qos.insert_type(mType, 0);
		RtecEventChannelAdmin::ConsumerAdmin_var consumerAdmin = nativeWraper->getEventService()->for_consumers();
		mSupplierProxy = consumerAdmin->obtain_push_supplier();
		mMyself = _this();
		mSupplierProxy->connect_push_consumer(mMyself.in(), qos.get_ConsumerQOS());
	}

	void push(const RtecEventComm::EventSet& data) {
		for (CORBA::ULong i = 0; i != data.length(); ++i) {
			const RtecEventComm::Event &e = data[i];
			onAnyEvent(e.data.any_value);
		}
	}
	
	virtual void onAnyEvent(const CORBA::Any & event) = 0;

	void disconnect() {
		mLog.v() << "AnyEventConsumer::disconnect()" << testutil::done;
		mSupplierProxy->disconnect_push_supplier();
		PortableServer::POA_ptr poa = mNativeWrapper->getPoa();
		PortableServer::ObjectId_var id = poa->reference_to_id(mMyself.ptr());
		poa->deactivate_object(id);
		mMyself = RtecEventComm::PushConsumer::_nil();
	}

	void disconnect_push_consumer() {
		mLog.v() << "disconnect_push_consumer()" << testutil::done;
		mSupplierProxy = RtecEventChannelAdmin::ProxyPushSupplier::_nil();
	}
	
protected:
	testutil::Log mLog;
	
private:
	CORBA::ULong mType;
	RtecEventChannelAdmin::ProxyPushSupplier_var mSupplierProxy;
	RtecEventComm::PushConsumer_var mMyself;
	NativeWrapper * mNativeWrapper;
};

} /* namespace corbabinding */

#endif /* SRC_COMMON_ANYEVENTCONSUMER_H_ */
