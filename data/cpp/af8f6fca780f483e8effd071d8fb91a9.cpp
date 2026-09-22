// Write a C++ function `rotateConveyor` that takes a vector of integers representing the contents of a circular conveyor belt of length `2n` (the first half is the upper row, the second half is the lower row), an integer `n` (the number of elements in each row), and an integer `t` (number of seconds to rotate). The belt moves to the right by 1 position each second, but because it is circular, after `2n` seconds it returns to its initial state. The function must return a new vector of length `2n` that shows the belt state after `t` seconds, but with the printed format requirement: the first `n` elements of the returned vector correspond to the upper row, the next `n` elements to the lower row. Additionally, the function should assume that `n >= 1`, `t >= 0`, and the input vector size is exactly `2n`. The function must be side-effect-free and use only the header `<vector>` and `<cstddef>` (or standard headers as needed). The returned vector must have the same order as the input (upper first row, then lower row) after applying the rotation, and the rotation is defined by: each element at index `i` moves to index `(i + 1) % (2n)`. However, the original code prints by taking the element at index `i - (t % 2n)` for each output index `i` (with wrap-around), which is equivalent to the same rotation but from the "reading" perspective. Your function should produce the same output as the original code: for each position `i` in the output, the value is `conv[(i - turns + m) % m]` where `m = 2n` and `turns = t % m`. The function should return a new vector, not modify the input, and must handle large `t` efficiently without simulating every second.

// The core observation is that the belt is circular with period `m = 2n`, so we only need to consider `turns = t % m` because after `m` seconds the belt repeats. The original code prints each output index `i` by fetching `conv[(i - turns + m) % m]`, which effectively rotates the belt to the right by `turns` positions. To build the result vector, we iterate over each output index `i` from 0 to `m-1`, compute the source index `src = (i - turns + m) % m` (the addition of `m` before modulo ensures non-negative), and push `input[src]` into the result. Edge cases: when `t` is a multiple of `m` (including `t=0` or `t = m, 2m, ...`), `turns = 0` and the result equals the input unchanged. When `turns` is large, modulo handles it by reducing to a small number. The loop runs in `O(m)` time and uses `O(m)` extra space for the result vector (not counting the input). Since `m = 2n`, this is `O(n)` time and space. The solution avoids any simulation and directly computes the mapping.

#include <vector>

// Rotate a circular conveyor belt of length 2*n (first n upper, next n lower) to the right by t seconds.
// Returns a new vector with the state after t seconds.
// Precondition: input.size() == 2*n, n >= 1, t >= 0.
std::vector<int> rotateConveyor(const std::vector<int>& input, int n, int t) {
    const int m = 2 * n;           // total number of elements
    const int turns = t % m;       // effective rotation offset (0 to m-1)
    
    std::vector<int> result(m);
    
    for (int i = 0; i < m; ++i) {
        // Source index: output position i reads from (i - turns) mod m
        int src = (i - turns + m) % m;
        result[i] = input[src];
    }
    
    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// Test the rotateConveyor function with various cases.
int main() {
    // Basic rotation with n=2, m=4, t=1
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> r1 = rotateConveyor(v1, 2, 1);
    assert((r1 == std::vector<int>{4, 1, 2, 3})); // upper row: 4,1; lower row: 2,3

    // t=0 returns same order
    std::vector<int> r2 = rotateConveyor(v1, 2, 0);
    assert((r2 == v1));

    // t equals period m=4 returns same order
    std::vector<int> r3 = rotateConveyor(v1, 2, 4);
    assert((r3 == v1));

    // t greater than period: t=5 equivalent to t=1
    std::vector<int> r4 = rotateConveyor(v1, 2, 5);
    assert((r4 == r1));

    // n=1, m=2, t=1 swaps the two elements
    std::vector<int> v2 = {7, 9};
    std::vector<int> r5 = rotateConveyor(v2, 1, 1);
    assert((r5 == std::vector<int>{9, 7}));

    // n=3, m=6, t=2
    std::vector<int> v3 = {10, 20, 30, 40, 50, 60};
    std::vector<int> r6 = rotateConveyor(v3, 3, 2);
    // Expected: output[i] = input[(i - 2 + 6) % 6]
    // i=0: src=4 -> 50; i=1: src=5 -> 60; i=2: src=0 -> 10; i=3: src=1 -> 20; i=4: src=2 -> 30; i=5: src=3 -> 40
    assert((r6 == std::vector<int>{50, 60, 10, 20, 30, 40}));

    // t = m*10 (multiple of period) returns same
    std::vector<int> r7 = rotateConveyor(v3, 3, 60);
    assert((r7 == v3));

    // All elements identical: any rotation returns same
    std::vector<int> v4 = {5, 5, 5, 5};
    std::vector<int> r8 = rotateConveyor(v4, 2, 3);
    assert((r8 == v4));

    return 0;
}
