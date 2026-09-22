// Write a C++ function that takes a non-negative `long long` integer and returns a `std::vector<int>` where each digit of the original number appears as a separate element, but in **reversed** order (i.e., the units digit first, then tens, hundreds, etc.). For example, given `12345`, the result should be `{5, 4, 3, 2, 1}`. The input will be non-negative and may be as large as `9,223,372,036,854,775,807` (the maximum value of `long long`). The function should handle `0` correctly: since `0` has no digits in the standard interpretation, return an empty vector. Do not use string conversion or any digit-counting library; compute the digits arithmetically.

// The algorithm repeatedly extracts the last digit of `n` using the modulo operator (`n % 10`) and appends it to the output vector. Then it removes that digit by integer division (`n /= 10`). Because we push digits in the order they are extracted (units first), the vector naturally ends up reversed compared to the usual left-to-right digit order. The loop continues until `n` becomes zero. The main edge case is `n == 0`: the loop would never execute because `while (n)` is false initially, so we must explicitly return an empty vector in that case (or handle it before the loop). For any positive input, the loop runs exactly as many times as there are decimal digits. Time complexity is O(d), where d is the number of digits (at most 19 for `long long`). Space complexity is O(d) for the output vector; no extra auxiliary space is used beyond a few variables. Use `const` for the input parameter because the function does not modify it (though we need a local copy, so we can take the parameter by value and modify that copy, or take a const reference and copy inside—either is fine; by-value is simplest and allows direct modification).

#include <vector>

// Return the decimal digits of n in reversed order (units digit first).
// For n == 0, return an empty vector.
std::vector<int> reverseDigits(long long n) {
    std::vector<int> result;
    if (n == 0) {
        return result;  // empty vector for zero
    }
    while (n > 0) {
        result.push_back(static_cast<int>(n % 10));
        n /= 10;
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared here (for this test block).
std::vector<int> reverseDigits(long long n);

int main() {
    assert(reverseDigits(0) == std::vector<int>{});
    assert(reverseDigits(7) == std::vector<int>{7});
    assert(reverseDigits(10) == std::vector<int>{0, 1});
    assert(reverseDigits(12345) == std::vector<int>{5, 4, 3, 2, 1});
    assert(reverseDigits(100000) == std::vector<int>{0, 0, 0, 0, 0, 1});
    assert(reverseDigits(9876543210LL) == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    assert(reverseDigits(9223372036854775807LL) == std::vector<int>{
        7, 0, 8, 5, 7, 7, 4, 5, 8, 6, 3, 0, 2, 7, 3, 3, 2, 2, 9});
    return 0;
}
