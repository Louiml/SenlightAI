Write a C++ function `int buyTicket(int* priorities, int n, int k)` that simulates a ticket counter where people stand in a queue, and the person with the highest priority (largest value in the array) is always served first. At each step, the person at the front of the queue is checked: if they have the highest priority among all remaining people, they are served (removed from the queue, and the time counter increments by 1); otherwise, they are moved to the back of the queue, and no time is counted. The function must return the time (number of served people) at which the person initially at index `k` (0-based) finally receives their ticket. The original array is not to be modified; duplicate priority values are allowed, and ties are broken by queue order (the person who reaches the front first among equal priorities is served first). The queue and priority queue should be implemented using standard library containers.

The core idea is to simulate the process using two data structures: a `std::queue<int>` storing the indices of people in their current order, and a `std::priority_queue<int>` (max-heap) storing the priorities of people still waiting. We repeatedly examine the person at the front of the queue. If that person's priority equals the top of the max-heap (the maximum priority among all waiting), then they are the next to be served: we pop that priority from the heap, pop the index from the queue, and increment a time counter. If their priority is less than the heap's top, we move that index to the back of the queue without counting time. The loop continues until the front of the queue is exactly index `k` and its priority equals the heap's top, meaning it's that person's turn. At that point we return `time + 1`, because the person at index `k` will be served now. Edge cases include: the target person has the highest priority initially (answer is 1), there are duplicate maximum priorities (the earliest in queue among them is served first), and `k` can be anywhere in the array. Time complexity is \(O(n \log n)\) due to heap operations and each queue rotation, and space complexity is \(O(n)\) for the two containers.

#include <queue>
#include <vector>

// Simulate a ticket counter and return the time when person at index k gets served.
// people is an array of priorities, n is its size, k is the 0-based target index.
int buyTicket(const int* people, int n, int k) {
    std::queue<int> line;          // queue of indices in current order
    std::priority_queue<int> maxHeap; // max-heap of priorities of people still waiting

    for (int i = 0; i < n; ++i) {
        line.push(i);
        maxHeap.push(people[i]);
    }

    int time = 0; // number of people already served

    while (true) {
        int frontIndex = line.front();

        // If front is the target and has the highest priority, we're done.
        if (frontIndex == k && people[frontIndex] == maxHeap.top()) {
            return time + 1;
        }

        // If the front person has less than max priority, move them to back.
        if (people[frontIndex] < maxHeap.top()) {
            line.pop();
            line.push(frontIndex);
        } else {
            // Front person has max priority; serve them.
            maxHeap.pop();
            line.pop();
            ++time;
        }
    }
}

#include <cassert>

int main() {
    // Basic case: target is already the highest priority.
    int arr1[] = {3, 1, 2};
    assert(buyTicket(arr1, 3, 0) == 1);

    // Target in the middle, needs to wait for higher priorities.
    int arr2[] = {2, 5, 1, 3};
    assert(buyTicket(arr2, 4, 2) == 3);

    // All equal priorities: serves in queue order.
    int arr3[] = {4, 4, 4, 4};
    assert(buyTicket(arr3, 4, 2) == 3);

    // Target is last in a queue where all before have higher priority.
    int arr4[] = {9, 8, 7, 6, 5};
    assert(buyTicket(arr4, 5, 4) == 5);

    // Target at index 1, first person has higher priority but moves to back.
    int arr5[] = {5, 3, 8, 1};
    assert(buyTicket(arr5, 4, 1) == 3);

    // Duplicate max priority: earliest in queue among them is served first.
    int arr6[] = {2, 4, 4, 3};
    assert(buyTicket(arr6, 4, 2) == 2);
    assert(buyTicket(arr6, 4, 1) == 1);

    // Single person.
    int arr7[] = {7};
    assert(buyTicket(arr7, 1, 0) == 1);

    // Target has the lowest priority, must wait for all higher ones.
    int arr8[] = {2, 3, 1, 5};
    assert(buyTicket(arr8, 4, 2) == 4);
}
