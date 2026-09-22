Write a C++ function named `fcfsAverageSeekTime` that takes a `const std::vector<int>&` representing the disk request sequence (in the order they arrive), and an integer `initialHeadPosition` representing the starting position of the disk head. The function must return the average seek time (as a `double`) for the First-Come, First-Served (FCFS) disk scheduling algorithm. The average is defined as the total absolute distance the head travels divided by the number of requests in the sequence. The vector may contain any integers (including negatives, which represent positions on the other side of the disk relative to an arbitrary origin), may be empty, or may contain duplicate values. Handle the empty vector case by returning `0.0`. Do not modify the input vector.

// The FCFS algorithm processes requests exactly in the order they appear. The total seek distance is the sum of absolute differences: first from `initialHeadPosition` to the first request, then between consecutive requests. We iterate once over the vector, maintaining a `previous` position (starting as the initial head position). For each request, add `abs(request - previous)` to a `total` accumulator, then update `previous` to the current request. After the loop, if the vector is empty, return `0.0` (since no requests means no seek time). Otherwise, divide `total` by the number of requests and return as a `double`. Time complexity is O(n) where n is the number of requests; space complexity is O(1) beyond the input vector (we only use a few scalar variables). Edge cases: single request → the result is just `abs(request - initial)/1`; duplicate values contribute zero extra distance between consecutive duplicates; negative positions work because `std::abs` handles them; empty vector must not cause division by zero.

#include <vector>
#include <cmath>
#include <cstddef>

// Returns the average seek time for FCFS disk scheduling.
// Takes a request queue (unchanged) and the initial head position.
// Returns 0.0 for an empty request queue.
double fcfsAverageSeekTime(const std::vector<int>& requests, int initialHeadPosition) {
    if (requests.empty()) {
        return 0.0;
    }

    double totalSeek = 0.0;
    int previous = initialHeadPosition;

    for (const int request : requests) {
        totalSeek += std::abs(request - previous);
        previous = request;
    }

    return totalSeek / static_cast<double>(requests.size());
}

#include <cassert>
#include <vector>
#include <cmath>

// Declaration of the function under test (must match the solution).
double fcfsAverageSeekTime(const std::vector<int>& requests, int initialHeadPosition);

int main() {
    // Simple non-empty sequence
    assert(std::abs(fcfsAverageSeekTime({98, 183, 37, 122, 14, 124, 65, 67}, 53) - 55.75) < 1e-9);
    // Single request: distance from head to that request
    assert(std::abs(fcfsAverageSeekTime({10}, 0) - 10.0) < 1e-9);
    // Duplicates contribute zero between themselves
    assert(std::abs(fcfsAverageSeekTime({5, 5, 5}, 5) - 0.0) < 1e-9);
    // Negative positions
    assert(std::abs(fcfsAverageSeekTime({-10, 20, -5}, 0) - (10.0 + 30.0 + 25.0)/3.0) < 1e-9);
    // Head already at the first request position
    assert(std::abs(fcfsAverageSeekTime({42, 42, 42}, 42) - 0.0) < 1e-9);
    // Empty vector returns 0.0
    assert(fcfsAverageSeekTime({}, 100) == 0.0);
    // Two requests, typical
    assert(std::abs(fcfsAverageSeekTime({20, 40}, 10) - ((10+20)/2.0)) < 1e-9);
    // Large values
    assert(std::abs(fcfsAverageSeekTime({1000000, -1000000}, 0) - 2000000.0/2.0) < 1e-9);
}
