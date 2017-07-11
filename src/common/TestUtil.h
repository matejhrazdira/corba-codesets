#ifndef UTILS_TESTUTIL_H_
#define UTILS_TESTUTIL_H_

#include "StringBuilder.h"

#include <ace/ACE.h>
#include <sstream>
#include <string>

namespace testutil {

typedef unsigned long long timestamp;

#ifdef __linux__
inline pid_t getTid() {
	return syscall(__NR_gettid); // less portable, more readable output than ACE_OS::thr_self();
}
#else // __linux__
inline ACE_thread_t getTid() {
	return ACE_OS::thr_self();
}
#endif // __linux__

inline pid_t getPid() {
	return ACE_OS::getpid();
}

inline timestamp currentTimeMillis() {
	return ACE_OS::gettimeofday().msec();
}

inline timestamp currentTimeMicros() {
	ACE_Time_Value now = ACE_OS::gettimeofday();
	return now.sec() * 1000LL * 1000LL + now.usec();
}

std::string currentTime();

class LogBuilder {
public:
	LogBuilder(std::ostream & target) : mBuffer(), mTarget(target) {}

	LogBuilder(std::ostream & target, const char * header) : LogBuilder(target) {
		mBuffer << header;
	}

	template<typename T> LogBuilder& operator<<(const T &value) {
		mBuffer << value;
		return *this;
	}

	LogBuilder & operator<<(LogBuilder & (*manipulator)(LogBuilder &)) {
		return manipulator(*this);
	}

	void flush() {
		mTarget << mBuffer.str() << std::flush;
	}
private:
	StringBuilder mBuffer;
	std::ostream & mTarget;
};

inline LogBuilder & done(LogBuilder & builder) {
	builder << '\n';
	builder.flush();
	return builder;
}

class Log {
public:
	Log(const std::string & tag, std::ostream & out, std::ostream & warn, std::ostream & err) :
			mTag(tag), mOut(out), mWarn(warn), mErr(err) {}
	LogBuilder v() { return log("V", mOut); }
	LogBuilder d() { return log("D", mOut); }
	LogBuilder i() { return log("I", mOut); }
	LogBuilder w() { return log("W", mWarn); }
	LogBuilder e() { return log("E", mErr); }
private:
	std::string mTag;
	std::ostream & mOut;
	std::ostream & mWarn;
	std::ostream & mErr;

	LogBuilder log(const char * level, std::ostream & output) {
		char header[128];
		sprintf(header, "%s    %5d / %5d    %s    %-36s",
				currentTime().c_str(), getPid(), getTid(), level, mTag.c_str());
		return LogBuilder(output, header);
	}
};

}

#endif /* UTILS_TESTUTIL_H_ */
