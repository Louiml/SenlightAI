Write a C++ function that accepts a non-empty singly-linked list of integers and returns the maximum pair sum, where a pair consists of the i-th node from the start and the i-th node from the end (0-indexed). The list always has an even number of nodes. The function should not modify the original list (it may create new nodes or a reverse copy, but the original list's structure and values must remain unchanged after the function returns). The list is represented by the standard `ListNode` struct with members `int val` and `ListNode* next`.

The key insight is that pair sums pair the first half with the reversed second half. We use the classic slow/fast pointer technique to find the middle of the list. Since the length is even, `slow` ends at the last node of the first half, and `slow->next` is the head of the second half. To avoid mutating the original list, we reverse the second half by building a new reversed list (using a helper that creates new `ListNode` objects copying values). Then we traverse the first half (original nodes) and the reversed second half simultaneously, summing corresponding values and tracking the maximum. After computing, the reversed list is a separate chain of nodes (we don't need to free them in a competitive programming context, but for a clean function we could ignore memory cleanup; the original list remains intact). Edge cases: even length guaranteed, so no need to handle odd-length; handle a list of two nodes (reverse of second half is a single node). Time complexity is O(n) for the two-pointer pass and O(n) for building and traversing the reversed list, so overall O(n). Auxiliary space is O(n) because we create a reversed copy of the second half.

#include <algorithm>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverse a linked list by creating a new copy (does not modify the input).
ListNode* reverseCopy(ListNode* head) {
    ListNode* dummy = new ListNode();
    ListNode* curr = head;
    while (curr) {
        ListNode* newNode = new ListNode(curr->val);
        newNode->next = dummy->next;
        dummy->next = newNode;
        curr = curr->next;
    }
    return dummy->next;
}

// Return the maximum sum of pairs (i-th from start + i-th from end).
// The original list is not modified.
int maxPairSum(const ListNode* head) {
    // Find the middle using two pointers.
    const ListNode* slow = head;
    const ListNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // slow now points to the last node of the first half.

    // Reverse the second half into a new list (copy).
    ListNode* secondHalf = reverseCopy(slow->next);

    // Traverse the first half (original) and the reversed second half.
    const ListNode* first = head;
    ListNode* second = secondHalf;
    int ans = 0;
    while (first && second) {
        ans = std::max(ans, first->val + second->val);
        first = first->next;
        second = second->next;
    }
    return ans;
}

#include <cassert>

int main() {
    // Helper to build a list from an initializer list.
    auto buildList = [](std::initializer_list<int> vals) {
        ListNode dummy;
        ListNode* tail = &dummy;
        for (int v : vals) {
            tail->next = new ListNode(v);
            tail = tail->next;
        }
        return dummy.next;
    };

    // Test 1: simple 2-node list.
    ListNode* l1 = buildList({5, 3});
    assert(maxPairSum(l1) == 8);

    // Test 2: 4 nodes.
    ListNode* l2 = buildList({1, 2, 3, 4});
    // pairs: (1+4)=5, (2+3)=5 -> max = 5
    assert(maxPairSum(l2) == 5);

    // Test 3: 4 nodes with asymmetric values.
    ListNode* l3 = buildList({10, 1, 1, 10});
    // pairs: (10+10)=20, (1+1)=2 -> max = 20
    assert(maxPairSum(l3) == 20);

    // Test 4: 6 nodes.
    ListNode* l4 = buildList({3, 2, 5, 4, 1, 6});
    // pairs: (3+6)=9, (2+1)=3, (5+4)=9 -> max = 9
    assert(maxPairSum(l4) == 9);

    // Test 5: negative values.
    ListNode* l5 = buildList({-5, -1, -2, -3});
    // pairs: (-5 + -3)=-8, (-1 + -2)=-3 -> max = -3
    assert(maxPairSum(l5) == -3);

    // Test 6: verify the original list is not modified.
    ListNode* orig = buildList({2, 7, 4, 9});
    ListNode* save = orig;
    int result = maxPairSum(orig);
    // Since we don't modify, we can just check the stored values remain.
    std::vector<int> vals;
    while (orig) {
        vals.push_back(orig->val);
        orig = orig->next;
    }
    assert(vals.size() == 4);
    assert(vals[0] == 2 && vals[1] == 7 && vals[2] == 4 && vals[3] == 9);
    assert(result == 11); // 2+9=11, 7+4=11

    // Test 7: large equal max at both ends.
    ListNode* l7 = buildList({100, 0, 0, 100});
    assert(maxPairSum(l7) == 200);

    return 0;
}
