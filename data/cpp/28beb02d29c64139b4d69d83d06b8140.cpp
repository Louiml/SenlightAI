// Write a C++ function that operates on a singly linked list of integers. The function must take a pointer to the head pointer of the list and a pointer to a specific node within that list, and delete that node from the list in O(1) time on average (i.e., without traversing the entire list to find the predecessor), while correctly handling all three cases: the node is not the tail, the node is the only node in the list, and the node is the tail but not the only node. The function should free the memory of the deleted node and update the head pointer when necessary. You are not allowed to use any container classes from the standard library; you must define your own node structure. The function signature must be: `void DeleteNode(LinkNode **pListHead, LinkNode *pToBeDeleted);` where `struct LinkNode { int value; LinkNode *next; };`. Your implementation must be self-contained and must not rely on any global state.
// The classic "delete a node in O(1) without knowing the head" trick works when the node is not the tail: copy the value and next pointer from the successor into the current node, then unlink and delete the successor. This avoids traversing to find the predecessor. However, when the node is the tail, we must fall back to traversing from the head to find the predecessor, because we cannot copy from a non-existent successor. There are three cases: (1) If `pToBeDeleted->next` is not null, copy the successor's value and next pointer, then delete the successor. (2) If the node is the tail and also the head (i.e., the only node), simply delete the node and set the head pointer to null. (3) If the node is the tail but not the head, traverse from the head until we find the node whose `next` equals `pToBeDeleted`, set that `next` to null, then delete the tail. All other nodes remain unchanged. The average time complexity is O(1) for non-tail deletions and O(n) only for tail deletions, where n is the list length. The worst-case time is O(n) when deleting the tail. Space complexity is O(1) as we only use a few temporary pointers. Edge cases include null head pointer, null node pointer, and the list having exactly one node. The function must not leak memory and must set the deleted pointer to null after freeing it.
#include <cstddef>

struct LinkNode {
    int value;
    LinkNode *next;
};

// Deletes a node from a singly linked list in O(1) average time.
// Handles three cases: non-tail, only node, and tail (non-only).
void DeleteNode(LinkNode **pListHead, LinkNode *pToBeDeleted) {
    if (pListHead == nullptr || pToBeDeleted == nullptr) {
        return;
    }

    // Case 1: Not the tail node.
    if (pToBeDeleted->next != nullptr) {
        LinkNode *pNext = pToBeDeleted->next;
        pToBeDeleted->value = pNext->value;
        pToBeDeleted->next = pNext->next;
        delete pNext;
        pNext = nullptr;
    }
    // Case 2: Only node in the list (also the tail).
    else if (*pListHead == pToBeDeleted) {
        delete pToBeDeleted;
        pToBeDeleted = nullptr;
        *pListHead = nullptr;
    }
    // Case 3: Tail but not the only node.
    else {
        LinkNode *pPrev = *pListHead;
        while (pPrev->next != pToBeDeleted) {
            pPrev = pPrev->next;
        }
        pPrev->next = nullptr;
        delete pToBeDeleted;
        pToBeDeleted = nullptr;
    }
}
#include <cassert>

// Reuse the LinkNode and DeleteNode declarations from above (or include them here).
// For clarity, the solution function is considered already defined.

// Helper to build a list from an array and return the head.
LinkNode* buildList(const int* arr, int size) {
    if (size == 0) return nullptr;
    LinkNode* head = new LinkNode{arr[0], nullptr};
    LinkNode* tail = head;
    for (int i = 1; i < size; ++i) {
        tail->next = new LinkNode{arr[i], nullptr};
        tail = tail->next;
    }
    return head;
}

// Helper to convert a list to a vector-like array (for simplicity, use a fixed size).
void listToArray(LinkNode* head, int* arr, int size) {
    LinkNode* cur = head;
    for (int i = 0; i < size; ++i) {
        arr[i] = cur->value;
        cur = cur->next;
    }
}

// Helper to count the length.
int listLength(LinkNode* head) {
    int len = 0;
    while (head) { ++len; head = head->next; }
    return len;
}

int main() {
    // Test 1: Delete a non-tail node (e.g., second node from a list of 5).
    {
        int vals[] = {1, 2, 3, 4, 5};
        LinkNode* head = buildList(vals, 5);
        LinkNode* nodeToDelete = head->next; // value 2
        DeleteNode(&head, nodeToDelete);
        assert(listLength(head) == 4);
        int out[4];
        listToArray(head, out, 4);
        assert(out[0] == 1 && out[1] == 3 && out[2] == 4 && out[3] == 5);
        // Clean up remaining
        while (head) { LinkNode* tmp = head->next; delete head; head = tmp; }
    }

    // Test 2: Delete the only node.
    {
        LinkNode* head = new LinkNode{42, nullptr};
        DeleteNode(&head, head);
        assert(head == nullptr);
    }

    // Test 3: Delete the tail node (not the only node).
    {
        int vals[] = {10, 20, 30};
        LinkNode* head = buildList(vals, 3);
        LinkNode* tail = head->next->next; // value 30
        DeleteNode(&head, tail);
        assert(listLength(head) == 2);
        int out[2];
        listToArray(head, out, 2);
        assert(out[0] == 10 && out[1] == 20);
        while (head) { LinkNode* tmp = head->next; delete head; head = tmp; }
    }

    // Test 4: Delete the head node (non-tail case, head has a successor).
    {
        int vals[] = {7, 8, 9};
        LinkNode* head = buildList(vals, 3);
        LinkNode* first = head;
        DeleteNode(&head, first); // value 7
        assert(listLength(head) == 2);
        assert(head->value == 8);
        assert(head->next->value == 9);
        while (head) { LinkNode* tmp = head->next; delete head; head = tmp; }
    }

    // Test 5: Delete a middle node with duplicates.
    {
        int vals[] = {5, 5, 5, 5};
        LinkNode* head = buildList(vals, 4);
        LinkNode* third = head->next->next; // third 5
        DeleteNode(&head, third);
        assert(listLength(head) == 3);
        int out[3];
        listToArray(head, out, 3);
        assert(out[0] == 5 && out[1] == 5 && out[2] == 5);
        while (head) { LinkNode* tmp = head->next; delete head; head = tmp; }
    }

    return 0;
}
