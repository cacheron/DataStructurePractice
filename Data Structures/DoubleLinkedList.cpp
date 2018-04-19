/*	Chaz Acheronti 2018	*/
/*	March 27 2018		*/
#include <iostream>
#include <exception>

using namespace std;

/////////////////////////////////
//		  Linked List.cpp		//
/////////////////////////////////

struct Node {
	Node *prev, *next;
	int value;
};

/*
DoubleLinkedList Class

Can be utilized as an array, stack, or linked list

Use Append, Insert, and Push to insert nodes

Use Pop, RemoveAt, RemoveValue(s) to delete nodes
*/
class DoubleLinkedList {
private:
	Node *head, *tail;

public:
	int count;

	// Construct a Doubley LinkedList
	DoubleLinkedList();

	// Append a node to the back by Node or value
	void Append(Node*);		 // Insert node at the end
	void Append(int);		 // Insert node at the end with value
	void Push(Node*);		 // Insert node at the front
	void Push(int);			 // Insert node at the front with value
	void Insert(int, Node*); // Insert at idx a node
	void Insert(int, int);	 // Insert at idx a node with value

							 // Remove by Node or value
	void RemoveAt(int);		// Remove node at idx
	void RemoveValue(int);	// Remove first occurence of node with value
	void RemoveValues(int); // Remove all nodes with value
	int Pop(); // note, cannot return a node if it is going to be deleted, can only return value

			   // return node at index
	Node* At(int);

	// print contents of linked list
	void Print();
};

class IndexOutOfBounds : public exception {
	virtual const char* what() const throw()
	{
		return "Linked List index access out of bounds.";
	}
};

// Construction

DoubleLinkedList::DoubleLinkedList() {
	count = 0;
	this->head = NULL;
	this->tail = NULL;
}

// Insertion into the list

