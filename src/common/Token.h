#ifndef SRC_COMMON_TOKEN_H_
#define SRC_COMMON_TOKEN_H_

#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Condition_Thread_Mutex.h>

namespace corbabinding {

class Token {
public:
	typedef ACE_Thread_Mutex Mutex;
	typedef ACE_Condition_Thread_Mutex Condition;

	Token() : mMutex(), mCondition(mMutex) {}
	Mutex & getMutex() { return mMutex; }
	void broadcast() { mCondition.broadcast(); }
	void wait() { mCondition.wait(); }
private:
	Mutex mMutex;
	Condition mCondition;
};

class Lock {
public:
	Lock(Token & token) : mLock(token.getMutex()) {}
private:
	ACE_Guard<Token::Mutex> mLock;
};

} /* namespace corbabinding */

#endif /* SRC_COMMON_TOKEN_H_ */
