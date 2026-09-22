// Write a C++ function `int longestFibonacciSubsequence(const std::vector<int>& arr)` that, given a strictly increasing array of positive integers `arr` of length `n` (with `1 ≤ n ≤ 1000` and each element up to `10^9`), returns the length of the longest subsequence (not necessarily contiguous) that forms a Fibonacci-like sequence. A Fibonacci-like sequence is defined as a sequence `x1, x2, x3, ...` where for every `i ≥ 3`, `xi = x_{i-1} + x_{i-2}`, and the sequence must have at least 3 elements. If no such subsequence exists of length ≥ 3, return `0`. The function must not modify the input and should be efficient enough for the constraints.
The provided solution uses a set-based greedy expansion approach. For each pair of indices `(start, next)` with `start < next`, treat `arr[start]` and `arr[next]` as the first two elements of a Fibonacci-like sequence. Then repeatedly compute the next value as the sum of the last two known values. Use a hash set (`unordered_set`) initialized with all array elements to check in `O(1)` whether the next value exists. If it does, extend the sequence length and continue; otherwise, stop. For each starting pair, we update the maximum length found. Since the array is strictly increasing, every valid Fibonacci-like subsequence will be detected when we consider its first two elements in that order (the smallest two indices). Edge cases include sequences of length exactly 3, sequences where the first two elements are large and sums overflow (avoid by using `long long` or careful checks), and the case where no sequence length ≥ 3 exists (return 0). Time complexity: there are O(n²) starting pairs, and for each pair, the internal while loop runs at most O(log(maxVal)) times because Fibonacci numbers grow exponentially; thus overall O(n² log M) with M = max value in arr, and O(n) auxiliary space for the set. In practice for n=1000 and max=1e9, this is very fast. The original code uses `int` for sums; in this task, since elements up to 1e9 and sequence length can grow, sums can exceed 2^31, so we must use `long long` for intermediate sums to avoid overflow.
#include <vector>
#include <unordered_set>
#include <algorithm>

// Returns the length of the longest Fibonacci-like subsequence in arr.
// A Fibonacci-like sequence has at least 3 elements, each equal to sum of previous two.
int longestFibonacciSubsequence(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    std::unordered_set<long long> values(arr.begin(), arr.end());
    int maxLen = 0;

    // Try every ordered pair (i, j) as the first two elements of a potential sequence.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long prev = arr[j];
            long long curr = static_cast<long long>(arr[i]) + arr[j];
            int len = 2;

            // Extend the sequence while the next value exists in the set.
            while (values.find(curr) != values.end()) {
                const long long next = prev + curr;
                prev = curr;
                curr = next;
                ++len;
            }

            // Only update if the sequence had at least 3 elements.
            if (len >= 3) {
                maxLen = std::max(maxLen, len);
            }
        }
    }
    return maxLen;
}
#include <cassert>
#include <vector>

// The solution function is defined above.
int main() {
    // Example from the original problem: [1,2,3,4,5,6,7,8] → longest is 1,2,3,5,8 (length 5)
    assert(longestFibonacciSubsequence({1,2,3,4,5,6,7,8}) == 5);

    // No valid sequence of length ≥3
    assert(longestFibonacciSubsequence({1,3,5}) == 0); // 1,3,5 not fib because 1+3=4 not 5

    // Exactly length 3
    assert(longestFibonacciSubsequence({1,2,3,10}) == 3); // 1,2,3

    // Larger numbers and overflow check
    assert(longestFibonacciSubsequence({1, 1000000000, 1000000001}) == 3); // 1 + 1e9 = 1000000001 (fits in long long)

    // Multiple overlapping sequences: [1,2,3,5,8] gives 1,2,3,5,8 (length 5)
    assert(longestFibonacciSubsequence({1,2,3,5,8}) == 5);

    // Single element or two elements → 0
    assert(longestFibonacciSubsequence({5}) == 0);
    assert(longestFibonacciSubsequence({5,8}) == 0);

    // Sequence not starting from smallest pairs: [2,4,6,10,16] → 2,4,6,10,16 (length 5)
    assert(longestFibonacciSubsequence({2,4,6,10,16}) == 5);

    // Duplicate values are not allowed by problem constraints but test non-strictly increasing input
    // Provided array is strictly increasing, so no duplicates expected.

    // Stress test with a long sequence: fibonacci numbers up to 10^9 (length ~45)
    std::vector<int> fibs = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269};
    // Remove duplicate 1 to make strictly increasing? But duplicate not allowed; use {1,2,3,...}
    std::vector<int> fibs2 = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040};
    assert(longestFibonacciSubsequence(fibs2) == static_cast<int>(fibs2.size())); // entire sequence

    return 0;
}
