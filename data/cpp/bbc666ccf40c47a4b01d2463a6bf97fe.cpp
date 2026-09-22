// Write a C++ function that takes the head of a singly linked list and returns the head of the reversed list. The list nodes are defined as `struct ListNode { int val; ListNode *next; };` with constructors for flexibility. The function must not allocate any new nodes; it must reverse the list by rearranging the existing nodes' `next` pointers. Handle an empty list (nullptr head) and a single-node list correctly. The solution should be iterative, not recursive, and must preserve the values and order of the nodes in the reversed list.

#include <cassert>

int main() {
    // Helper to create a list from an initializer list for testing
    ListNode* createList(std::initializer_list<int> values) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        for (int v : values) {
            tail->next = new ListNode(v);
            tail = tail->next;
        }
        return dummy.next;
    }

    // Helper to free the list
    void deleteList(ListNode* head) {
        while (head) {
            ListNode* temp = head->next;
            delete head;
            head = temp;
        }
    }

    // Helper to compare two lists by values
    bool listsEqual(ListNode* a, ListNode* b) {
        while (a && b) {
            if (a->val != b->val) return false;
            a = a->next;
            b = b->next;
        }
        return (a == nullptr && b == nullptr);
    }

    // Test 1: empty list
    assert(reverseLinkedList(nullptr) == nullptr);

    // Test 2: single node
    ListNode* single = new ListNode(5);
    ListNode* revSingle = reverseLinkedList(single);
    assert(revSingle == single);
    assert(revSingle->val == 5);
    assert(revSingle->next == nullptr);
    deleteList(revSingle); // just deletes single

    // Test 3: multiple nodes [1,2,3]
    ListNode* list1 = createList({1,2,3});
    ListNode* rev1 = reverseLinkedList(list1);
    ListNode* expected1 = createList({3,2,1});
    assert(listsEqual(rev1, expected1));
    deleteList(rev1);
    deleteList(expected1);

    // Test 4: two nodes [10,20]
    ListNode* list2 = createList({10,20});
    ListNode* rev2 = reverseLinkedList(list2);
    ListNode* expected2 = createList({20,10});
    assert(listsEqual(rev2, expected2));
    deleteList(rev2);
    deleteList(expected2);

    // Test 5: list with negative and duplicate values [-3,0,0,7]
    ListNode* list3 = createList({-3,0,0,7});
    ListNode* rev3 = reverseLinkedList(list3);
    ListNode* expected3 = createList({7,0,0,-3});
    assert(listsEqual(rev3, expected3));
    deleteList(rev3);
    deleteList(expected3);

    return 0;
}

#include <cstddef> // for nullptr

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverse a singly linked list iteratively and return the new head.
ListNode* reverseLinkedList(ListNode* head) {
    ListNode* current = head;
    ListNode* previous = nullptr;
    ListNode* nextTemp = nullptr;

    while (current != nullptr) {
        nextTemp = current->next; // save next node
        current->next = previous; // reverse the link
        previous = current;       // move previous forward
        current = nextTemp;       // move current forward
    }
    return previous; // new head of the reversed list
}

// The algorithm uses three pointers: `current` (to traverse the original list), `previous` (to build the reversed part), and `nextTemp` (to save the reference to the next node before breaking the link). Starting with `current = head` and `previous = nullptr`, we iterate while `current` is not null. In each iteration, we store `nextTemp = current->next` so we don't lose the rest of the list. Then we set `current->next = previous`, which reverses the link. Finally, we advance `previous` to `current` and `current` to `nextTemp`. After the loop, `previous` points to the new head of the reversed list. Edge cases: if the list is empty, the loop never runs and we return `nullptr`. If there is only one node, after one iteration `previous` becomes that node and `current` becomes `nullptr`, correctly returning the single node. Time complexity is O(n) where n is the number of nodes, because we visit each node exactly once. Space complexity is O(1) because we use only a constant number of pointer variables.
