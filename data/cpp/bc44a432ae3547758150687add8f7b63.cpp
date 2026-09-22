// Write a C++ function `reverseInGroups(Node* head, int k)` that takes the head of a singly linked list and a positive integer `k`, and returns a new head pointer after reversing the nodes in every group of size `k`. If the number of remaining nodes is less than `k`, reverse that final partial group as well. The original linked list must be modified in place (no new nodes allocated); only pointer rewiring is allowed. The function must handle edge cases where the list is empty, `k` is 1 (no change), or `k` is larger than the list length (full reversal). The node class is provided as `struct Node { int data; Node* next; };`.

The approach uses iterative reversal of each group of `k` nodes, followed by a recursive call on the remainder of the list after that group. For each group, we traverse up to `k` nodes (or until the end of the list), reversing the `next` pointers in that segment. After reversing a group, the original first node of the group becomes the tail, and its `next` pointer must be set to the result of recursively reversing the remaining list starting from the first node after the group. The function returns the new head of the reversed segment (which is the original last node of the group). Edge cases: if the list is empty or `k <= 0`, return `head` as-is. If `k == 1` or the list has fewer than `k` nodes, the reversal reverses the entire remaining portion correctly because the recursion handles the tail. The time complexity is O(n) because each node is visited exactly once during reversal. The recursion depth is O(n/k) in the worst case (which is O(n) when `k=1` but that case does no recursion because `k==1` stops after one iteration and calls recursively with `k` on the tail, leading to depth O(n) — actually the recursion depth is O(n/k) for `k>1`, but in the worst case `k=1`, the loop reverses one node and then recurses on the rest, so recursion depth is O(n). Space complexity is O(n/k) for the recursion stack, which is O(n) in the worst case.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Reverse nodes in groups of k; if fewer than k remain, reverse the tail.
// Returns the new head of the list after reversal.
Node* reverseInGroups(Node* head, int k) {
    if (head == nullptr || k <= 1) {
        return head;
    }

    Node* current = head;
    Node* prev = nullptr;
    Node* next = nullptr;
    int count = 0;

    // Reverse first k nodes (or fewer if list ends)
    while (current != nullptr && count < k) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
        ++count;
    }

    // Recursively reverse the remaining list starting from 'next'
    if (next != nullptr) {
        head->next = reverseInGroups(next, k);
    }

    // 'prev' is the new head of this group
    return prev;
}

#include <cassert>
#include <vector>

// Helper: build a linked list from vector
Node* buildList(const std::vector<int>& values) {
    Node* dummy = new Node(0);
    Node* tail = dummy;
    for (int v : values) {
        tail->next = new Node(v);
        tail = tail->next;
    }
    Node* result = dummy->next;
    delete dummy;
    return result;
}

// Helper: convert list to vector
std::vector<int> listToVector(Node* head) {
    std::vector<int> res;
    while (head) {
        res.push_back(head->data);
        head = head->next;
    }
    return res;
}

// Helper: free list memory
void deleteList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: empty list
    assert(reverseInGroups(nullptr, 3) == nullptr);

    // Test 2: k=1, unchanged
    Node* l1 = buildList({1,2,3});
    Node* r1 = reverseInGroups(l1, 1);
    assert(listToVector(r1) == std::vector<int>({1,2,3}));
    deleteList(r1);

    // Test 3: group size 2 on even-length list
    Node* l2 = buildList({1,2,3,4});
    Node* r2 = reverseInGroups(l2, 2);
    assert(listToVector(r2) == std::vector<int>({2,1,4,3}));
    deleteList(r2);

    // Test 4: group size 3 on list length 5 (last group of 2 reversed)
    Node* l3 = buildList({1,2,3,4,5});
    Node* r3 = reverseInGroups(l3, 3);
    assert(listToVector(r3) == std::vector<int>({3,2,1,5,4}));
    deleteList(r3);

    // Test 5: k larger than list length => full reversal
    Node* l4 = buildList({1,2,3});
    Node* r4 = reverseInGroups(l4, 10);
    assert(listToVector(r4) == std::vector<int>({3,2,1}));
    deleteList(r4);

    // Test 6: single element
    Node* l5 = buildList({42});
    Node* r5 = reverseInGroups(l5, 2);
    assert(listToVector(r5) == std::vector<int>({42}));
    deleteList(r5);

    // Test 7: group size 1 with k=0 should be unchanged (k<=1 returns head)
    Node* l6 = buildList({1,2,3});
    Node* r6 = reverseInGroups(l6, 0);
    assert(listToVector(r6) == std::vector<int>({1,2,3}));
    deleteList(r6);

    // Test 8: group size 2 on odd-length list (last single node stays)
    Node* l7 = buildList({1,2,3,4,5});
    Node* r7 = reverseInGroups(l7, 2);
    assert(listToVector(r7) == std::vector<int>({2,1,4,3,5}));
    deleteList(r7);

    return 0;
}
