/*
Write a C++ function `isLinkedListPalindrome(Node* head)` that takes the head of a singly linked list and returns `true` if the sequence of node data values forms a palindrome (reads the same forward and backward), and `false` otherwise. The linked list is defined by the `Node` struct with an `int data` member and a `Node* next` pointer (already provided). Handle empty lists and single-node lists gracefully (both are considered palindromes). You must not modify the original list structure permanently—after your function returns, the original list should remain in its original order (i.e., you may temporarily reverse a portion, but restore it before returning, or avoid modification entirely). The function should work for lists with both even and odd numbers of nodes.
*/

#include <iostream>

struct Node {
    int data;
    Node* next;
    Node(int val = 0) : data(val), next(nullptr) {}
};

// Helper to reverse a linked list and return new head
Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// Returns true if the linked list is a palindrome, false otherwise.
bool isLinkedListPalindrome(Node* head) {
    if (!head || !head->next) {
        // Empty or single node: palindrome
        return true;
    }

    // Step 1: Find middle using slow/fast pointers
    Node* slow = head;
    Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // Now slow points to the middle (for odd length) or first node of second half? 
    // For odd length, slow is the exact middle. For even, slow is the last node of first half.
    // We'll reverse from slow->next.

    // Step 2: Reverse the second half
    Node* secondHalf = reverseList(slow->next);
    Node* firstHalf = head;
    Node* secondHalfCopy = secondHalf; // keep for restoration

    // Step 3: Compare
    bool isPalin = true;
    while (secondHalf) {
        if (firstHalf->data != secondHalf->data) {
            isPalin = false;
            break;
        }
        firstHalf = firstHalf->next;
        secondHalf = secondHalf->next;
    }

    // Step 4: Restore original list (reverse the second half back)
    slow->next = reverseList(secondHalfCopy);

    return isPalin;
}

#include <cassert>

int main() {
    // Helper to build list from initializer list
    auto buildList = [](std::initializer_list<int> vals) {
        Node* head = nullptr;
        Node** ptr = &head;
        for (int v : vals) {
            *ptr = new Node(v);
            ptr = &((*ptr)->next);
        }
        return head;
    };

    // Test 1: Empty list
    assert(isLinkedListPalindrome(nullptr) == true);

    // Test 2: Single node
    Node* single = new Node(5);
    assert(isLinkedListPalindrome(single) == true);
    delete single;

    // Test 3: Two nodes same
    Node* twoSame = buildList({7, 7});
    assert(isLinkedListPalindrome(twoSame) == true);
    // clean up
    while (twoSame) { Node* temp = twoSame; twoSame = twoSame->next; delete temp; }

    // Test 4: Two nodes different
    Node* twoDiff = buildList({1, 2});
    assert(isLinkedListPalindrome(twoDiff) == false);
    while (twoDiff) { Node* temp = twoDiff; twoDiff = twoDiff->next; delete temp; }

    // Test 5: Odd length palindrome
    Node* oddPal = buildList({1, 2, 3, 2, 1});
    assert(isLinkedListPalindrome(oddPal) == true);
    while (oddPal) { Node* temp = oddPal; oddPal = oddPal->next; delete temp; }

    // Test 6: Odd length non-palindrome
    Node* oddNon = buildList({1, 2, 3, 4, 5});
    assert(isLinkedListPalindrome(oddNon) == false);
    while (oddNon) { Node* temp = oddNon; oddNon = oddNon->next; delete temp; }

    // Test 7: Even length palindrome
    Node* evenPal = buildList({1, 2, 2, 1});
    assert(isLinkedListPalindrome(evenPal) == true);
    while (evenPal) { Node* temp = evenPal; evenPal = evenPal->next; delete temp; }

    // Test 8: Even length non-palindrome
    Node* evenNon = buildList({1, 2, 3, 4});
    assert(isLinkedListPalindrome(evenNon) == false);
    while (evenNon) { Node* temp = evenNon; evenNon = evenNon->next; delete temp; }

    // Test 9: All same values
    Node* allSame = buildList({9, 9, 9, 9});
    assert(isLinkedListPalindrome(allSame) == true);
    while (allSame) { Node* temp = allSame; allSame = allSame->next; delete temp; }

    // Test 10: Long palindrome with even length
    Node* longPal = buildList({1, 2, 3, 4, 4, 3, 2, 1});
    assert(isLinkedListPalindrome(longPal) == true);
    while (longPal) { Node* temp = longPal; longPal = longPal->next; delete temp; }

    // No assertion failures means all passed
    return 0;
}

// The solution uses the classic "find middle then reverse second half" approach, but with a critical fix: the original code snippet has bugs (it returns `slow` prematurely and doesn't handle null pointers correctly), so we will implement a correct version. The algorithm:
// 1. If the list is empty or has one node, return `true`.
// 2. Use the slow/fast pointer technique to find the middle node. For even-length lists, the slow pointer should end at the first node of the second half (or the last node of the first half—we'll choose the former for simplicity). A common method: initialize `slow = head`, `fast = head`. Move `fast` two steps and `slow` one step until `fast` reaches the end. At the end, `slow` is at the middle. For odd length, `slow` is the exact middle; for even, `slow` is the end of the first half.
// 3. Reverse the second half of the list starting from `slow->next`.
// 4. Compare the first half (starting from `head`) with the reversed second half node by node. If all match, it's a palindrome.
// 5. To keep the original list intact, we should restore the reversed second half back to its original order before returning. This is good practice. Alternatively, we can reverse the second half, compare, and then reverse again to restore. Since the problem statement says "must not modify permanently", we restore.
// 6. Edge cases: empty list → true; single node → true; all same values → true; two-node lists (e.g., [1,2] false, [1,1] true).
//
// Time complexity: O(n) for finding middle, O(n) for reversing half, O(n) for comparison, O(n) for restoration — total O(n). Space complexity: O(1) auxiliary (only pointers).
