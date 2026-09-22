/*
Given a singly linked list where each node contains an integer value, write a C++ function `bool isPalindrome(const ListNode* head)` that returns `true` if the sequence of values in the list reads the same forward and backward, and `false` otherwise. The list may be empty (representing a palindrome) or contain any number of nodes. You may assume the list is not cyclic. The function should not modify the list, and you should provide a solution using recursion with an external pointer (e.g., a member variable or a reference parameter) to compare nodes from the front and back. Handle cases such as a single node, an odd-length palindrome (middle node can be ignored), and an even-length palindrome.
*/
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/**
 * Recursive helper that compares nodes from the front (via forward pointer)
 * and back (via recursion) to check palindrome status.
 * @param head current node in recursion (moving backward)
 * @param forward pointer that moves forward, passed by reference
 * @return true if the remaining portion is a palindrome
 */
bool checkPalindrome(const ListNode* head, const ListNode*& forward) {
    if (head == nullptr) return true;
    // Recurse to the end first
    if (!checkPalindrome(head->next, forward)) return false;
    // Compare current node with the forward pointer
    if (head->val != forward->val) return false;
    // Advance forward pointer
    forward = forward->next;
    return true;
}

/**
 * Determines whether a singly linked list is a palindrome.
 * @param head pointer to the first node (may be null)
 * @return true if the list is a palindrome, false otherwise
 */
bool isPalindrome(const ListNode* head) {
    const ListNode* forward = head;
    return checkPalindrome(head, forward);
}
#include <cassert>

int main() {
    // Test empty list
    ListNode* empty = nullptr;
    assert(isPalindrome(empty) == true);

    // Test single node
    ListNode node1(1);
    assert(isPalindrome(&node1) == true);

    // Test even-length palindrome: 1->2->2->1
    ListNode n1(1), n2(2), n3(2), n4(1);
    n1.next = &n2; n2.next = &n3; n3.next = &n4;
    assert(isPalindrome(&n1) == true);

    // Test even-length non-palindrome: 1->2->3->4
    ListNode m1(1), m2(2), m3(3), m4(4);
    m1.next = &m2; m2.next = &m3; m3.next = &m4;
    assert(isPalindrome(&m1) == false);

    // Test odd-length palindrome: 1->2->3->2->1
    ListNode p1(1), p2(2), p3(3), p4(2), p5(1);
    p1.next = &p2; p2.next = &p3; p3.next = &p4; p4.next = &p5;
    assert(isPalindrome(&p1) == true);

    // Test odd-length non-palindrome: 1->2->3->4->5
    ListNode q1(1), q2(2), q3(3), q4(4), q5(5);
    q1.next = &q2; q2.next = &q3; q3.next = &q4; q4.next = &q5;
    assert(isPalindrome(&q1) == false);

    // Test negative values palindrome: -1->2->-1
    ListNode r1(-1), r2(2), r3(-1);
    r1.next = &r2; r2.next = &r3;
    assert(isPalindrome(&r1) == true);

    // Test all same values: 5->5->5->5
    ListNode s1(5), s2(5), s3(5), s4(5);
    s1.next = &s2; s2.next = &s3; s3.next = &s4;
    assert(isPalindrome(&s1) == true);

    // Test list that is not palindrome but has matching ends: 7->8->9->7
    ListNode t1(7), t2(8), t3(9), t4(7);
    t1.next = &t2; t2.next = &t3; t3.next = &t4;
    assert(isPalindrome(&t1) == false);

    return 0;
}
// The solution uses a recursive helper that first traverses to the end of the list, then compares values as the recursion unwinds. A global or reference pointer (initially set to the head) moves forward one node each time the recursion returns. The recursion proceeds as follows: if the current node is null, return true (base case). Otherwise, recursively process the next node; if that result is false, propagate false immediately. Then compare the current node’s value with the value at the forward pointer; if they differ, return false. Otherwise, advance the forward pointer to its next node and return true. This effectively compares the first and last nodes, then the second and second-to-last, etc. Edge cases: empty list returns true; a single-node list returns true; for an odd-length list, the middle node is compared with itself and passes. Time complexity is O(n) because each node is visited once in the recursion and once by the forward pointer. Space complexity is O(n) due to the recursion stack depth, which is proportional to the list length.
