/*
Given a list of non-negative integers, write a C++ function that selects a subset of these numbers according to specific digit-counting and divisibility rules and returns the selected numbers in the order they were chosen. Count the number of zeros in each integer’s decimal representation (treating the number 0 as having exactly one zero). Then:
- Among numbers with exactly two zeros, pick the smallest one (if any) and add it to the result first.
- Then, iterate through all numbers with exactly one zero in their original input order; add a number to the result if its value has at most one digit (i.e., 0–9) or if the current sum is a multiple of 100 (including the starting sum of 0). Specifically, a number can be added if either it is a single-digit number, or the current accumulated sum is divisible by 100 (the sum being 0 counts as divisible by 100). If neither condition holds, skip that number.
- Finally, iterate through all numbers with zero zeros in their decimal representation in original input order; add a number to the result if it has at most one digit (0–9) and the current sum is a multiple of 10, OR if it has at most two digits and both the current sum is a multiple of 10 and the current sum is a multiple of 100. Note that the number 0 falls into the zero-zero category because it has no zeros in its decimal representation (since the counting of zeros treats 0 as having no zeros for this category – this is an important distinction: the rule for “zero zeros” counts actual digit zeros, so the number 0 has zero zeros). Skip numbers that do not satisfy the condition.

Return the count of selected numbers and the list itself, in the order they were chosen. The function should handle an input vector of size up to 100 and each number up to 1,000,000,000. If no number is selected, return an empty list.
*/

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>

// Count the number of zero digits in the decimal representation of a non-negative integer.
// For the number 0, this returns 0 because there are no digit positions.
int countZeroDigits(uint64_t x) {
    if (x == 0) return 0;
    int count = 0;
    while (x > 0) {
        if (x % 10 == 0) count++;
        x /= 10;
    }
    return count;
}

