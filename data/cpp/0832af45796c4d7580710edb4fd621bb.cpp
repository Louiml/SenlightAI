Write a C++ function `segregateEvenOdd` that takes the head of a singly linked list of integers and returns the head of a new linked list containing the same nodes, but with all even-valued nodes appearing before all odd-valued nodes. The relative order among the even nodes and among the odd nodes must be preserved. The function should not allocate any new nodes; it must reuse the existing nodes from the input list, and it must return the head of the rearranged list. The input list may be empty or contain one node, and the function should handle those cases gracefully. Assume the node structure is defined as in the problem statement (a `Node` class with `int data` and `Node* next`).

// The solution uses two dummy nodes to act as temporary heads for the even and odd sublists. A single traversal of the original list is performed, and for each node, we check whether its `data` value is even or odd. If even, we append it to the end of the even sublist; if odd, we append it to the end of the odd sublist. This preserves the relative order of nodes because we append in the order they appear. After the traversal, we connect the even sublist’s tail to the head of the odd sublist (skipping the dummy), and set the odd sublist’s tail’s `next` to `nullptr` to terminate the list. If there are no even or no odd nodes, the dummy heads correctly handle the cases, and we return the first real node of the even sublist (or odd sublist if no evens exist, but since evens are placed first, the return should be the head of the even list; if no evens exist, the even sublist is empty, and we return the odd sublist’s real head). Edge cases include an empty list (`head == nullptr`), a single node, and lists where all nodes are even or all are odd. Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) beyond the dummy nodes (which are constant). Note: The function should return the head of the rearranged list, and since we reuse nodes, the original list is modified.

#include <cstddef>

// Node definition for a singly linked list.
struct Node {
    int data;
    Node* next;
    Node() : data(0), next(nullptr) {}
    Node(int x) : data(x), next(nullptr) {}
    Node(int x, Node* next) : data(x), next(next) {}
};

// Segregates even-valued nodes before odd-valued nodes, preserving order.
// Returns the head of the rearranged list. Reuses existing nodes.
Node* segregateEvenOdd(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Node evenDummy(0);   // dummy head for even sublist
    Node oddDummy(0);    // dummy head for odd sublist
    Node* evenTail = &evenDummy;
    Node* oddTail = &oddDummy;

    Node* curr = head;
    while (curr != nullptr) {
        if ((curr->data) % 2 == 0) {
            evenTail->next = curr;
            evenTail = curr;
        } else {
            oddTail->next = curr;
            oddTail = curr;
        }
        curr = curr->next;
    }

    // Connect even sublist to odd sublist (skipping dummy).
    evenTail->next = oddDummy.next;
    // Terminate the final list.
    if (oddDummy.next != nullptr) {
        oddTail->next = nullptr;
    }

    return (evenDummy.next != nullptr) ? evenDummy.next : oddDummy.next;
}

#include <cassert>

// Helper to create a linked list from a vector.
Node* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    Node* head = new Node(values[0]);
    Node* curr = head;
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new Node(values[i]);
        curr = curr->next;
    }
    return head;
}

// Helper to extract values from a linked list.
std::vector<int> listToVector(const Node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to delete a linked list.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Mixed even and odd, preserving order.
    Node* list1 = createList({1, 2, 3, 4, 5});
    Node* result1 = segregateEvenOdd(list1);
    assert(listToVector(result1) == std::vector<int>({2, 4, 1, 3, 5}));
    deleteList(result1);

    // Test 2: All even.
    Node* list2 = createList({2, 4, 6});
    Node* result2 = segregateEvenOdd(list2);
    assert(listToVector(result2) == std::vector<int>({2, 4, 6}));
    deleteList(result2);

    // Test 3: All odd.
    Node* list3 = createList({1, 3, 5});
    Node* result3 = segregateEvenOdd(list3);
    assert(listToVector(result3) == std::vector<int>({1, 3, 5}));
    deleteList(result3);

    // Test 4: Single even node.
    Node* list4 = createList({8});
    Node* result4 = segregateEvenOdd(list4);
    assert(listToVector(result4) == std::vector<int>({8}));
    deleteList(result4);

    // Test 5: Single odd node.
    Node* list5 = createList({7});
    Node* result5 = segregateEvenOdd(list5);
    assert(listToVector(result5) == std::vector<int>({7}));
    deleteList(result5);

    // Test 6: Empty list.
    Node* list6 = nullptr;
    Node* result6 = segregateEvenOdd(list6);
    assert(result6 == nullptr);

    // Test 7: Even then odd already ordered.
    Node* list7 = createList({2, 4, 1, 3});
    Node* result7 = segregateEvenOdd(list7);
    assert(listToVector(result7) == std::vector<int>({2, 4, 1, 3}));
    deleteList(result7);

    // Test 8: Odd then even (reverse order).
    Node* list8 = createList({1, 2, 3, 4});
    Node* result8 = segregateEvenOdd(list8);
    assert(listToVector(result8) == std::vector<int>({2, 4, 1, 3}));
    deleteList(result8);

    return 0;
}
