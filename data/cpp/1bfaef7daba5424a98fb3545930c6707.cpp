/*
Implement a C++ function `DeepCopyWithRandom` that takes a pointer to the head of a singly-linked list where each node contains an integer `value`, a `next` pointer, and a `random` pointer (which may be null or point to any node in the list, including itself). The function must return a deep copy of the entire list: a new list with the same values, the same `next` linkage, and `random` pointers in the new list pointing to the corresponding copied nodes (or null if the original random pointer was null). The original list must remain unmodified. Do not use any auxiliary data structures such as maps or vectors; the copy must be created using a three-pass in-place algorithm that temporarily interleaves the copy nodes with the original nodes. The function should handle an empty list (returning nullptr), a list with a single node whose random pointer is null or points to itself, and a list where random pointers form cycles. The function signature should be `Node* DeepCopyWithRandom(Node* head)` where `Node` is a struct with `int value; Node* next; Node* random;` and a constructor `Node(int v)`. The solution must be const-correct at the appropriate internal points (though the function itself takes a non-const pointer because it temporarily modifies the original list during the copy, then restores it).
*/

#include <cstddef>

struct Node {
    int value;
    Node* next;
    Node* random;
    Node(int v) : value(v), next(nullptr), random(nullptr) {}
};

// Deep copy a linked list where each node has a random pointer.
// Returns head of the new list. Original list is restored.
Node* DeepCopyWithRandom(Node* head) {
    if (head == nullptr) {
        return nullptr;
    }

    // Pass 1: Create a copy node after each original node.
    Node* curr = head;
    while (curr != nullptr) {
        Node* copy = new Node(curr->value);
        copy->next = curr->next;
        curr->next = copy;
        curr = copy->next;
    }

    // Pass 2: Set random pointers for the copy nodes.
    curr = head;
    while (curr != nullptr) {
        Node* copy = curr->next;
        copy->random = (curr->random != nullptr) ? curr->random->next : nullptr;
        curr = copy->next;
    }

    // Pass 3: Separate the two interleaved lists.
    Node* newHead = head->next;
    curr = head;
    while (curr != nullptr) {
        Node* copy = curr->next;
        curr->next = copy->next;          // restore original next
        curr = curr->next;                // move to original next
        if (curr != nullptr) {
            copy->next = curr->next;      // link copy to next copy
        } else {
            copy->next = nullptr;
        }
    }

    return newHead;
}

int main() {
    // Test 1: Empty list
    Node* empty = nullptr;
    assert(DeepCopyWithRandom(empty) == nullptr);

    // Test 2: Single node, random = nullptr
    Node* n1 = new Node(1);
    Node* copy1 = DeepCopyWithRandom(n1);
    assert(copy1 != nullptr);
    assert(copy1 != n1);
    assert(copy1->value == 1);
    assert(copy1->next == nullptr);
    assert(copy1->random == nullptr);
    assert(n1->next == nullptr);
    assert(n1->random == nullptr);

    // Test 3: Single node, random points to itself
    Node* n2 = new Node(2);
    n2->random = n2;
    Node* copy2 = DeepCopyWithRandom(n2);
    assert(copy2 != nullptr);
    assert(copy2->value == 2);
    assert(copy2->random == copy2);
    assert(n2->random == n2);
    assert(n2->next == nullptr);

    // Test 4: Two nodes, random points to previous
    Node* a = new Node(10);
    Node* b = new Node(20);
    a->next = b;
    a->random = b;  // a's random -> b
    b->random = a;  // b's random -> a
    Node* copyAB = DeepCopyWithRandom(a);
    assert(copyAB != nullptr);
    assert(copyAB->value == 10);
    assert(copyAB->next != nullptr);
    assert(copyAB->next->value == 20);
    assert(copyAB->random == copyAB->next);   // copy of a -> copy of b
    assert(copyAB->next->random == copyAB);   // copy of b -> copy of a
    // Original unchanged
    assert(a->next == b);
    assert(a->random == b);
    assert(b->random == a);

    // Test 5: Three nodes, random cycle pointing to middle
    Node* x = new Node(1);
    Node* y = new Node(2);
    Node* z = new Node(3);
    x->next = y; y->next = z;
    x->random = z;  // x -> z
    y->random = y;  // y -> itself
    z->random = x;  // z -> x
    Node* copyXYZ = DeepCopyWithRandom(x);
    assert(copyXYZ->value == 1);
    assert(copyXYZ->next->value == 2);
    assert(copyXYZ->next->next->value == 3);
    assert(copyXYZ->random == copyXYZ->next->next); // copy of x -> copy of z
    assert(copyXYZ->next->random == copyXYZ->next); // copy of y -> itself
    assert(copyXYZ->next->next->random == copyXYZ); // copy of z -> copy of x
    // Original structure intact
    assert(x->next == y && y->next == z);
    assert(x->random == z && y->random == y && z->random == x);
}

// The algorithm follows the classic three-pass method for deep copying a linked list with random pointers without auxiliary memory.  
// **Pass 1 (Create interleaved copies):** Traverse the original list. For each original node `curr`, create a new node with the same value, insert it between `curr` and `curr->next`, and move to the original next node. After this pass, the list alternates: original, copy, original, copy, ...  
// **Pass 2 (Set random pointers of copies):** Traverse the interleaved list two nodes at a time (starting from head). For each original node `curr`, its copy is `curr->next`. The copy's random pointer should point to the copy of the original's random, which is `curr->random->next` if `curr->random` is not null; otherwise it is null.  
// **Pass 3 (Separate lists and restore original):** Traverse again, decouple the two lists. For each original node `curr`, keep its `next` pointing to the original next (which is `curr->next->next`), and set the copy's `next` to the next copy (which is `curr->next->next->next` if it exists). At the end, return the head of the copy list.  
// **Edge cases:** Empty list returns nullptr. Single node with null random works because the copy’s random is set to null. Single node with random pointing to itself: in pass 2, `curr->random` is the same node, so `curr->random->next` is the copy node, so the copy's random points to itself correctly. Cycles in random pointers are handled because the interleaved structure guarantees that any node's copy follows it.  
// **Time complexity:** O(n) with three linear passes over the list. **Space complexity:** O(1) auxiliary (excluding the newly created nodes which are required for the copy). The original list is temporarily modified but fully restored by the end.
