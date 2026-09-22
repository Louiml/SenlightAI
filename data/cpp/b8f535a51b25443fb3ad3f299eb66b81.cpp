Write a C++ function that takes an integer `N` and a pointer to the head of a singly linked list containing `N` elements, and returns a new linked list (with newly allocated nodes) where all even-valued nodes appear before all odd-valued nodes, preserving the original relative order within each parity group. The function should not modify the input list. The linked list nodes are defined as a `struct Node` with an `int data` and a `Node* next` pointer, initialized with a constructor `Node(int x)`. The returned list's head pointer must be returned. If the input list is empty or `N` is 0, return `nullptr`.
The main idea is to traverse the original linked list once, building two separate new lists: one for even values and one for odd values, each preserving the original order of elements. We maintain dummy head nodes (`evenDummy` and `oddDummy`) to simplify appending, and tail pointers to efficiently add nodes. For each node in the original list, we check if `data % 2 == 0`; if so, we append a new node with that value to the even list; otherwise, we append to the odd list. After the traversal, we link the tail of the even list to the head of the odd list (skipping the odd dummy), and return the head of the even list (skipping the even dummy). If there are no even elements, the evenDummy's next is `nullptr`, and we append the odd list directly; the returned head will point to the first odd element. Edge cases include an empty list (return `nullptr`), all even or all odd numbers, and duplicate values (which are preserved). Time complexity is O(N) because we visit each node exactly once. Auxiliary space is O(N) for the new linked list; no additional data structures are used beyond a few pointers.
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int x) : data(x), next(nullptr) {}
};

// Given the number of nodes N and the head of a singly linked list,
// return a new linked list with even-valued nodes first, then odd-valued nodes,
// preserving original relative order. The input list is not modified.
Node* divideEvenOdd(int N, Node* head) {
    if (head == nullptr || N == 0) {
        return nullptr;
    }

    Node evenDummy(0);
    Node oddDummy(0);
    Node* evenTail = &evenDummy;
    Node* oddTail = &oddDummy;

    Node* current = head;
    for (int i = 0; i < N && current != nullptr; ++i) {
        if (current->data % 2 == 0) {
            evenTail->next = new Node(current->data);
            evenTail = evenTail->next;
        } else {
            oddTail->next = new Node(current->data);
            oddTail = oddTail->next;
        }
        current = current->next;
    }

    evenTail->next = oddDummy.next;
    return evenDummy.next;
}
#include <cassert>
#include <vector>

// Helper to build a linked list from a vector
Node* buildList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    Node* head = new Node(values[0]);
    Node* tail = head;
    for (size_t i = 1; i < values.size(); ++i) {
        tail->next = new Node(values[i]);
        tail = tail->next;
    }
    return head;
}

// Helper to convert linked list to vector for comparison
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to free list memory
void freeList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: mixed even and odd
    {
        Node* input = buildList({1, 2, 3, 4, 5});
        Node* result = divideEvenOdd(5, input);
        assert(listToVector(result) == std::vector<int>({2, 4, 1, 3, 5}));
        freeList(input);
        freeList(result);
    }

    // Test 2: all even
    {
        Node* input = buildList({10, 20, 30});
        Node* result = divideEvenOdd(3, input);
        assert(listToVector(result) == std::vector<int>({10, 20, 30}));
        freeList(input);
        freeList(result);
    }

    // Test 3: all odd
    {
        Node* input = buildList({1, 3, 5});
        Node* result = divideEvenOdd(3, input);
        assert(listToVector(result) == std::vector<int>({1, 3, 5}));
        freeList(input);
        freeList(result);
    }

    // Test 4: odd first then even, order preserved
    {
        Node* input = buildList({5, 3, 8, 7, 2});
        Node* result = divideEvenOdd(5, input);
        assert(listToVector(result) == std::vector<int>({8, 2, 5, 3, 7}));
        freeList(input);
        freeList(result);
    }

    // Test 5: duplicate values
    {
        Node* input = buildList({2, 1, 2, 1});
        Node* result = divideEvenOdd(4, input);
        assert(listToVector(result) == std::vector<int>({2, 2, 1, 1}));
        freeList(input);
        freeList(result);
    }

    // Test 6: empty list
    {
        Node* result = divideEvenOdd(0, nullptr);
        assert(result == nullptr);
    }

    // Test 7: single even
    {
        Node* input = new Node(4);
        Node* result = divideEvenOdd(1, input);
        assert(listToVector(result) == std::vector<int>({4}));
        freeList(input);
        freeList(result);
    }

    // Test 8: single odd
    {
        Node* input = new Node(9);
        Node* result = divideEvenOdd(1, input);
        assert(listToVector(result) == std::vector<int>({9}));
        freeList(input);
        freeList(result);
    }

    // Test 9: negative numbers (parity by modulo)
    {
        Node* input = buildList({-3, -2, -1, 0, 1});
        Node* result = divideEvenOdd(5, input);
        assert(listToVector(result) == std::vector<int>({-2, 0, -3, -1, 1}));
        freeList(input);
        freeList(result);
    }

    // Test 10: input list unchanged
    {
        Node* input = buildList({1, 2, 3});
        Node* result = divideEvenOdd(3, input);
        assert(listToVector(input) == std::vector<int>({1, 2, 3}));
        freeList(input);
        freeList(result);
    }

    return 0;
}
