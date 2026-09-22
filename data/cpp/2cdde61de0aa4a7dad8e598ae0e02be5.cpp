// Write a C++ function that accepts the head pointer of a singly-linked list and returns a `std::vector<int>` containing the values of the list's nodes in reverse order (i.e., from tail to head). The list may be empty, contain a single node, or have many nodes. The function should not modify the list. You must use only the `ListNode` structure with `int val` and `ListNode* next` as given, and handle cyclic references if they appear (return an empty vector if a cycle is detected). The function should be `const`-correct and named `reverseLinkedList`.

#include <cassert>
#include <vector>

// ListNode is already defined in the solution part; here we just need the test.

int main() {
    // Empty list
    assert(reverseLinkedList(nullptr) == std::vector<int>());

    // Single node
    ListNode a(5);
    assert(reverseLinkedList(&a) == std::vector<int>{5});

    // Multiple nodes: 1 -> 2 -> 3 -> 4
    ListNode n1(1), n2(2), n3(3), n4(4);
    n1.next = &n2; n2.next = &n3; n3.next = &n4;
    assert(reverseLinkedList(&n1) == std::vector<int>({4, 3, 2, 1}));

    // List with negative values: -3 -> 0 -> 7 -> -1
    ListNode m1(-3), m2(0), m3(7), m4(-1);
    m1.next = &m2; m2.next = &m3; m3.next = &m4;
    assert(reverseLinkedList(&m1) == std::vector<int>({-1, 7, 0, -3}));

    // List with duplicate values: 2 -> 2 -> 2
    ListNode d1(2), d2(2), d3(2);
    d1.next = &d2; d2.next = &d3;
    assert(reverseLinkedList(&d1) == std::vector<int>({2, 2, 2}));

    // Cycle: 1 -> 2 -> 3 -> back to 1
    ListNode c1(1), c2(2), c3(3);
    c1.next = &c2; c2.next = &c3; c3.next = &c1;
    assert(reverseLinkedList(&c1) == std::vector<int>());

    // Self-cycle: single node pointing to itself
    ListNode s(10);
    s.next = &s;
    assert(reverseLinkedList(&s) == std::vector<int>());

    return 0;
}

#include <vector>
#include <algorithm>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return the values of the linked list in reverse order.
// Returns an empty vector if the list is empty or contains a cycle.
std::vector<int> reverseLinkedList(const ListNode* head) {
    // Detect cycle using Floyd's algorithm
    const ListNode *slow = head, *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return {}; // cycle detected
        }
    }

    // Collect values in forward order
    std::vector<int> result;
    const ListNode* current = head;
    while (current != nullptr) {
        result.push_back(current->val);
        current = current->next;
    }

    // Reverse the vector to get tail-to-head order
    std::reverse(result.begin(), result.end());
    return result;
}

// The solution traverses the linked list from head to tail, collecting each node's value into a vector. After the traversal completes, we reverse the vector in-place using `std::reverse` to achieve the tail-to-head order. To handle potential cycles, we use Floyd's Tortoise and Hare algorithm to detect if a cycle exists; if one is found, we return an empty vector immediately (since a cycle means the list has no natural end). Edge cases include: an empty list (head is `nullptr`), which should return an empty vector; a single-node list, which returns a vector with that one value; and a list with a cycle, which must be detected to avoid infinite loops. Time complexity is O(n) for the traversal plus O(n) for the reversal, totaling O(n), and space complexity is O(n) for the vector that stores the values. Cycle detection adds O(n) time and O(1) space.
