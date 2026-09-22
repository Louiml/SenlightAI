Implement a C++ function that manages a singly linked list of records, where each record stores an unsigned integer `index` and an unsigned integer `MADG`. The function must insert new records at the tail of the list (preserving insertion order) and then search for a record by its `index` value using a binary search algorithm designed for a sorted linked list. The input list is assumed to be sorted by `index` in ascending order. The function should return a pointer to the found node, or `nullptr` if no such record exists. After the search, the function must deallocate all list nodes to prevent memory leaks. To make this self-contained, design a struct `Node` with fields `index`, `MADG`, and a `next` pointer, and a struct `List` with `head` and `tail` pointers. The solution must implement the insertion, search, and cleanup in a single free function or a set of helper functions, but the main exported function should be `Node* processList(List& list, unsigned int targetIndex)`, which performs insertion of a single record (index=targetIndex, MADG=0) at the tail, then searches for the target index, and finally clears all nodes. If the record already exists (i.e., a node with the same index is found), do not insert a duplicate; just search and clean. The function returns the pointer to the found node before cleanup. The list may be empty initially, and the search must handle empty lists.
#include <cassert>

int main() {
    // Test 1: Empty list, insert and find
    {
        List l{nullptr, nullptr};
        Node* found = processList(l, 5);
        assert(found != nullptr);
        assert(found->index == 5);
        assert(found->MADG == 0);
        assert(l.head == nullptr); // after cleanup
    }

    // Test 2: List with one element (pre-populated manually) and search existing
    {
        List l;
        l.head = new Node{10, 100, nullptr};
        l.tail = l.head;
        Node* found = processList(l, 10);
        assert(found != nullptr && found->index == 10 && found->MADG == 100);
        assert(l.head == nullptr);
    }

    // Test 3: List with several elements, target exists
    {
        List l;
        Node* a = new Node{1, 10, nullptr};
        Node* b = new Node{3, 30, nullptr};
        Node* c = new Node{5, 50, nullptr};
        a->next = b; b->next = c;
        l.head = a; l.tail = c;
        Node* found = processList(l, 3);
        assert(found != nullptr && found->index == 3 && found->MADG == 30);
        assert(l.head == nullptr);
    }

    // Test 4: List with elements, target not exists -> inserted then found (MADG=0)
    {
        List l;
        Node* a = new Node{2, 20, nullptr};
        Node* b = new Node{4, 40, nullptr};
        a->next = b;
        l.head = a; l.tail = b;
        Node* found = processList(l, 6);
        assert(found != nullptr && found->index == 6 && found->MADG == 0);
        assert(l.head == nullptr);
    }

    // Test 5: Empty list, target 0
    {
        List l{nullptr, nullptr};
        Node* found = processList(l, 0);
        assert(found != nullptr && found->index == 0 && found->MADG == 0);
        assert(l.head == nullptr);
    }
}
#include <iostream>

struct Node {
    unsigned int index;
    unsigned int MADG;
    Node* next;
};

struct List {
    Node* head;
    Node* tail;
};

// Helper: create a new node
Node* createNode(unsigned int idx, unsigned int madg) {
    Node* p = new Node{idx, madg, nullptr};
    return p;
}

// Helper: add to tail (assumes list is sorted, used only when node not existing)
void addTail(List& l, unsigned int idx, unsigned int madg) {
    Node* p = createNode(idx, madg);
    if (l.head == nullptr) {
        l.head = l.tail = p;
    } else {
        l.tail->next = p;
        l.tail = p;
    }
}

// Helper: find middle node between start and last (last may be nullptr)
Node* middle(Node* start, Node* last) {
    if (start == nullptr) return nullptr;
    Node* slow = start;
    Node* fast = start->next;
    while (fast != last) {
        fast = fast->next;
        if (fast != last) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    return slow;
}

// Helper: binary search on sorted singly linked list
Node* binarySearch(const List& l, unsigned int target) {
    Node* start = l.head;
    Node* last = nullptr;
    while (true) {
        Node* mid = middle(start, last);
        if (mid == nullptr) return nullptr;
        if (mid->index == target) return mid;
        else if (mid->index < target) start = mid->next;
        else last = mid;
        if (last == nullptr || last->next == start) break;
    }
    return nullptr;
}

// Helper: clear all nodes
void clearList(List& l) {
    Node* p = l.head;
    while (p != nullptr) {
        Node* next = p->next;
        delete p;
        p = next;
    }
    l.head = l.tail = nullptr;
}

// Main exported function: ensure target exists (if not, add with MADG=0),
// search for it, then clean up. Returns pointer to found node before cleanup.
Node* processList(List& list, unsigned int targetIndex) {
    // If list is empty or target not found, insert new node at tail
    if (list.head == nullptr || binarySearch(list, targetIndex) == nullptr) {
        addTail(list, targetIndex, 0);
    }
    // Now guaranteed to exist
    Node* found = binarySearch(list, targetIndex);
    // Save pointer, then clear
    Node* result = found; // note: after clear, this pointer becomes dangling; but we return it.
    clearList(list);
    return result;
}
// The key challenge is implementing a binary search on a singly linked list without random access. Use the two-pointer technique to find the middle node between two boundaries (`start` and `last`). Initially, `last` is `nullptr` to indicate the end of the list; as the search narrows, `last` becomes a node pointer. The middle function moves a slow pointer one step and a fast pointer two steps until the fast pointer reaches `last`; this yields the middle node. The binary search loop repeatedly finds the middle between `start` and `last`. If the middle's index equals the target, return it. If middle's index is less than the target, move `start` to `mid->next`; otherwise, set `last` to `mid`. The loop continues until `last != nullptr && last->next == start` (meaning no more elements to search). This works because the list is sorted by index. Edge cases: empty list (head is null, directly return nullptr). Duplicate indices are disallowed per the problem (assume sorted unique; if duplicates exist, the first found is returned). Time complexity: O(log n) for the search with O(n) for middle computations per step (since finding middle takes O(n) in a singly list), so overall O(n log n) worst-case; insertion is O(1) with tail pointer; cleanup is O(n). Space complexity O(1) auxiliary. To avoid duplicate insertion, before inserting, search for the target; if found, skip insertion. Then perform the required search (or reuse the found pointer) and cleanup.
