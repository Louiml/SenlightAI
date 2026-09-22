Write a C++ function `removeBefore` that takes a singly linked list (represented by a head pointer passed by reference) and an integer key, and deletes the node immediately preceding the first occurrence of a node whose data equals `key`. If no such predecessor exists (i.e., the list is empty, the key is at the head, or the key is not found with a preceding node), the function should do nothing and leave the list unchanged. The function must handle edge cases such as a single-node list, a key found at the second node (deleting the head), and keys not present. The function should not leak memory, and should preserve the relative order of the remaining nodes. The linked list node structure with an integer `data` and a `next` pointer is provided; you only need to implement the function.

// The solution requires careful traversal of the singly linked list while maintaining a pointer to the node before the predecessor of the target key. The main idea is to traverse with three conceptual positions: the current node being inspected, the node before it (to be deleted if the current node’s next is the key), and the node before that (to relink). A simpler two-pointer approach: use a `prev` pointer that lags two steps behind the current traversal node. Start from the head; if the list has fewer than two nodes, there is no predecessor to delete. If the second node’s data equals the key, delete the head. Otherwise, iterate with `prev` pointing two nodes behind the current node; when the current node’s `next` has data equal to `key`, then `prev->next` is the node to delete. Relink `prev->next` to the node after the deleted one, delete the node, and break. Edge cases: empty list (do nothing), key at head (no predecessor so nothing), key not found (nothing), key at second node (delete head), and key at a later position (use two-step lag). Time complexity is O(n) in the worst case, where n is the list length, and auxiliary space is O(1) apart from the deleted node.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Delete the node immediately preceding the first node with data equal to key.
// If no such predecessor exists (empty list, key at head, or key not found with predecessor),
// the list remains unchanged.
void removeBefore(Node*& head, int key) {
    if (head == nullptr || head->next == nullptr) {
        return; // No node before key possible
    }
    
    // Special case: key is at the second node, delete the head
    if (head->next->data == key) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
        return;
    }
    
    // General case: traverse with a pointer two nodes behind the current position
    Node* prev = head;          // Node before the one to delete (two behind current)
    Node* current = head->next; // Node whose next might be the key
    
    while (current->next != nullptr) {
        if (current->next->data == key) {
            // Delete the node after prev (which is current itself)
            Node* toDelete = current;
            prev->next = current->next;
            delete toDelete;
            return;
        }
        prev = prev->next;       // Move prev one step
        current = current->next; // Move current one step
    }
    // If we exit loop, key not found with a predecessor
}

#include <cassert>

// Helper to create a list from array
Node* createList(int arr[], int n) {
    if (n == 0) return nullptr;
    Node* head = new Node(arr[0]);
    Node* temp = head;
    for (int i = 1; i < n; ++i) {
        temp->next = new Node(arr[i]);
        temp = temp->next;
    }
    return head;
}

// Helper to compare list with expected array
bool listMatches(Node* head, int expected[], int n) {
    Node* temp = head;
    for (int i = 0; i < n; ++i) {
        if (temp == nullptr || temp->data != expected[i]) return false;
        temp = temp->next;
    }
    return temp == nullptr;
}

int main() {
    // Test 1: Delete middle predecessor
    int arr1[] = {1, 2, 3, 4};
    Node* head1 = createList(arr1, 4);
    removeBefore(head1, 3); // delete node with 2
    int exp1[] = {1, 3, 4};
    assert(listMatches(head1, exp1, 3));
    
    // Test 2: Delete head when key is second
    int arr2[] = {10, 20, 30};
    Node* head2 = createList(arr2, 3);
    removeBefore(head2, 20); // delete head (10)
    int exp2[] = {20, 30};
    assert(listMatches(head2, exp2, 2));
    
    // Test 3: Key at head, nothing deleted
    int arr3[] = {5, 6, 7};
    Node* head3 = createList(arr3, 3);
    removeBefore(head3, 5); // nothing
    int exp3[] = {5, 6, 7};
    assert(listMatches(head3, exp3, 3));
    
    // Test 4: Key not found
    int arr4[] = {1, 2, 3};
    Node* head4 = createList(arr4, 3);
    removeBefore(head4, 99); // nothing
    int exp4[] = {1, 2, 3};
    assert(listMatches(head4, exp4, 3));
    
    // Test 5: Empty list
    Node* head5 = nullptr;
    removeBefore(head5, 1);
    assert(head5 == nullptr);
    
    // Test 6: Single node list
    Node* head6 = new Node(1);
    removeBefore(head6, 1); // nothing
    assert(head6 != nullptr && head6->data == 1 && head6->next == nullptr);
    
    // Test 7: Key at last node, delete previous
    int arr7[] = {10, 20, 30, 40};
    Node* head7 = createList(arr7, 4);
    removeBefore(head7, 40); // delete node with 30
    int exp7[] = {10, 20, 40};
    assert(listMatches(head7, exp7, 3));
    
    // Test 8: Only two nodes and key second, delete first
    int arr8[] = {7, 8};
    Node* head8 = createList(arr8, 2);
    removeBefore(head8, 8); // delete head
    int exp8[] = {8};
    assert(listMatches(head8, exp8, 1));
    
    return 0;
}
