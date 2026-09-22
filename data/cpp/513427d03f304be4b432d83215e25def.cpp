Write a C++ function that takes a non-empty vector of integers (which may contain duplicates, zeros, and negative numbers) and returns the maximum value obtainable by multiplying any two distinct elements at different positions in the vector. If the vector has fewer than two elements, the function should return 0 (since no valid pair exists). The function must not modify the input vector and must be efficient enough for vectors of size up to 10^5.
#include <cassert>
#include <vector>

// Function declaration (as defined in the solution)
long long MaxPairwiseProduct(const std::vector<int>& numbers);

int main() {
    // Basic positive numbers
    assert(MaxPairwiseProduct({1, 2, 3, 4}) == 12);
    // All negative numbers: product of two most negative gives largest positive
    assert(MaxPairwiseProduct({-1, -2, -3, -4}) == 12);
    // Mixed signs: product of largest positive and second largest positive (since negative*positive is negative)
    assert(MaxPairwiseProduct({-10, 2, 3, 4}) == 12);
    // With zeros
    assert(MaxPairwiseProduct({0, 0, 5}) == 0);
    assert(MaxPairwiseProduct({0, -1, 5}) == 0); // max product is 0 vs -5
    // Duplicate values
    assert(MaxPairwiseProduct({5, 5, 1}) == 25);
    assert(MaxPairwiseProduct({-5, -5, 1}) == 25);
    // Exactly two elements
    assert(MaxPairwiseProduct({7, -3}) == -21);
    assert(MaxPairwiseProduct({-2, -3}) == 6);
    // Less than two elements
    assert(MaxPairwiseProduct({42}) == 0);
    assert(MaxPairwiseProduct({}) == 0);
    // Large values to check overflow handling (using long long)
    assert(MaxPairwiseProduct({100000, 100000}) == 10000000000LL);
    return 0;
}
#include <vector>
#include <algorithm>
#include <limits>

// Return the maximum product of any two distinct elements in the vector.
// If the vector has fewer than two elements, return 0.
long long MaxPairwiseProduct(const std::vector<int>& numbers) {
    int n = static_cast<int>(numbers.size());
    if (n < 2) {
        return 0;
    }

    // Initialize extremes with first element
    int largest = numbers[0];
    int second_largest = std::numeric_limits<int>::min();
    int smallest = numbers[0];
    int second_smallest = std::numeric_limits<int>::max();

    for (int i = 1; i < n; ++i) {
        int value = numbers[i];

        // Update largest and second largest
        if (value > largest) {
            second_largest = largest;
            largest = value;
        } else if (value > second_largest) {
            second_largest = value;
        }

        // Update smallest and second smallest
        if (value < smallest) {
            second_smallest = smallest;
            smallest = value;
        } else if (value < second_smallest) {
            second_smallest = value;
        }
    }

    // If n == 2, second_largest and second_smallest are set correctly (they are the same element? Actually for n=2, second_largest will be the other element if the first is largest, but if first is not largest, then second_largest gets set in the loop. For n=2, the loop runs once, so it works.)
    // Compute the two candidate products
    long long candidate1 = static_cast<long long>(largest) * second_largest;
    long long candidate2 = static_cast<long long>(smallest) * second_smallest;

    // Return the larger (candidate1 might be negative if largest*second_largest is negative, but candidate2 might be positive if both smallest are negative)
    return std::max(candidate1, candidate2);
}
// The naive double loop (as in the provided snippet) is O(n^2) and will be too slow for large inputs. Instead, we find the maximum product by considering only the two largest numbers and the two smallest numbers, because the maximum product can come from either the product of the two largest numbers (if they are both positive) or the product of the two smallest numbers (if they are both negative, giving a positive product, or one negative and one positive but then the product is negative, so we only care about the larger of those two scenarios). However, we must also handle the case where the vector has exactly two elements: then the product is simply the product of those two. For size < 2, return 0.  
// A robust method: iterate once to find the largest, second largest, smallest, and second smallest values. Then compute max(largest * second_largest, smallest * second_smallest). This works because any two distinct elements must be from these four extremes. Edge cases: all negatives (the two smallest negatives yield the largest positive product), mixed signs (the product of two largest positives might be negative if one is negative? Actually if all numbers are negative, largest and second largest are the least negative, their product is positive but smaller than the product of the two most negative), zeros (product with zero is zero, and if there is at least one positive and one negative, the max product might be zero or positive). Also handle duplicates: if the largest number appears multiple times, second largest can be the same value (as long as there are at least two occurrences). Time complexity: O(n) single pass. Space complexity: O(1) extra.
