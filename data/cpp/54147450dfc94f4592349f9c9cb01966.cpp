/*
Write a C++ function named `calculateScore` that takes a single integer `n` (where 1 ≤ n ≤ 100) and returns an integer representing the maximum possible score in a "multiplication square" game. The game works as follows: you are given a target value `n`, and you must choose a two-digit number (from 10 to 99) such that when you divide `n` by its first digit (1-9), the result is exactly equal to its second digit (0-9). For each valid two-digit divisor `d` that satisfies this condition, you earn `n` points (so if there are `k` valid divisors, you earn `k * n` points). The score is calculated as `2025 - (k * n)`, where `k` is the count of valid two-digit divisors. If no such divisor exists, return 2025. The function must handle edge cases like `n` being prime, single-digit values, and values divisible only by 1 or itself.
*/

#include <cstdint>

// Count valid two-digit divisors of n where n / first_digit == second_digit.
// Returns 2025 - (count * n).
int calculateScore(int n) {
    if (n <= 0) {
        return 2025; // Safety for invalid input
    }
    
    int count = 0;
    for (int first = 1; first <= 9; ++first) {
        if (n % first == 0) {
            int second = n / first;
            if (second >= 0 && second <= 9) {
                // The two-digit number is first*10 + second, which is always >=10
                ++count;
            }
        }
    }
    
    return 2025 - (count * n);
}

#include <cassert>

int main() {
    assert(calculateScore(1) == 2024);   // Valid: 11 (1/1=1), count=1, score=2025-1
    assert(calculateScore(2) == 2021);   // Valid: 21 (2/1=2) and 12 (2/2=1), count=2, score=2025-4
    assert(calculateScore(9) == 1998);   // Valid: 91, 93, 99 (9/1=9,9/3=3,9/9=1), count=3, score=2025-27
    assert(calculateScore(10) == 2015);  // Valid: 51? 10/1=10 not valid; 10/2=5 -> 25, 10/5=2 -> 52, 10/10=1 not valid (first=10 invalid), count=2, score=2025-20
    assert(calculateScore(11) == 2014);  // Valid: 11? 11/1=11 no; 11/11=1 no (first=11 invalid), none, score=2025
    assert(calculateScore(100) == 2025); // Only first digits dividing 100: 1,2,4,5; quotients 100,50,25,20 all >9, count=0
    assert(calculateScore(45) == 1980);  // Valid: 91? 45/1=45 no; 45/3=15 no; 45/5=9 -> 59, 45/9=5 -> 95, count=2, score=2025-90
    return 0;
}

// The solution requires counting how many two-digit numbers `d` (from 10 to 99) divide `n` exactly and also satisfy the digit constraint: `n / first_digit == second_digit`. The digit constraint can be checked by iterating over possible first digits `i` from 1 to 9 (as the first digit cannot be 0) and verifying that `n` is divisible by `i` and that the quotient `n / i` is a valid second digit (0-9). If both conditions hold, then `d = 10*i + (n/i)` is a valid two-digit divisor, and we increment the count. Note that `n/i` could be 0, which would make `d` a single-digit number (e.g., `i=5, n/i=0` gives `d=50`, which is still two-digit since the tens digit is 5; however, the second digit is 0, which is allowed). The algorithm iterates `i` from 1 to 9, so at most 9 checks. Edge cases: if `n` is 0 or negative (though constraints say positive), the modulo operation could cause division by zero, so we assume positive input. If `n` is 1, then only `i=1` gives `n/i=1`, so `d=11` is valid, giving `k=1`, score = 2025 - 1*1 = 2024. Time complexity is O(1) since at most 9 iterations, and space complexity is O(1).
