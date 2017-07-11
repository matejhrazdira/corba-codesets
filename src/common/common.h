#ifndef SRC_COMMON_COMMON_H_
#define SRC_COMMON_COMMON_H_

#include "TestUtil.h"

#include <ace/String_Base.h>
#include <tao/Exception.h>

#include <iostream>

using namespace testutil;

inline Log getLog(const std::string & tag) { return Log(tag, std::cout, std::cout, std::cerr); }

inline StringBuilder & operator<<(StringBuilder & sb, const CORBA::Exception & e) {
	return sb << "CORBA::Exception: " << e._name() << " /// " << e._info().c_str() << " /// " << e._rep_id();
}

extern unsigned char cp850bytes[];
extern int cp850bytesLen;

extern unsigned char utf8bytes[];
extern int utf8bytesLen;

extern unsigned char isobytes[];
extern int isobytesLen;

#endif /* SRC_COMMON_COMMON_H_ */
