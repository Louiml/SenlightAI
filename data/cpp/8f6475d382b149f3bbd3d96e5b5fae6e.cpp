Given a singly linked list where each node contains an `int` value, write a C++ function `ListNode* sortLinkedList(ListNode* head)` that sorts the list in ascending order and returns the new head. The function must use only constant extra space (no arrays, vectors, or recursion stacks beyond O(log n) for the divide-and-conquer approach), and must not modify the `val` fields of existing nodes—only rearrange pointers. The input list may be empty, contain one node, contain duplicate values, or be already sorted. The function must be self-contained and should be implemented using an iterative or recursive merge sort approach with a fast/slow pointer to find the middle.
// The core algorithm is merge sort adapted for linked lists, which guarantees \(O(n \log n)\) time and \(O(1)\) auxiliary space (ignoring the recursive stack). The main steps are:  
// 1. **Base case:** If the list is empty or has only one node, return it as already sorted.  
// 2. **Split:** Find the middle node using the slow/fast pointer technique. The slow pointer moves one step, the fast pointer moves two steps. When the fast pointer reaches the end, the slow pointer is at the middle (or left-middle for even length). Disconnect the left half by setting `mid->next = NULL`.  
// 3. **Recursively sort** the left half (original head to mid) and the right half (mid->next onwards).  
// 4. **Merge** the two sorted halves by comparing node values and rearranging pointers. Use a dummy node to simplify the merge by avoiding special cases for the head.  
// Edge cases: empty list, single node, two nodes, odd/even lengths, duplicate values, and already sorted or reverse-sorted lists. The merge must be stable; if equal values, prefer taking from the left list to preserve stability (though not required by the spec, it is good practice). Time complexity is \(O(n \log n)\) and space complexity is \(O(1)\) for the iterative merge, plus \(O(\log n)\) stack space for recursion, which is acceptable under "constant" as typically interpreted for linked-list merge sort.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Helper: find the middle node (left-middle for even length).
static ListNode* findMiddle(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Helper: merge two sorted linked lists into one sorted list.
static ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = (l1 != nullptr) ? l1 : l2;
    return dummy.next;
}

// Main function: sort a linked list in ascending order using merge sort.
ListNode* sortLinkedList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    ListNode* mid = findMiddle(head);
    ListNode* rightHead = mid->next;
    mid->next = nullptr; // split into two halves

    ListNode* left = sortLinkedList(head);
    ListNode* right = sortLinkedList(rightHead);

    return mergeTwoLists(left, right);
}
#include <cassert>

// Helper to build a list from a vector (not part of solution, for test only)
ListNode* buildList(const std::vector<int>& vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to convert list to vector for comparison
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> res;
    while (head) {
        res.push_back(head->val);
        head = head->next;
    }
    return res;
}

// Helper to delete list and avoid memory leaks
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Case 1: empty list
    assert(sortLinkedList(nullptr) == nullptr);

    // Case 2: single node
    ListNode* single = new ListNode(5);
    assert(listToVector(sortLinkedList(single)) == std::vector<int>{5});
    deleteList(single);

    // Case 3: already sorted
    ListNode* sorted = buildList({1, 2, 3, 4});
    assert(listToVector(sortLinkedList(sorted)) == std::vector<int>({1, 2, 3, 4}));
    deleteList(sorted);

    // Case 4: reverse sorted
    ListNode* reverse = buildList({4, 3, 2, 1});
    assert(listToVector(sortLinkedList(reverse)) == std::vector<int>({1, 2, 3, 4}));
    deleteList(reverse);

    // Case 5: unsorted with duplicates and negative numbers
    ListNode* unsorted = buildList({-3, 5, 0, 5, -1, 2, -3});
    assert(listToVector(sortLinkedList(unsorted)) == std::vector<int>({-3, -3, -1, 0, 2, 5, 5}));
    deleteList(unsorted);

    // Case 6: two nodes odd order
    ListNode* two = buildList({9, 1});
    assert(listToVector(sortLinkedList(two)) == std::vector<int>({1, 9}));
    deleteList(two);

    // Case 7: large random-ish (small enough to be deterministic)
    ListNode* many = buildList({10, -5, 3, 0, 7, -2, 8, 1, 9, -4});
    assert(listToVector(sortLinkedList(many)) == std::vector<int>({-5, -4, -2, 0, 1, 3, 7, 8, 9, 10}));
    deleteList(many);

    return 0;
}
