// Write a C++ function `bool removeNode(CircularLinkedList& list, int rollno)` that removes the node with the given roll number from a circular singly linked list and returns `true` if the node was found and deleted, or `false` otherwise. The function must correctly handle all cases: deleting the only node (making the list empty), deleting the last node (the one pointed to by the `LAST` pointer), and deleting any middle or first node. You may assume the `CircularLinkedList` class provides `listEmpty()` and `search(int, Node**, Node**)` methods with the same signatures and behavior as in the provided code snippet (where `search` returns `true` if found, with `akram` set to the predecessor and `syah` set to the found node). The function should not print anything; it only modifies the list and returns a boolean. If the list is empty or the node is not found, return `false` without changing the list.

The core algorithm uses the existing `search` method to locate the target node and its predecessor. Before calling `search`, check if the list is empty; if so, return `false` immediately. After successfully finding the node (`search` returns `true`), there are three distinct deletion cases based on the position of the found node (`syah`) relative to `LAST`:
1. **Only node**: If `syah == LAST` and `syah->next == LAST` (i.e., the list has exactly one node), delete the node and set `LAST = NULL`.
2. **Last node** (but not only): If `syah == LAST` (and there is more than one node), set `LAST = akram` (the predecessor becomes the new last node) and then point `akram->next` to `syah->next` (which is the first node, since it's circular).
3. **Middle or first node**: Otherwise, bypass the node by setting `akram->next = syah->next`, then delete the node.
After deletion, return `true`. If `search` returns `false`, return `false` without modifying the list. Edge cases: empty list (return `false`), single node (case 1), target is the last node (case 2), target is the first node (middle case where `akram` is the last node). Time complexity is O(n) because `search` traverses the list linearly. Space complexity is O(1) since only a few pointers are used. The function must not print anything.

#include <iostream>

// Assuming the Node and CircularLinkedList definitions from the snippet are available.

// Remove the node with the given roll number from a circular linked list.
// Returns true if the node was found and deleted, false otherwise.
bool removeNode(CircularLinkedList& list, int rollno) {
    if (list.listEmpty()) {
        return false;
    }
    
    Node* akram = nullptr;  // predecessor
    Node* syah = nullptr;   // node to delete
    
    if (!list.search(rollno, &akram, &syah)) {
        return false;
    }
    
    // Case 1: Only node in the list
    if (syah == list.LAST && syah->next == list.LAST) {
        delete syah;
        list.LAST = nullptr;
    }
    // Case 2: Node is the last one (but not the only one)
    else if (syah == list.LAST) {
        list.LAST = akram;           // new last node is the predecessor
        akram->next = syah->next;    // link to the first node
        delete syah;
    }
    // Case 3: Middle or first node
    else {
        akram->next = syah->next;    // bypass the node
        delete syah;
    }
    
    return true;
}

#include <cassert>
#include <iostream>

// Include or paste the Node and CircularLinkedList definitions here for compilation.
// For brevity, the test assumes the full class definition is available.

int main() {
    // Test 1: Delete from empty list
    CircularLinkedList emptyList;
    assert(removeNode(emptyList, 5) == false);
    
    // Test 2: Delete the only node
    CircularLinkedList singleList;
    singleList.addNode(); // add one node, but user input is needed; for testing we manually set
    // Instead of user input, directly construct a list with one node for testing:
    // (Manual setup for testing purposes)
    CircularLinkedList list1;
    Node* n1 = new Node{1, "Alice", nullptr};
    n1->next = n1;
    list1.LAST = n1;
    assert(removeNode(list1, 1) == true);
    assert(list1.listEmpty() == true);
    
    // Test 3: Delete the last node from a multi-node list
    // Build circular list: 10->20->30 (LAST points to 30)
    CircularLinkedList list2;
    Node* n10 = new Node{10, "A", nullptr};
    Node* n20 = new Node{20, "B", nullptr};
    Node* n30 = new Node{30, "C", nullptr};
    n10->next = n20;
    n20->next = n30;
    n30->next = n10;
    list2.LAST = n30;
    assert(removeNode(list2, 30) == true);
    // Now LAST should be 20, and it points to 10, and 10 points to 20
    assert(list2.LAST->rollNumber == 20);
    assert(list2.LAST->next->rollNumber == 10);
    assert(list2.LAST->next->next->rollNumber == 20);
    
    // Test 4: Delete a middle node
    // Rebuild list: 10->20->30->40 (LAST=40)
    CircularLinkedList list3;
    Node* m10 = new Node{10, "A", nullptr};
    Node* m20 = new Node{20, "B", nullptr};
    Node* m30 = new Node{30, "C", nullptr};
    Node* m40 = new Node{40, "D", nullptr};
    m10->next = m20;
    m20->next = m30;
    m30->next = m40;
    m40->next = m10;
    list3.LAST = m40;
    assert(removeNode(list3, 20) == true);
    // After deletion: 10->30->40->10
    assert(list3.LAST->rollNumber == 40);
    assert(list3.LAST->next->rollNumber == 10);
    assert(list3.LAST->next->next->rollNumber == 30);
    
    // Test 5: Delete the first node (which is not LAST)
    // Rebuild: 10->20->30 (LAST=30) and delete 10
    CircularLinkedList list4;
    Node* f10 = new Node{10, "A", nullptr};
    Node* f20 = new Node{20, "B", nullptr};
    Node* f30 = new Node{30, "C", nullptr};
    f10->next = f20;
    f20->next = f30;
    f30->next = f10;
    list4.LAST = f30;
    assert(removeNode(list4, 10) == true);
    // After: 20->30->20
    assert(list4.LAST->rollNumber == 30);
    assert(list4.LAST->next->rollNumber == 20);
    assert(list4.LAST->next->next->rollNumber == 30);
    
    // Test 6: Node not found
    // Use list4 which has 20 and 30
    assert(removeNode(list4, 99) == false);
    
    // Clean up remaining nodes in the test lists to avoid leaks
    // (Not strictly necessary for assertion tests, but good practice)
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
