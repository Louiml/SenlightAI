/*
Write a C++ function that takes a singly linked list (represented by a `Node` struct with `int data` and `Node* next`) and determines whether the sequence of values is a palindrome. The function should handle both even and odd length lists, return `true` for empty or single-node lists, and must not modify the original list (the reversal should be performed on a copy of the second half). The function signature should be `bool isPalindrome(const Node* head)`.
*/

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Reverse a linked list and return the new head (does not modify the original list nodes).
static Node* reverseCopy(const Node* head) {
    Node* prev = nullptr;
    const Node* curr = head;
    while (curr != nullptr) {
        Node* newNode = new Node(curr->data);
        newNode->next = prev;
        prev = newNode;
        curr = curr->next;
    }
    return prev;
}

// Check if the linked list is a palindrome.
bool isPalindrome(const Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return true;
    }

    // Find the middle using slow/fast pointers.
    const Node* slow = head;
    const Node* fast = head;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // For odd length, skip the middle node.
    const Node* secondHalfStart = (fast->next == nullptr) ? slow->next : slow->next;

    // Reverse a copy of the second half.
    Node* reversedSecondHalf = reverseCopy(secondHalfStart);

    // Compare first half with reversed second half.
    const Node* first = head;
    Node* second = reversedSecondHalf;
    while (second != nullptr) {
        if (first->data != second->data) {
            // Free the reversed copy to avoid memory leak.
            Node* temp = reversedSecondHalf;
            while (temp != nullptr) {
                Node* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
            return false;
        }
        first = first->next;
        second = second->next;
    }

    // Free the reversed copy.
    Node* temp = reversedSecondHalf;
    while (temp != nullptr) {
        Node* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }

    return true;
}

#include <cassert>

// Helper to create a linked list from an initializer list for testing.
Node* createList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int val : values) {
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to delete a linked list.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
    }
}

int main() {
    // Empty list
    assert(isPalindrome(nullptr) == true);

    // Single node
    Node* list1 = createList({5});
    assert(isPalindrome(list1) == true);
    deleteList(list1);

    // Two nodes same values
    Node* list2 = createList({1, 1});
    assert(isPalindrome(list2) == true);
    deleteList(list2);

    // Two nodes different values
    Node* list3 = createList({1, 2});
    assert(isPalindrome(list3) == false);
    deleteList(list3);

    // Odd-length palindrome
    Node* list4 = createList({1, 2, 3, 2, 1});
    assert(isPalindrome(list4) == true);
    deleteList(list4);

    // Even-length palindrome
    Node* list5 = createList({1, 2, 2, 1});
    assert(isPalindrome(list5) == true);
    deleteList(list5);

    // Odd-length non-palindrome
    Node* list6 = createList({1, 2, 3, 4, 5});
    assert(isPalindrome(list6) == false);
    deleteList(list6);

    // Even-length non-palindrome
    Node* list7 = createList({1, 2, 3, 4});
    assert(isPalindrome(list7) == false);
    deleteList(list7);

    // Palindrome with large numbers
    Node* list8 = createList({100, 200, 100});
    assert(isPalindrome(list8) == true);
    deleteList(list8);

    // Input list should not be modified – verify after call
    Node* list9 = createList({1, 2, 3, 2, 1});
    bool result = isPalindrome(list9);
    assert(result == true);
    // Check original still intact
    Node* curr = list9;
    int expected[] = {1, 2, 3, 2, 1};
    for (int i = 0; i < 5; ++i) {
        assert(curr->data == expected[i]);
        curr = curr->next;
    }
    deleteList(list9);

    return 0;
}

// The solution reverses the second half of the list and compares it with the first half. First, find the middle node using the slow/fast pointer technique to handle arbitrary lengths in O(n). For an odd-length list, the middle node is the center and should be skipped; for even-length lists, the middle is the node just after the first half. Then, create a deep copy of the second half (starting from the node after the middle) to avoid modifying the input, reverse that copy, and compare its data with the first half's data node by node. If any mismatch occurs, return false; otherwise, return true. Edge cases: empty list or single node returns true immediately; even and odd lengths both work; null pointers are safely handled. Time complexity is O(n) for finding the middle, copying, reversing, and comparing, and space complexity is O(n) due to the copy of the second half (which could be O(1) if we were allowed to modify the input, but the task requires no modification).
