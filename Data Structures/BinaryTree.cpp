/*	Chaz Acheronti 2018	*/
/*	March 27 2018		*/
#include <iostream>
#include <exception>

struct Node {
	Node *left_child;
	Node *right_child;
	int value;
};

class BinaryTree {
private:
	Node *root;

	bool Insert(Node*, int value); // helper method for inserting normally
	void PrintTraverseInOrder(Node*);
	void PrintTraversePreOrder(Node*);
	void PrintTraversePostOrder(Node*);
public:
	BinaryTree();
	int count;

	// Insert a value using BST principles
	void InsertSorted(int);
	
	// Insert a value without sorting
	void Insert(int);

	// Return a Node given a value
	Node* Get(int);

	// Print all nodes in a tree
	void PrintInOrder();
	void PrintPreOrder();
	void PrintPostOrder();
};

BinaryTree::BinaryTree() {
	root = new Node;
	root->value = 0;
	root->left_child = NULL;
	root->right_child = NULL;
	count = 0;
}

void BinaryTree::InsertSorted(int value) {
	if (count == 0) {
		root->value = value;
		return;
	}

	Node *newNode;
	newNode->value = value;

	Node *itr_node = root;
	
	while (itr_node != NULL) {
		if (value > itr_node->value) {
			if (itr_node->right_child != NULL) {
				// traverse right
				itr_node = itr_node->right_child;
			}
			else {
				// insert as right child
				itr_node->right_child = newNode;
				count++;
				break;
			}
		}
		else if (value < itr_node->value) {
			if (itr_node->left_child != NULL) {
				// traverse left
				itr_node = itr_node->left_child;
			}
			else {
				// insert as left child
				itr_node->left_child = newNode;
				count++;
				break;
			}

		}
	}
}

void BinaryTree::Insert(int value) {
	if (count == 0) {
		root->value = value;
		count++;
		return;
	}
	
	Insert(root, value);
}

bool BinaryTree::Insert(Node* itr_node, int value) {
	// base case
	if (itr_node->left_child == NULL) {
		// Insert as left child
		Node* newNode = new Node;
		newNode->value = value;
		newNode->left_child = NULL;
		newNode->right_child = NULL;
		itr_node->left_child = newNode;
		count++;
		return true;
	}
	if (itr_node->right_child == NULL) {
		Node* newNode = new Node;
		newNode->value = value;
		newNode->left_child = NULL;
		newNode->right_child = NULL;
		itr_node->right_child = newNode;
		count++;
		return true;
	}

	// Both nodes are full

	// Traverse the left to attempt an insert
	bool inserted = false;
	Node* new_iter = itr_node;
	inserted = Insert(new_iter->left_child, value);

	if (inserted) return true;
	else {
		// Traverse the right to insert
		inserted = Insert(new_iter->right_child, value);
		if (inserted) return true;
	}

	return false;
}

void BinaryTree::PrintTraverseInOrder(Node* node) {
	if (node->left_child != NULL)
		PrintTraverseInOrder(node->left_child);
	
	std::cout << node->value << " ";
	
	if (node->right_child != NULL)
		PrintTraverseInOrder(node->right_child);
}

void BinaryTree::PrintTraversePreOrder(Node* node) {
	std::cout << node->value << " ";
	
	if (node->left_child != NULL)
		PrintTraversePreOrder(node->left_child);
	
	if (node->right_child != NULL)
		PrintTraversePreOrder(node->right_child);
}

void BinaryTree::PrintTraversePostOrder(Node* node) {
	if (node->left_child != NULL) 
		PrintTraversePostOrder(node->left_child);
	
	if (node->right_child != NULL)
		PrintTraversePostOrder(node->right_child);
	
	std::cout << node->value << " ";
}

void BinaryTree::PrintInOrder() {
	PrintTraverseInOrder(root);
}

void BinaryTree::PrintPreOrder() {
	PrintTraversePreOrder(root);
}

void BinaryTree::PrintPostOrder() {
	PrintTraversePostOrder(root);
}

int testTree() {
	BinaryTree tree;
	
	for (int i = 0; i < 11; i++) {
		tree.Insert(i);
	}
	
	tree.PrintPostOrder();
	
	system("pause");
	return 0;
}