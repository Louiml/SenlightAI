Write a C++ function `bool isNumberDivisibleByFour(int red, int green, int blue)` that takes three non-negative integers representing the counts of red, green, and blue items (each between 0 and 9 inclusive, a common constraint in RGB color problems), and returns `true` if the three-digit number formed by interpreting the red count as the hundreds digit, the green count as the tens digit, and the blue count as the ones digit is divisible by 4, and `false` otherwise. The function must accept the three counts as separate integer parameters, compute the combined number, and return a boolean result. The task should be solved without any input/output operations, focusing purely on arithmetic logic. Edge cases include values like (0,0,0) which forms 0 (divisible by 4), and values where the hundreds digit is zero, effectively forming a two-digit or one-digit number, which still must be checked correctly under the standard divisibility rule for 4 (last two digits divisible by 4). The function must be pure, deterministic, and avoid any global state.

The solution directly computes `n = red * 100 + green * 10 + blue` and checks whether `n % 4 == 0`. This works because integer arithmetic preserves the positional values exactly. The divisibility rule for 4 (last two digits divisible by 4) is a consequence of the fact that 100 is divisible by 4, so only the tens and ones digits matter; however, computing the full number and using the modulo operator is simpler and avoids manual extraction of digits. Edge cases: when all three counts are zero, `n = 0`, and `0 % 4 == 0`, so the function returns `true`. When blue and green are zero but red is non-zero, `n` is a multiple of 100, which is always divisible by 4, so the function returns `true`. Negative inputs are not expected per constraints, but if they were given, the modulo operator in C++ would yield a negative remainder, so we restrict to non-negative inputs. Time complexity is O(1) since only constant arithmetic operations are performed; space complexity is O(1) as well.

#include <cstdbool>

// Return true if the number formed by red (hundreds), green (tens), blue (ones) is divisible by 4.
bool isNumberDivisibleByFour(int red, int green, int blue) {
    const int number = red * 100 + green * 10 + blue;
    return number % 4 == 0;
}

#include <cassert>

int main() {
    // Basic cases
    assert(isNumberDivisibleByFour(1, 2, 3) == false); // 123 % 4 = 3
    assert(isNumberDivisibleByFour(1, 2, 4) == true);  // 124 % 4 = 0
    assert(isNumberDivisibleByFour(0, 0, 0) == true);  // 0 % 4 = 0
    assert(isNumberDivisibleByFour(4, 0, 0) == true);  // 400 % 4 = 0
    assert(isNumberDivisibleByFour(0, 1, 6) == true);  // 16 % 4 = 0
    assert(isNumberDivisibleByFour(0, 1, 7) == false); // 17 % 4 = 1
    assert(isNumberDivisibleByFour(9, 9, 9) == false); // 999 % 4 = 3
    assert(isNumberDivisibleByFour(9, 9, 6) == true);  // 996 % 4 = 0
    assert(isNumberDivisibleByFour(0, 0, 4) == true);  // 4 % 4 = 0
    assert(isNumberDivisibleByFour(0, 0, 1) == false); // 1 % 4 = 1
    return 0;
}
