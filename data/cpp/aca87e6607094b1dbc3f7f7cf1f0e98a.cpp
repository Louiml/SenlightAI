Write a C++ function named `splitLinkedListIntoParts` that takes a singly linked list (represented by a `ListNode` struct with `int val` and `ListNode* next`) and an integer `k`, and splits the list into exactly `k` consecutive parts. The parts should be as equal in length as possible: the size difference between any two parts should be at most 1, and the larger parts must appear earlier in the result. Return a `std::vector<ListNode*>` containing the heads of these `k` parts, with `nullptr` used for any empty parts (when `k` exceeds the list length). The function must modify the original list by breaking it into disjoint parts (i.e., the `next` pointer of the last node in each part must be set to `nullptr`). If the list is empty, the vector should contain `k` null pointers. The function must handle arbitrary non-negative `k` and any list length.

// The approach first computes the total length `L` of the linked list by traversing it. Then, if `k` parts are needed, each part will receive at least `L/k` nodes, and the first `L%k` parts will receive one extra node to account for the remainder. We traverse the list again, and for each of the `k` parts, we set `result[i]` to the current node (which becomes the head of part `i`), then advance exactly `eachBucketNodes + (remainderNodes > 0 ? 1 : 0)` nodes forward, keeping track of the previous node. After advancing, we sever the connection by setting `prev->next = nullptr`. We then decrement `remainderNodes`. This naturally handles empty parts: when `k` > `L`, after we process the first `L` nodes, the list pointer becomes null, and the remaining iterations will push `nullptr` into the vector because `curr` is null (the loop condition `curr && i < k` stops, but the vector is pre-initialized with null pointers). Edge cases include `k=0` (which is not allowed by typical constraints but we can handle by returning an empty vector), an empty list, and `k` much larger than `L`. The algorithm runs in O(L + k) time (O(L) for length calculation and O(L) for the splitting traversal, plus O(k) for vector initialization) and uses O(k) auxiliary space for the result vector (excluding the list nodes themselves). The space complexity is O(1) extra besides the output vector.

#include <vector>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Splits the linked list into k parts, as equal as possible.
// Returns a vector of heads of the parts; empty parts are nullptr.
std::vector<ListNode*> splitLinkedListIntoParts(ListNode* head, int k) {
    // If k is 0, return an empty vector (though k is typically positive).
    if (k <= 0) {
        return {};
    }

    // First pass: compute the total length of the list.
    int length = 0;
    for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
        ++length;
    }

    // Determine the base size and how many parts get an extra node.
    const int baseSize = length / k;
    int remainder = length % k;

    // Result vector initialized with nullptr for all parts.
    std::vector<ListNode*> parts(k, nullptr);

    ListNode* curr = head;
    // Process the first 'k' parts (or until the list is exhausted).
    for (int i = 0; i < k && curr != nullptr; ++i) {
        parts[i] = curr;  // The head of current part.

        // Compute the exact size of this part: baseSize plus 1 if remainder > 0.
        const int partSize = baseSize + (remainder > 0 ? 1 : 0);
        remainder = (remainder > 0) ? remainder - 1 : remainder;

        // Advance 'curr' to the node just after this part, tracking the last node.
        ListNode* prev = nullptr;
        for (int count = 0; count < partSize; ++count) {
            prev = curr;
            if (curr != nullptr) {
                curr = curr->next;
            }
        }

        // Sever the part from the rest of the list.
        if (prev != nullptr) {
            prev->next = nullptr;
        }
    }

    // The remaining entries in 'parts' are already nullptr, which is correct.
    return parts;
}

#include <cassert>
#include <vector>

// ListNode definition is assumed (as above). 
// We'll include it here for completeness to make the test self-contained.
// (In a real submission, you would only include the header.)

// Helper function to build a linked list from a vector of integers.
ListNode* buildList(const std::vector<int>& values) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy->next;
}

