/*
Write a C++ function that takes a pointer to the head of a doubly linked list and returns a boolean indicating whether the sequence of integer values stored in the list forms a palindrome. The function should handle both even and odd-length lists, including a single-node list and an empty list (null pointer). You may assume the list is perfectly linked in both directions (no cycles, valid prev pointers) and contains no sentinel nodes. The function must not modify the list; it should only traverse and compare values. To test, you will construct doubly linked lists manually (using a simple Node structure) and compare the function’s result against expected true/false outcomes.
*/

#include <cstddef> // for nullptr

struct Node {
    int data;
    Node* next;
    Node* prev;
    explicit Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

// Checks whether the doubly linked list starting at 'head' forms a palindrome.
// Returns true for an empty list or a single-node list.
bool isPalindromeDoublyLinkedList(const Node* head) {
    if (head == nullptr) {
        return true;
    }

    // Locate the tail (last node).
    const Node* tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
    }

    // Compare from both ends moving inward.
    const Node* left = head;
    const Node* right = tail;
    while (left != right && left->prev != right) {
        if (left->data != right->data) {
            return false;
        }
        left = left->next;
        right = right->prev;
    }

    // If we exit the loop without mismatch, it's a palindrome.
    return true;
}

#include <cassert>

int main() {
    // Helper to build a list from initializer list (for testing only).
    auto buildList = [](std::initializer_list<int> vals) {
        Node* head = nullptr;
        Node* tail = nullptr;
        for (int v : vals) {
            Node* n = new Node(v);
            if (!head) {
                head = tail = n;
            } else {
                tail->next = n;
                n->prev = tail;
                tail = n;
            }
        }
        return std::pair<Node*, Node*>(head, tail);
    };

    // Test 1: Empty list
    assert(isPalindromeDoublyLinkedList(nullptr) == true);

    // Test 2: Single node
    {
        auto [h, t] = buildList({5});
        assert(isPalindromeDoublyLinkedList(h) == true);
        delete h;
    }

    // Test 3: Two identical nodes (even palindrome)
    {
        auto [h, t] = buildList({2, 2});
        assert(isPalindromeDoublyLinkedList(h) == true);
        delete h;
        delete t;
    }

    // Test 4: Two different nodes (not palindrome)
    {
        auto [h, t] = buildList({1, 2});
        assert(isPalindromeDoublyLinkedList(h) == false);
        delete h;
        delete t;
    }

    // Test 5: Three nodes palindrome (odd)
    {
        auto [h, t] = buildList({1, 2, 1});
        assert(isPalindromeDoublyLinkedList(h) == true);
        delete h->next;
        delete h;
    }

    // Test 6: Three nodes not palindrome
    {
        auto [h, t] = buildList({1, 2, 3});
        assert(isPalindromeDoublyLinkedList(h) == false);
        delete h->next;
        delete h;
    }

    // Test 7: Longer even palindrome
    {
        auto [h, t] = buildList({1, 2, 3, 3, 2, 1});
        assert(isPalindromeDoublyLinkedList(h) == true);
        // cleanup (iterate and delete)
        Node* cur = h;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    // Test 8: Longer even not palindrome
    {
        auto [h, t] = buildList({1, 2, 3, 4, 5, 6});
        assert(isPalindromeDoublyLinkedList(h) == false);
        Node* cur = h;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    // Test 9: Longer odd palindrome with large numbers
    {
        auto [h, t] = buildList({100, -5, 42, -5, 100});
        assert(isPalindromeDoublyLinkedList(h) == true);
        Node* cur = h;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    // Test 10: Odd length where middle differs? Actually must be palindrome if ends match.
    {
        auto [h, t] = buildList({9, 8, 7, 8, 9});
        assert(isPalindromeDoublyLinkedList(h) == true);
        Node* cur = h;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    return 0;
}

// The algorithm uses two pointers: one starting at the head and moving forward, and another starting at the tail and moving backward. First, traverse from head to tail to locate the last node (tail). Then, in a loop, compare data at head and tail; if they differ, return false immediately. Move head forward (head = head->next) and tail backward (tail = tail->prev) until the pointers meet or cross. For an odd-length list, they meet at the same node (head == tail), at which point the loop ends and we return true. For an even-length list, they cross (head->prev == tail or head == tail->next) — the loop condition `head != tmp` combined with moving both pointers will terminate correctly because when they cross, `head` becomes `tail` originally or vice versa; in code we must handle that carefully. A simpler robust loop is `while (head != NULL && tail != NULL && head != tail && tail->next != head)` but the original snippet uses `while (head != tmp)`, and to avoid infinite loops in even case, after moving both pointers, if they cross, the condition fails (since `head` becomes the old `tmp` and `tmp` becomes the old `head`? Actually safer: use `while (head != NULL && tail != NULL && head != tail && head->prev != tail)`. Edge cases: empty list (null) should return true (vacuously palindrome). Single node: head == tail, loop not entered, return true. Time complexity O(n) because we traverse the list twice (once to find tail, once to compare). Space O(1) auxiliary.
