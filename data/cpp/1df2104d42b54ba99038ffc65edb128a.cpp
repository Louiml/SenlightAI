/*
Write a C++ function `rotateRight(ListNode* head, int k)` that rotates a singly linked list to the right by `k` positions. The function takes the head of a linked list and a non-negative integer `k`, and returns the new head of the rotated list. Rotation to the right means that the last `k` nodes move to the front in their original relative order. If `k` is greater than the length of the list, only `k % length` rotations are effectively needed. The function must handle empty lists (returning `nullptr`), single-node lists, and cases where `k` is a multiple of the list length (in which case the list is unchanged). The solution must modify the list in place and not allocate new nodes. Use the provided `ListNode` structure with `int val` and `ListNode* next`.
*/
#include <cstddef>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Rotate a singly linked list to the right by k positions.
// Returns the new head of the list. The list is modified in place.
ListNode* rotateRight(ListNode* head, int k) {
    if (head == nullptr) {
        return nullptr;
    }
    
    // Count the number of nodes and find the last node.
    int length = 1;
    ListNode* tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
        ++length;
    }
    
    // Effective rotations needed.
    int effectiveK = k % length;
    if (effectiveK == 0) {
        return head;
    }
    
    // Form a circular list by connecting tail to head.
    tail->next = head;
    
    // Find the new tail: after (length - effectiveK - 1) steps from head.
    int stepsToNewTail = length - effectiveK - 1;
    ListNode* newTail = head;
    while (stepsToNewTail > 0) {
        newTail = newTail->next;
        --stepsToNewTail;
    }
    
    // The new head is the node after newTail.
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;
    
    return newHead;
}
#include <cassert>

// Helper to build a list from a vector-like array (for testing only).
ListNode* buildList(const std::initializer_list<int>& values) {
    ListNode* dummy = new ListNode(0);
    ListNode* current = dummy;
    for (int val : values) {
        current->next = new ListNode(val);
        current = current->next;
    }
    return dummy->next;
}

// Helper to convert list to vector-like array (for testing only).
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: rotate by 2 on [1,2,3,4,5] -> [4,5,1,2,3]
    ListNode* l1 = buildList({1,2,3,4,5});
    ListNode* r1 = rotateRight(l1, 2);
    assert(listToVector(r1) == std::vector<int>({4,5,1,2,3}));
    
    // Test 2: rotate by 0 -> unchanged
    ListNode* l2 = buildList({1,2,3});
    ListNode* r2 = rotateRight(l2, 0);
    assert(listToVector(r2) == std::vector<int>({1,2,3}));
    
    // Test 3: rotate by length (multiple) -> unchanged
    ListNode* l3 = buildList({1,2,3});
    ListNode* r3 = rotateRight(l3, 3);
    assert(listToVector(r3) == std::vector<int>({1,2,3}));
    
    // Test 4: rotate by large k on [1,2,3] with k=5 (effective 2) -> [2,3,1]
    ListNode* l4 = buildList({1,2,3});
    ListNode* r4 = rotateRight(l4, 5);
    assert(listToVector(r4) == std::vector<int>({2,3,1}));
    
    // Test 5: empty list -> nullptr
    ListNode* r5 = rotateRight(nullptr, 10);
    assert(r5 == nullptr);
    
    // Test 6: single node [7] with k=5 -> [7]
    ListNode* l6 = buildList({7});
    ListNode* r6 = rotateRight(l6, 5);
    assert(listToVector(r6) == std::vector<int>({7}));
    
    // Test 7: rotate by 1 on [1,2,3] -> [3,1,2]
    ListNode* l7 = buildList({1,2,3});
    ListNode* r7 = rotateRight(l7, 1);
    assert(listToVector(r7) == std::vector<int>({3,1,2}));
    
    return 0;
}
// The core idea is to first determine the length of the list by traversing it once, while also keeping a pointer to the last node. If the list is empty, return `nullptr` immediately. If `k % length == 0`, no rotation is needed, so return the original head. Otherwise, we connect the last node's `next` to the original head to form a circular list. Then compute the number of steps needed to find the new tail: it is `length - (k % length) - 1` steps from the original head (since we need to break the link just before the new head). Traverse from the original head that many times, set the new head to the node after that position, break the link by setting the new tail's `next` to `nullptr`, and return the new head. Edge cases include empty list, `k = 0`, `k` being a multiple of length, and `k` very large. Time complexity is O(n) for the two traversals (counting and finding the split point), and space complexity is O(1) beyond the input list.
