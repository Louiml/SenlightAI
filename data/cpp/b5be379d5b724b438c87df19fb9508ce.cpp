/*
Given an array of `n` positive integers (1 ≤ n ≤ 20, each integer ≤ 10^5), write a C++ function `countSquareSubsequences` that returns the number of non-empty subsequences (not necessarily contiguous) whose product is a perfect square. The result must be returned modulo `100000007`. For example, for array `[2, 2, 4]`, valid subsequences include `[2,2]` (product 4), `[4]` (product 4), and `[2,2,4]` (product 16), giving 3 subsequences. Note that a single element that is itself a perfect square (like 4) is valid. Since the count can be large, return the answer modulo 100000007. The function should not count the empty subsequence.
*/

#include <bits/stdc++.h>

using namespace std;

const int MOD = 100000007;

int countSquareSubsequences(const vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return 0;

    // Collect all distinct prime factors from every number.
    set<int> primeSet;
    auto collectPrimes = [&](int x) {
        int num = x;
        for (int p = 2; p * p <= num; ++p) {
            if (num % p == 0) {
                primeSet.insert(p);
                while (num % p == 0) num /= p;
            }
        }
        if (num > 1) primeSet.insert(num);
    };
    for (int x : arr) collectPrimes(x);

    vector<int> primes(primeSet.begin(), primeSet.end());
    int P = static_cast<int>(primes.size());
    unordered_map<int, int> primeIdx;
    for (int i = 0; i < P; ++i) primeIdx[primes[i]] = i;

    // Build a parity mask for each element.
    vector<bitset<128>> masks(n);
    for (int i = 0; i < n; ++i) {
        int remaining = arr[i];
        bitset<128> mask;
        for (int j = 0; j < P; ++j) {
            int p = primes[j];
            if (remaining % p == 0) {
                int exponent = 0;
                while (remaining % p == 0) {
                    remaining /= p;
                    ++exponent;
                }
                if (exponent % 2 == 1) mask.set(j);
            }
        }
        // remaining should be 1 because all its prime factors are in the set.
        masks[i] = mask;
    }

    long long result = 0;
    int totalSubsets = 1 << n;
    for (int subset = 1; subset < totalSubsets; ++subset) {
        bitset<128> currentMask;
        for (int i = 0; i < n; ++i) {
            if (subset & (1 << i)) {
                currentMask ^= masks[i];
            }
        }
        if (currentMask.none()) {
            result = (result + 1) % MOD;
        }
    }

    return static_cast<int>(result);
}

#include <cassert>
#include <vector>

int countSquareSubsequences(const std::vector<int>& arr);

int main() {
    // Example from the problem statement
    std::vector<int> ex1 = {2, 2, 4};
    assert(countSquareSubsequences(ex1) == 3);

    // All ones: every non-empty subset has product 1 (perfect square)
    std::vector<int> ex2 = {1, 1};
    assert(countSquareSubsequences(ex2) == 3);

    // Single perfect square
    std::vector<int> ex3 = {4};
    assert(countSquareSubsequences(ex3) == 1);

    // Single non-square
    std::vector<int> ex4 = {2};
    assert(countSquareSubsequences(ex4) == 0);

    // Pair that multiplies to a square
    std::vector<int> ex5 = {2, 8};
    assert(countSquareSubsequences(ex5) == 1);

    // Mixed: squares and non-squares
    std::vector<int> ex6 = {1, 2, 4};
    // Subsets: {1}->1, {2}->2, {4}->4, {1,2}->2, {1,4}->4, {2,4}->8, {1,2,4}->8
    // Valid: {1}, {4}, {1,4} -> product 4, {1,2,4}? 8 not square. So 3 valid.
    assert(countSquareSubsequences(ex6) == 3);

    // Larger case: 10 elements all perfect squares
    std::vector<int> ex7(10, 9); // 9 = 3^2, all masks zero
    // 2^10 - 1 = 1023
    assert(countSquareSubsequences(ex7) == 1023);

    // n=20 all ones: should be 2^20 - 1 = 1048575 mod 100000007 = 1048575
    std::vector<int> ex8(20, 1);
    assert(countSquareSubsequences(ex8) == 1048575);

    return 0;
}

// A product of integers is a perfect square if and only if every prime factor appears an even number of times in the product. Since numbers can be up to 10^5 and n up to 20, the product may exceed standard integer types. Instead of tracking the product directly, we represent each number by a bitmask over distinct prime factors: a bit is set to 1 if that prime appears an odd number of times in the number. Then the product of a subsequence is a perfect square exactly when the XOR (bitwise exclusive or) of the masks of all selected numbers equals zero. We first collect all distinct prime factors from all numbers (at most 120 because each number has at most 6 distinct primes). Assign each such prime a bit index. Then for every element compute its mask. Finally, iterate over all non-empty subsets (using an integer bitmask from 1 to 2^n − 1), XOR the masks of included elements, and count those whose XOR is zero. This directly counts valid subsequences. Edge cases include arrays with only 1's (all masks zero, all 2^n − 1 subsequences valid), arrays with a single perfect square (answer 1), and arrays with no valid subsequence (answer 0). The time complexity is O(n · P + n · 2^n), where P is the number of distinct primes (≤ 120). For n ≤ 20 this is at most about 20 million operations, well within limits. Space complexity is O(P + n) for the prime list and element masks.
