/*
You are given an array of length `n` initially empty (all entries equal to 0) and a sequence of `m` operations. Each operation adds 1 to a specific index (1-based) of the array. After each operation, you must output `1` if all `n` positions have a non-zero value (i.e., every distinct index from 1 to n has been incremented at least once), and `0` otherwise. But there is an extra twist: whenever all `n` positions become non-zero simultaneously, **all** current values in the array are reset to 0 before the next operation continues. Write a C++ function that processes the operations (given as a vector of indices) and returns a string of `0`/`1` characters (each representing the output after the respective operation, in order). For example, if `n=3` and operations are `[1,2,3,1]`, then after the 1st op: array `[1,0,0]` → `0`; after 2nd: `[1,1,0]` → `0`; after 3rd: `[1,1,1]` → `1` (then reset to `[0,0,0]`); after 4th (index 1): `[1,0,0]` → `0`, so result is `"0010"`. Note: operations are guaranteed to be valid indices between 1 and n. The function should handle any `n >= 1` and `m >= 0` (if `m=0`, return an empty string).
*/

#include <string>
#include <vector>

std::string processOperations(int n, const std::vector<int>& ops) {
    std::string result;
    result.reserve(ops.size());
    std::vector<int> freq(n + 1, 0); // 1-based
    int distinct = 0;

    for (int idx : ops) {
        if (freq[idx] == 0) {
            ++distinct;
        }
        ++freq[idx];

        if (distinct == n) {
            result.push_back('1');
            // Reset all frequencies to 0
            for (int i = 1; i <= n; ++i) {
                if (--freq[i] == 0) {
                    --distinct;
                }
            }
        } else {
            result.push_back('0');
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// include function here

int main() {
    // basic case
    assert(processOperations(3, {1,2,3,1}) == "0010");
    // n=1: first op always gives 1, then reset
    assert(processOperations(1, {1,1,1}) == "111");
    // empty ops
    assert(processOperations(5, {}) == "");
    // no full set yet
    assert(processOperations(4, {1,1,2,3}) == "0000");
    // exactly reaches all then resets and reaches again
    assert(processOperations(2, {1,2,1,2}) == "0101");
    // repeated same index many times
    assert(processOperations(2, {1,1,1,2}) == "0001");
    // larger n, only one complete cycle
    assert(processOperations(5, {1,2,3,4,5}) == "00001");
    return 0;
}

// The key is to track a counter `distinct` representing how many distinct indices have appeared since the last reset. We also need a frequency array `freq[1..n]`. For each operation index `a`, if `freq[a] == 0` before increment, then this introduces a new distinct index, so we increment `distinct`. After incrementing `freq[a]`, if `distinct == n`, it means all positions are non-zero, so we append `'1'` to the result and then reset: for every `i` from 1 to n, decrement `freq[i]`; if it becomes 0, decrement `distinct`. Otherwise, we append `'0'`. Edge cases: `n=1` means after any operation, `distinct` becomes 1, so the first operation always outputs `'1'` and resets. If `m=0`, loop does nothing, returns empty string. Frequency array must be cleared per call; we can use a `vector<int>` initialized with zeros. The reset loop runs O(n) per full burst, but each time we reset we essentially mark that all indices are covered, and the number of resets is at most floor(m/n) because each reset requires at least n operations to happen again, so total time is O(m + (m/n)*n) = O(m). Space is O(n).
