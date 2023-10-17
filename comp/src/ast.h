#pragma once
#include <memory> 
#include "comp.h"
#include "tk.h"

enum class SynType
{
	None = 0,
	ScriptBody,
	ProcedureDefinition,
	ProcedureBody,
	Subexpression,

	IfStatment, // [if_condition, ...code]
	IfCondition,

	ElseStatment, // [else_condition, ...code]
	ElseCondition,

	Retrun, // [...code (expression)]

	Assigen,

	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Pow,

	Neg,

	AddThenAssign,
	SubThenAssign,
	MulThenAssign,
	DivThenAssign,
	PowThenAssign,
	ModThenAssign,

	BitAdd,
	BitOr,
	BitXor,
	BitNot,

	And,
	Or,
	Not,

	Equal,
	NotEqual,

	LessThen,
	LessEqThen,

	GreaterThen,
	GreaterEqThen,

	DictDecl,
	DcitItem,
	TableDecl,
	TableItem,
	ArrayDecl
};

enum class SearchReturn
{
	Left = -1,
	Found = 0,
	Right = 1,
};
 
typedef size_t CStackIndex_t;
typedef size_t StackIndex_t;
struct SynNode
{
	FORCEINLINE SynNode()
		: type{ SynType::None }
	{}

	FORCEINLINE SynNode(SynType t)
		: type{ t }
	{}

	FORCEINLINE SynNode(SynType t, std::vector<SynNode> c)
		: type{ t }, children{ c }
	{}

	SynType type;
	CStackIndex_t value;
	std::vector<SynNode> children;
};

typedef SearchReturn(*ASTSearchProc)(const SynNode *const);

class AbstractSynTree
{
public:
	AbstractSynTree(const TkPage_t &page);
private:
	SynNode m_root;
};
