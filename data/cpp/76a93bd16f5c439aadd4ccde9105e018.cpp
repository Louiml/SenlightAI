// Write a C++ function `std::vector<int> toNegativeBase2(int num)` that converts a non-negative integer `num` (including 0) into its representation in base -2 (negative base two). In base -2, each digit position represents powers of -2 (i.e., ... , 4, -2, 1). The standard representation must contain only digits 0 and 1, have no leading zeros (except for the number 0 itself, which is represented as a single digit 0), and be returned as a vector of integers in most-significant-digit-first order (e.g., for input 6, the output should be {1, 1, 0, 1, 0} because 1*(-2)^4 + 1*(-2)^3 + 0*(-2)^2 + 1*(-2)^1 + 0*(-2)^0 = 16 - 8 + 0 - 2 + 0 = 6). The function must handle `num = 0` correctly and must not use any external libraries beyond the standard ones.

The algorithm repeatedly divides `num` by -2, but with a special rule for odd remainders. At each step, we compute `num % (-2)`. Because C++'s modulo operation with negative divisors yields a result with the sign of the dividend, we cannot directly use the remainder when `num` is negative. Instead, we check if `num` is even: if even, the digit is 0 and `num` becomes `num / -2`. If odd, the digit must be 1, but to make the division exact in base -2, we subtract 1 from `num` before dividing by -2 (i.e., `num = (num - 1) / -2`). This works because if `num` is odd, `num - 1` is even, ensuring the division yields an integer. Continue until `num` becomes 0. During the loop, we push digits in least-significant-first order; after the loop, we remove any trailing zeros that were added (though by construction only leading zeros can occur, but we pop back zeros for safety), and if the vector is empty (for input 0), we set it to {0}. Finally, reverse the vector to get most-significant-first order. Edge case: input 0 returns {0}. Input 1 returns {1}. Input 2 returns {1,1,0}? Let's check: 2 -> even -> digit 0, num = 2 / -2 = -1; -1 odd -> digit 1, num = (-1-1)/-2 = 1; 1 odd -> digit 1, num = (1-1)/-2 = 0. Digits in reverse: {1,1,0} after reversal? Actually we push 0, then 1, then 1; reversal gives {1,1,0}. Check: 1*(-2)^2 + 1*(-2)^1 + 0*(-2)^0 = 4 - 2 + 0 = 2. Correct. Complexity: O(log|num|) time and O(log|num|) space (the vector size) because the number of digits is logarithmic in the magnitude.

#include <vector>
#include <algorithm>

// Convert a non-negative integer to its representation in base -2.
// Returns a vector of digits (0 or 1) in most-significant-first order.
std::vector<int> toNegativeBase2(int num) {
    std::vector<int> digits;
    if (num == 0) {
        digits.push_back(0);
        return digits;
    }
    while (num != 0) {
        if (num % (-2) == 0) {
            digits.push_back(0);
            num /= -2;
        } else {
            digits.push_back(1);
            num = (num - 1) / -2;
        }
    }
    // At this point digits are least-significant-first.
    // Remove any trailing zeros (which correspond to leading zeros after reversal).
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        digits.push_back(0);
    }
    std::reverse(digits.begin(), digits.end());
    return digits;
}

#include <cassert>
#include <vector>

// Declaration of the solution function.
std::vector<int> toNegativeBase2(int num);

int main() {
    assert(toNegativeBase2(0) == std::vector<int>{0});
    assert(toNegativeBase2(1) == std::vector<int>{1});
    assert(toNegativeBase2(2) == std::vector<int>{1, 1, 0});
    assert(toNegativeBase2(3) == std::vector<int>{1, 1, 1});
    assert(toNegativeBase2(4) == std::vector<int>{1, 0, 0});
    assert(toNegativeBase2(5) == std::vector<int>{1, 0, 1});
    assert(toNegativeBase2(6) == std::vector<int>{1, 1, 0, 1, 0});
    assert(toNegativeBase2(7) == std::vector<int>{1, 1, 0, 1, 1});
    assert(toNegativeBase2(10) == std::vector<int>{1, 1, 1, 1, 0});
    assert(toNegativeBase2(100) == std::vector<int>{1, 1, 0, 1, 0, 0, 1, 0, 0});
}
