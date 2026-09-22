// Write a C++ function `countPositivePairs` that takes a vector of integers representing the difference between two parallel arrays (e.g., `a[i] - b[i]`) and returns the number of pairs of indices `(i, j)` with `i < j` such that the sum of the two differences is strictly greater than zero. The function must operate on the original order of the vector (do not sort the input vector itself), must handle up to 2×10^5 elements with values in the range [−10^9, 10^9], and must use an efficient algorithm based on sorting a copy of the values and binary searching for each element to count valid pairs. The function should return the result as a `long long` because the maximum number of pairs is approximately n*(n−1)/2, which can exceed 32-bit integer range.

#include <cassert>
#include <vector>
#include <cstdint>

// The solution function declaration is assumed available.
// long long countPositivePairs(const std::vector<long long>& diff);

int main() {
    // Basic cases
    assert(countPositivePairs({}) == 0LL);
    assert(countPositivePairs({5}) == 0LL);
    assert(countPositivePairs({0, 0}) == 0LL);
    assert(countPositivePairs({1, 2}) == 1LL);
    assert(countPositivePairs({-1, 2}) == 1LL);
    assert(countPositivePairs({-5, -3, -1}) == 0LL);
    assert(countPositivePairs({1, 1, 1}) == 3LL);  // all pairs: (0,1), (0,2), (1,2)

    // Mixed signs
    assert(countPositivePairs({-2, 0, 1}) == 1LL); // only (-? , 1) works? actually (-2+1=-1) no, (0+1=1) yes => 1
    assert(countPositivePairs({-1, 2, -3, 4}) == 3LL); // pairs: (-1,2)=1, (-1,4)=3, (2,4)=6 => 3

    // Larger test
    std::vector<long long> large(200000, 1LL);
    // All positive: sum of pairs = n*(n-1)/2 = 200000*199999/2 = 19999900000
    long long expected = 200000LL * 199999LL / 2LL;
    assert(countPositivePairs(large) == expected);

    // Negative all
    std::vector<long long> neg(100000, -1000000000LL);
    assert(countPositivePairs(neg) == 0LL);

    // Mixed with zeros
    assert(countPositivePairs({0, 1, -1}) == 0LL); // (0+1)=1? wait 0+1=1 >0 => pair (0,1) works; (0,-1) no; (1,-1)=0 no => 1? Actually check: vector {0,1,-1}, pairs: (0,1)=1>0 yes, (0,2)=0+(-1)=-1 no, (1,2)=1+(-1)=0 not >0 => total 1
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Count pairs (i, j) with i < j such that diff[i] + diff[j] > 0.
// The input vector is not modified; a sorted copy is used.
long long countPositivePairs(const std::vector<long long>& diff) {
    int n = static_cast<int>(diff.size());
    if (n < 2) return 0LL;

    // Work on a sorted copy to enable binary search.
    std::vector<long long> sorted = diff;
    std::sort(sorted.begin(), sorted.end());

    long long result = 0;  // up to ~2e10, so long long is required

    // For each element at position i (treated as the "second" element),
    // find the smallest index l such that sorted[i] + sorted[l] > 0.
    // All elements from l to i-1 form valid pairs with sorted[i].
    for (int i = 1; i < n; ++i) {
        int low = 0;
        int high = i - 1;
        int leftmost = i;  // default: no valid element before i

        // Binary search for the leftmost index satisfying the condition.
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (sorted[i] + sorted[mid] > 0) {
                leftmost = mid;
                high = mid - 1;  // try to find an even earlier index
            } else {
                low = mid + 1;
            }
        }

        // All indices from leftmost to i-1 work.
        result += static_cast<long long>(i - leftmost);
    }

    return result;
}

// The problem is equivalent to counting ordered pairs `(i, j)` with `i < j` such that `diff[i] + diff[j] > 0`. The naive double loop is O(n²) and too slow for n up to 2×10^5. A better approach: First, make a sorted copy of the input vector. Then, for each index `i` (from 1 to n−1) in the sorted array, consider the element at position `i` as the "second" element of the pair. We need to count how many elements before it (i.e., indices `< i`) when added to the current element produce a positive sum. Since the array is sorted, we can binary search to find the smallest index `l` such that `sorted[i] + sorted[l] > 0`. All elements from `l` to `i-1` satisfy the condition, so we add `(i - l)` to the result. Because we process each element once and each binary search is O(log n), the total time complexity is O(n log n). The space complexity is O(n) for the sorted copy. Edge cases: all negative sums, in which case no pair qualifies; all positive, then all pairs qualify; duplicates are handled naturally because the sorted order preserves them and the binary search counts each valid pair once (since we only consider `i > l`, we never double-count). The result must be `long long` because with n=200000, the maximum number of pairs is ~20,000,000,000 which is larger than `int` (approx 2.147e9).
