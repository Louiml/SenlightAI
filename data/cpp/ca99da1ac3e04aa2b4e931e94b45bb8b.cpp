Write a C++ function `ListNode* mergeKLists(std::vector<ListNode*>& lists)` that merges `k` sorted linked lists into one sorted linked list. The input is a vector of pointers to the heads of `k` singly-linked lists, each sorted in non-decreasing order. Some lists may be empty (`nullptr`). The function must return the head of the resulting merged sorted list. Handle the case where the entire input vector is empty by returning `nullptr`. The merge must be stable (elements with equal values retain their relative order from the original lists) and must not create new nodes—only rearrange existing nodes by changing `next` pointers. The function should work correctly for large values of `k` and for lists of arbitrary length.
The optimal approach uses a min-heap (priority queue) to always extract the smallest current head among all lists. We initialize the heap by pushing the head of each non-empty list, with the pair `{node->val, node}` so the heap orders by value. At each step, pop the smallest node, append it to the result tail, and if that node has a `next`, push that next node into the heap. This ensures we always get the next smallest value across all lists. Edge cases: an empty vector returns `nullptr`; empty individual lists are ignored; if all lists are empty, the heap stays empty and the function returns `nullptr`. The algorithm visits each node exactly once. Time complexity: O(N log k) where N is total number of nodes, because each heap operation costs O(log k). Space complexity: O(k) for the heap, plus O(1) auxiliary pointers (ignoring the output list rearranged in-place). No new nodes are allocated; we only reuse existing ones.
#include <vector>
#include <queue>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merge k sorted linked lists into one sorted list by rearranging existing nodes.
ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;

    // Min-heap ordered by node value, then by pointer (for deterministic ordering).
    using NodePair = std::pair<int, ListNode*>;
    auto compare = [](const NodePair& a, const NodePair& b) {
        return a.first > b.first || (a.first == b.first && a.second > b.second);
    };
    std::priority_queue<NodePair, std::vector<NodePair>, decltype(compare)> pq(compare);

    // Push all non-null heads.
    for (ListNode* head : lists) {
        if (head != nullptr) {
            pq.push({head->val, head});
        }
    }

    ListNode dummyHead(0);
    ListNode* tail = &dummyHead;

    while (!pq.empty()) {
        auto [val, node] = pq.top();
        pq.pop();

        // If there is a next node in that list, push it.
        if (node->next != nullptr) {
            pq.push({node->next->val, node->next});
        }

        // Append current node to result.
        tail->next = node;
        tail = tail->next;
    }

    tail->next = nullptr; // Ensure termination.
    return dummyHead.next;
}
#include <cassert>
#include <vector>

// ... (ListNode definition and mergeKLists function as above)

// Helper to create a linked list from initializer list.
ListNode* makeList(std::initializer_list<int> values) {
    ListNode* head = nullptr;
    ListNode** current = &head;
    for (int v : values) {
        *current = new ListNode(v);
        current = &((*current)->next);
    }
    return head;
}

// Helper to convert linked list to vector for comparison.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to free memory of linked list.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Basic merge of three sorted lists.
    std::vector<ListNode*> lists1 = {makeList({1,4,5}), makeList({1,3,4}), makeList({2,6})};
    ListNode* result1 = mergeKLists(lists1);
    assert(listToVector(result1) == std::vector<int>({1,1,2,3,4,4,5,6}));
    deleteList(result1);

    // Test 2: Empty vector.
    std::vector<ListNode*> lists2;
    ListNode* result2 = mergeKLists(lists2);
    assert(result2 == nullptr);

    // Test 3: Some lists are empty.
    std::vector<ListNode*> lists3 = {nullptr, makeList({5}), nullptr, makeList({1,7})};
    ListNode* result3 = mergeKLists(lists3);
    assert(listToVector(result3) == std::vector<int>({1,5,7}));
    deleteList(result3);

    // Test 4: All lists empty.
    std::vector<ListNode*> lists4 = {nullptr, nullptr};
    ListNode* result4 = mergeKLists(lists4);
    assert(result4 == nullptr);

    // Test 5: Single list with one node.
    std::vector<ListNode*> lists5 = {makeList({42})};
    ListNode* result5 = mergeKLists(lists5);
    assert(listToVector(result5) == std::vector<int>({42}));
    deleteList(result5);

    // Test 6: Many lists with equal values.
    std::vector<ListNode*> lists6 = {makeList({1,1}), makeList({1}), makeList({1,1,1})};
    ListNode* result6 = mergeKLists(lists6);
    assert(listToVector(result6) == std::vector<int>({1,1,1,1,1,1}));
    deleteList(result6);

    // Test 7: Negative values.
    std::vector<ListNode*> lists7 = {makeList({-3,-1}), makeList({-5}), makeList({-2,0})};
    ListNode* result7 = mergeKLists(lists7);
    assert(listToVector(result7) == std::vector<int>({-5,-3,-2,-1,0}));
    deleteList(result7);

    // Test 8: Large k with many empty lists.
    std::vector<ListNode*> lists8(100, nullptr);
    lists8[0] = makeList({2});
    lists8[99] = makeList({1,3});
    ListNode* result8 = mergeKLists(lists8);
    assert(listToVector(result8) == std::vector<int>({1,2,3}));
    deleteList(result8);

    // Test 9: Already merged order.
    std::vector<ListNode*> lists9 = {makeList({1,2,3}), nullptr, makeList({4,5})};
    ListNode* result9 = mergeKLists(lists9);
    assert(listToVector(result9) == std::vector<int>({1,2,3,4,5}));
    deleteList(result9);

    // Test 10: Single list with many nodes.
    std::vector<ListNode*> lists10 = {makeList({5,10,15})};
    ListNode* result10 = mergeKLists(lists10);
    assert(listToVector(result10) == std::vector<int>({5,10,15}));
    deleteList(result10);

    return 0;
}
