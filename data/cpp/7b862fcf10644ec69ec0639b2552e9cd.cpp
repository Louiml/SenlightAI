// You are given a sequence of daily net gains (positive for profit, negative for loss) for a store over all days except one missing day. The total sum of gains over the full period is known to be balanced, meaning the absolute total positive gain equals the absolute total negative loss. Write a C++ function `long long missingBalanceValue(int n, const std::vector<long long>& gains)` that takes the total number of days `n` and a vector of exactly `n-1` observed gains (each may be positive, negative, or zero), and returns the missing day’s gain that would make the full period’s absolute positive sum equal to the absolute negative sum. Specifically, if the sum of absolute positive observed gains is greater than the sum of absolute observed losses, the missing value must be a negative number equal to the difference (so the answer is the negative of that difference); otherwise, the missing value is the positive difference. If the two sums are equal, the missing gain is zero. The function must handle up to 10^6 values with magnitudes up to 10^9, so use `long long` for sums and differences.
// The core idea is to compute the total absolute profit and total absolute loss from the given `n-1` values. Let `posSum` be the sum of all positive numbers, and `negSum` be the sum of absolute values of all negative numbers (ignoring zeroes). To balance the full set, the missing value `x` must satisfy `abs(posSum + max(x,0)) == abs(negSum + abs(min(x,0)))`. Since only one unknown value is missing, the simplest approach is to compare `posSum` and `negSum`. If `posSum > negSum`, the missing value must be a negative number that reduces the positive side or increases the negative side; mathematically, the only possibility is `x = -(posSum - negSum)`. If `negSum > posSum`, then `x = negSum - posSum`. If equal, `x = 0`. This works because adding a single value can only adjust the difference by that value (with sign), and the balance condition reduces to `abs(posSum + max(0,x)) == abs(negSum + abs(min(0,x)))`. For `posSum > negSum`, setting `x = -(posSum - negSum)` makes both sides equal: positive side becomes `posSum + 0 = posSum`, negative side becomes `negSum + (posSum - negSum) = posSum`. Similarly for the other case. Edge cases: all values positive/negative/zero, single observed value (n=2), large sums up to 10^15 (fits in `long long`), and the handle of zero values which do not affect sums. Time complexity is O(n) to sum the vector, space O(1) beyond input storage.
#include <vector>
#include <cstdlib>

// Returns the missing daily gain that balances absolute positive and negative sums.
long long missingBalanceValue(int n, const std::vector<long long>& gains) {
    long long posSum = 0;
    long long negSum = 0;
    // Sum absolute positive and absolute negative values.
    for (long long val : gains) {
        if (val > 0) {
            posSum += val;
        } else if (val < 0) {
            negSum += (-val);
        }
    }
    // The missing value must offset the difference.
    if (posSum > negSum) {
        return -(posSum - negSum);
    } else if (negSum > posSum) {
        return negSum - posSum;
    }
    return 0;
}
#include <cassert>
#include <vector>

// Declare the function (already defined above in a separate file).
long long missingBalanceValue(int n, const std::vector<long long>& gains);

int main() {
    // Case 1: observed gains sum to positive > negative, need negative missing.
    std::vector<long long> v1 = {10, -5, 3}; // pos=13, neg=5, diff=8 => missing -8
    assert(missingBalanceValue(4, v1) == -8);

    // Case 2: negative sum > positive, need positive missing.
    std::vector<long long> v2 = {-2, -3, 4}; // pos=4, neg=5, diff=1 => missing +1
    assert(missingBalanceValue(4, v2) == 1);

    // Case 3: balanced already, missing zero.
    std::vector<long long> v3 = {5, -5}; // pos=5, neg=5
    assert(missingBalanceValue(3, v3) == 0);

    // Case 4: all positive.
    std::vector<long long> v4 = {2, 3, 4}; // pos=9, neg=0 => missing -9
    assert(missingBalanceValue(4, v4) == -9);

    // Case 5: all negative.
    std::vector<long long> v5 = {-10, -2, -1}; // pos=0, neg=13 => missing +13
    assert(missingBalanceValue(4, v5) == 13);

    // Case 6: single value, n=2, value positive.
    std::vector<long long> v6 = {7}; // pos=7, neg=0 => missing -7
    assert(missingBalanceValue(2, v6) == -7);

    // Case 7: single value, n=2, value negative.
    std::vector<long long> v7 = {-7}; // pos=0, neg=7 => missing +7
    assert(missingBalanceValue(2, v7) == 7);

    // Case 8: contains zero and large values.
    std::vector<long long> v8 = {0, 1000000000LL, -500000000LL}; // pos=1e9, neg=5e8 => diff=5e8
    assert(missingBalanceValue(4, v8) == -500000000LL);

    // Case 9: large sums, check overflow safety (using long long).
    std::vector<long long> v9 = {1000000000LL, 1000000000LL, -1000000000LL}; // pos=2e9, neg=1e9 => missing -1e9
    assert(missingBalanceValue(4, v9) == -1000000000LL);

    // Case 10: multiple zeros and balanced.
    std::vector<long long> v10 = {0, 0, 0}; // pos=0, neg=0 => missing 0
    assert(missingBalanceValue(4, v10) == 0);

    return 0;
}
