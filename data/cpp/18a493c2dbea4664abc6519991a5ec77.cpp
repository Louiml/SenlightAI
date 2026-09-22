// Given a string `s` consisting only of lowercase letters `'c'`, `'f'`, `'y'`, and `'z'`, write a C++ function `std::string evaluateSequence(const std::string& s)` that processes the string from left to right while maintaining a running integer `balance` (initialized to 0) and a prefix-minimum `minBalance` (initialized to 0). For each character: if it is `'c'`, increment a counter `cCount` (number of `'c'` seen so far); if it is `'f'`, add `cCount` to `balance`; if it is `'z'`, increment a counter `zCount` (number of `'z'` seen so far); if it is `'y'`, subtract `zCount` from `balance`. After processing the entire string, if `minBalance >= 0`, return the string `"YES"`. Otherwise, return the string `"NO"` followed by a space and the absolute value of `minBalance` (i.e., `-minBalance`). The function must handle empty strings (though constraints may ensure non-empty) and any mix of the four letters. The output format must exactly match: either `"YES"` or `"NO <positive integer>"`.

The problem simulates a simple state machine with two counters: `cCount` tracks the number of `'c'` characters encountered, and `zCount` tracks the number of `'z'` characters encountered. The variable `balance` changes only when `'f'` appears (adding the current `cCount`) or when `'y'` appears (subtracting the current `zCount`). The prefix-minimum `minBalance` is the smallest value `balance` has reached during the sweep. The final result depends on whether any prefix of the balance is negative. If the minimum is non-negative, then all intermediate balances were non-negative, so the condition is satisfied. If the minimum is negative, the required "addition" to make the sequence valid is exactly `-minBalance`, because adding that constant to the entire balance trajectory would shift every balance upward by that amount, making the smallest balance zero (or positive) and all others non-negative. Edge cases include: a string with no `'f'` or `'y'` (balance stays 0, min is 0, returns "YES"), a string where `'y'` occurs before any `'z'` (subtraction by 0 has no effect), and large counts where `cCount` or `zCount` could become large — using a 64-bit integer for `balance`, `minBalance`, `cCount`, and `zCount` is prudent. Time complexity is O(n) for a string of length n, and space complexity is O(1) auxiliary.

#include <string>
#include <cstdint>
#include <algorithm>

// Evaluate a sequence of characters 'c','f','y','z' and return a verdict.
// Returns "YES" if the running balance never drops below 0.
// Otherwise returns "NO <positive integer>", where the integer is the
// minimum number of 'c' or 'z' increments needed to make all balances non-negative.
std::string evaluateSequence(const std::string& s) {
    std::int64_t balance = 0;
    std::int64_t minBalance = 0;
    std::int64_t cCount = 0;
    std::int64_t zCount = 0;

    for (char ch : s) {
        if (ch == 'c') {
            ++cCount;
        } else if (ch == 'f') {
            balance += cCount;
        } else if (ch == 'z') {
            ++zCount;
        } else if (ch == 'y') {
            balance -= zCount;
        }
        minBalance = std::min(minBalance, balance);
    }

    if (minBalance >= 0) {
        return "YES";
    } else {
        return "NO " + std::to_string(-minBalance);
    }
}

#include <cassert>
#include <string>

// Function under test (copied here for standalone test)
std::string evaluateSequence(const std::string& s) {
    std::int64_t balance = 0;
    std::int64_t minBalance = 0;
    std::int64_t cCount = 0;
    std::int64_t zCount = 0;

    for (char ch : s) {
        if (ch == 'c') {
            ++cCount;
        } else if (ch == 'f') {
            balance += cCount;
        } else if (ch == 'z') {
            ++zCount;
        } else if (ch == 'y') {
            balance -= zCount;
        }
        minBalance = std::min(minBalance, balance);
    }

    if (minBalance >= 0) {
        return "YES";
    } else {
        return "NO " + std::to_string(-minBalance);
    }
}

int main() {
    // All characters are 'c' or 'z' only: balance never changes, so YES.
    assert(evaluateSequence("cccc") == "YES");
    assert(evaluateSequence("zzzz") == "YES");
    assert(evaluateSequence("") == "YES"); // empty string

    // One 'f' after one 'c': balance becomes 1, stays non-negative.
    assert(evaluateSequence("cf") == "YES");

    // 'y' before any 'z' subtracts 0, then later 'z' increments zCount.
    assert(evaluateSequence("yczy") == "NO 1"); 
    // Explanation: 'y' subtracts 0 (zCount=0), 'c' (cCount=1), 'z' (zCount=1), 'y' subtracts 1 -> balance becomes -1, min = -1.

    // Two 'c's then one 'f' adds 2 -> balance=2, then one 'z' then two 'y's subtracts 1 then 1 -> balance=2-1-1=0.
    assert(evaluateSequence("ccfzyy") == "YES");

    // Single 'y' after nothing: subtracts 0, still 0.
    assert(evaluateSequence("y") == "YES");

    // Complex: "cfzy" -> 'c' (cCount=1), 'f' (+1=1), 'z' (zCount=1), 'y' (-1=0) -> min 0.
    assert(evaluateSequence("cfzy") == "YES");

    // "czy" -> 'c'(1), 'z'(zCount=1), 'y'(-1=-1) -> min -1.
    assert(evaluateSequence("czy") == "NO 1");

    // "ccfzzyy": cCount becomes 2, f -> +2, zCount becomes 2, two y subtract 2*2=4 -> balance 2-4 = -2, min=-2.
    assert(evaluateSequence("ccfzzyy") == "NO 2");

    // Many c then f then many y after many z.
    assert(evaluateSequence("ccccfzzzyyy") == "YES"); // balance: cCount=4, f -> +4, zCount=3, three y -> -3*3=-9, total = -5? Let's check: 4 - 9 = -5, min=-5 -> actually NO 5. Let's correct: "ccccfzzzyyy": c=4, f -> balance=4, z=3, y1 -> 4-3=1, y2 -> 1-3=-2, y3 -> -2-3=-5 -> min=-5.
    assert(evaluateSequence("ccccfzzzyyy") == "NO 5");

    return 0;
}
