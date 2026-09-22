/*
Write a C++ function `findOriginalArray` that takes a vector of integers `changed` (representing a doubled array where each element is either an original number or exactly twice an original number, and the original array has length `n/2`) and returns the original array. The input vector may contain duplicates, zeros, and negative numbers. If the input length is odd or if it is impossible to reconstruct a valid original array, return an empty vector. The returned original array should be sorted in non-decreasing order. For example, given `[1, 3, 4, 2, 6, 8]`, the original array is `[1, 3, 4]` because each element doubled yields `[2, 6, 8]`. For `[1, 2, 3]` (odd length) or `[1, 2, 4]` (impossible), return `{}`.
*/

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the original array that, when each element is doubled and merged with
// the originals, forms 'changed'. Returns an empty vector if impossible.
std::vector<int> findOriginalArray(const std::vector<int>& changed) {
    const int n = static_cast<int>(changed.size());
    if (n % 2 != 0) {
        return {};
    }

    auto sorted = changed;
    std::sort(sorted.begin(), sorted.end());

    std::unordered_map<int, int> count;
    for (const int number : sorted) {
        ++count[number];
    }

    std::vector<int> original;
    original.reserve(n / 2);

    for (const int number : sorted) {
        if (count[number] == 0) {
            continue;
        }
        const int doubled = number * 2;  // Note: number << 1 is also fine.
        if (count[doubled] == 0) {
            return {};
        }
        original.push_back(number);
        --count[number];
        --count[doubled];
    }
    return original;
}

#include <cassert>
#include <vector>

// The solution function is available (assumed to be included from above).
// Global main function with assertions.
int main() {
    // Basic case with positive numbers and duplicates
    assert(findOriginalArray({1, 3, 4, 2, 6, 8}) == std::vector<int>({1, 3, 4}));
    // Odd length -> impossible
    assert(findOriginalArray({1, 2, 3}) == std::vector<int>());
    // Impossible pairing
    assert(findOriginalArray({1, 2, 4}) == std::vector<int>());
    // Empty input
    assert(findOriginalArray({}) == std::vector<int>());
    // All zeros even count
    assert(findOriginalArray({0, 0}) == std::vector<int>({0}));
    // All zeros odd count -> impossible
    assert(findOriginalArray({0, 0, 0}) == std::vector<int>());
    // Negative numbers
    assert(findOriginalArray({-2, -4, 2, 4}) == std::vector<int>({-4, -2}));
    // Mixed negatives and positives, leading to failure
    assert(findOriginalArray({-1, -2, 2, 4}) == std::vector<int>());
    // Larger example with duplicates
    assert(findOriginalArray({1, 2, 1, 2, 2, 4}) == std::vector<int>({1, 1, 2}));
    // Single pair with zero
    assert(findOriginalArray({0, 0, 0, 0}) == std::vector<int>({0, 0}));
    return 0;
}

// The key insight is twofold: (1) The length must be even; otherwise, it is impossible to form pairs. (2) Sort the `changed` array in ascending order. Since every doubled value is at least as large as its original (and strictly larger for positive numbers, equal for zero), processing in sorted order ensures that when we encounter a number, if it is still available, it must be an original (because its smaller counterpart would have been processed earlier if it existed). We maintain a frequency map (`unordered_map<int,int>`) of all elements. Iterate through sorted `changed`; if the current number's count is zero, skip it. Otherwise, attempt to match it with its double (`number * 2`). If the double's count is zero, return empty. Otherwise, add the current number to the result, decrement counts for both the current number and its double. For zero, this works because `0 * 2 == 0`, and the count logic naturally pairs zeros: if `count[0]` is odd, the double's count will become zero prematurely and cause a failure; if even, all zeros get paired correctly. After processing, the result vector is already sorted because we iterate in sorted order. Edge cases include empty input (returns empty), all zeros (only valid if count is even), and negative numbers (doubles may be smaller in absolute value but still larger in sorted order because of negatives; however, since we sort, a negative number and its double are adjacent enough? Actually consider `[-2, -4]`: sorted is `[-4, -2]`. When we process `-4`, its double is `-8` which isn't present – this correctly fails because `-4` cannot be an original (its double would be `-8` which is not in the array). When we process `-2`, its double is `-4` which is present, so `-2` becomes original and `-4` is matched – correct. So sorting works for negatives as well. Complexity: Sorting takes `O(n log n)`, the frequency map build and iteration take `O(n)` time and `O(n)` auxiliary space (for the map and result).
