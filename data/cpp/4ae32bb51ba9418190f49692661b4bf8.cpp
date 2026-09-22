Write a C++ function `long long maximumPairwiseProduct(const std::vector<long long>& numbers)` that, given a non-empty vector of 64-bit signed integers, returns the maximum possible product of any two distinct elements in the vector. The vector may contain negative numbers, zeros, and duplicates. If two indices are distinct but hold the same value, they are considered valid for pairing. The solution must handle cases where the maximum product comes from two negative numbers (their product is positive and could be larger than the product of two positive numbers), and must also correctly process vectors with fewer than two elements (in such a case, return the product of the only element with itself, which is `numbers[0] * numbers[0]`). The function should be efficient for vectors of size up to \(10^5\).

#include <cassert>
#include <vector>

// Solution function declaration (already defined above)
long long maximumPairwiseProduct(const std::vector<long long>& numbers);

int main() {
    // Basic positive numbers
    assert(maximumPairwiseProduct({1, 2, 3, 4}) == 12); // 3*4
    // Two negative numbers produce a large positive
    assert(maximumPairwiseProduct({-10, -3, -1, 2}) == 30); // (-10)*(-3)
    // Mixed sign with zeros
    assert(maximumPairwiseProduct({-5, 0, 3, 4}) == 12); // 3*4
    // Single element (special case)
    assert(maximumPairwiseProduct({7}) == 49);
    // All equal
    assert(maximumPairwiseProduct({-2, -2, -2}) == 4); // (-2)*(-2)
    // Large numbers within 64-bit
    assert(maximumPairwiseProduct({1000000000LL, 1000000000LL}) == 1000000000000000000LL);
    // Negative and positive where product of two negatives is larger
    assert(maximumPairwiseProduct({-100, -1, 5, 2}) == 100); // (-100)*(-1)
    // Edge case with only two elements
    assert(maximumPairwiseProduct({-3, 2}) == -6); // only possible product
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Return the maximum product of two distinct elements from the input vector.
long long maximumPairwiseProduct(const std::vector<long long>& numbers) {
    const std::size_t n = numbers.size();
    if (n == 1) {
        return numbers[0] * numbers[0];
    }

    // Initialize with the first two elements.
    long long max1 = std::max(numbers[0], numbers[1]);
    long long max2 = std::min(numbers[0], numbers[1]);
    long long min1 = std::min(numbers[0], numbers[1]);
    long long min2 = std::max(numbers[0], numbers[1]);

    // Check the rest of the array.
    for (std::size_t i = 2; i < n; ++i) {
        const long long value = numbers[i];
        // For maximums
        if (value > max1) {
            max2 = max1;
            max1 = value;
        } else if (value > max2) {
            max2 = value;
        }
        // For minimums
        if (value < min1) {
            min2 = min1;
            min1 = value;
        } else if (value < min2) {
            min2 = value;
        }
    }

    const long long productMax = max1 * max2;
    const long long productMin = min1 * min2;
    return std::max(productMax, productMin);
}

// The maximum pairwise product of a list of integers is either the product of the two largest numbers or the product of the two smallest numbers (which, if both are negative, yields a large positive product). Therefore, the algorithm is to scan the vector once while maintaining the two largest values and the two smallest values. Initialize all trackers using the first two elements appropriately (or handle the single-element case separately). For each subsequent element, update the top two maxima and bottom two minima using comparisons. After the scan, compute the product of the two largest and the product of the two smallest, and return the larger of the two. Edge cases include: a vector with one element (return that element squared), vectors with zeros and negatives, and cases where the two largest include a negative and a positive (then the max product is likely from the two smallest or the two largest depending on magnitude). The time complexity is \(O(n)\) with \(O(1)\) extra space, where \(n\) is the size of the input vector.
