// Write a C++ function that rearranges a vector of integers so that all even numbers appear first (in their original relative order) followed by all odd numbers (in their original relative order). The function must not use extra storage proportional to the input size beyond the returned vector; however, it may use a single auxiliary vector. The input vector may be empty, contain only evens, only odds, duplicates, negatives, and large values. The function should accept the input by `const` reference and return the rearranged vector by value.

#include <cassert>
#include <vector>

// The solution function is declared above (included for completeness).
// Test cases:
int main() {
    // Empty input
    assert(sortArrayByParity({}) == std::vector<int>{});

    // Mixed numbers
    assert(sortArrayByParity({3, 1, 2, 4}) == std::vector<int>({2, 4, 3, 1}));
    assert(sortArrayByParity({1, 2, 3, 4, 5}) == std::vector<int>({2, 4, 1, 3, 5}));

    // Only evens
    assert(sortArrayByParity({2, 4, 6}) == std::vector<int>({2, 4, 6}));

    // Only odds
    assert(sortArrayByParity({1, 3, 5}) == std::vector<int>({1, 3, 5}));

    // Duplicates and negatives
    assert(sortArrayByParity({-2, -1, 0, 1, 2}) == std::vector<int>({-2, 0, 2, -1, 1}));

    // Single even
    assert(sortArrayByParity({4}) == std::vector<int>({4}));

    // Single odd
    assert(sortArrayByParity({7}) == std::vector<int>({7}));

    // Long sequence with interleaved parity
    assert(sortArrayByParity({2, 1, 4, 3, 6, 5, 8, 7}) == std::vector<int>({2, 4, 6, 8, 1, 3, 5, 7}));

    // All same parity but duplicates
    assert(sortArrayByParity({0, 0, 0}) == std::vector<int>({0, 0, 0}));
    assert(sortArrayByParity({9, 9, 9}) == std::vector<int>({9, 9, 9}));

    return 0;
}

#include <vector>

// Rearrange vector so evens appear first (stable), then odds (stable).
// Returns a new vector; input is not modified.
std::vector<int> sortArrayByParity(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<int> result(n, 0);
    int left = 0;      // next position for an even number
    int right = n - 1; // next position for an odd number (from the end)

    for (int value : nums) {
        if (value % 2 == 0) {
            result[left] = value;
            ++left;
        } else {
            result[right] = value;
            --right;
        }
    }
    return result;
}

// The algorithm uses a two-pointer placement technique into a result vector of the same size, initialized to zeros. A left pointer `j` starts at index 0 and is used to place even numbers in their original order. A right pointer `k` starts at `n-1` and is used to place odd numbers from the end backward, which preserves their original order when the vector is read from left to right later (because the first odd encountered is placed at the last slot, the second odd at second-to-last, etc.). Iterate through the input once; for each element, check parity using `% 2 == 0`. Place evens at `j` and increment it, place odds at `k` and decrement it. This ensures all evens occupy indices `[0, countEvens-1]` and odds occupy `[countEvens, n-1]`. The result is exactly the input with evens first then odds, each group in original order. Edge cases: empty input returns an empty vector; only evens or only odds work because the corresponding pointer moves only in one direction; negative numbers are handled by `%` in C++ (remainder has sign of dividend, but zero check works). Time complexity is O(n), space complexity is O(n) for the returned vector (the auxiliary vector is returned directly, so no extra storage beyond the output). No in-place modification is required.
