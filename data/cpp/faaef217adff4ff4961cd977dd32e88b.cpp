Write a C++ function `bookExchangeCycleLengths` that takes a vector of integers representing a permutation (each element is a distinct value from 0 to n-1, but in the task they are given as 1-indexed and you must internally convert) and returns a vector of the same length where each position `i` contains the number of steps needed to return to position `i` when repeatedly following the mapping `next = perm[current]` starting from `i`. The input vector contains values in the range `[1, n]` representing a 1-based permutation, and your function must return a 0-based vector of cycle lengths (i.e., for each starting index, how many jumps until you are back at that same index). The function must work for any `n` ≥ 1 and must not modify the input. Handle the case where the permutation is already the identity (each element maps to itself) correctly.
// The problem is a classic cycle-detection in a functional graph where each node has exactly one outgoing edge (since it's a permutation). For each starting index `i`, we simulate following the mapping until we return to `i`. Because every node has out-degree exactly 1, the graph consists solely of disjoint cycles, so starting from any node, you will eventually return to it. The naive approach for each `i` is to do a `do-while` loop: set `pos = i`, then repeatedly replace `pos` with `perm[pos]` (converting 1-based input to 0-based by subtracting 1 from each value when reading), and increment a counter until `pos` equals the original `i`. This yields the cycle length for that start. The time complexity is O(n^2) in the worst case because a cycle of length L is traversed L times for each of its L nodes (e.g., one big cycle), but for the constraints of the easy version (n ≤ 200 in original problem) this is acceptable. Space complexity is O(n) for the answer vector. Edge cases: if `n == 1`, the loop runs exactly once and returns 1; if the permutation is identity, each loop runs once and returns 1; if there are multiple disjoint cycles, each node's loop traverses its own cycle fully, which is correct even though it repeats work. We do not need to reorder or reuse cycle lengths – the simple simulation is sufficient and correct for all permutations. Ensure we use `const` reference for input and convert each element to 0-based index when accessing.
#include <vector>

// Given a 1-based permutation `perm` of size n, return a vector where
// result[i] is the number of steps to return to index i when following
// the mapping next = perm[current] (with 1-based values converted to 0-based).
std::vector<int> bookExchangeCycleLengths(const std::vector<int>& perm) {
    const int n = static_cast<int>(perm.size());
    std::vector<int> answer(n, 0);

    for (int i = 0; i < n; ++i) {
        int pos = i;
        int count = 0;
        do {
            pos = perm[pos] - 1; // convert 1-based to 0-based
            ++count;
        } while (pos != i);
        answer[i] = count;
    }

    return answer;
}
#include <cassert>
#include <vector>

// Declaration for testing
std::vector<int> bookExchangeCycleLengths(const std::vector<int>& perm);

int main() {
    // Identity permutation: each cycle length 1
    std::vector<int> p1 = {1, 2, 3};
    assert(bookExchangeCycleLengths(p1) == std::vector<int>({1, 1, 1}));

    // Single swap: cycle lengths 2 for both
    std::vector<int> p2 = {2, 1};
    assert(bookExchangeCycleLengths(p2) == std::vector<int>({2, 2}));

    // Single 3-cycle: each node returns after 3 steps
    std::vector<int> p3 = {2, 3, 1};
    assert(bookExchangeCycleLengths(p3) == std::vector<int>({3, 3, 3}));

    // Mixed cycles: {1,3,2,5,4} -> cycles: (1) length 1, (2,3) length 2, (4,5) length 2
    std::vector<int> p4 = {1, 3, 2, 5, 4};
    assert(bookExchangeCycleLengths(p4) == std::vector<int>({1, 2, 2, 2, 2}));

    // n=1
    std::vector<int> p5 = {1};
    assert(bookExchangeCycleLengths(p5) == std::vector<int>({1}));

    // Larger cycle of size 4
    std::vector<int> p6 = {2, 3, 4, 1};
    assert(bookExchangeCycleLengths(p6) == std::vector<int>({4, 4, 4, 4}));

    // Random permutation from original problem example: n=4, perm={2,3,4,1} already tested; another: {3,1,2} => cycle length 3
    std::vector<int> p7 = {3, 1, 2};
    assert(bookExchangeCycleLengths(p7) == std::vector<int>({3, 3, 3}));

    // Mixed: {2,1,4,3} -> two cycles of length 2
    std::vector<int> p8 = {2, 1, 4, 3};
    assert(bookExchangeCycleLengths(p8) == std::vector<int>({2, 2, 2, 2}));

    // Large cycle 5
    std::vector<int> p9 = {2, 3, 4, 5, 1};
    assert(bookExchangeCycleLengths(p9) == std::vector<int>({5, 5, 5, 5, 5}));

    // Edge: identity of size 6
    std::vector<int> p10 = {1, 2, 3, 4, 5, 6};
    assert(bookExchangeCycleLengths(p10) == std::vector<int>({1, 1, 1, 1, 1, 1}));

    return 0;
}
