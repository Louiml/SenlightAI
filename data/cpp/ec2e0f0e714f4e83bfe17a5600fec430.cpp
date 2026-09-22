// Implement a C++ function `doublyLinkedListOperations` that takes a vector of integers representing values to insert into a doubly linked list, along with a position and operation type (0 for insert at position, 1 for delete at position), and returns a vector of integers representing the final doubly linked list contents after performing the specified operation. The insertion should follow the logic demonstrated in the snippet: if position is 0, insert at head; if position is greater than or equal to current length, insert at tail; otherwise insert at the given 0-based position. Deletion should remove the node at the given 1-based position, with positions 1 and last handled specially, and for other positions, unlink and delete the middle node. The function should handle an empty list appropriately (insert creates first node, delete on empty does nothing and returns empty). The input vector may be empty, and the position for insertion may be any non-negative integer, while deletion position starts at 1.

The solution builds a custom doubly linked list using a `node` structure with `data`, `prev`, and `next` pointers. For insertion, we check if the list is empty (head and tail null), then create a new node and set both head and tail. If position is 0, we insert at head by linking the new node before the current head. If position is greater than or equal to the current length, we insert at tail by linking after tail. Otherwise, we traverse to find the node before the target position, then link the new node between that and its next node. For deletion, we handle three cases: deleting the head (position 1), deleting the tail (position equals length), and deleting a middle node. In each case, we properly update the `prev` and `next` pointers of neighbors and delete the target node to free memory. Edge cases include empty list (insert creates first node; delete does nothing), position 0 insertion on non-empty list, deletion at the only node (head equals tail), and invalid positions (for insertion we clamp to head/tail based on bounds; for deletion if position out of range, we do nothing). Time complexity for each insertion or deletion is O(n) in the worst case due to traversal to find position, and O(1) auxiliary space per operation. Space complexity for the list itself is O(n) where n is the number of nodes.

#include <vector>
#include <stdexcept>

struct node {
    int data;
    node* prev;
    node* next;
    node(int val) : data(val), prev(nullptr), next(nullptr) {}
    ~node() = default;
};

// Helper functions for doubly linked list operations (internal use)
static int getLength(node* head) {
    int len = 0;
    node* temp = head;
    while (temp) {
        len++;
        temp = temp->next;
    }
    return len;
}

static void insertAtHead(node*& head, node*& tail, int data) {
    node* n = new node(data);
    if (head == nullptr) {
        head = tail = n;
        return;
    }
    n->next = head;
    head->prev = n;
    head = n;
}

static void insertAtTail(node*& head, node*& tail, int data) {
    node* n = new node(data);
    if (head == nullptr) {
        head = tail = n;
        return;
    }
    n->prev = tail;
    tail->next = n;
    tail = n;
}

static void insertAtPosition(node*& head, node*& tail, int pos, int data) {
    if (pos <= 0) {
        insertAtHead(head, tail, data);
        return;
    }
    int len = getLength(head);
    if (pos >= len) {
        insertAtTail(head, tail, data);
        return;
    }
    if (head == nullptr) {
        node* n = new node(data);
        head = tail = n;
        return;
    }
    node* prev = head;
    int i = 0;
    while (i < pos - 1) {
        prev = prev->next;
        i++;
    }
    node* n = new node(data);
    node* next = prev->next;
    n->next = next;
    next->prev = n;
    prev->next = n;
    n->prev = prev;
}

static void deleteAtPosition(node*& head, node*& tail, int pos) {
    if (head == nullptr) return;
    int len = getLength(head);
    if (pos < 1 || pos > len) return;
    if (pos == 1) {
        node* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
    } else if (pos == len) {
        node* temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
    } else {
        node* prev = head;
        int i = 1;
        while (i < pos - 1) {
            prev = prev->next;
            i++;
        }
        node* current = prev->next;
        node* next = current->next;
        prev->next = next;
        next->prev = prev;
        delete current;
    }
}

static void freeList(node* head) {
    node* temp = head;
    while (temp) {
        node* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }
}

// Main task function: takes values to insert, operation type (0=insert,1=delete),
// position, and returns final list contents as vector.
std::vector<int> doublyLinkedListOperations(
    const std::vector<int>& initialValues,
    int operation,
    int position,
    int valueToInsert = 0) {
    
    node* head = nullptr;
    node* tail = nullptr;
    
    // Insert initial values at tail to preserve order
    for (int v : initialValues) {
        insertAtTail(head, tail, v);
    }
    
    if (operation == 0) {
        insertAtPosition(head, tail, position, valueToInsert);
    } else if (operation == 1) {
        deleteAtPosition(head, tail, position);
    }
    
    std::vector<int> result;
    node* temp = head;
    while (temp) {
        result.push_back(temp->data);
        temp = temp->next;
    }
    
    freeList(head);
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test insert at head (position 0)
    assert(doublyLinkedListOperations({}, 0, 0, 5) == std::vector<int>({5}));
    
    // Test insert at tail (position >= length)
    assert(doublyLinkedListOperations({1,2,3}, 0, 5, 9) == std::vector<int>({1,2,3,9}));
    
    // Test insert at middle
    assert(doublyLinkedListOperations({1,2,3}, 0, 1, 99) == std::vector<int>({1,99,2,3}));
    
    // Test delete head
    assert(doublyLinkedListOperations({1,2,3}, 1, 1) == std::vector<int>({2,3}));
    
    // Test delete tail
    assert(doublyLinkedListOperations({1,2,3}, 1, 3) == std::vector<int>({1,2}));
    
    // Test delete middle
    assert(doublyLinkedListOperations({1,2,3,4}, 1, 2) == std::vector<int>({1,3,4}));
    
    // Test delete only node
    assert(doublyLinkedListOperations({7}, 1, 1) == std::vector<int>({}));
    
    // Test delete on empty (should stay empty)
    assert(doublyLinkedListOperations({}, 1, 1) == std::vector<int>({}));
    
    // Test insert position 0 on non-empty
    assert(doublyLinkedListOperations({2,3}, 0, 0, 1) == std::vector<int>({1,2,3}));
    
    // Test insert at exact length (tail)
    assert(doublyLinkedListOperations({1,2}, 0, 2, 3) == std::vector<int>({1,2,3}));
    
    return 0;
}
