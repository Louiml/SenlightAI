/*
Write a C++ function `bool isHappyNumber(int n)` that determines whether a positive integer `n` is a "happy number." A happy number is defined by the following process: starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle that does not include 1. The function should return `true` if `n` is happy, and `false` otherwise. The input `n` is guaranteed to be a positive integer (greater than 0). Your solution should correctly handle potential infinite cycles and must not use any external libraries beyond the C++ standard library.
*/

#include <cassert>
#include <cmath>

// Compute the sum of the squares of the digits of n.
int sumOfSquares(int n) {
    int sum = 0;
    while (n > 0) {
        int digit = n % 10;
        sum += digit * digit;
        n /= 10;
    }
    return sum;
}

// Return true if n is a happy number, false otherwise.
bool isHappyNumber(int n) {
    int slow = n;
    int fast = n;
    do {
        slow = sumOfSquares(slow);               // Move slow by 1 step
        fast = sumOfSquares(sumOfSquares(fast)); // Move fast by 2 steps
    } while (slow != fast);
    return slow == 1; // If cycle ends at 1, n is happy.
}

int main() {
    // Basic happy numbers
    assert(isHappyNumber(1) == true);
    assert(isHappyNumber(19) == true);
    assert(isHappyNumber(100) == true);

    // Basic unhappy numbers
    assert(isHappyNumber(2) == false);
    assert(isHappyNumber(4) == false);
    assert(isHappyNumber(5) == false);

    // Larger numbers
    assert(isHappyNumber(7) == true);
    assert(isHappyNumber(20) == false);
    assert(isHappyNumber(68) == true);
    assert(isHappyNumber(139) == true);
    assert(isHappyNumber(86) == false);

    // Edge: very small positive integer (e.g., 0 not allowed per spec, but test 0 as a boundary)
    // We skip since n is guaranteed positive.
    return 0;
}

// The core algorithm is to repeatedly compute the sum of squares of digits until either we reach 1 (happy) or we detect a cycle (unhappy). There are two main approaches:
// 1. **Hash-set detection**: Store every number encountered during the iteration in an unordered_set. If we encounter a number already in the set before reaching 1, then we are in a cycle, and the number is not happy. If we reach 1, it is happy.
// 2. **Floyd’s cycle detection (tortoise and hare)**: Use two pointers, `slow` and `fast`, both starting at `n`. In each iteration, `slow` moves one step (one sum-of-squares computation) and `fast` moves two steps (two computations). If they meet, we have a cycle; the number is happy only if the meeting point is 1.
//
// Edge cases: The smallest happy number is 1 (which is already 1, so loop never runs). The process is guaranteed to eventually cycle for any non-happy number, and the cycle never includes 1. The sum of squares of digits for any number reduces the magnitude significantly for larger numbers, but for small numbers like 2, 3, 4, 5, 6, 8, 9, the process cycles. The time complexity is O(log n) steps per iteration (to extract digits) and the number of iterations is bounded (the process either reaches 1 or enters a cycle, and in practice the number of steps is logarithmic in `n`). Overall time is O(log n) amortized, and space is O(1) for Floyd’s method or O(cycle length) for the set method, but practically O(log n) storage in the set. The solution below uses Floyd’s cycle detection for constant auxiliary space.
