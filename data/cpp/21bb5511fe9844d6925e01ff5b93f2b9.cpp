/*
Write a C++ function `long minimumMultiplyByCount(const std::vector<long>& numbers)` that takes a non-empty vector of integers and returns the result of multiplying the smallest element by the number of elements minus one. The function should work correctly for both positive and negative values, handle duplicate minimum values, and assume the input vector is non-empty. The solution must not modify the input vector. The result must be computed using `long` to avoid overflow for large inputs.
*/

#include <vector>
#include <algorithm> // for std::min (optional, used for clarity)

// Returns the smallest element multiplied by (count - 1).
long minimumMultiplyByCount(const std::vector<long>& numbers) {
    // Precondition: numbers is non-empty.
    long minimum = numbers[0];
    for (std::size_t i = 1; i < numbers.size(); ++i) {
        minimum = std::min(minimum, numbers[i]);
    }
    return minimum * static_cast<long>(numbers.size() - 1);
}

#include <cassert>
#include <vector>

// Function declaration (the actual implementation is provided in the Solution section).
long minimumMultiplyByCount(const std::vector<long>& numbers);

int main() {
    assert(minimumMultiplyByCount({5, 3, 9, 1}) == 3L);      // min=1, size=4 -> 1*3
    assert(minimumMultiplyByCount({-4, -7, -2}) == -14L);    // min=-7, size=3 -> -7*2
    assert(minimumMultiplyByCount({10}) == 0L);              // min=10, size=1 -> 10*0
    assert(minimumMultiplyByCount({7, 7, 7}) == 14L);        // min=7, size=3 -> 7*2
    assert(minimumMultiplyByCount({0, -1, 2, 3}) == -3L);    // min=-1, size=4 -> -1*3
    assert(minimumMultiplyByCount({1000000000, 2000000000}) == 1000000000L); // min=1e9, size=2 -> 1e9*1
    assert(minimumMultiplyByCount({-5, -5, -5, -5}) == -15L); // min=-5, size=4 -> -5*3
    assert(minimumMultiplyByCount({2, 1, 1, 1}) == 3L);      // min=1, size=4 -> 1*3
    assert(minimumMultiplyByCount({-3}) == 0L);              // min=-3, size=1 -> -3*0
    assert(minimumMultiplyByCount({4, 2}) == 2L);           // min=2, size=2 -> 2*1
    return 0;
}

// The problem is straightforward: find the minimum element in the vector and multiply it by `(size - 1)`. Sorting the vector first is unnecessary and inefficient for this task; a simple linear scan suffices. Initialize `minimum` to the first element, then iterate through the rest of the vector updating `minimum` whenever a smaller value is found. After the scan, return `minimum * (numbers.size() - 1)`. Edge cases: when the vector has only one element, the result is `minimum * 0 = 0`. Duplicate minimum values do not affect the result since only the minimum value is used. Negative values are handled naturally since we are comparing with `<`. Time complexity is O(n), where n is the number of elements, and space complexity is O(1) auxiliary (not counting the input vector). No sorting is needed, which preserves the original input and is more efficient.
