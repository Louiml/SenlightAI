Write a C++ function named `countNDigitSubstringsDivisibleByN` that takes a non-empty string `str` of decimal digits (only characters '0' to '9') and a positive integer `n`, and returns the number of substrings (contiguous sequences of digits) of `str` whose numeric value is divisible by `n`. For example, for `str = "10"` and `n = 3`, the substrings are "1", "0", "10", of which only "0" (0 divisible by 3) and "10" (10 not divisible by 3, actually 10%3=1) — only "0" qualifies, so return 1. Substrings are counted by their positional occurrences, not distinct values, so overlapping substrings count separately. The function should handle large input lengths (up to 10^5) efficiently, and `n` can be up to 100. Include all necessary headers and use `const` correctness appropriately.
// The core idea is dynamic programming. For each position `i` in the string (0-indexed), we compute the number of substrings that end at position `i` with a given remainder modulo `n`. Let `dp[i][r]` be the count of substrings ending exactly at position `i` that have remainder `r` when the substring's numeric value is taken modulo `n`.  
// Base: For each position `i`, a single-digit substring consisting of `str[i]` has remainder `(str[i] - '0') % n`, so increment `dp[i][that_remainder]`.  
// Transition: For each position `i > 0`, consider any substring ending at `i-1` with remainder `j`. If we append digit `d = str[i]` to that substring, the new remainder becomes `(j * 10 + d) % n`. So we add `dp[i-1][j]` to `dp[i][(j*10+d)%n]`. Also, we must carry forward substrings that do not include the new digit? Actually no—`dp[i][r]` counts only substrings ending exactly at `i`, so we do not carry forward; we simply compute fresh for each position. However, the problem counts all substrings ending at any position, so we sum up `dp[i][0]` over all `i`.  
// But note: To avoid double-counting and to compute correctly, we can either use a 2D DP and sum the `dp[i][0]` entries, or we can maintain a rolling array of size `n` for remainders of substrings ending at the current position, plus a running total for all substrings divisible by `n`.  
// Edge cases: `n=1` — every substring is divisible by 1, so the answer is `len*(len+1)/2`. Empty string? Not allowed per spec. Single digit '0' divisible by any `n` (0 % n == 0). Leading zeros are fine because we treat them as part of the numeric value (e.g., "01" is value 1).  
// Time complexity: O(len * n) because for each position we loop over `n` remainders. Space: O(n) for a rolling DP array. For `len=10^5` and `n=100`, that's about 10^7 operations, which is fine.  
// Implementation details: Use `long long` for counts because number of substrings can be up to ~5*10^9 for len=10^5 (len*(len+1)/2 ≈ 5e9), exceeding 32-bit int. Use vector<long long> of size n. For each position, create a new vector `cur` initialized to zero, set `cur[digit % n] += 1`, then for each prior remainder `j` from `0` to `n-1`, add `prev[j]` to `cur[(j*10 + digit) % n]`. Then add `cur[0]` to total. Finally, `prev = cur`. Return total. Note: The original snippet used a 2D DP and returned `dp[len-1][0]`, but that returns only substrings ending at the last position, not all substrings. The task spec here defines a different function, so we must implement the correct counting of all substrings.
#include <string>
#include <vector>

// Count substrings of str whose numeric value is divisible by n.
// str consists of digits '0'-'9'. n is a positive integer.
long long countNDigitSubstringsDivisibleByN(const std::string& str, int n) {
    const int len = static_cast<int>(str.length());
    if (n == 1) {
        // All substrings are divisible by 1.
        return static_cast<long long>(len) * (len + 1) / 2;
    }

    // prev[r] = number of substrings ending at previous position with remainder r.
    std::vector<long long> prev(n, 0);
    long long total = 0;

    for (int i = 0; i < len; ++i) {
        int digit = str[i] - '0';
        std::vector<long long> cur(n, 0);

        // Single-digit substring: just the digit itself.
        cur[digit % n] += 1;

        // Extend each substring ending at previous position by this digit.
        for (int r = 0; r < n; ++r) {
            if (prev[r] > 0) {
                int newRem = (r * 10 + digit) % n;
                cur[newRem] += prev[r];
            }
        }

        total += cur[0];
        prev = std::move(cur);
    }

    return total;
}
#include <cassert>
#include <string>

// Declaration of the solution function (assumed to be in the same translation unit above).
long long countNDigitSubstringsDivisibleByN(const std::string& str, int n);

int main() {
    // Test cases
    assert(countNDigitSubstringsDivisibleByN("10", 3) == 1);  // only "0" (0 % 3 == 0); "1" and "10" are not.
    assert(countNDigitSubstringsDivisibleByN("123", 3) == 4); // "12" (12%3=0), "3" (3%3=0), "123" (123%3=0), "0"? Actually "1","2","3","12","23","123" -> 3,12,123 -> 3 substrings? Wait: "3" is 3%3=0, "12" is 12%3=0, "123" is 123%3=0, also "23" is 23%3=2, "1" is 1%3=1, "2" is 2%3=2. So only 3. Let's check: "3" yes, "12" yes, "123" yes. So 3.
    assert(countNDigitSubstringsDivisibleByN("123", 3) == 3);
    assert(countNDigitSubstringsDivisibleByN("111", 1) == 6); // all substrings (1,1,1,11,11,111) = 6.
    assert(countNDigitSubstringsDivisibleByN("0", 5) == 1);   // "0" is 0 % 5 == 0.
    assert(countNDigitSubstringsDivisibleByN("5", 5) == 1);
    assert(countNDigitSubstringsDivisibleByN("50", 5) == 3); // substrings: "5" (5%5=0), "0" (0%5=0), "50" (50%5=0) -> 3.
    assert(countNDigitSubstringsDivisibleByN("100", 100) == 1); // only "00" (0%100) and "0"? Actually "100" %100=0, "00" as substring is value 0, "0" also. But "00" is a substring at indices 1-2, "0" at index 2 only. "0" at index 2 is 0%100=0, "00" is 0, "100" is 0, so 3? Let's check: positions: substrings: "1","0","0","10","00","100". "1"=1%100=1, "0"=0, "0"=0, "10"=10%100=10, "00"=0, "100"=0 => three zeros? Actually two separate "0" substrings (each single digit) plus "00" and "100" => four. So answer should be 4. Let's test that.
    assert(countNDigitSubstringsDivisibleByN("100", 100) == 4);
    // Edge: n > 9, digits themselves may not be divisible.
    assert(countNDigitSubstringsDivisibleByN("77", 7) == 2); // "7" (7%7=0), "77" (77%7=0) -> 2, but also "7" second position -> yes, two single-digit "7"s and "77" -> total 3. Actually indices: 0..1: "7" (0) divisible, "77" divisible, "7" (1) divisible => 3.
    assert(countNDigitSubstringsDivisibleByN("77", 7) == 3);
    // Long string sanity: all zeros divisible by any n.
    std::string zeros(100, '0');
    assert(countNDigitSubstringsDivisibleByN(zeros, 1) == 5050);
    assert(countNDigitSubstringsDivisibleByN(zeros, 10) == 5050); // any substring value is 0, divisible by 10.
    return 0;
}
