#include "node.h"
#include <stdlib.h>

using namespace std;

class Tree
{
private:

public:
	Node* root;
	// int node_count = 1;
	std::vector<Node*> nodes;

	Tree(GameState startState);
	~Tree();
	void add(Node* parent, Node* child);
	bool contains(Node* child);
	std::vector<Node*> treeNodesAtDepth(int depth);
};

Tree::Tree(GameState startState)
{
	this->root = new Node(nullptr, startState, Card());
	this->root->isRoot = true;
	nodes.push_back(this->root);
}

Tree::~Tree()
{
	delete this->root;
	this->nodes.clear();
}

void Tree::add(Node* parent, Node* child)
{
	parent->addChild(child);
	this->nodes.push_back(child);
	// this->node_count++;
}

bool Tree::contains(Node* child)
{
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		if (child->card.cardId == nodes[i]->card.cardId && child->parent == nodes[i]->parent)
			return true;
	}

	return false;
}

std::vector<Node*> Tree::treeNodesAtDepth(int depth)
{
	std::vector<Node*> dnodes;
	for (auto n : this->nodes)
	{
		if(n->number_rollouts == depth)
			dnodes.push_back(n);
	}

	return dnodes;
}