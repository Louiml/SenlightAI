// Write a C++ function `int findMajorityElement(const std::vector<int>& arr, int l, int r)` that, given a 1-indexed array `arr` (size `n`, values in `[1, n]`) and a query range `[l, r]` (1-indexed, inclusive), returns any value `v` such that `v` appears more than `(r - l + 1) / 2` times in `arr[l..r]` (i.e., the strict majority), or returns `0` if no such value exists. The function must use a randomized Monte Carlo approach: sample up to 20 random positions from the range, check each sampled value's frequency using precomputed position lists, and return the first sampled value that qualifies as a majority; if none qualify, return `0`. The function should handle the case where `l > r` by returning `0`.

#include <cassert>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    srand(12345); // deterministic for reproducibility

    // Example: n=5, arr = [1,2,2,2,3] (1-indexed)
    std::vector<int> arr = {0, 1, 2, 2, 2, 3}; // index 0 unused
    std::vector<std::vector<int>> positions(4); // values 1..3
    for (int i = 1; i <= 5; ++i) {
        positions[arr[i]].push_back(i);
    }

    // Range [2,4] => values {2,2,2} => majority 2
    assert(findMajorityElement(arr, positions, 2, 4) == 2);
    // Range [1,5] => {1,2,2,2,3} => majority 2
    assert(findMajorityElement(arr, positions, 1, 5) == 2);
    // Range [1,2] => {1,2} => no majority => 0 (with high probability, but deterministic here)
    // Because with 20 samples, if the candidate is 1 or 2, neither has >1 occurrence, so 0.
    assert(findMajorityElement(arr, positions, 1, 2) == 0);
    // Range [3,3] => single element => majority 2
    assert(findMajorityElement(arr, positions, 3, 3) == 2);
    // Invalid range
    assert(findMajorityElement(arr, positions, 4, 2) == 0);

    // Larger test with no majority
    std::vector<int> arr2 = {0, 1, 2, 3, 4, 5}; // values 1..5, all distinct
    std::vector<std::vector<int>> pos2(6);
    for (int i = 1; i <= 5; ++i) {
        pos2[arr2[i]].push_back(i);
    }
    // Range [1,5] has no majority
    assert(findMajorityElement(arr2, pos2, 1, 5) == 0);

    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdlib>

// Precomputed position lists per value (1-indexed values). This is passed as a parameter.
// The function returns a value that appears more than half of the range, or 0 if none.
int findMajorityElement(const std::vector<int>& arr,
                        const std::vector<std::vector<int>>& positions,
                        int l, int r) {
    if (l > r) return 0;
    int len = r - l + 1;
    const int NUM_SAMPLES = 20;

    for (int k = 0; k < NUM_SAMPLES; ++k) {
        // Uniformly sample an index in [l, r]
        int idx = l + (rand() % len);
        int candidate = arr[idx];

        // Count occurrences of candidate in [l, r] using positions list
        const std::vector<int>& posList = positions[candidate]; // candidate is within [1, n]
        auto itLow = std::lower_bound(posList.begin(), posList.end(), l);
        auto itHigh = std::upper_bound(posList.begin(), posList.end(), r);
        int count = static_cast<int>(itHigh - itLow);

        if (count * 2 > len) {
            return candidate;
        }
    }
    return 0;
}

// The algorithm leverages the fact that, if a strict majority exists, it occupies more than half the range. By randomly sampling a small number of positions (e.g., 20), the probability of missing the majority element is at most `(1/2)^20` per query, which is negligible. For each sampled value, we need to count its occurrences in the range efficiently: precompute for each possible value a sorted vector of all indices where it appears. Then, using binary search (`lower_bound` and `upper_bound`), we can count occurrences in `O(log n)` time. The main steps: (1) precompute position lists for all values; (2) for each query, uniformly sample an index from the range, get the value at that index, count its occurrences in the range via the position list, and check if it exceeds half the length. Edge cases: `l > r` returns `0`; if the range length is 1, the single element is trivially the majority; if multiple values qualify (impossible for strict majority), the first found is returned. Time complexity: Preprocessing `O(n)` to build position lists; each query takes `O(20 * log n)` due to binary searches and sampling, which is effectively constant for typical sizes. Space complexity: `O(n)` for the position vectors (total elements across all vectors is `n`).
