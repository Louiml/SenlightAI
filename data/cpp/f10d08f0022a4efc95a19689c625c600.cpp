Write a C++ function `rotateSublist` that takes a non-empty singly linked list of integers in which the list consists of two strictly increasing contiguous segments (for example, `17 → 2 → 3 → 5 → 6 → 8 → 10 → 12 → 1` or a list already fully sorted) and swaps the two segments so that the resulting list is strictly increasing from start to end. The function must modify the list in place and return the new head pointer. The input list is guaranteed to have at least two nodes, and the boundary between the two increasing segments is exactly where the first decrease occurs; you may assume that after the first decrease, all subsequent values are strictly increasing until the end, and that the largest value of the first segment is less than the smallest value of the second segment. Handle the edge case where the list is already sorted (no decrease) by doing nothing.

// The key observation is that the list is composed of two already‑sorted increasing runs. We need to locate the first node where `current->num > current->next->num`; this marks the end of the first increasing segment. The next node is the start of the second segment. The first segment consists of nodes from the head up to that boundary node, and the second segment consists of all remaining nodes. Because the given constraints guarantee that every value in the first segment is smaller than every value in the second segment, the correct sorted order is obtained by moving the entire second segment to the front and appending the first segment to the end. This is a pointer manipulation problem: we store the head of the second segment, find the tail of the second segment, and then set the tail’s next pointer to the original head, while setting the new head to the start of the second segment. If the list is already sorted (no decrease found), we simply return the original head. The algorithm visits each node at most twice (once to find the boundary and once to find the tail of the second segment), so time complexity is O(n). Space complexity is O(1) auxiliary because we only use a few pointers.

#include <cstddef>

struct Node {
    int num;
    Node* next;
    explicit Node(int value) : num(value), next(nullptr) {}
};

// Rearranges the two increasing segments so the whole list becomes sorted.
// Returns the new head of the list after the rotation.
Node* rotateSublist(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    // Find the boundary where the first increasing segment ends.
    Node* boundary = head;
    while (boundary->next != nullptr && boundary->num < boundary->next->num) {
        boundary = boundary->next;
    }

    // If no decrease is found, the list is already sorted.
    if (boundary->next == nullptr) {
        return head;
    }

    // secondStart is the head of the second increasing segment.
    Node* secondStart = boundary->next;
    Node* secondTail = secondStart;

    // Find the tail of the second segment.
    while (secondTail->next != nullptr) {
        secondTail = secondTail->next;
    }

    // Link the second segment's tail to the original head.
    secondTail->next = head;
    // Terminate the list at the old boundary.
    boundary->next = nullptr;

    return secondStart;
}

#include <cassert>

// Helper to build a list from an initializer list.
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* newNode = new Node(v);
        if (tail == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to convert list to a vector-like array for comparison.
void collectAndCheck(Node* head, const int* expected, size_t len) {
    Node* cur = head;
    for (size_t i = 0; i < len; ++i) {
        assert(cur != nullptr);
        assert(cur->num == expected[i]);
        cur = cur->next;
    }
    assert(cur == nullptr);
}

// Free list memory.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Example from the snippet: 17 → 2 → 3 → 5 → 6 → 8 → 10 → 12 → 1
    Node* h1 = buildList({17, 2, 3, 5, 6, 8, 10, 12, 1});
    h1 = rotateSublist(h1);
    const int expected1[] = {1, 2, 3, 5, 6, 8, 10, 12, 17};
    collectAndCheck(h1, expected1, 9);
    deleteList(h1);

    // Already sorted list: 1 → 2 → 3
    Node* h2 = buildList({1, 2, 3});
    h2 = rotateSublist(h2);
    const int expected2[] = {1, 2, 3};
    collectAndCheck(h2, expected2, 3);
    deleteList(h2);

    // Two segments of equal length: 4 → 5 → 1 → 2 → 3
    Node* h3 = buildList({4, 5, 1, 2, 3});
    h3 = rotateSublist(h3);
    const int expected3[] = {1, 2, 3, 4, 5};
    collectAndCheck(h3, expected3, 5);
    deleteList(h3);

    // Two‑node list: 7 → 3
    Node* h4 = buildList({7, 3});
    h4 = rotateSublist(h4);
    const int expected4[] = {3, 7};
    collectAndCheck(h4, expected4, 2);
    deleteList(h4);

    // Larger second segment: 2 → 4 → 1 → 3 → 5 → 6
    Node* h5 = buildList({2, 4, 1, 3, 5, 6});
    h5 = rotateSublist(h5);
    const int expected5[] = {1, 3, 5, 6, 2, 4};
    // Wait: The constraints say first segment max < second segment min. This case violates (4 > 1), so skip.
    // Replace with a valid case: 5 → 6 → 1 → 2 → 3 → 4
    deleteList(h5);
    Node* h6 = buildList({5, 6, 1, 2, 3, 4});
    h6 = rotateSublist(h6);
    const int expected6[] = {1, 2, 3, 4, 5, 6};
    collectAndCheck(h6, expected6, 6);
    deleteList(h6);

    return 0;
}
