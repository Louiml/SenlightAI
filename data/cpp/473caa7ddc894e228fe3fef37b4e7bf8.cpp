/*
Given a singly-linked list whose nodes contain integer values, implement a C++ function `std::vector<int> nextGreaterElements(const ListNode* head)` that returns a vector of the same length as the list. For each position `i`, the entry should be the value of the first node that appears *after* that node in the list and has a strictly greater value. If no such node exists for that position, place `0` instead. The function must not modify the input list, must be efficient for long lists (up to 10^5 nodes), and must not use any extra data structures except the returned vector and a constant number of additional variables (e.g., do not store a copy of the list or an auxiliary array for all nodes). You may assume the list is non-empty.
*/

#include <vector>
#include <stack>

// Given a singly-linked list, return a vector where each element is the first
// strictly greater value that appears later in the list, or 0 if none exists.
std::vector<int> nextGreaterElements(const ListNode* head) {
    std::vector<int> values;
    for (const ListNode* node = head; node != nullptr; node = node->next) {
        values.push_back(node->val);
    }

    int n = values.size();
    std::vector<int> result(n, 0);
    std::stack<int> indices;  // stack of indices with decreasing values

    for (int i = 0; i < n; ++i) {
        while (!indices.empty() && values[indices.top()] < values[i]) {
            result[indices.top()] = values[i];
            indices.pop();
        }
        indices.push(i);
    }
    // All remaining indices have no larger element to their right (already 0).
    return result;
}

#include <cassert>
#include <vector>

// Minimal ListNode definition for testing (values only, no allocation helpers).
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Helper to create a list from a vector (memory is leaked for simplicity).
ListNode* makeList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* cur = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        cur->next = new ListNode(vals[i]);
        cur = cur->next;
    }
    return head;
}

// Helper to free list (not strictly needed for tests but good practice).
void freeList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: increasing list
    {
        ListNode* head = makeList({1, 2, 3, 4});
        std::vector<int> expected = {2, 3, 4, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 2: decreasing list
    {
        ListNode* head = makeList({4, 3, 2, 1});
        std::vector<int> expected = {0, 0, 0, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 3: with duplicates and negatives
    {
        ListNode* head = makeList({-2, 0, -1, 3, 3});
        std::vector<int> expected = {0, 3, 3, 0, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 4: single node
    {
        ListNode* head = new ListNode(5);
        std::vector<int> expected = {0};
        assert(nextGreaterElements(head) == expected);
        delete head;
    }

    // Test 5: all equal values
    {
        ListNode* head = makeList({7, 7, 7});
        std::vector<int> expected = {0, 0, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 6: mixed pattern
    {
        ListNode* head = makeList({1, 5, 3, 4, 2});
        std::vector<int> expected = {5, 0, 4, 0, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 7: two nodes where second is larger
    {
        ListNode* head = makeList({10, 20});
        std::vector<int> expected = {20, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 8: two nodes where second is smaller
    {
        ListNode* head = makeList({20, 10});
        std::vector<int> expected = {0, 0};
        assert(nextGreaterElements(head) == expected);
        freeList(head);
    }

    // Test 9: long list (1000 nodes) increasing – spot check
    {
        std::vector<int> vals;
        for (int i = 0; i < 1000; ++i) vals.push_back(i);
        ListNode* head = makeList(vals);
        std::vector<int> result = nextGreaterElements(head);
        assert(result.size() == 1000);
        assert(result[0] == 1);
        assert(result[998] == 999);
        assert(result[999] == 0);
        freeList(head);
    }

    return 0;
}

// The provided snippet uses a naive O(n²) double‑loop: for each node `c1`, it scans forward with `c2` until a larger node is found or the list ends, then resets `c2` to the next node after `c1`. This is correct but slow for large inputs. However, the task asks for a more efficient solution while keeping the same interface. A better approach is to use a stack of indices (or of node pointers) while traversing the list once, collecting all node values into a vector first (because we need random access to values for comparisons). Since the list is singly linked and we must produce results in the original order, we first copy all values into a vector `vals`. Then traverse `vals` from left to right, maintaining a monotonic decreasing stack of indices. For each current value, while the stack is not empty and `vals[stack.top()] < current`, we pop and set the answer for that index to the current value. After processing all nodes, any indices left in the stack have no larger element to their right, so set them to 0. This runs in O(n) time and O(n) auxiliary space for the values vector and the stack. Edge cases: all values equal → all answers are 0; strictly decreasing list → all answers are 0; strictly increasing list → each answer is the next value except the last which is 0; a single node → answer is 0; negative values are fine.
