Given a singly linked list of integers and a positive integer `K`, write a C++ function that removes the K-th node from the end of the list. If the list is empty or has only one node, or if `K` is larger than the list length, return the appropriate modified list according to the following rules: an empty list stays empty, a single-node list becomes empty, and if `K` exceeds the list length, return the original list unchanged. The function should return the new head pointer of the list after removal. You must not use any extra data structures like arrays or vectors; only pointer manipulation is allowed. The list nodes are defined with integer `data` and a `next` pointer. Handle edge cases where `K` equals the list length (removing the head) correctly.
// The solution uses the two-pointer (fast and slow) technique with a dummy node. Start both pointers at a dummy node whose `next` points to the original head. First, advance the fast pointer `K` steps forward. If during this advance the fast pointer reaches `NULL` before completing `K` steps, then `K` is larger than the list length, so return the original head unchanged. If the fast pointer successfully advances `K` steps, then both pointers are separated by exactly `K` nodes. Now move both pointers one step at a time until fast reaches the last node (i.e., `fast->next == NULL`). At that point, the slow pointer will be positioned exactly one node before the target node to be removed. Then update `slow->next = slow->next->next` to delete the target. Finally, return `dummy->next`, which is the new head (this handles the case where the head itself is deleted, since the dummy node still points to the updated head). Edge cases: empty list returns null; single-node list returns null (since removing the only node leaves empty); if `K` is larger than list length, the original list is returned. The algorithm runs in `O(n)` time (where `n` is the number of nodes) and uses `O(1)` auxiliary space.
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val = 0, Node* nxt = nullptr) : data(val), next(nxt) {}
};

// Removes the K-th node from the end of a singly linked list.
// Returns the new head pointer. If K is larger than list length, returns original head.
Node* removeKthNodeFromEnd(Node* head, int K) {
    // Empty list or K non-positive: nothing to remove
    if (head == nullptr || K <= 0) {
        return head;
    }

    // Dummy node simplifies removing the head
    Node* dummy = new Node(0, head);
    Node* fast = dummy;
    Node* slow = dummy;

    // Advance fast by K steps
    for (int i = 0; i < K; ++i) {
        if (fast == nullptr) {
            // K exceeds list length, return original head
            delete dummy;
            return head;
        }
        fast = fast->next;
    }

    // If K is exactly equal to list length, fast becomes nullptr after K steps.
    // In that case, we need to handle specifically: remove the head.
    if (fast == nullptr) {
        Node* newHead = head->next;
        delete head;
        delete dummy;
        return newHead;
    }

    // Move both until fast reaches the last node
    while (fast->next != nullptr) {
        fast = fast->next;
        slow = slow->next;
    }

    // slow now points to the node before the target
    Node* toDelete = slow->next;
    slow->next = toDelete->next;
    delete toDelete;

    Node* newHead = dummy->next;
    delete dummy;
    return newHead;
}
#include <cassert>

int main() {
    // Helper to create a list from initializer list (only for testing)
    auto makeList = [](std::initializer_list<int> vals) {
        Node* head = nullptr;
        Node* tail = nullptr;
        for (int v : vals) {
            Node* n = new Node(v);
            if (!head) head = tail = n;
            else { tail->next = n; tail = n; }
        }
        return head;
    };

    // Helper to convert list to vector (for easy comparison)
    auto toVector = [](Node* head) {
        std::vector<int> result;
        while (head) { result.push_back(head->data); head = head->next; }
        return result;
    };

    // Test 1: Remove 2nd from end in [1,2,3,4,5] -> [1,2,3,5]
    Node* head = makeList({1,2,3,4,5});
    head = removeKthNodeFromEnd(head, 2);
    assert((toVector(head) == std::vector<int>{1,2,3,5}));

    // Test 2: Remove 1st from end (last node) in [1,2,3] -> [1,2]
    head = makeList({1,2,3});
    head = removeKthNodeFromEnd(head, 1);
    assert((toVector(head) == std::vector<int>{1,2}));

    // Test 3: Remove K equal to list length (head removal) in [1,2,3] -> [2,3]
    head = makeList({1,2,3});
    head = removeKthNodeFromEnd(head, 3);
    assert((toVector(head) == std::vector<int>{2,3}));

    // Test 4: Single node, remove 1st from end -> empty
    head = makeList({7});
    head = removeKthNodeFromEnd(head, 1);
    assert(head == nullptr);

    // Test 5: Empty list -> remains nullptr
    head = nullptr;
    head = removeKthNodeFromEnd(head, 2);
    assert(head == nullptr);

    // Test 6: K larger than list length, return original
    head = makeList({1,2});
    Node* original = head;
    head = removeKthNodeFromEnd(head, 5);
    assert(head == original); // same pointer, no modification

    // Test 7: K = 0 or negative -> unchanged
    head = makeList({10,20,30});
    original = head;
    head = removeKthNodeFromEnd(head, 0);
    assert(head == original);

    return 0;
}
