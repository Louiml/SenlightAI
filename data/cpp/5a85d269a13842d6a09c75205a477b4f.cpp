/*
Write a C++ function named `fibonacciBelowLimit` that takes a single positive integer `limit` as input and returns a `std::vector<int>` containing all Fibonacci numbers (starting with 0 and 1) that are strictly less than `limit`. The function must produce the sequence in ascending order (0, 1, 1, 2, 3, 5, ...), but ensure that no duplicate 1 appears more than once in the output. In other words, the result should be a unique list of Fibonacci numbers below the limit. For example, for `limit = 10`, the output should be `{0, 1, 2, 3, 5, 8}` (not `{0, 1, 1, 2, 3, 5, 8}`). If `limit` is 0 or 1, the function returns an empty vector (since no Fibonacci number is strictly less than 1; 0 is allowed only if limit > 0, but for limit=1, 0 < 1, so actually return `{0}` for limit=1? Wait—re-read: For limit=1, 0 < 1, so return `{0}`). Clarify with edge cases: For limit=0, no numbers; for limit=1, return `{0}`; for limit=2, return `{0, 1}`; for limit≥3, include 0, 1, and subsequent terms less than limit. The function must handle large limits (up to 2^31-1) without overflow by stopping when the next Fibonacci number would be >= limit.
*/

#include <vector>

// Return unique Fibonacci numbers (starting 0, 1) strictly less than limit.
std::vector<int> fibonacciBelowLimit(int limit) {
    std::vector<int> result;
    if (limit <= 0) {
        return result;
    }

    // Always include 0 if it's below limit.
    if (0 < limit) {
        result.push_back(0);
    }

    // Include 1 if below limit (avoid duplicate 1 from sequence generation).
    if (1 < limit && (result.empty() || result.back() != 1)) {
        result.push_back(1);
    }

    int a = 0, b = 1;
    while (true) {
        int c = a + b; // Next Fibonacci term
        if (c >= limit) {
            break;
        }
        result.push_back(c);
        a = b;
        b = c;
    }

    return result;
}

#include <cassert>
#include <vector>

// Function declaration (from solution)
std::vector<int> fibonacciBelowLimit(int limit);

int main() {
    assert(fibonacciBelowLimit(0).empty());
    assert(fibonacciBelowLimit(1) == std::vector<int>{0});
    assert(fibonacciBelowLimit(2) == std::vector<int>{0, 1});
    assert(fibonacciBelowLimit(3) == std::vector<int>{0, 1, 2});
    assert(fibonacciBelowLimit(10) == std::vector<int>{0, 1, 2, 3, 5, 8});
    assert(fibonacciBelowLimit(100) == std::vector<int>{0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89});
    assert(fibonacciBelowLimit(5) == std::vector<int>{0, 1, 2, 3});
    assert(fibonacciBelowLimit(1) == std::vector<int>{0});
    return 0;
}

// The algorithm starts with the first two Fibonacci numbers: 0 and 1. We need to add 0 if it is less than `limit`, then add 1 if it is less than `limit` and not already added (since 1 appears twice when generating naively, we must avoid duplication). A clean approach: maintain a `vector<int>` result. Push 0 if `0 < limit`. Then, if `1 < limit` and the last element of result is not 1 (or if result is empty), push 1. Then compute next terms: `a = 0, b = 1`, and in a loop compute `c = a + b`, then if `c >= limit` break; otherwise push `c` and update `a = b; b = c;`. This naturally avoids duplication because we skip the duplicate 1 by initializing with a and b already used. Edge cases: `limit <= 0` return empty; `limit == 1` returns `{0}`; `limit == 2` returns `{0, 1}`; for any larger limit, the loop runs until the next term is out of range. Time complexity: O(k) where k is the number of Fibonacci numbers below the limit, which is O(log(limit)) because Fibonacci grows exponentially; space complexity is O(k) for the returned vector.
