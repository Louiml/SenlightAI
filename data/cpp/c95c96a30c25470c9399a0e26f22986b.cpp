// Implement a C++ function `doublyLinkedListInfo` that takes a pointer to the head of a doubly linked list (nodes have `data`, `prev`, and `next` members) and returns a `std::pair<int, int>` where the first element is the length of the list and the second element is the sum of all node values. The function must work correctly for an empty list (return `{0, 0}`) and handle lists of any size, including with negative values. The function should be `const`-correct (taking a `const Node*` head) and should not modify the list. Use the provided `Node` class structure as given, but do not rely on any global variables or helper functions.

// The solution requires traversing the doubly linked list from the head to the tail, counting nodes and accumulating their data values. Since it is a doubly linked list, we could traverse either direction, but starting from head and moving via `next` pointers is simplest. Handle the edge case where `head` is `nullptr` (empty list) by returning `{0, 0}`. For each node, increment a counter and add the node's data to a running sum. The traversal stops when we reach `nullptr`. Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) excluding the input list itself. The function should be declared as taking `const Node* head` to indicate it does not modify the list, and the `Node` class members should be accessed read-only.

#include <utility>  // for std::pair

// Node structure as provided in the snippet
class Node {
public:
    int data;
    Node* prev;
    Node* next;
    Node(int d) : data(d), prev(nullptr), next(nullptr) {}
};

// Returns {length, sum} of a doubly linked list
std::pair<int, int> doublyLinkedListInfo(const Node* head) {
    int length = 0;
    int sum = 0;
    const Node* current = head;
    while (current != nullptr) {
        length++;
        sum += current->data;
        current = current->next;
    }
    return {length, sum};
}

#include <cassert>
#include <utility>

int main() {
    // Empty list
    Node* head1 = nullptr;
    auto res1 = doublyLinkedListInfo(head1);
    assert(res1.first == 0 && res1.second == 0);

    // Single node
    Node* n2 = new Node(5);
    auto res2 = doublyLinkedListInfo(n2);
    assert(res2.first == 1 && res2.second == 5);

    // Multiple nodes with mixed values
    Node* n3a = new Node(10);
    Node* n3b = new Node(-3);
    Node* n3c = new Node(7);
    n3a->next = n3b; n3b->prev = n3a;
    n3b->next = n3c; n3c->prev = n3b;
    auto res3 = doublyLinkedListInfo(n3a);
    assert(res3.first == 3 && res3.second == 14);

    // All negative values
    Node* n4a = new Node(-2);
    Node* n4b = new Node(-5);
    n4a->next = n4b; n4b->prev = n4a;
    auto res4 = doublyLinkedListInfo(n4a);
    assert(res4.first == 2 && res4.second == -7);

    // Long list (1000 nodes with values 1..1000)
    Node* head5 = new Node(1);
    Node* tail5 = head5;
    for (int i = 2; i <= 1000; ++i) {
        Node* node = new Node(i);
        tail5->next = node;
        node->prev = tail5;
        tail5 = node;
    }
    auto res5 = doublyLinkedListInfo(head5);
    assert(res5.first == 1000 && res5.second == 500500);

    // Clean up (not required for test but good practice)
    // Note: Because of the destructor in the original code, this would cause issues,
    // so we skip actual deletion to keep the test simple.
}
