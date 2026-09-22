// Write a C++ function `bool isPalindromeList(ListNode* head)` that takes the head of a singly linked list and returns `true` if the sequence of integer values in the list forms a palindrome (reads the same forward and backward), and `false` otherwise. The list may be empty (head is `nullptr`), contain a single node, or contain duplicate or negative values. You are given the `ListNode` struct definition. Do not modify the list permanently; the function should restore the list's original structure before returning. The function must not use additional containers for storing all values (e.g., vector, string); instead, it should work by reversing the second half of the list in-place and comparing it with the first half, then restoring the list.

// The solution approach uses the classic “find middle, reverse second half, compare, restore” technique. First, use two pointers (`slow` and `fast`) to locate the middle node: advance `slow` one step and `fast` two steps per iteration until `fast` reaches the end. If the list has odd length, `slow` points to the middle node; we should move `slow` one step forward so that the second half starts after the middle. Then, reverse the second half of the list starting from `slow` using a standard iterative reversal, producing a new head `secondHalf`. Now compare the first half (starting from the original head) with the reversed second half node by node; since the second half's length is either equal to or one less than the first half (for odd-length lists), we only need to compare while both pointers are non-null. If all values match, it is a palindrome. Afterwards, restore the list by reversing `secondHalf` again and reconnecting it to the original tail; because reversing twice restores the original order, the list structure is preserved. Edge cases: empty list returns `true`; single-node list returns `true` without any reversal needed. Time complexity is O(n) because we traverse the list a constant number of times; space complexity is O(1) beyond the input, as we only use a few pointers.

#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Helper function to reverse a linked list and return the new head.
static ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    return prev;
}

// Returns true if the singly linked list forms a palindrome.
bool isPalindromeList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return true;
    }

    // Find the middle of the list.
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // For odd-length lists, slow is the exact middle; move one step forward to start the second half.
    if (fast != nullptr) {
        slow = slow->next;
    }

    // Reverse the second half.
    ListNode* secondHalf = reverseList(slow);

    // Compare the first half and reversed second half.
    ListNode* left = head;
    ListNode* right = secondHalf;
    bool isPalin = true;
    while (right != nullptr) {
        if (left->val != right->val) {
            isPalin = false;
            break;
        }
        left = left->next;
        right = right->next;
    }

    // Restore the original list by reversing the second half back.
    reverseList(secondHalf);

    return isPalin;
}

#include <cassert>

// Helper to create a linked list from an initializer list for testing.
ListNode* makeList(std::initializer_list<int> values) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to free the list.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Empty list
    assert(isPalindromeList(nullptr) == true);

    // Single node
    ListNode* single = new ListNode(5);
    assert(isPalindromeList(single) == true);
    deleteList(single);

    // Even-length palindrome
    ListNode* evenPal = makeList({1, 2, 2, 1});
    assert(isPalindromeList(evenPal) == true);
    deleteList(evenPal);

    // Odd-length palindrome
    ListNode* oddPal = makeList({1, 2, 3, 2, 1});
    assert(isPalindromeList(oddPal) == true);
    deleteList(oddPal);

    // Non-palindrome
    ListNode* nonPal = makeList({1, 2, 3, 4});
    assert(isPalindromeList(nonPal) == false);
    deleteList(nonPal);

    // Negative values and duplicates
    ListNode* negPal = makeList({-1, -2, -1});
    assert(isPalindromeList(negPal) == true);
    deleteList(negPal);

    // All same values
    ListNode* same = makeList({7, 7, 7, 7});
    assert(isPalindromeList(same) == true);
    deleteList(same);

    // Restore check: verify the list still has original order after isPalindromeList
    ListNode* restore = makeList({1, 2, 3, 2, 1});
    isPalindromeList(restore);
    ListNode* cur = restore;
    int expected[] = {1, 2, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        assert(cur != nullptr && cur->val == expected[i]);
        cur = cur->next;
    }
    assert(cur == nullptr);
    deleteList(restore);

    return 0;
}
