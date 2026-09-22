/*
Write a C++ function `int smallest_power_of_three_at_least(const std::string& decimal_string)` that takes a non-negative integer represented as a decimal string (possibly with leading zeros, but at least one digit) and returns the smallest integer `k >= 0` such that `3^k >= N`, where `N` is the numeric value of the input string. The result must be correct for arbitrarily large inputs (e.g., up to thousands of digits), so you cannot use built‑in big integers. You must implement multiplication and comparison of big numbers yourself using a base‑10 000 representation, and you may use an iterative or recursive fast exponentiation approach to test candidate powers of 3. The function should return 0 for input "0" (since `3^0 = 1 >= 0`). Edge cases include very large inputs, inputs exactly equal to powers of 3, and inputs like "1" (answer 0) or "2" (answer 1).
*/
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using BigInt = std::vector<long long>; // base 10000, least significant first

// Remove leading zeros and ensure at least one digit.
static void normalize(BigInt& a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
}

// Multiply big integer by a small integer (e.g., 3).
static void mul_small(BigInt& a, long long v) {
    long long carry = 0;
    for (size_t i = 0; i < a.size() || carry; ++i) {
        if (i == a.size()) a.push_back(0);
        long long cur = a[i] * v + carry;
        a[i] = cur % 10000;
        carry = cur / 10000;
    }
    normalize(a);
}

// Square a big integer using simple O(m^2) convolution and then normalize.
static void square(BigInt& a) {
    size_t n = a.size();
    BigInt res(2 * n, 0);
    for (size_t i = 0; i < n; ++i) {
        long long carry = 0;
        for (size_t j = 0; j < n; ++j) {
            long long cur = res[i + j] + a[i] * a[j] + carry;
            res[i + j] = cur % 10000;
            carry = cur / 10000;
        }
        size_t idx = i + n;
        while (carry) {
            long long cur = res[idx] + carry;
            res[idx] = cur % 10000;
            carry = cur / 10000;
            ++idx;
        }
    }
    while (res.size() > 1 && res.back() == 0) res.pop_back();
    a = res;
}

// Compute 3^e as a big integer.
static BigInt power_of_three(int e) {
    BigInt result = {1};
    BigInt base = {3};
    while (e > 0) {
        if (e & 1) {
            // multiply result by base (base is small initially, but becomes large)
            BigInt new_res(result.size() + base.size(), 0);
            for (size_t i = 0; i < result.size(); ++i) {
                long long carry = 0;
                for (size_t j = 0; j < base.size(); ++j) {
                    long long cur = new_res[i + j] + result[i] * base[j] + carry;
                    new_res[i + j] = cur % 10000;
                    carry = cur / 10000;
                }
                size_t idx = i + base.size();
                while (carry) {
                    long long cur = new_res[idx] + carry;
                    new_res[idx] = cur % 10000;
                    carry = cur / 10000;
                    ++idx;
                }
            }
            while (new_res.size() > 1 && new_res.back() == 0) new_res.pop_back();
            result = new_res;
        }
        square(base);
        e >>= 1;
    }
    return result;
}

// Compare two big integers: -1 if a<b, 0 if equal, 1 if a>b.
static int compare_big(const BigInt& a, const BigInt& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (int i = (int)a.size() - 1; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

// Convert decimal string to big integer (base 10000).
static BigInt from_string(const std::string& s) {
    BigInt res;
    int n = (int)s.size();
    int begin = 0;
    while (begin < n && s[begin] == '0') ++begin;
    if (begin == n) {
        res.push_back(0);
        return res;
    }
    std::string digits = s.substr(begin);
    int len = (int)digits.size();
    for (int i = len; i > 0; i -= 4) {
        int start = std::max(0, i - 4);
        std::string chunk = digits.substr(start, i - start);
        res.push_back(std::stoll(chunk));
    }
    return res;
}

// Main function: return smallest k >= 0 such that 3^k >= N.
int smallest_power_of_three_at_least(const std::string& decimal_string) {
    if (decimal_string.empty()) return 0;
    BigInt target = from_string(decimal_string);
    if (target.size() == 1 && target[0] == 0) return 0;

    // Estimate exponent using log10(3) ≈ 0.47712125471966244.
    int digits = (int)decimal_string.size();
    int estimate = (int)(digits / 0.47712125471966244) - 2;
    if (estimate < 0) estimate = 0;

    // Compute 3^estimate.
    BigInt cur = power_of_three(estimate);
    int k = estimate;
    // Increment until cur >= target.
    while (compare_big(cur, target) < 0) {
        mul_small(cur, 3); // multiply by 3 => exponent +1
        ++k;
    }
    return k;
}
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.

int main() {
    assert(smallest_power_of_three_at_least("0") == 0);
    assert(smallest_power_of_three_at_least("1") == 0);
    assert(smallest_power_of_three_at_least("2") == 1);
    assert(smallest_power_of_three_at_least("3") == 1);
    assert(smallest_power_of_three_at_least("4") == 2);
    assert(smallest_power_of_three_at_least("9") == 2);
    assert(smallest_power_of_three_at_least("10") == 3);
    assert(smallest_power_of_three_at_least("27") == 3);
    assert(smallest_power_of_three_at_least("28") == 4);
    assert(smallest_power_of_three_at_least("000123") == 5); // 3^5 = 243 >= 123
    return 0;
}
// We need to find the smallest exponent `k` such that `3^k >= N`. Directly computing `3^k` for huge `k` is infeasible with standard integers, so we represent numbers as vectors of base‑10 000 digits (least significant digit first). We can find a good starting estimate: since `log10(3^k) = k * log10(3)`, the number of digits of `N` is roughly `n = len(digits)`. Thus `k ≈ n / log10(3)`. We compute a starting exponent `start = max(0, floor(n / log10(3)) - 2)` and then test candidates around it. However, because we need the exact smallest exponent, we can simply iterate upward from `start` until we find a power that is `>= N`. To avoid recomputing from scratch each time, we compute `3^start` once, then repeatedly multiply by 3 (incrementing exponent) and compare. For each candidate `k`, we compare the big number for `3^k` against the big number for `N`. Since we start slightly below the true answer, the loop runs at most a few iterations (bounded by the margin from the estimate). For safety, we can also start from `max(0, n / 3 - 3)` but the log‑base formula is better. We use `long long` for the base and carry. We implement `mul_small` to multiply by a small integer (like 3), `normalize` to propagate carries and remove leading zeros, `compare` to compare two vectors, and a `power_of_three` function that builds `3^e` by repeated squaring (squaring uses a simple O(m^2) convolution, which is fine since the numbers have at most thousands of digits; we avoid FFT for simplicity). The time complexity is O((D^2) log k + M * D) where D is the number of base‑10 000 digits of the final power, k is the answer, and M is the small number of extra multiplications from the starting guess. Space is O(D). Leading zeros in the input are handled by stripping them before converting.
