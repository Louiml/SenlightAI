/*
Write a standalone C++ function that, given a non-negative integer `n` and a sequence of `n` integer values, constructs a circular singly linked list where each new value is inserted at the head (i.e., the "last" pointer always points to the most recently inserted node, and that node's `next` points to the first node ever inserted). The function must return the "last" pointer of the circular list and also provide a way to retrieve the elements in the correct order (starting from the first inserted node and following `next` links all the way around). Specifically, implement a function `buildCircularList(int n, const std::vector<int>& values)` that returns a `List*` pointer to the last node (which is the most recently inserted one). Also implement a helper `std::vector<int> traverseCircularList(const List* last, int expectedSize)` that returns the elements in the order they were inserted (i.e., starting from `last->next` and continuing for exactly `expectedSize` nodes, stopping before revisiting the starting node). The solution must avoid printing anything, handle edge cases like `n == 0` (return `nullptr` and an empty vector), and ensure memory correctness (no leaks—though for simplicity, you may assume the caller will delete the list appropriately, or provide a `deleteList` function as a bonus). The main complexity is correctly managing the circular link when inserting at the head and avoiding infinite loops in traversal.
*/
#include <vector>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

// Builds a circular singly linked list, inserting each new value at the head.
// Returns pointer to the last inserted node (or nullptr if n == 0).
ListNode* buildCircularList(int n, const std::vector<int>& values) {
    if (n <= 0) return nullptr;
    
    ListNode* last = nullptr;
    for (int i = 0; i < n; ++i) {
        ListNode* node = new ListNode(values[i]);
        if (last == nullptr) {
            last = node;
            last->next = last;  // self-loop for the first node
        } else {
            node->next = last->next;  // new node points to first inserted node
            last->next = node;        // old last now points to new node
            last = node;              // update last to the newest node
        }
    }
    return last;
}

// Traverses the circular list starting from the first inserted node.
// Collects exactly expectedSize values in insertion order.
std::vector<int> traverseCircularList(const ListNode* last, int expectedSize) {
    std::vector<int> result;
    if (last == nullptr || expectedSize <= 0) return result;
    
    const ListNode* p = last->next;  // first inserted node
    for (int i = 0; i < expectedSize; ++i) {
        result.push_back(p->val);
        p = p->next;
    }
    return result;
}

// Helper to free the entire circular list (caller should use this to avoid leaks).
void deleteCircularList(ListNode* last) {
    if (last == nullptr) return;
    ListNode* start = last->next;
    ListNode* current = start;
    do {
        ListNode* next = current->next;
        delete current;
        current = next;
    } while (current != start);
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution functions are assumed to be defined above in the same translation unit.
// We will declare them here for the test.
struct ListNode;
ListNode* buildCircularList(int n, const std::vector<int>& values);
std::vector<int> traverseCircularList(const ListNode* last, int expectedSize);
void deleteCircularList(ListNode* last);

int main() {
    // Empty list
    std::vector<int> empty;
    ListNode* last0 = buildCircularList(0, empty);
    assert(last0 == nullptr);
    assert(traverseCircularList(last0, 0).empty());

    // Single element
    std::vector<int> single = {5};
    ListNode* last1 = buildCircularList(1, single);
    assert(traverseCircularList(last1, 1) == std::vector<int>{5});
    deleteCircularList(last1);

    // Multiple elements in insertion order
    std::vector<int> vals = {1, 2, 3, 4};
    ListNode* last4 = buildCircularList(4, vals);
    assert(traverseCircularList(last4, 4) == vals);
    deleteCircularList(last4);

    // Negative numbers and duplicates
    std::vector<int> neg = {-5, 7, -5, 0, 7};
    ListNode* last5 = buildCircularList(5, neg);
    assert(traverseCircularList(last5, 5) == neg);
    deleteCircularList(last5);

    // Larger sequence, verify circularity by traversing multiple cycles
    std::vector<int> big = {10, 20, 30, 40, 50, 60};
    ListNode* last6 = buildCircularList(6, big);
    assert(traverseCircularList(last6, 6) == big);
    assert(traverseCircularList(last6, 12) == ([]{
        std::vector<int> twice = big;
        twice.insert(twice.end(), big.begin(), big.end());
        return twice;
    })());
    deleteCircularList(last6);

    std::cout << "All tests passed!\n";
    return 0;
}
// The core idea is to maintain a circular singly linked list where a single pointer `last` points to the most recently inserted node. Initially, `last` is `nullptr`. When inserting the first node, we create a node whose `next` points to itself, and set `last` to that node. For every subsequent insertion, we create a new node, set its `next` to `last->next` (which is the first node inserted), then set `last->next` to the new node, and finally update `last` to point to the new node. This maintains the invariant: `last->next` is the first inserted node, and traversing from `last->next` and following `next` exactly `n` times returns all nodes in insertion order and lands back at `last->next`. For traversal, we simply start at `last->next` and iterate `n` times, collecting values and moving `p = p->next`. Edge case `n == 0` returns `nullptr` and an empty vector. Time complexity is `O(n)` for building and `O(n)` for traversal, space is `O(n)` for the list nodes (plus the output vector). The main pitfalls are: forgetting to link the first node to itself, incorrectly updating `last` (which should point to the newest node), and in traversal, being off-by-one in the number of steps (must stop after exactly `n` nodes to avoid infinite loop).
