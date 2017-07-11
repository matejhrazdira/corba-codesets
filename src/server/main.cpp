#include "../gen/testS.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

#include "../common/EventProducer.h"
#include "../common/NativeWrapper.h"
#include "../common/Token.h"

#include "../common/common.h"

using namespace corbabinding;

class TestEventProducer : public corbabinding::EventProducer<test::SomeEvent> {
public:
	TestEventProducer() : EventProducer(1, test::EVENT_ID) {}
};

class ServerImpl: public virtual POA_test::Server {
public:
	ServerImpl(Token & token) : log(getLog("ServerImpl")), mToken(token), mEvProducer() {}

	virtual char * getCp850String() {
		CORBA::String_var result((const char *) cp850bytes);
		return result._retn();
	}

	virtual char * getUtf8String() {
		CORBA::String_var result((const char *) utf8bytes);
		return result._retn();
	}

	virtual char * getIso88591String() {
		CORBA::String_var result((const char *) isobytes);
		return result._retn();
	}

	virtual void pushUtf8String() {
		test::SomeEvent ev;
		ev.encoding = "UTF-8";
		ev.testString = getUtf8String();
		mEvProducer.push(ev);
	}

	virtual void pushCp850String() {
		test::SomeEvent ev;
		ev.encoding = "CP850";
		ev.testString = getCp850String();
		mEvProducer.push(ev);
	}

	virtual void pushIso88591String() {
		test::SomeEvent ev;
		ev.encoding = "ISO 8859-1";
		ev.testString = getIso88591String();
		mEvProducer.push(ev);
	}

	void connect(NativeWrapper & wrapper) {
		mEvProducer.connect(wrapper);
	}

	void disconnect() {
		mEvProducer.disconnect();
	}

	virtual ~ServerImpl(void) {}
private:
	Log log;
	Token & mToken;
	TestEventProducer mEvProducer;
};

int ACE_TMAIN(int argc, ACE_TCHAR* argv[]) {
	Log log = getLog("main");
	log.v() << "SERVER started" << done;
	try {
		Token t;
		Lock lock(t);
		ServerImpl serverImpl(t);
		NativeWrapper wrapper(argc, argv);
		serverImpl.connect(wrapper);
		test::Server_var server = serverImpl._this();
		wrapper.bindName("TestServer", server.in());
		t.wait();
		serverImpl.disconnect();
		log.i() << "exiting" << done;
	} catch (const CORBA::Exception & e) {
		log.e() << "exception " << e << done;
	}
	log.i() << "exit" << done;
	return 0;
}
