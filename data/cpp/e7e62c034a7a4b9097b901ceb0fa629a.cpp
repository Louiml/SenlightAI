// Write a standalone C++ function that takes a reference to a `std::vector<int>` representing the elements of a doubly linked list (in order from head to tail) and returns a new `std::vector<int>` that is the result of performing the following operations sequentially: delete the head node, delete the tail node, and then insert a new node with value `-1` after the current head node. If during any operation the list becomes empty or the required node does not exist (e.g., deleting tail when list is empty, or inserting after head when list is empty after deletions), the function should stop and return the current state of the list as a vector, without performing further operations. The function must replicate the exact behavior of the provided `DoublyLinkedList` methods (`DeleteHead`, `DeleteTail`, and `InsertAfterNode`), including the edge-case handling where operations are silently ignored on empty lists or when nodes are null. The input vector may be empty. The function should not modify the input vector; it should build and manipulate a local doubly linked list.
#include <cassert>
#include <vector>

// Function under test is declared above

int main() {
    // Empty input → no operations, empty result
    assert(processList({}) == std::vector<int>());

    // Single element → DeleteHead empties list, rest skipped
    assert(processList({42}) == std::vector<int>());

    // Two elements → DeleteHead leaves one, DeleteTail removes it, Insert skipped
    assert(processList({1, 2}) == std::vector<int>());

    // Three elements → DeleteHead leaves {2,3}, DeleteTail leaves {2}, Insert -1 after head gives {2,-1}
    assert(processList({1, 2, 3}) == (std::vector<int>{2, -1}));

    // Four elements → DeleteHead leaves {2,3,4}, DeleteTail leaves {2,3}, Insert -1 gives {2,-1,3}
    assert(processList({5, 6, 7, 8}) == (std::vector<int>{6, -1, 7}));

    // Larger list to verify general behavior
    // Original: [10,20,30,40,50] → DeleteHead → [20,30,40,50] → DeleteTail → [20,30,40] → Insert -1 after head → [20,-1,30,40]
    assert(processList({10, 20, 30, 40, 50}) == (std::vector<int>{20, -1, 30, 40}));

    // Negative values
    // [-3,-2,-1] → DeleteHead → [-2,-1] → DeleteTail → [-2] → Insert -1 → [-2,-1]
    assert(processList({-3, -2, -1}) == (std::vector<int>{-2, -1}));

    return 0;
}
#include <vector>

// Minimal doubly linked list node
struct Node {
    int data;
    Node* next;
    Node* prev;
    Node(int d, Node* n = nullptr, Node* p = nullptr) : data(d), next(n), prev(p) {}
};

// Applies DeleteHead, DeleteTail, then InsertAfterNode(head, -1) to a list built from input.
std::vector<int> processList(const std::vector<int>& input) {
    // Build doubly linked list from input vector
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int val : input) {
        Node* newNode = new Node(val);
        if (tail) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            head = tail = newNode;
        }
    }

    // Operation 1: DeleteHead
    if (head != nullptr) {
        if (head == tail) {
            // single node
            delete head;
            head = tail = nullptr;
        } else {
            Node* toDelete = head;
            head = head->next;
            head->prev = nullptr;
            delete toDelete;
        }
    }

    // Operation 2: DeleteTail
    if (tail != nullptr) {
        if (head == tail) {
            // single node (or maybe head==tail after deletion? handled above, but keep safe)
            delete tail;
            head = tail = nullptr;
        } else {
            Node* toDelete = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete toDelete;
        }
    }

    // Operation 3: InsertAfterNode(head, -1) if list is not empty
    if (head != nullptr) {
        Node* newNode = new Node(-1, head->next, head);
        head->next = newNode;
    }

    // Collect result
    std::vector<int> result;
    Node* current = head;
    while (current) {
        result.push_back(current->data);
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
    }

    return result;
}
// The solution must implement a minimal doubly linked list with `Node` structures containing `data`, `next`, and `prev` pointers. The algorithm first builds the linked list from the input vector by appending nodes to the tail, maintaining both `head` and `tail` pointers. Then it applies the three operations in order, following the exact edge-case logic from the snippet:
//
// 1. **DeleteHead**: If the list is empty, do nothing (return current list). Otherwise, move `head` to `head->next` and set `head->prev` to `nullptr`. If the list had only one element, `head` becomes null, but the snippet does not explicitly update `tail` in `DeleteHead`—however, in the snippet, `DeleteHead` is broken because it doesn't handle the single-node case (it would dereference null). To faithfully replicate the intended behavior, we must treat the single-node case carefully: after deletion, both `head` and `tail` become null. This is consistent with the `DeleteNthNode` and `DeleteNode` implementations that explicitly handle `head == tail`. For `DeleteHead`, we add that safety check: if `head == tail`, set both to null.
//
// 2. **DeleteTail**: If list is empty, do nothing. If list has exactly one node (i.e., `head == tail`), set both `head` and `tail` to null. Otherwise, move `tail` to `tail->prev` and set `tail->next` to null. Also delete the removed node to avoid memory leaks.
//
// 3. **InsertAfterNode(prevNode, -1)**: If the list is empty, do nothing (the snippet checks `IsEmpty()` and returns). Otherwise, create a new node with `data = -1`, set `next = prevNode->next` and `prev = prevNode`, then set `prevNode->next = newNode`. If `prevNode` is the current head, the new node becomes the second element. Note: if the list becomes empty after deletions, `InsertAfterNode` is skipped.
//
// After processing all three operations, we traverse the list from head to tail and collect the data into a result vector, then clean up all allocated nodes.
//
// Edge cases: empty input vector → all operations are skipped, result is empty. Input with one element → after `DeleteHead`, list becomes empty, so `DeleteTail` and `InsertAfterNode` are skipped, result is empty. Input with two elements → `DeleteHead` leaves one node, `DeleteTail` deletes it (empty), `InsertAfterNode` skipped. Input with three or more elements works normally.
//
// Time complexity: Building the list is O(n), each operation is O(1) (except `InsertAfterNode` is O(1) given a node pointer), and traversal for output is O(n). Total O(n). Space complexity: O(n) for the linked list nodes.
