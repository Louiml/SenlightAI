Write a C++ function that manages a singly linked list of integers via a `ListNode` structure and a `LinkedListManager` class. The class must support inserting nodes at the end, deleting the first node by value, deleting a node by its zero-based index, and printing the list contents. Additionally, it must implement a member function `getListAsVector()` that returns a `std::vector<int>` of the current list values, allowing external comparison. The class must handle edge cases: inserting into an empty list, deleting from an empty list (print an error message and return false), deleting a value that does not exist (print error and return false), deleting an out-of-range index (print error and return false), and deleting the head node. The function signatures should be: `void insertEnd(int val)`, `bool deleteByValue(int val)`, `bool deleteByIndex(int idx)`, `void printList() const`, and `std::vector<int> getListAsVector() const`. The class should manage its own memory properly with a destructor that deletes all nodes.
#include <cassert>
#include <vector>

int main() {
    LinkedListManager list;
    // Insert 1..5
    for (int i = 1; i <= 5; ++i) list.insertEnd(i);
    assert((list.getListAsVector() == std::vector<int>{1, 2, 3, 4, 5}));

    // Delete value 3
    assert(list.deleteByValue(3) == true);
    assert((list.getListAsVector() == std::vector<int>{1, 2, 4, 5}));

    // Delete index 1 (value 2)
    assert(list.deleteByIndex(1) == true);
    assert((list.getListAsVector() == std::vector<int>{1, 4, 5}));

    // Delete head (index 0)
    assert(list.deleteByIndex(0) == true);
    assert((list.getListAsVector() == std::vector<int>{4, 5}));

    // Delete value that doesn't exist
    assert(list.deleteByValue(99) == false);
    assert((list.getListAsVector() == std::vector<int>{4, 5}));

    // Delete index out of range
    assert(list.deleteByIndex(5) == false);
    assert((list.getListAsVector() == std::vector<int>{4, 5}));

    // Clear list by deleting remaining
    assert(list.deleteByIndex(1) == true); // deletes 5
    assert(list.deleteByValue(4) == true); // deletes 4
    assert(list.getListAsVector().empty());

    // Test deleting from empty list
    assert(list.deleteByValue(1) == false);
    assert(list.deleteByIndex(0) == false);

    // Test single-element list
    list.insertEnd(42);
    assert((list.getListAsVector() == std::vector<int>{42}));
    assert(list.deleteByValue(42) == true);
    assert(list.getListAsVector().empty());

    // Test insert after empty
    list.insertEnd(10);
    list.insertEnd(20);
    assert((list.getListAsVector() == std::vector<int>{10, 20}));

    // Destructor will clean up remaining nodes
    return 0;
}
#include <iostream>
#include <vector>

struct ListNode {
    int data;
    ListNode* next;
    ListNode(int val) : data(val), next(nullptr) {}
};

class LinkedListManager {
private:
    ListNode* head;

public:
    LinkedListManager() : head(nullptr) {}
    ~LinkedListManager() {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Insert at the end
    void insertEnd(int val) {
        ListNode* newNode = new ListNode(val);
        if (!head) {
            head = newNode;
            return;
        }
        ListNode* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }

    // Delete first occurrence by value
    bool deleteByValue(int val) {
        if (!head) {
            std::cout << "List is empty, cannot delete value " << val << std::endl;
            return false;
        }
        if (head->data == val) {
            ListNode* toDelete = head;
            head = head->next;
            std::cout << "Deleted value " << toDelete->data << std::endl;
            delete toDelete;
            return true;
        }
        ListNode* prev = head;
        ListNode* current = head->next;
        while (current) {
            if (current->data == val) {
                prev->next = current->next;
                std::cout << "Deleted value " << current->data << std::endl;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        std::cout << "Value " << val << " not found" << std::endl;
        return false;
    }

    // Delete node by zero-based index
    bool deleteByIndex(int idx) {
        if (!head) {
            std::cout << "List is empty, cannot delete index " << idx << std::endl;
            return false;
        }
        if (idx == 0) {
            ListNode* toDelete = head;
            head = head->next;
            std::cout << "Deleted index 0, value " << toDelete->data << std::endl;
            delete toDelete;
            return true;
        }
        ListNode* prev = head;
        int i = 0;
        while (i < idx - 1 && prev) {
            prev = prev->next;
            i++;
        }
        if (!prev || !prev->next) {
            std::cout << "Index " << idx << " out of range" << std::endl;
            return false;
        }
        ListNode* toDelete = prev->next;
        prev->next = toDelete->next;
        std::cout << "Deleted index " << idx << ", value " << toDelete->data << std::endl;
        delete toDelete;
        return true;
    }

    // Print list contents
    void printList() const {
        std::cout << "List: ";
        ListNode* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Return list as vector for comparison
    std::vector<int> getListAsVector() const {
        std::vector<int> result;
        ListNode* current = head;
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
};
// The solution uses a simple singly linked list with a head pointer. `insertEnd` traverses to the tail and appends a new node; if the list is empty, the new node becomes the head. `deleteByValue` first checks for an empty list (return false after printing an error), then handles the head case by updating the head pointer and deleting the old head, otherwise traverses with a `prev` pointer to unlink the matching node. `deleteByIndex` checks for an empty list, then if `idx == 0` removes the head; otherwise it moves `idx-1` steps forward, verifying that the node exists and has a next node; if valid, it unlinks and deletes the target, else prints an error and returns false. `printList` outputs the values space-separated. `getListAsVector` iterates and collects values. The destructor deletes all remaining nodes to prevent leaks. Edge cases include deleting from an empty list, deleting a non-existent value, deleting an out-of-range index, and deleting when the list has exactly one node. Time complexity: insert and all deletions are O(n) in the worst case (traversal), getListAsVector is O(n), print is O(n). Space complexity: O(1) auxiliary for operations, O(n) for the list itself.
