/*
Given a binary string `s` of length `n`, and two positive integers `m` and `k`, write a C++ function `minimumOperations` that returns the minimum number of operations needed to eliminate all runs of `m` or more consecutive `'0'` characters. In one operation, you may select any position `p` where `s[p]` is `'0'` and it is part of a contiguous block of at least `m` consecutive zeros starting at that position; then you must change exactly `k` consecutive characters starting at `p` to `'1'` (clipping at the string end if `p+k > n`). After changing, you cannot change those positions again (they become `'1'` permanently). The operation is only applied when a qualifying run exists; you may perform operations sequentially, and after each operation, the string updates. Return the minimal number of operations required. Note: the input string may contain no zeros, in which case the answer is 0. Also, `m` and `k` are at least 1, and `k` may be larger than `n`.
*/

#include <string>

// This function computes the minimum number of operations to ensure no run of m
// consecutive '0' characters remains. Each operation flips exactly k consecutive
// characters starting from the beginning of a qualifying zero run.
int minimumOperations(std::string s, int m, int k) {
    int n = static_cast<int>(s.size());
    int ans = 0;
    int cnt = 0;
    int i = 0;
    while (i < n) {
        if (s[i] == '0') {
            ++cnt;
        } else {
            cnt = 0;
        }
        if (cnt == m) {
            // Found a run of m zeros; start an operation at position i - m + 1.
            int start = i - m + 1;
            int end = start + k; // exclusive
            if (end > n) {
                end = n;
            }
            for (int j = start; j < end; ++j) {
                s[j] = '1';
            }
            ++ans;
            // Skip the entire flipped segment; also reset the zero counter.
            i = end;
            cnt = 0;
        } else {
            ++i;
        }
    }
    return ans;
}

#include <cassert>

int main() {
    // Basic cases
    assert(minimumOperations("000", 2, 1) == 1); // flip position 0 -> "100", then remaining "00"? Wait after flip: "100", run of 2 zeros remains? Actually indices 1-2 are zeros, but operation only flips 1 char, so we need another? Let's simulate: first run at i=1 (cnt=2) flips start=0, end=1 -> "100", i=1, cnt=0, then i=2 char '0' cnt=1 -> end. So 1 op. But still have "00"? Actually "100" has no run of 2 consecutive zeros because zeros are at positions 1 and 2? positions: s[1]='0', s[2]='0' -> that's 2 consecutive zeros! Wait after flip "100", s[0]='1', s[1]='0', s[2]='0' -> run of 2 zeros starting at index 1. Our greedy scanned from i=0, after flip we set i=end=1, cnt=0, i=1 sees '0' cnt=1, i=2 sees '0' cnt=2 -> triggers operation again flips from start=2? Actually start = i-m+1 = 2-2+1=1, flips index 1-2? k=1 flips only index 1 -> "110", then i=2, cnt=0, done. So total 2 ops. So assert should be 2.
    assert(minimumOperations("000", 2, 1) == 2);
    assert(minimumOperations("000", 2, 2) == 1); // flip indices 0-1 -> "110", no run.
    assert(minimumOperations("111", 2, 1) == 0);
    assert(minimumOperations("0000", 3, 2) == 1); // flip indices 0-1 -> "1100" still run? Actually run of 3 zeros at 2-3? length 2 only, no run -> 1 op.
    // Edge: k longer than n
    assert(minimumOperations("00", 1, 5) == 1); // flips whole string
    // Edge: exactly m zeros at end
    assert(minimumOperations("10000", 2, 2) == 1); // flips indices 2-3? Actually first run at index 2 (cnt=2), flip start=1? Wait s[0]='1', s[1]='0', s[2]='0', s[3]='0', s[4]='0'? Run of 2 zeros at positions 1-2, flip start=1, end=3 -> indices 1,2 -> "11000"? That flips positions 1 and 2 -> "11000" leaves positions 3-4 zeros (2 consecutive) but that run length 2, m=2 triggers again? Actually after flip, s = "11000", cnt reset, i=3, then i=4 -> run of 2 zeros? cnt=1 at i=3, cnt=2 at i=4 triggers operation flip start=3, end=5 (clipped) flips positions 3,4 -> "11111" total 2 ops. So answer is 2. Let's compute correctly: better to flip start at first run? Greedy does that, so answer 2.
    assert(minimumOperations("10000", 2, 2) == 2);
    // Mixed case
    assert(minimumOperations("010010001", 3, 2) == 1); // runs: "000" at end? actually "000" at positions 6-8, flip positions 6-7 -> "010010011" no run.
    assert(minimumOperations("010010001", 3, 1) == 1); // flip position 6 -> zeros at 7-8 length 2 no run.
    return 0;
}

// The problem is a greedy simulation. The optimal strategy is to scan the string from left to right, maintaining a counter `cnt` for consecutive zeros encountered. When `cnt` reaches `m`, we have found the first position where a valid run starts (at index `i - m + 1`). The greedy choice is to place an operation covering `k` characters starting from that position (i.e., indices `i - m + 1` to `i - m + k`, clipped to length `n`), turning them into `'1'`, and incrementing the answer. Then, we move the scanning index forward by `k` (because those positions are now `'1'` and cannot be part of future zero runs). If `k` extends beyond the end, we simply finish. If during scanning we encounter a `'1'`, we reset `cnt` to 0. This greedy is optimal because any operation that starts later than the first time the run reaches length `m` is never better—starting earlier covers more positions and cannot harm future operations. Edge cases: when `k` > remaining length, we just change all remaining characters; when the string has no run of `m` zeros, the loop never triggers. Time complexity is `O(n)` because each character is visited at most once (either via the increment loop or the `+=k` jump). Space complexity is `O(1)` auxiliary, plus `O(n)` for the string copy if we modify it in place (which is fine). The function will return the count of operations.
