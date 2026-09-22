/*
Write a C++ function that takes a doubly linked list (defined by its head pointer and tail pointer) and returns a `std::pair<std::string, std::string>` where the first string is the forward traversal from head to tail (values separated by single spaces, no trailing space), and the second string is the reverse traversal from tail to head (also space-separated). The list is guaranteed to be non-empty. The function must handle a single-node list correctly (both strings should contain just that one value). Do not modify the list. Use `const` pointers for traversal to ensure the list is not altered.
*/

#include <string>
#include <sstream>

struct Node {
    int val;
    Node* next;
    Node* pre;
    explicit Node(int v) : val(v), next(nullptr), pre(nullptr) {}
};

// Return a pair: first = forward traversal (head->tail), second = reverse traversal (tail->head)
std::pair<std::string, std::string> traverseBothWays(const Node* head, const Node* tail) {
    std::ostringstream forward;
    std::ostringstream reverse;

    const Node* cur = head;
    bool first = true;
    while (cur != nullptr) {
        if (!first) forward << " ";
        forward << cur->val;
        first = false;
        cur = cur->next;
    }

    cur = tail;
    first = true;
    while (cur != nullptr) {
        if (!first) reverse << " ";
        reverse << cur->val;
        first = false;
        cur = cur->pre;
    }

    return {forward.str(), reverse.str()};
}

#include <cassert>
#include <string>
#include <utility>

int main() {
    // List: 10 <-> 20 <-> 30
    Node* n1 = new Node(10);
    Node* n2 = new Node(20);
    Node* n3 = new Node(30);
    n1->next = n2; n2->pre = n1;
    n2->next = n3; n3->pre = n2;
    Node* head = n1;
    Node* tail = n3;

    auto result = traverseBothWays(head, tail);
    assert(result.first == "10 20 30");
    assert(result.second == "30 20 10");

    // Single node list
    Node* single = new Node(42);
    auto result2 = traverseBothWays(single, single);
    assert(result2.first == "42");
    assert(result2.second == "42");

    // Two-node list
    Node* m1 = new Node(-5);
    Node* m2 = new Node(7);
    m1->next = m2; m2->pre = m1;
    auto result3 = traverseBothWays(m1, m2);
    assert(result3.first == "-5 7");
    assert(result3.second == "7 -5");

    // List with negative and larger numbers
    Node* p1 = new Node(100);
    Node* p2 = new Node(-100);
    Node* p3 = new Node(0);
    p1->next = p2; p2->pre = p1;
    p2->next = p3; p3->pre = p2;
    auto result4 = traverseBothWays(p1, p3);
    assert(result4.first == "100 -100 0");
    assert(result4.second == "0 -100 100");

    // Cleanup (not strictly necessary for test but good practice)
    delete n3; delete n2; delete n1;
    delete single;
    delete m2; delete m1;
    delete p3; delete p2; delete p1;

    return 0;
}

// The solution requires two separate traversals of the doubly linked list. For the forward string, start at `head` and follow `next` pointers until `nullptr`. For the reverse string, start at `tail` and follow `pre` pointers until `nullptr`. Build each string using `std::ostringstream` or manual concatenation. Because the list is non-empty, each traversal will produce at least one value. Edge cases: single-node list where `head == tail`, and both loops must handle it correctly—the forward loop outputs the value then stops (since `next == nullptr`), and the reverse loop outputs the same value then stops. Time complexity is `O(n)` where `n` is the number of nodes, because each traversal visits each node exactly once. Space complexity is `O(n)` in total for the two output strings (since they store all values), but auxiliary space excluding the output is `O(1)`.
