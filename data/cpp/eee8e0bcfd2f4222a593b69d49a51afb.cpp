Write a C++ function `checkPairDifferences` that takes a vector of integers and returns a string `"YES"` if for every adjacent pair of elements, the absolute difference is exactly 5 or exactly 7. Otherwise, return `"NO"`. The function must handle vectors of any length (including size 0 and size 1, which should trivially return `"YES"`). The absolute difference must be computed using integer arithmetic, and negative input values are allowed.
#include <cassert>
#include <vector>
#include <string>

// Declaration from the solution (inline here for the test)
std::string checkPairDifferences(const std::vector<int>& arr);

int main() {
    // Empty vector: no pairs, so YES
    assert(checkPairDifferences({}) == "YES");
    // Single element: no pairs, so YES
    assert(checkPairDifferences({42}) == "YES");
    // All differences 5
    assert(checkPairDifferences({1, 6, 11, 16}) == "YES");
    // All differences 7
    assert(checkPairDifferences({0, 7, 14, 21}) == "YES");
    // Mixed 5 and 7
    assert(checkPairDifferences({10, 5, 12, 19}) == "YES");
    // Negative numbers
    assert(checkPairDifferences({-3, 2, -5, 2}) == "YES"); // diffs: 5,7,7
    // One invalid pair
    assert(checkPairDifferences({1, 2, 7}) == "NO"); // first diff 1 is invalid
    // Invalid at later position
    assert(checkPairDifferences({5, 0, 8, 1}) == "NO"); // last diff 7? actually 8-1=7, but 0-8=8 invalid
    // Large numbers
    assert(checkPairDifferences({1000000, 1000005, 1000012}) == "YES");
    // Duplicate values give difference 0 -> NO
    assert(checkPairDifferences({3, 3}) == "NO");

    return 0;
}
#include <vector>
#include <string>
#include <cstdlib>

// Returns "YES" if every adjacent pair has absolute difference 5 or 7, otherwise "NO".
std::string checkPairDifferences(const std::vector<int>& arr) {
    const std::size_t n = arr.size();
    for (std::size_t i = 0; i + 1 < n; ++i) {
        const int diff = std::abs(arr[i] - arr[i + 1]);
        if (diff != 5 && diff != 7) {
            return "NO";
        }
    }
    return "YES";
}
// The solution is a straightforward linear scan through the vector, checking each adjacent pair. For each index `i` from 0 to `n-2`, compute `abs(vec[i] - vec[i+1])` and verify it is either 5 or 7. If any pair fails, immediately return `"NO"`. If the loop completes without failure, return `"YES"`. Edge cases: an empty vector or a vector with one element has no adjacent pairs, so the rule is vacuously satisfied, and the function should return `"YES"`. The algorithm runs in O(n) time since it examines all `n-1` adjacent pairs exactly once, and uses O(1) auxiliary space because only a few scalar variables are needed. Integer overflow is not a concern because the absolute difference of two `int` values that fit in `int` range will also fit in `int` (though for safety, one could use `long long` to capture the difference, but it is unnecessary here given typical constraints).
