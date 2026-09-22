/*
Write a C++ function `void reverseSublist(Node*& head, int start, int end)` that reverses the nodes of a singly linked list between 1-based positions `start` and `end` (inclusive), modifying the list in place. The list is defined by a `Node` structure with an `int value` and a `Node* next` pointer. You may assume `1 ≤ start ≤ end ≤ length` and that the list is non-empty. The function must handle edge cases where the reversal segment includes the head node or spans the entire list, and it must not allocate new nodes or use extra data structures beyond a constant number of pointers.
*/

struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

// Reverse the sublist from 1-based positions start to end inclusive.
void reverseSublist(Node*& head, int start, int end) {
    if (head == nullptr || start == end) return;
    
    Node* pre = nullptr;
    Node* current = head;
    int pos = 1;
    
    // Move to the node just before start
    while (pos < start) {
        pre = current;
        current = current->next;
        ++pos;
    }
    
    // current is now the first node of the sublist
    Node* first = current;
    Node* prev = current;
    current = current->next;
    ++pos;
    
    // Reverse the sublist by moving nodes one by one
    while (pos <= end) {
        Node* nextNode = current->next;
        if (pre == nullptr) {
            // Reversing from the head
            current->next = head;
            head = current;
        } else {
            current->next = pre->next;
            pre->next = current;
        }
        prev->next = nextNode;
        current = nextNode;
        ++pos;
    }
    
    // If pre was null, head may have changed; otherwise head stays same.
}

#include <cassert>

int main() {
    // Helper to build list from array
    auto buildList = [](std::initializer_list<int> vals) {
        Node* head = nullptr;
        Node** tail = &head;
        for (int v : vals) {
            *tail = new Node(v);
            tail = &((*tail)->next);
        }
        return head;
    };
    
    // Helper to compare list to expected values
    auto listEqual = [](Node* head, std::initializer_list<int> vals) {
        Node* cur = head;
        for (int v : vals) {
            if (cur == nullptr || cur->value != v) return false;
            cur = cur->next;
        }
        return cur == nullptr;
    };
    
    // Test 1: Reverse middle segment
    Node* list1 = buildList({1,2,3,4,5});
    reverseSublist(list1, 2, 4);
    assert(listEqual(list1, {1,4,3,2,5}));
    
    // Test 2: Reverse from head
    Node* list2 = buildList({1,2,3,4});
    reverseSublist(list2, 1, 3);
    assert(listEqual(list2, {3,2,1,4}));
    
    // Test 3: Reverse entire list
    Node* list3 = buildList({1,2,3});
    reverseSublist(list3, 1, 3);
    assert(listEqual(list3, {3,2,1}));
    
    // Test 4: Single node segment
    Node* list4 = buildList({1,2,3,4});
    reverseSublist(list4, 2, 2);
    assert(listEqual(list4, {1,2,3,4}));
    
    // Test 5: Reverse last two nodes
    Node* list5 = buildList({1,2,3});
    reverseSublist(list5, 2, 3);
    assert(listEqual(list5, {1,3,2}));
    
    // Test 6: Two-node list, reverse both
    Node* list6 = buildList({10,20});
    reverseSublist(list6, 1, 2);
    assert(listEqual(list6, {20,10}));
    
    // Cleanup (not required for asserts but good practice)
    // (omitted for brevity)
    
    return 0;
}

// The solution reverses a contiguous sublist by first locating the node just before the `start` position (the "pre" node) and the node at position `start` (the "first reversed" node). We then iterate through the segment from `start+1` to `end`, moving each node to the front of the reversed segment (right after `pre`), similar to reversing a linked list iteratively. Edge cases: if `start == end`, no changes are made. If `start == 1`, there is no `pre` node, so the head must be updated after reversal. If `end == length`, the tail’s `next` remains `nullptr`. Time complexity is O(n) for a single pass to reach `end`, and space complexity is O(1) besides the input itself.
