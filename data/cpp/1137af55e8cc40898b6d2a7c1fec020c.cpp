/*
Write a C++ function `partitionList` that accepts a reference to the head pointer of a singly linked list and an integer `x`. The function must reorder the nodes in-place so that all nodes with data values strictly less than `x` appear before all nodes with data values greater than or equal to `x`. The relative order among the nodes within each group does not matter, and the function must not allocate new nodes or use extra containers; it should only change the `next` pointers. If the list is empty or contains only one node, the function should leave it unchanged. The function should handle duplicate values correctly, including multiple occurrences of `x`. You may assume each node's data is a valid integer. The function must preserve the original list's nodes and must not leak memory (though you only need to return the new head via the reference parameter).
*/

#include <cstddef>

struct Node {
    int data;
    Node* next;
    explicit Node(int d) : data(d), next(nullptr) {}
};

// Partition the list around value x: nodes with data < x come before data >= x.
// Modifies the list in-place; head is updated to the new front of the list.
void partitionList(Node*& head, int x) {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Node* tail = head;
    Node* curr = head;
    while (curr != nullptr) {
        Node* nextNode = curr->next;
        if (curr->data < x) {
            // Move current node to the front.
            curr->next = head;
            head = curr;
        } else {
            // Append current node to the back.
            tail->next = curr;
            tail = curr;
        }
        curr = nextNode;
    }
    tail->next = nullptr;
}

#include <cassert>

// Helper to build a list from an array and return the head.
Node* buildList(const int* arr, int size) {
    if (size == 0) return nullptr;
    Node* head = new Node(arr[0]);
    Node* curr = head;
    for (int i = 1; i < size; ++i) {
        curr->next = new Node(arr[i]);
        curr = curr->next;
    }
    return head;
}

// Helper to check that all nodes < x appear before nodes >= x.
bool isValidPartition(Node* head, int x) {
    bool seenGreaterOrEqual = false;
    while (head != nullptr) {
        if (head->data >= x) {
            seenGreaterOrEqual = true;
        } else if (seenGreaterOrEqual) {
            return false;
        }
        head = head->next;
    }
    return true;
}

// Helper to count occurrences of a value in the list.
int countValue(Node* head, int value) {
    int count = 0;
    while (head != nullptr) {
        if (head->data == value) ++count;
        head = head->next;
    }
    return count;
}

int main() {
    // Test 1: empty list
    Node* head1 = nullptr;
    partitionList(head1, 5);
    assert(head1 == nullptr);

    // Test 2: single node less than x
    int arr2[] = {3};
    Node* head2 = buildList(arr2, 1);
    partitionList(head2, 5);
    assert(head2->data == 3 && head2->next == nullptr);
    delete head2;

    // Test 3: single node greater than x
    int arr3[] = {8};
    Node* head3 = buildList(arr3, 1);
    partitionList(head3, 5);
    assert(head3->data == 8 && head3->next == nullptr);
    delete head3;

    // Test 4: classic example 3->5->8->5->10->2->1, x=5
    int arr4[] = {3, 5, 8, 5, 10, 2, 1};
    Node* head4 = buildList(arr4, 7);
    partitionList(head4, 5);
    assert(isValidPartition(head4, 5));
    assert(countValue(head4, 3) == 1 && countValue(head4, 2) == 1 && countValue(head4, 1) == 1);
    assert(countValue(head4, 5) == 2 && countValue(head4, 8) == 1 && countValue(head4, 10) == 1);
    // Clean up list4
    while (head4) { Node* temp = head4; head4 = head4->next; delete temp; }

    // Test 5: all values less than x
    int arr5[] = {1, 2, 3};
    Node* head5 = buildList(arr5, 3);
    partitionList(head5, 5);
    assert(isValidPartition(head5, 5));
    assert(head5->data == 1 && head5->next->next->data == 3); // order may vary, but all <5
    // Cleanup
    while (head5) { Node* temp = head5; head5 = head5->next; delete temp; }

    // Test 6: all values greater or equal to x
    int arr6[] = {5, 6, 9};
    Node* head6 = buildList(arr6, 3);
    partitionList(head6, 5);
    assert(isValidPartition(head6, 5));
    assert(countValue(head6, 5) == 1 && countValue(head6, 6) == 1 && countValue(head6, 9) == 1);
    while (head6) { Node* temp = head6; head6 = head6->next; delete temp; }

    // Test 7: duplicates of x and multiple less values
    int arr7[] = {5, 2, 5, 1, 5};
    Node* head7 = buildList(arr7, 5);
    partitionList(head7, 5);
    assert(isValidPartition(head7, 5));
    assert(countValue(head7, 1) == 1 && countValue(head7, 2) == 1 && countValue(head7, 5) == 3);
    while (head7) { Node* temp = head7; head7 = head7->next; delete temp; }

    // Test 8: already partitioned list, check no crash and validity
    int arr8[] = {1, 2, 7, 8, 9};
    Node* head8 = buildList(arr8, 5);
    partitionList(head8, 5);
    assert(isValidPartition(head8, 5));
    while (head8) { Node* temp = head8; head8 = head8->next; delete temp; }

    return 0;
}

// The core idea is to traverse the list once and move each node to either the front or the back of a growing partitioned list. Maintain a pointer `tail` that always points to the last node of the current list. Start with `tail` set to the original head and `curr` as the head. For each node, store its next node in a temporary pointer before modifying links. If the node's data is less than `x`, insert the node at the head by setting its `next` to the current head and updating the head to this node. If the node's data is greater than or equal to `x`, append it to the tail by setting `tail->next` to the node, then moving `tail` to the node. After processing all nodes, set `tail->next` to `nullptr` to terminate the list. Edge cases: an empty list (`head == nullptr`) or a single node requires no changes; pass the check at the start. When the original head itself is less than `x`, moving it to the front does not change its position but requires careful pointer handling to avoid cycles. The algorithm runs in O(n) time, visiting each node once, and uses O(1) auxiliary space, only constant pointer variables. This approach ensures stability is not required, as the problem does not demand maintaining relative order within groups.
