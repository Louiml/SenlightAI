// Create a C++ function that simulates a simplified round-robin CPU scheduler using a fixed time slot. The function should accept a `std::queue<int>` of process execution times (positive integers) and an integer `time_slice` (positive integer) as parameters. It must process the queue by repeatedly dequeuing a process, reducing its remaining time by the time_slice, and re-enqueuing it if it still has time remaining (i.e., original remaining time was greater than the time slice). Return a new `std::queue<int>` representing the final state of the queue after all processes have been completely executed (the returned queue must be empty). Ensure the function handles edge cases like an empty input queue or processes with times exactly equal to the time slice. The function should not modify the original queue (pass by const reference).

#include <cassert>
#include <queue>

// Declaration of the function under test (if not in same file, include header)
std::queue<int> roundRobinSchedule(const std::queue<int>& input_queue, int time_slice);

int main() {
    // Test 1: Empty input queue
    std::queue<int> empty_q;
    assert(roundRobinSchedule(empty_q, 5).empty());

    // Test 2: Single process with time exactly equal to time slice
    std::queue<int> q1;
    q1.push(5);
    assert(roundRobinSchedule(q1, 5).empty());

    // Test 3: Single process with time greater than time slice (e.g., 10, slice=3)
    std::queue<int> q2;
    q2.push(10);
    assert(roundRobinSchedule(q2, 3).empty());

    // Test 4: Multiple processes, all less than or equal to time slice
    std::queue<int> q3;
    q3.push(2);
    q3.push(4);
    q3.push(5);
    assert(roundRobinSchedule(q3, 5).empty());

    // Test 5: Multiple processes with some larger than time slice
    std::queue<int> q4;
    q4.push(7);
    q4.push(2);
    q4.push(9);
    q4.push(5);
    assert(roundRobinSchedule(q4, 3).empty());

    // Test 6: Verify original queue is not modified
    std::queue<int> original;
    original.push(8);
    original.push(6);
    original.push(4);
    std::queue<int> original_copy = original;
    roundRobinSchedule(original, 4);
    assert(original == original_copy);

    // Test 7: Edge with large time and small slice
    std::queue<int> q5;
    q5.push(100);
    q5.push(1);
    q5.push(50);
    assert(roundRobinSchedule(q5, 2).empty());

    // Test 8: Single process with time 1, slice 10 (less than slice)
    std::queue<int> q6;
    q6.push(1);
    assert(roundRobinSchedule(q6, 10).empty());

    // Test 9: All processes exactly equal to time slice
    std::queue<int> q7;
    q7.push(3);
    q7.push(3);
    q7.push(3);
    assert(roundRobinSchedule(q7, 3).empty());

    // Test 10: Mixed edge values (time=0 is not allowed per spec, but we assume positive; here use 1)
    std::queue<int> q8;
    q8.push(1);
    q8.push(2);
    q8.push(3);
    assert(roundRobinSchedule(q8, 1).empty());

    return 0;
}

#include <queue>

// Simulates round-robin scheduling with a fixed time slice.
// Returns an empty queue after all processes have completed.
std::queue<int> roundRobinSchedule(const std::queue<int>& input_queue, int time_slice) {
    std::queue<int> work_queue = input_queue; // Copy to avoid modifying original

    while (!work_queue.empty()) {
        int current_time = work_queue.front();
        work_queue.pop();

        if (current_time > time_slice) {
            work_queue.push(current_time - time_slice);
        }
        // If current_time <= time_slice, process is complete (not re-enqueued)
    }

    // All processes are done; return an empty queue per specification.
    return std::queue<int>();
}

// The solution simulates round-robin scheduling: each process at the front of the queue gets the CPU for exactly `time_slice` units of time. If its remaining time is greater than the time slice, it is moved to the back with its time reduced by `time_slice`; otherwise it is considered complete and not re-enqueued. The main algorithm loops while the queue is not empty, performing a pop and conditionally a push. Edge cases: if the queue is initially empty, return immediately. If a process has time exactly equal to `time_slice`, it is completed after one execution. All input times are positive integers, and `time_slice` is positive. Since we are simulating the entire execution, the loop runs until all processes finish. Time complexity is \(O(n \cdot \lceil \max\_time / time\_slice \rceil)\) in the worst case, but more precisely it is \(O(\text{total execution slots})\), where each iteration is constant time. Space complexity is \(O(1)\) auxiliary space beyond the input queue (and the output which is empty). The function takes the queue by const reference to avoid modification, creates a local copy to work with, and returns an empty queue (or the local copy if we want, but the specification says final state must be empty, so we return an empty queue after processing).
