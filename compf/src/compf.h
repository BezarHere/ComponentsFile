#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#define COMPF_LINK extern

namespace compf
{

	typedef struct { std::shared_ptr<uint8_t[]> buf; size_t len; } CompileCompfBuffer_t;

	class Value;
	union ValueUnion
	{
		union _large {
			std::unordered_map<std::string, Value> ds;
			std::unordered_map<int64_t, Value> di;
			std::vector<Value> v;
		};
		std::shared_ptr<_large> large;
		std::string str;
		double_t f; // can be also float, clamped
		int64_t i; // can be also bool, char, int
	};

	enum class ValueType
	{
		None,
		Int,
		Float,
		String,
		Dictionary,
		Table,
		Array
	};

	class Value
	{
	public:

	private:
		ValueType m_type;
		ValueUnion m_union;
	};



	COMPF_LINK std::vector<Value> decompile(const CompileCompfBuffer_t &bin);
	COMPF_LINK CompileCompfBuffer_t compile(const std::vector<Value> &data);
	COMPF_LINK CompileCompfBuffer_t compile(const std::string &src);

	COMPF_LINK std::vector<Value> decompile_file(const CompileCompfBuffer_t &bin);
	COMPF_LINK CompileCompfBuffer_t compile_file(const std::string &filepath);

	COMPF_LINK std::vector<Value> read(const std::string &src);
	COMPF_LINK std::vector<Value> load(const std::string &filepath);

}

#undef COMPF_LINK