#pragma once
#include "utils.h"
#include <array>
#include <string>

#define REG_KEYWORD(n) { {n} }

constexpr size_t FixedstrMaxLen = 16;
constexpr size_t FixedstrLen = 8;

typedef std::array<char, FixedstrMaxLen> FixedStrData_t;
typedef uint32_t FSHash_t;

CINLINE size_t fsd_length(const FixedStrData_t &data)
{
	for (size_t i = 1; i < FixedstrMaxLen; i++)
	{
		if (!data[ i ])
			return i;
	}
	return FixedstrMaxLen;
}

// just reverse mapping chars to uint32_t
CINLINE FSHash_t fs_hash(const char *v, const size_t n)
{
	if (n >= 32)
		return (FSHash_t)-1;
	FSHash_t hash{};

	for (uint32_t i{}; i < n; i++)
		hash += v[ i ] << (n - i) * 2;

	return hash;
}

struct FixedStr
{
	constexpr FixedStr(const FixedStrData_t v)
		: data{ v }, len{ fsd_length(v) }, hash{ fs_hash(v._Elems, len) }
	{
	}

	FixedStrData_t data;
	size_t len; // without the zero termination
	FSHash_t hash;
};

constexpr FixedStr Keywords[]
{
	REG_KEYWORD("proc"),

	REG_KEYWORD("if"),
	REG_KEYWORD("elif"),
	REG_KEYWORD("else"),
	REG_KEYWORD("return"),

	REG_KEYWORD("sin"),
	REG_KEYWORD("cos"),
	REG_KEYWORD("tan"),
	REG_KEYWORD("asin"),
	REG_KEYWORD("acos"),
	REG_KEYWORD("sign"),
};
constexpr size_t KeywordsCount = sizeof(Keywords) / sizeof(FixedStr);

CINLINE size_t keyword_index(const char *v, const size_t n)
{
	if (n >= FixedstrMaxLen)
		return false;
	const FSHash_t h = fs_hash(v, n);
	for (size_t i{}; i < KeywordsCount; i++)
	{
		if (Keywords[ i ].hash == h)
			return i;
	}
	return InvalidIndex;
}

enum class TkType : short
{
	Unknown = 0,
	Identifier,

	Newline,
	Whitespace,

	Number,
	Number_Hex = Number,
	Number_Dec,
	Number_Bin,

	EscapedSequnce,

	Keywords, // marker type, shouldn't apear
	KW_Proc = Keywords,

	KW_If,
	KW_Elif,
	KW_Else,
	KW_Return,
	KW_Sin,
	KW_Cos,
	KW_Tan,
	KW_Asin,
	KW_Acos,
	KW_Sign,


	Operators, // marker type, shouldn't apear

	ParenthseseOpen = '(',
	ParenthseseClose = ')',
	SqbracketOpen = '[',
	SqbracketClose = ']',
	BracketOpen = '{',
	BracketClose = '}',

	ExclamationMark = '!',
	//Bool = '!!',

	Ambersand = '&',
	And = '&&',

	Bar = '|',
	Or = '||',

	GreaterThen = '>',
	GreaterThenOrEqual = '>=',
	RightShift = '>>',

	SmallerThen = '<',
	SmallerThenOrEqual = '<=',
	LeftShift = '<<',

	Assign = '==',
	Equal = '=',

	//Dec = '--',
	Minus = '-',

	//Inc = '++',
	Plus = '+',

	Star = '*',

	FSlash = '/',


};

CINLINE TkType get_keyword_tk(const size_t index)
{
	if (index == InvalidIndex)
		return TkType::Identifier;
	return TkType((size_t)TkType::Keywords + index);
}

struct SrcPos
{
	static constexpr uint16_t nPos = (uint16_t)-1;
	uint16_t line, colom;
};

struct Tk
{
	Tk()
	{}

	Tk(TkType t, std::string s, SrcPos p)
		: type{ t }, str{ s }, pos{p} {}

	TkType type = TkType::Unknown;
	std::string str{};
	SrcPos pos{ SrcPos::nPos, SrcPos::nPos };
};

namespace std
{
	FORCEINLINE std::string to_string(SrcPos _Val)
	{
		return std::to_string(_Val.line) + ":" + std::to_string(_Val.colom);
	}
}

typedef std::vector<Tk> TkPage_t;

namespace std
{
	FORCEINLINE std::ostream &operator<<(std::ostream &stream, const Tk &tk)
	{
		return stream << "{ " << tk.pos.line << ", " << tk.pos.colom << "} " << (int)tk.type << ": \"" << tk.str << "\"\n";
	}

}
