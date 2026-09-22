Write a C++ function that reverses a segment of a singly-linked list between given 1-based positions `left` and `right` (inclusive), leaving all other nodes in their original order. The function must accept a pointer to the head node and the two positions, and return the new head of the modified list. The list is defined by a standard `ListNode` struct with `int val` and `ListNode* next`, including default and parameterized constructors. You may assume `1 <= left <= right <= length of list`, the list is non-empty, and the list has no cycles. The reversal must be done in-place without allocating new nodes, and you cannot use any container like `std::vector` to store nodes — only pointer manipulation is allowed.

#include <cassert>

// Helper to build a list from an initializer list for testing.
ListNode* buildList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy->next;
}

// Helper to convert a list to a vector for easy comparison.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Reverse middle segment
    ListNode* list1 = buildList({1,2,3,4,5});
    ListNode* result1 = reverseBetween(list1, 2, 4);
    assert((listToVector(result1) == std::vector<int>{1,4,3,2,5}));

    // Test 2: Reverse entire list (left=1, right=5)
    ListNode* list2 = buildList({1,2,3,4,5});
    ListNode* result2 = reverseBetween(list2, 1, 5);
    assert((listToVector(result2) == std::vector<int>{5,4,3,2,1}));

    // Test 3: Reverse only the head segment (left=1, right=2)
    ListNode* list3 = buildList({1,2,3});
    ListNode* result3 = reverseBetween(list3, 1, 2);
    assert((listToVector(result3) == std::vector<int>{2,1,3}));

    // Test 4: Reverse only the tail segment (left=4, right=5)
    ListNode* list4 = buildList({1,2,3,4,5});
    ListNode* result4 = reverseBetween(list4, 4, 5);
    assert((listToVector(result4) == std::vector<int>{1,2,3,5,4}));

    // Test 5: left == right (no change)
    ListNode* list5 = buildList({1,2,3,4,5});
    ListNode* result5 = reverseBetween(list5, 3, 3);
    assert((listToVector(result5) == std::vector<int>{1,2,3,4,5}));

    // Test 6: Single element list
    ListNode* list6 = buildList({42});
    ListNode* result6 = reverseBetween(list6, 1, 1);
    assert((listToVector(result6) == std::vector<int>{42}));

    // Test 7: Two-element list, full reverse
    ListNode* list7 = buildList({7,8});
    ListNode* result7 = reverseBetween(list7, 1, 2);
    assert((listToVector(result7) == std::vector<int>{8,7}));

    // Test 8: Reverse segment at the very beginning of a longer list (left=1, right=3)
    ListNode* list8 = buildList({10,20,30,40});
    ListNode* result8 = reverseBetween(list8, 1, 3);
    assert((listToVector(result8) == std::vector<int>{30,20,10,40}));
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

// Reverse the sublist from `start` to `end` (inclusive) recursively.
// Precondition: `start` and `end` are nodes in the same list, with `start` reachable from `end` by following `next`.
static ListNode* reverseSublist(ListNode* start, ListNode* end) {
    if (start == end) {
        return end;
    }
    ListNode* newHead = reverseSublist(start->next, end);
    start->next->next = start;  // make the next node point back to start
    start->next = nullptr;      // temporarily detach start
    return newHead;
}

// Reverse the nodes between positions `left` and `right` (1-based, inclusive) in the list starting at `head`.
// Returns the new head of the list.
ListNode* reverseBetween(ListNode* head, int left, int right) {
    // No reversal needed for empty list, single node, or equal positions.
    if (head == nullptr || head->next == nullptr || left == right) {
        return head;
    }

    // Find node at position `left` and its predecessor.
    ListNode* prev = nullptr;
    ListNode* current = head;
    for (int i = 1; i < left; ++i) {
        prev = current;
        current = current->next;
    }
    ListNode* leftNode = current;  // node at position left

    // Find node at position `right`.
    ListNode* last = leftNode;
    for (int i = left; i < right; ++i) {
        last = last->next;
    }
    ListNode* rightNode = last;
    ListNode* suffix = rightNode->next;  // node after the reversed segment

    // Reverse the segment.
    ListNode* reversedHead = reverseSublist(leftNode, rightNode);

    // Reconnect the prefix.
    if (prev != nullptr) {
        prev->next = reversedHead;
    }
    // Reconnect the suffix.
    leftNode->next = suffix;

    // If left == 1, the head of the whole list is the new reversed head.
    return (left == 1) ? reversedHead : head;
}

// The core idea is to first locate the node at position `left` and its predecessor. Then locate the node at position `right` and save the node that follows it (the suffix). After identifying the boundaries, recursively reverse the sublist starting at `left` and ending at `right`. The recursive reversal function takes the start and end nodes: if they are the same, it returns immediately; otherwise it recursively reverses `start->next` to `end`, then attaches `start` to the end of the reversed segment and sets `start->next` to `nullptr`. This returns the new head of the reversed segment, which is the original `right` node. After the reversal, reconnect the prefix (if any) to the new head of the reversed segment, and connect the tail (original `right->next`) to the node that was originally at `left` (now at the end of the reversed segment). If `left == 1`, the new head is the reversed segment’s head; otherwise the original head remains. Edge cases include `left == right` (no reversal needed), a single-node list, or a segment that includes the head or the tail. The algorithm runs in O(n) time because we traverse the list at most twice (once to find `left` and `right`, plus recursion over the segment), and uses O(k) stack space for the recursion, where k is the length of the reversed segment (worst-case O(n)).
