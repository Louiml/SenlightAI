// Write a C++ function that takes a non-negative integer `N` as input and returns a `std::vector<int>` representing the decimal digits of `N!` (factorial of N) in normal order (most significant digit first). The result must handle arbitrarily large factorial values that exceed built-in integer types (e.g., long long). For example, `factorial(5)` should return `{1, 2, 0}` (representing 120), and `factorial(20)` should return the digits of 2432902008176640000. Your function should work correctly for `N = 0` (returning `{1}`) and `N = 1` (returning `{1}`). Do not use any big-integer library; implement the multiplication manually using a vector to store digits in reverse order.

#include <cassert>
#include <vector>

// (The solution function is assumed to be available above.)

int main() {
    // 0! = 1
    assert(factorialDigits(0) == std::vector<int>({1}));
    // 1! = 1
    assert(factorialDigits(1) == std::vector<int>({1}));
    // 5! = 120
    assert(factorialDigits(5) == std::vector<int>({1, 2, 0}));
    // 10! = 3628800
    assert(factorialDigits(10) == std::vector<int>({3, 6, 2, 8, 8, 0, 0}));
    // 20! = 2432902008176640000
    assert(factorialDigits(20) == std::vector<int>({2, 4, 3, 2, 9, 0, 2, 0, 0, 8, 1, 7, 6, 6, 4, 0, 0, 0, 0}));
    // 25! = 15511210043330985984000000
    assert(factorialDigits(25) == std::vector<int>({1, 5, 5, 1, 1, 2, 1, 0, 0, 4, 3, 3, 3, 0, 9, 8, 5, 9, 8, 4, 0, 0, 0, 0, 0}));
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the decimal digits (most significant first) of N! as a vector<int>.
// N must be a non-negative integer.
std::vector<int> factorialDigits(int N) {
    std::vector<int> digits;
    digits.push_back(1); // 0! = 1! = 1

    for (int multiplier = 2; multiplier <= N; ++multiplier) {
        int carry = 0;

        for (size_t idx = 0; idx < digits.size(); ++idx) {
            int value = digits[idx] * multiplier + carry;
            digits[idx] = value % 10;
            carry = value / 10;
        }

        while (carry != 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    std::reverse(digits.begin(), digits.end());
    return digits;
}

// The main idea is to compute the factorial iteratively, multiplying the current result (stored digit by digit) by each integer from 2 to N. Represent the evolving product as a `std::vector<int>` where each element holds a single decimal digit (0-9), and for easy carry handling, store the digits in reverse order (least significant digit at index 0). Start with the vector containing `{1}` (representing 1). For each multiplier `i` from 2 to N, iterate through every existing digit position, multiply that digit by `i`, add the carry from the previous position, store the result's last digit (`num % 10`) at the current position, and pass the quotient (`num / 10`) as the new carry. After processing all existing digits, append any remaining carry digits one by one using modulo and division by 10. After completing all multiplications, reverse the vector to place the most significant digit first, and return it. Edge cases: `N = 0` and `N = 1` both return `{1}` because the loop starts at 2. The time complexity is O(N * D) where D is the number of digits in N!, which grows as O(N log N) for large N, so overall roughly O(N² log N) in the worst case. Space complexity is O(D) to store the digits, where D = floor(log10(N!)) + 1.
