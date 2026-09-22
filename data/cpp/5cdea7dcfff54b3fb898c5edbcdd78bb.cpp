/*
Write a C++ function `double computeFinalVelocity(int initialIndex, int dropTime, const std::vector<int>& heights, int capacitance)` that models the following physics scenario: There are `n` objects, each with a known height `A[i]` from which it is released from rest. Gravity is taken as `1` (unit acceleration), and the system has a capacitance-like constant `C` that influences time conversion. For each query, you are given a drop time `k2` (additional seconds to observe) and an object index `k1` (1-based) among the objects after their heights have been sorted in non-decreasing order. The object first falls from its sorted height, then after that fall is complete, it continues falling for an additional `k2` seconds starting from rest at a height of `0` (i.e., effectively a free-fall from rest for total time `t = (A[k1]^2)/(2*C) + k2`). The final speed is `sqrt(2*C*t)`. Your function should return that speed as a double. Assume `C > 0`, heights are positive integers, and all inputs fit within typical int ranges but the squared height may overflow int, so use 64-bit arithmetic.
*/

#include <vector>
#include <cmath>

// Precondition: `heights` is already sorted in non-decreasing order.
// `index` is 1-based into that sorted list.
// `dropTime` is the extra falling seconds after the initial fall.
// `capacitance` is the positive constant C.
double computeFinalVelocity(int index, int dropTime,
                            const std::vector<int>& heights,
                            int capacitance) {
    // Convert to 1-based access; assume valid index.
    long long h = heights[index - 1];
    long long C = capacitance;
    long long extra = dropTime;
    // Final speed = sqrt(h^2 + 2*C*extra) as derived.
    return std::sqrt(static_cast<double>(h * h + 2LL * C * extra));
}

#include <cassert>
#include <cmath>
#include <vector>

// Function prototype (defined above, but for testing we include its code or declare)
double computeFinalVelocity(int, int, const std::vector<int>&, int);

int main() {
    // Sorted heights {1, 2, 3, 4, 5}
    std::vector<int> h = {1, 2, 3, 4, 5};
    int C = 2;

    // Check a few cases using exact arithmetic (sqrt of perfect squares)
    // index=1, h=1, dropTime=0 -> sqrt(1^2 + 0) = 1.0
    assert(std::fabs(computeFinalVelocity(1, 0, h, C) - 1.0) < 1e-9);
    // index=2, h=2, dropTime=0 -> sqrt(4) = 2.0
    assert(std::fabs(computeFinalVelocity(2, 0, h, C) - 2.0) < 1e-9);
    // index=3, h=3, dropTime=0 -> sqrt(9) = 3.0
    assert(std::fabs(computeFinalVelocity(3, 0, h, C) - 3.0) < 1e-9);
    // index=5, h=5, dropTime=0 -> sqrt(25) = 5.0
    assert(std::fabs(computeFinalVelocity(5, 0, h, C) - 5.0) < 1e-9);

    // With dropTime=2, C=2: for h=3 -> sqrt(9 + 8) = sqrt(17) ≈ 4.1231
    double res = computeFinalVelocity(3, 2, h, C);
    assert(std::fabs(res - std::sqrt(17.0)) < 1e-9);

    // h=4, dropTime=1, C=3 -> sqrt(16 + 6) = sqrt(22)
    res = computeFinalVelocity(4, 1, h, 3);
    assert(std::fabs(res - std::sqrt(22.0)) < 1e-9);

    // Edge: large values, check no overflow and correct result
    std::vector<int> big = {100000};
    res = computeFinalVelocity(1, 100000, big, 100000);
    // h^2 = 1e10, 2*C*extra = 2*1e5*1e5=2e10, sum=3e10
    assert(std::fabs(res - std::sqrt(30000000000.0)) < 1e-6);

    // h=0 (allowed? heights positive per spec, but test zero just in case)
    std::vector<int> zero = {0};
    assert(std::fabs(computeFinalVelocity(1, 0, zero, 1) - 0.0) < 1e-9);
    // dropTime nonzero with h=0
    assert(std::fabs(computeFinalVelocity(1, 2, zero, 1) - 2.0) < 1e-9);

    // Multiple identical heights
    std::vector<int> dup = {1, 1, 2, 2};
    assert(std::fabs(computeFinalVelocity(2, 0, dup, 1) - 1.0) < 1e-9);
    assert(std::fabs(computeFinalVelocity(4, 0, dup, 1) - 2.0) < 1e-9);

    return 0;
}

// The key is to interpret the input correctly. The original code reads `n`, `C`, then for each of `n` objects reads a height `A[i]` and two dummy integers `k2` and `k3` that are ignored. Then it sorts the heights. For each of `m` queries, it reads a dummy `k2` and then an index `k1`. It computes `t = (long long) A[k1] * A[k1] / (2.0 * C)` which is the time the object takes to fall from height `A[k1]` under gravity 1 (since `distance = 0.5 * g * t^2` with `g=1` gives `t = sqrt(2*distance)` but they compute `t = distance / C`? Actually the snippet uses `t = A[k1]^2/(2*C)` and then `ans = sqrt(2*C*(t+k2))`. Notice that if we define `C` as a scaling constant such that the time to fall height `h` is `h/(2C)`? That seems odd, but we must follow the given formula exactly. So the final speed is `sqrt(2*C*((A[k1]^2)/(2*C) + k2)) = sqrt(A[k1]^2 + 2*C*k2)`. That simplifies! Indeed, `sqrt(2*C*(A^2/(2C) + k2)) = sqrt(A^2 + 2*C*k2)`. So the function simply returns `sqrt((double)A[k1]*A[k1] + 2.0 * C * k2)`. The sorting is important because after reading all heights, we sort them, so the index `k1` refers to the sorted order. Edge cases: use `long long` for `A^2` to avoid overflow; `C` and `k2` can be up to 10^5 or so, but the product fits in `double`. Time complexity: sorting takes O(n log n), each query O(1). Space: O(n) for the vector. The function should take a vector of heights, sort it internally (or assume it's already sorted? The task says "after sorting", so we should sort inside the function for independence). But to match the snippet, we can either sort inside or accept a sorted vector. Since the task description says "objects have heights, and after sorting", I'll have the function sort a copy of the heights vector to be self-contained. That way it doesn't modify input. Complexity: O(n log n + m) where m is number of queries? But our function only handles one query at a time, so we just sort every call? That would be inefficient. However the task is to write a function that answers a single query given the original heights vector; we can sort once and then serve queries, but as a standalone function, we might sort inside. To keep it simple and correct for a single call, we can sort the vector inside, but that would be O(n log n) per query, which is not great. Instead, the function can accept a pre-sorted vector. The task says "after their heights have been sorted", so we can assume the input vector is already sorted in non-decreasing order. That matches the original code (it sorts before queries). So I'll document that precondition. Then the function does constant work.
//
// The formula simplifies to `sqrt(A^2 + 2*C*k2)`. We must use `long long` for the squares to avoid overflow, then convert to double.
