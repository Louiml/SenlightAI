Write a C++ function `Node* filterList(Node* head)` that takes the head of a singly linked list constructed from user input (where each node contains an integer and input terminates when -1 is entered, and the terminator itself is not stored) and returns a new linked list containing only the nodes whose data values are strictly greater than all preceding values in the original list (i.e., a running maximum filter). For example, given the list 3 -> 1 -> 4 -> 2 -> 5 -> 6, the returned list should be 3 -> 4 -> 5 -> 6. The original list must remain unchanged; the new list must be built from newly allocated nodes. The input list may be empty (if the first value is -1) in which case return `nullptr`. Duplicates are not considered strictly greater, so equal values are filtered out. The function should preserve the relative order of the selected nodes as they appear in the original list.

#include <cassert>
#include <cstddef>

// Node definition and filterList declaration assumed to be available before this test.
int main() {
    // Helper to build a list from an array (terminated by -1 sentinel in input, but here array).
    Node* buildFromArray(const int* arr, int size) {
        if (size == 0) return nullptr;
        Node* head = new Node(arr[0]);
        Node* tail = head;
        for (int i = 1; i < size; ++i) {
            tail->next = new Node(arr[i]);
            tail = tail->next;
        }
        return head;
    }

    // Helper to convert list to vector for easy comparison.
    std::vector<int> toVector(const Node* head) {
        std::vector<int> v;
        while (head) { v.push_back(head->data); head = head->next; }
        return v;
    }

    // Test 1: Normal increasing pattern
    int arr1[] = {3,1,4,2,5,6};
    Node* list1 = buildFromArray(arr1, 6);
    Node* result1 = filterList(list1);
    std::vector<int> expected1 = {3,4,5,6};
    assert(toVector(result1) == expected1);

    // Test 2: All increasing
    int arr2[] = {1,2,3,4};
    Node* list2 = buildFromArray(arr2, 4);
    Node* result2 = filterList(list2);
    std::vector<int> expected2 = {1,2,3,4};
    assert(toVector(result2) == expected2);

    // Test 3: All decreasing
    int arr3[] = {5,4,3,2,1};
    Node* list3 = buildFromArray(arr3, 5);
    Node* result3 = filterList(list3);
    std::vector<int> expected3 = {5};
    assert(toVector(result3) == expected3);

    // Test 4: Empty list
    Node* result4 = filterList(nullptr);
    assert(result4 == nullptr);

    // Test 5: Single element
    int arr5[] = {42};
    Node* list5 = buildFromArray(arr5, 1);
    Node* result5 = filterList(list5);
    std::vector<int> expected5 = {42};
    assert(toVector(result5) == expected5);

    // Test 6: Duplicates not strictly greater
    int arr6[] = {2,2,2,3};
    Node* list6 = buildFromArray(arr6, 4);
    Node* result6 = filterList(list6);
    std::vector<int> expected6 = {2,3};
    assert(toVector(result6) == expected6);

    // Test 7: Negative numbers and zeros
    int arr7[] = {-5, -1, -10, 0, 3, -1, 4};
    Node* list7 = buildFromArray(arr7, 7);
    Node* result7 = filterList(list7);
    std::vector<int> expected7 = {-5, -1, 0, 3, 4};
    assert(toVector(result7) == expected7);

    // Test 8: INT_MIN as first element (should be filtered out because not > INT_MIN)
    int arr8[] = {INT_MIN, 0, 1};
    Node* list8 = buildFromArray(arr8, 3);
    Node* result8 = filterList(list8);
    std::vector<int> expected8 = {0, 1};
    assert(toVector(result8) == expected8);

    // Clean up memory (not strictly required for assert tests but good practice)
    auto deleteList = [](Node* head) {
        while (head) { Node* temp = head; head = head->next; delete temp; }
    };
    deleteList(list1); deleteList(result1);
    deleteList(list2); deleteList(result2);
    deleteList(list3); deleteList(result3);
    deleteList(list5); deleteList(result5);
    deleteList(list6); deleteList(result6);
    deleteList(list7); deleteList(result7);
    deleteList(list8); deleteList(result8);

    return 0;
}

#include <climits>
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int n) : data(n), next(nullptr) {}
};

// Return a new linked list containing only nodes whose data is strictly
// greater than all previous data values in the original list.
// The original list is not modified.
Node* filterList(const Node* head) {
    if (head == nullptr) {
        return nullptr;
    }

    int currentMax = INT_MIN;
    Node* resultHead = nullptr;
    Node* resultTail = nullptr;

    const Node* current = head;
    while (current != nullptr) {
        if (current->data > currentMax) {
            Node* newNode = new Node(current->data);
            if (resultHead == nullptr) {
                resultHead = newNode;
                resultTail = newNode;
            } else {
                resultTail->next = newNode;
                resultTail = newNode;
            }
            currentMax = current->data;
        }
        current = current->next;
    }

    return resultHead;
}

// The solution traverses the original linked list once, maintaining a variable `currentMax` that tracks the maximum value seen so far. For each node, if its data is strictly greater than `currentMax`, we create a new node with that data and append it to the tail of the result list; then we update `currentMax` to this new value. If the data is not greater, we skip it. Edge cases: (1) empty list: return `nullptr` immediately. (2) First node: since no prior values, `currentMax` should be initialized to negative infinity (e.g., `INT_MIN` from `<climits>`), so the first node always qualifies (as long as it isn't INT_MIN, but if it is, it won't be picked because it's not strictly greater—this is acceptable and consistent). (3) The original list is never modified; we only read from it. (4) The result list is built by appending to the tail, so we keep both `resultHead` and `resultTail` pointers. Time complexity is O(n) where n is the number of nodes in the original list, as we visit each node once. Space complexity is O(m) where m is the number of selected nodes (new allocations), plus O(1) auxiliary for pointers.
