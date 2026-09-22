Write a C++ function `ListNode* reverseEveryTwo(ListNode* head)` that takes the head of a singly linked list and swaps every two adjacent nodes, i.e., the first and second nodes swap, the third and fourth nodes swap, and so on. If the list has an odd number of nodes, the last node remains in its original position. The function should return the new head of the modified list. The input list may be empty or contain up to `10^5` nodes. The node values are integers. The solution must not allocate new nodes—only rearrange pointers. The function should be `const`-correct with respect to the list structure (though the list itself is mutable) and must not use recursion (to avoid stack overflow on long lists). Time complexity must be O(n) and auxiliary space O(1).
#include <cassert>

ListNode* makeList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    ListNode* head = dummy->next;
    delete dummy;
    return head;
}

bool listEqual(ListNode* head, std::initializer_list<int> expected) {
    ListNode* p = head;
    for (int v : expected) {
        if (p == nullptr || p->val != v) return false;
        p = p->next;
    }
    return p == nullptr;
}

int main() {
    // Test 1: empty list
    assert(reverseEveryTwo(nullptr) == nullptr);

    // Test 2: single node
    ListNode* a = new ListNode(1);
    assert(reverseEveryTwo(a) == a);
    assert(a->val == 1);

    // Test 3: even length
    ListNode* head3 = makeList({1, 2, 3, 4});
    head3 = reverseEveryTwo(head3);
    assert(listEqual(head3, {2, 1, 4, 3}));

    // Test 4: odd length
    ListNode* head4 = makeList({1, 2, 3, 4, 5});
    head4 = reverseEveryTwo(head4);
    assert(listEqual(head4, {2, 1, 4, 3, 5}));

    // Test 5: two nodes
    ListNode* head5 = makeList({1, 2});
    head5 = reverseEveryTwo(head5);
    assert(listEqual(head5, {2, 1}));

    // Test 6: five nodes with duplicates
    ListNode* head6 = makeList({7, 7, 8, 8, 9});
    head6 = reverseEveryTwo(head6);
    assert(listEqual(head6, {7, 7, 8, 8, 9}));

    // Test 7: large even length (1000 nodes)
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int i = 1; i <= 1000; ++i) {
        tail->next = new ListNode(i);
        tail = tail->next;
    }
    ListNode* head7 = dummy->next;
    delete dummy;
    head7 = reverseEveryTwo(head7);
    ListNode* p = head7;
    for (int i = 2; i <= 1000; i += 2) {
        assert(p->val == i); p = p->next;
        assert(p->val == i - 1); p = p->next;
    }
    
    return 0;
}
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Swap every two adjacent nodes in the linked list.
ListNode* reverseEveryTwo(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    ListNode* newHead = head->next;
    ListNode* prevTail = nullptr;
    ListNode* first = head;
    
    while (first != nullptr && first->next != nullptr) {
        ListNode* second = first->next;
        ListNode* nextPair = second->next;
        
        // Rewire the pair
        if (prevTail != nullptr) {
            prevTail->next = second;
        }
        first->next = nextPair;
        second->next = first;
        
        // Advance
        prevTail = first;
        first = nextPair;
    }
    
    return newHead;
}
// The approach uses three pointers: `first` (the current node), `second` (the next node), and `prevTail` which points to the last node of the already processed part. At each step, we swap the pair (`first`, `second`) by making `prevTail->next` point to `second`, then `first->next` point to `second->next`, and finally `second->next` point to `first`. After the swap, `prevTail` is updated to `first` (since `first` now becomes the tail of the processed segment), and we move `first` to the original `second->next` (saved before rewiring). If there is an odd tail, it remains untouched. Edge cases: empty list or single node → return as is; list with even count → all pairs swapped; the head update is done once by capturing the second node as the new head before the loop. Complexity: each node is visited once, so O(n) time; only a constant number of pointers are used, so O(1) space.
