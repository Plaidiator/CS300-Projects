#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string> // atoi
#include <time.h>
#include <list>

using namespace std;

/// <summary>
/// Segments of code are adapted from code made in the hashtable assignment
/// </summary>

typedef struct Ht_item Ht_item;

// Structure to hold course information
struct Course {
    string courseName;
    string title;
    string preReqs;
    Course() {
        title = "Empty";
    }
};

class HashTable {
private:
    HashTable** table;
    int tableSize;
    struct Node {
        Course course;
        unsigned key;
        Node* next;

        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        Node(Course aCourse, unsigned newKey) : Node(aCourse) {
            key = newKey;
        }
    };    

    vector<Node> nodes;

    unsigned size = 10;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void PrintAll();
    void insert(Course course);
    Course PrintCourse(string courseName);

    HashTable(int n) {
        tableSize = n;
        table = new HashTable * [tableSize];
        for (int i = 0; i < tableSize; ++i) {
            table[i] = NULL;
        }
    }
};

HashTable::HashTable() {
    nodes.resize(size);
}

HashTable::HashTable(unsigned int size) {
    // resize nodes size
    this->tableSize = size;
    nodes.resize(size);
}

HashTable::~HashTable() {
    // delete
    nodes.erase(nodes.begin());
}

void HashTable::PrintAll() {
    // for node begin to end iterate  
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Print(nodes[i].course);
    }
}

Course HashTable::PrintCourse(string courseName) {
    Course course;

    // initialize key 
    unsigned key = hash(atoi(courseName.c_str()));

    Node* node = &(nodes.at(key));

    // check keys of hashtable for match
    if (node != nullptr && node->key != UINT_MAX && node->course.courseName.compare(courseName) == 0) {
        return node->course;
    }
    if (node == nullptr) {
        return course;
    }

    // check each element of hashtable
    while (node != nullptr) {
        if (node->key != UINT_MAX && node->course.courseName.compare(courseName) == 0) {
            return node->course;
        }
        node = node->next;
    }

    return course;
}

void Print(Course course) {
    cout << course.courseName << ", " << course.title << " | Prerequisites: " << course.preReqs << endl;
}

unsigned int HashTable::hash(int key) {
    return key % size;
}

void HashTable::insert(Course course) {
    unsigned key = hash(atoi(course.courseName.c_str()));

    Node* past = &(nodes.at(key));

    if (past == nullptr) {
        Node* nextNode = new Node(course, key);
        // call function to put information into table
        nodes.insert(nodes.begin() + key, (*nextNode));
    }
    else {
        // if node exists
        if (past->key == UINT_MAX) {
            // save information to table
            past->key = key;
            past->course = course;
            past->next = nullptr;
        }
        else {
            // find next available node
            while (past->next != nullptr) {
                past = past->next;
            }
        }
    }
}

void loadCourses(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file... " << csvPath << endl;

    string line, item;
    vector<string> row;

    fstream file(csvPath, ios::in);

    Course course;

    while (getline(file, line)) {
        row.clear();   // clear for next line

        stringstream str(line);

        int i = 0;
        while (getline(str, item, ',')) {
            // put each item to vector
            row.push_back(item);
            ++i;
        }
        
        // save depending on number of prerequisites
        if (row.size() > 3) {
            course.courseName = row[0];
            course.title = row[1];
            course.preReqs = row[2] + " & " + row[3];
        }
        else if (row.size() > 2) {
            course.courseName = row[0];
            course.title = row[1];
            course.preReqs = row[2];
        }
        else {
            course.courseName = row[0];
            course.title = row[1];
        }

        hashTable->insert(course);
    }
    cout << course.courseName << endl;

    cout << "Done." << endl << endl;
}

int main() {
	int input = 0;
    Course course;
    HashTable* courses;

    cout << "Welcome to the course planner." << endl << endl;
	while (input != 9) {
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl << endl;
        cout << "What would you like to do? ";

        cin >> input;

        courses = new HashTable(10);

        if (input == 1) {
            loadCourses("courses.csv", courses);
        }
        else if (input == 2) {
            // print list
            cout << "Here is a sample schedule: " << endl;
            courses->PrintAll();
        }
        else if (input == 3) {
            // get course and print info
            string search;

            cout << "What course do you want to know about? ";
            cin >> search;

            course = courses->PrintCourse(search);

        }
        else if (input == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }
        else {
            cout << input << " is not a valid option." << endl << endl;
        }
	}
    return 0;
}

