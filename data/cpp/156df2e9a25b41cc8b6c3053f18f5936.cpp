Write a C++ function that merges two sorted singly linked lists into one sorted linked list. The function must accept two pointers to the head nodes of the input lists and return a pointer to the head of the merged list. The input lists are each sorted in non-decreasing order (i.e., each node’s `val` is ≤ the next node’s `val`). If either list is empty, return the other list. The merged list must also be sorted in non-decreasing order, and the original lists should not be copied—the solution must reuse the existing nodes (no new nodes except a temporary guard). The function must be a free function named `mergeSortedLists` (not a class method), with signature `ListNode* mergeSortedLists(ListNode* list1, ListNode* list2)`, where `ListNode` is a struct with fields `int val` and `ListNode* next`. The function should handle lists of any length, including zero-length lists, and must not modify the input list structures except for rearranging their `next` pointers.

// The core algorithm is a standard merge of two sorted linked lists, using a dummy head node to simplify edge cases. We maintain a pointer `tail` that always points to the last node of the merged list built so far. At each step, we compare the values of the current nodes `l1` and `l2`; we append the smaller node to `tail->next`, advance that list’s pointer, and then move `tail` to the newly appended node. This loop continues until one of the lists becomes empty. Once one list is exhausted, we simply attach the remaining non-empty list to `tail->next` (this is correct because the remaining list is already sorted and all its elements are greater than or equal to the last appended value). Finally, we return the node after the dummy head, which is the real head of the merged list. Important edge cases: both lists empty (returns `nullptr`), one list empty (returns the other list unchanged), and lists with equal values (the algorithm uses `<` for comparison, so if equal, the node from `l2` is chosen; this is arbitrary but still produces a valid sorted merge). Time complexity is O(n + m) where n and m are lengths of the input lists, because we traverse each node exactly once. Space complexity is O(1) auxiliary, since we only use a few pointers, no recursive calls, and no dynamic allocations except the dummy node.

#include <cstddef>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Merge two sorted linked lists into one sorted list. Reuses the nodes.
// Returns the head of the merged list. If both lists are empty, returns nullptr.
ListNode* mergeSortedLists(ListNode* list1, ListNode* list2) {
    // Dummy node simplifies attaching the first real node.
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (list1 != nullptr && list2 != nullptr) {
        if (list1->val < list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    // Attach the remainder of the non-empty list (or both if empty).
    tail->next = (list1 != nullptr) ? list1 : list2;

    return dummy.next;
}

#include <cassert>
#include <vector>

int main() {
    // Helper to build a list from a vector.
    auto buildList = [](const std::vector<int>& vals) -> ListNode* {
        if (vals.empty()) return nullptr;
        ListNode* head = new ListNode(vals[0]);
        ListNode* cur = head;
        for (size_t i = 1; i < vals.size(); ++i) {
            cur->next = new ListNode(vals[i]);
            cur = cur->next;
        }
        return head;
    };

    // Helper to convert list to vector for comparison.
    auto toVector = [](ListNode* head) {
        std::vector<int> result;
        while (head) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    };

    // Test 1: Both lists non-empty.
    ListNode* l1 = buildList({1, 2, 4});
    ListNode* l2 = buildList({1, 3, 4});
    ListNode* merged = mergeSortedLists(l1, l2);
    assert(toVector(merged) == (std::vector<int>{1, 1, 2, 3, 4, 4}));

    // Test 2: One list empty.
    ListNode* l3 = buildList({});  // nullptr
    ListNode* l4 = buildList({5, 6});
    merged = mergeSortedLists(l3, l4);
    assert(toVector(merged) == (std::vector<int>{5, 6}));

    // Test 3: Both lists empty.
    merged = mergeSortedLists(nullptr, nullptr);
    assert(merged == nullptr);

    // Test 4: One element each.
    ListNode* l5 = buildList({7});
    ListNode* l6 = buildList({2});
    merged = mergeSortedLists(l5, l6);
    assert(toVector(merged) == (std::vector<int>{2, 7}));

    // Test 5: Lists with duplicate values and longer lengths.
    ListNode* l7 = buildList({1, 1, 1});
    ListNode* l8 = buildList({0, 2, 2});
    merged = mergeSortedLists(l7, l8);
    assert(toVector(merged) == (std::vector<int>{0, 1, 1, 1, 2, 2}));

    // Test 6: One list is completely smaller than the other.
    ListNode* l9 = buildList({-5, -3, -1});
    ListNode* l10 = buildList({0, 4});
    merged = mergeSortedLists(l9, l10);
    assert(toVector(merged) == (std::vector<int>{-5, -3, -1, 0, 4}));

    // Test 7: Negative and zero values.
    ListNode* l11 = buildList({-10, -2});
    ListNode* l12 = buildList({-7, 0, 3});
    merged = mergeSortedLists(l11, l12);
    assert(toVector(merged) == (std::vector<int>{-10, -7, -2, 0, 3}));

    return 0;
}
