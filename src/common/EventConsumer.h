#ifndef SRC_COMMON_EVENTCONSUMER_H_
#define SRC_COMMON_EVENTCONSUMER_H_

#include "AnyEventConsumer.h"

namespace corbabinding {

template<typename T>
class EventConsumer : public AnyEventConsumer {
public:
	EventConsumer(CORBA::ULong type) : AnyEventConsumer(type) {}

	void onAnyEvent(const CORBA::Any & e) {
		T * event;
		if ((e >>= event) == 0) {
			mLog.w() << "received invalid event" << testutil::done;
		} else {
			onEvent(event);
		}
	}

	virtual void onEvent(T * event) = 0;
};

} /* namespace corbabinding */

#endif /* SRC_COMMON_EVENTCONSUMER_H_ */
