Given a singly linked list containing integer values, write a C++ function that rearranges the nodes so that all nodes at odd positions (1st, 3rd, 5th, …) appear first, followed by all nodes at even positions (2nd, 4th, 6th, …), preserving the original relative order within each group. The function should modify the list in place and return the head pointer of the rearranged list. The list will contain at least one node; handle lists of size 1 and 2 gracefully. Do not allocate new nodes or use extra storage.

// The approach uses three pointers: `odd` (points to the current odd-position node), `even` (points to the current even-position node), and `evenHead` (stores the head of the even list). Initialize `odd` to the head, `even` to `head->next`, and `evenHead` to `even`. Traverse the list while both `odd->next` and `even->next` are non‑null. In each iteration, set `odd->next = even->next` (link odd to the next odd), move `odd` forward, then set `even->next = odd->next` (link even to the next even), and move `even` forward. After the loop, connect `odd->next` to `evenHead`. Edge cases: If the list has only one node, `even` is null, so the function should simply return the head; if the list has two nodes, the while loop does not execute, and we just link `odd->next` to `evenHead` (which is the second node, already correct). Time complexity is O(n) with a single pass; space complexity is O(1) since only pointers are used.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Rearrange the linked list so that odd-indexed nodes come before even-indexed nodes.
// Returns the head of the modified list. The list must contain at least one node.
Node* reorderOddEven(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Node* odd = head;
    Node* even = head->next;
    Node* evenHead = even;

    while (odd->next != nullptr && even->next != nullptr) {
        odd->next = even->next;
        odd = odd->next;
        even->next = odd->next;
        even = even->next;
    }

    odd->next = evenHead;
    return head;
}

#include <cassert>
#include <vector>

// Helper to build a list from a vector
Node* buildList(const std::vector<int>& values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (head == nullptr) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to vector for comparison
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to free list memory
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Single node
    Node* l1 = buildList({5});
    l1 = reorderOddEven(l1);
    assert(listToVector(l1) == std::vector<int>({5}));
    freeList(l1);

    // Two nodes
    Node* l2 = buildList({1, 2});
    l2 = reorderOddEven(l2);
    assert(listToVector(l2) == std::vector<int>({1, 2}));
    freeList(l2);

    // Four nodes (original example: 1,2,3,4)
    Node* l3 = buildList({1, 2, 3, 4});
    l3 = reorderOddEven(l3);
    assert(listToVector(l3) == std::vector<int>({1, 3, 2, 4}));
    freeList(l3);

    // Five nodes
    Node* l4 = buildList({1, 2, 3, 4, 5});
    l4 = reorderOddEven(l4);
    assert(listToVector(l4) == std::vector<int>({1, 3, 5, 2, 4}));
    freeList(l4);

    // Six nodes
    Node* l5 = buildList({1, 2, 3, 4, 5, 6});
    l5 = reorderOddEven(l5);
    assert(listToVector(l5) == std::vector<int>({1, 3, 5, 2, 4, 6}));
    freeList(l5);

    // Negative and zero values
    Node* l6 = buildList({0, -1, 2, -3, 4});
    l6 = reorderOddEven(l6);
    assert(listToVector(l6) == std::vector<int>({0, 2, 4, -1, -3}));
    freeList(l6);

    // Long even-length list
    Node* l7 = buildList({10, 20, 30, 40, 50, 60, 70, 80});
    l7 = reorderOddEven(l7);
    assert(listToVector(l7) == std::vector<int>({10, 30, 50, 70, 20, 40, 60, 80}));
    freeList(l7);

    return 0;
}