// Return a pair: first = number of selected items, second = the selected items in order.
// Input: a vector of non-negative integers.
std::pair<int, std::vector<int>> selectNumbers(const std::vector<int>& numbers) {
    std::vector<int> oneZero, twoZero, zeroZero;
    
    // Categorize numbers.
    for (int x : numbers) {
        if (x == 0) {
            oneZero.push_back(0);  // 0 is treated as having exactly one zero for the one-zero group.
            continue;
        }
        int cnt = countZeroDigits(static_cast<uint64_t>(x));
        if (cnt == 1) oneZero.push_back(x);
        else if (cnt == 2) twoZero.push_back(x);
        else if (cnt == 0) zeroZero.push_back(x);
        // Numbers with more than 2 zeros are ignored.
    }
    
    std::vector<int> ans;
    long long sum = 0;
    
    // Process twoZero group: pick the smallest one.
    if (!twoZero.empty()) {
        std::sort(twoZero.begin(), twoZero.end());
        ans.push_back(twoZero[0]);
        sum += twoZero[0];
    }
    
    // Process oneZero group in original order.
    for (int x : oneZero) {
        // Condition: either single-digit, or current sum divisible by 100 (including sum==0).
        bool singleDigit = (x >= 0 && x <= 9);
        bool sumDivisibleBy100 = (sum == 0 || sum % 100 == 0);
        if (singleDigit || sumDivisibleBy100) {
            ans.push_back(x);
            sum += x;
        }
    }
    
    // Process zeroZero group in original order.
    for (int x : zeroZero) {
        int digits = (x == 0) ? 0 : static_cast<int>(log10(static_cast<double>(x))) + 1;
        // First condition: at most one digit (0-9) and sum divisible by 10.
        if (digits <= 1 && (sum == 0 || sum % 10 == 0)) {
            ans.push_back(x);
            sum += x;
            continue;
        }
        // Second condition: at most two digits and sum divisible by both 10 and 100.
        if (digits <= 2 && (sum == 0 || (sum % 10 == 0 && sum % 100 == 0))) {
            ans.push_back(x);
            sum += x;
        }
    }
    
    return {static_cast<int>(ans.size()), ans};
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is included above; here we test it.

int main() {
    // Test basic case.
    {
        std::vector<int> input = {0, 10, 100, 5, 200};
        auto result = selectNumbers(input);
        assert(result.first == 3);
        std::vector<int> expected = {100, 0, 5};
        assert(result.second == expected);
    }
    // Test with no selected numbers.
    {
        std::vector<int> input = {11, 22, 33};
        auto result = selectNumbers(input);
        assert(result.first == 0);
        assert(result.second.empty());
    }
    // Test with only zeros.
    {
        std::vector<int> input = {0, 0};
        auto result = selectNumbers(input);
        assert(result.first == 2);
        std::vector<int> expected = {0, 0};
        assert(result.second == expected);
    }
    // Test two-zero group with multiple candidates.
    {
        std::vector<int> input = {1000, 100, 200, 10};
        auto result = selectNumbers(input);
        // twoZero = {100, 200} -> pick 100. Then oneZero = {10} -> since sum=100 is divisible by 100, add 10.
        // zeroZero = {1000} -> has 3 zeros, ignored.
        assert(result.first == 2);
        std::vector<int> expected = {100, 10};
        assert(result.second == expected);
    }
    // Test zeroZero condition with two-digit numbers.
    {
        std::vector<int> input = {100, 11, 12};
        // twoZero = {100} -> pick 100, sum=100. oneZero empty. zeroZero = {11,12} both two-digit.
        // For 11: digits=2, sum=100 divisible by both 10 and 100 → add.
        // For 12: digits=2, sum=111 not divisible by 10 → skip.
        auto result = selectNumbers(input);
        assert(result.first == 2);
        std::vector<int> expected = {100, 11};
        assert(result.second == expected);
    }
    // Test large numbers and overflow safety.
    {
        std::vector<int> input = {100000000, 5, 100, 0};
        auto result = selectNumbers(input);
        // twoZero = {100000000}? count zeros 8, so not in twoZero. Actually 100000000 has 8 zeros → ignored.
        // oneZero = {0} → added (sum=0). zeroZero = {5} → single digit, sum=0 divisible by 10 → add.
        assert(result.first == 2);
        std::vector<int> expected = {0, 5};
        assert(result.second == expected);
    }
    // Test that the original order is preserved for oneZero and zeroZero groups.
    {
        std::vector<int> input = {10, 1, 20, 2, 100};
        // twoZero = {100} → pick 100, sum=100.
        // oneZero = {10,1,20,2} in original order. For 10: sum=100 divisible by 100 → add, sum=110. For 1: single digit → add, sum=111. For 20: sum=111 not divisible by 100 and not single digit → skip. For 2: single digit → add.
        auto result = selectNumbers(input);
        std::vector<int> expected = {100, 10, 1, 2};
        assert(result.first == 4);
        assert(result.second == expected);
    }
    // Test empty input.
    {
        std::vector<int> input;
        auto result = selectNumbers(input);
        assert(result.first == 0);
        assert(result.second.empty());
    }
    std::cout << "All tests passed.\n";
}

// The key challenge is to correctly count the number of zero digits in each number. For the number 0 itself, you must be careful: the problem treats 0 as having exactly one zero for the `a[1]` group (since the original code explicitly pushes 0 into `a[1]`), but for the zero-zero group, the function must count actual digit zeros, so 0 has zero zeros. So we need two different counting approaches: one for categorizing into groups (where 0 belongs to the one-zero group) and one for checking the zero-zero group (where 0 has zero zeros). The algorithm proceeds by scanning the input once, categorizing numbers into three vectors: `oneZero` (numbers with exactly one digit zero, including 0), `twoZero` (numbers with exactly two digit zeros), and `zeroZero` (numbers with zero digit zeros, excluding 0 itself because 0 goes to `oneZero`). Then sort `twoZero` and take its minimum if non-empty. Then iterate through `oneZero` in original order, applying the condition: if the number is a single-digit (0–9) OR the current sum is divisible by 100 (including sum==0), add it and update sum. Then iterate through `zeroZero` in original order, but note the original code has a subtle bug: the first condition checks `log10(a[0][i]) <= 0` (i.e., single-digit, including 0) and `sum%10==0`; the second condition checks `log10(a[0][i]) <= 1` (i.e., two-digit) and both `sum%10==0` and `sum%100==0`. We must replicate this logic exactly. The time complexity is O(n log n) due to sorting `twoZero` (at most n elements), and space complexity O(n) for the output vectors. Edge cases: numbers with more than two zeros are ignored entirely; the sum can grow large, so use `long long` for sum to avoid overflow; the number 0 appears in `oneZero` and is a single-digit, so it will be added whenever sum is 0 or divisible by 100 (which is always true for sum=0). Also, the original code has commented-out lines that are not part of the logic, so we ignore them.
