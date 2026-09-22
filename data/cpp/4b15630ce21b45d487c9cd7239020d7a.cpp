Write a C++ free function `Node* removeNonIncreasingNodes(Node* head)` that takes a singly linked list of integers and returns the head of a new linked list containing only those nodes whose value is greater than or equal to the maximum value seen so far from the head of the original list. In other words, scan the list from left to right; keep a node if its data is at least as large as every previous node's data; discard all other nodes. The original list order must be preserved in the output. The linked list is defined by the standard `struct Node` with `int data` and `Node* next`, and a constructor. The list is non-empty. The function should not allocate new nodes; it should remove unwanted nodes from the existing list and return a pointer to the new head (which may be the original head). Do not print anything and do not modify the node values.

// The problem reduces to selecting nodes that form a non-decreasing sequence when read from head to tail. A straightforward approach is to traverse the list once while maintaining the maximum value encountered so far. For each node, if its data is greater than or equal to the current maximum, we keep it (and update the maximum), otherwise we delete it from the list. This can be done with a single pass and constant extra space. Edge cases: if the list has only one node, it is trivially kept; if the list is strictly decreasing, only the first node is kept; if all values are equal, all nodes are kept. Time complexity is O(n) because each node is visited once, and space complexity is O(1) since we only use a few pointers. Deleting a node requires careful pointer updates to avoid losing track of the next node. The implementation should use a dummy node or handle the head separately; using a pointer-to-pointer simplifies the removal logic.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int x) : data(x), next(nullptr) {}
};

// Keep nodes whose data is >= all previous nodes' data; return new head.
// The original list is modified in place; no new nodes are allocated.
Node* removeNonIncreasingNodes(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    int maxSoFar = head->data;
    Node* prev = head;
    Node* curr = head->next;
    while (curr != nullptr) {
        if (curr->data >= maxSoFar) {
            maxSoFar = curr->data;
            prev = curr;
            curr = curr->next;
        } else {
            // Remove curr
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        }
    }
    return head;
}

#include <cassert>

// Helper to build a linked list from an initializer list.
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (head == nullptr) {
            head = n;
            tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to vector for comparison.
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    Node* curr = head;
    while (curr != nullptr) {
        result.push_back(curr->data);
        curr = curr->next;
    }
    return result;
}

// Helper to delete list.
void deleteList(Node* head) {
    Node* curr = head;
    while (curr != nullptr) {
        Node* next = curr->next;
        delete curr;
        curr = next;
    }
}

int main() {
    // Case 1: Already non-decreasing, all kept.
    Node* h1 = buildList({1, 2, 3, 4});
    Node* r1 = removeNonIncreasingNodes(h1);
    assert(listToVector(r1) == std::vector<int>({1, 2, 3, 4}));
    deleteList(r1);

    // Case 2: Strictly decreasing, keep only first.
    h1 = buildList({5, 4, 3, 2, 1});
    r1 = removeNonIncreasingNodes(h1);
    assert(listToVector(r1) == std::vector<int>({5}));
    deleteList(r1);

    // Case 3: Mixed with equal values.
    h1 = buildList({4, 4, 3, 4, 2, 5});
    r1 = removeNonIncreasingNodes(h1);
    assert(listToVector(r1) == std::vector<int>({4, 4, 4, 5}));
    deleteList(r1);

    // Case 4: Single element.
    h1 = new Node(10);
    r1 = removeNonIncreasingNodes(h1);
    assert(listToVector(r1) == std::vector<int>({10}));
    deleteList(r1);

    // Case 5: All equal.
    h1 = buildList({7, 7, 7});
    r1 = removeNonIncreasingNodes(h1);
    assert(listToVector(r1) == std::vector<int>({7, 7, 7}));
    deleteList(r1);

    // Case 6: Alternating pattern.
    h1 = buildList({3, 1, 2, 0, 5, 4});
    r1 = removeNonIncreasingNodes(h1);
    assert(listToVector(r1) == std::vector<int>({3, 5}));
    deleteList(r1);

    return 0;
}
