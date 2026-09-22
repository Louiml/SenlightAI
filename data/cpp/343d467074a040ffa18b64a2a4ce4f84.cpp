Write a C++ function `mergeZeroNodes(ListNode* head)` that takes the head of a singly-linked list whose node values are non-negative integers, where the list always starts and ends with a node of value `0` and contains exactly one `0` node between consecutive groups of non-zero positive integers. The function must modify the list in-place so that each block of non-zero values between two `0` nodes is replaced by a single node containing the sum of that block, and the original `0` nodes are removed (except that the returned head is the first summed node, not the original leading `0`). The final list should contain only the summed values, in their original order, with no `0` nodes at all. For example, given `0 -> 3 -> 1 -> 0 -> 4 -> 5 -> 2 -> 0`, the function should return a list with values `4 -> 11`. Your implementation must not allocate new nodes; it must reuse the existing nodes, and it must handle edge cases gracefully, such as a list with a single non-zero block (e.g., `0 -> 7 -> 0` returning `7`). You may assume the input list is valid as described.

The main algorithm works by skipping the initial `0` node, then iterating through the list with a pointer `current` that represents the start of the next block. For each block, we use a second pointer `end` to traverse until it hits a `0` node, accumulating the sum of all non-zero values in that block. After the inner loop stops, `end` points at the separating `0`. We set `current->val` to the accumulated sum, and then link `current->next` directly to `end->next`, effectively removing both the trailing `0` and all nodes in the block except the first (which now holds the sum). Then we advance `current` to `current->next` for the next block. The process repeats until `current` becomes `nullptr`, at which point all nodes have been processed and the updated head points to the first summed node. Important edge cases include: a list with exactly one block (e.g., `0 -> x -> 0`); lists where a block has only one non-zero value; and consecutive `0` nodes never occur by problem constraints. The algorithm runs in O(n) time because each node is visited at most twice (once in the outer pointer, once in the inner traversal), and uses O(1) extra space beyond the list itself, since we only manipulate pointers and an integer sum.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// Given a list starting and ending with 0 and with 0 between blocks,
// merge each block of non-zero nodes into a single summed node.
ListNode* mergeZeroNodes(ListNode* head) {
    // Skip the initial zero node.
    head = head->next;

    ListNode* current = head;

    while (current != nullptr) {
        ListNode* end = current;
        int sum = 0;

        // Accumulate all non-zero values until the next zero.
        while (end->val != 0) {
            sum += end->val;
            end = end->next;
        }

        // Store the sum in the current node and skip the rest of the block
        // plus the zero node that ended it.
        current->val = sum;
        current->next = end->next;

        // Move to the next block's first node (which may be nullptr).
        current = current->next;
    }

    return head;
}

#include <cassert>

// Helper to build a list from an initializer list.
ListNode* makeList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy->next;
}

// Helper to compare list values to a vector.
bool listEquals(ListNode* head, std::initializer_list<int> expected) {
    ListNode* p = head;
    for (int v : expected) {
        if (p == nullptr || p->val != v) return false;
        p = p->next;
    }
    return p == nullptr;
}

int main() {
    // Case 1: 0 -> 3 -> 1 -> 0 -> 4 -> 5 -> 2 -> 0
    ListNode* l1 = makeList({0, 3, 1, 0, 4, 5, 2, 0});
    ListNode* r1 = mergeZeroNodes(l1);
    assert(listEquals(r1, {4, 11}));

    // Case 2: Single block: 0 -> 7 -> 0
    ListNode* l2 = makeList({0, 7, 0});
    ListNode* r2 = mergeZeroNodes(l2);
    assert(listEquals(r2, {7}));

    // Case 3: Single element block: 0 -> 1 -> 0 -> 2 -> 0 -> 3 -> 0
    ListNode* l3 = makeList({0, 1, 0, 2, 0, 3, 0});
    ListNode* r3 = mergeZeroNodes(l3);
    assert(listEquals(r3, {1, 2, 3}));

    // Case 4: Larger sums and multiple blocks
    ListNode* l4 = makeList({0, 10, 20, 0, 5, 0, 100, 200, 300, 0});
    ListNode* r4 = mergeZeroNodes(l4);
    assert(listEquals(r4, {30, 5, 600}));

    // Case 5: Only a single zero? Not allowed by spec, but ensure no crash if head->next is null.
    ListNode* l5 = makeList({0});
    ListNode* r5 = mergeZeroNodes(l5);
    assert(r5 == nullptr);

    return 0;
}
