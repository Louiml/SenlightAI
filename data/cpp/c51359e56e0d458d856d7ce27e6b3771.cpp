Write a C++ function `addTwoNumbers(ListNode* l1, ListNode* l2)` that takes two non-empty singly-linked lists representing two non-negative integers in **reverse order** (each node contains a single digit, and digits are stored such that the most significant digit is at the tail). The function must return a new linked list (also in reverse order) representing the sum of the two numbers. The lists may have different lengths, and a final carry must be handled by appending an extra node with value 1. The function should not modify the input lists, should create new nodes for the result, and should be `const`-correct where applicable. The signature must be `ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2);` — the function does not own the input lists and must not delete them. You may use the provided `ListNode` structure and a helper `addToTail` if needed, but the solution should be self-contained with proper memory management for the new result nodes.
#include <cassert>
#include <vector>

// Helper to build a list from a vector of ints.
ListNode* buildList(const std::vector<int>& vals) {
    ListNode* head = nullptr;
    for (int v : vals) {
        addToTail(head, v);
    }
    return head;
}

// Helper to convert a list to a vector for comparison.
std::vector<int> listToVector(const ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to free all nodes in a list.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: 342 + 465 = 807 (represented as 2->4->3 + 5->6->4 = 7->0->8)
    ListNode* l1 = buildList({2, 4, 3});
    ListNode* l2 = buildList({5, 6, 4});
    ListNode* sum = addTwoNumbers(l1, l2);
    std::vector<int> expected = {7, 0, 8};
    assert(listToVector(sum) == expected);
    deleteList(l1); deleteList(l2); deleteList(sum);

    // Test 2: 0 + 0 = 0
    l1 = buildList({0});
    l2 = buildList({0});
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>{0});
    deleteList(l1); deleteList(l2); deleteList(sum);

    // Test 3: Different lengths: 9999999 + 9999 = 10009998 (represented reversed)
    l1 = buildList({9,9,9,9,9,9,9}); // 9,999,999
    l2 = buildList({9,9,9,9});       // 9,999
    sum = addTwoNumbers(l1, l2);
    expected = {8,9,9,9,0,0,0,1}; // 10009998 reversed
    assert(listToVector(sum) == expected);
    deleteList(l1); deleteList(l2); deleteList(sum);

    // Test 4: Final carry: 5 + 5 = 10 (5->null + 5->null = 0->1)
    l1 = buildList({5});
    l2 = buildList({5});
    sum = addTwoNumbers(l1, l2);
    expected = {0, 1};
    assert(listToVector(sum) == expected);
    deleteList(l1); deleteList(l2); deleteList(sum);

    // Test 5: One list is null? Not allowed per task, but test longer list with null-like? 
    // We can simulate by empty vector? But task says non-empty lists, so skip.

    // Test 6: Large carry chain: 999 + 1 = 1000 (reversed: 9->9->9 + 1->null = 0->0->0->1)
    l1 = buildList({9,9,9});
    l2 = buildList({1});
    sum = addTwoNumbers(l1, l2);
    expected = {0,0,0,1};
    assert(listToVector(sum) == expected);
    deleteList(l1); deleteList(l2); deleteList(sum);

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

// Append a new node with value `val` to the tail of the list pointed to by `head`.
void addToTail(ListNode*& head, int val) {
    ListNode* newNode = new ListNode(val);
    if (head == nullptr) {
        head = newNode;
        return;
    }
    ListNode* tmp = head;
    while (tmp->next != nullptr) {
        tmp = tmp->next;
    }
    tmp->next = newNode;
}

// Add two numbers represented as reverse-order linked lists.
// Returns a new linked list representing the sum (also in reverse order).
// Does not modify the input lists.
ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) {
    int sum = 0;
    int carry = 0;
    ListNode* result = nullptr;

    while (l1 != nullptr || l2 != nullptr) {
        int val1 = (l1 != nullptr) ? l1->val : 0;
        int val2 = (l2 != nullptr) ? l2->val : 0;
        sum = val1 + val2 + carry;
        carry = sum / 10;
        sum = sum % 10;
        addToTail(result, sum);

        if (l1 != nullptr) l1 = l1->next;
        if (l2 != nullptr) l2 = l2->next;
    }

    if (carry != 0) {
        addToTail(result, carry);
    }

    return result;
}
// The core algorithm is a digit-wise addition similar to elementary school addition. We iterate through both lists simultaneously, starting from the heads (which represent the least significant digits). At each step, we sum the current digits from `l1` and `l2` (if present) plus a `carry` (initialized to 0). The new digit is `sum % 10`, and the updated carry is `sum / 10` (since digits are 0–9, `carry` is either 0 or 1). We append this new digit to the result list. If one list is exhausted, we treat its digits as 0. After the loop, if there is a remaining carry, we append a final digit of 1. Edge cases include: one list being longer than the other (handle by checking `l1` and `l2` independently), both lists having same length with a carry at the end (e.g., 999 + 1 = 1000), and the result being exactly one digit longer than the longest input. Time complexity is `O(max(m, n))` where `m` and `n` are lengths of the two lists, and auxiliary space is `O(max(m, n))` for the new nodes (not counting the input lists). The input lists are not modified, and the function uses only constant extra space besides the output.
