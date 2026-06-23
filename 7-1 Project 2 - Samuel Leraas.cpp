// 7-1 Project 2 - Samuel Leraas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Structure and Class Definitions

struct Course {
    string courseNumber;
    string title;
	vector<string> prerequisites;
};

// Internal strcuture for Binary Search Tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

// Default constructor for Node
    Node() {
		left = nullptr;
		right = nullptr;
    }

// Constructor with parameterized course
    Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

//Binary Search Tree class for managing courses
class BinarySearchTree {

private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	Node* searchNode(Node* node, string courseNumber);
	void destroyTree(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void Insert(Course course);
	void PrintInOrder();
	Course Search(string courseNumber);
};

// Default Constructor for BinarySearchTree
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

// Destructor for BinarySearchTree
BinarySearchTree::~BinarySearchTree() {
	destroyTree(root);
}

// Recursive function to delete all nodes in the tree
void BinarySearchTree::destroyTree(Node* node) {
	if (node != nullptr) {
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}
}

//Insert a course into the tree
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

// Print the courses in alphanumeric order
void BinarySearchTree::PrintInOrder() {
	this->inOrder(root);
}

// Search for a course by its course number
Course BinarySearchTree::Search(string courseNumber) {
	Node* current = searchNode(root, courseNumber);
	if (current != nullptr) {
		return current->course;
	}
	// If the course is not found, return an empty course with a message
	Course emptyCourse;
	emptyCourse.courseNumber = "Course not found";
	return emptyCourse;
	}

// Internal function to add a node to the tree
void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

// Internal function to perform in-order traversal of the tree
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << ": " << node->course.title << endl;
		inOrder(node->right);
	}
}

// Internal function to search for a node by course number
Node* BinarySearchTree::searchNode(Node* node, string courseNumber) {
	if (node == nullptr || node->course.courseNumber == courseNumber) {
		return node;
	}
	if (courseNumber.compare(node->course.courseNumber) < 0) {
		return searchNode(node->left, courseNumber);
	}
	else {
		return searchNode(node->right, courseNumber);
	}
}

/****************************************************************
*Helper Utilities and Global Variables
*****************************************************************/

// Helper to split lines on comma boundaries

vector<string> split(const string& str, char delimiter) {
	vector<string> tokens;
	string token;
	istringstream tokenStream(str);
	while (getline(tokenStream, token, delimiter)) {
		token.erase(remove(token.begin(), token.end(), '\r'), token.end());
		token.erase(remove(token.begin(), token.end(), '\n'), token.end());
		tokens.push_back(token);
	}
	return tokens;
}

// Parse file and read courses into the binary search tree
bool loadCourses(string filename, BinarySearchTree* bst) {
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Error opening file: " << filename << endl;
		return false;
	}

	string line;
	int coursesLoaded = 0;

	while (getline(file, line)) {
		if (line.empty()) 
			continue;
	
	vector<string> tokens = split(line, ',');

	if (tokens.size() < 2) {
		cout << "Error parsing line: " << line << " (skipping due to insufficent fields)" << endl;
		continue;
	}

	Course course;
	course.courseNumber = tokens[0];
	course.title = tokens[1];

	for (size_t i = 2; i < tokens.size(); ++i) {
		course.prerequisites.push_back(tokens[i]);
	}


bst->Insert(course);
coursesLoaded++;
}

file.close();
cout << "Successfully loaded " << coursesLoaded << " courses loaded into the data structure!" << endl;
return true;
}

// Main function to demonstrate loading courses and searching for a course
int main() {
	BinarySearchTree* courseTree = new BinarySearchTree();
	string filename;
	string searchId;
	bool isDataLoaded = false;
	int choice = 0;

	cout << "Welcome to the Academic Advisor Course Planner!" << endl;

	while (true) {
		cout << "\n========================================" << endl;
		cout << "1. Load File Data" << endl;
		cout << "2. Print Aplhanumeric Course List" << endl;
		cout << "3. Print Course Title and Prerequisites" << endl;
		cout << "9. Exit Program" << endl;
		cout << "========================================" << endl;

		if (!(cin >> choice)) {
			cout << "Error: Invalid input. Please enter a number corresponding to the menu options." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		switch (choice) {
		case 1:
			cout << "Enter the filename to load course data: ";
			std::getline(cin >> std::ws, filename);
			isDataLoaded = loadCourses(filename, courseTree);
			break;
		case 2:
			if (!isDataLoaded) {
				cout << "Please load course data first (Option 1)." << endl;
			}
			else {
				cout << "\nAlphanumeric Course List:" << endl;
				courseTree->PrintInOrder();
			}
			break;
		case 3:
			if (!isDataLoaded) {
				cout << "Please load course data first (Option 1)." << endl;
			}
			else {
				cout << "Enter the course number to search for (e.g., CS101): ";
				std::getline(cin >> std::ws, searchId);
				Course foundCourse = courseTree->Search(searchId);
				if (foundCourse.courseNumber == "Course not found") {
					cout << foundCourse.courseNumber << endl;
				}
				else {
					cout << "\nCourse Title: " << foundCourse.title << endl;
					if (foundCourse.prerequisites.empty()) {
						cout << "Prerequisites: None" << endl;
					}
					else {
						cout << "Prerequisites: ";
						for (const string& prereq : foundCourse.prerequisites) {
							cout << prereq << " ";
						}
						cout << endl;
					}
				}
			}
			break;
		case 9:
			cout << "Exiting program. Goodbye!" << endl;
			delete courseTree;
			return 0;
		default:
			cout << "Invalid choice. Please select a valid option from the menu." << endl;
			break;
		}
	}
}