// Write a C++ function `ListNode* buildRandomLinkedList(int n, int seed)` that creates a singly linked list containing `n` nodes (where `n >= 1`), with each node's value set to a random integer in the range `[1, 100]` (inclusive). The random number generator must be seeded with the provided `seed` parameter so that the same seed always produces the same list. The function should return the head pointer of the newly created list. Additionally, implement a helper function `void deleteLinkedList(ListNode* head)` that deallocates all nodes in the list to prevent memory leaks. Your solution must define the `ListNode` struct exactly as given in the snippet (with `val` and `next` members, and three constructors). The main challenge is correctly generating deterministic pseudo-random values and managing dynamic memory.

// The main algorithm is straightforward: generate `n` distinct node objects with random values. First, construct a new `ListNode` for the head with a random value (using `rand() % 100 + 1`). Then, for each of the remaining `n-1` nodes, create a new node and link it to the previous node's `next` pointer. To ensure determinism, call `srand(seed)` exactly once at the beginning of the function, before any `rand()` calls. Edge cases: if `n` is less than 1, the function should return `nullptr` (though the task specifies `n >= 1`, it's good practice to handle gracefully). For deletion, iterate through the list, saving the next pointer before deleting the current node. Time complexity is `O(n)` for both creation and deletion, and space complexity is `O(1)` auxiliary (excluding the list itself, which is `O(n)`). The `const` correctness: the `head` parameter in deletion is not `const` because we modify the pointer while walking, but we can mark the `next` member as `const`? No, we cannot because we need to follow it. We just need to ensure the function does not modify the nodes' values; only the pointers are walked.

#include <cstdlib>
#include <ctime>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Create a linked list of n nodes with random values in [1,100] using given seed.
ListNode* buildRandomLinkedList(int n, int seed) {
    if (n < 1) return nullptr;
    srand(seed);
    ListNode* head = new ListNode(rand() % 100 + 1);
    ListNode* current = head;
    for (int i = 1; i < n; ++i) {
        current->next = new ListNode(rand() % 100 + 1);
        current = current->next;
    }
    return head;
}

// Delete all nodes in the linked list to free memory.
void deleteLinkedList(ListNode* head) {
    while (head) {
        ListNode* temp = head->next;
        delete head;
        head = temp;
    }
}

#include <cassert>

int main() {
    // Test with n=1
    ListNode* list1 = buildRandomLinkedList(1, 42);
    assert(list1 != nullptr);
    assert(list1->val >= 1 && list1->val <= 100);
    assert(list1->next == nullptr);
    deleteLinkedList(list1);

    // Test determinism: same seed same list
    ListNode* list2a = buildRandomLinkedList(10, 7);
    ListNode* list2b = buildRandomLinkedList(10, 7);
    ListNode* p1 = list2a;
    ListNode* p2 = list2b;
    while (p1 && p2) {
        assert(p1->val == p2->val);
        p1 = p1->next;
        p2 = p2->next;
    }
    assert(p1 == nullptr && p2 == nullptr);
    deleteLinkedList(list2a);
    deleteLinkedList(list2b);

    // Test different seed produces different list (likely)
    ListNode* list3a = buildRandomLinkedList(5, 1);
    ListNode* list3b = buildRandomLinkedList(5, 2);
    bool differ = false;
    p1 = list3a;
    p2 = list3b;
    while (p1 && p2) {
        if (p1->val != p2->val) { differ = true; break; }
        p1 = p1->next;
        p2 = p2->next;
    }
    assert(differ);
    deleteLinkedList(list3a);
    deleteLinkedList(list3b);

    // Test n=0 returns nullptr
    ListNode* empty = buildRandomLinkedList(0, 99);
    assert(empty == nullptr);
    // No need to delete

    // Test all values are within range and correct count
    ListNode* list4 = buildRandomLinkedList(100, 123);
    int count = 0;
    ListNode* ptr = list4;
    while (ptr) {
        assert(ptr->val >= 1 && ptr->val <= 100);
        count++;
        ptr = ptr->next;
    }
    assert(count == 100);
    deleteLinkedList(list4);

    return 0;
}
