// Given an integer `n` and a modulus `d`, followed by a sequence of `n` integers, write a C++ function `long long minimalAdjustmentCost(int n, int d, const std::vector<int>& values)` that simulates a circular dial with positions modulo `d`. Starting from the first value, for each consecutive pair of values `a` and `b`, you must rotate the dial (which wraps around from `d-1` to `0`) from the previous value to the current value. The cost to move from one number to another is the minimum number of steps on the circle; i.e., for two residues `r1` and `r2` modulo `d`, the cost is `min((r2 - r1 + d) % d, (d - (r2 - r1 + d) % d) % d)`. However, there is one twist: you are allowed to "adjust" the current input value before computing the cost, by changing it modulo `d` to any residue, but the adjustment is free; the only thing that matters is the distance on the circle from the previous (adjusted) residue to the current (adjusted) residue. In effect, when reading each value, you may choose any residue congruent to the input value modulo `d` (only one choice exists) and you must compute the cost between consecutive residues, but you also have the option to "reset" the position after the first element: specifically, after reading the first value, you set your current position to that value modulo `d`. For each subsequent value `x`, compute the direct circular distance between the previous residue and `x % d`, then add that distance to the total answer, and update the previous residue to `x % d`. The task is to return the total minimal cost, which, due to the constraint that each `x` is fixed modulo `d`, is simply the sum of those circular distances. The input may contain negative numbers, and `d` is positive (ensure your modulo handles negatives correctly). For example, with `d=10`, moving from `8` to `2` costs `min((2-8+10)%10=4, (10-4)=6) = 4`; moving from `2` to `7` costs `min(5,5)=5`. Edge cases include `d=1` (all costs are 0, since all residues are 0), a single element (cost 0), and large `n` up to 1e6.

#include <cassert>
#include <vector>

// The solution function is declared above. This test checks correctness via asserts.
int main() {
    // Example from the description: d=10, sequence 8,2 => cost 4
    assert(minimalAdjustmentCost(2, 10, {8, 2}) == 4);
    // d=10, 2->7 cost 5
    assert(minimalAdjustmentCost(2, 10, {2, 7}) == 5);
    // Single element, cost 0
    assert(minimalAdjustmentCost(1, 10, {5}) == 0);
    // d=1, any sequence cost 0
    assert(minimalAdjustmentCost(4, 1, {100, -200, 3, -7}) == 0);
    // Negative values: normalize residues. d=5, -1 -> 4, 3 -> 3, distance min(4,1)=1? Actually (3-4+5)=4, backward=1 => cost 1
    assert(minimalAdjustmentCost(2, 5, {-1, 3}) == 1);
    // A longer sequence: d=6, values 0,3,0 => distances: 0->3 cost 3, 3->0 cost 3, total 6
    assert(minimalAdjustmentCost(3, 6, {0, 3, 0}) == 6);
    // Zero distance when same residue: d=7, 4, -3 (which is residue 4), cost 0
    assert(minimalAdjustmentCost(2, 7, {4, -3}) == 0);
    // Large d, small sequence: d=100, 99, 1 => forward=(1-99+100)=2, backward=98 => cost 2
    assert(minimalAdjustmentCost(2, 100, {99, 1}) == 2);
    // n=0 edge (though not typical): return 0
    assert(minimalAdjustmentCost(0, 10, {}) == 0);
    // Random small check: d=8, sequence 6,2,7 => 6->2: forward=4,back=4 cost4; 2->7: forward=5,back=3 cost3; total7
    assert(minimalAdjustmentCost(3, 8, {6, 2, 7}) == 7);
    return 0;
}

#include <vector>
#include <cstdlib>

// Compute the minimum total cost to move a dial around a circle of size d,
// visiting residues of each value in sequence. The cost between two residues
// is the shorter arc length on the circle.
long long minimalAdjustmentCost(int n, int d, const std::vector<int>& values) {
    if (d <= 0 || n == 0) return 0;

    long long totalCost = 0;
    int prev = -1; // will be set on first element

    for (int i = 0; i < n; ++i) {
        // Normalize to a non-negative residue in [0, d-1]
        int curr = ((values[i] % d) + d) % d;

        if (i == 0) {
            prev = curr;
            continue;
        }

        // Circular distance from prev to curr
        int forward = (curr - prev + d) % d;
        int backward = (d - forward) % d; // for forward=0, d% d=0
        totalCost += (forward < backward) ? forward : backward;

        prev = curr;
    }

    return totalCost;
}

// The problem reduces to computing the shortest distance on a circle of circumference `d` between consecutive numbers' residues. The key is to handle modulo correctly for negative inputs: in C++, the `%` operator can yield a negative remainder, so we normalize the residue of each value `x` to be in `[0, d-1]` using `((x % d) + d) % d`. Then, for each consecutive pair of residues `prev` and `curr`, the circular distance is `min((curr - prev + d) % d, (d - (curr - prev + d) % d) % d)`. Note that the second term is always `d - first_term` but modulo `d`; when `first_term` is 0, the second is also 0 (since `d % d = 0`), so the formula works. For `d=1`, both terms evaluate to 0 because `(curr - prev + 1) % 1 = 0` and `(1 - 0) % 1 = 0`. We do not need any dynamic programming because each value's residue is fixed modulo `d`; there is no choice. The first value only sets the initial position, and the total cost is the sum of all circular distances between consecutive residues. Time complexity is `O(n)` with `O(1)` extra space (we can process on the fly without storing the vector, but since the function signature takes a vector, we iterate over it). Edge cases: `n=0` should return 0 (though problem likely guarantees `n>=1`), `d` can be large but within `int` range; we use `long long` for the answer to avoid overflow (max sum is about `n * (d/2)`, which could exceed `int` for `n=1e6` and `d=2e9`). Also, the original code has a bug: it incorrectly updates `las` to `x` after computing `x%d`, but it actually redeclares `x` each loop; the correct logic is to store the normalized residue of the current read value and use it as `prev` for the next iteration.
