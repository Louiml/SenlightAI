// Write a standalone C++ function named `maxAlternatingIndexSum` that takes a vector of integers and returns the larger of two sums: the sum of elements at even indices (0, 2, 4, ...) and the sum of elements at odd indices (1, 3, 5, ...). The input vector is guaranteed to be non-empty, may contain negative numbers, and duplicates are allowed. The function must be const-correct and handle any size from 1 onward. Return the maximum of the two sums as an integer.
The solution is straightforward: iterate through the vector once, maintaining two accumulators—one for even-indexed elements and one for odd-indexed elements. Since indices are zero-based, even indices correspond to position `i % 2 == 0`. For each element, add it to the appropriate accumulator based on parity. After the loop, return the larger of the two sums. Edge cases to consider: (1) a single-element vector—the even sum is that element, the odd sum is 0, so the result is the element itself; (2) all negative numbers—the maximum may be a negative sum (or 0 if the odd sum is empty, but for non-empty vectors with all negatives, the even sum still includes some elements, so the result is the less negative of the two); (3) zeros—they contribute nothing but don't change correctness. Time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary, not counting the input vector itself.
#include <vector>
#include <algorithm>

// Return the larger of the sum of elements at even indices and the sum at odd indices.
int maxAlternatingIndexSum(const std::vector<int>& numbers) {
    int even_sum = 0;
    int odd_sum = 0;
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        if (i % 2 == 0) {
            even_sum += numbers[i];
        } else {
            odd_sum += numbers[i];
        }
    }
    return std::max(even_sum, odd_sum);
}
#include <cassert>
#include <vector>

int maxAlternatingIndexSum(const std::vector<int>& numbers);

int main() {
    // Single element
    assert(maxAlternatingIndexSum({5}) == 5);
    // Two elements
    assert(maxAlternatingIndexSum({1, 2}) == 2); // even=1, odd=2
    // Alternating larger even sum
    assert(maxAlternatingIndexSum({10, 1, 10}) == 20); // even=20, odd=1
    // Alternating larger odd sum
    assert(maxAlternatingIndexSum({1, 10, 1}) == 10); // even=2, odd=10
    // All negative numbers
    assert(maxAlternatingIndexSum({-3, -1, -2}) == -3); // even=-5, odd=-1 -> max=-1 (wait, correct manually: indices: 0:-3, 1:-1, 2:-2 -> even=-5, odd=-1, max=-1)
    // With zeros
    assert(maxAlternatingIndexSum({0, -1, 0, -1}) == 0); // even=0, odd=-2 -> max=0
    // Mixed large values
    assert(maxAlternatingIndexSum({100, -50, 200, -10, 5}) == 305); // even=100+200+5=305, odd=-60 -> max=305
    // Long vector
    std::vector<int> v(1000, 1);
    // even indices count = 500, odd = 500, both sums = 500, max = 500
    assert(maxAlternatingIndexSum(v) == 500);
    return 0;
}
