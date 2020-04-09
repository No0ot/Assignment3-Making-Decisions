#include "Node.h"

Node::Node() :id(0), data(0), parent(nullptr),
left(nullptr), right(nullptr), isRoot(false), isLeaf(false)
{
}

Node::~Node()
= default;

std::string Node::toString()
{
	return "(" + std::to_string(id) + ")";
}