#include	<iostream>
#include	<fstream>
#include	<cstdlib>
#include	<cstdio>
#include	<ctime>
#include	"BST.h"
#include    "Student.h"

using namespace std;

bool readFile(const char *, BST *);
int menu();

int main() {

	/*
		case 4:									(d) clone subtree of t1 into t2
			if (t1.empty()) {
				cout << "\nThe tree is empty. Please read a data file first.\n";
				break;
			}
			cout << "\nEnter the student id of the subtree root to be cloned: ";
			cin >> id;
			cin.ignore(1000, '\n');

			key = Student();
			key.id = id;
			if (!t2.empty()) t2.clear();		

			if (!t2.CloneSubtree(t1, key))
				cout << "\nCannot clone subtree.\n";
			else {
				cout << "\n----- t1 (pre-order) : " << t1.size() << " node(s) -----\n";
				t1.preOrderPrint();
				cout << "\n----- t2, the cloned subtree (pre-order) : " << t2.size() << " node(s) -----\n";
				t2.preOrderPrint();
			}
			break;
	*/
	
	system("pause");
	return 0;
}
