Given a singly linked list where each node stores a single decimal digit (0-9) and the list represents a non-negative integer with the head as the most significant digit, write a C++ function named `removeLeadingZeros` that takes a pointer to the head node of such a list and returns a pointer to the head of the modified list with all leading zeros removed. The function must handle the special cases of an empty list (return nullptr) and a list consisting entirely of zeros (return a new single node with data 0, or the existing node if only zeros are present). The function should preserve the order and values of the remaining digits and properly deallocate any removed leading zero nodes to avoid memory leaks. The original list must be modified in place.

The main algorithm involves traversing the list from the head, skipping over nodes with data equal to 0 until either a non-zero digit is found or the end of the list is reached. If a non-zero node is encountered, that node becomes the new head, and any previously skipped zero nodes must be deleted to free memory. If the entire list consists of zeros, we delete all nodes except one (or if the list is empty, we return nullptr), and set that remaining node's data to 0 and its next to nullptr so we return a single-zero node. Edge cases include an empty list (return nullptr immediately), a list starting with non-zero (no changes needed), a list with a single zero, and a list with multiple zeros followed by non-zero digits. Time complexity is O(n) where n is the number of nodes, as we traverse each node at most once. Space complexity is O(1) auxiliary, using only a few pointer variables; we do not allocate new nodes except possibly creating one new node when the list is empty or all zeros, but that is constant.

#include <cstddef> // for nullptr

// Node structure for singly linked list storing a digit
struct Node {
    int data;       // digit 0-9
    Node* next;     // pointer to next node
    
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Remove all leading zeros from a singly linked list representing a number.
// Returns pointer to the new head. Modifies the list in place.
// If the list is empty, returns nullptr.
// If all digits are zeros, returns a single node with data=0.
Node* removeLeadingZeros(Node* head) {
    // Handle empty list
    if (head == nullptr) {
        return nullptr;
    }
    
    // Find the first non-zero node, keeping track of previous node
    Node* prev = nullptr;
    Node* current = head;
    
    // Skip all leading zero nodes
    while (current != nullptr && current->data == 0) {
        prev = current;
        current = current->next;
    }
    
    // Case 1: All nodes are zeros (current == nullptr)
    if (current == nullptr) {
        // Delete all nodes except one, then set that one to 0
        // If only one node, just keep it
        if (head->next != nullptr) {
            Node* temp = head->next;
            while (temp->next != nullptr) {
                Node* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
            // Delete the second-to-last node? Actually simpler: delete all after head
            Node* toDelete = head->next;
            while (toDelete != nullptr) {
                Node* nextNode = toDelete->next;
                delete toDelete;
                toDelete = nextNode;
            }
            head->next = nullptr;
        }
        head->data = 0; // ensure it's zero
        return head;
    }
    
    // Case 2: Found a non-zero node at 'current'
    // Delete all skipped zero nodes (from head up to prev)
    if (prev != nullptr) {
        Node* temp = head;
        while (temp != current) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        // Now 'current' is the new head
        return current;
    }
    
    // Case 3: head was already non-zero
    return head;
}

#include <cassert>

// Helper to create a list from an array
Node* createList(int arr[], int size) {
    if (size == 0) return nullptr;
    Node* head = new Node(arr[0]);
    Node* temp = head;
    for (int i = 1; i < size; ++i) {
        temp->next = new Node(arr[i]);
        temp = temp->next;
    }
    return head;
}

// Helper to convert list to std::string for comparison
std::string listToString(Node* head) {
    std::string result;
    Node* temp = head;
    while (temp != nullptr) {
        result += ('0' + temp->data);
        temp = temp->next;
    }
    return result;
}

// Helper to delete list (for cleanup)
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Leading zeros removed
    int arr1[] = {0, 0, 1, 2, 3};
    Node* list1 = createList(arr1, 5);
    Node* result1 = removeLeadingZeros(list1);
    assert(listToString(result1) == "123");
    deleteList(result1);

    // Test 2: No leading zeros, unchanged
    int arr2[] = {5, 0, 7};
    Node* list2 = createList(arr2, 3);
    Node* result2 = removeLeadingZeros(list2);
    assert(listToString(result2) == "507");
    deleteList(result2);

    // Test 3: All zeros -> single zero
    int arr3[] = {0, 0, 0};
    Node* list3 = createList(arr3, 3);
    Node* result3 = removeLeadingZeros(list3);
    assert(listToString(result3) == "0");
    deleteList(result3);

    // Test 4: Single zero -> single zero
    int arr4[] = {0};
    Node* list4 = createList(arr4, 1);
    Node* result4 = removeLeadingZeros(list4);
    assert(listToString(result4) == "0");
    deleteList(result4);

    // Test 5: Empty list -> nullptr
    Node* list5 = nullptr;
    Node* result5 = removeLeadingZeros(list5);
    assert(result5 == nullptr);

    // Test 6: Single non-zero
    int arr6[] = {9};
    Node* list6 = createList(arr6, 1);
    Node* result6 = removeLeadingZeros(list6);
    assert(listToString(result6) == "9");
    deleteList(result6);

    // Test 7: Zeros then all zeros? already covered, but edge: 0,0,0,1
    int arr7[] = {0, 0, 0, 1};
    Node* list7 = createList(arr7, 4);
    Node* result7 = removeLeadingZeros(list7);
    assert(listToString(result7) == "1");
    deleteList(result7);

    return 0;
}
