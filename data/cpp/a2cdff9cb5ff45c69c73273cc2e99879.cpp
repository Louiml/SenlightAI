// Write a C++ function that takes two integers `a` and `b` (with `1 ≤ a ≤ b`), constructs a sequence defined as follows: start with a list, for each integer `i` from 1 to `b`, append `i` repeated `i` times (i.e., 1,2,2,3,3,3,4,4,4,4,...), but stop appending as soon as the list length reaches `b` (any extra values beyond `b` are ignored). The function should return the sum of the elements from index `a-1` to the end of this truncated list (i.e., the sum of all elements from the `a`-th position to the last element). If `a > b`, return 0. The function must be named `sumSegment` and take parameters `a` and `b` by value, returning an `int`. Assume the input is valid: `a` and `b` are positive integers within the range `[1, 10^5]`, and the result fits in a 32-bit signed integer.
// The key is to construct the truncated list efficiently. The naive approach of building a vector and pushing elements one by one works but is `O(b)` time and `O(b)` space. Since `b` can be up to 100,000, this is acceptable. The algorithm: initialize an empty vector. For `i` from 1 to `b` inclusive, for `j` from 0 to `i-1` inclusive, if the vector size is already `b`, break out of the inner loop (and essentially stop appending any more). Otherwise, push `i` into the vector. After building the vector, iterate from index `a-1` to `size-1` summing the values. Edge cases: (1) If `a > b`, return 0 because the requested starting index is beyond the list length. (2) If `a == 1`, the sum covers the entire list. (3) The inner loop may attempt to push more than `b` elements; the `if (vt.size() >= b) continue` skips pushing but does not break, so we need to ensure we break out of both loops once the vector is full to avoid wasting iterations. In the reference solution, we use a `break` in the inner loop and a flag or check in the outer loop. Complexity: The loop runs until the vector size reaches `b`; the total number of iterations is roughly the sum of `i` for `i` up to about `sqrt(2b)`, which is `O(sqrt(b))` iterations, but each push is O(1), so building the vector is `O(sqrt(b))` time in practice, but worst-case if we don't break early it could be `O(b^2)`. To be safe, we break early: once the vector is full, stop all loops. The final summation is `O(b)` in the worst case. Total time `O(b)` worst-case, `O(sqrt(b))` best-case, space `O(b)`.
#include <vector>

// Sum elements from index (a-1) to end of the truncated sequence 1,2,2,3,3,3,...
// The sequence is built by appending i repeated i times, stopping when length reaches b.
// Returns 0 if a > b.
int sumSegment(int a, int b) {
    if (a > b) return 0;

    std::vector<int> seq;
    seq.reserve(b); // optional, avoids reallocations

    bool full = false;
    for (int i = 1; i <= b && !full; ++i) {
        for (int j = 0; j < i; ++j) {
            if (static_cast<int>(seq.size()) >= b) {
                full = true;
                break;
            }
            seq.push_back(i);
        }
    }

    int total = 0;
    for (int idx = a - 1; idx < static_cast<int>(seq.size()); ++idx) {
        total += seq[idx];
    }
    return total;
}
#include <cassert>

int main() {
    // Example from original code: a=3, b=7 -> sequence: 1,2,2,3,3,3,4,4,4,4,5,5,5,5,5
    // Truncated to length 7: 1,2,2,3,3,3,4 -> sum from index 2 (0-based) to end: 2+3+3+3+4 = 15
    assert(sumSegment(3, 7) == 15);

    // a=1, b=1 -> sequence: [1] -> sum = 1
    assert(sumSegment(1, 1) == 1);

    // a=1, b=5 -> sequence: 1,2,2,3,3 -> sum = 11
    assert(sumSegment(1, 5) == 11);

    // a=5, b=5 -> sequence: 1,2,2,3,3 -> sum from index 4 (last element) = 3
    assert(sumSegment(5, 5) == 3);

    // a > b -> returns 0
    assert(sumSegment(6, 5) == 0);

    // a=4, b=4 -> sequence: 1,2,2,3 -> sum from index 3 = 3
    assert(sumSegment(4, 4) == 3);

    // a=2, b=3 -> sequence: 1,2,2 -> sum from index 1 = 2+2 = 4
    assert(sumSegment(2, 3) == 4);

    // a=1, b=2 -> sequence: 1,2 -> sum = 3
    assert(sumSegment(1, 2) == 3);

    // Edge: a=b=10^5 (large) – just ensure it runs without crash, we don't assert value
    int large = sumSegment(1, 100000);
    assert(large > 0); // sanity check

    // Another check: a=2, b=10 -> sequence length 10: 1,2,2,3,3,3,4,4,4,4
    // sum from index 1 to end: 2+2+3+3+3+4+4+4+4 = 29
    assert(sumSegment(2, 10) == 29);
}
