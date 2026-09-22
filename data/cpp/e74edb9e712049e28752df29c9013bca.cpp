// Write a C++ function `restoreOriginalNumber` that takes a vector `divisors` of positive integers (the sorted list of all proper divisors of some unknown positive integer `x`, where `x` is not 1 and not prime; note that the list includes every divisor except `1` and `x` itself). Your function must return the original number `x` if a valid `x` exists such that its proper divisor list (excluding `1` and `x`) exactly equals the given sorted vector; otherwise return `-1`. The input vector is guaranteed to be sorted in non-decreasing order and contain at least one element. The original number `x` must be an integer greater than 1, and the proper divisors list must contain all divisors of `x` except `1` and `x`. For example, if `x = 12`, proper divisors (excluding `1` and `12`) are `[2,3,4,6]`. Your solution should use only integer arithmetic (no floating point) and handle large numbers up to 10^12.

The key observation is that if the sorted proper divisors list is valid, then the original number `x` must be the product of the smallest and largest elements in the list. Why? Because the smallest proper divisor (excluding 1) is the smallest prime factor, and the largest proper divisor (excluding `x`) is `x` divided by that same smallest factor, so their product is exactly `x`. So we first compute `candidate = divisors[0] * divisors.back()`. Then we generate the full set of all divisors of `candidate` (including 1 and `candidate`). From that full set, we remove `1` and `candidate` itself to get the expected proper divisor list. We then compare this expected list with the input list. If they are exactly equal (same size and same elements, both sorted), then `candidate` is valid; otherwise, return -1. Important edge cases: the original number must be non-prime and greater than 1, but if the input vector has only one element, the smallest prime factor would be that element and the candidate would be its square (since largest would equal that same element, because the list is symmetric? actually if n is a perfect square of a prime, e.g., 9 -> divisors [3], then product = 3*3=9, and full divisors {1,3,9} → proper list {3} matches, so valid). Also careful with duplicates: if a divisor repeats (which cannot happen in a true divisor set), the comparison will fail. Also, if candidate is 1 or prime, the proper divisor list would be empty, but our input has at least one element, so that can't happen. Time complexity: generating divisors of `candidate` by trial division up to sqrt(candidate) takes O(sqrt(candidate)), which for up to 10^12 is fine (max 10^6 iterations). Sorting is already given, and we only sort the generated list if needed (we can generate in ascending order by push then sort, or use a method to produce sorted). Space is O(number of divisors). We must ensure no overflow: use `long long` for multiplication.

#include <vector>
#include <algorithm>
#include <cmath>

// Given the sorted list of all proper divisors (excluding 1 and the number itself),
// determine the original number if it exists, otherwise return -1.
long long restoreOriginalNumber(const std::vector<long long>& divisors) {
    if (divisors.empty()) {
        return -1; // Not enough information
    }

    // Candidate original number = smallest * largest divisor
    long long candidate = divisors.front() * divisors.back();

    // Generate all divisors of candidate
    std::vector<long long> allDivisors;
    for (long long i = 1; i * i <= candidate; ++i) {
        if (candidate % i == 0) {
            allDivisors.push_back(i);
            if (i != candidate / i) {
                allDivisors.push_back(candidate / i);
            }
        }
    }
    std::sort(allDivisors.begin(), allDivisors.end());

    // Build the expected proper divisor list: all divisors except 1 and candidate itself
    std::vector<long long> expected;
    for (long long d : allDivisors) {
        if (d != 1 && d != candidate) {
            expected.push_back(d);
        }
    }

    // Compare input with expected
    if (divisors == expected) {
        return candidate;
    }
    return -1;
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is assumed to be declared above

int main() {
    // Case 1: 12 -> proper divisors excluding 1 and 12: [2,3,4,6]
    assert(restoreOriginalNumber({2,3,4,6}) == 12);

    // Case 2: 9 -> proper divisors: [3]
    assert(restoreOriginalNumber({3}) == 9);

    // Case 3: 16 -> proper divisors: [2,4,8]
    assert(restoreOriginalNumber({2,4,8}) == 16);

    // Case 4: Invalid list (missing a divisor)
    assert(restoreOriginalNumber({2,4,6}) == -1);

    // Case 5: 6 -> proper divisors: [2,3]
    assert(restoreOriginalNumber({2,3}) == 6);

    // Case 6: 28 -> proper divisors: [2,4,7,14]
    assert(restoreOriginalNumber({2,4,7,14}) == 28);

    // Case 7: Large number 100000000000? Actually 10^12 is 1,000,000,000,000
    // 10^12 = 2^12 * 5^12? Let's use a simpler large: 1234567890.
    // 1234567890 proper divisors excluding 1 and itself are many; test with a known.
    // We'll test a perfect square of a prime: 101^2 = 10201, proper divisors [101]
    assert(restoreOriginalNumber({101}) == 10201);

    // Case 8: Invalid because candidate would be prime? Actually if input is {2}, product = 4, but 4 has proper divisor [2], so valid.
    // Test a genuine invalid: {2,3,5} - product 10, proper divisors of 10 are [2,5], not matching.
    assert(restoreOriginalNumber({2,3,5}) == -1);

    // Case 9: Single element that is not a perfect square of prime? e.g., {4} -> product 16, proper divisors [2,8] not matching.
    assert(restoreOriginalNumber({4}) == -1);

    // Case 10: Empty vector (though spec says at least one, test robustness)
    assert(restoreOriginalNumber({}) == -1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
