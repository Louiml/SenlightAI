Write a C++ function `bool isHappyNumber(int n)` that determines whether a positive integer `n` is a "happy number" according to the following process: repeatedly replace the number by the sum of the squares of its digits. If the process eventually reaches 1, the number is happy; if it enters a cycle that never includes 1, it is not happy. The function must handle any positive integer value within the `int` range, including edge cases like `1` (which is immediately happy) and large values that may produce sums exceeding the original input (though still within `int` range). The solution must detect cycles without using any external libraries beyond the standard C++ headers, and must not modify the input parameter. Return `true` if the number is happy, and `false` otherwise.

The algorithm simulates the process using a hash set (or unordered map) to track previously seen numbers. Starting with the input `n`, we loop while `n` is not yet 1 and not previously seen. In each iteration, we compute the sum of squares of the digits of `n` by extracting each digit via modulo and division, then update `n` to that sum. If we encounter a number already in the set, we have entered a cycle and return `false`. If we reach 1, we return `true`. Edge cases: `n = 1` returns `true` immediately; `n = 0` is not a positive integer, but if it were passed, the loop would skip and incorrectly return `true`, so the function should assume positive input. The cycle detection is critical because the process is guaranteed to either reach 1 or enter a cycle for any positive integer (a known mathematical fact). Time complexity is O(log n) per iteration for digit sum, and the overall complexity is O(k * log n) where k is the number of steps until 1 or cycle; practically, k is small (often ≤ 20) for typical inputs. Space complexity is O(k) for storing seen numbers (using unordered_set or unordered_map).

#include <unordered_set>

// Determine if a positive integer n is a happy number.
bool isHappyNumber(int n) {
    std::unordered_set<int> seen;
    while (n != 1 && seen.find(n) == seen.end()) {
        seen.insert(n);
        int sum = 0;
        while (n > 0) {
            int digit = n % 10;
            sum += digit * digit;
            n /= 10;
        }
        n = sum;
    }
    return n == 1;
}

#include <cassert>

int main() {
    // Basic happy numbers
    assert(isHappyNumber(1) == true);
    assert(isHappyNumber(7) == true);
    assert(isHappyNumber(19) == true);
    assert(isHappyNumber(100) == true);
    // Basic unhappy numbers
    assert(isHappyNumber(2) == false);
    assert(isHappyNumber(4) == false);
    assert(isHappyNumber(20) == false);
    // Larger values
    assert(isHappyNumber(12345) == false);
    assert(isHappyNumber(999999999) == false);
    assert(isHappyNumber(1000000000) == true);
    // Edge case: value that cycles quickly
    assert(isHappyNumber(3) == false);
    assert(isHappyNumber(5) == false);
    return 0;
}
