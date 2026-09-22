// Write a C++ function named `sortLinkedList` that takes the head of a singly linked list (where each node has an integer `val` and a `next` pointer) and returns the head of the same linked list after sorting its values in non-decreasing order (ascending). The function must modify the list in place (i.e., it may change node values, but not rearrange the nodes themselves). For example, given a list `4 -> 2 -> 1 -> 3`, the function should return a list with values `1 -> 2 -> 3 -> 4`. The input list may be empty (head is `nullptr`), contain duplicate values, or contain negative numbers. The function should handle these cases without errors. You are expected to use a simple approach: copy the node values into a standard container, sort them, and then write them back into the nodes. Ensure your implementation is const-correct where applicable (e.g., do not modify the input pointer itself, but modify the node contents).

#include <cassert>
#include <vector>

// Include the ListNode definition and sortLinkedList function here
// (or place them in a header).

// Helper to build a list from a vector.
ListNode* makeList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* current = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        current->next = new ListNode(vals[i]);
        current = current->next;
    }
    return head;
}

// Helper to compare list values with a vector.
bool listEqualsVector(ListNode* head, const std::vector<int>& expected) {
    size_t i = 0;
    while (head != nullptr) {
        if (i >= expected.size() || head->val != expected[i]) return false;
        head = head->next;
        ++i;
    }
    return i == expected.size();
}

// Helper to delete a linked list.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: basic unsorted list
    {
        ListNode* head = makeList({4, 2, 1, 3});
        head = sortLinkedList(head);
        assert(listEqualsVector(head, {1, 2, 3, 4}));
        deleteList(head);
    }

    // Test 2: already sorted list
    {
        ListNode* head = makeList({1, 2, 3});
        head = sortLinkedList(head);
        assert(listEqualsVector(head, {1, 2, 3}));
        deleteList(head);
    }

    // Test 3: empty list
    {
        ListNode* head = nullptr;
        head = sortLinkedList(head);
        assert(head == nullptr);
    }

    // Test 4: single node
    {
        ListNode* head = makeList({42});
        head = sortLinkedList(head);
        assert(listEqualsVector(head, {42}));
        deleteList(head);
    }

    // Test 5: duplicates
    {
        ListNode* head = makeList({5, 3, 5, 1, 3});
        head = sortLinkedList(head);
        assert(listEqualsVector(head, {1, 3, 3, 5, 5}));
        deleteList(head);
    }

    // Test 6: negative numbers
    {
        ListNode* head = makeList({0, -3, 7, -10, 2});
        head = sortLinkedList(head);
        assert(listEqualsVector(head, {-10, -3, 0, 2, 7}));
        deleteList(head);
    }

    // Test 7: large reverse order with many elements
    {
        std::vector<int> input;
        std::vector<int> expected;
        for (int i = 100; i >= 0; --i) {
            input.push_back(i);
            expected.push_back(i);
        }
        std::sort(expected.begin(), expected.end());
        ListNode* head = makeList(input);
        head = sortLinkedList(head);
        assert(listEqualsVector(head, expected));
        deleteList(head);
    }

    // Test 8: list with zeros and negative duplicates
    {
        ListNode* head = makeList({0, 0, -1, -1, 2, 2});
        head = sortLinkedList(head);
        assert(listEqualsVector(head, {-1, -1, 0, 0, 2, 2}));
        deleteList(head);
    }

    return 0;
}

#include <vector>
#include <algorithm>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Sort the linked list in ascending order by copying values to a vector,
// sorting the vector, and writing values back to the nodes.
ListNode* sortLinkedList(ListNode* head) {
    if (head == nullptr) {
        return head;
    }

    std::vector<int> values;
    ListNode* current = head;
    while (current != nullptr) {
        values.push_back(current->val);
        current = current->next;
    }

    std::sort(values.begin(), values.end());

    current = head;
    for (int value : values) {
        current->val = value;
        current = current->next;
    }

    return head;
}

// The main algorithm is straightforward: traverse the linked list once, storing each node's value into a `std::vector<int>`. After reaching the end of the list, sort the vector using `std::sort` from the `<algorithm>` header. Then, traverse the linked list a second time, overwriting each node's value with the corresponding sorted value from the vector in order. The function returns the original head pointer, since the node structure (the pointers) remains unchanged; only the values are modified.  
// Edge cases:  
// - **Empty list**: If `head` is `nullptr`, the vector will be empty, the second loop will not execute, and we return `nullptr`.  
// - **Single node**: The vector has one element, sort is trivial, the second loop writes that value back, and the list remains unchanged.  
// - **Duplicates**: Duplicate values are handled naturally by sorting; they will appear consecutively in the sorted vector and be written back in that order.  
// - **Negative numbers**: `std::sort` works with integers, including negatives, in ascending order.  
// Time complexity: Traversing the list twice is O(N), and sorting the vector takes O(N log N) in the average and worst cases, so overall O(N log N).  
// Space complexity: O(N) for the vector that stores all values.  
// This approach is not optimal for memory, but it is simple, correct, and matches the given code snippet's approach.
