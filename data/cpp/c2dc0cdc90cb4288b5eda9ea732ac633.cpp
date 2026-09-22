/*
Write a C++ function `sumOfPairs` that takes a positive integer `n` and returns the sum of values given by `n` pairs of integers, where each pair `(a, b)` is provided via standard input. The function must read the pairs in order, compute each `a + b`, accumulate the total sum, and return that total as an `int`. You may assume the input contains exactly `n` pairs of integers, each within the range of a 32-bit signed integer, and that the total sum will not overflow.
*/

#include <cstdio>

// Reads n pairs from standard input and returns the sum of all a + b values.
int sumOfPairs(int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        int a, b;
        std::scanf("%d %d", &a, &b);
        total += a + b;
    }
    return total;
}

#include <cassert>
#include <cstdio>
#include <sstream>
#include <iostream>

// Forward declaration (or use the solution directly if in the same file).
int sumOfPairs(int n);

// Redirect std::cin to use a stringstream for deterministic testing.
int testSum(const std::string& input, int n) {
    std::istringstream buffer(input);
    std::streambuf* old = std::cin.rdbuf(buffer.rdbuf());
    int result = sumOfPairs(n);
    std::cin.rdbuf(old);
    return result;
}

int main() {
    assert(testSum("1 2\n3 4\n", 2) == 10);
    assert(testSum("0 0\n0 0\n0 0\n", 3) == 0);
    assert(testSum("-5 5\n", 1) == 0);
    assert(testSum("100 200\n-50 30\n1 1\n", 3) == 282);
    assert(testSum("", 0) == 0);
    std::cout << "All tests passed.\n";
    return 0;
}

// The solution reads the count of pairs `n` from standard input using `scanf` or a stream. Then it loops `n` times, each iteration reading two integers `a` and `b`, adding their sum to a running total, and returning the total after the loop. Edge cases include when `n` is 0 (the loop does not run, and the function returns 0) and when individual sums overflow—since the total is guaranteed not to overflow, we can accumulate directly. The algorithm runs in \(O(n)\) time and uses \(O(1)\) auxiliary space, as only a few scalar variables are needed.
