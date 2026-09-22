/*
Write a C++ function that accepts a linked list node pointer (where each node contains an integer `data` and a `next` pointer) and prints the list's elements in reverse order using recursion, but with the following modification: it must first process the entire list recursively to reach the last node, then print each node's data during the unwinding phase only if the node is not the last node (i.e., skip printing the final node's data). If the list is empty (pointer is `nullptr`), the function does nothing. The function should not modify the list, should use recursion only, and should handle a single-node list by printing nothing.
*/

#include <iostream>

// Definition for a singly-linked list node.
struct ListNode {
    int data;
    ListNode* next;
    ListNode(int val) : data(val), next(nullptr) {}
};

// Print all elements of the linked list in reverse order, 
// but skip the last node (the original tail). 
// If the list is empty or has one node, nothing is printed.
// Uses recursion and does not modify the list.
void printReverseExceptLast(const ListNode* head) {
    // Base case: empty list or reached the end.
    if (head == nullptr) {
        return;
    }
    
    // Recurse on the rest of the list first.
    printReverseExceptLast(head->next);
    
    // After recursion, if this node is not the last one (i.e., it has a next),
    // print its data. This skips the original tail because its next is null.
    if (head->next != nullptr) {
        std::cout << head->data;
        // Optionally separate outputs, but the task doesn't require a specific format.
        // We'll just print the integer with no extra whitespace.
    }
}

#include <cassert>
#include <sstream>
#include <vector>

// Helper to capture output from the function.
std::string captureOutput(const ListNode* head) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printReverseExceptLast(head);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

// Helper to build a linked list from a vector.
ListNode* buildList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* current = head;
    for (size_t i = 1; i < values.size(); ++i) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }
    return head;
}

// Helper to delete a linked list.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test empty list: prints nothing.
    assert(captureOutput(nullptr) == "");
    
    // Test single node: prints nothing.
    ListNode* single = new ListNode(42);
    assert(captureOutput(single) == "");
    deleteList(single);
    
    // Test two nodes: [1,2] -> reverse except last prints "1".
    ListNode* list1 = buildList({1, 2});
    assert(captureOutput(list1) == "1");
    deleteList(list1);
    
    // Test three nodes: [1,2,3] -> reverse except last prints "21" (2 then 1).
    ListNode* list2 = buildList({1, 2, 3});
    assert(captureOutput(list2) == "21");
    deleteList(list2);
    
    // Test five nodes: [10,20,30,40,50] -> prints "40302010".
    ListNode* list3 = buildList({10, 20, 30, 40, 50});
    assert(captureOutput(list3) == "40302010");
    deleteList(list3);
    
    // Test negative values: [-5,0,7] -> prints "0-5" (since 0 then -5).
    ListNode* list4 = buildList({-5, 0, 7});
    assert(captureOutput(list4) == "0-5");
    deleteList(list4);
    
    // Test same values: [3,3,3] -> prints "33".
    ListNode* list5 = buildList({3, 3, 3});
    assert(captureOutput(list5) == "33");
    deleteList(list5);
    
    return 0;
}

// The core idea is to perform a recursive traversal to the end of the list, then during the unwinding (return) phase, print the data of each node *except* the last one. The simplest way is to define a helper recursive function that, given a node, recurses on `node->next` first, and then if `node->next` is not null (meaning the current node is not the last), prints its data. That way, the last node (whose `next` is null) will not print. For an empty list (nullptr), the function returns immediately. For a single-node list, the recursion goes to `next` (null), then returns, never printing, which matches the specification. Edge cases: null pointer, single node, multiple nodes. Time complexity: \(O(n)\) for \(n\) nodes because each node is visited once during the forward recursion and once during unwinding. Space complexity: \(O(n)\) due to the call stack depth, assuming a linear list.
