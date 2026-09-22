/*
Write a C++ function named `extractDigits` that takes a non-negative integer `N` as input and returns a `std::vector<int>` containing its decimal digits in reverse order (i.e., from least significant to most significant). If `N` is 0, the function must return a vector containing the single element `{0}`. The function should not print anything; it should only return the collected digits. For full credit, the function must be `const`-correct (mark the parameter as `const int N` if it is passed by value, and ensure the return type is appropriate), and the implementation must use a loop with modulo and division operations. The solution must be self-contained and usable as a standalone exercise; do not include a `main` function in the function definition.
*/
#include <vector>

// Extracts digits of a non-negative integer in reverse order (least significant first).
// If N is 0, returns {0}.
std::vector<int> extractDigits(const int N) {
    std::vector<int> digits;
    if (N == 0) {
        digits.push_back(0);
        return digits;
    }

    int value = N; // copy to a mutable local variable
    while (value > 0) {
        digits.push_back(value % 10);
        value /= 10;
    }
    return digits;
}
#include <cassert>
#include <vector>

// Forward declaration for testing (normally in a header)
std::vector<int> extractDigits(const int N);

int main() {
    assert(extractDigits(0) == std::vector<int>({0}));
    assert(extractDigits(5) == std::vector<int>({5}));
    assert(extractDigits(123) == std::vector<int>({3, 2, 1}));
    assert(extractDigits(1000) == std::vector<int>({0, 0, 0, 1}));
    assert(extractDigits(987654321) == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
    assert(extractDigits(1) == std::vector<int>({1}));
    assert(extractDigits(10) == std::vector<int>({0, 1}));
    assert(extractDigits(999) == std::vector<int>({9, 9, 9}));
    return 0;
}
// The core algorithm is straightforward: repeatedly extract the last digit of the number using the modulo operator `% 10`, store that digit in a vector, then remove the last digit by integer division `/ 10`. This process continues until the number becomes 0. The key edge case is when the input is exactly 0: the loop condition `while (N > 0)` would immediately terminate, leaving an empty vector, which is incorrect. Therefore, we must check if `N == 0` at the start and return a vector with a single `0` in that case. For all positive integers, the loop runs once per digit, so for a number with `d` digits, the time complexity is `O(d)`, and the auxiliary space complexity is `O(d)` because we store exactly `d` digits in the vector. The modulo and division operations are constant-time per iteration, so the overall efficiency is linear in the number of digits, which is optimal since we must touch every digit.
