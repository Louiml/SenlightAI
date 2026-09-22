// Write a C++ function `vector<int> possibleCValues(int capacityA, int capacityB, int capacityC)` that simulates the classic "pour water between three buckets" problem. Initially, bucket A is empty, bucket B is empty, and bucket C is full to its capacity. You may repeatedly pour water from one bucket into another until either the source bucket is empty or the destination bucket is full (no water is lost). The function must return a sorted list (in increasing order) of all possible amounts of water that can end up in bucket C when bucket A is empty (i.e., when the simulation reaches a state where bucket A has 0 units). The capacities are positive integers up to 20. The function should explore all reachable states using depth-first search and avoid revisiting the same state (a, b, c). Return the list as a `std::vector<int>`. The returned vector may be empty if no such state is reachable with bucket A empty (though in practice at least one will exist).
#include <cassert>
#include <vector>
#include <functional>
#include <array>
#include <algorithm>

// Include the solution function here (or link to it).
// For brevity, assume the function is defined above.

int main() {
    // Test 1: Example from typical problem: capacities 8, 9, 10
    std::vector<int> result1 = possibleCValues(8, 9, 10);
    std::vector<int> expected1 = {1, 2, 8, 9, 10};
    assert(result1 == expected1);

    // Test 2: Small capacities, e.g., A=1, B=1, C=2
    std::vector<int> result2 = possibleCValues(1, 1, 2);
    std::vector<int> expected2 = {0, 1, 2};
    assert(result2 == expected2);

    // Test 3: All capacities equal 1
    std::vector<int> result3 = possibleCValues(1, 1, 1);
    std::vector<int> expected3 = {1};
    assert(result3 == expected3);

    // Test 4: A is larger than C, but we always start with C full
    std::vector<int> result4 = possibleCValues(5, 5, 3);
    // Possible C values: 0,1,2,3 (verify manually or via known simulation)
    std::vector<int> expected4 = {0, 1, 2, 3};
    assert(result4 == expected4);

    // Test 5: Maximum capacities (20 each) just to ensure no crash and reasonable result
    std::vector<int> result5 = possibleCValues(20, 20, 20);
    // At minimum, the initial state (0,0,20) gives c=20
    assert(!result5.empty());
    assert(result5.front() >= 0 && result5.back() <= 20);
    assert(std::is_sorted(result5.begin(), result5.end()));

    // Test 6: Very small capacity A=0? But capacities are positive, so A=1, B=2, C=3
    std::vector<int> result6 = possibleCValues(1, 2, 3);
    std::vector<int> expected6 = {0, 1, 2, 3};
    assert(result6 == expected6);

    return 0;
}
#include <vector>
#include <array>
#include <algorithm>

// Simulate pouring between three buckets and return all possible C amounts when A is empty.
std::vector<int> possibleCValues(int capacityA, int capacityB, int capacityC) {
    // visited[a][b][c] = true if we have already explored this state.
    std::array<std::array<std::array<bool, 21>, 21>, 21> visited{};
    std::vector<int> results;

    // Depth-first search over all reachable states.
    // Use a lambda with std::function to allow recursion.
    std::function<void(int, int, int)> dfs = [&](int a, int b, int c) {
        if (visited[a][b][c]) return;
        visited[a][b][c] = true;

        if (a == 0) {
            results.push_back(c);
        }

        // Compute available space in each bucket.
        int spaceA = capacityA - a;
        int spaceB = capacityB - b;
        int spaceC = capacityC - c;

        // Pour from A to B
        int pour = std::min(a, spaceB);
        dfs(a - pour, b + pour, c);

        // Pour from A to C
        pour = std::min(a, spaceC);
        dfs(a - pour, b, c + pour);

        // Pour from B to A
        pour = std::min(b, spaceA);
        dfs(a + pour, b - pour, c);

        // Pour from B to C
        pour = std::min(b, spaceC);
        dfs(a, b - pour, c + pour);

        // Pour from C to A
        pour = std::min(c, spaceA);
        dfs(a + pour, b, c - pour);

        // Pour from C to B
        pour = std::min(c, spaceB);
        dfs(a, b + pour, c - pour);
    };

    dfs(0, 0, capacityC);

    // Remove duplicates and sort.
    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());
    return results;
}
// The problem is a state-space search over all possible triples `(a, b, c)` representing current water amounts in buckets A, B, and C. Since each capacity is at most 20, there are at most \(21 \times 21 \times 21 = 9261\) states, which is tiny. Use a 3D boolean visited array (or `std::set` of tuples) to mark visited states. Starting from `(0, 0, capacityC)`, for each state, if `a == 0`, record `c` as a valid outcome. Then for each of the 6 possible pour operations (A→B, A→C, B→A, B→C, C→A, C→B), compute the new state by pouring as much as possible: the amount poured is `min(source, destinationCapacity - destinationAmount)`. Update source and destination accordingly and recurse. To avoid stack overflow, recursion depth is bounded by number of states (≤9261), so it's safe. After the search completes, collect all valid `c` values into a set, sort them, and return as a vector. Edge cases: capacities can be equal, pouring when source is empty or destination is full results in no change (but we should avoid infinite loops by visited check). Time complexity is O(number of states × 6) = O(1) effectively, space complexity O(number of states). The initial state always has a=0, so at least one valid c (the full capacity) will be present.
