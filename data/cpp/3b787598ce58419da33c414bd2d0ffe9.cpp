// Write a C++ function `template <typename T> LinkedList<T> mergeSortedLinkedLists(const LinkedList<T>& A, const LinkedList<T>& B)` that takes two sorted singly linked lists (each in non-decreasing order) and returns a new linked list containing all elements from both lists in sorted order. The function must not modify the input lists and must work for any data type that supports `operator<` (or `operator>`). You may use the provided `Node<T>` and `LinkedList<T>` class definitions exactly as given in the snippet; your solution must be a free function (outside the class) that accesses the private `first` pointer via a public accessor method (you may add such a method to the class). The merged list must be a new list with new nodes; do not reuse nodes from the inputs. Assume the input lists are non-empty and already sorted. Handle duplicate values correctly (all duplicates appear in the merged list). The function must return the merged list by value. Provide a detailed explanation of the algorithm, edge cases (empty lists, lists with one element, duplicates), and time/space complexity.

The main idea is to use a standard merge algorithm for sorted sequences, similar to merging two sorted arrays, but adapted for linked lists. We traverse both input lists simultaneously using two pointers, `pA` and `pB`, starting from their heads. At each step, we compare `pA->data` and `pB->data`, create a new node for the smaller (or equal) value, append it to the tail of the result list, and advance the corresponding pointer. If both values are equal, we can choose either one first; here, we choose the element from `A` when equal to maintain stable order (though order of duplicates doesn't matter). After one list is exhausted, we copy all remaining nodes from the other list. Since we must not modify the input lists, we cannot simply link the remaining nodes; we must create new copies of each node. The edge cases include when one list is empty (return a copy of the other), and when both lists have exactly one element. The algorithm runs in O(n+m) time, where n and m are the lengths of the inputs, because each node is visited exactly once. Space complexity is O(n+m) because we allocate exactly one new node per input node for the result. We need access to the `first` pointer of both input lists; since the class has private `first`, we add a public `getHead()` method returning a copy of the pointer. Also, to build the result, we need to set the next pointers, but we don't have a setter; we can either add a `setNext` method or make the free function a friend. For simplicity, we add a public `getFirst()` accessor and use the existing public `Insert` method? No, Insert is O(n) per insertion and would be O(n^2). Better to add a friend or a public method to get/set next. The provided class already has a public `Display`, `Insert`, `Delete`, `Length`, and the constructor. We can add a public method `Node<T>* getFirst() const` and `void setFirst(Node<T>* p)` for simplicity. Alternatively, we can create a helper function inside the class. The reference solution will add such accessors.

#include <iostream>

template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
};

template <typename T>
class LinkedList {
private:
    Node<T>* first;
public:
    LinkedList() : first(nullptr) {}
    LinkedList(const T A[], int n) {
        first = new Node<T>;
        first->data = A[0];
        first->next = nullptr;
        Node<T>* last = first;
        for (int i = 1; i < n; ++i) {
            Node<T>* t = new Node<T>;
            t->data = A[i];
            t->next = nullptr;
            last->next = t;
            last = t;
        }
    }
    ~LinkedList() {
        Node<T>* temp = first;
        while (first) {
            first = first->next;
            delete temp;
            temp = first;
        }
    }
    // Accessor for external merging function
    Node<T>* getFirst() const { return first; }
    // Mutator for building result list
    void setFirst(Node<T>* p) { first = p; }
};

// Free function to merge two sorted linked lists without modifying inputs
template <typename T>
LinkedList<T> mergeSortedLinkedLists(const LinkedList<T>& A, const LinkedList<T>& B) {
    Node<T>* pA = A.getFirst();
    Node<T>* pB = B.getFirst();
    
    // If either list is empty, return a copy of the other
    if (!pA) {
        LinkedList<T> result;
        Node<T>* dummy = new Node<T>; // temporary to build list
        Node<T>* tail = dummy;
        while (pB) {
            tail->next = new Node<T>{pB->data, nullptr};
            tail = tail->next;
            pB = pB->next;
        }
        result.setFirst(dummy->next);
        delete dummy;
        return result;
    }
    if (!pB) {
        LinkedList<T> result;
        Node<T>* dummy = new Node<T>;
        Node<T>* tail = dummy;
        while (pA) {
            tail->next = new Node<T>{pA->data, nullptr};
            tail = tail->next;
            pA = pA->next;
        }
        result.setFirst(dummy->next);
        delete dummy;
        return result;
    }
    
    // General case: merge with new nodes
    LinkedList<T> result;
    Node<T>* dummy = new Node<T>; // dummy head to simplify
    Node<T>* tail = dummy;
    
    while (pA && pB) {
        if (pA->data <= pB->data) {
            tail->next = new Node<T>{pA->data, nullptr};
            tail = tail->next;
            pA = pA->next;
        } else {
            tail->next = new Node<T>{pB->data, nullptr};
            tail = tail->next;
            pB = pB->next;
        }
    }
    // Copy remaining nodes from A
    while (pA) {
        tail->next = new Node<T>{pA->data, nullptr};
        tail = tail->next;
        pA = pA->next;
    }
    // Copy remaining nodes from B
    while (pB) {
        tail->next = new Node<T>{pB->data, nullptr};
        tail = tail->next;
        pB = pB->next;
    }
    
    result.setFirst(dummy->next);
    delete dummy;
    return result;
}

