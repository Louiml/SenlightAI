// Write a C++ function `int minCoinsToExceedHalf(const std::vector<int>& values)` that takes a vector of positive integers (representing coin denominations or values) and returns the minimum number of coins needed so that the sum of the selected coins is strictly greater than the sum of the remaining coins. You may choose any subset of coins of any size, but the goal is to find the smallest subset size such that the sum of the largest possible coins of that size exceeds half the total sum. In other words, sort the values in descending order, then find the smallest prefix length `k` (starting from 1) where the sum of the first `k` largest values is greater than the sum of the rest. The input vector may contain duplicates. Return the required `k`. If the vector is empty, return 0. You can assume the total sum of all values fits in a 32-bit integer.
// The approach is to first compute the total sum of all values. Then sort the vector in descending order (largest first) because to minimize the number of coins while maximizing their sum, we should always pick the largest available coins. After sorting, for each possible prefix length `k` starting from 1, compute the sum of the first `k` elements. Check if this prefix sum is strictly greater than the sum of all other elements, which is `total_sum - prefix_sum`. Since we check prefix lengths in increasing order, the first time this condition holds is the minimal number of coins. Edge cases: if the vector is empty, return 0; if the first largest element alone is already greater than the sum of the rest, the answer is 1 (this happens when one coin is larger than all others combined). Duplicates are handled naturally by sorting. The algorithm is correct because any optimal subset of `k` coins cannot have a sum greater than the sum of the `k` largest elements (by rearrangement inequality), so checking the prefix of the sorted array is sufficient. Time complexity: O(n log n) due to sorting, plus O(n) for the prefix loop, so overall O(n log n). Space complexity: O(1) if we sort in place (but the function takes a const reference, so we copy the vector or sort a copy, making it O(n) auxiliary space; alternatively, we can accept a copy or sort the vector in place by taking a non-const parameter—but the task requires const correctness, so we'll copy). We'll use a copy of the input to sort.
#include <vector>
#include <algorithm>
#include <numeric>

// Returns the minimum number of largest coins needed so that their sum
// strictly exceeds the sum of the remaining coins.
int minCoinsToExceedHalf(const std::vector<int>& values) {
    if (values.empty()) return 0;

    // Work on a copy to preserve the input and allow sorting
    std::vector<int> sorted_values = values;
    std::sort(sorted_values.begin(), sorted_values.end(), std::greater<int>());

    int total_sum = std::accumulate(sorted_values.begin(), sorted_values.end(), 0);

    int prefix_sum = 0;
    for (int k = 1; k <= static_cast<int>(sorted_values.size()); ++k) {
        prefix_sum += sorted_values[k - 1];
        if (prefix_sum > total_sum - prefix_sum) {
            return k;
        }
    }
    // This point should never be reached because the sum of all coins
    // always exceeds half (it equals the total), but return size as fallback
    return static_cast<int>(sorted_values.size());
}
#include <cassert>
#include <vector>

// Declaration of the function under test (include the solution code above)
int minCoinsToExceedHalf(const std::vector<int>& values);

int main() {
    // Basic case: need 2 largest coins (3+2 > 1+1)
    assert(minCoinsToExceedHalf({1, 2, 3, 1}) == 2);

    // One coin dominates all others
    assert(minCoinsToExceedHalf({10, 1, 1, 1}) == 1);

    // All equal: need more than half the count
    assert(minCoinsToExceedHalf({5, 5, 5, 5}) == 3); // 5+5+5=15 > 5

    // Single coin
    assert(minCoinsToExceedHalf({7}) == 1);

    // Two coins equal: need one because 5 > 5? No, must be strictly greater, so need 2
    assert(minCoinsToExceedHalf({5, 5}) == 2);

    // Empty vector
    assert(minCoinsToExceedHalf({}) == 0);

    // Larger case with duplicates: [8,8,4,4,4] total=28, need sum>14
    // 8+8=16>12, so k=2
    assert(minCoinsToExceedHalf({8, 8, 4, 4, 4}) == 2);

    // Case where no prefix works until full length (but sum of all > half always)
    // Example: [1,1] total=2, need >1, 1+1>0 works, but check [1] gives 1>1 false, so k=2
    assert(minCoinsToExceedHalf({1, 1}) == 2);

    return 0;
}
