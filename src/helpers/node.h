class Node
{
private:

public:
	Node* parent;
	std::vector<Node*> children;
	int depth = 0;

	GameState gameState;
	Card card;

	int ns = 0;
	double cumulative_reward = 0.0;
	double value;
	int number_rollouts = 0;

	bool isRoot = false;

	Node();
	Node(Node* parent);
	Node(Node* node, bool isCopy);
	Node(Node* parent, GameState state, Card card);
	~Node();
	void addChild(Node* child);
	void makeChild(Node* state);
	void copy(Node* node);
};

Node::Node()
{
	//do nothing
}

Node::Node(Node* parent)
{
	makeChild(parent);
}

Node::Node(Node* node, bool isCopy)
{
	if(isCopy)
	{
		this->parent = node->parent;
		this->depth = node->depth;

		this->gameState.copy(node->gameState);
		this->card = node->card;

		this->ns = node->ns;
		this->cumulative_reward = node->cumulative_reward;
		this->value = node->value;
	}
}

Node::Node(Node* parent, GameState state, Card card)
{
	this->parent = parent;
	this->gameState = state;
	this->card = card;
}

Node::~Node()
{
	for (Node* n : this->children)
		delete n;
	this->children.clear();
}

void Node::addChild(Node* child)
{
	child->depth = this->depth + 1;
	this->children.push_back(child);
}

void Node::makeChild(Node* state)
{
	this->parent = state;
	this->depth = state->depth + 1;

	this->gameState.copy(state->gameState);
	this->card = state->card;

	this->ns = state->ns;
	this->cumulative_reward = state->cumulative_reward;
	this->value = state->value;
}