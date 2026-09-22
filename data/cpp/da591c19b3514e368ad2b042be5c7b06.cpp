// Write a C++ function `Node* deleteMiddle(Node* head)` that takes the head pointer of a singly linked list of integers (constructed with the given `Node` struct: `int data; Node* next;`) and deletes the middle node from the list. The middle is defined as the node at position `⌊n/2⌋` using 0-based indexing, i.e., for a list of length `n`, if `n` is odd, delete the exact center; if `n` is even, delete the first of the two central nodes (e.g., for length 4, delete index 1; for length 5, delete index 2). The function must return the new head of the list. If the list is empty or has exactly one node, return `nullptr` (do not print anything). The function must not print any output; it should only modify the list. Use only `new` for dynamic allocation (no `malloc`/`free`; you may use `delete` to free the removed node). The linked list will be built externally, and your function will be called on it directly. Your solution must handle lists of length up to 10^5 efficiently.

// The algorithm uses the classic "two-pointer" technique (fast and slow pointers). Initialize two pointers `slow` and `fast` both at `head`. Also keep a `prev` pointer initialized to `nullptr` to track the node just before `slow`. Traverse the list while `fast` is not `nullptr` and `fast->next` is not `nullptr`. In each iteration, advance `prev = slow`, `slow = slow->next`, and `fast = fast->next->next` (or `fast = fast->next` if the next is null, but the loop condition handles that). At termination, `slow` points to the middle node to delete. The position matches the required definition: for even length, `fast` becomes `nullptr` after stepping twice per loop, so `slow` ends at index `n/2` (0-based) which is the first of the two middle nodes (e.g., length 4: slow ends at index 2? Let's verify: n=4, fast=0, slow=0; loop1: fast=2, slow=1; loop2: fast=next->next of node2? Actually careful: for even length, after loop1 fast is at index2, then fast->next is index3, fast->next->next is null, so loop2 condition: fast!=null and fast->next!=null => fast=2, fast->next=3 (not null) so enters loop2: prev=slow(1), slow=2, fast=fast->next->next = null, stop. slow at index2, which is the second of two middle (indices 1 and 2). But the task says delete the first of two central nodes (index 1). So we must adjust: use fast instead of fast->next->next? Let's reconsider: For even length, we want index `n/2 - 1`? Actually typical "middle" with slow/fast gives index floor(n/2) for odd, and index n/2 for even (the second of two). To get first of two, we can use a different approach: count length first, or use fast that moves one step per loop. Simpler: traverse with a counter to find length, then traverse again to the node at position `length/2` (integer division) where for even length, `length/2` gives index `n/2` which for n=4 is 2 (second). To get first, use `(length-1)/2`? Let's compute: For n=4, we want index 1. `(n-1)/2` = 1. For n=5, we want index 2, `(5-1)/2=2`. So generally the position to delete is `(n-1)/2` integer division. That works for both even and odd. So we can do two passes: first count length `len`, then traverse `pos = (len-1)/2` steps from head, keeping track of previous node, and delete that node. This is O(n) time, O(1) space. Edge cases: empty list or single node return nullptr. If len=2, pos=0, delete head, return head->next. If len>1, we also need to free deleted node using `delete`. Complexity: O(n) time, O(1) auxiliary space.

#include <cstddef>

struct Node {
    int data;
    Node* next;
};

// Delete the middle node of a singly linked list.
// Middle is defined by position (len-1)/2 using 0-based indexing.
// Returns the head of the modified list (or nullptr if empty/single-node).
Node* deleteMiddle(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return nullptr;
    }

    // First pass: count length
    int length = 0;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        ++length;
    }

    // Position to delete: (length-1)/2
    int pos = (length - 1) / 2;

    // Traverse to that position, keeping track of previous node
    Node* prev = nullptr;
    Node* cur = head;
    for (int i = 0; i < pos; ++i) {
        prev = cur;
        cur = cur->next;
    }

    // Now cur is the node to delete, prev is its predecessor
    if (prev == nullptr) {
        // Deleting head
        head = cur->next;
    } else {
        prev->next = cur->next;
    }
    delete cur;
    return head;
}

#include <cassert>

// Helper to build a list from a vector-like initializer list
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node{v, nullptr};
        if (!head) head = n;
        else tail->next = n;
        tail = n;
    }
    return head;
}

// Helper to convert list to vector<int> for comparison
std::vector<int> listToVector(const Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to free list
void freeList(Node* head) {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Empty list
    assert(deleteMiddle(nullptr) == nullptr);

    // Test 2: Single node list
    Node* single = new Node{42, nullptr};
    assert(deleteMiddle(single) == nullptr);

    // Test 3: Two nodes -> delete head (position 0)
    Node* two = buildList({1, 2});
    Node* twoHead = deleteMiddle(two);
    assert(listToVector(twoHead) == std::vector<int>({2}));
    freeList(twoHead);

    // Test 4: Three nodes -> delete middle (position 1)
    Node* three = buildList({1, 2, 3});
    Node* threeHead = deleteMiddle(three);
    assert(listToVector(threeHead) == std::vector<int>({1, 3}));
    freeList(threeHead);

    // Test 5: Four nodes -> delete position 1 (first of two middles)
    Node* four = buildList({10, 20, 30, 40});
    Node* fourHead = deleteMiddle(four);
    assert(listToVector(fourHead) == std::vector<int>({10, 30, 40}));
    freeList(fourHead);

    // Test 6: Five nodes -> delete position 2
    Node* five = buildList({5, 15, 25, 35, 45});
    Node* fiveHead = deleteMiddle(five);
    assert(listToVector(fiveHead) == std::vector<int>({5, 15, 35, 45}));
    freeList(fiveHead);

    // Test 7: Even length 6 -> delete position (6-1)/2 = 2
    Node* six = buildList({1, 2, 3, 4, 5, 6});
    Node* sixHead = deleteMiddle(six);
    assert(listToVector(sixHead) == std::vector<int>({1, 2, 4, 5, 6}));
    freeList(sixHead);

    // Test 8: Odd length 7 -> delete position 3
    Node* seven = buildList({1, 2, 3, 4, 5, 6, 7});
    Node* sevenHead = deleteMiddle(seven);
    assert(listToVector(sevenHead) == std::vector<int>({1, 2, 3, 5, 6, 7}));
    freeList(sevenHead);

    // Test 9: Large list (1..100) -> delete middle position 49
    std::vector<int> large;
    for (int i = 1; i <= 100; ++i) large.push_back(i);
    Node* largeList = buildList({large.begin(), large.end()});
    Node* largeHead = deleteMiddle(largeList);
    auto vec = listToVector(largeHead);
    assert(vec.size() == 99);
    // Expected: position 49 (0-based) removed, so remove element at index 49 from original
    std::vector<int> expected = large;
    expected.erase(expected.begin() + 49);
    assert(vec == expected);
    freeList(largeHead);

    // Test 10: Every node distinct, check memory (no leak detection here)
    // Just ensure no crash on long list
    Node* longList = buildList({1, 2, 3, 4, 5, 6, 7, 8, 9});
    Node* longHead = deleteMiddle(longList);
    assert(listToVector(longHead).size() == 8);
    freeList(longHead);

    return 0;
}
