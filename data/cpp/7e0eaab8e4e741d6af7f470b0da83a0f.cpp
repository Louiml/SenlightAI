// Write a C++ function `bool isPalindromeLinkedList(Node* head)` that takes the head of a singly linked list (using the `Node` structure defined below) and returns `true` if the list is a palindrome (reads the same forward and backward), and `false` otherwise. The list contains integer values. You may assume the `Node` structure has `int data` and `Node* next`, and that the list is non-circular. The function must not modify the original list (it should work on a copy or use comparison without altering nodes). Handle edge cases: an empty list and a single-node list are considered palindromes.
//
// ```cpp
// struct Node {
//     int data;
//     Node* next;
//     Node(int val) : data(val), next(nullptr) {}
// };
// ```

The solution uses a two‑pointer approach to find the middle of the list without modifying it. First, we compute the length `len` by traversing the list once. Then, we create a copy of the second half of the list (starting from the node at position `len/2`) by duplicating nodes into a new list. We reverse this copied half. Then we compare the first half of the original list (from `head` up to `len/2` nodes) with the reversed copy node by node. If all values match, the list is a palindrome. Important edge cases: empty list and single‑node list return `true`. Lists with an odd number of nodes: the middle node is ignored (we copy only the nodes after the middle). Time complexity is O(n) for traversal, copying, reversing, and comparison – overall O(n). Space complexity is O(n) for the copied half, but this avoids mutating the input list. Alternative approaches (like reversing in place) would be O(1) space but would require restoring the list; we choose the copy approach for safety and clarity.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Helper to reverse a linked list (returns new head).
static Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr != nullptr) {
        Node* forward = curr->next;
        curr->next = prev;
        prev = curr;
        curr = forward;
    }
    return prev;
}

// Helper to compute length.
static int getLength(Node* head) {
    int len = 0;
    Node* temp = head;
    while (temp != nullptr) {
        ++len;
        temp = temp->next;
    }
    return len;
}

// Helper to copy a list starting from 'start'.
static Node* copyFrom(Node* start) {
    if (start == nullptr) return nullptr;
    Node* newHead = new Node(start->data);
    Node* tail = newHead;
    Node* temp = start->next;
    while (temp != nullptr) {
        tail->next = new Node(temp->data);
        tail = tail->next;
        temp = temp->next;
    }
    return newHead;
}

// Returns true if the linked list is a palindrome.
bool isPalindromeLinkedList(Node* head) {
    if (head == nullptr) return true;
    int len = getLength(head);
    if (len == 1) return true;

    int half = len / 2;  // number of nodes to compare

    // Find the start of the second half (skip half nodes).
    Node* secondHalfStart = head;
    for (int i = 0; i < half; ++i) {
        secondHalfStart = secondHalfStart->next;
    }

    // Copy the second half, then reverse the copy.
    Node* secondCopy = copyFrom(secondHalfStart);
    Node* reversedSecond = reverseList(secondCopy);

    // Compare first half with reversed second half.
    Node* p1 = head;
    Node* p2 = reversedSecond;
    for (int i = 0; i < half; ++i) {
        if (p1->data != p2->data) {
            // Clean up copied list.
            while (reversedSecond != nullptr) {
                Node* next = reversedSecond->next;
                delete reversedSecond;
                reversedSecond = next;
            }
            return false;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    // Clean up copied list.
    while (reversedSecond != nullptr) {
        Node* next = reversedSecond->next;
        delete reversedSecond;
        reversedSecond = next;
    }
    return true;
}

#include <cassert>

// Provided Node definition and function declaration here.
// (Assume the solution code above is included before this main.)

int main() {
    // Test 1: empty list
    Node* head1 = nullptr;
    assert(isPalindromeLinkedList(head1) == true);

    // Test 2: single node
    Node* head2 = new Node(5);
    assert(isPalindromeLinkedList(head2) == true);
    delete head2;

    // Test 3: two identical nodes (palindrome)
    Node* head3 = new Node(1);
    head3->next = new Node(1);
    assert(isPalindromeLinkedList(head3) == true);
    delete head3->next;
    delete head3;

    // Test 4: two different nodes (not palindrome)
    Node* head4 = new Node(1);
    head4->next = new Node(2);
    assert(isPalindromeLinkedList(head4) == false);
    delete head4->next;
    delete head4;

    // Test 5: odd-length palindrome (1 2 3 2 1)
    Node* head5 = new Node(1);
    head5->next = new Node(2);
    head5->next->next = new Node(3);
    head5->next->next->next = new Node(2);
    head5->next->next->next->next = new Node(1);
    assert(isPalindromeLinkedList(head5) == true);
    // Clean up
    Node* temp = head5;
    while (temp != nullptr) {
        Node* old = temp;
        temp = temp->next;
        delete old;
    }

    // Test 6: even-length palindrome (1 2 2 1)
    Node* head6 = new Node(1);
    head6->next = new Node(2);
    head6->next->next = new Node(2);
    head6->next->next->next = new Node(1);
    assert(isPalindromeLinkedList(head6) == true);
    temp = head6;
    while (temp != nullptr) {
        Node* old = temp;
        temp = temp->next;
        delete old;
    }

    // Test 7: non-palindrome (1 2 3 4)
    Node* head7 = new Node(1);
    head7->next = new Node(2);
    head7->next->next = new Node(3);
    head7->next->next->next = new Node(4);
    assert(isPalindromeLinkedList(head7) == false);
    temp = head7;
    while (temp != nullptr) {
        Node* old = temp;
        temp = temp->next;
        delete old;
    }

    // Test 8: original list must not be modified
    Node* head8 = new Node(1);
    head8->next = new Node(2);
    head8->next->next = new Node(1);
    Node* original = head8;
    assert(isPalindromeLinkedList(head8) == true);
    // Verify original ordering is unchanged
    Node* verify = head8;
    int values[3];
    int idx = 0;
    while (verify != nullptr) {
        values[idx++] = verify->data;
        verify = verify->next;
    }
    assert(values[0] == 1 && values[1] == 2 && values[2] == 1);
    temp = head8;
    while (temp != nullptr) {
        Node* old = temp;
        temp = temp->next;
        delete old;
    }

    return 0;
}
