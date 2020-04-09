#pragma once
#ifndef __TREE__
#define __TREE__

#include "Node.h"
#include <string>

class Tree
{
public:
	Tree();
	Tree(int depth);
	~Tree();

	std::string toString();

private:
	int m_depth;
	int m_nodeCount;
	Node* m_root;
	std::string m_treeString;

	std::string m_buildTreeString(Node* node);
	Node* m_buildTree(Node* node, int depth, int id);
};
#endif /* defined (__TREE__) */