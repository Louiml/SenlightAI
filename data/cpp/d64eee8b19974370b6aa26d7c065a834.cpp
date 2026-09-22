Given an integer `n` followed by `n` integers on a single line (or spread across multiple lines), write a C++ function that returns the sum of the absolute differences between every pair of consecutive integers in the sequence. That is, for a sequence `a_1, a_2, ..., a_n`, compute `|a_1 - a_2| + |a_2 - a_3| + ... + |a_{n-1} - a_n|`. If `n` is 0 or 1, the function should return 0. The integers can be negative, and the sum may exceed the range of a 32-bit integer, so use a 64-bit type for the result. The function must handle arbitrarily large `n` efficiently without using extra storage for the entire sequence.
#include <sstream>
#include <cassert>

int main() {
    // Test 1: Basic positive sequence
    std::istringstream in1("5 1 2 3 4 5");
    assert(sumAbsoluteConsecutiveDifferences(in1) == 4);

    // Test 2: Negative numbers
    std::istringstream in2("4 -5 -1 -10 0");
    // Differences: |-5 - (-1)| = 4, |-1 - (-10)| = 9, |-10 - 0| = 10 => total = 23
    assert(sumAbsoluteConsecutiveDifferences(in2) == 23);

    // Test 3: n = 0, no integers follow
    std::istringstream in3("0");
    assert(sumAbsoluteConsecutiveDifferences(in3) == 0);

    // Test 4: n = 1, single integer should be ignored
    std::istringstream in4("1 42");
    assert(sumAbsoluteConsecutiveDifferences(in4) == 0);

    // Test 5: n = 2, simple difference
    std::istringstream in5("2 7 -3");
    assert(sumAbsoluteConsecutiveDifferences(in5) == 10);

    // Test 6: Large values causing overflow in 32-bit
    std::istringstream in6("3 1000000000 -1000000000 1000000000");
    // 2000000000 + 2000000000 = 4000000000 (exceeds int)
    assert(sumAbsoluteConsecutiveDifferences(in6) == 4000000000LL);

    // Test 7: Duplicate consecutive values
    std::istringstream in7("4 5 5 5 5");
    assert(sumAbsoluteConsecutiveDifferences(in7) == 0);

    // Test 8: Mixed signs and zeros
    std::istringstream in8("6 -2 0 3 -1 0 4");
    // | -2-0 |=2, |0-3|=3, |3-(-1)|=4, |-1-0|=1, |0-4|=4 => total=14
    assert(sumAbsoluteConsecutiveDifferences(in8) == 14);

    // Test 9: Multiple lines (stream handles whitespace naturally)
    std::istringstream in9("4\n10\n20\n30\n40");
    assert(sumAbsoluteConsecutiveDifferences(in9) == 30);

    // Test 10: n = 1 with extra whitespace after integer
    std::istringstream in10("1   99   ");
    assert(sumAbsoluteConsecutiveDifferences(in10) == 0);

    return 0;
}
#include <cstdlib>
#include <iostream>
#include <cstdint>

// Read n and then n integers from the input stream, returning the sum of
// absolute differences between consecutive integers. Returns 0 if n <= 1.
long long sumAbsoluteConsecutiveDifferences(std::istream& input) {
    int n;
    input >> n;

    if (n <= 1) {
        // If n == 1, we still need to consume that integer from the stream.
        if (n == 1) {
            int dummy;
            input >> dummy;
        }
        return 0LL;
    }

    long long total = 0LL;
    int prev;
    input >> prev; // first integer

    for (int i = 2; i <= n; ++i) {
        int curr;
        input >> curr;
        total += std::llabs(static_cast<long long>(curr) - static_cast<long long>(prev));
        prev = curr;
    }

    return total;
}
// The main idea is to read the integers one by one and process them on the fly, keeping track of the previous integer to compute the consecutive difference. Since we only need adjacent pairs, we only need to store the previously read integer and the current sum. We read `n` first, then if `n` is 0 or 1 we can immediately return 0 (after possibly reading the single integer if `n == 1`, but we can just skip it). For `n >= 2`, we read the first integer as `prev`, then for each of the remaining `i = 2` to `n`, we read `curr`, add `abs(curr - prev)` to the sum using `long long`, and then set `prev = curr`. Edge cases include negative values (absolute value handles them) and large `n` where the sum might exceed `int` (using `long long` solves this). Time complexity is O(n) because we process each integer exactly once. Space complexity is O(1) auxiliary space (we don't store the sequence). The function can be designed to read from `std::istream` (like `std::cin`) rather than a container to be efficient and mimic the input format.
