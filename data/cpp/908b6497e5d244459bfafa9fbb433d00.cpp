// Write a C++ function `reverseKGroups` that takes the head of a singly linked list and a positive integer `k`, and reverses the linked list in groups of size `k`. If the number of nodes in the list is not a multiple of `k`, the remaining nodes at the end (fewer than `k`) should remain in their original order, not reversed. The function should return the new head of the list after the group-wise reversal. You may assume `k >= 1`. If the list is empty, return `nullptr`. The node structure is already defined with `int data` and `node* next`. You should implement the helper function to find the k-th node and the reversal logic without using extra memory (in-place), and the solution must not modify `k` in an unexpected way (it should remain unchanged for the caller).
#include <cassert>
#include <iostream>

// Assume node struct and functions from solution are above (or included).

// Helper to build list from vector
node* buildList(std::initializer_list<int> vals) {
    node* head = nullptr;
    node** ptr = &head;
    for (int v : vals) {
        *ptr = new node{v, nullptr};
        ptr = &((*ptr)->next);
    }
    return head;
}

// Helper to convert list to vector for comparison
std::vector<int> listToVector(node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to free list
void freeList(node* head) {
    while (head) {
        node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Normal group reversal, exact multiple
    node* head1 = buildList({1,2,3,4,5,6});
    head1 = reverseKGroups(head1, 2);
    assert(listToVector(head1) == std::vector<int>({2,1,4,3,6,5}));
    freeList(head1);

    // Test 2: Leftover nodes (not multiple)
    node* head2 = buildList({1,2,3,4,5,6,7});
    head2 = reverseKGroups(head2, 3);
    assert(listToVector(head2) == std::vector<int>({3,2,1,6,5,4,7}));
    freeList(head2);

    // Test 3: k = 1 (no change)
    node* head3 = buildList({1,2,3});
    head3 = reverseKGroups(head3, 1);
    assert(listToVector(head3) == std::vector<int>({1,2,3}));
    freeList(head3);

    // Test 4: Empty list
    node* head4 = nullptr;
    head4 = reverseKGroups(head4, 3);
    assert(head4 == nullptr);

    // Test 5: Single node
    node* head5 = buildList({42});
    head5 = reverseKGroups(head5, 5);
    assert(listToVector(head5) == std::vector<int>({42}));
    freeList(head5);

    // Test 6: k larger than list length
    node* head6 = buildList({1,2,3});
    head6 = reverseKGroups(head6, 10);
    assert(listToVector(head6) == std::vector<int>({1,2,3}));
    freeList(head6);

    // Test 7: All nodes in one group (k == length)
    node* head7 = buildList({1,2,3,4});
    head7 = reverseKGroups(head7, 4);
    assert(listToVector(head7) == std::vector<int>({4,3,2,1}));
    freeList(head7);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <cstddef>

// Node structure for singly linked list
struct node {
    int data;
    node* next;
};

// Helper: find the k-th node from given start (1-indexed). Returns nullptr if fewer than k nodes remain.
node* findKth(node* start, int k) {
    node* current = start;
    int count = 1; // start counts as first
    while (current != nullptr && count < k) {
        current = current->next;
        ++count;
    }
    return (count == k) ? current : nullptr;
}

// Helper: reverse a linked list segment (from head to tail inclusive) and return new head.
node* reverseSegment(node* head) {
    node* prev = nullptr;
    node* curr = head;
    while (curr != nullptr) {
        node* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }
    return prev;
}

// Main function: reverse linked list in groups of size k.
node* reverseKGroups(node* head, int k) {
    if (head == nullptr || k <= 1) {
        return head;
    }

    node* temp = head;          // current start of a group
    node* prevgroupTail = nullptr; // tail of the previous reversed group
    node* newHead = nullptr;    // final head after all reversals

    while (temp != nullptr) {
        node* kth = findKth(temp, k);
        if (kth == nullptr) {
            // Remaining nodes are fewer than k: leave them as is, but link previous group tail to temp.
            if (prevgroupTail != nullptr) {
                prevgroupTail->next = temp;
            } else {
                // No previous group (k > list length) → head remains original.
                newHead = head;
            }
            break;
        }

        node* nextgroup = kth->next;
        kth->next = nullptr; // detach this group

        node* reversedHead = reverseSegment(temp);
        if (newHead == nullptr) {
            newHead = reversedHead; // first group: reversedHead is actually kth
        } else {
            prevgroupTail->next = reversedHead;
        }

        // After reversal, original temp becomes the tail of this group.
        prevgroupTail = temp;
        temp = nextgroup;
    }

    // If we always had complete groups and exhausted exactly, prevgroupTail->next was set to nextgroup
    // but in the loop we don't link that until next iteration or break. Actually the loop
    // handles linking via break or next iteration. So ensure final linking if loop ends naturally.
    if (temp == nullptr && prevgroupTail != nullptr) {
        // prevgroupTail->next already set to nextgroup? In our loop, we set prevgroupTail->next only
        // when there is a next group. For the last complete group, nextgroup was nullptr, so
        // we should ensure it's nullptr. It already is because we detached and set prevgroupTail = temp
        // and temp = nextgroup (which is nullptr). So nothing else needed.
    }

    return (newHead != nullptr) ? newHead : head;
}
// The core idea is to process the list in chunks of size `k`. Use a pointer `temp` to track the current start of a group. For each group, locate the `k`-th node from `temp` (call it `kth`). If `kth` is `nullptr`, then the remaining nodes are fewer than `k`; in that case, leave them as is but connect the previous group's tail to this remaining segment (or set head if no previous group). Otherwise, detach the group by setting `kth->next = nullptr`, reverse the group from `temp` to `kth` using three-pointer iterative reversal, and reconnect: if this is the first group, set the new head to `kth`; otherwise, link the previous group's tail (`prevnode`) to `kth`. After reversing a complete group, the original start `temp` becomes the tail of that reversed group, so set `prevnode = temp`, then move `temp` to `nextnode` (the node just after `kth`). Edge cases: empty list (return `nullptr`), `k=1` (list unchanged), group size exactly equal to list length (entire list reversed), and leftover nodes less than `k` (leave them in original order). Time complexity is O(n) because each node is visited at most twice (once to find k-th node, once during reversal). Space complexity is O(1) auxiliary (only a few pointers).
