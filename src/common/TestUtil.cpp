#include "TestUtil.h"

#include <ace/ACE.h>
#include <ace/OS_NS_time.h>

namespace testutil {

std::string currentTime() {
	static const char * format = "%Y-%m-%d %H:%M:%S";
    static int offset = 4+1+2+1+2+1+2+1+2+1+2;
    char buff[32];
    ACE_Time_Value now = ACE_OS::gettimeofday();
    tm localTime;
    time_t seconds = now.sec();
    ACE_OS::localtime_r(&seconds, &localTime);
    ACE_OS::strftime(buff, sizeof(buff), format, &localTime);
    sprintf(buff + offset, ".%06ld", now.usec());
    return buff;
}

}
