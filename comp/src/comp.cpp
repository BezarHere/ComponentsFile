#include "pch.h"
#include "comp.h"
#include "ast.h"
#include "tk.h"

struct TkWriter
{
	inline TkWriter()
	{
		tks.reserve(1024);
	}

	inline void push_back(const Tk &tk)
	{
		tks.push_back(tk);
		pos.colom += (uint32_t)tk.str.length();
	}

	inline void push_back(const TkType t, const std::string &str)
	{
		tks.emplace_back(t, str, pos);
		pos.colom += (uint32_t)str.length();
	}

	TkPage_t tks{ };
	SrcPos pos{ 0, 0 };
};

long fsize(FILE *_f)
{
	long cur = ftell(_f);
	fseek(_f, 0, FILE_END);
	const long sz = ftell(_f);
	fseek(_f, cur, FILE_BEGIN);
	return sz;
}

std::string read_all_text(const std::string &path)
{
	FILE *f = nullptr;
	errno_t err = fopen_s(&f, path.c_str(), "r");
	
	if (err == -1)
		bite::warn("Error while reading all text from \"" + path + "\"");

	if (!f)
		return "";

	const size_t size = fsize(f);
	char *buf = new char[size + 1] {};
	fread_s(buf, size + 1, 1, size, f);
	fclose(f);
	return std::string(buf);
}

TkPage_t detokenize(const std::string &src)
{
	TkWriter tks;

	for (StringReader reader{ src }; reader;)
	{
		const char c = reader.read();

		if (is_letter(c) || c == '_')
		{
			const size_t anchor = reader.get_index() - 1;

			while (reader)
			{
				const char c2 = reader.read();

				if (!is_valid_identifier_continution(c2))
					break;
			}
			reader.retreat();

			const size_t n = reader.get_index() - anchor;
			const std::string s = reader.str(anchor, n);

			tks.push_back(get_keyword_tk(keyword_index(reader.current(), n)), s);
			continue;
		}

		if (is_digit(c))
		{
			const size_t anchor = reader.get_index() - 1;
			NumberBase base = NumberBase::Decimal;
			const bool is_first_digit_zero = c == '0';

			while (reader)
			{
				const char c2 = reader.read();

				if (is_digit(c2))
					continue;

				if (is_first_digit_zero && reader.get_index() - anchor == 1)
				{
					if (c2 == 'h')
						base = NumberBase::Hex;
					else if (c2 == 'b')
						base = NumberBase::Binary;
					else if (c2 == 'd')
						base = NumberBase::Decimal;
					else
						bite::raise("Unexpedted expression in number");
					continue;
				}

				break;
			}
			reader.retreat();

			const size_t n = reader.get_index() - anchor;

			tks.push_back(TkType((int)TkType::Number + (int)base), reader.str(anchor, n));
			continue;
		}

		if (c == '\\')
		{
			const size_t anchor = reader.get_anchor();
			if (!reader.march())
				bite::raise("Escaped EOF");
			
			tks.push_back(TkType::EscapedSequnce, reader.str(anchor, 2));
			continue;
		}

		if (is_whitespace(c))
		{
			const size_t anchor = reader.get_anchor();
			const bool is_newline = c == '\n';

			if (is_newline)
			{
				while (reader)
				{
					if (reader.read() != '\n')
						break;
				}
			}
			else
			{
				while (reader)
				{
					if (!is_whitespace(reader.read()))
						break;
				}
			}
			reader.retreat();
			
			const size_t n = reader.get_index() - anchor;


			tks.push_back(is_newline ? TkType::Newline : TkType::Whitespace, reader.str(anchor, n));

			if (is_newline)
			{
				tks.pos.line += (uint32_t)n;
				tks.pos.colom = 0;
			}
			continue;
		}

		if (reader.peek_next() == c)
		{
			const size_t anchor = reader.get_index() - 1;
			const int id = (int(c) << 8) + (int)reader.read();
			tks.push_back((TkType)id, reader.str(anchor, 2));
		}
		else
		{
			tks.push_back((TkType)int(c), std::string{ c });
		}
	}

	return tks.tks;
}

namespace comp
{

	std::vector<Value> decompile(const CompileCompfBuffer_t &bin)
	{

		return std::vector<Value>();
	}

	std::vector<Value> read(const std::string &src)
	{
		TkPage_t v = detokenize(src);

		std::cout << v.size() << '\n';
		for (const auto &p : v)
			std::cout << p << std::endl;

		AbstractSynTree tree{ v };

		return std::vector<Value>();
	}

	std::vector<Value> load(const std::string &filepath)
	{
		return read(read_all_text(filepath));
	}

}
