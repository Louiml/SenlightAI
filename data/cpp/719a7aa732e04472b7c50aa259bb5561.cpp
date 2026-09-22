Write a C++ function named `incrementLargeInteger` that takes a non-empty vector of integers `digits` representing a non-negative integer, where each element is a single digit from 0 to 9 (no leading zeros unless the number is zero, and the most significant digit is at index 0), and returns a new vector representing the value after adding one. The function must handle all valid inputs, including cases where all digits are 9 (e.g., `[9,9,9]` → `[1,0,0,0]`), a single digit `[9]` → `[1,0]`, and numbers like `[0]` → `[1]`. The original input vector must not be modified; the function should work with `const std::vector<int>&` and return a vector by value. Ensure proper handling of carry propagation across all digits.

The algorithm processes the digits from the least significant digit (rightmost, index `size-1`) toward the most significant (leftmost, index 0). For each digit, if it is less than 9, we can increment it by 1, and since no carry is generated beyond that point, we return the result immediately. If the digit is 9, we set it to 0 (carry of 1) and continue to the next more significant digit. If we finish the loop without returning (meaning every digit was 9), we need to prepend a `1` at the beginning of the vector to handle the carry overflow (e.g., 999 → 1000). Since we must not modify the input, we start by copying the input into a local mutable vector. Edge cases: empty vector should not occur (task guarantees non-empty); handling `[9]` → `[1,0]`; any vector with leading zeros except `[0]` should not occur per constraints but the algorithm still works correctly. Time complexity is O(n) in the worst case (all digits 9), and O(1) on average when the last digit is not 9; space complexity is O(1) auxiliary plus the O(n) for the returned copy.

#include <vector>

// Returns a vector representing the integer value of `digits` incremented by one.
// The input vector is not modified; each element must be a single digit (0-9).
std::vector<int> incrementLargeInteger(const std::vector<int>& digits) {
    std::vector<int> result = digits;  // make a mutable copy

    for (int i = static_cast<int>(result.size()) - 1; i >= 0; --i) {
        if (result[i] < 9) {
            ++result[i];
            return result;
        }
        result[i] = 0;  // carry to next more significant digit
    }

    // All digits were 9, so we need an extra leading 1 (e.g., 999 -> 1000)
    result.insert(result.begin(), 1);
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic increments
    assert((incrementLargeInteger({1, 2, 3}) == std::vector<int>{1, 2, 4}));
    assert((incrementLargeInteger({0}) == std::vector<int>{1}));
    assert((incrementLargeInteger({9}) == std::vector<int>{1, 0}));
    assert((incrementLargeInteger({1, 9}) == std::vector<int>{2, 0}));
    assert((incrementLargeInteger({9, 9}) == std::vector<int>{1, 0, 0}));
    assert((incrementLargeInteger({9, 9, 9}) == std::vector<int>{1, 0, 0, 0}));

    // Carry propagation through middle digits
    assert((incrementLargeInteger({1, 9, 9}) == std::vector<int>{2, 0, 0}));
    assert((incrementLargeInteger({8, 9, 9}) == std::vector<int>{9, 0, 0}));

    // Large vector with all nines
    std::vector<int> allNines(50, 9);
    std::vector<int> expectedAllNines(51, 0);
    expectedAllNines[0] = 1;
    assert(incrementLargeInteger(allNines) == expectedAllNines);

    // Input not modified
    std::vector<int> original = {9, 9};
    incrementLargeInteger(original);
    assert((original == std::vector<int>{9, 9}));
}
