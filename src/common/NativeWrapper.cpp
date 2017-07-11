#include "NativeWrapper.h"

#include "../common/common.h"

using namespace testutil;

namespace corbabinding {

NativeWrapper::NativeWrapper(int argc, ACE_TCHAR * argv[]) : mLog(getLog("NativeWrapper")) {
	mLog.v() << "initializing ORB: " << byIndex(argv, argc) << done;
	mOrb = CORBA::ORB_init(argc, argv);

	mLog.v() << "initializing POA" << done;
	CORBA::Object_ptr poaObject = mOrb->resolve_initial_references("RootPOA");
	mPoa = PortableServer::POA::_narrow(poaObject);
	mPoaManager = mPoa->the_POAManager();
	mPoaManager->activate();

	mLog.v() << "resolving initial references" << done;
	CORBA::Object_var naming_context_object = mOrb->resolve_initial_references("NameService");
	mNameService = CosNaming::NamingContext::_narrow(naming_context_object.in());

	mLog.v() << "resolving event service" << done;
	CosNaming::Name name(1);
	name.length(1);
	name[0].id = CORBA::string_dup("EventService");
	CORBA::Object_var ec_object = mNameService->resolve(name);
	mEventService = RtecEventChannelAdmin::EventChannel::_narrow(ec_object.in());

	mLog.v() << "starting ORB thread" << done;
	mOrbThread = 0;
	mThreadManager.spawn(runOrb, this);
}

NativeWrapper::~NativeWrapper() {
	mLog.v() << "deactivating POA" << done;
	mPoaManager->deactivate(true, true);
	mLog.v() << "shutting down ORB" << done;
	mOrb->shutdown(true);
	mLog.v() << "destroying ORB" << done;
	mOrb->destroy();
	mLog.v() << "joining threads" << done;
	mThreadManager.wait();
	mLog.v() << "destructor finished" << done;
}

void * NativeWrapper::runOrb(void * arg) {
	NativeWrapper * instance = (NativeWrapper *) arg;
	instance->mOrbThread = ACE_OS::thr_self();
	instance->mLog.v() << "starting ORB" << done;
	instance->mOrb->run();
	instance->mLog.v() << "ORB finished" << done;
	return 0x0;
}

} /* namespace corbabinding */
