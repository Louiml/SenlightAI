// Write a C++ function `countLargerSeen(const std::vector<long long>& arr)` that, for each element in the input array, returns a vector of the same length where the value at position `i` is the number of elements that appear *after* index `i` in the original array and are **strictly larger** than `arr[i]`. For example, given `{5, 2, 6, 1}`, the output should be `{1, 1, 0, 0}` because: for `5`, only `6` is larger and after it; for `2`, both `6` and (wait, `6` and nothing else? actually `6` is larger and after, also `1` is not larger) so count is 1; for `6`, nothing larger after; for `1`, nothing larger after. The input may contain duplicates, and all values are within the range of a 32-bit signed integer. The function must be efficient for large `n` (up to 2e5). You are not allowed to use a naive O(n²) approach.
// The solution processes the array from right to left, maintaining a data structure that can answer "how many elements greater than X have been seen so far?" This is naturally a Fenwick tree (Binary Indexed Tree) or a segment tree over the value domain. Since values can be up to 1e9 (or even negative), we first coordinate-compress the values to ranks 1..k (where k is the number of distinct values). Then, as we iterate from the last element to the first, we query the number of ranks greater than the current element's rank, which gives the count of larger elements to its right. After the query, we update the Fenwick tree by adding 1 at the current rank. Edge cases: duplicates are handled naturally by rank compression (equal values map to same rank), and we must convert original values to ranks properly (higher value → higher rank). Time complexity is O(n log n) due to sorting for compression and each query/update being O(log n). Space complexity is O(n) for the compressed array and Fenwick tree.
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns, for each position, the count of later elements that are strictly larger.
std::vector<long long> countLargerSeen(const std::vector<long long>& arr) {
    int n = (int)arr.size();
    if (n == 0) return {};

    // Coordinate compression: sort unique values, map each to a rank 1..k.
    std::vector<long long> sorted = arr;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    int k = (int)sorted.size();

    // Fenwick tree for cumulated frequencies, 1-indexed.
    std::vector<int> bit(k + 1, 0);

    auto bit_add = [&](int idx, int delta) {
        while (idx <= k) {
            bit[idx] += delta;
            idx += idx & -idx;
        }
    };

    auto bit_sum = [&](int idx) {
        int s = 0;
        while (idx > 0) {
            s += bit[idx];
            idx -= idx & -idx;
        }
        return s;
    };

    std::vector<long long> result(n, 0);

    // Process from right to left.
    for (int i = n - 1; i >= 0; --i) {
        // rank of current value (1-based), rank = lower_bound + 1
        int rank = int(std::lower_bound(sorted.begin(), sorted.end(), arr[i]) - sorted.begin()) + 1;

        // How many larger values (rank > current) are already seen?
        int larger = bit_sum(k) - bit_sum(rank);
        result[i] = larger;

        // Insert current value.
        bit_add(rank, 1);
    }

    return result;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic example.
    std::vector<long long> a1 = {5, 2, 6, 1};
    std::vector<long long> r1 = countLargerSeen(a1);
    assert(r1 == std::vector<long long>({1, 1, 0, 0}));

    // All equal.
    std::vector<long long> a2 = {3, 3, 3};
    std::vector<long long> r2 = countLargerSeen(a2);
    assert(r2 == std::vector<long long>({0, 0, 0}));

    // Strictly increasing.
    std::vector<long long> a3 = {1, 2, 3, 4};
    std::vector<long long> r3 = countLargerSeen(a3);
    assert(r3 == std::vector<long long>({3, 2, 1, 0}));

    // Strictly decreasing.
    std::vector<long long> a4 = {4, 3, 2, 1};
    std::vector<long long> r4 = countLargerSeen(a4);
    assert(r4 == std::vector<long long>({0, 0, 0, 0}));

    // With negative numbers and duplicates.
    std::vector<long long> a5 = {-1, 0, -1, 2, 0};
    std::vector<long long> r5 = countLargerSeen(a5);
    // For -1 at index 0: larger later: 0,2,0 (three) but -1 also later? index2 is -1 not larger, so 3.
    // For 0 at index1: larger later: 2 (only) -> 1
    // For -1 at index2: larger later: 2,0 -> 2
    // For 2 at index3: none -> 0
    // For 0 at index4: none -> 0
    assert(r5 == std::vector<long long>({3, 1, 2, 0, 0}));

    // Empty input.
    std::vector<long long> a6 = {};
    assert(countLargerSeen(a6).empty());

    // Single element.
    std::vector<long long> a7 = {7};
    assert(countLargerSeen(a7) == std::vector<long long>({0}));

    // Large values.
    std::vector<long long> a8 = {1000000000LL, -1000000000LL, 0};
    std::vector<long long> r8 = countLargerSeen(a8);
    // For 1e9: none -> 0
    // For -1e9: both 1e9 and 0 are larger -> 2
    // For 0: only 1e9 is larger -> 1
    assert(r8 == std::vector<long long>({0, 2, 1}));

    return 0;
}
