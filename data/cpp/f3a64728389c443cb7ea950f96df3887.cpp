Write a C++ function that simulates a sequence of insertions and deletions on a doubly linked list of unique integers. The list is initially built by inserting a given sequence of `n` integers in order, one after another at the current tail position. After construction, process `q` operations: type `1 x y` inserts `y` immediately after the node with value `x` (guaranteed to exist), and type `2 x` removes the node with value `x` (guaranteed to exist). All inserted values are unique during the entire process. The function should take the initial sequence as a vector and a vector of operations (each encoded as `{type, x, y}` where `y` is ignored for deletions), and return a vector of the remaining list values in order from head to tail. Use a hash map from value to node pointer for O(1) access. Assume all inputs are valid as described.
// We maintain a doubly linked list with sentinel head and tail nodes to simplify boundary operations. A `std::unordered_map<int, Node*>` stores each value’s node pointer for O(1) lookup. For insertion, we locate the node for `x`, create a new node with value `y`, link it between `x` and `x`’s next, and update the map. For deletion, we locate the node for `x`, unlink it from its previous and next, and erase from the map. After processing all operations, we traverse from `head->next` until `tail` and collect values. Time complexity is O(n + q) for construction and operations, plus O(remaining) for traversal. Space is O(n + q) for the list and map. Edge cases: inserting after the last real node works because the tail sentinel handles the boundary; deleting any node including the last works similarly. Values are guaranteed unique, so no collision handling beyond map assignment is needed. The sentinel values are irrelevant as they never appear in output.
#include <vector>
#include <unordered_map>

struct DLLNode {
    int val;
    DLLNode* prev;
    DLLNode* next;
    DLLNode(int v, DLLNode* p, DLLNode* n) : val(v), prev(p), next(n) {}
};

// Simulate operations on a doubly linked list with unique values.
// initial: the initial sequence of integers.
// ops: each operation is {type, x, y}, where y is ignored for deletions.
// type 1: insert y immediately after node with value x.
// type 2: remove node with value x.
// Returns the remaining list values in order.
std::vector<int> processList(const std::vector<int>& initial, const std::vector<std::vector<int>>& ops) {
    // Sentinel nodes
    DLLNode* head = new DLLNode(-1, nullptr, nullptr);
    DLLNode* tail = new DLLNode(-1, nullptr, nullptr);
    head->next = tail;
    tail->prev = head;

    std::unordered_map<int, DLLNode*> m;

    // Build initial list
    DLLNode* cur = head;
    for (int val : initial) {
        DLLNode* node = new DLLNode(val, cur, cur->next);
        cur->next->prev = node;
        cur->next = node;
        cur = node;
        m[val] = node;
    }

    // Process operations
    for (const auto& op : ops) {
        int type = op[0];
        int x = op[1];
        if (type == 1) {
            int y = op[2];
            DLLNode* xNode = m[x];
            DLLNode* newNode = new DLLNode(y, xNode, xNode->next);
            xNode->next->prev = newNode;
            xNode->next = newNode;
            m[y] = newNode;
        } else { // type == 2
            DLLNode* xNode = m[x];
            xNode->prev->next = xNode->next;
            xNode->next->prev = xNode->prev;
            delete xNode;
            m.erase(x);
        }
    }

    // Collect result
    std::vector<int> result;
    for (DLLNode* it = head->next; it != tail; it = it->next) {
        result.push_back(it->val);
    }

    // Clean up remaining nodes
    DLLNode* it = head;
    while (it != nullptr) {
        DLLNode* next = it->next;
        delete it;
        it = next;
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above; test it directly.
int main() {
    // Initial [1,2,3]; insert 4 after 2, delete 1 -> [2,4,3]
    std::vector<int> init1 = {1, 2, 3};
    std::vector<std::vector<int>> ops1 = {{1, 2, 4}, {2, 1, 0}};
    assert(processList(init1, ops1) == std::vector<int>({2, 4, 3}));

    // Empty initial, no ops -> empty result
    assert(processList({}, {}) == std::vector<int>());

    // Single element, delete it -> empty
    assert(processList({5}, {{2, 5, 0}}) == std::vector<int>());

    // Insert after tail sentinel: initial [10], insert 20 after 10, then 30 after 20
    assert(processList({10}, {{1, 10, 20}, {1, 20, 30}}) == std::vector<int>({10, 20, 30}));

    // Insert before head (after nothing? Actually after the first element)
    // More complex sequence: start [7,8], insert 6 after 7, delete 8, insert 9 after 6
    std::vector<int> init2 = {7, 8};
    std::vector<std::vector<int>> ops2 = {{1, 7, 6}, {2, 8, 0}, {1, 6, 9}};
    assert(processList(init2, ops2) == std::vector<int>({7, 6, 9}));

    // Many operations: initial [1,2], insert 3 after 2, delete 2, insert 4 after 1
    assert(processList({1, 2}, {{1, 2, 3}, {2, 2, 0}, {1, 1, 4}}) == std::vector<int>({1, 4, 3}));

    // Duplicate values should not occur per spec, but ensure uniqueness is maintained
    // Not testing duplicates here.

    // Edge: delete all nodes one by one
    std::vector<int> init3 = {1, 2, 3, 4};
    std::vector<std::vector<int>> ops3 = {{2, 1, 0}, {2, 2, 0}, {2, 3, 0}, {2, 4, 0}};
    assert(processList(init3, ops3) == std::vector<int>());

    // Insert after a node that later gets deleted: insert 5 after 3, then delete 3
    assert(processList({1, 2, 3}, {{1, 3, 5}, {2, 3, 0}}) == std::vector<int>({1, 2, 5}));

    // Large sequence: build then insert at end repeatedly
    std::vector<int> init4 = {10};
    std::vector<std::vector<int>> ops4;
    for (int i = 11; i <= 15; ++i) {
        ops4.push_back({1, i-1, i});
    }
    assert(processList(init4, ops4) == std::vector<int>({10, 11, 12, 13, 14, 15}));

    return 0;
}
