Write a C++ function that determines whether a given positive integer `n` can be represented as a sum of distinct powers of three (e.g., 3^0, 3^1, 3^2, ...). A number is valid if and only if each power of three can be used at most once, and no negative or fractional powers are allowed. The function should return `true` if such a representation exists, and `false` otherwise. For example, 12 = 9 + 3 = 3^2 + 3^1 is valid, but 13 = 9 + 3 + 1 = 3^2 + 3^1 + 3^0 is also valid because each power is used once, so all distinct. However, 5 = 3^1 + 1 + 1 is invalid because it requires two copies of 3^0. The input `n` is guaranteed to be a positive integer (≥ 1) and fits within a 32-bit signed integer.
// The problem is equivalent to checking whether `n` can be written in base-3 using only digits 0 and 1 (no digit 2), because each distinct power of three corresponds to a position in the ternary representation where the digit is either 0 (not used) or 1 (used once). If any digit in the base-3 representation is 2, then that would require using the same power of three twice, which is forbidden. Therefore, the algorithm repeatedly divides `n` by 3 and checks the remainder: if the remainder is ever 2, return `false`. If the loop completes without encountering a remainder of 2, return `true`. Edge cases: `n = 1` (ternary "1") is valid; `n = 2` (ternary "2") is invalid; `n = 3` (ternary "10") is valid; `n = 4` (ternary "11") is valid; `n = 5` (ternary "12") has a 2 in the units place and is invalid. Time complexity is O(log₃ n) because each iteration removes one ternary digit. Space complexity is O(1) as we only use a few integer variables.
#include <cstdint>

// Returns true if the positive integer n can be expressed as a sum of distinct powers of three.
bool isSumOfDistinctPowersOfThree(int n) {
    while (n > 0) {
        // If any base-3 digit is 2, a power would need to be used more than once.
        if (n % 3 == 2) {
            return false;
        }
        n /= 3;
    }
    return true;
}
#include <cassert>

int main() {
    assert(isSumOfDistinctPowersOfThree(1) == true);   // 3^0
    assert(isSumOfDistinctPowersOfThree(3) == true);   // 3^1
    assert(isSumOfDistinctPowersOfThree(4) == true);   // 3^1 + 3^0
    assert(isSumOfDistinctPowersOfThree(12) == true);  // 3^2 + 3^1
    assert(isSumOfDistinctPowersOfThree(13) == true);  // 3^2 + 3^1 + 3^0
    assert(isSumOfDistinctPowersOfThree(2) == false);  // would need two 3^0
    assert(isSumOfDistinctPowersOfThree(5) == false);  // 3^1 + 3^0 + 3^0
    assert(isSumOfDistinctPowersOfThree(15) == false); // 3^2 + 3^1 + 3^1
    assert(isSumOfDistinctPowersOfThree(27) == true);  // 3^3
    assert(isSumOfDistinctPowersOfThree(28) == true);  // 3^3 + 3^0
}
