// Write a C++ function that takes two linked lists representing non-negative integers in reverse order (each node holds a single digit 0-9, and the head is the least significant digit), and returns a new linked list representing their sum, also in reverse order. The input lists may be of different lengths, may contain leading zeros, and may be empty (representing 0). The solution must handle carries correctly, including a final carry that adds an extra digit. The function should not modify the input lists, and each node in the result must be dynamically allocated. You need to provide a clean, reusable function that follows the specification exactly.
#include <cassert>
#include <vector>

// Helper to convert a vector of digits (least significant first) to a linked list.
ListNode* makeList(const std::vector<int>& digits) {
    ListNode dummy(0);
    ListNode* current = &dummy;
    for (int d : digits) {
        current->next = new ListNode(d);
        current = current->next;
    }
    return dummy.next;
}

// Helper to convert a linked list to a vector of digits (least significant first).
std::vector<int> toVector(const ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete a linked list.
void deleteList(const ListNode* head) {
    while (head != nullptr) {
        const ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // 342 + 465 = 807 (reverse: 2->4->3 + 5->6->4 = 7->0->8)
    ListNode* l1 = makeList({2, 4, 3});
    ListNode* l2 = makeList({5, 6, 4});
    ListNode* result = addTwoNumbers(l1, l2);
    assert(toVector(result) == std::vector<int>({7, 0, 8}));
    deleteList(l1); deleteList(l2); deleteList(result);

    // 0 + 0 = 0 (empty lists represent 0; result should be a single 0 node)
    ListNode* l3 = makeList({0});
    ListNode* l4 = makeList({});
    result = addTwoNumbers(l3, l4);
    assert(toVector(result) == std::vector<int>({0}));
    deleteList(l3); deleteList(l4); deleteList(result);

    // 999 + 1 = 1000 (reverse: 9->9->9 + 1 = 0->0->0->1)
    ListNode* l5 = makeList({9, 9, 9});
    ListNode* l6 = makeList({1});
    result = addTwoNumbers(l5, l6);
    assert(toVector(result) == std::vector<int>({0, 0, 0, 1}));
    deleteList(l5); deleteList(l6); deleteList(result);

    // 0 + 0 = 0 when both lists are empty (represented as nullptr)
    result = addTwoNumbers(nullptr, nullptr);
    assert(result != nullptr && result->val == 0 && result->next == nullptr);
    deleteList(result);

    // 5 + 5 = 10
    ListNode* l7 = makeList({5});
    ListNode* l8 = makeList({5});
    result = addTwoNumbers(l7, l8);
    assert(toVector(result) == std::vector<int>({0, 1}));
    deleteList(l7); deleteList(l8); deleteList(result);

    return 0;
}
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Adds two numbers represented by linked lists in reverse digit order.
// Returns a new linked list representing the sum in reverse order.
// The input lists are not modified.
ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) {
    ListNode dummy(0);  // Dummy head to simplify building result.
    ListNode* current = &dummy;
    int carry = 0;

    while (l1 != nullptr || l2 != nullptr || carry != 0) {
        const int x = (l1 != nullptr) ? l1->val : 0;
        const int y = (l2 != nullptr) ? l2->val : 0;
        const int sum = carry + x + y;
        carry = sum / 10;
        current->next = new ListNode(sum % 10);
        current = current->next;

        if (l1 != nullptr) l1 = l1->next;
        if (l2 != nullptr) l2 = l2->next;
    }

    return dummy.next;
}
// The main idea is to simulate the manual addition of two numbers digit by digit from the least significant digit (head) to the most significant digit (tail). Use a dummy head node to simplify building the result list, and maintain a `carry` variable that starts at 0. In each iteration, if either list has a node or the carry is nonzero, extract the current digit from each list (0 if the list is null), compute the sum as `carry + x + y`, set the new digit as `sum % 10`, update `carry` to `sum / 10`, and advance to the next nodes in input lists and the result list. Continue until both lists are null and the carry is zero. Edge cases include: one list longer than the other, both lists empty (result should be a single node with value 0), and a final carry that creates an extra node (e.g., 9+1=10 → result list [0,1]). Time complexity is O(max(n,m)) where n and m are lengths of the input lists, and space complexity is O(max(n,m)+1) for the new list.
