// Write a C++ function `std::vector<int> longestIncreasingSubsequence(const std::vector<int>& arr)` that takes a vector of integers and returns the longest strictly increasing subsequence (LIS) as a vector of the actual values in order. If multiple LIS exist, return any one of them. The input may contain negative numbers, repeated values, and the vector may be empty (in which case return an empty vector). The sequence must be strictly increasing, so equal adjacent values are not allowed. The output should be the elements themselves, not their indices or lengths. The function must handle up to \(10^6\) elements efficiently.

// The problem is the classic longest increasing subsequence with reconstruction of the actual elements, requiring an \(O(N \log N)\) solution because of potential large input size. We use a standard patience-sorting binary search approach: maintain a `tails` array where `tails[k]` is the smallest possible tail value for an increasing subsequence of length `k+1`, and also maintain an `index_history` array that stores, for each position in the original array, the length of the LIS ending at that position (i.e., the index in `tails` where we place the current element). Then, to reconstruct the subsequence, we iterate from the end of the original array backwards, picking elements whose stored length matches the current target length (starting from the LIS length minus 1 down to 0). Because we scan from the end, we correctly pick the rightmost occurrences, ensuring a valid subsequence. Empty input returns an empty vector. Handle duplicates by using `lower_bound` (which finds the first position with a value not less than the element), so equal values will be treated as a new larger element, preventing strict increase violation. Time complexity is \(O(N \log N)\) due to binary search, and space complexity is \(O(N)\) for all auxiliary arrays.

#include <vector>
#include <algorithm>

// Returns the longest strictly increasing subsequence of arr as a vector of values.
std::vector<int> longestIncreasingSubsequence(const std::vector<int>& arr) {
    if (arr.empty()) return {};

    const int n = static_cast<int>(arr.size());
    std::vector<int> tails;             // tails[k] = smallest tail for LIS of length k+1
    std::vector<int> pos_of_len(n, 0);  // pos_of_len[i] = length index (0-based) of LIS ending at arr[i]

    for (int i = 0; i < n; ++i) {
        auto it = std::lower_bound(tails.begin(), tails.end(), arr[i]);
        int pos = static_cast<int>(it - tails.begin());
        if (it == tails.end()) {
            tails.push_back(arr[i]);
        } else {
            *it = arr[i];
        }
        pos_of_len[i] = pos;
    }

    int lis_len = static_cast<int>(tails.size());
    std::vector<int> result(lis_len);
    int target = lis_len - 1;
    for (int i = n - 1; i >= 0 && target >= 0; --i) {
        if (pos_of_len[i] == target) {
            result[target] = arr[i];
            --target;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    // Basic cases
    assert(longestIncreasingSubsequence({3, 1, 2}) == std::vector<int>({1, 2}));
    assert(longestIncreasingSubsequence({1, 2, 3}) == std::vector<int>({1, 2, 3}));
    assert(longestIncreasingSubsequence({3, 2, 1}) == std::vector<int>({1})); // any single element works

    // Empty input
    assert(longestIncreasingSubsequence({}).empty());

    // Duplicates (must be strictly increasing)
    assert(longestIncreasingSubsequence({2, 2, 2}) == std::vector<int>({2})); 
    assert(longestIncreasingSubsequence({1, 2, 2, 3}) == std::vector<int>({1, 2, 3}));

    // Negative and mixed numbers
    assert(longestIncreasingSubsequence({-5, -1, -10, 0}) == std::vector<int>({-5, -1, 0}));

    // Larger test with known length and value pattern
    std::vector<int> input = {10, 9, 2, 5, 3, 7, 101, 18};
    std::vector<int> res = longestIncreasingSubsequence(input);
    assert(res.size() == 4);
    // The result must be strictly increasing and use elements from input in order
    for (size_t i = 1; i < res.size(); ++i) assert(res[i-1] < res[i]);

    // Stress-ish test with 100000 elements (values 1..100000 shuffled)
    std::vector<int> large(100000);
    for (int i = 0; i < 100000; ++i) large[i] = i + 1; // already increasing
    std::vector<int> large_res = longestIncreasingSubsequence(large);
    assert(large_res.size() == 100000);
    for (size_t i = 0; i < large_res.size(); ++i) assert(large_res[i] == static_cast<int>(i) + 1);

    // Reverse large test (should produce length 1)
    std::vector<int> rev(1000);
    for (int i = 0; i < 1000; ++i) rev[i] = 1000 - i;
    std::vector<int> rev_res = longestIncreasingSubsequence(rev);
    assert(rev_res.size() == 1);

    return 0;
}
