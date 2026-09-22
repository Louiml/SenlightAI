Write a C++ function `reverseEveryK` that takes a singly linked list (defined by a `node` structure with integer `data` and a `node* next` pointer) and a positive integer `k`. The function should reverse the nodes in groups of exactly `k`. If the total number of nodes is not a multiple of `k`, the final group (containing fewer than `k` nodes) should be left in its original order. The function must return the new head pointer of the modified list and should not allocate new nodes; it should only reassign pointers. The input list may be empty or may have any number of nodes (including less than `k`), and `k` is guaranteed to be greater than zero. You may assume the list contains no cycles and that the node structure already exists with a constructor that initializes `data` and sets `next` to `nullptr`.

// The core idea is to use a recursive approach that processes the list in chunks of size `k`. At each recursion step, we reverse the first `k` nodes (if they exist) using iterative pointer manipulation similar to reversing a full list: we maintain three pointers (`previous`, `current`, `next`) and invert links step by step. After reversing the first chunk, `head` (the original head of this chunk) becomes the tail of this reversed chunk, and its `next` pointer must point to the result of recursively processing the remaining list starting from the node that follows the chunk. If the remaining list has fewer than `k` nodes, the recursion will stop when `current` becomes `nullptr` before counting `k` nodes—in that case, the partial group is left as-is by simply returning the original head of that partial chunk (since the loop stops early and we don't modify `head->next`). Edge cases: (1) Empty list: the function checks if `head` is `nullptr` and returns `nullptr`. (2) `k` larger than list length: the loop reverses only the available nodes, but because `nextPtr` may become `nullptr` after processing all nodes, we must avoid calling recursion on `nullptr`; the condition `if (nextPtr != nullptr)` ensures we only recurse if more nodes exist. (3) `k` equal to 1: reversing a single node leaves the list unchanged; the loop runs once per node but recursion handles correctly. Time complexity is O(n) where n is the number of nodes, since each node is visited exactly once during reversal and the recursion processes each chunk once. Space complexity is O(n/k) due to recursion stack depth (worst case O(n) when k=1), but auxiliary pointer space is O(1) per call.

#include <cstddef>

struct node {
    int data;
    node* next;
    node(int val) : data(val), next(nullptr) {}
};

// Reverse nodes of the list in groups of size k.
// If the final group has fewer than k nodes, leave it unchanged.
// Returns the new head of the modified list.
node* reverseEveryK(node* head, int k) {
    if (head == nullptr || k <= 1) {
        return head;
    }

    node* previous = nullptr;
    node* current = head;
    node* nextNode = nullptr;
    int count = 0;

    // Reverse first k nodes of the current segment.
    while (current != nullptr && count < k) {
        nextNode = current->next;
        current->next = previous;
        previous = current;
        current = nextNode;
        ++count;
    }

    // If there are more nodes remaining, recursively process them.
    if (nextNode != nullptr) {
        // After reversal, head (original first node) is now the tail of this segment.
        head->next = reverseEveryK(nextNode, k);
    }

    // previous is the new head of this reversed segment.
    return previous;
}

#include <cassert>

// Helper to build a list from an array
node* buildList(const int* values, int size) {
    if (size == 0) return nullptr;
    node* head = new node(values[0]);
    node* temp = head;
    for (int i = 1; i < size; ++i) {
        temp->next = new node(values[i]);
        temp = temp->next;
    }
    return head;
}

// Helper to convert list to vector-like for comparison (use count and values)
bool isEqual(node* head, const int* expected, int size) {
    node* temp = head;
    for (int i = 0; i < size; ++i) {
        if (temp == nullptr || temp->data != expected[i]) return false;
        temp = temp->next;
    }
    return temp == nullptr;
}

// Helper to delete list
void deleteList(node* head) {
    while (head) {
        node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Empty list
    node* empty = nullptr;
    assert(reverseEveryK(empty, 2) == nullptr);

    // Test 2: k > list length → unchanged
    int arr1[] = {1, 2, 3};
    node* list1 = buildList(arr1, 3);
    node* result1 = reverseEveryK(list1, 5);
    int expected1[] = {1, 2, 3};
    assert(isEqual(result1, expected1, 3));

    // Test 3: Exact multiple of k
    int arr2[] = {1, 2, 3, 4, 5, 6};
    node* list2 = buildList(arr2, 6);
    node* result2 = reverseEveryK(list2, 2);
    int expected2[] = {2, 1, 4, 3, 6, 5};
    assert(isEqual(result2, expected2, 6));

    // Test 4: Not multiple of k, partial group left
    int arr3[] = {1, 2, 3, 4, 5};
    node* list3 = buildList(arr3, 5);
    node* result3 = reverseEveryK(list3, 3);
    int expected3[] = {3, 2, 1, 4, 5};
    assert(isEqual(result3, expected3, 5));

    // Test 5: k=1 (no change)
    int arr4[] = {7, 8, 9};
    node* list4 = buildList(arr4, 3);
    node* result4 = reverseEveryK(list4, 1);
    int expected4[] = {7, 8, 9};
    assert(isEqual(result4, expected4, 3));

    // Test 6: Single node with k=1
    int arr5[] = {42};
    node* list5 = buildList(arr5, 1);
    node* result5 = reverseEveryK(list5, 1);
    int expected5[] = {42};
    assert(isEqual(result5, expected5, 1));

    // Test 7: k=2 on odd length
    int arr6[] = {1, 2, 3, 4, 5};
    node* list6 = buildList(arr6, 5);
    node* result6 = reverseEveryK(list6, 2);
    int expected6[] = {2, 1, 4, 3, 5};
    assert(isEqual(result6, expected6, 5));

    // Clean up
    deleteList(result1);
    deleteList(result2);
    deleteList(result3);
    deleteList(result4);
    deleteList(result5);
    deleteList(result6);
    return 0;
}
