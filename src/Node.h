#pragma once
#ifndef __NODE__
#define __NODE__
#include <string>

struct Node
{
	Node();
	~Node();

	int id; // handle
	int data; // some data

	Node* parent;
	Node* left;
	Node* right;

	bool isRoot;
	bool isLeaf;

	std::string toString();
};

#endif /* defined (__NODE__) */
