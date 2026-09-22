Write a C++ function `ListNode* sortLinkedList(ListNode* head)` that sorts a singly linked list in ascending order using the merge sort algorithm. The list nodes are defined by the `ListNode` struct with fields `val` and `next`. The function must return a pointer to the head of the sorted list. The input list may be empty, contain duplicate values, or have an arbitrary length. The function should not use extra containers like vectors or arrays and must operate in-place by rearranging node pointers. The solution must be recursive and use the divide-and-conquer technique: split the list into two halves, sort each half recursively, and merge the two sorted halves. Handle edge cases where the list has zero or one node by returning the head unchanged. Do not allocate new nodes except for a temporary dummy node during the merge step.

#include <cassert>

// Helper to create a list from a vector-like initializer list.
ListNode* createList(std::initializer_list<int> values) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : values) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

// Helper to convert list to vector for easy comparison.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete the list.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Empty list
    ListNode* empty = nullptr;
    assert(sortLinkedList(empty) == nullptr);

    // Test 2: Single node
    ListNode* single = new ListNode(5);
    assert(sortLinkedList(single) == single);
    deleteList(single);

    // Test 3: Already sorted
    ListNode* sorted = createList({1, 2, 3, 4});
    ListNode* sortedResult = sortLinkedList(sorted);
    assert(listToVector(sortedResult) == std::vector<int>({1, 2, 3, 4}));
    deleteList(sortedResult);

    // Test 4: Reverse sorted
    ListNode* reverse = createList({5, 4, 3, 2, 1});
    ListNode* reverseResult = sortLinkedList(reverse);
    assert(listToVector(reverseResult) == std::vector<int>({1, 2, 3, 4, 5}));
    deleteList(reverseResult);

    // Test 5: Duplicate values
    ListNode* dup = createList({3, 1, 2, 3, 2});
    ListNode* dupResult = sortLinkedList(dup);
    assert(listToVector(dupResult) == std::vector<int>({1, 2, 2, 3, 3}));
    deleteList(dupResult);

    // Test 6: Negative and positive mixed
    ListNode* mixed = createList({-2, 5, -1, 0, 3});
    ListNode* mixedResult = sortLinkedList(mixed);
    assert(listToVector(mixedResult) == std::vector<int>({-2, -1, 0, 3, 5}));
    deleteList(mixedResult);

    // Test 7: Multiple of same value
    ListNode* same = createList({7, 7, 7});
    ListNode* sameResult = sortLinkedList(same);
    assert(listToVector(sameResult) == std::vector<int>({7, 7, 7}));
    deleteList(sameResult);

    return 0;
}

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merges two sorted linked lists into one sorted list.
ListNode* mergeSortedLists(ListNode* left, ListNode* right) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    
    while (left != nullptr && right != nullptr) {
        if (left->val <= right->val) {
            cur->next = left;
            left = left->next;
        } else {
            cur->next = right;
            right = right->next;
        }
        cur = cur->next;
    }
    
    if (left != nullptr) cur->next = left;
    if (right != nullptr) cur->next = right;
    
    return dummy.next;
}

// Sorts a singly linked list in ascending order using merge sort.
ListNode* sortLinkedList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) return head;
    
    // Find the middle node using slow and fast pointers.
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // Split the list into two halves.
    ListNode* rightHead = sortLinkedList(slow->next);
    slow->next = nullptr;
    ListNode* leftHead = sortLinkedList(head);
    
    // Merge the sorted halves.
    return mergeSortedLists(leftHead, rightHead);
}

// The solution uses merge sort for linked lists because it provides guaranteed O(n log n) time complexity and O(log n) recursion stack space. The algorithm proceeds by:
//
// 1. **Base case**: If the list is empty or has one node, it is already sorted; return head.
// 2. **Find the middle** using the slow-fast pointer technique. Initialize `slow = head` and `fast = head->next`. Move `slow` one step and `fast` two steps until `fast` is null or `fast->next` is null. At the end, `slow` points to the last node of the left half.
// 3. **Split the list**: Record the head of the right half as `slow->next`, then set `slow->next = nullptr` to break the list into two independent halves.
// 4. **Recursively sort** both halves by calling `sortList` on the left head and the right head.
// 5. **Merge** the two sorted halves. Use a dummy node to simplify pointer manipulation. Compare the current node values from each half, attach the smaller node to the merged list, and advance that half's pointer. When one half is exhausted, attach the remainder of the other half.
//
// Edge cases: empty list (return nullptr), single node (return head), duplicate values (merge should use `<=` to maintain stability, but `>` works fine for sorting), very long lists (recursion depth O(log n) because each split halves the list). The merge step uses O(1) extra space excluding the dummy node, and the recursion uses O(log n) stack space. Time complexity is O(n log n) for all cases.
