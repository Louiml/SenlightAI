// Given a non-empty singly-linked list of integers, write a C++ function that removes the middle node and returns the head of the resulting list. If the list has an even number of nodes, the middle node is the second of the two central nodes (i.e., the node at index `n/2` when using 0-based indexing, where `n` is the list length). For a list of length 1, the result should be an empty list (nullptr). The original list may be modified in place, and no extra nodes should be allocated except when replacing the head with `nullptr` for a single-node list. The function signature must match: `ListNode* deleteMiddle(ListNode* head)`.

#include <cassert>

// Helper to build a list from a vector of values.
ListNode* buildList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* tail = head;
    for (size_t i = 1; i < values.size(); ++i) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }
    return head;
}

// Helper to convert a list to a vector of values.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to free list memory.
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Single node -> empty.
    ListNode* list1 = buildList({5});
    ListNode* result1 = deleteMiddle(list1);
    assert(result1 == nullptr);
    freeList(result1); // nullptr safe

    // Test 2: Two nodes -> remove second node.
    ListNode* list2 = buildList({1, 2});
    ListNode* result2 = deleteMiddle(list2);
    assert(listToVector(result2) == std::vector<int>({1}));
    freeList(result2);

    // Test 3: Three nodes -> remove middle (index 1).
    ListNode* list3 = buildList({10, 20, 30});
    ListNode* result3 = deleteMiddle(list3);
    assert(listToVector(result3) == std::vector<int>({10, 30}));
    freeList(result3);

    // Test 4: Four nodes -> remove second of two central (index 2).
    ListNode* list4 = buildList({1, 2, 3, 4});
    ListNode* result4 = deleteMiddle(list4);
    assert(listToVector(result4) == std::vector<int>({1, 2, 4}));
    freeList(result4);

    // Test 5: Five nodes -> remove middle (index 2).
    ListNode* list5 = buildList({5, 4, 3, 2, 1});
    ListNode* result5 = deleteMiddle(list5);
    assert(listToVector(result5) == std::vector<int>({5, 4, 2, 1}));
    freeList(result5);

    // Test 6: Even length 6 -> remove index 3.
    ListNode* list6 = buildList({0, 1, 2, 3, 4, 5});
    ListNode* result6 = deleteMiddle(list6);
    assert(listToVector(result6) == std::vector<int>({0, 1, 2, 4, 5}));
    freeList(result6);
}

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Removes the middle node (second of the two central nodes if even length)
// from a non-empty linked list and returns the head of the modified list.
// For a single-node list, returns nullptr.
ListNode* deleteMiddle(ListNode* head) {
    // Count total number of nodes.
    int length = 0;
    for (ListNode* current = head; current != nullptr; current = current->next) {
        ++length;
    }
    
    // Single-node list becomes empty.
    if (length == 1) {
        return nullptr;
    }
    
    // Middle index is length/2 (0-based). Need to stop at the node before it.
    int stepsToMiddle = length / 2;
    
    // Traverse to the node just before the middle.
    ListNode* prev = head;
    while (stepsToMiddle > 1) {
        prev = prev->next;
        --stepsToMiddle;
    }
    
    // Unlink the middle node (prev->next is the middle).
    ListNode* middleNode = prev->next;
    prev->next = middleNode->next;
    
    return head;
}

// The task requires deleting the node at the exact middle position based on the list's total length. The simplest robust approach involves a two-pass method: first, traverse the entire list to count the total number of nodes `n`. The middle index is `mid = n/2` (integer division). If `n == 1`, the head itself is the middle node, so we return `nullptr` (since the list becomes empty). Otherwise, we traverse the list again, moving a pointer until it points to the node just before the middle node (i.e., we stop when `mid` becomes 1, because we need to skip the node at index `mid`). Then we update the `next` pointer of that predecessor to skip the middle node, effectively unlinking it. We do not explicitly delete the node (to keep the task focused on pointer manipulation, though in production one should `delete` it). Edge cases include: a single-node list (return nullptr), a two-node list (middle is the second node, so we remove the tail), and lists with more nodes. Time complexity is O(n) for counting and O(n) for the second traversal, so overall O(n) with a constant number of passes (2 passes). Space complexity is O(1) auxiliary, as we only use a few pointer variables.
