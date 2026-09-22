Implement a C++ function `int queueElementAt(const int* queueArray, int size, int numberOfElements, int frontIndex, int rearIndex, int position)` that simulates a circular queue stored in a fixed-size array. The queue is represented by a raw array `queueArray` of capacity `size`, with `numberOfElements` indicating how many items are currently stored, and `frontIndex` and `rearIndex` being the current front and rear indices (with rear initialized to the position of the last enqueued element, or -1 if empty). The function should return the element at the given logical position (0-based, where 0 is the front of the queue) without modifying the queue. If the position is invalid (less than 0 or greater than or equal to `numberOfElements`), return -1. The queue follows the standard circular wrap-around behavior, meaning that after reaching the end of the array, the rear wraps to index 0. The function must handle edge cases such as an empty queue, a full queue, and positions near the rear when wrapping occurs.

#include <cassert>

int main() {
    // Basic case: queue not wrapped, size 5, 3 elements
    int q1[5] = {10, 20, 30, 0, 0};
    assert(queueElementAt(q1, 5, 3, 0, 2, 0) == 10);
    assert(queueElementAt(q1, 5, 3, 0, 2, 1) == 20);
    assert(queueElementAt(q1, 5, 3, 0, 2, 2) == 30);
    assert(queueElementAt(q1, 5, 3, 0, 2, 3) == -1); // out of range

    // Wrapped case: front at 3, rear at 1, size 5, 4 elements
    int q2[5] = {40, 50, 0, 10, 20}; // logical: 10 (front at idx3), 20, 40, 50
    assert(queueElementAt(q2, 5, 4, 3, 1, 0) == 10);
    assert(queueElementAt(q2, 5, 4, 3, 1, 1) == 20);
    assert(queueElementAt(q2, 5, 4, 3, 1, 2) == 40);
    assert(queueElementAt(q2, 5, 4, 3, 1, 3) == 50);

    // Empty queue
    int q3[5] = {0, 0, 0, 0, 0};
    assert(queueElementAt(q3, 5, 0, 0, -1, 0) == -1);

    // Full queue with wrap (front at 2, rear at 1, size 5)
    int q4[5] = {30, 40, 10, 20, 0}; // logical: 10,20,30,40 (front idx2)
    assert(queueElementAt(q4, 5, 4, 2, 1, 0) == 10);
    assert(queueElementAt(q4, 5, 4, 2, 1, 3) == 40);

    // Single element queue
    int q5[5] = {0, 0, 0, 7, 0};
    assert(queueElementAt(q5, 5, 1, 3, 3, 0) == 7);
    assert(queueElementAt(q5, 5, 1, 3, 3, 1) == -1);

    // Invalid position negative
    assert(queueElementAt(q1, 5, 3, 0, 2, -1) == -1);
}

#include <cstddef> // for std::size_t

// Get element at logical position 'position' (0 = front) from a circular queue.
// Returns -1 if position is invalid or queue is empty.
int queueElementAt(const int* queueArray, int size, int numberOfElements, int frontIndex, int rearIndex, int position) {
    // If queue is empty or position is out of range, return -1
    if (numberOfElements <= 0 || position < 0 || position >= numberOfElements) {
        return -1;
    }
    // Compute physical index in circular array
    int physicalIndex = (frontIndex + position) % size;
    return queueArray[physicalIndex];
}

// To solve this, we need to understand the circular queue indexing. The front index points to the first element, and the rear index points to the last element. Since the queue is circular, the physical index of the element at logical position `pos` (0 = front) is `(frontIndex + pos) % size`. This formula works for both non-wrapped and wrapped cases because the modulo operation handles wrap-around. Before accessing, we must validate that `position >= 0` and `position < numberOfElements`; if invalid, return -1. Also, if `numberOfElements == 0`, the queue is empty, and any position is invalid. The time complexity is O(1) because we only perform arithmetic and a single array access. Space complexity is O(1) as we use only a constant amount of extra space. Important edge cases include: empty queue (numberOfElements == 0), position exactly at the front (0), position at the rear (numberOfElements-1), and positions that cause wrapping (frontIndex + pos >= size). We must also ensure that the input array size and numberOfElements are consistent; the caller is responsible for that, but our function trusts the inputs per the contract.
