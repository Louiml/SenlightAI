// Write a C++ function `countActiveBins` that takes a vector of non-negative integer scores (representing bin indices for a dense binned score vector, where 0 means inactive and values 1..maxBin are active bins), and returns the number of distinct active bin labels present in the input. The input vector may contain duplicates, may have values beyond a given maximum bin count (which should be ignored if they exceed `maxBin`), and must be processed in a single pass without modifying the original vector. The function should accept the vector by const reference, along with an integer `maxBin` (1 <= maxBin <= 10^6), and return a `uint32_t` count. The count must treat bin 0 as inactive and never count it, and bins above `maxBin` are ignored entirely.

The problem reduces to counting unique values in a range [1, maxBin] from a vector while ignoring 0 and values > maxBin. The simplest approach is to use a boolean or bitset vector of size `maxBin + 1` (including index 0, but we can ignore it) to mark which bins have been seen. Traverse the input once; for each value `v`, if `v >= 1 && v <= maxBin` and the marker at `v` is false, set it true and increment the counter. This gives O(n + maxBin) time for traversal and initialization of the marker (though the marker initialization could be O(maxBin), which may dominate if maxBin is large but acceptable under the constraints). Alternatively, use `std::vector<char>` or `std::vector<bool>`. Edge cases: empty input returns 0; all zeros returns 0; values beyond maxBin are skipped; duplicates are counted once; maxBin=1 only counts value 1. Space complexity is O(maxBin) for the marker, or O(maxBin/8) if using bitset, but `std::vector<bool>` is space-efficient. If we want to avoid O(maxBin) initialization, we could use a hash set, but given the constraints (maxBin up to 1e6), a boolean vector is fine and deterministic. Time complexity O(n + maxBin) but the initialization of the bool vector is O(maxBin). We can use `std::vector<char>` and `memset` or initialize with zeros. For performance, use `std::vector<bool>` which packs bits.

#include <cstdint>
#include <vector>

// Count distinct active bin labels (1..maxBin) in the input scores.
// Values <=0 or >maxBin are ignored. Duplicates counted once.
uint32_t countActiveBins(const std::vector<int>& scores, int maxBin) {
    // Use a boolean vector to mark seen bins. Index 0 is unused.
    // std::vector<bool> is space-efficient.
    std::vector<bool> seen(maxBin + 1, false);
    uint32_t count = 0;

    for (int v : scores) {
        if (v >= 1 && v <= maxBin && !seen[v]) {
            seen[v] = true;
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Declaration or include of the solution function goes here.
// For this standalone test, we just re-declare it.
uint32_t countActiveBins(const std::vector<int>& scores, int maxBin);

int main() {
    // Basic case: distinct bins
    assert(countActiveBins({1, 2, 3}, 5) == 3);
    // Duplicates
    assert(countActiveBins({2, 2, 3, 3}, 5) == 2);
    // Zeros ignored
    assert(countActiveBins({0, 1, 0, 2}, 5) == 2);
    // Values above maxBin ignored
    assert(countActiveBins({1, 100, 2}, 5) == 2);
    // Empty input
    assert(countActiveBins({}, 5) == 0);
    // Only zeros
    assert(countActiveBins({0, 0}, 5) == 0);
    // maxBin = 1, only value 1 counts
    assert(countActiveBins({1, 1, 2, 0}, 1) == 1);
    // Negative values ignored
    assert(countActiveBins({-1, 3, -5}, 5) == 1);
    // All possible bins
    assert(countActiveBins({1,2,3,4,5}, 5) == 5);
    // Large maxBin but few values
    assert(countActiveBins({5}, 1000000) == 1);
    return 0;
}
