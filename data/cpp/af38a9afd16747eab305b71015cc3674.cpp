/*
Write a C++ function named `findIntersectionValue` that takes two pointers to the head nodes of two singly linked lists (which may share a common suffix, i.e., they merge at some node and then continue together) and returns the integer data value stored at the first node that belongs to both lists. If the lists do not intersect (i.e., they have no common node), return -1. Assume each node stores a positive integer value and there are no cycles. The function should not modify the lists or allocate extra memory proportional to the list sizes; it must use only a constant amount of auxiliary space. The two lists are guaranteed to be non-empty.
*/

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Count the number of nodes in a linked list.
int listLength(const Node* head) {
    int length = 0;
    while (head != nullptr) {
        ++length;
        head = head->next;
    }
    return length;
}

// Return the data value of the intersection node, or -1 if no intersection.
int findIntersectionValue(Node* head1, Node* head2) {
    int len1 = listLength(head1);
    int len2 = listLength(head2);

    // Advance the longer list so both pointers are equidistant from the end.
    int diff = len1 - len2;
    Node* longer = head1;
    Node* shorter = head2;
    if (len2 > len1) {
        diff = len2 - len1;
        longer = head2;
        shorter = head1;
    }

    for (int i = 0; i < diff; ++i) {
        longer = longer->next;
    }

    // Traverse both lists together until they meet or one ends.
    while (longer != nullptr && shorter != nullptr) {
        if (longer == shorter) {
            return longer->data;
        }
        longer = longer->next;
        shorter = shorter->next;
    }

    return -1;
}

int main() {
    // Test 1: Lists intersect at node with data 8.
    Node* common = new Node(8);
    common->next = new Node(9);
    common->next->next = new Node(10);

    Node* head1 = new Node(1);
    head1->next = new Node(2);
    head1->next->next = common;

    Node* head2 = new Node(3);
    head2->next = common;

    assert(findIntersectionValue(head1, head2) == 8);

    // Test 2: Lists intersect at the first node of one list.
    Node* common2 = new Node(5);
    Node* h1b = common2;
    Node* h2b = new Node(4);
    h2b->next = common2;
    assert(findIntersectionValue(h1b, h2b) == 5);

    // Test 3: Lists of equal length intersect at a node.
    Node* common3 = new Node(7);
    Node* h1c = new Node(1);
    h1c->next = common3;
    Node* h2c = new Node(2);
    h2c->next = common3;
    assert(findIntersectionValue(h1c, h2c) == 7);

    // Test 4: No intersection.
    Node* h1d = new Node(1);
    h1d->next = new Node(2);
    Node* h2d = new Node(3);
    h2d->next = new Node(4);
    assert(findIntersectionValue(h1d, h2d) == -1);

    // Test 5: Different lengths, no intersection.
    Node* h1e = new Node(1);
    h1e->next = new Node(2);
    h1e->next->next = new Node(3);
    Node* h2e = new Node(4);
    assert(findIntersectionValue(h1e, h2e) == -1);
}

// The key observation is that if two lists intersect, they share all nodes from the intersection point to the end. Therefore, the difference in lengths between the two lists can be used to align them. First, compute the lengths of both lists. The longer list is advanced by the absolute difference in lengths, so that both remaining pointers are equidistant from the end and from the intersection point. Then move both pointers together step by step; the first time they point to the same node, that is the intersection, and its data value is returned. If no common node is found, traversal ends with both pointers reaching `nullptr`, and the function returns -1.
//
// Edge cases: (1) If one list is longer, handle the difference correctly by choosing the appropriate head as the “advanced” list. (2) The intersection point could be the very first node of one list if the other list starts there, which is handled naturally. (3) If the lists have equal length, no initial advancement is needed. (4) The function must not modify the lists or rely on any sentinel values. Time complexity is O(m+n) because we traverse each list once to compute lengths and then once more for the aligned traversal. Space complexity is O(1) because only a few constant-size variables are used.
