#include "Tree.h"

Tree::Tree() :m_depth(0), m_nodeCount(0), m_root(nullptr), m_treeString("")
{
}

Tree::Tree(int depth) : m_depth(depth), m_nodeCount(0), m_root(nullptr), m_treeString("")
{
	m_root = m_buildTree(m_root, depth, 0);
	m_root->isRoot = true;
}

Tree::~Tree()
= default;

std::string Tree::toString()
{
	if (m_treeString.empty())
	{
		m_treeString = m_buildTreeString(m_root);
	}

	return m_treeString;
}

std::string Tree::m_buildTreeString(Node * node)
{
	std::string treeString = node->toString();
	if (!node->isLeaf)
	{
		treeString += "\n"; // adds a new line
		treeString += m_buildTreeString(node->left);
		treeString += " ";
		treeString += m_buildTreeString(node->right);

	}
	/*else
	{
		treeString += " " + node->parent->right->toString();
	}*/

	return treeString;
}

Node* Tree::m_buildTree(Node * node, const int depth, const int id)
{
	node = new Node();
	node->id = id;

	// base case
	if (depth > 0)
	{
		// build left sub-tree
		node->left = m_buildTree(node->left, depth - 1, (id * 2) + 1);

		// build right sub-tree
		node->right = m_buildTree(node->right, depth - 1, (id * 2) + 2);

		node->left->parent = node;
		node->right->parent = node;
	}
	else
	{
		node->isLeaf = true;
	}

	m_nodeCount++;

	return node;
}