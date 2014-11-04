//John W
//10-27-2014
//COMPANY Coding Exercise

#include <iostream>
using namespace std;

/*
Node for mutable trinary tree.
Generic to allow multiple types of data.
Type used must have a valid comparator that overloads > and <.
If mutated only with tritree_insert and tritree_delete
functions, then the resulting tritree will be in
strictly non-descending order.

No balancing occurs, so insertion and deletion
have O(n) worst case time complexities,
where n is the number of nodes in the trinary tree.
*/
template <typename T>
struct Node {
	T data;
	Node *right, *middle, *left;
	Node(T key) {
		this->data = key;
		this->right = NULL;
		this->middle = NULL;
		this->left = NULL;
	}
};

/*
tritree_insert is a function that inserts a node with
data=key into a tree (or subtree) rooted in *root
Parameters: Pointer to a node, passed by reference
			Generic key, passed by value
Returns: Nothing
Notes: while (true) loop is vulnerable.
	If this function is called on a node controlled by
	an adversary, function can be made to loop forever.
*/
template <typename T>
void tritree_insert(Node<T> *&root, T key) {
	if (root == NULL) {				//if no root exists
		root = new Node<T>(key);	//key node becomes root node
		return;
	}
	else {
		Node<T> *current = root;
		while (true) {				//insertion should never fail under normal circumstances
			if (current->data == key) {		//Case 1 - found node with data == key
				while (current->middle != NULL) {
					current = current->middle;
				}
				current->middle = new Node<T>(key); //New node inserted at middle of found node
				return;
			}
			else if (current->data < key) {
				if (current->right == NULL) {	//Case 2a - No matching nodes
					current->right = new Node<T>(key); //and node should be right child
					return;
				}
				else {
					current = current->right;
				}
			}
			else if (current->data > key) {
				if (current->left == NULL) {	//Case 2b - No matching nodes
					current->left = new Node<T>(key); //and node should be left child
					return;
				}
				else {
					current = current->left;
				}
			}
		}
	}
}

/*
tritree_delete is a function that deletes a node with
data=key from a tree (or subtree) rooted in *root
Parameters: Pointer to a node, passed by reference
			Generic key, passed by value
Returns: Nothing
Notes: Fails silently when no node with data==key can be found.
	Deletes one node per call.
*/
template <typename T>
void tritree_delete(Node<T> *&root, T key) {
	Node<T> *current = root;
	Node<T> *parent = NULL;
	
	if (root == NULL) { //Special case: tree is empty
		return;
	}
	while (current->data != key) { //Loop finds node in tree with simple search
		if (current->data < key) {
			if (current->right != NULL) {
				parent = current;
				current = current->right;
			}
			else {
				return; //key not found
			}
		}
		else if (current->data > key) {
			if (current->left != NULL) {
				parent = current;
				current = current->left;
			}
			else {
				return; //key not found
			}
		}
	}
	if (current->middle != NULL) { //Case 1: Element appears more than once (so it has middle children)
		Node<T> *temp = current->middle;
		current->middle = current->middle->middle; //it suffices to remove the second element in the middle chain
		delete temp; //and then delete it
		return;
	}
	else if ((current->right == NULL) && (current->left == NULL)) { //Case 2: Leaf element
		if (parent == NULL) { //Element is root, has no parent
			root = NULL;
		}
		else if (parent->right == current) { //Element is right child of parent
			parent->right = NULL;
		}
		else if (parent->left == current) { //Element is left child of parent
			parent->left = NULL;
		}
		delete current;
		return;
	}
	else if (current->left == NULL) { //Case 3: Element has only right child
		if (parent == NULL) { //Element is root, has no parent
			root = current->right;
		}
		else if (parent->right == current) { //Element is right child of parent
			parent->right = current->right;
		}
		else if (parent->left == current) { //Element is left child of parent
			parent->left = current->right;
		}
		delete current;
		return;
	}
	else if (current->right == NULL) { //Case 4: Element has only left child
		if (parent == NULL) {
			root = current->left;
		}
		if (parent->right == current) {
			parent->right = current->left;
		}
		else if (parent->left == current) {
			parent->left = current->left;
		}
		delete current;
		return;
	}
	else { //Case 5: Element has two children
		Node<T> *succ = current->right;
		parent = current;
		while (succ->left != NULL) { //finds in-order successor
			parent = succ;
			succ = succ->left;
		}
		current->data = succ->data; //replaces node with its in-order successor
		current->middle = succ->middle; //and all its middle children
			//if current had any middle children, case 1 would have handled it
			//so no data is lost in the above operation
			
		//the successor can have at most one child (a right child)
		if (parent != current) {	//successor is a left child
			parent->left = succ->right; //replace successor with its right child (can be null)
		}
		else if (parent == current) { //if successor is a right child
			current->right = succ->right; //replace successor with its right child (can be null)
		}
		delete succ;
		return;
	}
}

/*
tritree_traverse performs a recursive in-order traversal
of a given tritree rooted at *node.
Modifies no data and prints tree contents in non-decreasing order.
*/
template <typename T>
void tritree_traverse(Node<T> *node) {
	if (node == NULL) {
		return;
	}
	tritree_traverse(node->left);
	cout << node->data << ' ';
	tritree_traverse(node->middle);
	tritree_traverse(node->right);
	return;
}


int main() {
	Node<int> *root = NULL; //initializes root of integer trinary tree to NULL
	
	tritree_insert(root, 5); //tests root insert
	tritree_insert(root, 5); //tests middle child insert
	tritree_insert(root, 1); //tests left child insert
	tritree_insert(root, 10); //tests right child insert
	tritree_insert(root, 10);
	tritree_insert(root, 10);
	tritree_insert(root, 15);
	tritree_insert(root, 0);
	tritree_traverse(root); //expected output: 0 1 5 5 10 10 10 10 15
	
	cout << "\n-----\n";
	
	tritree_delete(root, 5); //delete root with middle child
	tritree_delete(root, 15); //delete leaf
	tritree_delete(root, 10); //delete leaf with middle child
	tritree_delete(root, 5); //delete root with two children
	tritree_delete(root, 1); //delete node with one child
	tritree_delete(root, -100000); //delete nonexistent node
	tritree_traverse(root);	//expected output: 0 10 10
	
	cout << "\n-----\n";

	tritree_insert(root, 5); //insertion after deletion
	tritree_traverse(root); //expected output: 0 5 10 10
}