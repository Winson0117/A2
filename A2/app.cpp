#include	<iostream>
#include	<fstream>
#include	<cstdlib>
#include	<cstdio>
#include	<ctime>
#include	"BST.h"
#include    "Student.h"
#include    <cstring>
#include    <limits>
#include    <string>

using namespace std;

bool readFile(const char *, BST *);
int menu();

int main() {
	BST t1;
	int choice;

	// Keep showing the menu until the user selects Exit.
	do {
		choice = menu();
		switch (choice) {
		case 1: {
			// Ask for a filename so the program also works with marker test files.
			string filename;
			cout << "Enter input filename: ";
			cin >> filename;
			if (!readFile(filename.c_str(), &t1)) cout << "Unable to open input file.\n";
			break;
		}
		case 2:
			// deepestNodes() is intentionally excluded from this first merge.
			cout << "deepestNodes() has not been merged yet.\n";
			break;
		case 3: {
			int order, source;
			cout << "Enter order (1 = ascending, 2 = descending): "; cin >> order;
			cout << "Enter source (1 = screen, 2 = student-info.txt): "; cin >> source;
			if (!t1.display(order, source)) cout << "Unable to display students.\n";
			else if (source == 2) cout << "Student records written to student-info.txt.\n";
			break;
		}
		case 4: {
			Student item;
			BST t2;
			// Only the id is needed because the BST search key is the student id.
			cout << "Enter the student id at the root of the subtree: "; cin >> item.id;
			if (!t2.CloneSubtree(t1, item)) cout << "Cannot clone subtree.\n";
			else {
				cout << "\n----- t1 (pre-order): " << t1.size() << " node(s) -----\n";
				t1.preOrderPrint();
				cout << "\n----- t2, cloned subtree (pre-order): " << t2.size() << " node(s) -----\n";
				t2.preOrderPrint();
			}
			break;
		}
		case 5:
			cout << "printLevelNodes() has not been merged yet.\n";
			break;
		case 6:
			if (!t1.printPath()) cout << "The tree is empty.\n";
			break;
		case 7:
			cout << "Goodbye.\n";
			break;
		}
	} while (choice != 7);
	return 0;
}

static string valueAfterEquals(const string &line) {
	// Extract and trim the text that appears after '=' on an input line.
	size_t pos = line.find('=');
	if (pos == string::npos) return "";
	string value = line.substr(pos + 1);
	size_t first = value.find_first_not_of(" \t\r\n");
	size_t last = value.find_last_not_of(" \t\r\n");
	return first == string::npos ? "" : value.substr(first, last - first + 1);
}

template <size_t N>
static void copyField(char (&destination)[N], const string &source) {
	// Copy safely into the fixed-size character arrays in Student.
	strncpy_s(destination, N, source.c_str(), _TRUNCATE);
}

bool readFile(const char *filename, BST *t1) {
	// Opening failure is reported to main through the false return value.
	ifstream input(filename);
	if (!input) return false;
	int records = 0;
	string line;
	while (getline(input, line)) {
		// A new record always begins with the "Student Id" label.
		if (line.rfind("Student Id", 0) != 0) continue;
		Student student;
		student.id = stoi(valueAfterEquals(line));
		if (!getline(input, line)) break; copyField(student.name, valueAfterEquals(line));
		if (!getline(input, line)) break; copyField(student.address, valueAfterEquals(line));
		if (!getline(input, line)) break; copyField(student.DOB, valueAfterEquals(line));
		if (!getline(input, line)) break; copyField(student.phone_no, valueAfterEquals(line));
		if (!getline(input, line)) break; copyField(student.course, valueAfterEquals(line));
		if (!getline(input, line)) break; student.cgpa = stod(valueAfterEquals(line));
		// insert() positions the complete record according to its student id.
		if (t1->insert(student)) records++;
	}
	cout << records << " student record(s) successfully read.\n";
	return true;
}

int menu() {
	// Display every assignment operation and return the user's selection.
	int choice;
	cout << "\n(1) Read data to BST\n"
		 << "(2) Print deepest nodes\n"
		 << "(3) Display student\n"
		 << "(4) Clone Subtree\n"
		 << "(5) Print Level Nodes\n"
		 << "(6) Print Path\n"
		 << "(7) Exit\n"
		 << "Enter choice: ";
	cin >> choice;
	return choice;
}
