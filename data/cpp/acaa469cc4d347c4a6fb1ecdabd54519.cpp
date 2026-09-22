// Write a C++ function that takes the head of a singly linked list and returns the head of the reversed list. The linked list nodes are defined as `struct ListNode { int val; ListNode *next; };` with constructors. The function must reverse the list iteratively, without using recursion or modifying the values inside nodes—only the `next` pointers should be changed. The input list is guaranteed to be non-cyclic. Edge cases include an empty list (`head == nullptr`) and a list with a single node, which should both be returned unchanged. The function should have signature `ListNode* reverseList(ListNode* head)`.
#include <cassert>

// Helper to create a list from an initializer list
ListNode* createList(std::initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to delete a list
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head->next;
        delete head;
        head = temp;
    }
}

// Helper to compare lists
bool listsEqual(ListNode* a, ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

int main() {
    // Empty list
    assert(reverseList(nullptr) == nullptr);

    // Single node
    ListNode* single = new ListNode(5);
    assert(reverseList(single) == single);
    assert(single->next == nullptr);
    delete single;

    // General cases
    ListNode* list1 = createList({1,2,3,4,5});
    ListNode* rev1 = reverseList(list1);
    assert(listsEqual(rev1, createList({5,4,3,2,1})));
    deleteList(rev1);

    ListNode* list2 = createList({1});
    ListNode* rev2 = reverseList(list2);
    assert(listsEqual(rev2, createList({1})));
    deleteList(rev2);

    ListNode* list3 = createList({1,2});
    ListNode* rev3 = reverseList(list3);
    assert(listsEqual(rev3, createList({2,1})));
    deleteList(rev3);

    ListNode* list4 = createList({1,2,3});
    ListNode* rev4 = reverseList(list4);
    assert(listsEqual(rev4, createList({3,2,1})));
    deleteList(rev4);
}
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverses a singly linked list iteratively and returns the new head.
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* nextNode = current->next;
        current->next = prev;
        prev = current;
        current = nextNode;
    }
    return prev;
}
// The iterative reversal approach traverses the list once, maintaining three pointers: a `prev` pointer (initialized to `nullptr`), a `current` pointer (starting at `head`), and a `next` pointer to preserve the remainder of the list before rewiring. For each node, we save `current->next` into `next`, then set `current->next = prev`, then move `prev` to `current` and `current` to `next`. After the loop ends, `prev` points to the new head of the reversed list, which is returned. For an empty list or a single-node list, the loop naturally handles it: the empty list returns `nullptr`, and the single-node loop runs once, setting its `next` to `nullptr` and returning that node. Edge cases include `nullptr` input, single node, and robust handling without memory leaks (no dynamic allocation). Time complexity is O(n) because each node is visited once. Space complexity is O(1) auxiliary space, using only a constant number of pointers.
