Write a C++ function `bool isPalindromeList(ListNode* head)` that takes the head of a singly-linked list and returns `true` if the sequence of node values reads the same forward and backward (i.e., forms a palindrome), and `false` otherwise. The list may contain zero or more nodes, and values can be any 32-bit signed integers. You must not modify the original list in any way (the function should be `const`-correct, taking the head by pointer and promising not to mutate the nodes). The function must be standalone and include the definition of the `ListNode` struct as provided (you may copy it). Handle edge cases such as an empty list or a single-node list correctly (both are palindromes). Do not use any auxiliary container like `std::vector` or `std::string`; instead, solve it by creating a reversed copy of the list, or by using the two-pointer approach with a reversed second half (choose the reversed-copy method for simplicity), and ensure no memory leaks.
// The approach is to create a full copy of the original list (preserving the original), then reverse the original list in-place (since we own a copy, we can safely mutate the original nodes—but to be extra safe and not modify the caller's list, we instead reverse the copy). Better: we reverse the copy to compare against the original. Steps: (1) If `head` is `nullptr`, return `true` (empty list is a palindrome). (2) Create a deep copy of the list, node by node, storing the copied head. (3) Reverse the copied list in place (using a standard iterative reversal). (4) Traverse both the original and the reversed copy simultaneously, comparing node values. If any mismatch, return `false`. (5) After traversal, delete the entire reversed copy (which is the same as the copied list) to free memory. If all values matched, return `true`. Edge cases: single-node list: copy has one node, reverse is same, comparison succeeds. List with all same values: works. Negative values: works. Time complexity: O(n) for copying, O(n) for reversing, O(n) for comparison, total O(n). Space complexity: O(n) for the copy. Note: We must be careful with deletion to avoid double-free; we delete the whole copied list by traversing and deleting nodes after the comparison.
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Returns true if the linked list is a palindrome, false otherwise.
// Does not modify the original list. Handles empty and single-node lists.
bool isPalindromeList(const ListNode* head) {
    if (head == nullptr) {
        return true;
    }

    // Create a deep copy of the list
    ListNode* copyHead = new ListNode(head->val);
    ListNode* copyTail = copyHead;
    const ListNode* originalCur = head->next;
    while (originalCur) {
        copyTail->next = new ListNode(originalCur->val);
        copyTail = copyTail->next;
        originalCur = originalCur->next;
    }

    // Reverse the copy in place
    ListNode* prev = nullptr;
    ListNode* cur = copyHead;
    while (cur) {
        ListNode* next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    ListNode* reversedCopyHead = prev;

    // Compare original list with reversed copy
    const ListNode* origIter = head;
    ListNode* copyIter = reversedCopyHead;
    bool isPalin = true;
    while (origIter && copyIter) {
        if (origIter->val != copyIter->val) {
            isPalin = false;
            break;
        }
        origIter = origIter->next;
        copyIter = copyIter->next;
    }

    // Free the copied list (which is now the reversed copy)
    ListNode* toDelete = reversedCopyHead;
    while (toDelete) {
        ListNode* next = toDelete->next;
        delete toDelete;
        toDelete = next;
    }

    return isPalin;
}
#include <cassert>

int main() {
    // Test 1: Empty list is palindrome
    ListNode* empty = nullptr;
    assert(isPalindromeList(empty) == true);

    // Test 2: Single node list
    ListNode* single = new ListNode(5);
    assert(isPalindromeList(single) == true);
    delete single;

    // Test 3: Two different values
    ListNode* twoDiff = new ListNode(1, new ListNode(2));
    assert(isPalindromeList(twoDiff) == false);
    delete twoDiff->next;
    delete twoDiff;

    // Test 4: Two same values
    ListNode* twoSame = new ListNode(3, new ListNode(3));
    assert(isPalindromeList(twoSame) == true);
    delete twoSame->next;
    delete twoSame;

    // Test 5: Even-length palindrome
    ListNode* evenPalin = new ListNode(1, new ListNode(2, new ListNode(2, new ListNode(1))));
    assert(isPalindromeList(evenPalin) == true);
    ListNode* cur = evenPalin;
    while (cur) { ListNode* next = cur->next; delete cur; cur = next; }

    // Test 6: Even-length non-palindrome
    ListNode* evenNon = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(4))));
    assert(isPalindromeList(evenNon) == false);
    cur = evenNon;
    while (cur) { ListNode* next = cur->next; delete cur; cur = next; }

    // Test 7: Odd-length palindrome
    ListNode* oddPalin = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(2, new ListNode(1)))));
    assert(isPalindromeList(oddPalin) == true);
    cur = oddPalin;
    while (cur) { ListNode* next = cur->next; delete cur; cur = next; }

    // Test 8: All same values
    ListNode* allSame = new ListNode(7, new ListNode(7, new ListNode(7)));
    assert(isPalindromeList(allSame) == true);
    cur = allSame;
    while (cur) { ListNode* next = cur->next; delete cur; cur = next; }

    // Test 9: Negative values palindrome
    ListNode* neg = new ListNode(-1, new ListNode(-2, new ListNode(-1)));
    assert(isPalindromeList(neg) == true);
    cur = neg;
    while (cur) { ListNode* next = cur->next; delete cur; cur = next; }

    // Test 10: Negative values non-palindrome
    ListNode* negNon = new ListNode(-1, new ListNode(2, new ListNode(-1, new ListNode(3))));
    assert(isPalindromeList(negNon) == false);
    cur = negNon;
    while (cur) { ListNode* next = cur->next; delete cur; cur = next; }

    return 0;
}