#include <cassert>
#include <iostream>

// Assume the template definitions from the solution are included above.

int main() {
    // Test case 1: Basic merge
    int a1[] = {1, 3, 5};
    int b1[] = {2, 4, 6};
    LinkedList<int> A(a1, 3);
    LinkedList<int> B(b1, 3);
    LinkedList<int> merged1 = mergeSortedLinkedLists(A, B);
    int expected1[] = {1, 2, 3, 4, 5, 6};
    Node<int>* p = merged1.getFirst();
    for (int i = 0; i < 6; ++i) {
        assert(p != nullptr);
        assert(p->data == expected1[i]);
        p = p->next;
    }
    assert(p == nullptr);

    // Test case 2: Duplicate values
    int a2[] = {1, 2, 2, 3};
    int b2[] = {2, 3, 4};
    LinkedList<int> A2(a2, 4);
    LinkedList<int> B2(b2, 3);
    LinkedList<int> merged2 = mergeSortedLinkedLists(A2, B2);
    int expected2[] = {1, 2, 2, 2, 3, 3, 4};
    p = merged2.getFirst();
    for (int i = 0; i < 7; ++i) {
        assert(p != nullptr);
        assert(p->data == expected2[i]);
        p = p->next;
    }
    assert(p == nullptr);

    // Test case 3: One list empty (B empty)
    int a3[] = {5, 10};
    LinkedList<int> A3(a3, 2);
    int b3[] = {0};  // dummy, but we create empty by passing n=0? Constructor requires n>0, so create single then delete? Simpler: create B with size 0 is not allowed by constructor. We'll test with a list that is empty by constructing then deleting? Better: modify to use the defined constructor can't do empty. So we'll test by creating a list with one element and then deleting? That's not empty. We'll just test the merge with one list having length 1 and other length 1, and separately test the case when we pass an empty list? But the constructor doesn't support 0 elements. To test empty, we can manually create a LinkedList with default constructor and then setFirst(nullptr). So we'll do that.
    LinkedList<int> A3(a3, 2);
    LinkedList<int> B3; // default constructor sets first = nullptr
    LinkedList<int> merged3 = mergeSortedLinkedLists(A3, B3);
    int expected3[] = {5, 10};
    p = merged3.getFirst();
    for (int i = 0; i < 2; ++i) {
        assert(p != nullptr);
        assert(p->data == expected3[i]);
        p = p->next;
    }
    assert(p == nullptr);

    // Test case 4: Both empty
    LinkedList<int> A4;
    LinkedList<int> B4;
    LinkedList<int> merged4 = mergeSortedLinkedLists(A4, B4);
    assert(merged4.getFirst() == nullptr);

    // Test case 5: Single element each
    int a5[] = {1};
    int b5[] = {0};
    LinkedList<int> A5(a5, 1);
    LinkedList<int> B5(b5, 1);
    LinkedList<int> merged5 = mergeSortedLinkedLists(A5, B5);
    int expected5[] = {0, 1};
    p = merged5.getFirst();
    for (int i = 0; i < 2; ++i) {
        assert(p != nullptr);
        assert(p->data == expected5[i]);
        p = p->next;
    }
    assert(p == nullptr);

    // Test double type
    double a6[] = {1.5, 2.5};
    double b6[] = {1.0, 3.0};
    LinkedList<double> A6(a6, 2);
    LinkedList<double> B6(b6, 2);
    LinkedList<double> merged6 = mergeSortedLinkedLists(A6, B6);
    double expected6[] = {1.0, 1.5, 2.5, 3.0};
    Node<double>* pd = merged6.getFirst();
    for (int i = 0; i < 4; ++i) {
        assert(pd != nullptr);
        assert(pd->data == expected6[i]);
        pd = pd->next;
    }
    assert(pd == nullptr);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
