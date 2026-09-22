/*
Write a C++ function that takes a reference to a fixed-size vector of four integers (simulated with `std::array<int,4>` or a custom struct) and modifies it in place: first, determine the sum of the elements at odd indices (1 and 3), then set the elements at indices 2 and 3 to that sum (overwriting the original values), while leaving indices 0 and 1 unchanged. The function should return nothing (void) and must be `const`-correct on inputs it does not mutate. The input vector will always contain exactly four integers; handle negative values and zeros naturally.
*/

#include <array>

// Modify a fixed-size vector of 4 integers in place.
// Sets elements at indices 2 and 3 to the sum of elements at indices 1 and 3.
void setSegmentToSum(std::array<int,4>& v) {
    const int sum = v[1] + v[3]; // Save sum before overwriting v[3]
    v[2] = sum;
    v[3] = sum;
}

#include <cassert>
#include <array>

int main() {
    // Basic case: positive values
    std::array<int,4> a = {1, 2, 3, 4};
    setSegmentToSum(a);
    assert((a == std::array<int,4>{1, 2, 6, 6}));

    // Negative values
    std::array<int,4> b = {-1, -2, 3, -5};
    setSegmentToSum(b);
    assert((b == std::array<int,4>{-1, -2, -7, -7}));

    // Zeros
    std::array<int,4> c = {0, 0, 0, 0};
    setSegmentToSum(c);
    assert((c == std::array<int,4>{0, 0, 0, 0}));

    // Mixed values where v[3] is zero
    std::array<int,4> d = {5, 10, 100, 0};
    setSegmentToSum(d);
    assert((d == std::array<int,4>{5, 10, 10, 10}));

    // Values with all same
    std::array<int,4> e = {7, 7, 7, 7};
    setSegmentToSum(e);
    assert((e == std::array<int,4>{7, 7, 14, 14}));

    return 0;
}

// The solution must compute `sum = v[1] + v[3]` before modifying any element, because setting `v[3]` to `sum` would corrupt the original value needed for the calculation if done in sequence. Therefore, read both indices, compute the sum, then assign `v[2] = sum` and `v[3] = sum`. Edge cases: if `v[3]` is negative, the sum could be negative; if values are zero, sum is zero—both handled naturally. The operation is in-place, no extra storage beyond a local integer. Time complexity is O(1) (fixed four elements), space complexity O(1) auxiliary.
