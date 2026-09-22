// Write a C++ function that takes a non-negative integer `N` and returns a `std::vector<int>` containing the decimal digits of `N!` (factorial of `N`) in the correct order (most significant digit first). The factorial can be very large and exceed the range of built-in integer types, so you must implement arbitrary-precision multiplication using a vector to store digits in reverse order internally. For example, `factorialDigits(5)` should return `{1, 2, 0}`. Handle the edge case `N = 0` where `0! = 1`. The function signature should be `std::vector<int> factorialDigits(int N)`.

The algorithm computes the factorial by iteratively multiplying a vector of digits (stored in reverse order, with least significant digit at index 0) by each integer from 2 up to `N`. For each multiplication, we simulate the standard long-multiplication process: for each digit in the vector, compute `digit * i + carry`, store the last digit of the product in the current position, and pass the rest as carry to the next digit. After processing all existing digits, any remaining carry is appended as new most significant digits (still in reverse order). After the loop, reverse the vector to get the digits in the correct order. The initial vector is `{1}` representing `1!` and `0!`. Edge cases: `N = 0` and `N = 1` both return `{1}` since the loop does not execute. Time complexity is `O(N * D)` where `D` is the number of digits in `N!`, which grows roughly as `O(N log N)`, so overall time is `O(N^2 log N)` in the worst case. Space complexity is `O(D)` for the digit vector, which is the size of the output.

#include <vector>
#include <algorithm>

/**
 * @brief Computes the decimal digits of N! (factorial of N).
 * 
 * @param N Non-negative integer input.
 * @return std::vector<int> Digits of N! in most-significant-first order.
 */
std::vector<int> factorialDigits(int N) {
    std::vector<int> digits;
    digits.push_back(1); // 0! = 1 and 1! = 1

    for (int factor = 2; factor <= N; ++factor) {
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            int product = digits[i] * factor + carry;
            digits[i] = product % 10;
            carry = product / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    std::reverse(digits.begin(), digits.end());
    return digits;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above or included here.

int main() {
    // N = 0: 0! = 1
    assert(factorialDigits(0) == std::vector<int>{1});

    // N = 1: 1! = 1
    assert(factorialDigits(1) == std::vector<int>{1});

    // N = 5: 5! = 120
    assert(factorialDigits(5) == std::vector<int>{1, 2, 0});

    // N = 6: 6! = 720
    assert(factorialDigits(6) == std::vector<int>{7, 2, 0});

    // N = 10: 10! = 3628800
    assert(factorialDigits(10) == std::vector<int>{3, 6, 2, 8, 8, 0, 0});

    // N = 12: 12! = 479001600
    assert(factorialDigits(12) == std::vector<int>{4, 7, 9, 0, 0, 1, 6, 0, 0});

    // N = 20: 20! = 2432902008176640000 (20 digits)
    assert(factorialDigits(20) == std::vector<int>{2, 4, 3, 2, 9, 0, 2, 0, 0, 8, 1, 7, 6, 6, 4, 0, 0, 0, 0, 0});

    // N = 25: 25! = 15511210043330985984000000 (26 digits)
    assert(factorialDigits(25) == std::vector<int>{1, 5, 5, 1, 1, 2, 1, 0, 0, 4, 3, 3, 3, 0, 9, 8, 5, 9, 8, 4, 0, 0, 0, 0, 0, 0});

    return 0;
}
