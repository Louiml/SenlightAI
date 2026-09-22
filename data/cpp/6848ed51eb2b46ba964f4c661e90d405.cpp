Given a head address, a total number of nodes, and a positive integer K, write a C++ function `reverseEveryK` that takes as input the head address (as an int), the total node count N, K, and an array of node records (each containing data and next address), and returns the head address of the linked list after reversing the links of every K consecutive nodes. If the remaining nodes at the end are fewer than K, they must be left in their original order. The addresses are non‑negative 5‑digit integers, with -1 representing NULL. The input array is indexed by node address, and each node record stores its `data` and `next` address. The function must modify the `next` pointers in the array in‑place and return the new head address. For example, given N=6, K=4, and a list 1→2→3→4→5→6 (where 1 is the head address, etc.), the returned head should be 4, and the `next` pointers should be updated so that the list becomes 4→3→2→1→5→6. If K=3, the result should be 3→2→1→6→5→4. The function must handle cases where the input list may contain invalid nodes (nodes not reachable from the head) or when K > N; in such cases, the list should remain unchanged. The complexity should be O(N) time and O(1) extra space, excluding the array storage.
The solution uses an iterative approach with a sentinel logic similar to the classic "reverse every K nodes" problem, but applied to an array-based linked list where nodes are indexed by their address. We maintain a pointer `p` that initially points to the head address. For each group of K nodes, we first check if there are at least K nodes remaining by traversing K steps from the current position; if we hit -1 before reaching K nodes, we stop. Then we reverse the links inside that group. Reversing a group requires careful pointer manipulation: we set `pp` to the node that follows the group (i.e., the next of the last node), then traverse from the first node to the last, reversing each `next` pointer. After reversal, the new last node is the original first node, and its `next` is set to the node that follows the group. Then we update the reference to the head of the group (which is stored in a pointer variable that points either to `front` or to a `next` field of a preceding node) to the new first node (original last). Finally, we advance `p` to the new last node's `next` field, which now points to the node following the reversed group. This process repeats until fewer than K nodes remain. Edge cases: if K=1, no reversal is needed; if K>N or the list is shorter than K from the current point, we break. Also, the input may contain nodes that are not reachable from the head; those are ignored. Time complexity is O(N) because each node is visited at most twice (once for the group check and once for reversal). Space complexity is O(1) extra, since we only use a few integer pointers.
#include <vector>
#include <cstddef>

// Node record as stored in the array, indexed by address.
struct ListNodeRecord {
    int data;
    int next;
};

// Reverse every K nodes in the linked list stored in 'nodes'.
// 'nodes' is a vector indexed by node address; 'head' is the starting address.
// Returns the new head address after reversals.
int reverseEveryK(int head, int K, const std::vector<int>& data, const std::vector<int>& next) {
    if (K <= 1 || head == -1) {
        return head;
    }
    
    // Create a local mutable copy of 'next' pointers to modify.
    std::vector<int> nextCopy = next; // size up to max address
    
    // Helper lambda to get the address of the K-th node starting from 'cur'.
    // Returns -1 if fewer than K nodes remain.
    auto getKth = [&](int cur) -> int {
        int steps = K - 1;
        while (steps-- && cur != -1) {
            cur = nextCopy[cur];
        }
        return cur;
    };
    
    // 'dummy' is an artificial node that points to the head; its address is -2.
    // We store its 'next' pointer in a separate variable.
    int dummyNext = head;
    
    // 'prevGroupLast' points to the node before the current group.
    // Initially, it's the dummy node (address -2), whose 'next' field we track.
    int prevGroupLast = -2; // sentinel address
    int* prevNextPtr = &dummyNext; // points to the variable that holds the link to the first node of the group
    
    int cur = head;
    
    while (cur != -1) {
        int kth = getKth(cur);
        if (kth == -1) {
            break; // fewer than K nodes remain, stop
        }
        
        // 'kth' is the last node of the group.
        // Reverse the links from 'cur' to 'kth'.
        int pp = nextCopy[kth]; // the node after the group
        int p = cur;
        int pn = nextCopy[p];
        
        while (true) {
            nextCopy[p] = pp;
            pp = p;
            if (pp == kth) {
                break;
            }
            p = pn;
            pn = nextCopy[pn];
        }
        
        // Now the group is reversed: original first becomes last, original last becomes first.
        // Update the link from the previous group (or dummy) to point to the new first (kth).
        *prevNextPtr = kth;
        
        // The new 'last' of the group is the original first 'cur'.
        // Its 'next' field already points to 'pp' (the node after the group), which is correct.
        
        // Move to the next group:
        // 'prevGroupLast' becomes the new last (original first).
        prevGroupLast = cur;
        // The next group's first node is the one after the reversed group, which is stored in nextCopy[cur] (since after reversal, nextCopy[cur] = pp = old next after group).
        cur = nextCopy[cur];
        // Update the pointer to the link that should point to the next group's first.
        // That link is now the 'next' field of the new last node (prevGroupLast).
        // We need to set prevNextPtr to point to nextCopy[prevGroupLast].
        prevNextPtr = &nextCopy[prevGroupLast];
    }
    
    // If no reversal was done (e.g., K>N or head=-1), dummyNext remains the original head.
    return dummyNext;
}
#include <cassert>
#include <vector>

