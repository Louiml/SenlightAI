Write a C++ function that builds a singly linked list from a vector of integers and returns a new vector containing the original list values followed by the value of a newly appended tail node. The function should take two parameters: a `const std::vector<int>&` representing the initial node values (the first element becomes the head, subsequent elements are appended in order), and an integer `tailValue` to be added as a new node at the end. The function must return a `std::vector<int>` that contains all original values in order, then `tailValue` at the end. If the input vector is empty, the function should return a vector containing only `tailValue`. The linked list must be created dynamically using `new`, properly traversed, and the new tail node must be linked after the last existing node. Memory should be managed (freed) appropriately to avoid leaks, though the solution need not include explicit cleanup if it would overcomplicate the demonstration.
// The solution constructs a linked list manually using a `struct Node` with `int val` and `Node* next`. We iterate through the input vector, creating nodes and linking them. For an empty vector, we directly create a head node with `tailValue` and return `{tailValue}`. Otherwise, we create the head from the first element and then append the remaining elements. After building the initial list, we traverse to the last node (where `next == nullptr`) and attach a new node holding `tailValue`. Then we traverse the entire list and copy each node's value into a result vector, which is returned. Edge cases include an empty input and a single-element list; both are handled naturally by the algorithm. Time complexity is O(n) for building the list, O(n) for finding the tail, and O(n) for copying, so overall O(n), where n is the number of original elements. Space complexity is O(n) for the dynamically allocated nodes plus O(n) for the result vector, so O(n) auxiliary (excluding the output). We use `const` for the input vector to ensure no modification.
#include <vector>

struct Node {
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};

// Build a linked list from values, append tailValue, and return all values in order.
std::vector<int> appendToLinkedList(const std::vector<int>& values, int tailValue) {
    if (values.empty()) {
        return {tailValue};
    }

    Node* head = new Node(values[0]);
    Node* current = head;
    for (size_t i = 1; i < values.size(); ++i) {
        current->next = new Node(values[i]);
        current = current->next;
    }

    // Append the new tail node
    current->next = new Node(tailValue);

    // Collect all values into result vector
    std::vector<int> result;
    current = head;
    while (current != nullptr) {
        result.push_back(current->val);
        current = current->next;
    }

    // Clean up memory (optional but good practice)
    current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    return result;
}
#include <cassert>
#include <vector>

// Forward declaration of the solution function (already defined above)
std::vector<int> appendToLinkedList(const std::vector<int>& values, int tailValue);

int main() {
    // Basic case with multiple elements
    assert(appendToLinkedList({1, 2, 3}, 4) == std::vector<int>({1, 2, 3, 4}));
    
    // Single element list
    assert(appendToLinkedList({5}, 10) == std::vector<int>({5, 10}));
    
    // Empty input
    assert(appendToLinkedList({}, 7) == std::vector<int>({7}));
    
    // Negative and zero values
    assert(appendToLinkedList({-3, 0, 8}, -2) == std::vector<int>({-3, 0, 8, -2}));
    
    // Duplicate values
    assert(appendToLinkedList({2, 2, 2}, 2) == std::vector<int>({2, 2, 2, 2}));
    
    // Large list (e.g., 1000 elements)
    std::vector<int> large_input;
    for (int i = 0; i < 1000; ++i) large_input.push_back(i);
    std::vector<int> expected = large_input;
    expected.push_back(1000);
    assert(appendToLinkedList(large_input, 1000) == expected);
    
    // Check that original vector is not modified (passed by const)
    std::vector<int> original = {10, 20, 30};
    appendToLinkedList(original, 40);
    assert(original == std::vector<int>({10, 20, 30}));
    
    return 0;
}
