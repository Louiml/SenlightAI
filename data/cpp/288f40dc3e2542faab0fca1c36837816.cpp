/*
Given a set of character-frequency pairs and the Huffman codes generated for them, write a C++ function that reconstructs the frequency-sorted linked list in the same reverse-sorted order as the original `rev_insert` logic (descending by frequency), and returns a pointer to its head. The input is provided as a vector of pairs (character, frequency), and the function should create nodes with `char c`, `int f`, and `bool is_leaf` fields, maintaining the exact same insertion behavior as the provided `rev_insert` method: if the new node's frequency equals an existing node’s frequency, the new node must be placed after all existing nodes with that same frequency (i.e., stable sorting by descending frequency). The function must not modify the input vector.
*/

#include <vector>

struct HuffmanNode {
    char c;
    int f;
    bool is_leaf;
    HuffmanNode* next;
    HuffmanNode(char ch, int freq) : c(ch), f(freq), is_leaf(true), next(nullptr) {}
};

// Reconstructs a reverse-sorted (descending by frequency) linked list with stable insertion.
// Returns the head pointer of the created list, or nullptr if input is empty.
HuffmanNode* buildReverseSortedList(const std::vector<std::pair<char, int>>& freq_pairs) {
    HuffmanNode* head = nullptr;

    for (const auto& p : freq_pairs) {
        HuffmanNode* new_node = new HuffmanNode(p.first, p.second);

        if (!head) {
            head = new_node;
            continue;
        }

        // Stable insertion: stop when we find a node with strictly smaller frequency.
        HuffmanNode* trav = head;
        HuffmanNode* prev = nullptr;
        while (trav != nullptr) {
            if (new_node->f > trav->f) { // new node should come before this one
                break;
            }
            // If equal frequency, continue past to preserve original order.
            prev = trav;
            trav = trav->next;
        }

        // Insert before trav, or at tail if trav is null.
        if (prev == nullptr) {
            new_node->next = head;
            head = new_node;
        } else {
            prev->next = new_node;
            new_node->next = trav;
        }
    }

    return head;
}

#include <cassert>
#include <vector>
#include <utility>

// (The solution function and struct are assumed to be defined above.)

int main() {
    // Basic empty case
    std::vector<std::pair<char, int>> empty;
    assert(buildReverseSortedList(empty) == nullptr);

    // Single element
    std::vector<std::pair<char, int>> single = {{'a', 5}};
    HuffmanNode* head = buildReverseSortedList(single);
    assert(head != nullptr);
    assert(head->c == 'a');
    assert(head->f == 5);
    assert(head->next == nullptr);
    delete head;

    // Multiple distinct frequencies, descending order already
    std::vector<std::pair<char, int>> desc = {{'a', 10}, {'b', 5}, {'c', 1}};
    head = buildReverseSortedList(desc);
    assert(head->c == 'a' && head->f == 10);
    assert(head->next->c == 'b' && head->next->f == 5);
    assert(head->next->next->c == 'c' && head->next->next->f == 1);
    assert(head->next->next->next == nullptr);
    delete head->next->next;
    delete head->next;
    delete head;

    // Input in ascending order (must be reversed)
    std::vector<std::pair<char, int>> asc = {{'c', 1}, {'b', 5}, {'a', 10}};
    head = buildReverseSortedList(asc);
    assert(head->c == 'a' && head->f == 10);
    assert(head->next->c == 'b' && head->next->f == 5);
    assert(head->next->next->c == 'c' && head->next->next->f == 1);
    assert(head->next->next->next == nullptr);
    delete head->next->next;
    delete head->next;
    delete head;

    // Stable order for equal frequencies
    std::vector<std::pair<char, int>> stable = {{'x', 3}, {'y', 3}, {'z', 3}};
    head = buildReverseSortedList(stable);
    assert(head->c == 'x' && head->f == 3);
    assert(head->next->c == 'y' && head->next->f == 3);
    assert(head->next->next->c == 'z' && head->next->next->f == 3);
    assert(head->next->next->next == nullptr);
    delete head->next->next;
    delete head->next;
    delete head;

    // Mixed frequencies with duplicates
    std::vector<std::pair<char, int>> mixed = {{'a', 2}, {'b', 7}, {'c', 2}, {'d', 7}, {'e', 1}};
    head = buildReverseSortedList(mixed);
    // Expected: b(7), d(7), a(2), c(2), e(1)
    assert(head->c == 'b' && head->f == 7);
    assert(head->next->c == 'd' && head->next->f == 7);
    assert(head->next->next->c == 'a' && head->next->next->f == 2);
    assert(head->next->next->next->c == 'c' && head->next->next->next->f == 2);
    assert(head->next->next->next->next->c == 'e' && head->next->next->next->next->f == 1);
    assert(head->next->next->next->next->next == nullptr);
    HuffmanNode* cur = head;
    while (cur) {
        HuffmanNode* next = cur->next;
        delete cur;
        cur = next;
    }

    // Ensure original input was not modified
    assert(mixed[0] == std::make_pair('a', 2));
    assert(mixed[2] == std::make_pair('c', 2));

    return 0;
}

// The core task is to implement a stable insertion sort into a singly linked list, ordered by descending frequency. The given `rev_insert` method walks from the head and stops when it finds a node with a smaller frequency (strictly less than the new node’s frequency), then inserts the new node before that node. This means if frequencies are equal, the traversal continues past them, so equal-frequency nodes appear in their original insertion order (stable). We follow the same logic: start with an empty list, for each input pair in the given order, create a new node and insert it using the stable descending-frequency rule. Edge cases include an empty input (return nullptr), a single element, duplicate frequencies, and ensuring that the original relative order of equal-frequency elements from the input vector is preserved. Time complexity is O(n^2) in the worst case (each insertion scans the list), and O(n) in the best case if the input is already in descending order; auxiliary space is O(n) for the list itself, excluding the input storage.
