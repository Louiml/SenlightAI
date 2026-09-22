// Write a C++ function named `removeDuplicatesFromSortedList` that takes a singly linked list represented by the following `ListNode` struct and returns the head of the same list after removing all duplicate values, keeping only the first occurrence of each value. The input list is guaranteed to be sorted in non-decreasing order. The function must modify the list in place (no new nodes may be allocated) and must work correctly for empty lists, single-node lists, lists with all identical values, and lists with no duplicates. You may not use any standard library containers or helper functions—only pointer manipulation. After the function returns, the resulting list must remain sorted and contain no consecutive equal values.
#include <cassert>

// Helper to create a list from an initializer list (not required by task, but used for testing).
ListNode* createList(std::initializer_list<int> values) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to check a list against an expected vector of values and free memory.
void assertListEquals(ListNode* head, std::initializer_list<int> expected) {
    ListNode* curr = head;
    for (int e : expected) {
        assert(curr != nullptr && curr->val == e);
        curr = curr->next;
    }
    assert(curr == nullptr);
    // Free memory: delete all nodes in the list
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Empty list
    ListNode* empty = nullptr;
    empty = removeDuplicatesFromSortedList(empty);
    assert(empty == nullptr);

    // Single node
    ListNode* single = new ListNode(5);
    single = removeDuplicatesFromSortedList(single);
    assertListEquals(single, {5});

    // No duplicates
    ListNode* noDup = createList({1, 2, 3});
    noDup = removeDuplicatesFromSortedList(noDup);
    assertListEquals(noDup, {1, 2, 3});

    // All duplicates
    ListNode* allDup = createList({7, 7, 7, 7});
    allDup = removeDuplicatesFromSortedList(allDup);
    assertListEquals(allDup, {7});

    // Mixed duplicates, some consecutive
    ListNode* mixed = createList({1, 1, 2, 3, 3, 4, 5, 5});
    mixed = removeDuplicatesFromSortedList(mixed);
    assertListEquals(mixed, {1, 2, 3, 4, 5});

    // Duplicates at beginning and end
    ListNode* edges = createList({2, 2, 2, 3, 4, 4});
    edges = removeDuplicatesFromSortedList(edges);
    assertListEquals(edges, {2, 3, 4});

    return 0;
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

// Removes all duplicate values from a sorted linked list, keeping the first occurrence.
// Returns the head of the modified list. Removes duplicate nodes to free memory.
ListNode* removeDuplicatesFromSortedList(ListNode* head) {
    ListNode* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->val == current->next->val) {
            ListNode* duplicate = current->next;
            current->next = duplicate->next;
            delete duplicate;  // free the removed node
        } else {
            current = current->next;
        }
    }
    return head;
}
// The core idea is to traverse the linked list with a single pointer `current` starting at the head. At each step, we check if the next node exists and has the same value as the current node. If yes, we skip the duplicate by setting `current->next` to `current->next->next` and then delete the skipped node to avoid memory leaks (though in a typical LeetCode-style problem deletion might be omitted, it's good practice here). If the next value differs, we advance `current` to that next node. This continues until `current` becomes null or `current->next` becomes null. Edge cases: an empty list or a list with one node immediately returns head unchanged. A list where all values are equal will compress to a single node. The algorithm is linear in the number of nodes: each node is visited at most once as `current`, and each duplicate node is examined once and removed. Time complexity is O(n) and auxiliary space is O(1) if we delete removed nodes (or O(n) if not, but we assume proper deletion). The solution requires careful pointer handling to avoid null dereferences.
