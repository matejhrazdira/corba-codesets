#include "../gen/testC.h"
#include <orbsvcs/CosNamingC.h>
#include <orbsvcs/RtecEventCommS.h>
#include <orbsvcs/RtecEventChannelAdminC.h>
#include <orbsvcs/Event_Utilities.h>
#include <ace/streams.h>
#include <ace/Thread_Manager.h>

#include "../common/EventConsumer.h"
#include "../common/NativeWrapper.h"
#include "../common/Token.h"
#include "../common/common.h"

using namespace corbabinding;

class MyEventConsumer: public EventConsumer<test::SomeEvent> {
public:
	MyEventConsumer() :
			EventConsumer(test::EVENT_ID), log(getLog("MyEventConsumer")) {
	}

	void onEvent(test::SomeEvent * event) {
		log.i() << "Got event with encoding '" << event->encoding.in() << "' and raw content : " << asHex(event->testString.in(), strlen(event->testString.in())) << done;
	}
private:
	Log log;
};

int ACE_TMAIN(int argc, ACE_TCHAR* argv[]) {
	Log log = getLog("main");
	log.i() << "CLIENT started" << done;
	try {
		NativeWrapper wrapper(argc, argv);

		test::Server_var server = wrapper.resolveName("TestServer", (test::Server *) nullptr);

		CORBA::String_var str;
		const char * bytes;

		str = server->getUtf8String();
		bytes = str.in();
		log.i() << "utf 8 (raw)        : " << asHex(utf8bytes, utf8bytesLen) << done;
		log.i() << "utf 8 (from server): " << asHex(bytes, strlen(bytes)) << done;

		str = server->getIso88591String();
		bytes = str.in();
		log.i() << "iso (raw)          : " << asHex(isobytes, isobytesLen) << done;
		log.i() << "iso (from server)  : " << asHex(bytes, strlen(bytes)) << done;

		str = server->getCp850String();
		bytes = str.in();
		log.i() << "cp850 (raw)        : " << asHex(cp850bytes, cp850bytesLen) << done;
		log.i() << "cp850 (from server): " << asHex(bytes, strlen(bytes)) << done;

		MyEventConsumer evConsumer;
		evConsumer.connect(&wrapper);

		server->pushUtf8String();
		ACE_OS::sleep(ACE_Time_Value(0, 500 * 1000));

		server->pushCp850String();
		ACE_OS::sleep(ACE_Time_Value(0, 500 * 1000));

		server->pushIso88591String();
		ACE_OS::sleep(ACE_Time_Value(0, 500 * 1000));

		evConsumer.disconnect();

		log.i() << "exiting" << done;
	} catch (const CORBA::Exception & e) {
		log.e() << "exception " << e << done;
	}
	log.i() << "exit" << done;
	return 0;
}
