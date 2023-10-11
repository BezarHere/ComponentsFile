#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#define COMP_LINK extern

namespace comp
{

	typedef struct { std::shared_ptr<uint8_t[]> buf; size_t len; } CompileCompfBuffer_t;

	class Value;

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

	class Value final
	{
	public:
		~Value()
		{
			switch (m_type)
			{
			case ValueType::None:
			case ValueType::Int:
			case ValueType::Float:
				break;
			case ValueType::String:
				m_union.str.~basic_string();
				break;
			case ValueType::Dictionary:
				m_union.ds.~shared_ptr();
				break;
			case ValueType::Table:
				m_union.di.~shared_ptr();
				break;
			case ValueType::Array:
				m_union.v.~shared_ptr();
				break;
			default:
				break;
			}
		}

	private:
		ValueType m_type;
		union _uV
		{
			~_uV() {}
			std::shared_ptr<std::unordered_map<std::string, Value>> ds;
			std::shared_ptr< std::unordered_map<int64_t, Value>> di;
			std::shared_ptr<std::vector<Value>> v;
			std::string str;
			double_t f; // can be also float, clamped
			int64_t i; // can be also bool, char, int
		} m_union;
	};



	COMP_LINK std::vector<Value> decompile(const CompileCompfBuffer_t &bin);
	COMP_LINK CompileCompfBuffer_t compile(const std::vector<Value> &data);
	COMP_LINK CompileCompfBuffer_t compile(const std::string &src);

	COMP_LINK std::vector<Value> decompile_file(const CompileCompfBuffer_t &bin);
	COMP_LINK CompileCompfBuffer_t compile_file(const std::string &filepath);

	COMP_LINK std::vector<Value> read(const std::string &src);
	COMP_LINK std::vector<Value> load(const std::string &filepath);

}

#undef COMP_LINK