void DoubleLinkedList::Append(Node* newNode) {
	if (count <= 0) {
		// set as head
		head = newNode;
		tail = newNode;
	}
	else {
		// append to the tail, then move the tail pointer
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
	count++;
}

void DoubleLinkedList::Append(int value) {
	Node* newNode = new Node;
	newNode->value = value;
	newNode->next = NULL;
	newNode->prev = NULL;
	Append(newNode);
}

void DoubleLinkedList::Insert(int idx, Node* newNode) {
	if (idx >= count || idx < 0) {
		IndexOutOfBounds ex;
		throw ex;
	}

	if (idx == 0) {
		Push(newNode);
		return;
	}
	else if (idx == count - 1) {
		Append(newNode);
		return;
	}

	// Node is in the middle
	Node* itr_node = head;

	for (int i = 0; i < idx; i++) {
		itr_node = itr_node->next;
	}

	itr_node->prev->next = newNode;
	itr_node->prev = newNode;
	newNode->next = itr_node;

	count++;
}

void DoubleLinkedList::Insert(int idx, int value) {
	Node* newNode = new Node;
	newNode->value = value;
	newNode->next = NULL;
	Insert(idx, newNode);
}

void DoubleLinkedList::Push(Node* newNode) {
	newNode->next = head;
	newNode->prev = NULL;
	head->prev = newNode;
	head = newNode;
	count++;
}

void DoubleLinkedList::Push(int value) {
	Node* newNode = new Node;
	newNode->value = value;
	newNode->next = NULL;
	newNode->prev = NULL;
	Push(newNode);
}

// Deletion

void DoubleLinkedList::RemoveAt(int idx) {
	if (idx >= count || idx < 0) {
		IndexOutOfBounds ex;
		throw ex;
	}

	Node* itr_node = head;

	for (int i = 0; i < idx; i++) {
		itr_node = itr_node->next;
	}

	if (itr_node->prev != NULL) {
		if (itr_node->next != NULL) {
			// we are in the middle
			itr_node->prev->next = itr_node->next;
			itr_node->next->prev = itr_node->prev;
			delete itr_node;
			count--;
		}
		else {
			// found node is the tail
			itr_node->prev->next = NULL;
			tail = itr_node->prev;
			delete itr_node;
			count--;
		}
	}
	else {
		// we are at the head
		head = head->next;
		delete itr_node;
		count--;
	}
}

void DoubleLinkedList::RemoveValue(int value) {
	Node* itr_node = head;

	bool found = false;

	while (itr_node != NULL && !found) {
		if (itr_node->value == value) found = true;

		if (!found) {
			itr_node = itr_node->next;
		}
	}

	if (found) {
		if (itr_node->prev->next != NULL) {
			if (itr_node->next != NULL) {
				// we are in the middle
				itr_node->prev->next = itr_node->next;
				delete itr_node;
				count--;
			}
			else {
				// found node is the tail
				itr_node->prev->next = NULL;
				tail = itr_node->prev;
				delete itr_node;
				count--;
			}
		}
		else {
			// we are at the head
			head = head->next;
			delete itr_node;
			count--;
		}
	}
}

void DoubleLinkedList::RemoveValues(int value) {
	Node* itr_node = head;
	Node* temp = NULL;

	bool found = false;

	while (itr_node != NULL) {
		if (itr_node->value == value) found = true;

		if (found) {
			temp = itr_node;
			found = false;

			if (itr_node->prev != NULL) {
				if (itr_node->next != NULL) {
					// we are in the middle
					itr_node->prev->next = temp->next;
					delete temp;
					count--;
					itr_node->prev = itr_node;
					itr_node = itr_node->next;
				}
				else {
					// found node is the tail
					tail = itr_node->prev;
					itr_node->prev->next = NULL;
					itr_node = 0;
					delete itr_node;
					count--;
				}
			}
			else {
				// we are at the head
				head = head->next;
				delete temp;
				count--;
				// increment
				itr_node = head;
				itr_node->prev = NULL;
			}
		}
		else {
			itr_node = itr_node->next;
		}
	}
}

int DoubleLinkedList::Pop() {
	Node* temp = head; // get the head node
	head = head->next; // mode head pointer to next node

	int value = temp->value; // save node's value
	delete temp;
	count--;

	return value;
}

Node* DoubleLinkedList::At(int idx) {
	if (idx >= count || idx < 0) {
		IndexOutOfBounds ex;
		throw ex;
	}

	Node* itr_node = head;
	for (int i = 0; i < idx; i++) {
		itr_node = itr_node->next;
	}

	return itr_node;
}

// Printing

void DoubleLinkedList::Print() {
	Node* itr_node = head;

	int idx = 0;

	std::cout << "Count: " << count << std::endl;

	while (itr_node != NULL) {
		std::cout << itr_node->value << "   ";
		itr_node = itr_node->next;
		idx++;
	}

	std::cout << std::endl;
	for (int i = 0; i < count; i++) std::cout << "-----";
	std::cout << std::endl;

}

int test_DL() {
	// NOTE: there is no sorting in this example
	DoubleLinkedList list;
	list.Append(0);

	for (int i = 1; i < 10; i++) {
		list.Append(i);
		list.Push(-i);
	}

	list.Print();

	// Insertion
	std::cout << "\nInsertion\n=======================================";

	std::cout << "\n\nAppending to the list (20) . . . \n";
	list.Append(20);
	list.Print();

	std::cout << "\n\nPusing to the list (-20) . . . \n";
	list.Push(-20);
	list.Print();

	std::cout << "\n\nInsertion to the list (-15) at index 1 . . . \n";
	list.Insert(1, -15);
	list.Print();

	// Deletion
	std::cout << "\nDeletion\n=======================================";

	std::cout << "\n\nPopping the top . . . \n";
	int value = list.Pop();
	std::cout << "Popped value " << value << std::endl;
	list.Print();

	std::cout << "\n\nRemoving all values of 9 . . . \n";
	list.RemoveValues(9);
	list.Print();

	std::cout << "\n\nDeletion at index 3 . . . \n";
	list.RemoveAt(3);
	list.Print();

	system("pause");
	return 0;
}