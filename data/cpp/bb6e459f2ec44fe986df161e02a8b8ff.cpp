// Write a C++ function that takes a singly linked list as input, detects whether it contains a cycle (a loop where a node’s `next` pointer eventually points back to an earlier node), and if so, removes the cycle by setting the `next` pointer of the last node in the cycle (the node whose `next` points to the cycle’s start) to `nullptr`. The function should return the original head of the list after the loop is removed. If there is no cycle, the list must remain unchanged. The function must handle edge cases such as an empty list, a single node without a cycle, and a cycle that starts at the head node itself. You may assume the list uses a `Node` struct with an integer `data` field and a `Node* next` pointer.

#include <cassert>

// Helper to create a linked list from an array and optionally create a cycle.
Node* createList(int* arr, int size, int cycleIndex = -1) {
    if (size == 0) return nullptr;
    Node* head = new Node(arr[0]);
    Node* current = head;
    Node* cycleNode = nullptr;
    if (cycleIndex == 0) cycleNode = head;
    for (int i = 1; i < size; ++i) {
        current->next = new Node(arr[i]);
        current = current->next;
        if (i == cycleIndex) cycleNode = current;
    }
    if (cycleNode != nullptr) current->next = cycleNode;
    return head;
}

// Helper to count nodes in acyclic list (after removal).
int countNodes(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

// Helper to free acyclic list.
void freeList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Empty list
    Node* empty = nullptr;
    assert(removeLoop(empty) == nullptr);

    // Test 2: Single node, no cycle
    Node* single = new Node(5);
    assert(removeLoop(single) == single);
    assert(single->next == nullptr);
    delete single;

    // Test 3: No cycle, multiple nodes
    int arr1[] = {1, 2, 3, 4};
    Node* list1 = createList(arr1, 4);
    Node* result1 = removeLoop(list1);
    assert(result1 == list1);
    assert(countNodes(result1) == 4);
    freeList(result1);

    // Test 4: Cycle in the middle (nodes 1->2->3->4->2)
    int arr2[] = {1, 2, 3, 4};
    Node* list2 = createList(arr2, 4, 1); // cycle back to node with value 2
    Node* result2 = removeLoop(list2);
    assert(countNodes(result2) == 4);
    // Verify no cycle by traversing and checking cannot revisit nodes
    Node* current = result2;
    int steps = 0;
    while (current != nullptr) {
        current = current->next;
        ++steps;
        assert(steps <= 4); // traversing too far indicates a cycle
    }
    freeList(result2);

    // Test 5: Cycle starting at head (1->2->3->1)
    int arr3[] = {1, 2, 3};
    Node* list3 = createList(arr3, 3, 0);
    Node* result3 = removeLoop(list3);
    assert(countNodes(result3) == 3);
    // The cycle should be broken, so the last node's next is null
    Node* tail = result3;
    while (tail->next != nullptr) tail = tail->next;
    assert(tail->next == nullptr);
    freeList(result3);

    // Test 6: Single node with self-loop
    Node* selfLoop = new Node(7);
    selfLoop->next = selfLoop;
    Node* result4 = removeLoop(selfLoop);
    assert(result4 == selfLoop);
    assert(selfLoop->next == nullptr);
    delete selfLoop;

    return 0;
}

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Detects and removes a cycle in a singly linked list. Returns the head of the (now acyclic) list.
Node* removeLoop(Node* head) {
    if (head == nullptr) {
        return nullptr;
    }

    // Floyd's cycle detection
    Node* slow = head;
    Node* fast = head;
    Node* meetingPoint = nullptr;
    
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            meetingPoint = slow;
            break;
        }
    }
    
    // No cycle detected
    if (meetingPoint == nullptr) {
        return head;
    }
    
    // Find the start of the cycle
    Node* start = head;
    while (start != meetingPoint) {
        start = start->next;
        meetingPoint = meetingPoint->next;
    }
    
    // Find the node just before the cycle start and break the link
    Node* temp = start;
    while (temp->next != start) {
        temp = temp->next;
    }
    temp->next = nullptr;
    
    return head;
}

// The solution uses Floyd’s Cycle Detection algorithm (tortoise and hare). First, two pointers—`slow` and `fast`—start at the head; `slow` advances one node per step and `fast` advances two nodes per step. If they meet, a cycle exists, and the meeting point is stored. If either pointer becomes `nullptr` during traversal, the list is acyclic and we return the original head unchanged. After detecting a cycle, we find the starting node of the cycle: reset `slow` to the head, keep `intersection` at the meeting point, and move both one step at a time until they meet—that meeting node is the start of the cycle. Then, from that start, traverse the cycle until we find the node whose `next` points back to the start; set that node’s `next` to `nullptr` to break the loop. Edge cases: empty list returns `nullptr`; single node with no cycle returns head; if the cycle includes the head node, the traversal correctly finds the last node in the cycle (the one pointing back to head) and breaks the link. Time complexity is O(n) because the slow/fast pointers traverse the list at most twice (once for detection, once for finding start, and once more for breaking), and space complexity is O(1) since only a few pointers are used.
