/*
Write a C++ function that takes a vector of integers (with at least two elements) and returns the maximum possible product of any two distinct elements in the vector. The function must correctly handle negative numbers, zeros, and duplicate values. The input vector may be unsorted and of any size (but at least 2). The function should be efficient enough to handle vectors with up to 10^6 elements, so a quadratic O(n²) brute-force approach is not acceptable. The function signature should be `long long MaxPairwiseProduct(const std::vector<int>& numbers)`. The return type is `long long` because the product of two large integers may exceed the range of `int`. The function should be implemented as a free function with no `main`.
*/
#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the maximum product of any two distinct elements from the input vector.
// Assumes the input vector has at least two elements.
// Handles negative numbers, zeros, and duplicates correctly.
// Time: O(n), Space: O(1) extra.
long long MaxPairwiseProduct(const std::vector<int>& numbers) {
    // Initialize the four extremes with the first two elements.
    long long largest = std::max(numbers[0], numbers[1]);
    long long secondLargest = std::min(numbers[0], numbers[1]);
    long long smallest = std::min(numbers[0], numbers[1]);
    long long secondSmallest = std::max(numbers[0], numbers[1]);

    // Scan the rest of the vector.
    for (std::size_t i = 2; i < numbers.size(); ++i) {
        const long long value = numbers[i];

        // Update the two largest values.
        if (value > largest) {
            secondLargest = largest;
            largest = value;
        } else if (value > secondLargest) {
            secondLargest = value;
        }

        // Update the two smallest values.
        if (value < smallest) {
            secondSmallest = smallest;
            smallest = value;
        } else if (value < secondSmallest) {
            secondSmallest = value;
        }
    }

    // The maximum product is either from the two largest or the two smallest.
    const long long product1 = largest * secondLargest;
    const long long product2 = smallest * secondSmallest;
    return std::max(product1, product2);
}
#include <cassert>
#include <vector>

// Declare the function (assume it's defined elsewhere).
long long MaxPairwiseProduct(const std::vector<int>& numbers);

int main() {
    // Basic positive case.
    assert(MaxPairwiseProduct({1, 2, 3, 4}) == 12);
    // Mixed negatives and positives.
    assert(MaxPairwiseProduct({-10, -3, 1, 2}) == 30);
    // All negatives.
    assert(MaxPairwiseProduct({-5, -2, -1}) == 10);
    // Duplicate values.
    assert(MaxPairwiseProduct({5, 5, 3}) == 25);
    // With zero.
    assert(MaxPairwiseProduct({0, 5, 7}) == 35);
    // Large product exceeding int range.
    assert(MaxPairwiseProduct({100000, 100000}) == 10000000000LL);
    // Two elements only.
    assert(MaxPairwiseProduct({-4, -9}) == 36);
    // Negatives and positive with zero.
    assert(MaxPairwiseProduct({-2, -1, 0, 3}) == 3);
    // Unsorted.
    assert(MaxPairwiseProduct({8, 1, -6, 4, -10}) == 60);
}
// The goal is to find the maximum product of any two distinct elements. The brute-force solution checks all pairs in O(n²), which is too slow for large inputs. The optimal approach considers that the maximum product comes from either (1) the two largest positive numbers, or (2) the two smallest negative numbers (since a negative times a negative gives a positive product, and the most negative numbers have the largest absolute values). Therefore, we can find the largest, second largest, smallest, and second smallest elements in a single pass. Then compute `product1 = largest * secondLargest` and `product2 = smallest * secondSmallest`, and return the maximum of these two products. This works because any other pair (e.g., a large positive with a small negative) will be smaller than one of these two candidates. Important edge cases include: vectors with mixed negative and positive numbers (e.g., `{-10, -3, 1, 2}` → the best is `-10 * -3 = 30`), vectors with zeros (e.g., `{0, 1}` → product 0), vectors with all negative numbers (e.g., `{-5, -2, -1}` → the two smallest are -5 and -2, product 10; the largest two are -2 and -1, product 2), and vectors with duplicate values (e.g., `{5, 5, 3}` → product 25). The algorithm requires a single pass through the vector to find the four extremes, so time complexity is O(n) and space complexity is O(1) extra (excluding input storage). If the vector has fewer than two elements, the function's behavior is undefined, but we can assume the input is valid per the task specification.
