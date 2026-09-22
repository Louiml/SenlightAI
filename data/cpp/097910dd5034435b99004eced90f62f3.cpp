/*
Write a standalone C++ function `bool isSubsetOf(const Set& a, const Set& b)` that returns `true` if every element in set `a` is also present in set `b`, and `false` otherwise. The `Set` class is provided below, but you may not modify it. The sets are represented as singly linked lists of integers with no duplicates, in no particular order. The function should handle empty sets: an empty set is a subset of any set (including another empty set). Do not use the existing `equivalent` method; implement your own logic using the public methods available in the `Set` class. The function must be `const`-correct (it should not modify either set). Assume that the list structure is always valid (no cycles, proper `next` pointers) and that each node’s `data` is an integer.
*/

#include <cstddef> // for nullptr

// Forward declaration of Node and Set as given in the snippet.
class Node {
public:
    Node() : data(0), next(nullptr) {}
    int data;
    Node* next;
};

class Set {
public:
    Set() : element(nullptr) {}
    int addElement(int e);
    Set* unions(Set* b);
    Set* intersetcions(Set* b);
    bool contains(int a) const;
    bool equivalent(Set* b) const;
    void listAll() const;
    // Public accessor to get the head pointer (not originally present but needed for testing).
    Node* getHead() const { return element; }

private:
    Node* element;
};

// The required free function: returns true if every element of a is in b.
bool isSubsetOf(const Set& a, const Set& b) {
    for (Node* cur = a.getHead(); cur != nullptr; cur = cur->next) {
        if (!b.contains(cur->data)) {
            return false;
        }
    }
    return true;
}

#include <cassert>

int main() {
    Set empty;
    Set a, b, c, d;

    // a = {1,2,3}
    a.addElement(1);
    a.addElement(2);
    a.addElement(3);

    // b = {1,2,3,4}
    b.addElement(1);
    b.addElement(2);
    b.addElement(3);
    b.addElement(4);

    // c = {2,4}
    c.addElement(2);
    c.addElement(4);

    // d = {1,5}
    d.addElement(1);
    d.addElement(5);

    // Test cases
    assert(isSubsetOf(a, b) == true);      // a is subset of b
    assert(isSubsetOf(b, a) == false);     // b is not subset of a
    assert(isSubsetOf(c, a) == false);     // 4 is not in a
    assert(isSubsetOf(c, b) == true);      // {2,4} is subset of {1,2,3,4}
    assert(isSubsetOf(d, b) == false);     // 5 is not in b
    assert(isSubsetOf(empty, a) == true);  // empty set is subset of any set
    assert(isSubsetOf(empty, empty) == true); // empty is subset of empty
    assert(isSubsetOf(a, empty) == false); // non-empty is not subset of empty
    assert(isSubsetOf(a, a) == true);      // set is subset of itself

    return 0;
}

// The solution iterates through all elements of set `a` and checks whether each value exists in set `b` using the public `contains` method of `Set`. If any element of `a` is not found in `b`, the function returns `false`. If the entire list of `a` is traversed without a miss, return `true`. Edge cases: if `a` is empty (its `element` pointer is `nullptr`), the loop does not run and the function returns `true` immediately, which is correct since the empty set is a subset of any set. If `b` is empty but `a` is non‑empty, the loop will encounter a missing element and return `false`. Time complexity is \(O(n \cdot m)\), where \(n\) is the number of elements in `a` and \(m\) is the number of elements in `b`, because each `contains` call scans `b` linearly from the head. Space complexity is \(O(1)\) extra space (only a few pointer variables), not counting the memory used by the list nodes themselves.
