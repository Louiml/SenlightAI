/*
Write a C++ function named `makeMonotoneIncreasing` that takes a non-negative integer `n` and returns the largest integer less than or equal to `n` whose digits are in non-decreasing order (i.e., each digit is less than or equal to the digit to its right). For example, given `n = 332`, the function should return `299`, and given `n = 1234`, it should return `1234` (already monotone). The input is guaranteed to be a non-negative integer, and the function must work for values up to `2,147,483,647` (the maximum `int`). The function should be efficient and avoid excessive string operations if possible, but a string-based approach is acceptable. Return the result as an `int`.
*/

#include <string>
#include <algorithm>

// Returns the largest integer <= n whose digits are non-decreasing (monotone increasing).
int makeMonotoneIncreasing(int n) {
    std::string digits = std::to_string(n);
    const int len = static_cast<int>(digits.size());

    for (int i = len - 1; i >= 1; --i) {
        if (digits[i] < digits[i - 1]) {
            // Decrement the previous digit and turn all following digits into '9'.
            --digits[i - 1];
            for (int j = i; j < len; ++j) {
                digits[j] = '9';
            }
        }
    }

    // stoi handles leading zeros (e.g., "09" becomes 9).
    return std::stoi(digits);
}

int main() {
    assert(makeMonotoneIncreasing(1234) == 1234);
    assert(makeMonotoneIncreasing(332) == 299);
    assert(makeMonotoneIncreasing(10) == 9);
    assert(makeMonotoneIncreasing(0) == 0);
    assert(makeMonotoneIncreasing(9) == 9);
    assert(makeMonotoneIncreasing(111) == 111);
    assert(makeMonotoneIncreasing(120) == 119);
    assert(makeMonotoneIncreasing(1000) == 999);
    assert(makeMonotoneIncreasing(2147483647) == 1999999999); // Note: 1999999999 is <= max int, but 2147483647 is not monotone
    assert(makeMonotoneIncreasing(987654321) == 899999999);
    return 0;
}

// The solution works by converting the integer to a string and scanning from right to left to find the first position where a digit is smaller than the digit to its left (i.e., `temp[i] < temp[i-1]`). When such a violation is found, we need to decrement the left digit (`temp[i-1]--`) and set all digits to the right of that position to `'9'` to make the number as large as possible while still being less than the original. Because decrementing `temp[i-1]` might itself create a new violation with the digit to its left, we continue scanning leftwards; the loop naturally handles this because after modifying, we continue iterating from the end again. However, note that after setting the right part to `'9'`, the suffix is already non-decreasing, so re-scanning from the end is safe. Edge cases: if `n` is already monotone (e.g., `1234`), the function returns it unchanged. If `n` is a single digit, it is always monotone. If `n` is like `10`, we get `"10"` → scan: `0 < 1` → decrement `1` to `0`, set suffix to `'9'` → `"09"` → `stoi` gives `9`. Time complexity is `O(d^2)` in the worst case because for each detected violation we may update a suffix, but with the given implementation it is `O(d^2)` where `d` is the number of digits (at most 10 for `int`), so effectively constant. Space complexity is `O(d)` for the string.
