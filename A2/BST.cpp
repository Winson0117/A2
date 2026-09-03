#include <iostream>
#include <fstream>
#include <algorithm>
#include "BST.h"


using namespace std;


BST::BST() {
	root = NULL;
	count = 0;
}


BST::BST(const BST &other) {
	// Make an independent copy so passing a BST by value is safe.
	root = cloneNode(other.root);
	count = other.count;
}


BST &BST::operator=(const BST &other) {
	if (this != &other) {
		BTNode *newRoot = cloneNode(other.root);
		clear(root);
		root = newRoot;
		count = other.count;
	}
	return *this;
}


BST::~BST() {
	// Release every dynamically allocated node when the tree goes out of scope.
	clear(root);
	root = NULL;
	count = 0;
}


bool BST::empty() {
	if (count == 0) return true;
	return false;
}


int BST::size() {
	return count;
}


void BST::preOrderPrint() {
	if (root == NULL) return;// handle special case
	else preOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::preOrderPrint2(BTNode *cur) {

	if (cur == NULL) return;
	cur->item.print(cout);
	preOrderPrint2(cur->left);
	preOrderPrint2(cur->right);
}


void BST::inOrderPrint() {
	if (root == NULL) return;// handle special case
	else inOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::inOrderPrint2(BTNode *cur) {

	if (cur == NULL) return;

	inOrderPrint2(cur->left);
	cur->item.print(cout);
	inOrderPrint2(cur->right);
}


void BST::postOrderPrint() {
	if (root == NULL) return;// handle special case
	else postOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::postOrderPrint2(BTNode *cur) {
	if (cur == NULL) return;
	postOrderPrint2(cur->left);
	postOrderPrint2(cur->right);
	cur->item.print(cout);
}



int BST::countNode() {
	int	counter = 0;
	if (root == NULL) return 0;
	countNode2(root, counter);
	return counter;
}


void BST::countNode2(BTNode *cur, int &nodeCount) {
	if (cur == NULL) return;
	countNode2(cur->left, nodeCount);
	countNode2(cur->right, nodeCount);
	nodeCount++;
}


bool BST::findGrandsons(type grandFather) {
	if (root == NULL) return false;
	return (fGS2(grandFather, root));
}


bool BST::fGS2(type grandFather, BTNode *cur) {
	if (cur == NULL) return false;
	//if (cur->item == grandFather) {
	if (cur->item.compare2(grandFather)){

		fGS3(cur, 0);// do another TT to find grandsons
		return true;
	}
	if (fGS2(grandFather, cur->left)) return true;
	return fGS2(grandFather, cur->right);
}


void BST::fGS3(BTNode *cur, int level) {
	if (cur == NULL) return;
	if (level == 2) {
		cur->item.print(cout);
		return;  // No need to search downward
	}
	fGS3(cur->left, level + 1);
	fGS3(cur->right, level + 1);
}



void BST::topDownLevelTraversal() {
	BTNode			*cur;
	Queue		    q;


	if (empty()) return; 	// special case
	q.enqueue(root);	// Step 1: enqueue the first node
	while (!q.empty()) { 	// Step 2: do 2 operations inside
		q.dequeue(cur);
		if (cur != NULL) {
			cur->item.print(cout);

			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);
		}
	}
}

//insert for BST
bool BST::insert(type newItem) {
	BTNode	*cur = new BTNode(newItem);
	if (!cur) return false;		// special case 1
	if (root == NULL) {
		root = cur;
		count++;
		return true; 			// special case 2
	}
	insert2(root, cur);			// normal
	count++;
	return true;
}


void BST::insert2(BTNode *cur, BTNode *newNode) {
	//if (cur->item > newNode->item) {
	if (cur->item.compare1(newNode->item)){
		if (cur->left == NULL)
			cur->left = newNode;
		else
			insert2(cur->left, newNode);
	}
	else {
		if (cur->right == NULL)
			cur->right = newNode;
		else
			insert2(cur->right, newNode);
	}
}



bool BST::remove(type item) {
	if (root == NULL) return false; 		// special case 1: tree is empty
	return remove2(root, root, item); 		// normal case
}

bool BST::remove2(BTNode *pre, BTNode *cur, type item) {

	// Turn back when the search reaches the end of an external path
	if (cur == NULL) return false;

	// normal case: manage to find the item to be removed
	//if (cur->item == item) {
	if (cur->item.compare2(item)){
		if (cur->left == NULL || cur->right == NULL)
			case2(pre, cur);	// case 2 and case 1: cur has less than 2 sons
		else
			case3(cur);		// case 3, cur has 2 sons
		count--;				// update the counter
		return true;
	}

	// Current node does NOT store the current item -> ask left sub-tree to check
	//if (cur->item > item)
	if (cur->item.compare1(item))
		return remove2(cur, cur->left, item);

	// Item is not in the left subtree, try the right sub-tree instead
	return remove2(cur, cur->right, item);
}


void BST::case2(BTNode *pre, BTNode *cur) {

	// special case: delete root node
	if (pre == cur) {
		if (cur->left != NULL)	// has left son?
			root = cur->left;
		else
			root = cur->right;

		delete cur;
		return;
	}

	if (pre->right == cur) {		// father is right son of grandfather? 
		if (cur->left == NULL)			// father has no left son?
			pre->right = cur->right;			// connect gfather/gson
		else
			pre->right = cur->left;
	}
	else {						// father is left son of grandfather?
		if (cur->left == NULL)			// father has no left son? 
			pre->left = cur->right;				// connect gfather/gson
		else
			pre->left = cur->left;
	}

	delete cur;					// remove item
}


void BST::case3(BTNode *cur) {
	BTNode		*is, *isFather;

	// get the IS and IS_parent of current node
	is = isFather = cur->right;
	while (is->left != NULL) {
		isFather = is;
		is = is->left;
	}

	// copy IS node into current node
	cur->item = is->item;

	// Point IS_Father (grandfather) to IS_Child (grandson)
	if (is == isFather)
		cur->right = is->right;		// case 1: There is no IS_Father    
	else
		isFather->left = is->right;	// case 2: There is IS_Father

	// remove IS Node
	delete is;
}

int BST::height(BTNode *cur) const {
	if (cur == NULL) return 0;
	return 1 + max(height(cur->left), height(cur->right));
}

void BST::display2(BTNode *cur, int order, ostream &out) const {
	// Stop when the traversal passes a leaf node.
	if (cur == NULL) return;
	if (order == 1) {
		// In-order traversal produces ascending student ids in a BST.
		display2(cur->left, order, out);
		cur->item.print(out);
		display2(cur->right, order, out);
	} else {
		// Reverse in-order traversal produces descending student ids.
		display2(cur->right, order, out);
		cur->item.print(out);
		display2(cur->left, order, out);
	}
}

bool BST::deepestNodes() {
	if (root == NULL) return false;

	Queue q;
	BTNode *cur;
	BTNode *deepest[100];
	int deepestCount = 0;

	q.enqueue(root);

	while (!q.empty()) {
		int levelSize = q.size();
		deepestCount = 0;

		for (int i = 0; i < levelSize; i++) {
			q.dequeue(cur);
			deepest[deepestCount++] = cur;

			if (cur->left != NULL) {
				q.enqueue(cur->left);
			}

			if (cur->right != NULL) {
				q.enqueue(cur->right);
			}
		}
	}

	for (int i = 0; i < deepestCount; i++) {
		cout << deepest[i]->item.id << endl;
	}
	return true;
}

bool BST::display(int order, int source) {
	// Reject an empty tree and unsupported parameter values.
	if (empty()) return false;
	if (order != 1 && order != 2) return false;
	if (source == 1) {
		// Send every record to the console.
		display2(root, order, cout);
		return true;
	}
	if (source == 2) {
		// Use the same recursive traversal with a file output stream.
		ofstream out("student-info.txt");
		if (!out) return false;
		display2(root, order, out);
		return true;
	}
	return false;
}

BTNode *BST::findNode(BTNode *cur, const type &item) const {
	// Search only the possible branch by comparing the student id key.
	if (cur == NULL) return NULL;
	if (cur->item.id == item.id) return cur;
	if (item.id < cur->item.id) return findNode(cur->left, item);
	return findNode(cur->right, item);
}

BTNode *BST::cloneNode(const BTNode *cur) const {
	// Recursively allocate a new node and copy both child subtrees.
	if (cur == NULL) return NULL;
	BTNode *copy = new BTNode(cur->item);
	copy->left = cloneNode(cur->left);
	copy->right = cloneNode(cur->right);
	return copy;
}

void BST::clear(BTNode *cur) {
	// Delete children before their parent (post-order deletion).
	if (cur == NULL) return;
	clear(cur->left);
	clear(cur->right);
	delete cur;
}

bool BST::CloneSubtree(BST t1, type item) {
	// The assignment requires the destination tree to be empty before cloning.
	if (!empty() || t1.empty()) return false;

	// Locate the requested subtree root in the source tree.
	BTNode *subtreeRoot = findNode(t1.root, item);
	if (subtreeRoot == NULL) return false;

	// Deep-copy the subtree and recalculate the destination node count.
	root = cloneNode(subtreeRoot);
	count = 0;
	countNode2(root, count);
	return true;
}

bool BST::printLevelNodes() {
	if (root == NULL) return false;

	Queue q;
	BTNode* cur;

	q.enqueue(root);

	int level = 1;

	while (!q.empty()) {
		int levelSize = q.size();
		cout << "Level " << level << " nodes: ";

		for (int i = 0; i < levelSize; i++) {
			q.dequeue(cur);
			cout << cur->item.id;

			if (i < levelSize - 1) {
				cout << " ";
			}

			if (cur->left != NULL) {
				q.enqueue(cur->left);
			}

			if (cur->right != NULL) {
				q.enqueue(cur->right);
			}
		}
		cout << endl;
		level++;
	}
	return true;
}

void BST::printPath2(BTNode *cur, int path[], int length) const {
	// Add the current id to the root-to-current-node path.
	if (cur == NULL) return;
	path[length++] = cur->item.id;

	// A leaf completes one external path, so print the stored ids.
	if (cur->left == NULL && cur->right == NULL) {
		for (int i = 0; i < length; i++) {
			if (i > 0) cout << "   ";
			cout << path[i];
		}
		cout << endl;
		return;
	}
	// Explore both branches; each recursive call receives its own length value.
	printPath2(cur->left, path, length);
	printPath2(cur->right, path, length);
}

bool BST::printPath() {
	if (empty()) return false;
	// A root-to-leaf path cannot contain more ids than the tree height.
	int *path = new int[height(root)];
	cout << "Below are all the external paths for the tree:\n\n";
	printPath2(root, path, 0);
	delete[] path;
	return true;
}




