Write a C++ function `reverseInGroups(struct node* head, int k)` that takes the head of a singly linked list (where each node has an integer `data` and a `next` pointer) and a positive integer `k`. The function must reverse the list in groups of size `k`. If the number of remaining nodes is fewer than `k`, reverse that final partial group as well. Return the pointer to the head of the newly reversed list. The original list must not allocate any new nodes; only pointer reassignment is allowed. The function signature must be `struct node* reverseInGroups(struct node* head, int k)` and the list is guaranteed to have at least one element.

// The solution uses recursion. We start at the head and reverse the first `k` nodes by iterating with three pointers: `prev`, `curr`, and `next`. After reversing those `k` nodes, `prev` becomes the new head of this reversed segment, and `curr` points to the start of the remaining list. Since the original head of the entire list was the first node of the segment, after reversal it becomes the tail of that segment. We set its `next` pointer to the result of recursively calling the function on the remaining list starting at `curr`. Finally, we return `prev` as the new head of the entire list. Edge cases include when `k` equals 1 (no change), when `k` is larger than the length of the list (reverse entire list), and when the list has exactly one node. Time complexity is O(n) because each node is visited exactly once. Space complexity is O(n/k) due to recursive call stack depth, in the worst case O(n) when k=1.

#include <cstddef>

struct node {
    int data;
    struct node* next;
    node(int x) : data(x), next(nullptr) {}
};

// Reverse a singly linked list in groups of size k.
// Returns the head of the modified list.
struct node* reverseInGroups(struct node* head, int k) {
    if (!head) return nullptr;

    struct node* prev = nullptr;
    struct node* curr = head;
    struct node* next = nullptr;
    int count = 0;

    // Reverse first k nodes (or fewer if list ends)
    while (curr != nullptr && count < k) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
        ++count;
    }

    // head is now the last node of the reversed group.
    // Recursively reverse the remaining list and connect it.
    if (next != nullptr) {
        head->next = reverseInGroups(next, k);
    }

    return prev; // new head of this group
}

#include <cassert>

// Helper to create list from initializer list
struct node* createList(std::initializer_list<int> values) {
    struct node* head = nullptr;
    struct node* tail = nullptr;
    for (int v : values) {
        struct node* newNode = new node(v);
        if (!head) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to convert list to vector for easy comparison
std::vector<int> listToVector(struct node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Basic group reversal
    struct node* list1 = createList({1, 2, 3, 4, 5});
    list1 = reverseInGroups(list1, 2);
    assert(listToVector(list1) == std::vector<int>({2, 1, 4, 3, 5}));

    // Test 2: k larger than list size
    struct node* list2 = createList({1, 2, 3});
    list2 = reverseInGroups(list2, 5);
    assert(listToVector(list2) == std::vector<int>({3, 2, 1}));

    // Test 3: k = 1 (no change)
    struct node* list3 = createList({7, 8, 9});
    list3 = reverseInGroups(list3, 1);
    assert(listToVector(list3) == std::vector<int>({7, 8, 9}));

    // Test 4: single element
    struct node* list4 = createList({42});
    list4 = reverseInGroups(list4, 3);
    assert(listToVector(list4) == std::vector<int>({42}));

    // Test 5: Exact multiple of k
    struct node* list5 = createList({1, 2, 3, 4, 5, 6});
    list5 = reverseInGroups(list5, 3);
    assert(listToVector(list5) == std::vector<int>({3, 2, 1, 6, 5, 4}));

    // Test 6: k equal to list length
    struct node* list6 = createList({10, 20, 30});
    list6 = reverseInGroups(list6, 3);
    assert(listToVector(list6) == std::vector<int>({30, 20, 10}));

    // Test 7: Negative numbers
    struct node* list7 = createList({-1, -2, -3, -4});
    list7 = reverseInGroups(list7, 2);
    assert(listToVector(list7) == std::vector<int>({-2, -1, -4, -3}));

    return 0;
}
