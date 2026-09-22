// Given a template-based singly linked list `Lista<T>` with support for insertion at head/tail, ordered insertion, deletion, searching, and a derived `Stack<T>` class, write a standalone C++ function `template <typename T> void reverseList(Lista<T>& list)` that reverses the order of the nodes in the list in-place (without allocating new nodes). The function should handle empty lists and single-element lists gracefully. The reversal must be efficient: it should visit each node at most once and use only a constant amount of auxiliary space (e.g., three temporary pointer variables). The function should preserve the list's contents and only change the pointers. After reversal, the original head becomes the tail, and the original tail becomes the new head. The function must be compatible with the provided `Lista` class interface (using `getHead()`, and traversing via `getSucc()`), and must correctly update the internal `head` pointer via the public interface (since `Lista` does not expose a setter for head; you may add a `setHead` public method or use a friend function approach, but for the task, assume you can modify the class or use a helper that accesses `head` — in the reference solution we'll add a `setHead` method).

The reversal of a singly linked list requires iterating through the list while adjusting the `succ` pointer of each node to point to its previous node. The main algorithm uses three pointers: `current` (starts at head), `next` (temporary to remember the next node before overwriting the link), and `prev` (starts as nullptr and becomes the new head at the end). For each node, save the next pointer, set `current->succ` to `prev`, then move `prev` to `current` and `current` to the saved `next`. After the loop, set the list's head to `prev`. Edge cases: empty list and single-element list (both return immediately since no pointer changes needed). Time complexity is O(n) where n is the number of nodes, always visiting every node exactly once. Space complexity is O(1) auxiliary, as only a constant number of pointers are used. The solution respects the existing class interface by using `getHead()` and `setHead()` (which we add to `Lista`), and traversing via `getSucc()` and `setSucc()` (which already exist). The `setHead` method is a minimal addition to the class, but the task specification allows modifying the given class if needed; in the free function, we can assume `setHead` is available (we'll provide the modified class in the solution for completeness, but the function itself is self-contained).

#include <iostream>

// Minimal modification to the provided Lista class: add a public setHead method.
// The rest of the class remains unchanged. For brevity, we show the essential parts.

template <typename T>
class Nodo {
    T x;
    Nodo<T>* succ;
    friend class Lista;
public:
    Nodo(T val) : x(val), succ(nullptr) {}
    T getX() const { return this->x; }
    Nodo<T>* getSucc() const { return this->succ; }
    void setX(T val) { this->x = val; }
    void setSucc(Nodo<T>* next) { this->succ = next; }
};

template <typename T>
class Lista {
    Nodo<T>* head;
public:
    Lista() : head(nullptr) {}
    bool isEmpty() const { return this->head == nullptr; }
    Nodo<T>* getHead() const { return this->head; }
    void setHead(Nodo<T>* newHead) { this->head = newHead; } // Added for task

    void insertHead(T x) {
        if (this->isEmpty()) { this->head = new Nodo<T>(x); return; }
        Nodo<T>* temp = new Nodo<T>(x);
        temp->setSucc(this->head);
        this->head = temp;
    }

    void insertTail(T x) {
        if (this->isEmpty()) { this->head = new Nodo<T>(x); return; }
        Nodo<T>* app = this->head;
        while (app->getSucc() != nullptr) app = app->getSucc();
        app->setSucc(new Nodo<T>(x));
    }

    void deleteHead() {
        if (this->isEmpty()) return;
        Nodo<T>* temp = this->head;
        this->head = this->head->getSucc();
        delete temp;
    }

    // Other methods (insertInOrder, deleteTail, deleteNode, ricerca) omitted for brevity
    // In the full solution they remain as in the original snippet.
};

// Solution function: reverse the list in-place.
template <typename T>
void reverseList(Lista<T>& list) {
    if (list.isEmpty() || list.getHead()->getSucc() == nullptr) return; // 0 or 1 nodes

    Nodo<T>* prev = nullptr;
    Nodo<T>* current = list.getHead();
    Nodo<T>* next = nullptr;

    while (current != nullptr) {
        next = current->getSucc(); // Save next node
        current->setSucc(prev);    // Reverse link
        prev = current;           // Move prev forward
        current = next;           // Move current forward
    }

    list.setHead(prev); // New head is the original tail
}

#include <cassert>

int main() {
    // Test 1: Empty list
    Lista<int> emptyList;
    reverseList(emptyList);
    assert(emptyList.isEmpty());

    // Test 2: Single element
    Lista<int> singleList;
    singleList.insertHead(42);
    reverseList(singleList);
    assert(singleList.getHead() != nullptr);
    assert(singleList.getHead()->getX() == 42);
    assert(singleList.getHead()->getSucc() == nullptr);

    // Test 3: Multiple elements (5,4,3,2,1)
    Lista<int> list;
    for (int i = 5; i >= 1; --i) list.insertHead(i); // inserts 1,2,3,4,5 as list head is 5
    reverseList(list);
    // After reversal, expected sequence: 5,4,3,2,1
    Nodo<int>* node = list.getHead();
    int expected[] = {5,4,3,2,1};
    for (int i = 0; i < 5; ++i) {
        assert(node != nullptr);
        assert(node->getX() == expected[i]);
        node = node->getSucc();
    }
    assert(node == nullptr);

    // Test 4: Two elements (1,2)
    Lista<int> twoList;
    twoList.insertHead(2);
    twoList.insertHead(1);
    reverseList(twoList);
    assert(twoList.getHead()->getX() == 2);
    assert(twoList.getHead()->getSucc()->getX() == 1);
    assert(twoList.getHead()->getSucc()->getSucc() == nullptr);

    // Test 5: List with duplicate values (3,3,3)
    Lista<int> dupList;
    dupList.insertHead(3);
    dupList.insertHead(3);
    dupList.insertHead(3);
    reverseList(dupList);
    Nodo<int>* d = dupList.getHead();
    int count = 0;
    while (d != nullptr) { assert(d->getX() == 3); d = d->getSucc(); count++; }
    assert(count == 3);

    // Test 6: Verify original head is now tail (checking last node's successor)
    Lista<int> verifyList;
    verifyList.insertHead(10);
    verifyList.insertHead(20);
    verifyList.insertHead(30);
    Nodo<int>* originalTail = verifyList.getHead()->getSucc()->getSucc(); // value 10
    reverseList(verifyList);
    Nodo<int>* newHead = verifyList.getHead();
    assert(newHead->getX() == 10);
    assert(newHead->getSucc()->getX() == 20);
    assert(newHead->getSucc()->getSucc()->getX() == 30);
    assert(newHead->getSucc()->getSucc()->getSucc() == nullptr);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
