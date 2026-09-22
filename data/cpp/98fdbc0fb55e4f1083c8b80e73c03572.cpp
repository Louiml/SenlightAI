/*
Design a C++ function named `manageSeats` that simulates a seat reservation system for a theater with seats numbered from 1 to `n` (inclusive). The function should take an integer `n` (the total number of seats) and a vector of strings representing a sequence of operations. Each operation is either `"reserve"` (to reserve the lowest-numbered available seat) or `"unreserve X"` (to make seat number `X` available again; you may assume `X` is always a valid seat that was previously reserved and is currently not available). The function should process the operations in order and return a vector of integers containing the seat numbers returned by each `"reserve"` operation, in the order they were reserved. If `n` is less than or equal to 0, return an empty vector. The function must handle up to 10^5 operations efficiently, including many interleaved reserves and unreserves. For example, with `n = 5` and operations `["reserve", "reserve", "unreserve 2", "reserve", "reserve"]`, the return should be `[1, 2, 2, 3]`.
*/

#include <vector>
#include <string>
#include <queue>
#include <sstream>

// Simulate seat management. Returns seat numbers reserved in order.
std::vector<int> manageSeats(int n, const std::vector<std::string>& operations) {
    std::vector<int> result;
    if (n <= 0) return result;

    // Min-heap to track available seats.
    std::priority_queue<int, std::vector<int>, std::greater<int>> available;
    for (int i = 1; i <= n; ++i) {
        available.push(i);
    }

    for (const auto& op : operations) {
        if (op == "reserve") {
            if (!available.empty()) {
                int seat = available.top();
                available.pop();
                result.push_back(seat);
            }
        } else {
            // Format: "unreserve X"
            std::istringstream iss(op);
            std::string command;
            int seatNumber;
            iss >> command >> seatNumber;
            available.push(seatNumber);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic example from task
    std::vector<std::string> ops1 = {"reserve", "reserve", "unreserve 2", "reserve", "reserve"};
    assert(manageSeats(5, ops1) == std::vector<int>({1, 2, 2, 3}));

    // Single reserve
    std::vector<std::string> ops2 = {"reserve"};
    assert(manageSeats(1, ops2) == std::vector<int>({1}));

    // Unreserve lowest then reserve again
    std::vector<std::string> ops3 = {"reserve", "reserve", "reserve", "unreserve 1", "reserve"};
    assert(manageSeats(3, ops3) == std::vector<int>({1, 2, 3, 1}));

    // n = 0 returns empty
    std::vector<std::string> ops4 = {"reserve"};
    assert(manageSeats(0, ops4) == std::vector<int>());

    // Repeated unreserve of same seat (valid if reserved multiple times) — test with valid sequence
    std::vector<std::string> ops5 = {"reserve", "unreserve 1", "unreserve 1", "reserve"};
    assert(manageSeats(3, ops5) == std::vector<int>({1, 1}));

    // Larger n with interleaving
    std::vector<std::string> ops6 = {"reserve", "reserve", "unreserve 1", "reserve", "unreserve 2", "reserve"};
    assert(manageSeats(5, ops6) == std::vector<int>({1, 2, 1, 2}));

    // Ensure lowest available always chosen after unreserve
    std::vector<std::string> ops7 = {"reserve", "reserve", "unreserve 2", "reserve", "reserve"};
    assert(manageSeats(4, ops7) == std::vector<int>({1, 2, 2, 1}));

    // Check no reserve when n positive
    std::vector<std::string> ops8 = {};
    assert(manageSeats(3, ops8) == std::vector<int>());

    return 0;
}

// The core idea is to use a min-heap (a priority queue that always gives the smallest element first) to track available seats. Initially, all seats from 1 to `n` are pushed into the heap. For each `"reserve"` operation, we pop the smallest seat number from the heap (which is guaranteed to be available) and record it in the result vector. For each `"unreserve X"` operation, we push `X` back into the heap, making it available again. Edge cases: if `n <= 0`, there are no seats, so we return an empty vector; also, if a `"reserve"` is called when the heap is empty (which should not happen if the operations are valid — we can assume input is valid, but we could guard against it by ignoring or handling gracefully, but for simplicity we assume validity). The time complexity is O((n + m) log n) where `m` is the number of operations, because each push or pop on a priority queue of size up to `n` takes O(log n) time. The space complexity is O(n) for the heap and O(result size) for the output vector.
