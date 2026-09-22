// Write a C++ function `reverseLinkedListInGroups(Node* head, int k)` that takes the head of a singly linked list and a positive integer `k`, and returns the head of the list after reversing every group of `k` nodes. If the number of nodes remaining is fewer than `k`, leave that final group unchanged (i.e., do not reverse it). For example, for a list `1->2->3->4->5->6->7` and `k=3`, the result should be `3->2->1->6->5->4->7->NULL`. The function must handle edge cases such as an empty list, a single node, `k=1` (no reversal), and a list whose length is an exact multiple of `k`. You may assume the `Node` struct is defined as in the snippet with an `int data` and a `Node* next` pointer, and the function should not modify the list's node values, only their links.
The core algorithm is a recursive, group-wise reversal of the linked list. The idea:
1. Start from the current head of the remaining list.
2. Count `k` nodes. If fewer than `k` nodes remain, return the current head unchanged (base case).
3. Otherwise, reverse the first `k` nodes using an iterative three-pointer technique (prev, curr, forward) just like reversing a normal linked list. After reversal, the original first node becomes the tail of this group, and its `next` should point to the result of recursively reversing the rest of the list.
4. Return the new head of the reversed group (which was originally the `k`-th node).

The recursion continues until the remaining part has fewer than `k` nodes. Key edge cases:
- Empty list → return `nullptr`.
- `k=1` → no reversal because the loop would reverse one node, but simpler to just return head as is (handle by a quick check).
- List length is exact multiple of `k` → all groups reversed, final group is also reversed, and recursion returns `nullptr` at the end.
- Last partial group → the base case detects fewer than `k` nodes and returns that head, so it remains in original order.

Time complexity: O(n) because each node is visited exactly once during the group counting and then again during reversal, but overall it's linear. Space complexity: O(n/k) due to the recursion stack depth, but in the worst case (k=1) it would be O(n), but since we handle k=1 specially, it's O(n/k). With a typical linked list, this is acceptable.
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val = 0) : data(val), next(nullptr) {}
};

// Reverse every group of k nodes in the linked list.
// If fewer than k nodes remain at the end, leave them as is.
// Returns the new head of the list.
Node* reverseLinkedListInGroups(Node* head, int k) {
    if (head == nullptr || k <= 1) {
        return head;
    }

    // Count nodes in the current group.
    Node* temp = head;
    int count = 0;
    while (temp != nullptr && count < k) {
        temp = temp->next;
        count++;
    }

    // If fewer than k nodes, do not reverse this group.
    if (count < k) {
        return head;
    }

    // Reverse the first k nodes iteratively.
    Node* prev = nullptr;
    Node* curr = head;
    Node* forward = nullptr;
    int steps = 0;
    while (curr != nullptr && steps < k) {
        forward = curr->next;
        curr->next = prev;
        prev = curr;
        curr = forward;
        steps++;
    }

    // Recursively reverse the rest of the list, attach the result to the tail.
    if (forward != nullptr) {
        head->next = reverseLinkedListInGroups(forward, k);
    } else {
        head->next = nullptr;
    }

    // prev points to the new head of this group.
    return prev;
}
#include <cassert>

// Helper to create a linked list from a vector for testing.
Node* createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    Node* head = new Node(vals[0]);
    Node* tail = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        tail->next = new Node(vals[i]);
        tail = tail->next;
    }
    return head;
}

// Helper to convert list to vector for comparison.
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to free all nodes.
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Normal reversal in groups.
    Node* head1 = createList({1,2,3,4,5,6,7});
    Node* result1 = reverseLinkedListInGroups(head1, 3);
    assert(listToVector(result1) == std::vector<int>({3,2,1,6,5,4,7}));
    freeList(result1);

    // Test 2: Exact multiple of k.
    Node* head2 = createList({1,2,3,4});
    Node* result2 = reverseLinkedListInGroups(head2, 2);
    assert(listToVector(result2) == std::vector<int>({2,1,4,3}));
    freeList(result2);

    // Test 3: k=1 (no reversal).
    Node* head3 = createList({5,6,7,8});
    Node* result3 = reverseLinkedListInGroups(head3, 1);
    assert(listToVector(result3) == std::vector<int>({5,6,7,8}));
    freeList(result3);

    // Test 4: Fewer than k nodes remaining at the end.
    Node* head4 = createList({1,2,3,4,5});
    Node* result4 = reverseLinkedListInGroups(head4, 4);
    assert(listToVector(result4) == std::vector<int>({4,3,2,1,5}));
    freeList(result4);

    // Test 5: Single node.
    Node* head5 = createList({42});
    Node* result5 = reverseLinkedListInGroups(head5, 1);
    assert(listToVector(result5) == std::vector<int>({42}));
    freeList(result5);

    // Test 6: Empty list.
    Node* head6 = nullptr;
    Node* result6 = reverseLinkedListInGroups(head6, 3);
    assert(result6 == nullptr);

    // Test 7: k greater than list length.
    Node* head7 = createList({1,2,3});
    Node* result7 = reverseLinkedListInGroups(head7, 5);
    assert(listToVector(result7) == std::vector<int>({1,2,3}));
    freeList(result7);

    // Test 8: Exact multiple of k but k=1.
    Node* head8 = createList({9,9,7});
    Node* result8 = reverseLinkedListInGroups(head8, 1);
    assert(listToVector(result8) == std::vector<int>({9,9,7}));
    freeList(result8);

    // Test 9: List with duplicate values.
    Node* head9 = createList({0,0,9,9,9,7});
    Node* result9 = reverseLinkedListInGroups(head9, 2);
    assert(listToVector(result9) == std::vector<int>({0,0,9,9,9,7}));
    freeList(result9);

    return 0;
}
