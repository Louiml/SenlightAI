Given a doubly linked list with integer data, write a C++ function that rearranges the nodes in-place so that all nodes at odd positions (1st, 3rd, 5th, ...) appear first, followed by all nodes at even positions (2nd, 4th, 6th, ...), preserving the relative order within each group. The function should take the head pointer of the list, modify the links directly (without allocating new nodes or using an external array), and return the new head. Handle edge cases including empty list, single node, two nodes, and lists with odd/even lengths. The function must be named `rearrangeOddEven` and must operate solely by adjusting `next` and `back` pointers of the existing nodes; do not modify the `data` values.
The optimal solution separates the list into two interleaved chains: the odd-indexed chain starting at the original head, and the even-indexed chain starting at `head->next`. Maintain two pointers, `odd` and `even`, where `odd` points to the current tail of the odd chain and `even` to the current tail of the even chain. In each iteration, relink `odd->next` to `even->next` (the next odd node) and `even->next` to `even->next->next` (the next even node), then advance both pointers. Update the `back` pointers accordingly for the doubly linked list, setting `odd->next->back = odd` and `even->next->back = even` when those links are changed. Continue while `even` and `even->next` are non-null. After the loop, connect the tail of the odd chain to the head of the even chain (`odd->next = evenHead` and `evenHead->back = odd`), and set the even chain's tail's `next` to `nullptr` if it exists (the loop already ensures this, but for safety handle the case where the even chain is empty). Edge cases: if `head` is null, return null; if `head->next` is null (single node), return head unchanged; if the list has exactly two nodes, no loop runs, and directly connect `head->next` after the odd chain (which is `head`) to `evenHead` (which is `head->next`), which is a no-op but handles correctly. Time complexity is O(n) because each node is visited once. Space complexity is O(1) auxiliary space (only a few pointers), excluding the input list itself.
#include <cstddef>  // for nullptr

// Definition of the doubly linked list node.
struct Node {
    int data;
    Node* next;
    Node* back;
    Node(int val) : data(val), next(nullptr), back(nullptr) {}
};

// Rearrange a doubly linked list so odd-positioned nodes come first,
// then even-positioned nodes, preserving relative order within each group.
Node* rearrangeOddEven(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;  // Empty or single node: no change needed.
    }

    Node* odd = head;          // Tail of the odd-positioned chain
    Node* evenHead = head->next; // Head of the even-positioned chain
    Node* even = evenHead;     // Tail of the even-positioned chain

    while (even != nullptr && even->next != nullptr) {
        // Link odd's next to the next odd node (which is even->next)
        odd->next = even->next;
        odd->next->back = odd;   // set backward pointer
        odd = odd->next;         // advance odd

        // Link even's next to the next even node (which is odd->next)
        even->next = odd->next;
        if (even->next != nullptr) {
            even->next->back = even; // set backward pointer
        }
        even = even->next;       // advance even
    }

    // Connect the tail of the odd chain to the head of the even chain
    odd->next = evenHead;
    if (evenHead != nullptr) {
        evenHead->back = odd;
    }

    return head;
}
#include <cassert>
#include <vector>

// Helper to create a doubly linked list from a vector.
Node* createList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    Node* head = new Node(vals[0]);
    Node* prev = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        Node* curr = new Node(vals[i]);
        curr->back = prev;
        prev->next = curr;
        prev = curr;
    }
    return head;
}

// Helper to extract data from the linked list in order.
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to free the linked list memory.
void deleteList(Node* head) {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Empty list
    assert(rearrangeOddEven(nullptr) == nullptr);

    // Test 2: Single node
    Node* single = createList({5});
    assert(listToVector(rearrangeOddEven(single)) == std::vector<int>({5}));
    deleteList(single);

    // Test 3: Two nodes [1,2] -> [1,2]
    Node* two = createList({1,2});
    assert(listToVector(rearrangeOddEven(two)) == std::vector<int>({1,2}));
    deleteList(two);

    // Test 4: Three nodes [1,2,3] -> [1,3,2]
    Node* three = createList({1,2,3});
    assert(listToVector(rearrangeOddEven(three)) == std::vector<int>({1,3,2}));
    deleteList(three);

    // Test 5: Four nodes [1,2,3,4] -> [1,3,2,4]
    Node* four = createList({1,2,3,4});
    assert(listToVector(rearrangeOddEven(four)) == std::vector<int>({1,3,2,4}));
    deleteList(four);

    // Test 6: Five nodes [1,2,3,4,5] -> [1,3,5,2,4]
    Node* five = createList({1,2,3,4,5});
    assert(listToVector(rearrangeOddEven(five)) == std::vector<int>({1,3,5,2,4}));
    deleteList(five);

    // Test 7: Ten nodes [1..10] -> [1,3,5,7,9,2,4,6,8,10]
    Node* ten = createList({1,2,3,4,5,6,7,8,9,10});
    assert(listToVector(rearrangeOddEven(ten)) == std::vector<int>({1,3,5,7,9,2,4,6,8,10}));
    deleteList(ten);

    // Test 8: Check doubly-linked structure integrity: forward and backward traversal.
    Node* list = createList({1,2,3,4,5});
    Node* rearranged = rearrangeOddEven(list);
    // Forward
    std::vector<int> fwd = listToVector(rearranged);
    assert(fwd == std::vector<int>({1,3,5,2,4}));
    // Backward from tail
    Node* tail = rearranged;
    while (tail->next) tail = tail->next;
    std::vector<int> rev;
    while (tail) {
        rev.push_back(tail->data);
        tail = tail->back;
    }
    std::vector<int> expected_rev = {4,2,5,3,1};
    assert(rev == expected_rev);
    deleteList(rearranged);

    return 0;
}
