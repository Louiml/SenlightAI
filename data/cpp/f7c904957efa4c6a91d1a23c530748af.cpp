/*
Write a C++ function that takes the head of a singly linked list and returns the head of the linked list after swapping every two adjacent nodes. The swap must be performed by changing node pointers, not by swapping values inside nodes. If the linked list has an odd number of nodes, the last node remains in place. The function should handle empty lists and single-node lists by returning the input unchanged. The nodes are defined using the standard `ListNode` structure with `val` and `next` members. You may use either an iterative or recursive approach, but your implementation must be self-contained and must not modify the values inside the nodes.
*/

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(val), next(next) {}
};

// Swap every two adjacent nodes in the linked list and return the new head.
ListNode* swapPairs(ListNode* head) {
    // Base case: empty list or single node
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    // The second node becomes the new head after swapping
    ListNode* newHead = head->next;
    
    // Recursively swap the rest of the list starting from the third node
    head->next = swapPairs(newHead->next);
    
    // Make the original second node point to the original first node
    newHead->next = head;
    
    return newHead;
}

#include <cassert>
#include <vector>

// Helper to build a linked list from a vector
ListNode* buildList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* current = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        current->next = new ListNode(vals[i]);
        current = current->next;
    }
    return head;
}

// Helper to convert a linked list to a vector for comparison
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to free memory
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test empty list
    ListNode* empty = nullptr;
    assert(listToVector(swapPairs(empty)) == std::vector<int>());
    
    // Test single node
    ListNode* single = buildList({5});
    assert(listToVector(swapPairs(single)) == std::vector<int>({5}));
    deleteList(single);
    
    // Test two nodes
    ListNode* two = buildList({1, 2});
    ListNode* swappedTwo = swapPairs(two);
    assert(listToVector(swappedTwo) == std::vector<int>({2, 1}));
    deleteList(swappedTwo);
    
    // Test even length
    ListNode* even = buildList({1, 2, 3, 4, 5, 6});
    ListNode* swappedEven = swapPairs(even);
    assert(listToVector(swappedEven) == std::vector<int>({2, 1, 4, 3, 6, 5}));
    deleteList(swappedEven);
    
    // Test odd length
    ListNode* odd = buildList({1, 2, 3, 4, 5});
    ListNode* swappedOdd = swapPairs(odd);
    assert(listToVector(swappedOdd) == std::vector<int>({2, 1, 4, 3, 5}));
    deleteList(swappedOdd);
    
    // Test with duplicate values
    ListNode* dup = buildList({1, 1, 2, 2});
    ListNode* swappedDup = swapPairs(dup);
    assert(listToVector(swappedDup) == std::vector<int>({1, 1, 2, 2}));
    deleteList(swappedDup);
    
    // Test negative values and longer list
    ListNode* neg = buildList({-1, -2, -3, -4, -5, -6, -7});
    ListNode* swappedNeg = swapPairs(neg);
    assert(listToVector(swappedNeg) == std::vector<int>({-2, -1, -4, -3, -6, -5, -7}));
    deleteList(swappedNeg);
    
    return 0;
}

// The solution can be implemented either recursively or iteratively. The recursive approach works by first checking if the list is empty or has only one node, in which case we return the head unchanged. Otherwise, we identify the second node (the new head after swapping), then recursively process the sublist starting from the third node (head->next->next). After the recursion returns the new head of the sublist, we set the original first node's next to point to that result, then set the second node's next to point to the original first node, and finally return the second node as the new head. This correctly swaps pairs and preserves the linkage for odd counts where the final node remains unswapped. The iterative approach uses a dummy node to simplify handling the head change, then loops while both the current node and its next exist, performing pointer reassignments to swap the pair and advancing two nodes forward. Both approaches run in O(n) time where n is the number of nodes, and the recursive approach uses O(n) call stack space in the worst case, while the iterative approach uses O(1) extra space. Edge cases include empty lists, single-node lists, even-length lists, odd-length lists where the last node stays, and lists with exactly two nodes.
