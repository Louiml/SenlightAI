// Write a C++ function `splitListBySign` that takes three singly linked list pointers as arguments: an input list `La` and two empty output lists `Lb` and `Lc`. The function should traverse the input list and distribute its nodes into the output lists: all nodes containing negative integer values should be moved to `Lb` (preserving their original relative order), and all nodes containing non-negative values (including zero) should be moved to `Lc` (also preserving the original relative order). After the operation, the input list `La` should be empty (all its nodes have been relocated), and the function should not allocate any new nodes—it must reuse the existing nodes by rearranging pointers. Return `void`. The function should handle an empty input list gracefully (both outputs remain empty). Assume each node stores an integer and a pointer to the next node, and that the lists are singly linked without a sentinel (dummy) head node.
// The core idea is to iterate through the source list `La` using a current pointer, and for each node, detach it by advancing the traversal pointer first, then append the node to the tail of the appropriate output list. To maintain constant-time append operations, we maintain both a head and a tail pointer for each output list. Initially, `Lb` and `Lc` are null pointers (empty). For each node in `La`, check if its value is negative: if so, append it to `Lb`; otherwise, append it to `Lc`. Appending is done by setting the current tail's next pointer (if tail exists) to the new node, then updating the tail; if the list was empty, set both head and tail to the node. After processing, set the next pointer of the last node of each output list to `nullptr`, and set `La` to `nullptr` to indicate it's empty. Edge cases: empty `La` — loop does nothing and outputs remain null; all nodes negative or all non-negative — one output list receives all nodes, the other remains null; the last node of each output list must be terminated. Time complexity is O(n) where n is the number of nodes in `La`, since each node is visited once and constant work per node. Space complexity is O(1) auxiliary, as no new nodes are created.
#include <cstddef> // for nullptr

// Node definition for singly linked list
struct LinkNode {
    int data;
    LinkNode* next;
};

// Split list La into Lb (negative values) and Lc (non-negative values).
// Reuses existing nodes; La becomes empty after the operation.
void splitListBySign(LinkNode*& La, LinkNode*& Lb, LinkNode*& Lc) {
    Lb = nullptr;
    Lc = nullptr;
    LinkNode* tailB = nullptr;
    LinkNode* tailC = nullptr;

    LinkNode* current = La;
    while (current != nullptr) {
        LinkNode* nextNode = current->next; // save next before detaching
        current->next = nullptr; // detach node

        if (current->data < 0) {
            // Append to Lb
            if (tailB == nullptr) {
                Lb = current;
                tailB = current;
            } else {
                tailB->next = current;
                tailB = current;
            }
        } else {
            // Append to Lc
            if (tailC == nullptr) {
                Lc = current;
                tailC = current;
            } else {
                tailC->next = current;
                tailC = current;
            }
        }

        current = nextNode;
    }

    La = nullptr; // input list is now empty
}
#include <cassert>

int main() {
    // Helper to create a list from an array
    LinkNode* createList(int arr[], int n) {
        if (n == 0) return nullptr;
        LinkNode* head = new LinkNode{arr[0], nullptr};
        LinkNode* tail = head;
        for (int i = 1; i < n; ++i) {
            tail->next = new LinkNode{arr[i], nullptr};
            tail = tail->next;
        }
        return head;
    }

    // Helper to compare list values with array
    bool listMatches(LinkNode* head, int arr[], int n) {
        for (int i = 0; i < n; ++i) {
            if (head == nullptr || head->data != arr[i]) return false;
            head = head->next;
        }
        return head == nullptr;
    }

    // Helper to free list
    void freeList(LinkNode* head) {
        while (head) {
            LinkNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Test 1: Mixed values
    {
        int a[] = {-1, -2, -3, 4, 5, -17, 20, -23, 25};
        int n = sizeof(a) / sizeof(a[0]);
        LinkNode* La = createList(a, n);
        LinkNode* Lb = nullptr;
        LinkNode* Lc = nullptr;
        splitListBySign(La, Lb, Lc);
        int b[] = {-1, -2, -3, -17, -23};
        int c[] = {4, 5, 20, 25};
        assert(listMatches(Lb, b, 5));
        assert(listMatches(Lc, c, 4));
        assert(La == nullptr);
        freeList(Lb);
        freeList(Lc);
    }

    // Test 2: All negative
    {
        int a[] = {-5, -1, -10};
        LinkNode* La = createList(a, 3);
        LinkNode* Lb = nullptr;
        LinkNode* Lc = nullptr;
        splitListBySign(La, Lb, Lc);
        int b[] = {-5, -1, -10};
        assert(listMatches(Lb, b, 3));
        assert(Lc == nullptr);
        assert(La == nullptr);
        freeList(Lb);
    }

    // Test 3: All non-negative (including zero)
    {
        int a[] = {0, 3, 7};
        LinkNode* La = createList(a, 3);
        LinkNode* Lb = nullptr;
        LinkNode* Lc = nullptr;
        splitListBySign(La, Lb, Lc);
        int c[] = {0, 3, 7};
        assert(Lb == nullptr);
        assert(listMatches(Lc, c, 3));
        assert(La == nullptr);
        freeList(Lc);
    }

    // Test 4: Empty list
    {
        LinkNode* La = nullptr;
        LinkNode* Lb = nullptr;
        LinkNode* Lc = nullptr;
        splitListBySign(La, Lb, Lc);
        assert(La == nullptr);
        assert(Lb == nullptr);
        assert(Lc == nullptr);
    }

    // Test 5: Single node negative
    {
        int a[] = {-7};
        LinkNode* La = createList(a, 1);
        LinkNode* Lb = nullptr;
        LinkNode* Lc = nullptr;
        splitListBySign(La, Lb, Lc);
        int b[] = {-7};
        assert(listMatches(Lb, b, 1));
        assert(Lc == nullptr);
        assert(La == nullptr);
        freeList(Lb);
    }

    return 0;
}
