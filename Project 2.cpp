// Name         : Project2.cpp
// Author       : Danny Forte
// Date         : 4/20/25 (Happy Easter)
// Description  : Program designed to load a course data file, parse it, create an alphanumericaly sorted list, search through list and create a new list with course prereq's, and print out results.




#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

// helper function for case senstivity during search
// converts string to uppercase
std::string toUpperCase(const std::string& str) {
    std::string upperStr = str;
    for (auto& c : upperStr) {
        c = std::toupper(c);
    }
    return upperStr;
}


// Struct to hold course data
struct Course {
    std::string courseNumber;
    std::string courseTitle;
    std::vector<std::string> prerequisites;
};

// TreeNode struct for binary search tree
struct TreeNode {
    Course course;
    TreeNode* leftChild = nullptr;
    TreeNode* rightChild = nullptr;

    TreeNode(Course c) : course(c) {}
};

//  insert a course object into the binary search tree
TreeNode* insertIntoBinarySearchTree(TreeNode* root, Course course) {
    if (root == nullptr) {
        return new TreeNode(course);
    }
    if (course.courseNumber < root->course.courseNumber) {
        root->leftChild = insertIntoBinarySearchTree(root->leftChild, course);
    }
    else {
        root->rightChild = insertIntoBinarySearchTree(root->rightChild, course);
    }
    return root;
}

//  load data from file into binary search tree
TreeNode* loadDataStructure(const std::string& filePath) {
    std::ifstream file(filePath); // Open the file
    if (!file.is_open()) {
        std::cerr << "Error: File not found." << std::endl;
        return nullptr;
    }

    TreeNode* root = nullptr; // Initialize tree root
    std::string line;

    std::cout << "Loading data from file..." << std::endl;

    while (std::getline(file, line)) { // Read file line by line
        std::istringstream iss(line);
        std::string component;
        std::vector<std::string> components;

        while (std::getline(iss, component, ',')) { // Split line by commas
            components.push_back(component);
        }

        if (components.size() < 2) { // Validate line
            std::cerr << "Error: Invalid line." << std::endl;
            continue;
        }

        Course course;
        course.courseNumber = components[0]; // First column is course number
        course.courseTitle = components[1];  // Second column is course title

        // Remaining columns are prerequisites, if any
        if (components.size() > 2) {
            for (size_t i = 2; i < components.size(); ++i) {
                course.prerequisites.push_back(components[i]);
            }
        }
        else {
            course.prerequisites.clear(); // Clear prerequisites vector
        }

        root = insertIntoBinarySearchTree(root, course); // Insert course into tree
    }

    file.close(); // Close the file
    std::cout << "Data loaded successfully!" << std::endl;
    return root;
}


// Function to print course details
void printCourseDetails(TreeNode* root, const std::string& courseNumber) {
    if (root == nullptr) {
        std::cout << "Error: Course not found." << std::endl;
        return;
    }

    if (courseNumber == root->course.courseNumber) {
        std::cout << "Course Number: " << root->course.courseNumber << std::endl;
        std::cout << "Course Title: " << root->course.courseTitle << std::endl;

        // Check if prerequisites vector is empty
        if (root->course.prerequisites.empty()) {
            std::cout << "Prerequisites: None" << std::endl;
        }
        else {
            std::cout << "Prerequisites: ";
            for (size_t i = 0; i < root->course.prerequisites.size(); ++i) {
                std::cout << root->course.prerequisites[i];
                if (i < root->course.prerequisites.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl; // Newline after all course details**
    }
    else if (courseNumber < root->course.courseNumber) {
        printCourseDetails(root->leftChild, courseNumber);
    }
    else {
        printCourseDetails(root->rightChild, courseNumber);
    }
}


// Function to print all courses in sorted order
void printSortedCourses(TreeNode* root) {
    if (root != nullptr) {
        printSortedCourses(root->leftChild);
        std::cout << "Course Number: " << root->course.courseNumber << std::endl;
        std::cout << "Course Title: " << root->course.courseTitle << std::endl;

        // Check if prerequisites vector is empty
        if (root->course.prerequisites.empty()) {
            std::cout << "Prerequisites: None" << std::endl;
        }
        else {
            std::cout << "Prerequisites: ";
            for (size_t i = 0; i < root->course.prerequisites.size(); ++i) {
                std::cout << root->course.prerequisites[i];
                if (i < root->course.prerequisites.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl; // Newline after all course details**
        printSortedCourses(root->rightChild);
    }
}



// Menu implementation
void menu(TreeNode*& root) {
    int choice;
    do {
        std::cout << "ABCU Computer Science Department" << std::endl;
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Load Course File" << std::endl;
        std::cout << "2. Print Course List" << std::endl;
        std::cout << "3. Print Selected Course and its Prerequisites" << std::endl;
        std::cout << "9. Exit" << std::endl;

        std::cin >> choice;  // read user input

        switch (choice) {
        case 1: {
            std::string filePath;
            std::cout << "Enter file path: (default path is Course.CSV)";
            std::cin >> filePath;
            root = loadDataStructure(filePath);
            break;
        }
        case 2:
            std::cout << "Course Information:" << std::endl;
            printSortedCourses(root);
            break;
        case 3: {
            std::string courseNumber;
            std::cout << "Enter Course Number: ";
            std::cin >> courseNumber;
// Convert input to uppercase
            courseNumber = toUpperCase(courseNumber);
            printCourseDetails(root, courseNumber);
            break;
        }
        case 9:
            std::cout << "Program Ended. Goodbye!" << std::endl;
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    } while (choice != 9);
}

// Main function
int main() {
    TreeNode* root = nullptr;
    menu(root);
    return 0;
}
