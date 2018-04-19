/*	Chaz Acheronti 2018	*/
/*	March 30 2018		*/
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <functional>

  ////////////////////////////////////////
 // Hash Set Implemented with Vector   //
////////////////////////////////////////
//	Separate chaining resolution w/ linked list
//	Function pointer comparison	(MUST RETURN 0 FOR EQUAL)
//	Function pointer hashing		(Must be defined within TABLE SIZE bounds)

size_t TABLE_SIZE = 10;

// Math Functions
namespace Math {
	int stringComparator(std::string a, std::string b) {
		return a.compare(b);
	}

	int sizeHashFunc(std::string key) {
		// convert a string key to a hash
		unsigned long hash = 0;
		
		for (size_t i = 0; i < key.size(); i++) {
			hash = 77 * hash + (unsigned char)key[i];
		}

		return hash % TABLE_SIZE;
	}
}


// Hash entry with generic types
template <typename T_key, typename T_value>
class HashNode {
private:
	T_key key;
	T_value value;

public:
	HashNode(T_key key, T_value value) {
		this->key = key;
		this->value = value;
		this->next = NULL;
	}

	T_key GetKey() {
		return key;
	}

	T_value GetValue() {
		return value;
	}

	void SetValue(T_value value) {
		this->value = value;
	}

	void Print() {
		std::cout << "{ '" << key << "' => " << value << " }";
	}

	HashNode *next;
};

// hash table class container for hashmap entries
template <typename T_key, typename T_value>
class HashMap {
private:
	size_t size;
	size_t entries;
	std::vector<size_t> chainSize;
	
	HashNode<T_key, T_value> **Table;

	int (*comparatorFunc)(T_key, T_key); // user defined comparator for finding keys?
	int(*hashFunc)(T_key);

public:
	// initialize array of pointers to hashNode entries O(N)
	HashMap(size_t size, int(*hashFunc)(T_key), int(*comparator)(T_key, T_key)) {
		this->size = size;
		
		Table = new HashNode<T_key, T_value>*[this->size];
		chainSize.resize(this->size);

		this->hashFunc = hashFunc;
		this->comparatorFunc = comparator;
		
		for (size_t i = 0; i < this->size; i++) {
			Table[i] = NULL;
		}
	};

	// loop through the table and delete entries O(N)
	~HashMap() {
		for (size_t i = 0; i < size; i++) {
			HashNode<T_key, T_value>* chainNode = Table[i];

			while (chainNode != NULL) {
				HashNode<T_key, T_value>* prevNode = chainNode;
				chainNode = chainNode->next;
				//delete prevNode;
			}
		}
		delete[] Table;
	};

	HashNode<T_key, T_value>* Get(T_key key) {
		// find the location this key maps to
		int hash = hashFunc(key);
		HashNode<T_key, T_value>* entry = Table[hash];
		
		// is this the key?
		int comparison = comparatorFunc(entry->GetKey(), key);
		
		if (comparison == 0) {
			// we found the right node!
			return entry;
		} else {
			// the key does not match
			// are there collision entries?
			if (chainSize[hash] > 0) {
				// iterate through the chain
				while (entry != NULL) {
					entry = entry->next;
					if (comparison == 0) {
						return entry;
					}
				}
			}
		}

		std::cout << "Entry not found\n";
		
		return NULL;
	}

	void Insert(T_key key, T_value value) {
		// get the hash index
		int hash = hashFunc(key);
		HashNode<T_key, T_value> *entry = Table[hash];

		if (entry == NULL) {
			// Insert into the table
			Table[hash] = new HashNode<T_key, T_value>(key, value);
			entries++;
		} 
		else {
			// Location Collision
			// are they the same key?
			int comparison = comparatorFunc(entry->GetKey(), key);
			if (comparison == 0) {
				// update the value
				entry->SetValue(value);
			}
			else {
				// keys of the entries are different
				// collision resolution: Separate chaining
				HashNode<T_key, T_value>* prevNode = entry;
				entry = entry->next;
				
				while (entry != NULL) {
					prevNode = entry;
					entry= entry->next;
				}

				prevNode->next = new HashNode<T_key, T_value>(key, value);
				chainSize[hash] += 1;
				entries++;
			}
		}
	}

	size_t Size() {
		return size;
	}

	size_t Entries() {
		return entries;
	}

	void Remove(T_key key) {
		// Due to separate chaining and linkedlist connections
		// we must iterate through after the key is found
		
		// get index
		int hash = hashFunc(key);

		HashNode<T_key, T_value> *entry = Table[hash];

		if (entry == NULL) {
			std::cout << "Entry not found\n";
			return;
		}
		else {
			// Entry is occupied
			HashNode<T_key, T_value> *prev = NULL;

			while (entry != NULL) {
				int comparison = comparatorFunc(entry->GetKey(), key);

				if (comparison == 0) {
					// we found the key
					// is this a chain node or root node?
					if (prev == NULL) {
						if (entry->next != NULL) {
							// need to absorb the node
							prev = entry;
							Table[hash] = entry->next;
							delete prev;
						}
						else {
							// safe to delete
							delete entry;
						}
						entries -= 1;
						return;
					}
					else {
						// this is a chain node
						// unlink the node
						if (entry->next != NULL) {
							prev->next = entry->next;
						}
						else {
							prev->next = NULL;
						}

						entries -= 1;
						delete entry;
						return;
					}
				}
				prev = entry;
				entry = entry->next;
			}

		}
	}
	
	void Print() {
		// print all chains in order
		HashNode<T_key, T_value> *prev = NULL;
		HashNode<T_key, T_value> *entry = NULL;

		for (size_t i = 0; i < size; i++) {
			entry = Table[i];

			if (entry == NULL) std::cout << "<none>";

			while (entry != NULL) {
				entry->Print();
				if (chainSize[i] > 0) std::cout << ", ";
				entry = entry->next;
			}
			std::cout << std::endl;
		}
	}
};

int main() {
	HashMap<std::string, int> ages(TABLE_SIZE, *Math::sizeHashFunc, *Math::stringComparator);

	ages.Insert("Chaz", 21);
	ages.Insert("Marilyn", 22);
	ages.Insert("Cole", 24);
	ages.Insert("Randy", 56);
	ages.Insert("Alex", 18);
	ages.Insert("Kyle", 21);
	ages.Insert("Teagan", 21);
	ages.Insert("Tanner", 21);
	ages.Insert("Adam", 21);
	ages.Insert("Brian", 21);
	ages.Insert("Peter", 21);

	std::cout << "Getting entries: \n";
	ages.Get("Chaz")->Print();

	std::cout << std::endl;
	
	ages.Get("Marilyn")->Print();

	std::cout << "\nHashMap Size: " << ages.Size();
	std::cout << "\nEntries Size: " << ages.Entries() << std::endl;


	// BDAY 4/4
	ages.Insert("Chaz", 22);
	HashNode<std::string, int> *ChazEntry = ages.Get("Chaz");

	ChazEntry->Print();

	ages.Remove("Marilyn");

	std::cout << "\nEntries after removal: " << ages.Entries() << std::endl;
	
	ages.Print();

	system("pause");

	return 0;
}