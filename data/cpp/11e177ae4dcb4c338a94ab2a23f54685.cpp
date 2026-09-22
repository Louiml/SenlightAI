/*
Write a C++ function named `printArmstrongUpTo` that takes a positive integer `n` as input and returns a `std::vector<int>` containing all Armstrong numbers (also called narcissistic numbers) from `1` to `n` inclusive, in ascending order. An Armstrong number of `k` digits is a number such that the sum of its digits each raised to the power `k` equals the number itself. For example, `153` is an Armstrong number because `1^3 + 5^3 + 3^3 = 153`. The function must handle `n` up to `1,000,000` efficiently. If `n` is less than `1`, return an empty vector. Do not use floating-point `pow` to avoid rounding errors; instead use integer exponentiation.
*/

#include <vector>

// Compute base^exp using integer arithmetic only.
int intPower(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

// Return all Armstrong numbers from 1 to n inclusive, in ascending order.
std::vector<int> printArmstrongUpTo(int n) {
    std::vector<int> result;
    if (n < 1) {
        return result;
    }
    for (int candidate = 1; candidate <= n; ++candidate) {
        int number = candidate;
        int digitCount = 0;
        while (number != 0) {
            ++digitCount;
            number /= 10;
        }
        int sum = 0;
        number = candidate;
        while (number != 0) {
            int digit = number % 10;
            sum += intPower(digit, digitCount);
            number /= 10;
        }
        if (sum == candidate) {
            result.push_back(candidate);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function declaration is assumed to be available.
std::vector<int> printArmstrongUpTo(int n);

int main() {
    // Armstrong numbers below 200: 1, 153
    std::vector<int> result1 = printArmstrongUpTo(200);
    std::vector<int> expected1 = {1, 153};
    assert(result1 == expected1);

    // Up to 1000: add 370, 371, 407
    std::vector<int> result2 = printArmstrongUpTo(1000);
    std::vector<int> expected2 = {1, 153, 370, 371, 407};
    assert(result2 == expected2);

    // n = 1 includes just 1
    assert(printArmstrongUpTo(1) == std::vector<int>({1}));

    // n = 0 returns empty
    assert(printArmstrongUpTo(0).empty());

    // Negative n returns empty
    assert(printArmstrongUpTo(-5).empty());

    // Large n: includes 1634 and 8208
    std::vector<int> result3 = printArmstrongUpTo(10000);
    std::vector<int> expected3 = {1, 153, 370, 371, 407, 1634, 8208, 9474};
    assert(result3 == expected3);

    // Check that 153 is present and 100 is not
    std::vector<int> result4 = printArmstrongUpTo(153);
    assert(result4.back() == 153);
    bool contains100 = false;
    for (int v : result4) {
        if (v == 100) contains100 = true;
    }
    assert(!contains100);

    // Check ascending order
    std::vector<int> result5 = printArmstrongUpTo(10000);
    for (size_t i = 1; i < result5.size(); ++i) {
        assert(result5[i - 1] < result5[i]);
    }

    return 0;
}

// The solution processes each integer from `1` to `n` individually. For each candidate number `x`, we first count its number of digits `k` by repeatedly dividing by 10. Then we compute the sum of each digit raised to the power `k`. To avoid floating-point inaccuracies from `pow(rem, digitCount)` (which can produce off-by-one errors for large powers), we implement a helper function `intPower(base, exp)` that performs integer exponentiation via a simple loop (or fast exponentiation by squaring). We then compare the computed sum to the original number; if equal, we append it to the result vector. Edge cases: `n` less than `1` returns an empty vector; numbers like `0` are not included because the loop starts at `1`. The algorithm runs in `O(n * d)` time where `d` is the maximum digit count (at most 7 for `n` ≤ 1,000,000), so it is effectively `O(n)` per practical purposes. Space complexity is `O(m)` where `m` is the number of Armstrong numbers found, plus `O(1)` auxiliary space for the loop and helper functions.
