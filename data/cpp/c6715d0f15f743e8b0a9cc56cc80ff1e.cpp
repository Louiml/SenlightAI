// Given positive integers `y`, `k`, and `n` (with `1 ≤ y, k ≤ n ≤ 10^9`), write a C++ function `printMultiplesAfterY` that returns a string containing all integers `x` such that `y < x ≤ n` and `x` is divisible by `k`, separated by spaces in ascending order. If no such integers exist, return the string `"-1"`. The function must not print anything; instead, it should build and return the result as a `std::string`. Each integer in the output must be in its decimal form without leading zeros.

#include <cassert>
#include <string>

// Declaration of the function under test.
std::string printMultiplesAfterY(int y, int k, int n);

int main() {
    // Basic cases
    assert(printMultiplesAfterY(5, 3, 15) == "6 9 12 15");
    assert(printMultiplesAfterY(10, 5, 30) == "15 20 25 30");
    
    // When y is a multiple of k, the first output is y + k
    assert(printMultiplesAfterY(6, 3, 12) == "9 12");
    
    // No multiples in range
    assert(printMultiplesAfterY(7, 5, 9) == "-1");
    assert(printMultiplesAfterY(10, 20, 25) == "-1");
    
    // Single multiple
    assert(printMultiplesAfterY(2, 4, 4) == "4");
    
    // y close to n
    assert(printMultiplesAfterY(8, 2, 10) == "10");
    
    // Large k, only one possible
    assert(printMultiplesAfterY(1, 10, 10) == "10");
    
    // Consecutive multiples when k=1
    assert(printMultiplesAfterY(3, 1, 7) == "4 5 6 7");
}

#include <string>
#include <vector>

// Returns a string of all integers x such that y < x <= n and x % k == 0,
// separated by spaces in ascending order. If none exist, returns "-1".
std::string printMultiplesAfterY(int y, int k, int n) {
    // First multiple of k that is strictly greater than y.
    int first = y + (k - y % k);
    
    std::vector<int> multiples;
    for (int x = first; x <= n; x += k) {
        multiples.push_back(x);
    }
    
    if (multiples.empty()) {
        return "-1";
    }
    
    std::string result;
    for (size_t i = 0; i < multiples.size(); ++i) {
        if (i > 0) {
            result += " ";
        }
        result += std::to_string(multiples[i]);
    }
    return result;
}

// The key is to find the smallest multiple of `k` that is strictly greater than `y`. That value is `k - (y % k)` added to `y`, but careful: if `y % k == 0`, then `k - 0 = k`, so `y + k` is the first multiple (since we need `x > y`). So the first candidate is `first = y + (k - y % k)` (if `y % k` is 0, this becomes `y + k`, which is correct). Then iterate `x = first, first + k, first + 2k, ...` while `x ≤ n`, appending each to the result with a space separator (except after the last). If no numbers are appended, return `"-1"`. Edge cases: when `k > n - y`, there may be no multiples; when `y` is already a multiple of `k`, the first multiple is `y + k`; ensure the loop condition uses `x + y <= n` equivalently as `x <= n` since `x` already includes the offset. Time complexity is `O((n-y)/k)` in the worst case when `k=1`, but with given constraints it's at most ~10^9 iterations which is too slow; however, the problem statement likely expects a more efficient approach: we can directly compute the number of terms as `count = (n - first) / k + 1` if `first ≤ n`, and produce them in a loop. Since `n` can be up to 10^9, if `k=1` and `y=1`, we'd output ~10^9 numbers, which is infeasible in practice; but the task is designed for a theoretical solution, and the reference implementation will iterate. Time complexity is `O((n-first)/k + 1)` and space is `O(count)` for the output string, which matches the problem's intent. For safety, we can assume `n-y` is small enough for output, but for a robust solution we should note it. The algorithm is straightforward: find first multiple, then step by `k`.
