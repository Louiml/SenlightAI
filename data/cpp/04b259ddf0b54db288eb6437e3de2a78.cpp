Given two positive integers `pos` and `t` (where `t` is between 1 and 16), write a C++ function that returns the smallest positive integer `n` (in base 10) such that among all positive integers from 1 to `n` inclusive, there are at least `pos` numbers whose hexadecimal representation (without leading zeros, using lowercase letters a-f) contains **no digit that appears more than `t` times**. More formally, define a number as *valid* if every hexadecimal digit `d` (0-15) appears at most `t` times in its hexadecimal representation. The function should return the `pos`-th valid number in ascending order (1-based indexing). If `pos` is 0, return 0. The input `pos` can be up to 10^7 and `t` between 1 and 16. The function must handle `n` up to at least 2e9.
#include <bits/stdc++.h>
int main() {
    // Basic tests
    assert(kthValidNumber(1, 16) == 1); // all numbers valid if t=16
    assert(kthValidNumber(10, 16) == 10);
    assert(kthValidNumber(16, 1) == 30); // first 16 numbers are 0x1..0xf, then 0x10 is invalid because '1' appears twice
    // t=2: first invalid is 0x11 (17 decimal) and 0x22 etc, but sequence: 1..15 valid, 16=0x10 valid, 17=0x11 invalid, so 17th valid is 18? Let's compute: up to 15:15 valid, 16 (0x10) valid (1 and 0 each once), 17 (0x11) invalid, 18 (0x12) valid, so 16th valid is 18.
    assert(kthValidNumber(16, 2) == 18);
    // t=1: every digit at most once: numbers: 1..15, then 16 (0x10) valid, 17 invalid, 18 valid, ... but 0x21? Actually up to 0x1f: 31 numbers? Let's check: 1..15 =15, 16 (0x10) =16, 17 (0x11) invalid, 18 (0x12)=17, 19=18, 20=19, 21=20, 22=21, 23=22, 24=23, 25=24, 26=25, 27=26, 28=27, 29=28, 30=29, 31=30 (0x1f), then 32 (0x20) valid =31, 33 invalid, 34 (0x22) invalid, 35 (0x23) valid =32. So 32nd valid is 35.
    assert(kthValidNumber(32, 1) == 35);
    // Edge: pos=0 returns 0
    assert(kthValidNumber(0, 1) == 0);
    // Large pos
    assert(kthValidNumber(1000000, 1) > 0);
    printf("All tests passed.\n");
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Global DP for memoization
int dp[9][2][2][9][1 << 16];
int freq[17];
int targetFreq; // t
vector<int> num;

// Digit DP: count valid numbers from 0 to the given number represented by 'num'
int solve(int isStart, int isSmall, int pos, int msk, int sz) {
    if (pos == 0) {
        // reached the end; this is a valid number (including 0, but later subtract 1)
        return 1;
    }
    int &ret = dp[sz][isStart][isSmall][pos][msk];
    if (ret != -1 && isSmall) return ret;

    int limit;
    int idx = num.size() - pos;
    if (isSmall) limit = 15; else limit = num[idx];

    int total = 0;
    int newMsk;
    if (!isStart) {
        // already started: can place any digit 0..limit
        for (int d = 0; d <= limit; ++d) {
            if (freq[d] == targetFreq) continue;
            newMsk = msk;
            if (freq[d] == targetFreq - 1) newMsk |= (1 << d);
            freq[d]++;
            total += solve(0, isSmall || d < num[idx], pos - 1, newMsk, sz);
            freq[d]--;
        }
    } else {
        // still at leading zeros: first non-zero digit can be 1..limit
        for (int d = 1; d <= limit; ++d) {
            if (freq[d] == targetFreq) continue;
            newMsk = msk;
            if (freq[d] == targetFreq - 1) newMsk |= (1 << d);
            freq[d]++;
            total += solve(0, isSmall || d < num[idx], pos - 1, newMsk, sz);
            freq[d]--;
        }
        // option to place a leading zero: still not started, but we reduce length
        total += solve(1, 1, pos - 1, 0, sz - 1);
    }

    return ret = total;
}

// Count numbers from 0 to n (inclusive) that are valid
int countValidUpTo(long long n) {
    if (n < 0) return 0;
    num.clear();
    long long temp = n;
    if (temp == 0) {
        num.push_back(0);
    }
    while (temp > 0) {
        num.push_back(temp % 16);
        temp /= 16;
    }
    reverse(num.begin(), num.end());
    memset(freq, 0, sizeof(freq));
    return solve(1, 0, (int)num.size(), 0, (int)num.size()) - 1; // subtract the empty number
}

// Find the smallest n such that countValidUpTo(n) >= pos
long long kthValidNumber(int pos, int t) {
    if (pos == 0) return 0;
    // Reset DP once (since t is fixed for this call)
    memset(dp, -1, sizeof(dp));
    targetFreq = t;
    long long lo = 1, hi = 2000000000LL, ans = 0;
    while (lo <= hi) {
        long long mid = (lo + hi) / 2;
        int cnt = countValidUpTo(mid);
        if (cnt >= pos) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}
// We need to count valid numbers up to a given `n` and binary search for the smallest `n` such that the count is at least `pos`. The count is done using digit DP over hexadecimal digits. For a given `n`, we convert it to a vector of hex digits (most significant first). The DP state is: position `pos` (from most significant), whether we have started the number (`isStart`), whether we are already smaller than the prefix (`isSmall`), the current digit frequency mask (`msk`) that uses 16 bits to indicate which digits have already reached `t-1` occurrences, and the total length `sz` of the number (to track how many digits remain, which matters because leading zeros are not counted). The DP returns the number of valid completions. We memoize on these dimensions. Important edge cases: leading zeros are allowed only before the first nonzero digit; the number 0 is not counted (we subtract 1 from the result of `calc(n)`). The binary search range is from 1 to 2e9, but since `pos` can be large, we ensure the high bound is enough (we can set hi=2e9 as in the snippet). The final answer is the smallest number with count >= pos. After finding it, we must verify correctness—but the DP already ensures it. Time complexity: O(log(2e9) * number_of_states) where states are O(pos_len * 2 * 2 * 16 * 2^16) but in practice the mask is only 16 bits and many states are not visited; roughly O(16 * 2 * 2 * 16 * 2^16) per call, but with memoization across queries (DP array is static and reused for all `calc` calls). Space: O(9 * 2 * 2 * 9 * 65536) integers, which is about 9*2*2*9*65536 ≈ 21 million, but note `sz` can be up to 8 for 2e9, so we use 9 as max size. This fits memory.
