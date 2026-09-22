Write a C++ function named `processLinkedListCommands` that simulates the behavior of the provided code snippet. The function should take a vector of command pairs, where each command is a pair of integers `(type, value)`. The types are: `0` = insert at head, `1` = insert at tail, `2` = delete at position `value` (0‑based index). After each command, the function must record the current list contents as a string (elements separated by a single space, ending with a newline). The function returns a single string containing the concatenation of all such snapshots, exactly as the original `main` would print. If the list becomes empty, the snapshot is an empty string followed by a newline. Assume the input is valid (types are 0,1,2; positions for deletion are non‑negative). Handle edge cases: deleting from an empty list or an out‑of‑range position should leave the list unchanged (and still produce the snapshot). The function must not modify the input vector. Use a custom singly linked list with dynamic nodes; you may define a helper `Node` struct inside the function or as a private helper outside. Ensure proper memory cleanup by deleting all nodes before returning.

The solution mirrors the logic in the provided code snippet. We maintain a singly linked list with a `Node` struct having `value` and `next` pointer. For each command pair `(x, v)`:
- If `x == 0`, create a new node with value `v` and insert it at the head.
- If `x == 1`, traverse to the tail and append a new node; if the list is empty, set the head to the new node.
- If `x == 2`, we attempt to delete the node at position `v`. If the list is empty, do nothing. If `v == 0`, delete the head (update head to `head->next`). Otherwise, traverse `pos-1` steps; if we encounter a null pointer or the next node is null before reaching the target, the position is invalid and we do nothing. Otherwise, unlink and delete the node at that position.
After each command, we traverse the list and build a string of values separated by spaces, appending a newline. We concatenate all such strings into a single result string. Important edge cases: deleting from an empty list, deleting index 0, deleting an index that is exactly the last element, and deleting an invalid index. Time complexity is O(n) per command (for insertion at tail or deletion traversal, and for snapshot traversal), so for q commands and average list length L, it is O(q * L). Space complexity is O(q * L) for the output string and O(L) for the list itself.

#include <string>
#include <vector>
#include <sstream>

struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

// Simulate linked list commands and return snapshots as a single string.
std::string processLinkedListCommands(const std::vector<std::pair<int, int>>& commands) {
    Node* head = nullptr;
    std::ostringstream result;

    for (const auto& cmd : commands) {
        int type = cmd.first;
        int value = cmd.second;

        if (type == 0) { // insert at head
            Node* new_node = new Node(value);
            new_node->next = head;
            head = new_node;
        } else if (type == 1) { // insert at tail
            Node* new_node = new Node(value);
            if (head == nullptr) {
                head = new_node;
            } else {
                Node* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = new_node;
            }
        } else if (type == 2) { // delete at position 'value'
            if (head == nullptr) {
                // list is empty; nothing to delete
            } else if (value == 0) {
                Node* to_delete = head;
                head = head->next;
                delete to_delete;
            } else {
                Node* temp = head;
                bool valid = true;
                for (int i = 1; i <= value - 1; ++i) {
                    if (temp->next == nullptr || temp->next->next == nullptr) {
                        valid = false;
                        break;
                    }
                    temp = temp->next;
                }
                if (valid && temp->next != nullptr) {
                    Node* to_delete = temp->next;
                    temp->next = temp->next->next;
                    delete to_delete;
                }
            }
        }

        // Snapshot current list
        Node* current = head;
        bool first = true;
        while (current != nullptr) {
            if (!first) {
                result << " ";
            }
            result << current->value;
            first = false;
            current = current->next;
        }
        result << "\n";
    }

    // Clean up remaining nodes
    while (head != nullptr) {
        Node* to_delete = head;
        head = head->next;
        delete to_delete;
    }

    return result.str();
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Basic insert at head and tail, and delete
    std::vector<std::pair<int, int>> commands1 = {{0, 5}, {1, 10}, {2, 0}};
    std::string result1 = processLinkedListCommands(commands1);
    assert(result1 == "5\n5 10\n10\n");

    // Test 2: Delete from empty list
    std::vector<std::pair<int, int>> commands2 = {{2, 0}};
    assert(processLinkedListCommands(commands2) == "\n");

    // Test 3: Remove head when only one element
    std::vector<std::pair<int, int>> commands3 = {{0, 7}, {2, 0}, {1, 3}};
    std::string result3 = processLinkedListCommands(commands3);
    assert(result3 == "7\n\n3\n");

    // Test 4: Delete invalid index (out of bounds)
    std::vector<std::pair<int, int>> commands4 = {{1, 1}, {1, 2}, {2, 5}};
    std::string result4 = processLinkedListCommands(commands4);
    assert(result4 == "1\n1 2\n1 2\n");

    // Test 5: Delete last element
    std::vector<std::pair<int, int>> commands5 = {{1, 10}, {1, 20}, {1, 30}, {2, 2}};
    std::string result5 = processLinkedListCommands(commands5);
    assert(result5 == "10\n10 20\n10 20 30\n10 20\n");

    // Test 6: Multiple operations leading to empty list
    std::vector<std::pair<int, int>> commands6 = {{0, 1}, {0, 2}, {2, 0}, {2, 0}};
    std::string result6 = processLinkedListCommands(commands6);
    assert(result6 == "1\n2 1\n1\n\n");

    // Test 7: Delete index 1 from list of size 2
    std::vector<std::pair<int, int>> commands7 = {{1, 1}, {1, 2}, {2, 1}};
    std::string result7 = processLinkedListCommands(commands7);
    assert(result7 == "1\n1 2\n1\n");

    // Test 8: Insert at head then delete head repeatedly
    std::vector<std::pair<int, int>> commands8 = {{0, 1}, {0, 2}, {0, 3}, {2, 0}};
    std::string result8 = processLinkedListCommands(commands8);
    assert(result8 == "1\n2 1\n3 2 1\n2 1\n");

    // Test 9: Empty command list
    std::vector<std::pair<int, int>> commands9 = {};
    assert(processLinkedListCommands(commands9) == "");

    // Test 10: Mixed operations with no deletes
    std::vector<std::pair<int, int>> commands10 = {{0, 1}, {1, 2}, {0, 3}};
    std::string result10 = processLinkedListCommands(commands10);
    assert(result10 == "1\n1 2\n3 1 2\n");

    return 0;
}
