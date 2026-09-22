// Given a singly linked list of integers, write a C++ function named `splitIntoSecondHalf` that takes a `LinkedList<int>` by value (or const reference, but returning a new list is fine) and returns a new `LinkedList<int>` containing only the elements from the second half of the original list. If the list has an odd number of nodes, the middle node should be included in the returned second half. The function must not modify the original list. The `LinkedList` class is provided in the snippet and supports `push_back`, `pop_front`, and `display`, but you may use only `push_back` and iteration for this task. You may assume the list is non-empty. The second half is defined as starting from the node at position `floor(n/2)` when using 0-based indices (i.e., for n=5, the returned list should contain elements at indices 2,3,4; for n=4, indices 2,3). Your function should return the new list by value.
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Assume the Singly::LinkedList class from the solution is already defined above.
// To keep test self-contained, paste the solution code before this test.

int main() {
    // Test 1: even number of elements
    Singly::LinkedList<int> a;
    for (int i = 1; i <= 4; ++i) a.push_back(i);
    Singly::LinkedList<int> second = a.splitIntoSecondHalf();
    vector<int> expected = {3, 4};
    Singly::Node<int>* cur = second.head;
    for (int val : expected) {
        assert(cur != nullptr && cur->value == val);
        cur = cur->next;
    }
    assert(cur == nullptr);

    // Test 2: odd number of elements (middle included)
    Singly::LinkedList<int> b;
    for (int i = 1; i <= 5; ++i) b.push_back(i);
    second = b.splitIntoSecondHalf();
    expected = {3, 4, 5};
    cur = second.head;
    for (int val : expected) {
        assert(cur != nullptr && cur->value == val);
        cur = cur->next;
    }
    assert(cur == nullptr);

    // Test 3: single element
    Singly::LinkedList<int> c;
    c.push_back(42);
    second = c.splitIntoSecondHalf();
    expected = {42};
    cur = second.head;
    for (int val : expected) {
        assert(cur != nullptr && cur->value == val);
        cur = cur->next;
    }
    assert(cur == nullptr);

    // Test 4: two elements
    Singly::LinkedList<int> d;
    d.push_back(10);
    d.push_back(20);
    second = d.splitIntoSecondHalf();
    expected = {20};
    cur = second.head;
    for (int val : expected) {
        assert(cur != nullptr && cur->value == val);
        cur = cur->next;
    }
    assert(cur == nullptr);

    // Test 5: ensure original list is not modified (size and order)
    assert(a.numberOfItems == 4);
    cur = a.head;
    expected = {1, 2, 3, 4};
    for (int val : expected) {
        assert(cur != nullptr && cur->value == val);
        cur = cur->next;
    }
    assert(cur == nullptr);

    cout << "All tests passed!" << endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

namespace Singly {
    template<typename T>
    class Node {
        public:
        T value;
        Node<T>* next;
        Node<T>* previous;
    };

    template<typename T>
    class LinkedList {
        public:
        Node<T>* head;
        Node<T>* tail;
        int numberOfItems;

        LinkedList() {
            head = nullptr;
            tail = nullptr;
            numberOfItems = 0;
        }

        LinkedList(const LinkedList& other) { // copy constructor for safety
            head = nullptr;
            tail = nullptr;
            numberOfItems = 0;
            Node<T>* cur = other.head;
            while (cur != nullptr) {
                push_back(cur->value);
                cur = cur->next;
            }
        }

        LinkedList& operator=(const LinkedList& other) {
            if (this == &other) return *this;
            // clear existing list
            while (head != nullptr) {
                Node<T>* temp = head;
                head = head->next;
                delete temp;
            }
            head = nullptr;
            tail = nullptr;
            numberOfItems = 0;
            Node<T>* cur = other.head;
            while (cur != nullptr) {
                push_back(cur->value);
                cur = cur->next;
            }
            return *this;
        }

        ~LinkedList() {
            Node<T>* cur = head;
            while (cur != nullptr) {
                Node<T>* next = cur->next;
                delete cur;
                cur = next;
            }
        }

        void push_back(T n) {
            numberOfItems++;
            Node<T>* newNode = new Node<T>();
            newNode->value = n;
            newNode->next = nullptr;
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            }
            else if (head->next == nullptr) {
                head->next = newNode;
                tail = newNode;
            }
            else {
                tail->next = newNode;
                tail = newNode;
            }
        }

        void pop_front() {
            if (head == nullptr) return;
            numberOfItems--;
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            if (head == nullptr) tail = nullptr;
        }

        void display() {
            Node<T>* cur = head;
            while (cur != nullptr) {
                cout << cur->value << " ";
                cur = cur->next;
            }
            cout << endl;
        }

        // Return a new list containing only the second half of this list.
        LinkedList<T> splitIntoSecondHalf() const {
            LinkedList<T> result;
            if (head == nullptr) return result; // empty input, return empty (though task says non-empty)

            // Use slow-fast pointer to find the start of the second half.
            Node<T>* slow = head;
            Node<T>* fast = head;
            while (fast != nullptr && fast->next != nullptr) {
                slow = slow->next;
                fast = fast->next->next;
            }
            // Now slow points to the first node of the second half.
            Node<T>* cur = slow;
            while (cur != nullptr) {
                result.push_back(cur->value);
                cur = cur->next;
            }
            return result;
        }
    };
}

// Free function wrapper to match task style (optional, but we provide it)
template<typename T>
Singly::LinkedList<T> getSecondHalf(const Singly::LinkedList<T>& list) {
    return list.splitIntoSecondHalf();
}
// The core idea is to find the starting node of the second half using the classic slow‑and‑fast pointer technique. Initialize both `slow` and `fast` to the head of the list. Move `fast` two steps at a time and `slow` one step at a time. When `fast` reaches the end (either `fast == nullptr` for even-length lists or `fast->next == nullptr` for odd-length lists), `slow` will be pointing to the first element of the second half. This works because when `fast` has moved `k` steps, `fast` is twice as far ahead as `slow`. For an odd number of nodes (e.g., 5), `fast` ends at the last node (since `fast->next` becomes nullptr after 2 steps from head), and `slow` ends at index 2, which is the middle. For an even number (e.g., 4), `fast` becomes nullptr after traversing two steps (head->next->next), and `slow` ends at index 2, which is the first of the second half. After locating `slow`, iterate from that node to the end, pushing each value into a new `LinkedList<int>`. Edge cases: if the list has exactly one node, `slow` will be the only node and the returned list will contain that one element; if the list has two nodes, `slow` will be the second node. Time complexity is O(n) because we traverse the list once with the two pointers and then again to copy the second half, giving O(n) total. Space complexity is O(n) for the new list, but auxiliary space (excluding the returned list) is O(1). The original list is not mutated because we never modify pointers or values of the input list.
