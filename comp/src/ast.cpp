#include "pch.h"
#include "ast.h"

SynNode *AbstractSynTree::find(ASTSearchProc search_proc)
{
	SynNode *cur = &m_root;
	while (cur)
	{
		switch (search_proc(cur))
		{
		case SearchReturn::Left:
			cur = cur->left.get();
			break;
		case SearchReturn::Found:
			return cur;
		case SearchReturn::Right:
			cur = cur->right.get();
			break;
		default:
			cur = nullptr;
		}
	}
	return cur;
}

const SynNode *AbstractSynTree::find(ASTSearchProc search_proc) const
{
	const SynNode *cur = &m_root;
	while (cur)
	{
		switch (search_proc(cur))
		{
		case SearchReturn::Left:
			cur = cur->left.get();
			break;
		case SearchReturn::Found:
			return cur;
		case SearchReturn::Right:
			cur = cur->right.get();
			break;
		default:
			cur = nullptr;
		}
	}
	return cur;
}
