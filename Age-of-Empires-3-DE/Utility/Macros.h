#pragma once
#include <cstdint>
// Classes
#define STR_MERGE_IMPL(X, Y)					X##Y
#define STR_MERGE(X, Y)							STR_MERGE_IMPL(X, Y)
#define MAKE_PAD(Size)							BYTE STR_MERGE(pad_, __COUNTER__) [ Size ]
//#define DEFINE_MEMBER_CLASS
//#define DEFINE_MEMBER_CLASS_INHERITANCE
#define DEFINE_MEMBER(Type, Name)				Type Name
#define DEFINE_MEMBER_0(Offset, X)				X
#define DEFINE_MEMBER_N(Offset, X)				struct { MAKE_PAD(Offset); X; }
#define DEFINE_MEMBER_B(Type, Name)				DEFINE_MEMBER_N(DEFINE_MEMBER_CLASS::Name, Type Name)

namespace Engine
{
	// rotate left
	template<class T> T __ROL__(T value, int count)
	{
		const unsigned int nbits = sizeof(T) * 8;

		if (count > 0)
		{
			count %= nbits;
			T high = value >> (nbits - count);
			if (T(-1) < 0)
				high &= ~((T(-1) << count));
			value <<= count;
			value |= high;
		}
		else
		{
			count = -count % nbits;
			T low = value << (nbits - count);
			value >>= count;
			value |= low;
		}
		return value;
	}

	unsigned int __ROL4__(int32_t value, unsigned int count) { return __ROL__((unsigned int)value, count); }
}