// The function is declared above; we just need to test it.
int main() {
    // Build a linked list: addresses 1 -> 2 -> 3 -> 4 -> 5 -> 6
    // Data values 1,2,3,4,5,6 (not important for tests)
    // Using a vector large enough to hold addresses 1-6.
    std::vector<int> data(100000, 0);
    std::vector<int> next(100000, -1);
    
    // Setup: address 1 has data 1, next 2; etc.
    data[1] = 1; next[1] = 2;
    data[2] = 2; next[2] = 3;
    data[3] = 3; next[3] = 4;
    data[4] = 4; next[4] = 5;
    data[5] = 5; next[5] = 6;
    data[6] = 6; next[6] = -1;
    
    // Test K=4: Expected order 4->3->2->1->5->6
    int head = reverseEveryK(1, 4, data, next);
    assert(head == 4);
    assert(next[4] == 3);
    assert(next[3] == 2);
    assert(next[2] == 1);
    assert(next[1] == 5);
    assert(next[5] == 6);
    assert(next[6] == -1);
    
    // Reset the list for another test
    next[1] = 2; next[2] = 3; next[3] = 4; next[4] = 5; next[5] = 6; next[6] = -1;
    
    // Test K=3: Expected 3->2->1->6->5->4
    head = reverseEveryK(1, 3, data, next);
    assert(head == 3);
    assert(next[3] == 2);
    assert(next[2] == 1);
    assert(next[1] == 6);
    assert(next[6] == 5);
    assert(next[5] == 4);
    assert(next[4] == -1);
    
    // Reset again
    next[1] = 2; next[2] = 3; next[3] = 4; next[4] = 5; next[5] = 6; next[6] = -1;
    
    // Test K=2: Expected 2->1->4->3->6->5
    head = reverseEveryK(1, 2, data, next);
    assert(head == 2);
    assert(next[2] == 1);
    assert(next[1] == 4);
    assert(next[4] == 3);
    assert(next[3] == 6);
    assert(next[6] == 5);
    assert(next[5] == -1);
    
    // Test K=1: no change
    next[1] = 2; next[2] = 3; next[3] = 4; next[4] = 5; next[5] = 6; next[6] = -1;
    head = reverseEveryK(1, 1, data, next);
    assert(head == 1);
    assert(next[1] == 2);
    assert(next[2] == 3);
    
    // Test K=7 (greater than N): no change
    next[1] = 2; next[2] = 3; next[3] = 4; next[4] = 5; next[5] = 6; next[6] = -1;
    head = reverseEveryK(1, 7, data, next);
    assert(head == 1);
    assert(next[1] == 2);
    
    // Test single node list: head=10, next=-1
    next[10] = -1; data[10] = 100;
    head = reverseEveryK(10, 1, data, next);
    assert(head == 10);
    assert(next[10] == -1);
    
    // Test empty list: head = -1
    head = reverseEveryK(-1, 3, data, next);
    assert(head == -1);
    
    return 0;
}
