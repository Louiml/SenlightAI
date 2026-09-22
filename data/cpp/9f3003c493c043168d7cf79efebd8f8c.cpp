Write a standalone C++ function that removes the node at a given 1-based index from a doubly linked list and returns the head of the modified list. The list is guaranteed to have at least three nodes, and the index will be a valid deletion position (i.e., not the head or the tail, and within the current list length). The function must handle the general case where the node to delete is anywhere strictly inside the list (after the first and before the last node). The function should not print anything; it only modifies the list structure and returns the head pointer. You must implement the function from scratch, including a `Node` structure with `val`, `next`, and `prev` pointers. The function signature should be: `Node* delete_at_any_position(Node* head, int idx)`. Assume all nodes are dynamically allocated, and properly free the memory of the removed node.

#include <cassert>

// Test helper to build a list from an initializer list.
Node* build_list(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* new_node = new Node(v);
        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
    }
    return head;
}

// Test helper to convert list to vector for comparison.
std::vector<int> to_vector(Node* head) {
    std::vector<int> result;
    Node* temp = head;
    while (temp != nullptr) {
        result.push_back(temp->val);
        temp = temp->next;
    }
    return result;
}

// Test helper to delete all nodes (cleanup).
void free_list(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Delete second node from list of 3 (index 2).
    Node* head1 = build_list({10, 20, 30});
    head1 = delete_at_any_position(head1, 2);
    assert((to_vector(head1) == std::vector<int>{10, 30}));
    free_list(head1);

    // Test 2: Delete middle node from list of 5 (index 3).
    Node* head2 = build_list({1, 2, 3, 4, 5});
    head2 = delete_at_any_position(head2, 3);
    assert((to_vector(head2) == std::vector<int>{1, 2, 4, 5}));
    free_list(head2);

    // Test 3: Delete second-to-last node from list of 4 (index 3).
    Node* head3 = build_list({5, 15, 25, 35});
    head3 = delete_at_any_position(head3, 3);
    assert((to_vector(head3) == std::vector<int>{5, 15, 35}));
    free_list(head3);

    // Test 4: Delete second node from list of many (index 2) after multiple deletes.
    Node* head4 = build_list({0, 100, 200, 300, 400});
    head4 = delete_at_any_position(head4, 2); // removes 100, list becomes {0,200,300,400}
    head4 = delete_at_any_position(head4, 2); // removes 200, list becomes {0,300,400}
    assert((to_vector(head4) == std::vector<int>{0, 300, 400}));
    free_list(head4);

    // Test 5: Ensure head is unchanged when deleting interior node.
    Node* head5 = build_list({7, 8, 9, 10});
    Node* original_head5 = head5;
    Node* returned_head5 = delete_at_any_position(head5, 2);
    assert(returned_head5 == original_head5);
    assert((to_vector(returned_head5) == std::vector<int>{7, 9, 10}));
    free_list(head5);

    return 0;
}

#include <cstddef>

// Node structure for a doubly linked list.
struct Node {
    int val;
    Node* next;
    Node* prev;
    explicit Node(int value) : val(value), next(nullptr), prev(nullptr) {}
};

// Delete the node at the given 1-based index (must be an interior node) and return the head.
Node* delete_at_any_position(Node* head, int idx) {
    if (head == nullptr || idx <= 0) return head; // defensive, but constraints guarantee validity

    Node* current = head;
    // Move to the node just before the deletion target.
    for (int i = 1; i < idx; ++i) {
        if (current == nullptr || current->next == nullptr) return head; // invalid index fallback
        current = current->next;
    }

    // current now points to the node before the target.
    Node* target = current->next;
    if (target == nullptr) return head; // target does not exist

    // Relink: bypass the target node.
    current->next = target->next;
    if (target->next != nullptr) {
        target->next->prev = current;
    }

    delete target;
    return head;
}

// The solution traverses the list from the head, moving `idx-1` steps forward to reach the node immediately before the target node. Because the index is guaranteed to be a valid interior position (between 1 and length-2, with length ≥3), the previous node exists, the target node exists, and the next node after the target exists. To delete, we connect the previous node's `next` pointer to the target's `next`, and connect that next node's `prev` pointer back to the previous node. Then we free the target node's memory. The head does not change because we never delete the first node, so we return the original head. Edge cases: if the index is 1 (second node), we still have `head` non-null and `temp->next` non-null; if the index is the second-to-last node, the next node is the tail and still has a `prev` pointer to update. No special-case for empty or single-node lists is needed because the constraints guarantee validity. Time complexity is O(n) in the worst case to traverse to the deletion point; space complexity is O(1) for the traversal pointer.
