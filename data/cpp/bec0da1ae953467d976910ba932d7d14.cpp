Write a C++ function named `countLuckyEnough` that takes three parameters: a vector of integers `numbers`, and an integer `k` representing the maximum allowed number of lucky digits (4 or 7) per number. The function should return the count of numbers in the vector that contain at most `k` lucky digits in their decimal representation. For example, the number 447 has 3 lucky digits, and 123 has 0. The function must handle negative numbers correctly (ignore the minus sign when counting lucky digits), zero values (which have 0 lucky digits), and large integers up to 10^18. Edge cases include an empty vector (return 0), `k` being 0 (only numbers with no lucky digits count), and `k` being large (all numbers count).
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(countLuckyEnough({4, 7, 47, 123}, 2) == 4);
    assert(countLuckyEnough({4, 7, 47, 123}, 1) == 3);
    assert(countLuckyEnough({4, 7, 47, 123}, 0) == 1);
    
    // Negative numbers - sign ignored
    assert(countLuckyEnough({-4, -77, -123}, 2) == 3);
    assert(countLuckyEnough({-4, -77, -123}, 1) == 2);
    
    // Zero
    assert(countLuckyEnough({0, 0, 4}, 0) == 2);
    
    // Large numbers
    assert(countLuckyEnough({4444444444LL, 1234567890LL, 7777777777LL}, 5) == 2);
    
    // Empty vector
    assert(countLuckyEnough({}, 10) == 0);
    
    // Large k - all count
    assert(countLuckyEnough({444, 777, 123}, 10) == 3);
    
    // Mixed
    assert(countLuckyEnough({4, 14, 24, 34, 40, 41, 42, 43, 44}, 2) == 9);
    assert(countLuckyEnough({4, 14, 24, 34, 40, 41, 42, 43, 44}, 1) == 4);
    
    return 0;
}
#include <vector>
#include <cstdlib>

// Count numbers that contain at most k lucky digits (4 or 7) in their decimal representation.
int countLuckyEnough(const std::vector<long long>& numbers, int k) {
    int result = 0;
    for (long long num : numbers) {
        long long absNum = std::llabs(num); // ignore sign
        int luckyCount = 0;
        if (absNum == 0) {
            luckyCount = 0; // zero has no lucky digits
        } else {
            while (absNum > 0) {
                int digit = absNum % 10;
                if (digit == 4 || digit == 7) {
                    luckyCount++;
                }
                absNum /= 10;
            }
        }
        if (luckyCount <= k) {
            result++;
        }
    }
    return result;
}
// The solution iterates through each integer in the input vector. For each number, we take its absolute value to ignore the sign (using `llabs` for long long to avoid overflow with LLONG_MIN). Then we repeatedly extract the last digit using modulo 10 and check if it is 4 or 7, incrementing a counter. We divide the number by 10 each iteration until it becomes zero. After processing all digits, we compare the counter to `k`; if it's less than or equal to `k`, we increment the result counter. Special handling: for the number 0, the while loop never executes (since 0 is not > 0), so the count remains 0, which is correct. The time complexity is O(n * d), where n is the number of elements and d is the average number of digits (max ~19 for 64-bit integers). The space complexity is O(1) auxiliary, excluding the input vector. No special edge cases other than ensuring we use `long long` for large values and handle negative numbers via absolute value.
