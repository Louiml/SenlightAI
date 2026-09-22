/*
Write a C++ function named `computeDoublingPay` that takes an integer parameter `numDays` (assumed to be between 1 and 30, inclusive) and returns a `std::pair<unsigned long long, unsigned long long>` where the first element is the salary on the final day (in pennies) and the second element is the total pay accumulated over all days (in pennies). The function must simulate the following: starting with a salary of 1 penny on day 1, each subsequent day the salary doubles (i.e., multiplied by 2), and the total pay is the sum of daily salaries from day 1 through the final day. The function should not print anything to the console; it should only compute and return the result. The returned numbers may be large (for 30 days, the final day salary is 1<<29 = 536,870,912 pennies, and the total is (1<<30)-1 = 1,073,741,823 pennies, so use `unsigned long long` to safely handle all values). You must ensure the function is correct for edge cases like `numDays = 1` (salary 1, total 1) and `numDays = 30`.
*/

#include <utility>

// Returns a pair where first = final daily salary (in pennies), second = total pay (in pennies)
// for a doubling salary starting at 1 penny on day 1.
// The parameter numDays must be between 1 and 30 inclusive.
std::pair<unsigned long long, unsigned long long> computeDoublingPay(int numDays) {
    unsigned long long salary = 1;   // day 1 salary in pennies
    unsigned long long totalPay = salary; // total pay after day 1

    for (int day = 2; day <= numDays; ++day) {
        salary *= 2;                 // salary doubles each day
        totalPay += salary;          // add today's salary to total
    }

    return {salary, totalPay};
}

#include <cassert>

int main() {
    // Test for 1 day: salary = 1, total = 1
    auto result1 = computeDoublingPay(1);
    assert(result1.first == 1ull);
    assert(result1.second == 1ull);

    // Test for 3 days: salaries 1,2,4 => total 7, final salary 4
    auto result3 = computeDoublingPay(3);
    assert(result3.first == 4ull);
    assert(result3.second == 7ull);

    // Test for 5 days: salaries 1,2,4,8,16 => total 31, final salary 16
    auto result5 = computeDoublingPay(5);
    assert(result5.first == 16ull);
    assert(result5.second == 31ull);

    // Test for 10 days: final salary = 1<<9 = 512, total = (1<<10)-1 = 1023
    auto result10 = computeDoublingPay(10);
    assert(result10.first == 512ull);
    assert(result10.second == 1023ull);

    // Test for 30 days (maximum): final salary = 1<<29 = 536870912, total = (1<<30)-1 = 1073741823
    auto result30 = computeDoublingPay(30);
    assert(result30.first == 536870912ull);
    assert(result30.second == 1073741823ull);

    // Test for 2 days: salaries 1,2 => total 3, final salary 2
    auto result2 = computeDoublingPay(2);
    assert(result2.first == 2ull);
    assert(result2.second == 3ull);

    return 0;
}

// The solution approach uses a simple loop to simulate the doubling. Initialize `salary = 1` and `totalPay = 1` for day 1. Then for each subsequent day from 2 to `numDays`, double the salary (either by `salary *= 2` or `salary <<= 1`, but using multiplication is clearer) and add it to `totalPay`. This is a linear simulation with exactly `numDays - 1` iterations, so time complexity is O(n) where n = numDays (but n is at most 30, so constant time in practice). Space complexity is O(1) as only a few local variables are used. Edge cases: `numDays = 1` should not enter the loop and return (1,1). `numDays = 0` is not expected per the problem specification, but we could handle it defensively by treating it as 1 or returning (0,0); however, the task specifies valid input in [1,30], so we assume valid input. Since the maximum value fits in `unsigned long long` (about 1.07 billion), there is no overflow risk.
