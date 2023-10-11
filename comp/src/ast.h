#pragma once
#include <memory> 
#include "comp.h"

enum class SynType
{

};

enum class SearchReturn
{
	Left = -1,
	Found = 0,
	Right = 1,
};

struct SynNode
{
	SynType type;
	comp::Value value;
	std::shared_ptr<SynNode> left, right;
};


typedef SearchReturn(*ASTSearchProc)(const SynNode *const);

class AbstractSynTree
{
public:

	SynNode *find(ASTSearchProc search_proc);
	const SynNode *find(ASTSearchProc search_proc) const;

private:
	SynNode m_root;
};
