Design a C++ function that builds a singly linked list of student records, where each record alternates between a name and an ID number, and every node stores exactly one string value. The function must accept the student data as a `std::vector<std::string>` where even indices hold names and odd indices hold IDs (i.e., index 0 = name 1, index 1 = ID 1, index 2 = name 2, index 3 = ID 2, and so on). The function should construct a linked list in the given input order, link all nodes sequentially, and then traverse the entire list to return a single string that contains all stored values separated by `" | "` (e.g., `"Alice | 12345 | Bob | 67890"`). The input vector will always have an even length greater than zero, so each student has both a name and an ID. The function must be self-contained, avoid memory leaks (free all allocated nodes before returning), and handle arbitrary student counts.
#include <cassert>
#include <string>
#include <vector>

// The function under test is assumed to be defined above
// (declared here for clarity, but in the same translation unit)

int main() {
    // Basic alternating data with two students
    std::vector<std::string> data1 = {"Alice", "12345", "Bob", "67890"};
    assert(buildAndPrintStudentList(data1) == "Alice | 12345 | Bob | 67890");

    // Single student (two elements)
    std::vector<std::string> data2 = {"Zoe", "999"};
    assert(buildAndPrintStudentList(data2) == "Zoe | 999");

    // Five students (ten elements)
    std::vector<std::string> data3 = {"A", "1", "B", "2", "C", "3", "D", "4", "E", "5"};
    assert(buildAndPrintStudentList(data3) == "A | 1 | B | 2 | C | 3 | D | 4 | E | 5");

    // Duplicate names/IDs should be preserved in order
    std::vector<std::string> data4 = {"X", "10", "X", "10"};
    assert(buildAndPrintStudentList(data4) == "X | 10 | X | 10");

    // Empty input (edge case) should return empty string
    std::vector<std::string> data5;
    assert(buildAndPrintStudentList(data5) == "");

    // Very long string values
    std::vector<std::string> data6 = {"Long Name Here", "123456789", "Another Name", "987654321"};
    assert(buildAndPrintStudentList(data6) == "Long Name Here | 123456789 | Another Name | 987654321");

    return 0;
}
#include <string>
#include <vector>

// Node in the singly linked list
struct Node {
    std::string data;
    Node* next;
    Node(const std::string& val) : data(val), next(nullptr) {}
};

// Build a linked list from alternating name/ID strings and return all data joined by " | "
std::string buildAndPrintStudentList(const std::vector<std::string>& studentData) {
    if (studentData.empty()) {
        return "";
    }

    Node* head = nullptr;
    Node* tail = nullptr;

    // Build the linked list
    for (const std::string& value : studentData) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Traverse and collect data
    std::string result;
    Node* current = head;
    bool first = true;
    while (current != nullptr) {
        if (!first) {
            result += " | ";
        }
        result += current->data;
        first = false;
        current = current->next;
    }

    // Free memory
    current = head;
    while (current != nullptr) {
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
    }

    return result;
}
// The core approach is to build a singly linked list iteratively from the input vector. Start with a `head` pointer set to `nullptr` and a `tail` pointer also `nullptr`. For each string in the vector, create a new node using `new`, copy the string into the node’s `data` field, and set `next` to `nullptr`. If the list is empty, assign both `head` and `tail` to this new node; otherwise, set `tail->next` to the new node and update `tail` to point to the new node. After building, traverse from `head` while the pointer is not null, appending each node’s data to a result string with the `" | "` separator (only between elements, not at the end). After traversing, walk through the list again and `delete` each node to free memory. Edge cases: if the vector is empty (though not expected per spec, handle gracefully by returning an empty string), and if there is exactly one element (no separator needed). Time complexity is O(n) for building, O(n) for traversal, and O(n) for deletion, where n is the number of nodes. Space complexity is O(n) for the list nodes themselves, plus O(n) for the result string, but no additional auxiliary data structures are used.
