#ifndef PCH_H
#define PCH_H

#include <array>
#include <sstream>
#include <fstream>
#include <intrin.h>
//#if defined(_WIN32) || defined(_W64)
//#include <sysinfoapi.h>
//#endif
#include <Bite.h>

constexpr size_t InvalidIndex = (size_t)-1;
constexpr size_t LastBit = (size_t)1<<(sizeof(size_t) * 8 - 1);

enum FILESeekOrigin { FILE_Begin, FILE_Current, FILE_End };

using bite::format;
using bite::formatv;
using bite::span;
using bite::raise;
using bite::warn;

union flaged_size_t
{
	size_t sz;
	struct
	{
		char ch[ sizeof(size_t) - 1 ];
		char flag;
	};
};
static_assert(sizeof(flaged_size_t) == sizeof(size_t), "flaged_size_t != size_t");

#endif //PCH_H