/*
Write a C++ function named `josephusOrder` that takes three integer parameters: `n` (the number of people in the circle, labeled 1 through n), `s` (the starting position, 1-based index of the person from whom counting begins), and `m` (the step count; every m-th person is eliminated). The function must simulate the classic Josephus problem where people stand in a circle, counting begins at person `s`, and every m-th person is removed (counting the starting person as 1). The function should return a `std::vector<int>` containing the order in which people are eliminated, ending with the last survivor as the final element. Assume all inputs are positive integers with `s ≤ n` and `m ≥ 1`. Do not use any external containers like `std::list`; instead, implement your own singly circular linked list or use the provided `CircleList` concept, but your solution must be self-contained (include your own node structure and list logic). Your function must not print anything and must handle edge cases such as `n=1`, `m=1`, and `s` not being 1.
*/

#include <vector>

// Node for singly circular linked list
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Simulate Josephus problem and return elimination order (last element is survivor)
std::vector<int> josephusOrder(int n, int s, int m) {
    std::vector<int> result;
    if (n <= 0) return result;
    if (n == 1) { result.push_back(1); return result; }

    // Build circular list 1..n
    Node* head = new Node(1);
    Node* tail = head;
    for (int i = 2; i <= n; ++i) {
        tail->next = new Node(i);
        tail = tail->next;
    }
    tail->next = head; // make circular

    // Move current pointer to start at person s (1-based)
    Node* current = head;
    Node* prev = tail; // previous node before current (needed for removal)
    for (int i = 1; i < s; ++i) {
        prev = current;
        current = current->next;
    }

    int remaining = n;
    while (remaining > 1) {
        // Advance m-1 steps to land on m-th person
        for (int step = 1; step < m; ++step) {
            prev = current;
            current = current->next;
        }
        // Record and remove current node
        result.push_back(current->data);
        Node* toDelete = current;
        current = current->next; // next becomes new current
        prev->next = current;    // unlink
        delete toDelete;
        --remaining;
    }

    // Last remaining is survivor
    result.push_back(current->data);
    delete current;
    return result;
}

#include <cassert>
#include <vector>

// (Include the josephusOrder function here)

int main() {
    // n=1: only survivor
    auto r1 = josephusOrder(1, 1, 3);
    assert(r1 == std::vector<int>({1}));

    // n=5, s=1, m=2: elimination order 2,4,1,5,3 (last is survivor 3)
    auto r2 = josephusOrder(5, 1, 2);
    assert(r2 == std::vector<int>({2, 4, 1, 5, 3}));

    // n=7, s=3, m=3: known order 5,1,4,2,7,3,6 (survivor 6)
    auto r3 = josephusOrder(7, 3, 3);
    assert(r3 == std::vector<int>({5, 1, 4, 2, 7, 3, 6}));

    // m=1: remove in order starting at s, wrapping
    auto r4 = josephusOrder(4, 2, 1);
    // start at 2, then 3,4,1 (survivor 1)
    assert(r4 == std::vector<int>({2, 3, 4, 1}));

    // Classic 41,1,3: last survivor is 31 (known problem)
    auto r5 = josephusOrder(41, 1, 3);
    assert(r5.back() == 31);
    // Check full order length and first elimination
    assert(r5.size() == 41);
    assert(r5.front() == 3);

    // n=6, s=6, m=1: start at 6, then 1,2,3,4,5 (survivor 5)
    auto r6 = josephusOrder(6, 6, 1);
    assert(r6 == std::vector<int>({6, 1, 2, 3, 4, 5}));

    // n=2, s=2, m=2: count 2 from person 2 -> eliminate 2, survivor 1
    auto r7 = josephusOrder(2, 2, 2);
    assert(r7 == std::vector<int>({2, 1}));

    return 0;
}

// The solution requires simulating the elimination process on a circular list of integers from 1 to n. First, build a circular singly linked list by inserting nodes in order 1..n, connecting the last node back to the first. To start at the correct position, traverse from the head to the (s-1)th node (0-indexed) to set the "current" pointer to person `s`. Then, for each elimination step, we need to advance `m-1` steps (because the current person counts as 1, so moving m-1 steps lands on the m-th person). After advancing, record that person's value, remove the node, and set the current pointer to the node following the removed one (which is the next person for the next count). Repeat until only one node remains, then push that survivor's value as the final element. Edge cases: if `n=1`, return `{1}` immediately; if `m=1`, elimination proceeds directly in order starting from `s`, wrapping around; if `s` is 1, starting at head is fine. The time complexity is O(n * m) because each removal requires m steps, and there are n removals. In the worst case (m large), this is O(n*m), but for typical m small it's near O(n). Space complexity is O(n) for the linked list nodes.
