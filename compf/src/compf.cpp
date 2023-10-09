#include "pch.h"
#include "compf.h"
#include "utils.h"

enum class TkType
{
	Unknown = 0,
	Identifier,

	ParenthseseOpen = '(',
	ParenthseseClose = ')',
	SqbracketOpen = '[',
	SqbracketClose = ']',
	BracketOpen = '{',
	BracketClose = '}',

	ExclamationMark = '!',
	Bool = '!!',

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

	Dec = '--',
	Minus = '-',

	Inc = '++',
	Plus = '+',

	Star = '*',

	FSlash = '/',

	BSlash = '\\',
	EscapedSlash = '\\\\',
};

struct Tk
{
	TkType type;
	std::string str;
};

typedef std::vector<Tk> TkPage_t;

TkPage_t &&detokenize(const std::string &src)
{
	TkPage_t tks(1024);

	for (StringReader reader{ src }; reader; reader.advance())
	{
		const char c = reader.read();


	}

	return std::move(tks);
}

namespace compf
{



	std::vector<Value> decompile(const CompileCompfBuffer_t &bin)
	{
	}

	std::vector<Value> read(const std::string &src)
	{

	}

}