// Helper function to check if the parts match expected sizes and values.
bool checkParts(const std::vector<ListNode*>& parts, const std::vector<int>& expectedSizes) {
    if (parts.size() != expectedSizes.size()) return false;
    for (size_t i = 0; i < parts.size(); ++i) {
        int len = 0;
        for (ListNode* p = parts[i]; p != nullptr; p = p->next) ++len;
        if (len != expectedSizes[i]) return false;
    }
    return true;
}

// Helper to free all allocated lists.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Empty list with k=3 -> all parts null.
    {
        ListNode* head = nullptr;
        auto parts = splitLinkedListIntoParts(head, 3);
        assert(parts.size() == 3);
        for (auto p : parts) assert(p == nullptr);
        // Nothing to delete.
    }

    // Test 2: List of [1,2,3], k=5 -> parts of sizes [1,1,1,0,0].
    {
        ListNode* head = buildList({1,2,3});
        auto parts = splitLinkedListIntoParts(head, 5);
        assert(checkParts(parts, {1,1,1,0,0}));
        // Verify values and that links are cut.
        assert(parts[0]->val == 1 && parts[0]->next == nullptr);
        assert(parts[1]->val == 2 && parts[1]->next == nullptr);
        assert(parts[2]->val == 3 && parts[2]->next == nullptr);
        assert(parts[3] == nullptr && parts[4] == nullptr);
        for (auto p : parts) if (p) deleteList(p);
    }

    // Test 3: List of [1,2,3,4,5,6,7,8,9,10], k=3 -> sizes [4,3,3].
    {
        ListNode* head = buildList({1,2,3,4,5,6,7,8,9,10});
        auto parts = splitLinkedListIntoParts(head, 3);
        assert(checkParts(parts, {4,3,3}));
        // Verify values of heads.
        assert(parts[0]->val == 1);
        assert(parts[1]->val == 5);
        assert(parts[2]->val == 8);
        // Verify cut: last node of first part has next==nullptr.
        ListNode* p = parts[0];
        while (p->next) p = p->next;
        assert(p->next == nullptr);
        for (auto part : parts) deleteList(part);
    }

    // Test 4: List of [1], k=1 -> sizes [1].
    {
        ListNode* head = buildList({1});
        auto parts = splitLinkedListIntoParts(head, 1);
        assert(checkParts(parts, {1}));
        assert(parts[0]->val == 1 && parts[0]->next == nullptr);
        deleteList(parts[0]);
    }

    // Test 5: List of [1,2], k=2 -> sizes [1,1].
    {
        ListNode* head = buildList({1,2});
        auto parts = splitLinkedListIntoParts(head, 2);
        assert(checkParts(parts, {1,1}));
        assert(parts[0]->val == 1 && parts[0]->next == nullptr);
        assert(parts[1]->val == 2 && parts[1]->next == nullptr);
        deleteList(parts[0]);
        deleteList(parts[1]);
    }

    // Test 6: List of [1,2,3,4], k=4 -> sizes [1,1,1,1].
    {
        ListNode* head = buildList({1,2,3,4});
        auto parts = splitLinkedListIntoParts(head, 4);
        assert(checkParts(parts, {1,1,1,1}));
        for (int i = 0; i < 4; ++i) {
            assert(parts[i]->val == i+1 && parts[i]->next == nullptr);
            deleteList(parts[i]);
        }
    }

    // Test 7: List of [1,2,3,4,5], k=2 -> sizes [3,2].
    {
        ListNode* head = buildList({1,2,3,4,5});
        auto parts = splitLinkedListIntoParts(head, 2);
        assert(checkParts(parts, {3,2}));
        assert(parts[0]->val == 1 && parts[0]->next != nullptr);
        assert(parts[1]->val == 4 && parts[1]->next != nullptr);
        // Check last node of first part has next==nullptr
        ListNode* p = parts[0];
        while (p->next) p = p->next;
        assert(p->next == nullptr);
        deleteList(parts[0]);
        deleteList(parts[1]);
    }

    // Test 8: k larger than list length and list null.
    {
        ListNode* head = nullptr;
        auto parts = splitLinkedListIntoParts(head, 7);
        assert(parts.size() == 7);
        for (auto p : parts) assert(p == nullptr);
    }

    return 0;
}
