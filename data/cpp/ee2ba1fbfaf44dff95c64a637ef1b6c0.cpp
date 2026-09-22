// Write a C++ function `bool removeMiddleNode(Node* node)` that takes a pointer to a node within a singly linked list (excluding the head, but possibly including the tail) and deletes that node from the list by copying the data from the next node into the current node and then linking around the next node. The function should return `true` on success and `false` if the input pointer is null or points to the last node (since no next node exists to copy from). You are given a `Node` struct with `int data` and `Node* next`, and a `LinkedList` class with `Node* head` and methods `insertHead(int)` and `printList()`. The task is to implement the function so that it correctly modifies the list in-place without needing access to the head pointer.

// The core idea is to avoid traversing the list to find the previous node, which would be necessary for a conventional deletion. Instead, we copy the data and pointer of the next node into the current node, effectively replacing the current node with its successor. This works only if the current node has a non-null next pointer. If the node is null or is the tail (next is null), we cannot perform the operation and return `false`. Edge cases include: (1) deleting the tail – must return false; (2) deleting a node whose next is the tail – works because we copy the tail's data and set the current node's next to null, effectively removing the tail; (3) deleting a node in the middle – works directly; (4) the list has only one node (head is also tail) – but this function is not meant to delete the head, but if called on the head when it's also the tail, it returns false. The time complexity is O(1) because we do a constant number of operations regardless of list length. The space complexity is O(1) as we only use a few temporary pointers.

#include <iostream>

// Node structure for singly linked list
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Removes a node (not the tail) by copying data from the next node.
// Returns true on success, false if node is null or is the last node.
bool removeMiddleNode(Node* node) {
    if (node == nullptr || node->next == nullptr)
        return false;

    Node* nextNode = node->next;
    node->data = nextNode->data;      // copy data from next node
    node->next = nextNode->next;      // bypass the next node
    delete nextNode;                  // free memory of the removed node

    return true;
}

#include <cassert>
#include <iostream>
#include <vector>

// Include the solution (assume it's above in a real file)
// Node struct and removeMiddleNode as above.

// Helper to build a list from vector and return head
Node* buildList(const std::vector<int>& vals) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : vals) {
        Node* n = new Node(v);
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
    }
    return head;
}

// Helper to convert list to vector for comparison
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head) { result.push_back(head->data); head = head->next; }
    return result;
}

// Helper to free list
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Delete middle node from [1,2,3,4] -> [1,3,4]
    {
        Node* head = buildList({1,2,3,4});
        Node* target = head->next; // 2
        bool ret = removeMiddleNode(target);
        assert(ret == true);
        assert(listToVector(head) == std::vector<int>({1,3,4}));
        freeList(head);
    }

    // Test 2: Delete node before tail from [5,6,7] -> [5,7]
    {
        Node* head = buildList({5,6,7});
        Node* target = head->next; // 6
        bool ret = removeMiddleNode(target);
        assert(ret == true);
        assert(listToVector(head) == std::vector<int>({5,7}));
        freeList(head);
    }

    // Test 3: Attempt to delete tail -> returns false and list unchanged
    {
        Node* head = buildList({10,20});
        Node* tail = head->next;
        bool ret = removeMiddleNode(tail);
        assert(ret == false);
        assert(listToVector(head) == std::vector<int>({10,20}));
        freeList(head);
    }

    // Test 4: Attempt to delete null -> returns false
    {
        Node* head = buildList({1});
        bool ret = removeMiddleNode(nullptr);
        assert(ret == false);
        assert(listToVector(head) == std::vector<int>({1}));
        freeList(head);
    }

    // Test 5: Delete first node when list has only [1,2] -> [2]
    {
        Node* head = buildList({1,2});
        bool ret = removeMiddleNode(head);
        assert(ret == true);
        assert(listToVector(head) == std::vector<int>({2}));
        freeList(head);
    }

    // Test 6: Delete middle of longer list [1,2,3,4,5] -> [1,3,4,5]
    {
        Node* head = buildList({1,2,3,4,5});
        Node* target = head->next->next; // 3
        bool ret = removeMiddleNode(target);
        assert(ret == true);
        assert(listToVector(head) == std::vector<int>({1,2,4,5}));
        freeList(head);
    }

    std::cout << "All tests passed.\n";
    return 0;
}
