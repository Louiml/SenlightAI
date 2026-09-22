// Write a C++ function named `missingSmallestPositive` that takes a `std::vector<int>` containing \(n\) integers (with \(0 \le n \le 3000\)) and returns the smallest positive integer (starting from 1) that is not present in the vector. The input vector may contain duplicates, negative numbers, zeros, and values greater than 3000. The function should handle an empty vector by returning 1. The solution must not modify the input vector, and must run efficiently for the given constraints.

The problem is a classic "find the smallest missing positive integer" task, but with a fixed maximum bound. Since the answer can be at most 3001 (because the vector can contain at most 3000 distinct positive numbers from 1 to 3000), we can use a boolean array of size 3002. First, we iterate over the input vector and for each value that lies between 1 and 3000, we mark `present[value] = true`. Ignore any values outside [1, 3000] because they cannot affect the answer. Then we iterate `i` from 1 upward and return the first `i` for which `present[i]` is false. If all from 1 to 3000 are marked, the answer is 3001. Edge cases: empty vector returns 1; duplicates cause no issue; non-positive numbers are ignored. Time complexity is \(O(n + M)\) where \(M = 3000\), which simplifies to \(O(n)\) since `M` is constant. Space complexity is \(O(3000) = O(1)\) auxiliary as the marked array is fixed-size. The use of a boolean array of fixed size makes the solution simple and fast, while avoiding sorting or hash set overhead.

#include <vector>
#include <cstddef>

// Returns the smallest positive integer (>=1) not present in the vector.
// The input vector is not modified. The maximum considered value is 3000,
// so the answer is always in [1, 3001].
int missingSmallestPositive(const std::vector<int>& values) {
    constexpr int kMaxLimit = 3000;
    // present[i] is true if integer i was seen in values (for i in 1..3000)
    bool present[kMaxLimit + 2] = {}; // zero-initialized

    for (int v : values) {
        if (v >= 1 && v <= kMaxLimit) {
            present[v] = true;
        }
    }

    for (int i = 1; i <= kMaxLimit + 1; ++i) {
        if (!present[i]) {
            return i;
        }
    }

    // This point is unreachable due to the loop bound, but added for completeness.
    return kMaxLimit + 1;
}

#include <cassert>
#include <vector>

// Declare the function under test (from solution or include the header)
int missingSmallestPositive(const std::vector<int>& values);

int main() {
    // Basic case with a missing small number
    assert(missingSmallestPositive({1, 2, 3, 5}) == 4);
    // Case with all numbers from 1 to 5 present
    assert(missingSmallestPositive({3, 1, 4, 2, 5}) == 6);
    // Case with duplicates and negative numbers
    assert(missingSmallestPositive({-1, 0, 2, 2, 1, -5}) == 3);
    // Empty vector
    assert(missingSmallestPositive({}) == 1);
    // Only large out-of-range values
    assert(missingSmallestPositive({10000, 5000}) == 1);
    // Only numbers above 3000, but also include 1..3000? Here only check single missing in middle
    assert(missingSmallestPositive({1, 2, 4, 3000}) == 3);
    // All numbers from 1 to 3000 present
    std::vector<int> all;
    for (int i = 1; i <= 3000; ++i) all.push_back(i);
    assert(missingSmallestPositive(all) == 3001);
    // Duplicates of all numbers but missing 1
    std::vector<int> withDuplicates = {2, 2, 3, 3, 4, 4};
    assert(missingSmallestPositive(withDuplicates) == 1);
    // Single element 1
    assert(missingSmallestPositive({1}) == 2);
    // Single element 0
    assert(missingSmallestPositive({0}) == 1);
    // Large n with a missing medium number
    std::vector<int> many;
    for (int i = 1; i <= 500; ++i) many.push_back(i);  // 1..500
    for (int i = 502; i <= 2000; ++i) many.push_back(i); // skip 501
    assert(missingSmallestPositive(many) == 501);
}
