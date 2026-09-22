// Write a C++ function that takes two singly linked lists of integers, `p` and `q`, and merges the second list into the first at alternate positions. Specifically, starting from the heads, the function should insert nodes from `q` into `p` such that after the operation, the list `p` contains `p1, q1, p2, q2, p3, q3, ...` where `p_i` are nodes from the original `p` and `q_i` are nodes from the original `q`. The function should modify the lists in-place without allocating new nodes. After the merge, the remaining nodes of `q` (if any) form the new head of `q`. The function signature should be `void mergeAlternate(node* p, node** q)`, where `p` is the head of the first list and `q` is a pointer to the head pointer of the second list. If either list is empty, the function should do nothing. The node structure is defined as `struct node { int data; node* next; node(int d) : data(d), next(nullptr) {} };`. Your solution must handle lists of arbitrary lengths, and the function must not create any new nodes; it must only rewire existing pointers.

// The core algorithm uses two pointers, one for each list. Starting with `p_curr` pointing to the current node in `p` and `q_curr` pointing to the current node in `q`, we repeatedly save their next pointers (`p_next` and `q_next`). Then we make the current `q` node point to `p_next` and make `p_curr` point to the `q` node. This inserts `q_curr` right after `p_curr`. Then we advance `p_curr` to `p_next` and `q_curr` to `q_next`. This loop continues while both `p_curr` and `q_curr` are non-null. After the loop, the remaining nodes of `q` (starting from `q_curr`) become the new head of `q`; we set `*q = q_curr`. Important edge cases: if either list is initially empty, do nothing. If `p` is shorter than `q`, the loop stops when `p_curr` becomes null, and all remaining `q` nodes stay in `q`. If `q` is shorter, the loop stops when `q_curr` becomes null, and `q` becomes null (empty). Time complexity is O(n + m) where n and m are lengths of the lists, but since we only traverse each node once, it is O(n) for the merged portion (bounded by the shorter list). Space complexity is O(1) as we only use a few pointers.

#include <cstddef>

struct node {
    int data;
    node* next;
    node(int d) : data(d), next(nullptr) {}
};

// Merge nodes from list q into list p at alternate positions.
// p: head of the first list (non-const, since we modify its next pointers)
// q: pointer to head pointer of the second list; after merge, *q points to remaining nodes of q
void mergeAlternate(node* p, node** q) {
    if (p == nullptr || (*q) == nullptr) {
        return;
    }

    node* p_curr = p;
    node* q_curr = *q;

    while (p_curr != nullptr && q_curr != nullptr) {
        node* p_next = p_curr->next;
        node* q_next = q_curr->next;

        // Insert q_curr right after p_curr
        q_curr->next = p_next;
        p_curr->next = q_curr;

        // Advance both pointers
        p_curr = p_next;
        q_curr = q_next;
    }

    // Update head of q to the remaining nodes (or nullptr if exhausted)
    *q = q_curr;
}

#include <cassert>

// Helper to create a list from an array (for testing)
node* createList(const int* arr, int size) {
    if (size == 0) return nullptr;
    node* head = new node(arr[0]);
    node* curr = head;
    for (int i = 1; i < size; ++i) {
        curr->next = new node(arr[i]);
        curr = curr->next;
    }
    return head;
}

// Helper to convert a list to a vector for comparison
std::vector<int> listToVector(node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to delete a list (for memory cleanup)
void deleteList(node* head) {
    while (head != nullptr) {
        node* temp = head->next;
        delete head;
        head = temp;
    }
}

int main() {
    // Test 1: Standard case from the snippet
    {
        int pArr[] = {1, 2, 3};
        int qArr[] = {4, 5, 6, 7, 8};
        node* p = createList(pArr, 3);
        node* q = createList(qArr, 5);
        mergeAlternate(p, &q);
        std::vector<int> pExpected = {1, 4, 2, 5, 3, 6};
        std::vector<int> qExpected = {7, 8};
        assert(listToVector(p) == pExpected);
        if (q == nullptr) assert(qExpected.empty());
        else assert(listToVector(q) == qExpected);
        deleteList(p);
        deleteList(q);
    }

    // Test 2: Empty first list
    {
        int qArr[] = {1, 2};
        node* p = nullptr;
        node* q = createList(qArr, 2);
        mergeAlternate(p, &q);
        assert(p == nullptr);
        assert(listToVector(q) == std::vector<int>({1, 2}));
        deleteList(q);
    }

    // Test 3: Empty second list
    {
        int pArr[] = {5, 6};
        node* p = createList(pArr, 2);
        node* q = nullptr;
        mergeAlternate(p, &q);
        assert(listToVector(p) == std::vector<int>({5, 6}));
        assert(q == nullptr);
        deleteList(p);
    }

    // Test 4: Both lists have equal length
    {
        int pArr[] = {1, 2};
        int qArr[] = {3, 4};
        node* p = createList(pArr, 2);
        node* q = createList(qArr, 2);
        mergeAlternate(p, &q);
        assert(listToVector(p) == std::vector<int>({1, 3, 2, 4}));
        assert(q == nullptr);
        deleteList(p);
    }

    // Test 5: Second list shorter than first
    {
        int pArr[] = {1, 2, 3, 4};
        int qArr[] = {9};
        node* p = createList(pArr, 4);
        node* q = createList(qArr, 1);
        mergeAlternate(p, &q);
        std::vector<int> pExpected = {1, 9, 2, 3, 4};
        assert(listToVector(p) == pExpected);
        assert(q == nullptr);
        deleteList(p);
    }

    // Test 6: Both lists have a single node
    {
        node* p = new node(10);
        node* q = new node(20);
        mergeAlternate(p, &q);
        assert(p->data == 10 && p->next->data == 20 && p->next->next == nullptr);
        assert(q == nullptr);
        delete p->next;
        delete p;
    }

    // Test 7: First list longer than second, second fully consumed
    {
        int pArr[] = {1, 2, 3, 4, 5};
        int qArr[] = {6, 7};
        node* p = createList(pArr, 5);
        node* q = createList(qArr, 2);
        mergeAlternate(p, &q);
        std::vector<int> pExpected = {1, 6, 2, 7, 3, 4, 5};
        assert(listToVector(p) == pExpected);
        assert(q == nullptr);
        deleteList(p);
    }

    // Test 8: Ensure no new nodes are allocated (we can't easily assert this, but we verify pointer reuse)
    // This test checks that the total number of nodes in both lists remains the same as before.
    {
        int pArr[] = {1, 2};
        int qArr[] = {3, 4, 5};
        node* p = createList(pArr, 2);
        node* q = createList(qArr, 3);
        int countBefore = 0;
        for (node* cur = p; cur; cur = cur->next) ++countBefore;
        for (node* cur = q; cur; cur = cur->next) ++countBefore;
        mergeAlternate(p, &q);
        int countAfter = 0;
        for (node* cur = p; cur; cur = cur->next) ++countAfter;
        for (node* cur = q; cur; cur = cur->next) ++countAfter;
        assert(countBefore == countAfter);
        deleteList(p);
        deleteList(q);
    }

    return 0;
}
