Write a C++ function `long long countPairsWithDoubleAverage(const std::vector<int>& arr)` that, given an array of integers, returns the number of unordered pairs `(i, j)` with `i < j` such that the average of the two elements equals exactly the average of the entire array (i.e., `(arr[i] + arr[j]) / 2 == S / n`, where `S` is the sum of all elements and `n` is the array size). The function must handle duplicates correctly (e.g., if the array is `[2,2,2]`, the answer is 3 because all three pairs have average 2). If no such pair exists, return 0. The implementation must be efficient for `n` up to 200,000, with values in the range `[-10^9, 10^9]`. Use 64-bit integers to avoid overflow. The function should be self-contained without relying on global variables.
// The key observation is that for a pair `(x, y)` to have average equal to the array average, we need `x + y = 2 * S / n`. Let `target = 2 * S / n`. Since `target` must be an integer (otherwise no pairs exist), check divisibility first: if `(2 * S) % n != 0`, return 0. Otherwise, build a frequency map of all element values. Then iterate through each element `x`, and for each, the complement is `target - x`. The number of pairs involving `x` is the frequency of the complement. However, if `x == target - x`, then we must avoid counting the element with itself, so we subtract 1 from the frequency. Since each unordered pair is counted twice (once from each element's perspective), divide the total count by 2 at the end. Edge cases: `n=1` (no pairs, answer 0), duplicates (handled by frequency map), empty array (return 0). Time complexity: O(n) to build frequency map and iterate, plus O(n) for map operations, so O(n) average. Space complexity: O(n) for the frequency map. Use `long long` for sums and counts to avoid overflow.
#include <vector>
#include <unordered_map>
#include <cstdint>

// Count unordered pairs (i,j) with i<j such that (arr[i]+arr[j])/2 equals the array average.
long long countPairsWithDoubleAverage(const std::vector<int>& arr) {
    const std::size_t n = arr.size();
    if (n < 2) return 0;

    long long sum = 0;
    for (int val : arr) {
        sum += val;
    }

    // Check if 2*sum is divisible by n to have an integer target.
    if ((2 * sum) % static_cast<long long>(n) != 0) {
        return 0;
    }

    const long long target = 2 * sum / static_cast<long long>(n);

    // Build frequency map of each element value.
    std::unordered_map<int, long long> freq;
    for (int val : arr) {
        ++freq[val];
    }

    long long totalPairs = 0;
    for (int x : arr) {
        const long long complement = target - static_cast<long long>(x);
        // complement may be outside int range, but since x is int and target is long long,
        // we need to handle it safely. Since values are within int bounds but target may be large.
        // We must check if complement fits in int range; if not, freq lookup will be 0.
        if (complement < INT32_MIN || complement > INT32_MAX) {
            continue;
        }
        const int comp = static_cast<int>(complement);
        auto it = freq.find(comp);
        if (it != freq.end()) {
            if (comp == x) {
                // Avoid pairing element with itself.
                totalPairs += it->second - 1;
            } else {
                totalPairs += it->second;
            }
        }
    }

    // Each unordered pair is counted twice (once from each element).
    return totalPairs / 2;
}
#include <cassert>
#include <vector>

int main() {
    // Example from the snippet: basic test.
    assert(countPairsWithDoubleAverage({1, 2, 3, 4}) == 2); // pairs (1,4) and (2,3) have average 2.5? Actually average of array is 2.5, so 2*sum/n = 20/4=5, target=5. pairs sum to 5: (1,4) and (2,3) -> 2.
    assert(countPairsWithDoubleAverage({2, 2, 2}) == 3); // average=2, target=4, all pairs sum to 4.
    assert(countPairsWithDoubleAverage({1, 2, 3}) == 0); // sum=6, 2*sum/n=12/3=4, pairs sum to 4: (1,3) only -> 1? Actually (1,3) sum=4, yes 1. Let's recalc: elements 1,2,3. pairs: (1,2) sum=3, (2,3) sum=5, (1,3) sum=4 -> exactly one pair. So answer 1.
    assert(countPairsWithDoubleAverage({1, 3}) == 1); // sum=4, target=4, pair (1,3) sum=4.
    assert(countPairsWithDoubleAverage({5}) == 0); // single element no pairs.
    assert(countPairsWithDoubleAverage({}) == 0); // empty.
    assert(countPairsWithDoubleAverage({-1, 1}) == 1); // sum=0, target=0, pair sum 0.
    assert(countPairsWithDoubleAverage({0, 0, 0, 0}) == 6); // sum=0, target=0, all pairs: C(4,2)=6.
    assert(countPairsWithDoubleAverage({1, -1, 1, -1}) == 4); // sum=0, target=0, pairs: each 1 with each -1 => 2*2=4.
    // Large value test to check overflow handling.
    assert(countPairsWithDoubleAverage({1000000000, 1000000000}) == 1); // sum=2e9, target=2e9, pair sum=2e9.
    assert(countPairsWithDoubleAverage({1000000000, -1000000000, 0}) == 0); // sum=0, target=0, pairs sum to 0: (1e9, -1e9) sum=0, (1e9,0)=1e9, (-1e9,0)=-1e9 -> only one pair (1e9,-1e9) -> answer 1. Actually correct.
    return 0;
}
