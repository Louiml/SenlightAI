You are managing an airplane parking lot with `g` gates numbered `1` through `g`. The gates are initially all available. A sequence of `p` planes will arrive in order; for the `i`-th plane, you are given an integer `a` (1 ≤ a ≤ g) representing its preferred gate. When a plane arrives, you must assign it the highest-numbered available gate that is ≤ `a`. If no such gate exists, the plane cannot park, and the process stops immediately (no further planes are considered). Write a C++ function `int parkPlanes(int g, const std::vector<int>& arrivals)` that returns the total number of planes successfully parked.

The problem mimics a greedy assignment with a "nearest smaller available" rule. Maintain a balanced BST (or sorted set) of all currently available gates, initially containing `1..g`. For each preferred gate `a`, find the greatest available gate ≤ `a`. If one exists, park the plane there (remove it from the set) and increment the count. If none exists, stop processing and return the count. Edge cases: no available gates at all (return 0 immediately or on first failure), and all planes fit. The balanced set allows O(log g) per query. Total time O(p log g), space O(g).

#include <set>
#include <vector>

// Assign planes to gates greedily with nearest-smaller rule.
// Returns number of successful parkings.
int parkPlanes(int g, const std::vector<int>& arrivals) {
    std::set<int> available;
    for (int i = 1; i <= g; ++i) {
        available.insert(i);
    }
    
    int parked = 0;
    for (int a : arrivals) {
        // Find first gate > a, then step back to the greatest <= a
        auto it = available.upper_bound(a);
        if (it == available.begin()) {
            // No gate <= a exists
            break;
        }
        --it;
        // Safe: *it is the greatest available <= a
        available.erase(it);
        ++parked;
    }
    return parked;
}

#include <cassert>
#include <vector>

int parkPlanes(int g, const std::vector<int>& arrivals);

int main() {
    // All planes fit exactly in order
    assert(parkPlanes(5, {1,2,3,4,5}) == 5);
    // Some planes park at lower gates than preferred
    assert(parkPlanes(3, {3,3,3}) == 3);
    // First plane parks at 2, second at 1, third fails
    assert(parkPlanes(2, {2,2,1}) == 2);
    // No gates available from start (should not happen with g>=1, but test g=0)
    assert(parkPlanes(0, {1,2}) == 0);
    // Failure on first plane
    assert(parkPlanes(1, {2,1}) == 0);
    // All gates filled, then extra plane fails
    assert(parkPlanes(3, {3,2,1,3}) == 3);
    // Mixed assignments
    assert(parkPlanes(4, {4,3,2,1,4,3}) == 4);
    // Only one gate, multiple planes using it once
    assert(parkPlanes(1, {1,1,1}) == 1);
    // Stop immediately at first failure
    assert(parkPlanes(5, {2,5,4,1,3}) == 5);
    // Larger case
    assert(parkPlanes(10, {5,5,5,5,5,5,5,5,5,5,5}) == 5);
    
    return 0;
}
