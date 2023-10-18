#include "pch.h"
#include "ast.h"

typedef const TkPage_t &const_tkpage_ref;
typedef const Tk *const_Ptk;

CINLINE bool is_operator_type(const TkType t)
{
	return t > TkType::Operators;
}

FORCEINLINE size_t next_index_explicit_tk(const_Ptk tk, const size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		if (tk[ i ].type != TkType::Whitespace)
			return i;
	}
	return InvalidIndex;
}

FORCEINLINE size_t next_index_explicit_tk_lined(const_Ptk tk, const size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		const auto t = tk[ i ].type;
		if (t != TkType::Whitespace)
			return i;
	}
	return InvalidIndex;
}

FORCEINLINE span<size_t> mark_expression_start_tk_indexes(const_Ptk tk, const size_t n)
{
	std::vector<size_t> ind;
	ind.reserve(1024);



	return span<size_t>{ ind };
}

FORCEINLINE void _parse_expression(const_Ptk tk, const size_t n, SynState &state)
{
	for (size_t i = 0; i < n; i++)
	{
		const Tk &t = tk[ i ];

		switch (t.type)
		{
		case TkType::Identifier:
			{
				const size_t k = next_index_explicit_tk(tk + i, n - i);

				// No more explicit tks in cur line
				if (k == InvalidIndex)
				{
					//raise(format("Expected expression at line {} but EOF reached", t.pos.line));
				}

				const Tk &kt = tk[ k ];

				if (kt.type == TkType::Newline)
				{
					raise("Expected expression after identifier at " + std::to_string(t.pos));
				}

				switch (kt.type)
				{
				case TkType::Equal:
					nodes.emplace_back();
					break;
				default:
					raise("Unexpected token after identifier at " + std::to_string(tk[ k ].pos));
				}
			}
			break;
			// skip ws
		case TkType::Whitespace:
			break;
		default:
			break;
		}

	}

}

FORCEINLINE void _parse_block(const_Ptk tk, const size_t n, SynState &state)
{
	for (size_t i = 0; i < n; i++)
	{
		const Tk &t = tk[ i ];

		switch (t.type)
		{
		case TkType::Identifier:
			{
				const size_t k = next_index_explicit_tk_lined(tk + i, n - i);

				// No more explicit tks in cur line
				if (k == InvalidIndex)
				{
					raise(format("Expected expression at line {} but EOF reached", t.pos.line));
				}
				
				const Tk &kt = tk[ k ];

				if (kt.type == TkType::Newline)
				{
					raise(format("Expected expression after identifier at line {}", tk[ k ].pos.line));
				}

				switch (kt.type)
				{
				case TkType::Assign:
					nodes.emplace_back();
					break;
				default:
					raise(format("Unexpected token after identifier at {}", std::to_string(tk[ k ].pos)));
				}

				
			}
			break;
		default:
			break;
		}

	}
}


FORCEINLINE SynNode parse(const_tkpage_ref tks)
{
	return { SynType::ScriptBody, _parse_block(tks.data(), tks.size()) };
}

AbstractSynTree::AbstractSynTree(const_tkpage_ref page)
	: m_root{ parse(page) }
{
}
