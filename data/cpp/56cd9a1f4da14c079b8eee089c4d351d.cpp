// Write a standalone C++ function named `mergeKSortedLinkedLists` that takes a `std::vector<ListNode*>` of singly-linked lists, each of which is already sorted in non-decreasing order, and returns a single sorted linked list (as a `ListNode*`) containing all elements from all input lists. The input vector may be empty, may contain null pointers (representing empty lists), and may have lists of varying lengths. The total number of nodes across all lists will not exceed 10^4, and node values are integers within the range [-10^4, 10^4]. The function must handle all edge cases gracefully, including an empty vector, a vector of all null lists, and lists with duplicate values. Implement the function without modifying the input lists' structure; instead, create and return a new merged list by reusing the existing nodes in sorted order (i.e., you may rearrange the `next` pointers of the input nodes, but you must not allocate new nodes for the values themselves, except for a temporary dummy head if needed).
// The most efficient approach to merge K sorted linked lists is to use a min-heap (priority queue) that stores the current smallest node from each non-empty list. Initialize the heap by pushing the head node of each non-empty list, using a comparator that orders nodes by their `val` field. Then repeatedly pop the node with the smallest value from the heap, append it to the result list, and if that node has a successor, push that successor into the heap. This ensures that at any time the heap contains at most one node from each input list, so its size is O(K) where K is the number of input lists. The time complexity is O(N log K) where N is the total number of nodes across all lists, because each node is pushed and popped exactly once, and each heap operation takes O(log K). The space complexity is O(K) for the heap, plus O(1) for the dummy node. Important edge cases include: (1) an empty vector `lists` – return `nullptr`; (2) a vector containing only null pointers or empty lists – return `nullptr`; (3) lists of length 1 – straightforward; (4) duplicate values across lists – handled naturally by the heap's ordering. The `priority_queue` must use a custom comparator because the default `std::pair` comparison would break ties by the pointer address, which is not deterministic; instead, we compare only the `val` field, and if two values are equal, we may break ties by pointer address to maintain strict weak ordering.
#include <queue>
#include <vector>

/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Comparator for priority_queue: compare only node values, break ties by pointer.
struct CompareNode {
    bool operator()(const ListNode* a, const ListNode* b) const {
        if (a->val != b->val) {
            return a->val > b->val;  // min-heap: return true if a should come after b
        }
        return a > b;  // tie-breaker: compare pointers (any deterministic order)
    }
};

// Merge K sorted linked lists into one sorted linked list.
// Uses a min-heap to repeatedly pick the smallest head among all lists.
ListNode* mergeKSortedLinkedLists(std::vector<ListNode*>& lists) {
    // Initialize a min-heap of ListNode* using custom comparator.
    std::priority_queue<ListNode*, std::vector<ListNode*>, CompareNode> minHeap;

    // Push the head of every non-empty list.
    for (ListNode* head : lists) {
        if (head != nullptr) {
            minHeap.push(head);
        }
    }

    // Use a dummy node to simplify building the result.
    ListNode dummy(0);
    ListNode* tail = &dummy;

    // While there are nodes in the heap, take the smallest one.
    while (!minHeap.empty()) {
        ListNode* smallest = minHeap.top();
        minHeap.pop();

        // If the smallest node has a successor, push it into the heap.
        if (smallest->next != nullptr) {
            minHeap.push(smallest->next);
        }

        // Append the smallest node to the result list.
        tail->next = smallest;
        tail = tail->next;
    }

    // The merged list starts at dummy.next (may be nullptr if all input empty).
    return dummy.next;
}
#include <cassert>
#include <vector>

// (ListNode definition included here for completeness)
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Helper function to build a linked list from a vector of ints.
ListNode* buildList(const std::vector<int>& vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper function to convert a linked list to a vector of ints.
std::vector<int> listToVector(const ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to free a linked list (to avoid memory leaks in tests).
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

// The solution function declaration (must match exactly).
ListNode* mergeKSortedLinkedLists(std::vector<ListNode*>& lists);

int main() {
    // Test 1: Example from problem statement.
    std::vector<ListNode*> lists1;
    lists1.push_back(buildList({1,4,5}));
    lists1.push_back(buildList({1,3,4}));
    lists1.push_back(buildList({2,6}));
    ListNode* merged1 = mergeKSortedLinkedLists(lists1);
    assert(listToVector(merged1) == std::vector<int>({1,1,2,3,4,4,5,6}));
    freeList(merged1);

    // Test 2: Empty list of lists.
    std::vector<ListNode*> lists2;
    ListNode* merged2 = mergeKSortedLinkedLists(lists2);
    assert(merged2 == nullptr);

    // Test 3: List containing only null (empty) lists.
    std::vector<ListNode*> lists3 = {nullptr, nullptr, nullptr};
    ListNode* merged3 = mergeKSortedLinkedLists(lists3);
    assert(merged3 == nullptr);

    // Test 4: Single list with one element.
    std::vector<ListNode*> lists4;
    lists4.push_back(buildList({42}));
    ListNode* merged4 = mergeKSortedLinkedLists(lists4);
    assert(listToVector(merged4) == std::vector<int>({42}));
    freeList(merged4);

    // Test 5: Many lists with duplicates and varying lengths.
    std::vector<ListNode*> lists5;
    lists5.push_back(buildList({-5, -1}));
    lists5.push_back(buildList({-5}));
    lists5.push_back(buildList({0, 10}));
    lists5.push_back(nullptr);
    lists5.push_back(buildList({-5, 0, 0, 7}));
    ListNode* merged5 = mergeKSortedLinkedLists(lists5);
    assert(listToVector(merged5) == std::vector<int>({-5,-5,-5,-1,0,0,0,7,10}));
    freeList(merged5);

    // Test 6: Single list with multiple elements (already sorted).
    std::vector<ListNode*> lists6;
    lists6.push_back(buildList({1,2,3,4,5}));
    ListNode* merged6 = mergeKSortedLinkedLists(lists6);
    assert(listToVector(merged6) == std::vector<int>({1,2,3,4,5}));
    freeList(merged6);

    // Test 7: Two lists where all elements of one are greater than the other.
    std::vector<ListNode*> lists7;
    lists7.push_back(buildList({1,2}));
    lists7.push_back(buildList({100,200}));
    ListNode* merged7 = mergeKSortedLinkedLists(lists7);
    assert(listToVector(merged7) == std::vector<int>({1,2,100,200}));
    freeList(merged7);

    // Test 8: Edge case: one list has many nodes to ensure heap works correctly.
    std::vector<ListNode*> lists8;
    std::vector<int> big;
    for (int i = 0; i < 100; ++i) big.push_back(i);
    lists8.push_back(buildList(big));
    ListNode* merged8 = mergeKSortedLinkedLists(lists8);
    assert(listToVector(merged8) == big);
    freeList(merged8);

    // Clean up the input lists (note: after merging, nodes are in merged list, so we already freed them above.
    // The original list nodes are part of the merged list and freed by freeList(mergedX).
    // For safety, also free the vector pointers (they are all part of merged lists, so no extra deletion needed).

    return 0;
}
