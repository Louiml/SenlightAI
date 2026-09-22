// Write a C++ function named `printPalindromicLuckyNumbers` that takes a single integer `digitSum` (where `1 <= digitSum <= 54`) and returns a `std::vector<int>` containing all 5-digit and 6-digit palindromic numbers whose digits sum exactly to `digitSum`. A palindromic number reads the same forwards and backwards. The returned vector must be sorted in ascending order. If no such numbers exist, return an empty vector. The function should not print anything; it should only compute and return the vector.

// The solution iterates separately over all 5-digit numbers (10000 to 99999) and all 6-digit numbers (100000 to 999999). For each number, extract its digits using integer division and modulus operations. For a 5-digit number, the palindrome condition requires the 1st digit equals the 5th, and the 2nd equals the 4th. For a 6-digit number, the condition requires 1st equals 6th, 2nd equals 5th, and 3rd equals 4th. Also compute the sum of all digits and check it equals `digitSum`. If both conditions hold, push the number into the result vector. Since we iterate in increasing order, the vector is automatically sorted. Edge cases: `digitSum` can be too small or too large to produce any valid numbers — the loops handle this naturally. Time complexity is O(900000) ≈ O(1) effectively, as the range is fixed (90,000 five-digit and 900,000 six-digit numbers). Space complexity is O(k) where k is the number of results, but at most a few hundred.

#include <vector>

// Returns all 5-digit and 6-digit palindromic numbers whose digit sum equals digitSum.
// The result is sorted in ascending order.
std::vector<int> printPalindromicLuckyNumbers(const int digitSum) {
    std::vector<int> result;

    // 5-digit numbers: 10000 to 99999
    for (int i = 10000; i <= 99999; ++i) {
        const int w = i / 10000;
        const int q = (i % 10000) / 1000;
        const int b = (i % 1000) / 100;
        const int s = (i % 100) / 10;
        const int g = i % 10;

        if (w + q + b + s + g == digitSum && w == g && q == s) {
            result.push_back(i);
        }
    }

    // 6-digit numbers: 100000 to 999999
    for (int i = 100000; i <= 999999; ++i) {
        const int sw = i / 100000;
        const int w = (i % 100000) / 10000;
        const int q = (i % 10000) / 1000;
        const int b = (i % 1000) / 100;
        const int s = (i % 100) / 10;
        const int g = i % 10;

        if (sw + w + q + b + s + g == digitSum && sw == g && w == s && q == b) {
            result.push_back(i);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // No 5-digit or 6-digit numbers sum to 1.
    assert(printPalindromicLuckyNumbers(1) == std::vector<int>{});

    // Sum = 2: only 10001 (5-digit) and no 6-digit.
    std::vector<int> expected_2 = {10001};
    assert(printPalindromicLuckyNumbers(2) == expected_2);

    // Sum = 3: 10101, 20002 (5-digit); 100001 (6-digit)
    std::vector<int> expected_3 = {100001, 10101, 20002};
    assert(printPalindromicLuckyNumbers(3) == expected_3);

    // Sum = 4: 11011, 20202, 30003 (5-digit); 100001? No, 100001 sum=2, 110011 sum=4, 200002 sum=4
    std::vector<int> expected_4 = {11011, 110011, 200002, 20202, 30003};
    assert(printPalindromicLuckyNumbers(4) == expected_4);

    // Sum = 5: includes 11111, 21012, 31013, 40004, etc.
    assert(printPalindromicLuckyNumbers(5).front() == 101101);
    assert(printPalindromicLuckyNumbers(5).back() == 500005);

    // Sum = 54: only 999999 (6-digit) and no 5-digit.
    std::vector<int> expected_54 = {999999};
    assert(printPalindromicLuckyNumbers(54) == expected_54);

    // Check monotonic order for a larger sum.
    std::vector<int> result = printPalindromicLuckyNumbers(20);
    for (size_t idx = 1; idx < result.size(); ++idx) {
        assert(result[idx - 1] < result[idx]);
    }

    // Sum = 27: sanity check total count is nonzero.
    assert(!printPalindromicLuckyNumbers(27).empty());

    return 0;
}
