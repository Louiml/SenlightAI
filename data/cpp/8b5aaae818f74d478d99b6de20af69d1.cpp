/*
Write a standalone C++ function that implements a singly linked list reversal using both a recursive and an iterative approach. The function should take a linked list's head pointer (where the list uses the `SingleNode` structure with `int data` and `SingleNode* next` fields, and a sentinel head node is used as in typical implementations), and return the new head pointer after reversal. The reversal must properly handle empty lists, single-node lists, and lists with cycles (where reversal is undefined—return the original head in that case). The function should not allocate new nodes; it must rearrange pointers in place. Provide two separate functions: `reverseIterative` and `reverseRecursive`, each accepting a `SingleNode*` head and returning a `SingleNode*` to the new head. The functions must be const-correct where applicable (though pointer mutation is needed, so no const on the parameters). Ensure the sentinel head node's `next` is correctly updated after reversal so that the list remains traversable from the sentinel.
*/
#include <cstddef>

// Node structure from the given context
struct SingleNode {
    int data;
    SingleNode* next;
};

// Helper to detect if the list starting from head (excluding sentinel) has a cycle
bool hasCycle(SingleNode* head) {
    if (head == nullptr || head->next == nullptr) return false;
    SingleNode* slow = head->next;
    SingleNode* fast = head->next;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// Iterative reversal of a singly linked list with a sentinel head node.
// Returns the new head pointer (the sentinel's next after reversal).
SingleNode* reverseIterative(SingleNode* sentinel) {
    if (sentinel == nullptr || sentinel->next == nullptr) return sentinel;
    if (hasCycle(sentinel)) return sentinel; // cannot reverse cyclic list

    SingleNode* prev = nullptr;
    SingleNode* curr = sentinel->next;
    SingleNode* next = nullptr;

    while (curr != nullptr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    sentinel->next = prev; // new head
    return sentinel;
}

// Recursive helper that reverses the list starting at 'node' and returns the new head.
SingleNode* reverseRecursiveHelper(SingleNode* node) {
    if (node == nullptr || node->next == nullptr) return node;
    SingleNode* newHead = reverseRecursiveHelper(node->next);
    node->next->next = node;
    node->next = nullptr;
    return newHead;
}

// Recursive reversal with sentinel head node.
// Returns the new head pointer (the sentinel's next after reversal).
SingleNode* reverseRecursive(SingleNode* sentinel) {
    if (sentinel == nullptr || sentinel->next == nullptr) return sentinel;
    if (hasCycle(sentinel)) return sentinel; // cannot reverse cyclic list

    sentinel->next = reverseRecursiveHelper(sentinel->next);
    return sentinel;
}
#include <cassert>
#include <cstdlib>

// Helper to create a list from an array (sentinel as first node)
SingleNode* createList(int arr[], int n) {
    SingleNode* sentinel = new SingleNode{0, nullptr};
    SingleNode* tail = sentinel;
    for (int i = 0; i < n; ++i) {
        tail->next = new SingleNode{arr[i], nullptr};
        tail = tail->next;
    }
    return sentinel;
}

// Helper to check if two lists have same values (starting from sentinel->next)
bool compareLists(SingleNode* sentinel1, int arr[], int n) {
    SingleNode* p = sentinel1->next;
    for (int i = 0; i < n; ++i) {
        if (p == nullptr || p->data != arr[i]) return false;
        p = p->next;
    }
    return p == nullptr;
}

// Helper to clean up list
void deleteList(SingleNode* sentinel) {
    SingleNode* p = sentinel;
    while (p != nullptr) {
        SingleNode* temp = p;
        p = p->next;
        delete temp;
    }
}

int main() {
    // Test 1: Iterative reversal of even-length list
    int arr1[] = {1,2,3,4};
    SingleNode* list1 = createList(arr1, 4);
    reverseIterative(list1);
    int expected1[] = {4,3,2,1};
    assert(compareLists(list1, expected1, 4));
    deleteList(list1);

    // Test 2: Recursive reversal of odd-length list
    int arr2[] = {1,2,3,4,5};
    SingleNode* list2 = createList(arr2, 5);
    reverseRecursive(list2);
    int expected2[] = {5,4,3,2,1};
    assert(compareLists(list2, expected2, 5));
    deleteList(list2);

    // Test 3: Empty list (sentinel only)
    SingleNode* list3 = new SingleNode{0, nullptr};
    reverseIterative(list3);
    assert(list3->next == nullptr);
    reverseRecursive(list3);
    assert(list3->next == nullptr);
    delete list3;

    // Test 4: Single-node list
    int arr4[] = {42};
    SingleNode* list4 = createList(arr4, 1);
    reverseIterative(list4);
    int expected4[] = {42};
    assert(compareLists(list4, expected4, 1));
    reverseRecursive(list4);
    assert(compareLists(list4, expected4, 1));
    deleteList(list4);

    // Test 5: Cyclic list is returned unchanged
    SingleNode* list5 = new SingleNode{0, nullptr};
    list5->next = new SingleNode{1, nullptr};
    list5->next->next = new SingleNode{2, nullptr};
    list5->next->next->next = list5->next; // cycle to second node
    assert(reverseIterative(list5) == list5);
    assert(reverseRecursive(list5) == list5);
    // Cleanup: delete first three nodes manually (avoid infinite loop)
    delete list5->next->next;
    delete list5->next;
    delete list5;

    return 0;
}
// The solution uses two strategies. For iterative reversal, we traverse the list from the first actual node (skipping the sentinel) and reverse the links using three pointers: `prev`, `curr`, and `next`. At each step, `curr->next` is set to `prev`, and we advance. After the loop, the original first node's `next` becomes `NULL`, and the original last node becomes the new first. Then we update the sentinel's `next` to point to this new first node. Edge cases: if the sentinel's `next` is `NULL` (empty) or points to a single node, we return the sentinel after doing nothing. For cycles, we must detect them to avoid infinite loops—use a fast-slow pointer (Floyd's cycle detection) to check if a cycle exists before reversal; if a cycle exists, return the head unchanged. For recursive reversal, we recursively reverse the sub-list starting from the first node, and then fix pointers: the original first node's `next` becomes `NULL`, and the recursive call returns the new head. We handle the sentinel by updating its `next` to the returned head. Time complexity is O(n) for both, and space complexity is O(1) for iterative and O(n) for recursive due to call stack.
