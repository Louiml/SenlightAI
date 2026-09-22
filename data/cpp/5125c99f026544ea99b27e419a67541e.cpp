// Write a C++ function `ListNode* partitionLinkedList(ListNode* head, int x)` that takes the head of a singly linked list (where each node contains an integer `val` and a `next` pointer, and the list may be empty) and a partition value `x`. The function must reorder the list in-place so that all nodes with values strictly less than `x` appear before all nodes with values greater than or equal to `x`, while preserving the original relative order of nodes within each partition group. Return the new head of the reordered list. The solution must use only constant extra space (i.e., no copying of nodes, no auxiliary containers), and you must define the `ListNode` struct yourself (with a constructor taking `int`). You may assume the list is acyclic.
// The approach is to process the list in one pass while maintaining two pointers: `tail`, which points to the last node in the "less than x" partition (initially `nullptr`), and `prev`, which tracks the node immediately before the current node to facilitate deletion. First, we move `tail` along the list while nodes have values `< x`; after this loop, `tail` is either the last such node or `nullptr` if the first node already has value `>= x`. Then, we iterate through the remaining nodes. When we encounter a node with value `< x`, we detach it from its current position (by linking `prev->next` to `curr->next`), then insert it right after `tail`. If `tail` is `nullptr`, the inserted node becomes the new head, and we attach the existing list after it; otherwise, we set the inserted node's `next` to `tail->next` and update `tail->next`. After insertion, update `tail` to the inserted node, and set `curr` to `prev->next` to continue scanning. If the current node value is `>= x`, simply advance `prev` and `curr`. Edge cases include an empty list or a single node (return immediately), all values `< x` (loop finds no node to move), and all values `>= x` (second loop finds no smaller nodes). The algorithm runs in O(n) time because each node is visited at most twice and each move take O(1) operations, and O(1) extra space.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Partition the list so that all nodes with value < x come before nodes with value >= x,
// preserving relative order within each group. Returns the new head.
ListNode* partitionLinkedList(ListNode* head, int x) {
    if (!head || !head->next) {
        return head;
    }

    ListNode* prev = nullptr;
    ListNode* curr = head;
    ListNode* tail = nullptr;  // last node in the "< x" partition

    // Move tail to the last node with value < x, or leave as nullptr if first node >= x.
    while (curr && curr->val < x) {
        tail = curr;
        curr = curr->next;
    }

    while (curr) {
        if (curr->val < x) {
            // Detach the current node.
            ListNode* temp = curr;
            prev->next = curr->next;

            // Insert temp after tail (or as new head if tail is null).
            if (!tail) {
                temp->next = head;   // attach existing list after temp
                head = temp;         // temp becomes new head
                tail = temp;         // update tail to temp
            } else {
                temp->next = tail->next; // connect to remainder of list
                tail->next = temp;       // link after tail
                tail = temp;             // update tail
            }

            // Continue scanning from the predecessor's next.
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    return head;
}
#include <cassert>

// Helper to build a list from a vector-like initializer list (using basic loop).
ListNode* buildList(const std::initializer_list<int>& values) {
    ListNode* head = nullptr;
    ListNode** tail = &head;
    for (int v : values) {
        *tail = new ListNode(v);
        tail = &((*tail)->next);
    }
    return head;
}

// Helper to check list order.
bool listEquals(ListNode* head, const std::initializer_list<int>& expectations) {
    const int* expected = expectations.begin();
    while (head && expected != expectations.end()) {
        if (head->val != *expected) return false;
        head = head->next;
        ++expected;
    }
    return head == nullptr && expected == expectations.end();
}

int main() {
    // Test 1: Basic partition with mixed values.
    ListNode* l1 = buildList({1, 4, 3, 2, 5, 2});
    l1 = partitionLinkedList(l1, 3);
    assert(listEquals(l1, {1, 2, 2, 4, 3, 5}));

    // Test 2: All values less than x.
    ListNode* l2 = buildList({1, 2, 3});
    l2 = partitionLinkedList(l2, 5);
    assert(listEquals(l2, {1, 2, 3}));

    // Test 3: All values greater/equal to x.
    ListNode* l3 = buildList({5, 6, 7});
    l3 = partitionLinkedList(l3, 4);
    assert(listEquals(l3, {5, 6, 7}));

    // Test 4: First node greater/equal, then a smaller node appears.
    ListNode* l4 = buildList({5, 1, 4, 2});
    l4 = partitionLinkedList(l4, 3);
    assert(listEquals(l4, {1, 2, 5, 4}));

    // Test 5: Empty list.
    ListNode* l5 = nullptr;
    assert(partitionLinkedList(l5, 3) == nullptr);

    // Test 6: Single node.
    ListNode* l6 = buildList({7});
    l6 = partitionLinkedList(l6, 3);
    assert(listEquals(l6, {7}));

    // Test 7: All equal to x.
    ListNode* l7 = buildList({3, 3, 3});
    l7 = partitionLinkedList(l7, 3);
    assert(listEquals(l7, {3, 3, 3}));

    // Test 8: Alternating smaller/larger values.
    ListNode* l8 = buildList({10, 1, 9, 2, 8, 3});
    l8 = partitionLinkedList(l8, 5);
    assert(listEquals(l8, {1, 2, 3, 10, 9, 8}));

    // Test 9: First node is smaller, then larger, then smaller again.
    ListNode* l9 = buildList({1, 5, 2});
    l9 = partitionLinkedList(l9, 3);
    assert(listEquals(l9, {1, 2, 5}));

    return 0;
}
