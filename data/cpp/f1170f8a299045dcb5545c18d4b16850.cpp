/*
Design a C++ function `splitEvenly` that takes a vector of integers and returns a new vector where all even numbers have been replaced by their smallest possible positive integer square root if the number is a perfect square of an even integer, and unchanged otherwise. For example, `16` becomes `4`, `36` becomes `6`, `4` becomes `2`, but `12` stays `12`, `9` stays `9` (since 3 is odd), and `0` stays `0`. The function should preserve the relative order of elements and work for any vector size, including empty vectors. Use exact integer arithmetic; do not use floating-point functions.
*/
#include <vector>
#include <cstddef>

// Replace even numbers that are perfect squares of even integers with their square root.
// Preserve order; leave all other numbers unchanged.
std::vector<int> splitEvenly(const std::vector<int>& input) {
    std::vector<int> result;
    result.reserve(input.size());

    for (int value : input) {
        if (value <= 0 || value % 2 != 0) {
            // Non-positive or odd numbers are unchanged (including 0).
            result.push_back(value);
            continue;
        }

        // Find integer square root via binary search (value >= 2, value is even).
        int low = 0;
        int high = value;  // value >= 2, so high is at least 2
        int root = 0;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            long long square = static_cast<long long>(mid) * mid; // avoid overflow
            if (square == value) {
                root = mid;
                break;
            } else if (square < value) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        // If root*root == value, push the root (which is even by construction),
        // otherwise push the original value.
        if (root * root == value) {
            result.push_back(root);
        } else {
            result.push_back(value);
        }
    }

    return result;
}
#include <vector>
#include <cassert>

// Free function declared above (or included from header) — here we assume it's available.
std::vector<int> splitEvenly(const std::vector<int>& input);

int main() {
    // Basic cases
    assert((splitEvenly({16, 36, 4, 12, 9, 0, -4}) == std::vector<int>{4, 6, 2, 12, 9, 0, -4}));
    
    // Empty input
    assert((splitEvenly({}) == std::vector<int>{}));
    
    // No even perfect squares
    assert((splitEvenly({1, 3, 5, 7, 11}) == std::vector<int>{1, 3, 5, 7, 11}));
    
    // Even numbers that are not perfect squares
    assert((splitEvenly({2, 6, 10, 14}) == std::vector<int>{2, 6, 10, 14}));
    
    // Larger square: 100 -> 10, 144 -> 12
    assert((splitEvenly({100, 144, 196}) == std::vector<int>{10, 12, 14}));
    
    // Odd perfect square remains unchanged
    assert((splitEvenly({25, 49, 81}) == std::vector<int>{25, 49, 81}));
    
    // Mixed vector preserving order
    assert((splitEvenly({4, 5, 9, 16, 8, 36, 0}) == std::vector<int>{2, 5, 9, 4, 8, 6, 0}));
    
    // Duplicate values
    assert((splitEvenly({4, 4, 4}) == std::vector<int>{2, 2, 2}));
    
    // Large even perfect square (2^5 = 32? no, 1024 = 32^2)
    assert((splitEvenly({1024, 4096}) == std::vector<int>{32, 64}));
    
    // Single element
    assert((splitEvenly({64}) == std::vector<int>{8}));
    
    return 0;
}
// The solution iterates over the input vector and for each element checks if it is an even number that is a perfect square of an even integer. A number `n` qualifies if `n` is even, `n > 0`, and there exists an even integer `k` such that `k * k == n`. To find `k` without floating-point, we can compute the integer square root via a simple loop (or binary search) from 0 upward until `k*k > n`. If `k*k == n` and `k` is even, replace `n` with `k`. Since even numbers have even squares (and conversely, if `n` is even and a perfect square, its square root must be even because the square of an odd number is odd), we can simplify: if `n` is even and its integer square root squared equals `n`, then the root is necessarily even. For numbers where the square root is not integer, we leave the number unchanged. Edge cases: `0` has root 0 (which is even), but since `0` is a perfect square, some interpretations might change it; the task specifies `0` should remain unchanged, so handle `n == 0` explicitly. Negative numbers are not perfect squares in integer arithmetic, so they remain unchanged. Time complexity is O(m * sqrt(max_n)) per element in the worst case if we use linear search for the root, but we can optimize with binary search to O(m * log(max_n)). Space complexity is O(m) for the output vector. The task expects a free function without a `main` in the solution section.
