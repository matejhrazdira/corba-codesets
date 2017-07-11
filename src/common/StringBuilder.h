#ifndef UTILS_STRINGBUILDER_H_
#define UTILS_STRINGBUILDER_H_

#include <string>
#include <sstream>
#include <iomanip>

namespace testutil {

template <typename T> class IterableWrapper;
template <typename T, typename U> class ArrayWrapper;
template <typename T, typename U> class HexWrapper;

class StringBuilder {
public:
	StringBuilder() {}

	template<typename T> inline StringBuilder & operator<<(const T &value) {
		mBuffer << value;
		return *this;
	}

	template<typename T> inline StringBuilder & operator<<(const IterableWrapper<T> & value) {
		value.append(*this);
		return *this;
	}

	template<typename T, typename U> inline StringBuilder & operator<<(const ArrayWrapper<T, U> & value) {
		value.append(*this);
		return *this;
	}

	template<typename T, typename U> inline StringBuilder & operator<<(const HexWrapper<T, U> & value) {
		value.append(*this);
		return *this;
	}

	template<typename T> inline T operator>>(T (*func)(const StringBuilder &)) {
		return func(*this);
	}

	inline const std::string str() const {
		return mBuffer.str();
	}

	inline const char * c_str() const {
		mTmpStr = mBuffer.str();
		return mTmpStr.c_str();
	}
private:
	std::stringstream mBuffer;
	mutable std::string mTmpStr;
};

inline const std::string str(const StringBuilder & sb) {
	return sb.str();
}

inline const char * c_str(const StringBuilder & sb) {
	return sb.c_str();
}

template <typename T>
class IterableWrapper {
public:
	IterableWrapper(const T & iterable, const std::string & separator) : mIterable(iterable), mSeparator(separator) {}
	void append(StringBuilder & builder) const {
		auto endIt = mIterable.end();
		auto it = mIterable.begin();
		if (it != endIt) {
			builder << *(it++);
		}
		while (it != endIt) {
			builder << mSeparator << *(it++);
		}
	}
private:
	const T & mIterable;
	const std::string & mSeparator;
};

template <typename T> inline IterableWrapper<T> byIterator(const T & iterable, const std::string & separator = ", ") {
	return IterableWrapper<T>(iterable, separator);
}

template <typename T, typename U>
class ArrayWrapper {
public:
	ArrayWrapper(const T & array, const U lenght, const std::string & separator) : mArray(array), mLength(lenght), mSeparator(separator) {}
	void append(StringBuilder & builder) const {
		if (mLength > 0) {
			builder << mArray[0];
		}
		for (U i = 1; i < mLength; i++) {
			builder << mSeparator << mArray[i];

		}
	}
private:
	const T & mArray;
	const U mLength;
	const std::string & mSeparator;
};

template <typename T, typename U> inline ArrayWrapper<T, U> byIndex(const T & array, const U length, const std::string & separator = ", ") {
	return ArrayWrapper<T, U>(array, length, separator);
}

template <typename T, typename U>
class HexWrapper {
public:
	HexWrapper(const T & array, const U length) : mArray(array), mLength(length) {}
	void append(StringBuilder & output) const {
		output << std::hex << std::setfill('0');
		for (U i = 0; i < mLength; i++) {
			output << std::setw(2) << (mArray[i] & 0xff);
		}
	}
private:
	const T & mArray;
	const U mLength;
};

template <typename T, typename U> inline HexWrapper<T, U> asHex(const T & array, const U length) {
	return HexWrapper<T, U>(array, length);
}

} /* namespace testutil */

#endif /* UTILS_STRINGBUILDER_H_ */
