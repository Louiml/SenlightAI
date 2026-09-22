Write a C++ function that takes a single integer `k` and returns a vector of integers representing a valid output sequence that satisfies the same structure as the given snippet: the sequence must have length 4, the first element must be `3`, the second element must be `k+1`, and the remaining two elements must be `1` and `1`. The function should return the sequence as a `std::vector<int>` in that exact order. You may assume `k` is a valid integer (positive, negative, or zero). The function should not read from or write to standard input/output; it must simply construct and return the vector.
The problem is straightforward: the given code reads `k`, then prints `3`, then prints `k+1`, then prints `1` three times but with a trailing space, which effectively outputs `3 k+1 1 1`. However, the printed line has `3` on its own line, then `k+1 1 1 1` on the next line (since the loop prints three 1's, but the snippet prints `k+1` and then three 1's, but the expected output is `3` then `k+1 1 1 1`? Actually, the code prints `3` on line 1, then line 2 has `k+1` followed by three `1`s. But the task description clarifies the sequence length is 4: first element 3, second k+1, third 1, fourth 1. This matches the snippet if we ignore the extra printed 1 (the loop runs 3 times, printing three 1's, but only the first two are meant as part of the sequence? The snippet prints: `cout << 3;` then `cout << k+1 << ' ';` then loop prints three `1` with spaces, so the second line is `k+1 1 1 1` (three ones). So the entire output is two lines: `3` and `k+1 1 1 1`. But the task describes a vector of length 4: `[3, k+1, 1, 1]`, which is a simplification. The reference solution should return exactly that vector. Edge cases: `k` can be any integer, including negative, zero, or large; `k+1` may overflow if `k` is INT_MAX, but since the task does not specify limits, we assume standard integer behavior. Time complexity is O(1) because we always create a fixed-size vector; space complexity is O(1) auxiliary (the vector itself is O(1) size). The solution simply constructs the vector with the four elements in order.
#include <vector>

// Return a sequence of four integers: {3, k+1, 1, 1}.
std::vector<int> buildSequence(int k) {
    return {3, k + 1, 1, 1};
}
#include <cassert>
#include <vector>

// Declaration of the function under test
std::vector<int> buildSequence(int k);

int main() {
    assert(buildSequence(0) == std::vector<int>({3, 1, 1, 1}));
    assert(buildSequence(5) == std::vector<int>({3, 6, 1, 1}));
    assert(buildSequence(-3) == std::vector<int>({3, -2, 1, 1}));
    assert(buildSequence(100) == std::vector<int>({3, 101, 1, 1}));
    assert(buildSequence(-1) == std::vector<int>({3, 0, 1, 1}));
    assert(buildSequence(2) == std::vector<int>({3, 3, 1, 1}));
    assert(buildSequence(7) == std::vector<int>({3, 8, 1, 1}));
    assert(buildSequence(-10) == std::vector<int>({3, -9, 1, 1}));
    assert(buildSequence(1) == std::vector<int>({3, 2, 1, 1}));
    assert(buildSequence(42) == std::vector<int>({3, 43, 1, 1}));
    return 0;
}
