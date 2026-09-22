// Using a fixed-capacity array-based circular queue as inspiration, write a C++ function `int simulateQueueOperations(const std::vector<int>& operations, bool verbose)` that processes a sequence of operations encoded as integers: positive values (1–99) represent `enqueue(value)`, `0` represents `dequeue()` (returning the dequeued value or -1 if empty), and `-1` represents `peek()` (printing "Front: X" or "Queue가 비어있습니다." if empty, and returning the front value or -1). The queue has a maximum capacity of 5 elements (indices 0–4) using a circular buffer with `front` and `rear` pointers. `enqueue` on a full queue prints "Queue가 꽉찼습니다." and does nothing; `dequeue` on an empty queue prints "Queue가 비어있습니다." and returns -1; `peek` on an empty queue prints the same empty message and returns -1. The function must return the sum of all values returned by `dequeue`, `peek`, and successful `enqueue` operations (note: unsuccessful operations return -1 but should not be added). For each successful `enqueue`, the message "삽입 : value(index)" is printed; for each successful `dequeue`, "삭제 : value(index)" is printed, where `index` is the logical position (0-based) at which the operation occurred (i.e., the `front` pointer before increment for dequeue, `rear` pointer before increment for enqueue, and the current `front` for peek). The input vector will contain only integers in the set {-1, 0, 1..99} and will have at least one element. The function must be `const`-correct where applicable and must not print any extra output beyond the specified messages when `verbose` is `true`; when `verbose` is `false`, suppress all printing (both success and error messages) but still perform the operations and compute the sum.

// The approach is to simulate a circular queue of fixed size 5 using two integer indices `front` and `rear`, both initialized to 0. The queue is empty when `front == rear` and full when `front == (rear + 1) % MAX_SIZE`. For each operation in the input vector:
// - If value > 0 (enqueue): check if full; if full, print error (if verbose) and do not add to sum. Otherwise, store value at `arr[rear]`, print the insertion message with the current `rear` index, add the value to the sum, then update `rear = (rear + 1) % MAX_SIZE`.
// - If value == 0 (dequeue): check if empty; if empty, print error and not add. Otherwise, retrieve `arr[front]`, print deletion with current `front`, add the retrieved value to sum, then update `front = (front + 1) % MAX_SIZE`.
// - If value == -1 (peek): check if empty; if empty, print error and not add. Otherwise, print the front value and add the front value to sum (since `peek` returns the front value).
// Edge cases: empty queue operations, full queue operations, wrapping around indices after multiple insertions/deletions, and mixing operations in any order. The worst-case number of operations is `n` (size of input), each O(1), so total time O(n). Space is O(1) for the queue array plus O(1) for indices, and O(n) for the input vector itself (not counted as auxiliary because it is provided).

#include <vector>
#include <iostream>

// Simulates a circular queue of capacity 5 and returns sum of successful operation values.
int simulateQueueOperations(const std::vector<int>& operations, bool verbose = true) {
    const int MAX_SIZE = 5;
    int arr[MAX_SIZE] = {0};
    int front = 0;
    int rear = 0;
    int totalSum = 0;

    for (int op : operations) {
        if (op > 0) { // enqueue
            if (front == (rear + 1) % MAX_SIZE) { // full
                if (verbose) std::cout << "Queue가 꽉찼습니다." << std::endl;
                continue;
            }
            arr[rear] = op;
            if (verbose) std::cout << "삽입 : " << op << "(" << rear << "번째)" << std::endl;
            totalSum += op;
            rear = (rear + 1) % MAX_SIZE;
        } else if (op == 0) { // dequeue
            if (front == rear) { // empty
                if (verbose) std::cout << "Queue가 비어있습니다." << std::endl;
                continue;
            }
            int value = arr[front];
            if (verbose) std::cout << "삭제 : " << value << "(" << front << "번째)" << std::endl;
            totalSum += value;
            front = (front + 1) % MAX_SIZE;
        } else if (op == -1) { // peek
            if (front == rear) { // empty
                if (verbose) std::cout << "Queue가 비어있습니다." << std::endl;
                continue;
            }
            if (verbose) std::cout << "Front : " << arr[front] << std::endl;
            totalSum += arr[front];
        }
    }
    return totalSum;
}

#include <cassert>
#include <vector>
#include <iostream>

// Function declaration (for completeness; in actual test, include the solution above)
int simulateQueueOperations(const std::vector<int>&, bool);

int main() {
    // Basic operations from the original snippet
    assert(simulateQueueOperations({3,7,5,-1,0,0,-1}, false) == (3+7+5+3+7+3)); // sum: enq 3,7,5; peek 3; deq 3,7; peek 3
    // Empty queue operations
    assert(simulateQueueOperations({0,-1,0}, false) == 0); // all fail, sum 0
    // Full queue: insert 5 elements, then extra insert fails
    assert(simulateQueueOperations({1,2,3,4,5,6}, false) == (1+2+3+4+5)); // 6 fails, sum 15
    // Wrapping: fill, dequeue twice, insert two more
    assert(simulateQueueOperations({1,2,3,4,5,0,0,6,7}, false) == (1+2+3+4+5+1+2+6+7)); // sum all successful
    // Mix: enqueue, peek, dequeue, dequeue on empty, enqueue
    assert(simulateQueueOperations({10,-1,0,0,20}, false) == (10+10+10+20)); // peek adds 10, deq adds 10, second deq fails, enq adds 20
    // Single element then peek after dequeue
    assert(simulateQueueOperations({42,0,-1}, false) == (42+42)); // deq returns 42, peek fails
    // Negative enqueue values are not allowed per spec but ignore: we only handle >0
    assert(simulateQueueOperations({5,5,5,5,5,0,0,0,0,0,0}, false) == 25); // five enq, five deq all succeed, sixth deq fails (empty)
    // Increasing then wrapping: fill and read all
    assert(simulateQueueOperations({1,2,3,4,5,0,0,0,0,0}, false) == (1+2+3+4+5+1+2+3+4+5)); // all ten sum = 30
    // Peek on full queue (only if non-empty)
    assert(simulateQueueOperations({7,8,-1}, false) == (7+8+7)); // peek adds front 7
    // Large sequence with many failures
    assert(simulateQueueOperations({1,0,0,2,0,3,0}, false) == (1+1+2+2+3)); // enq1, deq1, fail, enq2, deq2, enq3, deq3
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
