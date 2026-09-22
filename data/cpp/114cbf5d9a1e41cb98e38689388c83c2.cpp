/*
Write a C++ function `ListNode* mergeKLists(vector<ListNode*>& lists)` that takes a vector of sorted singly-linked lists (each in non-decreasing order) and merges them into one sorted singly-linked list. The lists may be empty (nullptr), and the vector itself may be empty. The function must return the head of the merged list. Use a recursive divide-and-conquer approach: recursively split the vector into halves, merge the two resulting lists with a helper function that merges two sorted linked lists in place (without allocating new nodes), and return the final merged head. The input lists should not be modified except for rearranging their `next` pointers. The function signature must match exactly as described.
*/
#include <vector>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merge two sorted linked lists in-place.
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    if (l1 == nullptr) return l2;
    if (l2 == nullptr) return l1;
    
    if (l1->val <= l2->val) {
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoLists(l1, l2->next);
        return l2;
    }
}

// Recursively merge lists from index range [start, end].
ListNode* mergeRange(int start, int end, std::vector<ListNode*>& lists) {
    if (start > end) return nullptr;
    if (start == end) return lists[start];
    
    int mid = start + (end - start) / 2;
    ListNode* left = mergeRange(start, mid, lists);
    ListNode* right = mergeRange(mid + 1, end, lists);
    return mergeTwoLists(left, right);
}

// Merge k sorted linked lists.
ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    int n = static_cast<int>(lists.size());
    return mergeRange(0, n - 1, lists);
}
#include <cassert>
#include <vector>

// (ListNode struct and functions from solution are assumed to be available above)

// Helper to create a list from vector.
ListNode* createList(const std::vector<int>& vals) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int v : vals) {
        ListNode* node = new ListNode(v);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to convert list to vector for comparison.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Empty vector
    {
        std::vector<ListNode*> lists;
        ListNode* merged = mergeKLists(lists);
        assert(merged == nullptr);
    }

    // Test 2: Single list
    {
        std::vector<int> a = {1, 3, 5};
        ListNode* la = createList(a);
        std::vector<ListNode*> lists = {la};
        ListNode* merged = mergeKLists(lists);
        assert(listToVector(merged) == a);
    }

    // Test 3: Multiple lists with duplicates and empty lists
    {
        ListNode* l1 = createList({1, 4, 5});
        ListNode* l2 = createList({1, 3, 4});
        ListNode* l3 = createList({2, 6});
        ListNode* l4 = nullptr; // empty
        std::vector<ListNode*> lists = {l1, l2, l3, l4};
        ListNode* merged = mergeKLists(lists);
        assert(listToVector(merged) == std::vector<int>({1, 1, 2, 3, 4, 4, 5, 6}));
    }

    // Test 4: All lists empty
    {
        std::vector<ListNode*> lists = {nullptr, nullptr};
        ListNode* merged = mergeKLists(lists);
        assert(merged == nullptr);
    }

    // Test 5: Large number of lists with single elements
    {
        std::vector<ListNode*> lists;
        for (int i = 10; i >= 1; --i) {
            lists.push_back(createList({i}));
        }
        ListNode* merged = mergeKLists(lists);
        assert(listToVector(merged) == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }

    // Test 6: Lists already sorted but one is much longer
    {
        ListNode* l1 = createList({-100, -50, 0, 50, 100});
        ListNode* l2 = createList({-1, 2});
        std::vector<ListNode*> lists = {l1, l2};
        ListNode* merged = mergeKLists(lists);
        assert(listToVector(merged) == std::vector<int>({-100, -50, -1, 0, 2, 50, 100}));
    }

    return 0;
}
// The solution uses a recursive divide-and-conquer strategy. The main function `mergeKLists` calls a helper `mergeRange` on the full index range `[0, n-1]`. If the range is invalid (start > end), return nullptr. If only one list exists in the range, return it directly. Otherwise, compute the midpoint `mid`, recursively merge the left half and right half, then merge the two resulting sorted lists using `mergeTwoLists`. The `mergeTwoLists` function compares the heads of both lists: if one is null, return the other; otherwise, pick the smaller head, recursively merge the rest of that list with the other list, and attach it. This is done in-place with no new nodes. Edge cases: empty vector (return nullptr), vector containing nullptrs (handled naturally), and lists already sorted. Time complexity: each merge step processes each node once per recursion level, and there are O(log k) levels, so total O(N log k) where N is total nodes and k is number of lists. Space complexity: O(log k) due to recursion stack, or O(1) excluded.
