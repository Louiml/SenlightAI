// Write a C++ function that implements merge sort on a singly linked list of integers, sorting the nodes in non-decreasing order by their `data` field. The function must take the head pointer of the linked list and return the head pointer of the fully sorted list. The list may be empty or contain one or more nodes. You must not use arrays, vectors, or any auxiliary container—only pointer manipulation and recursion are allowed. The solution must handle duplicate values, and the relative order of equal elements need not be preserved (since the sort is not required to be stable). The function signature should be `ListNode* mergeSortList(ListNode* head)`, where `ListNode` is a struct with `int data` and `ListNode* next`.
#include <cassert>
#include <vector>
#include <algorithm>

// Function to build a list from a vector (for testing only).
ListNode* buildList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* tail = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        tail->next = new ListNode(vals[i]);
        tail = tail->next;
    }
    return head;
}

// Function to convert list to vector (for testing only).
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Function to delete a list (for testing only).
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* tmp = head->next;
        delete head;
        head = tmp;
    }
}

int main() {
    // Test 1: empty list
    ListNode* empty = nullptr;
    assert(mergeSortList(empty) == nullptr);

    // Test 2: single node
    ListNode* single = new ListNode(5);
    assert(mergeSortList(single)->data == 5);
    deleteList(single);

    // Test 3: already sorted
    ListNode* sorted = buildList({1, 2, 3, 4});
    std::vector<int> sorted_result = listToVector(mergeSortList(sorted));
    assert(sorted_result == std::vector<int>({1, 2, 3, 4}));
    deleteList(sorted);

    // Test 4: reverse order
    ListNode* reversed = buildList({5, 4, 3, 2, 1});
    std::vector<int> reversed_result = listToVector(mergeSortList(reversed));
    assert(reversed_result == std::vector<int>({1, 2, 3, 4, 5}));
    deleteList(reversed);

    // Test 5: duplicates
    ListNode* dup = buildList({3, 1, 2, 1, 3});
    std::vector<int> dup_result = listToVector(mergeSortList(dup));
    assert(dup_result == std::vector<int>({1, 1, 2, 3, 3}));
    deleteList(dup);

    // Test 6: even length random
    ListNode* even = buildList({10, -1, 0, 7, -5, 2});
    std::vector<int> even_result = listToVector(mergeSortList(even));
    assert(even_result == std::vector<int>({-5, -1, 0, 2, 7, 10}));
    deleteList(even);

    // Test 7: odd length with negatives
    ListNode* odd = buildList({-3, -8, 4, 0, -1});
    std::vector<int> odd_result = listToVector(mergeSortList(odd));
    assert(odd_result == std::vector<int>({-8, -3, -1, 0, 4}));
    deleteList(odd);

    // Test 8: all equal
    ListNode* all_eq = buildList({7, 7, 7, 7});
    std::vector<int> all_eq_result = listToVector(mergeSortList(all_eq));
    assert(all_eq_result == std::vector<int>({7, 7, 7, 7}));
    deleteList(all_eq);

    // Test 9: large random check (optional, but here with small size)
    std::vector<int> original = {42, 17, -3, 99, 0, -7, 23, 4, 1, 55};
    ListNode* random = buildList(original);
    std::vector<int> random_result = listToVector(mergeSortList(random));
    std::sort(original.begin(), original.end());
    assert(random_result == original);
    deleteList(random);

    return 0;
}
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int data;
    ListNode* next;
    ListNode(int val) : data(val), next(nullptr) {}
};

// Helper to find the middle node (left-middle for even-length lists).
ListNode* findMiddle(ListNode* head) {
    if (head == nullptr) return nullptr;
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Merge two sorted linked lists and return the merged head.
ListNode* mergeLists(ListNode* left, ListNode* right) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (left != nullptr && right != nullptr) {
        if (left->data <= right->data) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    if (left != nullptr) tail->next = left;
    if (right != nullptr) tail->next = right;
    return dummy.next;
}

// Main function: sorts the linked list using merge sort.
ListNode* mergeSortList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) return head;

    ListNode* mid = findMiddle(head);
    ListNode* rightHead = mid->next;
    mid->next = nullptr;

    ListNode* sortedLeft = mergeSortList(head);
    ListNode* sortedRight = mergeSortList(rightHead);

    return mergeLists(sortedLeft, sortedRight);
}
// The standard approach is a recursive divide-and-conquer merge sort. First, handle the base case: if the list is empty or has only one node, return it as-is. Otherwise, find the middle node using the slow/fast pointer technique: `slow` moves one step, `fast` moves two steps, starting `fast` from `head->next` so that when `fast` reaches the end, `slow` is the left-middle (for even-length lists, the first of the two middle nodes). This ensures the split is balanced. Set `mid = slow`, then `right = mid->next`, and `mid->next = nullptr` to break the list into two halves. Recursively sort the left half (starting at `head`) and the right half (starting at `right`). Then merge the two sorted lists by creating a dummy head node (e.g., `ListNode* dummy = new ListNode(-1)`) and a tail pointer, comparing the `data` of the current nodes from each half, and appending the smaller one. After one list is exhausted, append the remainder of the other. Finally, return `dummy->next` (and optionally delete the dummy node to avoid a memory leak, but since we only need the resulting head, we can skip deletion). Edge cases include empty list, single node, two nodes, odd/even lengths, and duplicate values—all handled naturally by the comparisons. Time complexity is O(n log n) because each merge step processes all n nodes and there are log n levels. Space complexity is O(log n) due to recursion stack depth, plus O(1) auxiliary space for pointers (ignoring the dummy node). No extra container is used.
