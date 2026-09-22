// Write a C++ function named `reverseDoublyLinkedList` that takes the head pointer of a doubly linked list (where each node contains an integer value, a `next` pointer, and a `prev` pointer) and reverses the list in place. The function should return the new head pointer of the reversed list. The input list may be empty (head is `nullptr`), or contain one or more nodes. Do not allocate any new nodes; only swap the `next` and `prev` pointers of existing nodes. The function must be `const`-correct in the sense that it does not modify any data other than the pointers inside the nodes.
#include <cassert>

// Helper to create a doubly linked list from an array.
Node* createList(int arr[], int size) {
    if (size == 0) return nullptr;
    Node* head = new Node{arr[0], nullptr, nullptr};
    Node* tail = head;
    for (int i = 1; i < size; ++i) {
        Node* newNode = new Node{arr[i], nullptr, tail};
        tail->next = newNode;
        tail = newNode;
    }
    return head;
}

// Helper to convert list to vector for comparison.
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete list and free memory.
void deleteList(Node* head) {
    while (head) {
        Node* temp = head->next;
        delete head;
        head = temp;
    }
}

int main() {
    // Test 1: Empty list
    Node* empty = nullptr;
    assert(reverseDoublyLinkedList(empty) == nullptr);

    // Test 2: Single node list
    int singleArr[] = {42};
    Node* single = createList(singleArr, 1);
    Node* revSingle = reverseDoublyLinkedList(single);
    assert(listToVector(revSingle) == std::vector<int>({42}));
    deleteList(revSingle);

    // Test 3: Multiple nodes
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);
    Node* rev1 = reverseDoublyLinkedList(list1);
    assert(listToVector(rev1) == std::vector<int>({5, 4, 3, 2, 1}));
    deleteList(rev1);

    // Test 4: Two nodes
    int arr2[] = {10, 20};
    Node* list2 = createList(arr2, 2);
    Node* rev2 = reverseDoublyLinkedList(list2);
    assert(listToVector(rev2) == std::vector<int>({20, 10}));
    deleteList(rev2);

    // Test 5: Already reversed list (original ascending) – reverse it back
    int arr3[] = {9, 8, 7};
    Node* list3 = createList(arr3, 3);
    Node* rev3 = reverseDoublyLinkedList(list3);
    assert(listToVector(rev3) == std::vector<int>({7, 8, 9}));
    deleteList(rev3);

    // Test 6: Reverse twice should return original order
    int arr4[] = {100, 200, 300};
    Node* list4 = createList(arr4, 3);
    Node* rev4a = reverseDoublyLinkedList(list4);
    Node* rev4b = reverseDoublyLinkedList(rev4a);
    assert(listToVector(rev4b) == std::vector<int>({100, 200, 300}));
    deleteList(rev4b);

    return 0;
}
#include <cstddef>

// Node structure for a doubly linked list.
struct Node {
    int val;
    Node* next;
    Node* prev;
};

// Reverses a doubly linked list in place.
// Returns the new head of the reversed list.
// Assumes the list is well-formed (no cycles).
Node* reverseDoublyLinkedList(Node* head) {
    if (head == nullptr) {
        return nullptr;
    }
    
    Node* current = head;
    Node* temp = nullptr;
    
    // Traverse the list, swapping next and prev for each node.
    while (current != nullptr) {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        
        // Move to the next node in the original order.
        // After swapping, the original next is now stored in current->prev.
        current = current->prev;
    }
    
    // After the loop, temp points to the last processed node (originally last in list),
    // and its prev (which is the original next) is null. So temp is the new head.
    return temp;
}
// The core idea is to traverse the list once, and for each node, swap its `next` and `prev` pointers. After swapping, we move to the node that was originally the `next` node (now accessible via the updated `prev` pointer). At the end of the traversal, the original last node (which now has `next == nullptr` after swap) becomes the new head. We must handle the edge case of an empty list (return `nullptr`) and a single-node list (swapping pointers leaves it unchanged, and we must correctly return the same node). The main algorithm is iterative with constant extra space. Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) (only a few temporary pointers). A subtle point: after swapping `ptr->next` and `ptr->prev`, the next node to process is `ptr->prev` (which originally was `ptr->next`). We must ensure we do not dereference a null pointer when advancing.
