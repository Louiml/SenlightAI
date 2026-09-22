/*
Write a C++ function named `specialPair` that takes a single integer `n` (with \(1 \le n \le 10^9\)) and returns a `std::pair<int, int>` representing two distinct positive integers \(a\) and \(b\). If \(n = 1\), the only valid pair is \((1, 1)\) (even though they are not distinct, this is the required output). For all \(n > 1\), the pair must satisfy \(a = n\) and \(b = n - 1\). The function must return the pair in the order (larger, smaller). Assume the input is always valid. The function must be `const`-correct and use no external libraries beyond standard headers.
*/
#include <utility>

// Return a pair of integers as per the special rule.
// For n == 1, returns {1, 1}.
// For n > 1, returns {n, n - 1}.
std::pair<int, int> specialPair(int n) {
    if (n == 1) {
        return {1, 1};
    }
    return {n, n - 1};
}
#include <cassert>

int main() {
    assert(specialPair(1) == std::make_pair(1, 1));
    assert(specialPair(2) == std::make_pair(2, 1));
    assert(specialPair(3) == std::make_pair(3, 2));
    assert(specialPair(10) == std::make_pair(10, 9));
    assert(specialPair(1000000000) == std::make_pair(1000000000, 999999999));
}
// The problem is trivial: the output pair is directly determined by the value of `n`. For \(n = 1\), we return `{1, 1}` because there is no smaller positive integer. For \(n > 1\), we return `{n, n-1}`. No loops or complex logic are needed. Edge case: if `n` is `1`, returning `{n, n-1}` would give `{1, 0}`, which is invalid because `0` is not positive, so we handle it separately. Time complexity is \(O(1)\) (constant time), and space complexity is \(O(1)\) as we only store the return pair.
