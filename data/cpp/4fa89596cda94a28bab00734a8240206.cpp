Write a C++ function that takes a reference to the head of a singly linked list of integers and an integer position `pos` (0-indexed). The function should delete the node at the given position if it exists, and return `true` if deletion was successful, or `false` if the position is out of range (including an empty list). After deletion, the function must update the head pointer correctly if the deleted node was the first node. The function should not leak memory (delete the removed node properly) and should handle positions of 0 or greater. The linked list nodes are defined by the `Node` class provided in the snippet, with `int val` and `Node* next`. Your function should be named `delete_at_position` and should have the signature `bool delete_at_position(Node*& head, int pos)`.
// The solution walks through the linked list to find the node just before the target position. For an empty list or a negative position, return `false` immediately. If `pos == 0`, the head node is to be removed: save a pointer to the old head, move `head` to `head->next`, delete the old head, and return `true`. For `pos > 0`, traverse with a temporary pointer starting at `head`, moving forward `pos-1` times. If during traversal the temporary pointer or its `next` becomes `nullptr`, the position is out of range, so return `false`. Otherwise, the target node is `tmp->next`; link `tmp->next` to `tmp->next->next`, delete the target node, and return `true`. This handles the single-node list for position 0 correctly (head becomes `nullptr`). Edge cases include deleting the only node, deleting the last node, and invalid positions beyond length. The algorithm runs in `O(pos)` time, which is `O(n)` in the worst case, and uses `O(1)` auxiliary space.
#include <cstddef>

// Definition of Node (provided for completeness)
class Node {
public:
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};

// Delete the node at the given zero-based position.
// Returns true on success, false if the list is empty or position is invalid.
bool delete_at_position(Node*& head, int pos) {
    if (head == nullptr || pos < 0) {
        return false;
    }
    if (pos == 0) {
        Node* old_head = head;
        head = head->next;
        delete old_head;
        return true;
    }
    Node* tmp = head;
    // Move to the node just before the target position.
    for (int i = 0; i < pos - 1; ++i) {
        if (tmp == nullptr || tmp->next == nullptr) {
            return false; // Position out of range.
        }
        tmp = tmp->next;
    }
    if (tmp == nullptr || tmp->next == nullptr) {
        return false; // Target node does not exist.
    }
    Node* target = tmp->next;
    tmp->next = target->next;
    delete target;
    return true;
}
#include <cassert>

int main() {
    // Helper to build a list from an array for testing.
    Node* build_list(const int* arr, int size) {
        if (size == 0) return nullptr;
        Node* head = new Node(arr[0]);
        Node* cur = head;
        for (int i = 1; i < size; ++i) {
            cur->next = new Node(arr[i]);
            cur = cur->next;
        }
        return head;
    }

    // Helper to convert list to vector for comparison.
    std::vector<int> list_to_vector(Node* head) {
        std::vector<int> v;
        while (head) {
            v.push_back(head->val);
            head = head->next;
        }
        return v;
    }

    // Test 1: Delete head from multi-node list.
    int arr1[] = {1,2,3};
    Node* head1 = build_list(arr1, 3);
    assert(delete_at_position(head1, 0) == true);
    assert((list_to_vector(head1) == std::vector<int>{2,3}));

    // Test 2: Delete middle position.
    int arr2[] = {5,6,7,8};
    Node* head2 = build_list(arr2, 4);
    assert(delete_at_position(head2, 2) == true);
    assert((list_to_vector(head2) == std::vector<int>{5,6,8}));

    // Test 3: Delete last node.
    int arr3[] = {9,10};
    Node* head3 = build_list(arr3, 2);
    assert(delete_at_position(head3, 1) == true);
    assert((list_to_vector(head3) == std::vector<int>{9}));

    // Test 4: Delete only node becomes empty.
    int arr4[] = {42};
    Node* head4 = build_list(arr4, 1);
    assert(delete_at_position(head4, 0) == true);
    assert(head4 == nullptr);

    // Test 5: Invalid negative position.
    int arr5[] = {1};
    Node* head5 = build_list(arr5, 1);
    assert(delete_at_position(head5, -1) == false);
    assert((list_to_vector(head5) == std::vector<int>{1}));

    // Test 6: Position out of range (equal to length).
    int arr6[] = {1,2};
    Node* head6 = build_list(arr6, 2);
    assert(delete_at_position(head6, 2) == false);
    assert((list_to_vector(head6) == std::vector<int>{1,2}));

    // Test 7: Position out of range (greater than length).
    int arr7[] = {1,2,3};
    Node* head7 = build_list(arr7, 3);
    assert(delete_at_position(head7, 5) == false);
    assert((list_to_vector(head7) == std::vector<int>{1,2,3}));

    // Test 8: Empty list.
    Node* head8 = nullptr;
    assert(delete_at_position(head8, 0) == false);
    assert(head8 == nullptr);

    // Test 9: Delete second node in a two-node list.
    int arr9[] = {7,8};
    Node* head9 = build_list(arr9, 2);
    assert(delete_at_position(head9, 1) == true);
    assert((list_to_vector(head9) == std::vector<int>{7}));

    // Test 10: Delete first node from a list where next nodes exist.
    int arr10[] = {10,20,30};
    Node* head10 = build_list(arr10, 3);
    assert(delete_at_position(head10, 0) == true);
    assert((list_to_vector(head10) == std::vector<int>{20,30}));

    // Clean up remaining lists manually (for complete memory safety).
    // Not strictly needed for the assert checks, but done to avoid leaks.
    auto cleanup = [](Node*& h) {
        while (h) {
            Node* nxt = h->next;
            delete h;
            h = nxt;
        }
    };
    cleanup(head1);
    cleanup(head2);
    cleanup(head3);
    cleanup(head4);
    cleanup(head5);
    cleanup(head6);
    cleanup(head7);
    cleanup(head8);
    cleanup(head9);
    cleanup(head10);

    return 0;
}
