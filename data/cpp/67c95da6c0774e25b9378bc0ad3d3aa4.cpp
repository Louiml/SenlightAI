// Write a C++ function `swap_nodes(node* head, int i, int j)` that swaps the nodes at positions `i` and `j` (0-indexed) in a singly linked list, given the head pointer. The function must adjust the next pointers correctly and return the new head. The linked list uses the provided `node` structure (with `int data` and `node* next`). Assume `0 <= i < list_length` and `0 <= j < list_length`, but `i` and `j` may be in any order. The swap must be done by rearranging pointers, not by swapping data values. Handle edge cases such as adjacent nodes, swapping with the head, and ensuring the list remains intact after the swap.

#include <cassert>
#include <iostream>

// Assume node struct and swap_nodes from solution above are included here.

node* create_list(const std::vector<int>& vals) {
    node* head = nullptr;
    node* tail = nullptr;
    for (int v : vals) {
        node* n = new node(v);
        if (!head) head = n;
        else tail->next = n;
        tail = n;
    }
    return head;
}

bool verify_list(node* head, const std::vector<int>& expected) {
    node* curr = head;
    for (int v : expected) {
        if (!curr || curr->data != v) return false;
        curr = curr->next;
    }
    return curr == nullptr;
}

int main() {
    // Test 1: Swap non-adjacent middle nodes
    node* l1 = create_list({1,2,3,4,5});
    l1 = swap_nodes(l1, 1, 3);
    assert(verify_list(l1, {1,4,3,2,5}));

    // Test 2: Swap head with a later node
    node* l2 = create_list({10,20,30,40});
    l2 = swap_nodes(l2, 0, 2);
    assert(verify_list(l2, {30,20,10,40}));

    // Test 3: Swap adjacent nodes
    node* l3 = create_list({7,8,9});
    l3 = swap_nodes(l3, 0, 1);
    assert(verify_list(l3, {8,7,9}));

    // Test 4: Swap with tail
    node* l4 = create_list({5,6,7,8});
    l4 = swap_nodes(l4, 1, 3);
    assert(verify_list(l4, {5,8,7,6}));

    // Test 5: Swap two nodes in reverse index order (j < i)
    node* l5 = create_list({1,2,3,4,5});
    l5 = swap_nodes(l5, 4, 1);
    assert(verify_list(l5, {1,5,3,4,2}));

    // Test 6: Single element list, swap i=j (no change)
    node* l6 = create_list({42});
    l6 = swap_nodes(l6, 0, 0);
    assert(verify_list(l6, {42}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <cstddef>

struct node {
    int data;
    node* next;
    node(int data) : data(data), next(nullptr) {}
};

// Swap nodes at indices i and j (0-indexed) in a singly linked list.
// Returns the new head of the list.
node* swap_nodes(node* head, int i, int j) {
    if (head == nullptr || i == j) {
        return head;
    }

    // Ensure i < j for simpler handling.
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }

    node* iNode = nullptr;
    node* iNodePrev = nullptr;
    node* jNode = nullptr;
    node* jNodePrev = nullptr;

    node* current = head;
    node* prev = nullptr;
    int index = 0;

    // Traverse until we find both nodes.
    while (current != nullptr && (iNode == nullptr || jNode == nullptr)) {
        if (index == i) {
            iNode = current;
            iNodePrev = prev;
        }
        if (index == j) {
            jNode = current;
            jNodePrev = prev;
        }
        prev = current;
        current = current->next;
        ++index;
    }

    // Both nodes were found (assuming valid indices).
    // Step 1: Fix the predecessor links.
    if (iNodePrev != nullptr) {
        iNodePrev->next = jNode;
    } else {
        head = jNode; // i-node was the head.
    }

    if (jNodePrev != nullptr) {
        jNodePrev->next = iNode;
    } else {
        head = iNode; // j-node was the head (only possible if i==0 and j>0, but i<j so this won't happen since iNodePrev==nullptr handles it; kept for safety).
    }

    // Step 2: Fix the next pointers of the swapped nodes.
    node* tempNext = jNode->next;
    jNode->next = iNode->next;
    iNode->next = tempNext;

    return head;
}

// The algorithm traverses the linked list once up to the larger index (`max(i, j)`) while tracking the previous and current pointers for both i-node and j-node. We store four pointers: `iNode`, `iNodePrev`, `jNode`, `jNodePrev`. To avoid redundant traversal, we exit the loop once both nodes are found. After locating them, we handle re-linking in two cases: if `i < j`, we first detach the j-node and re-link its predecessor to i-node, then fix the connections between the two nodes; the tricky part is saving the next pointers before modification. If `i > j`, we swap the roles. Special handling occurs when one of the nodes is the head (previous pointer is null): in that case, the other node becomes the new head. If the nodes are adjacent (e.g., `j = i+1`), the pointer gymnastics must avoid dangling references—this is handled by carefully saving `jNode->next` (or `iNode->next`) before reassigning. Complexity: O(n) time and O(1) extra space.
