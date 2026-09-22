// Write a C++ function named `isLinkedListPalindrome` that takes a pointer to the head of a singly linked list (where each node contains an integer `data` and a `next` pointer) and returns a `bool` indicating whether the sequence of integers stored in the list reads the same forward and backward (i.e., is a palindrome). The function must handle lists of any length, including empty lists (head is `nullptr`, which should be considered a palindrome) and lists with a single element. The input list is not modified. You may use any standard library containers or algorithms, but the function must be self-contained and not rely on any global variables or external state. The node structure is defined as `struct Node { int data; Node *next; Node(int x) : data(x), next(nullptr) {} };` and is provided in the test harness.

// The solution approach is straightforward: traverse the linked list once to copy all the integer values into a dynamic array (e.g., `std::vector<int>`). Once the vector contains all values, compare the elements from the front and back using two indices: one starting at index 0 and the other at `size - 1`. Move both indices toward each other, and if at any point the values at those indices differ, the list is not a palindrome, so return `false`. If the loop completes without finding a mismatch, return `true`. Edge cases include an empty list (`head == nullptr`), which yields an empty vector and should return `true`, and a single-element list, where the vector has one element, so the loop with `i < j` does not run and returns `true`. Duplicate values are handled naturally since we compare directly. Time complexity: O(n), where n is the number of nodes, because we traverse the list once to build the vector and then traverse the vector once (with two indices) to compare. Space complexity: O(n) because we store the values in a vector; no additional space beyond the vector is used (excluding the input list itself, which is not copied).

#include <vector>

struct Node {
    int data;
    Node *next;
    Node(int x) : data(x), next(nullptr) {}
};

// Check if the singly linked list is a palindrome.
bool isLinkedListPalindrome(const Node* head) {
    std::vector<int> values;
    const Node* temp = head;
    while (temp != nullptr) {
        values.push_back(temp->data);
        temp = temp->next;
    }

    size_t left = 0;
    size_t right = values.size();
    while (left < right) {
        if (values[left] != values[right - 1]) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}

#include <cassert>

int main() {
    // Single node
    Node* head1 = new Node(5);
    assert(isLinkedListPalindrome(head1) == true);
    delete head1;

    // Two nodes, palindrome
    Node* head2 = new Node(1);
    head2->next = new Node(1);
    assert(isLinkedListPalindrome(head2) == true);
    delete head2->next;
    delete head2;

    // Two nodes, not palindrome
    Node* head3 = new Node(1);
    head3->next = new Node(2);
    assert(isLinkedListPalindrome(head3) == false);
    delete head3->next;
    delete head3;

    // Odd-length palindrome
    Node* head4 = new Node(1);
    head4->next = new Node(2);
    head4->next->next = new Node(1);
    assert(isLinkedListPalindrome(head4) == true);
    // clean up
    Node* temp = head4;
    while (temp) { Node* n = temp->next; delete temp; temp = n; }

    // Even-length palindrome
    Node* head5 = new Node(1);
    head5->next = new Node(2);
    head5->next->next = new Node(2);
    head5->next->next->next = new Node(1);
    assert(isLinkedListPalindrome(head5) == true);
    temp = head5;
    while (temp) { Node* n = temp->next; delete temp; temp = n; }

    // Not palindrome with multiple duplicates
    Node* head6 = new Node(1);
    head6->next = new Node(3);
    head6->next->next = new Node(2);
    head6->next->next->next = new Node(1);
    assert(isLinkedListPalindrome(head6) == false);
    temp = head6;
    while (temp) { Node* n = temp->next; delete temp; temp = n; }

    // Null head (empty list) is palindrome
    assert(isLinkedListPalindrome(nullptr) == true);

    return 0;
}
