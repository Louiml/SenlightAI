// Write a standalone C++ function named `maximumProductAfterDecrement` that takes a non-empty vector of integers (`std::vector<int>`) and returns the maximum possible product of `(a-1) * (b-1)` where `a` and `b` are two distinct elements from the vector. The function must not modify the input vector, must handle negative numbers, duplicates, and vectors of size 2 or larger, and must return the result as an `int`. The algorithm should not use additional sorting of the input itself; instead, you may copy or use constant space tracking. The function should be `const`‑correct and reside in a header‑eligible format.

The key observation is that for any two distinct indices `i` and `j`, the product `(nums[i]-1)*(nums[j]-1)` is maximized when `nums[i]` and `nums[j]` are the two largest values in the array. This is because the function `f(x) = x-1` is strictly increasing, so larger original values yield larger `(x-1)`. Therefore, the optimal pair is exactly the two largest numbers in the vector. Edge cases: (1) Duplicates are allowed — the two largest may be equal, and that is fine; distinct indices are guaranteed since vector size ≥ 2. (2) Negative numbers are fine because we only care about the maximum values — the algebra still holds because sorting ascending and taking the last two elements works even with negatives. (3) The vector size is at least 2, so we never access out‑of‑bounds. Implementation approach: traverse the vector once to find the largest and second largest values without sorting, avoiding mutation of the input. Initialize largest and second largest appropriately (e.g., `INT_MIN` and `INT_MIN`), then update as we iterate. However, careful handling is needed to ensure the two largest are distinct indices; using a single pass with two variables works if we update correctly: for each element, if it is greater than the current largest, shift previous largest to second largest and set new largest; else if it is greater than the second largest, update second largest. This works even with duplicates because equal values will be captured when the new value is not strictly greater than largest but equal — we need to allow `>=` for second largest update with correct condition to handle equal values at distinct indices. Better approach: just copy the vector and sort the copy, then pick last two. That is simpler and still O(n log n) time, O(n) auxiliary space for the copy. Since we must not modify input, copying is acceptable. Provide const-correctness by taking `const std::vector<int>&`. Time complexity: O(n log n) due to sorting a copy; space complexity: O(n) for the copy. A linear‑time alternative exists (single pass with two variables) but requires careful duplicate handling; the sorting solution is more robust and simpler for a teaching task. We will use the copy‑sort approach.

#include <vector>
#include <algorithm>

// Returns the maximum product of (a-1)*(b-1) where a and b are two distinct
// elements from the input vector. The input vector is not modified.
int maximumProductAfterDecrement(const std::vector<int>& nums) {
    // Copy the input to allow sorting without mutating the caller's data.
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    // The two largest elements are at the end of the sorted vector.
    int n = sorted.size();
    int largest = sorted[n - 1];
    int second_largest = sorted[n - 2];

    return (largest - 1) * (second_largest - 1);
}

#include <cassert>
#include <vector>

// This function is assumed to be declared above (as in the solution).
int maximumProductAfterDecrement(const std::vector<int>& nums);

int main() {
    // Basic positive case
    assert(maximumProductAfterDecrement({3, 4, 5, 2}) == 12); // (5-1)*(4-1)=4*3=12
    // Two elements
    assert(maximumProductAfterDecrement({1, 10}) == 0); // (10-1)*(1-1)=9*0=0
    // Duplicates
    assert(maximumProductAfterDecrement({5, 5, 5}) == 16); // (5-1)*(5-1)=4*4=16
    // Negative numbers
    assert(maximumProductAfterDecrement({-3, -2, -1, 0}) == 2); // (-1-1)*(0-1)=(-2)*(-1)=2
    // All same numbers
    assert(maximumProductAfterDecrement({7, 7}) == 36); // (7-1)*(7-1)=6*6=36
    // Mixed with zeros
    assert(maximumProductAfterDecrement({0, 1, 2}) == 0); // (2-1)*(1-1)=1*0=0
    // Large numbers (still fit in int)
    assert(maximumProductAfterDecrement({100, 99, 98}) == 9702); // (100-1)*(99-1)=99*98=9702
    // Unsorted input
    assert(maximumProductAfterDecrement({9, 1, 8, 2}) == 56); // (9-1)*(8-1)=8*7=56
    // Negative and positive mix
    assert(maximumProductAfterDecrement({-5, 3, 2}) == 2); // (3-1)*(2-1)=2*1=2
    // Size exactly 2 with negative values
    assert(maximumProductAfterDecrement({-10, -4}) == 55); // (-4-1)*(-10-1)=(-5)*(-11)=55
}
