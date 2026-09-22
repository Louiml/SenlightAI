/*
Write a C++ function that takes a positive integer `n` and returns a `std::vector<int>` containing the first `n+1` numbers of a modified Fibonacci-like sequence, where the first element is always 0, and each subsequent element is the sum of the previous two elements in the sequence, but with a twist: the sequence starts with `a=0`, `b=1`, and `c=0` as initialized in the snippet, and after printing 0, for each step from 1 to `n`, the next value is computed as `a = b + c`, then `b` becomes the old `c`, and `c` becomes the new `a`. The function must return a vector of exactly `n+1` integers (the initial 0 followed by `n` computed values). Handle edge cases: if `n` is 0, return just `{0}`. Assume `n` is non-negative.
*/

#include <vector>

// Return the first n+1 numbers of the modified sequence starting with 0.
std::vector<int> modifiedSequence(int n) {
    std::vector<int> result;
    result.push_back(0);
    int a = 0, b = 1, c = 0;
    for (int i = 1; i <= n; ++i) {
        a = b + c;
        b = c;
        c = a;
        result.push_back(a);
    }
    return result;
}

#include <cassert>
#include <vector>

// Solution function prototype
std::vector<int> modifiedSequence(int n);

int main() {
    // Test n=0
    assert(modifiedSequence(0) == std::vector<int>({0}));
    // Test n=1
    assert(modifiedSequence(1) == std::vector<int>({0, 1}));
    // Test n=2
    assert(modifiedSequence(2) == std::vector<int>({0, 1, 1}));
    // Test n=3
    assert(modifiedSequence(3) == std::vector<int>({0, 1, 1, 2}));
    // Test n=4 (matches the snippet's output for n=4: 0,1,1,2,3)
    assert(modifiedSequence(4) == std::vector<int>({0, 1, 1, 2, 3}));
    // Test n=5
    assert(modifiedSequence(5) == std::vector<int>({0, 1, 1, 2, 3, 5}));
    // Test n=6
    assert(modifiedSequence(6) == std::vector<int>({0, 1, 1, 2, 3, 5, 8}));
    // Test n=10 (checking a longer sequence)
    assert(modifiedSequence(10) == std::vector<int>({0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55}));
    return 0;
}

// The algorithm simulates the loop from the snippet directly. Initialize `a = 0`, `b = 1`, `c = 0`, and a vector `result` that begins with `0`. For each iteration `i` from 1 to `n` (inclusive), compute `a = b + c`, then assign `b = c` and `c = a`, then append `a` to the vector. This is a straight translation. Edge cases: when `n = 0`, the loop does not execute, and the vector contains only `{0}`. When `n` is large, values may overflow `int`; the problem statement does not specify constraints, but for typical exercises this is acceptable; mention that if larger values were expected, we would use `long long` or arbitrary precision. Time complexity is O(n) because we perform constant work per iteration and build a vector of size n+1. Space complexity is O(n) for the returned vector, and O(1) auxiliary space for the loop variables.
