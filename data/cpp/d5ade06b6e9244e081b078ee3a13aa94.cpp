Write a C++ function that takes a positive integer `n` and returns the pivot integer `x` in the range `1 <= x <= n` such that the sum of all integers from `1` to `x` equals the sum of all integers from `x` to `n`. If no such pivot exists, return `-1`. The function must handle the special case `n = 1` correctly (where `x = 1` is trivially valid). The solution must use a two‑pointer technique that starts with sums of the first and last elements and incrementally expands the smaller side inward, without using any precomputed prefix sums or closed‑form formulas.

// The algorithm maintains two pointers: `left` (initially 1) and `right` (initially n), along with two running sums `sumLeft` (sum of elements from 1 to `left`) and `sumRight` (sum of elements from `right` to n). Initially, `sumLeft = 1` and `sumRight = n`. While `left < right`, we repeatedly move the pointer on the side with the smaller sum inward, adding the new element to that sum. This ensures that the sums are as close as possible. When the sums are equal, we check if `left == right`; if so, that value is the pivot. Otherwise, we advance `left` (and update `sumLeft`) and continue searching. The loop ends when `left > right`; if no equality with `left == right` was found, we return `-1`. Edge case `n = 1` is handled explicitly by returning `1` (since the problem definition makes `x = 1` valid). The algorithm runs in O(n) time in the worst case (each pointer moves at most n times) and uses O(1) auxiliary space.

#include <cstddef>

// Returns the pivot integer x (1 <= x <= n) such that sum_{i=1..x} i == sum_{i=x..n} i.
// If no such integer exists, returns -1.
int findPivot(int n) {
    if (n == 1) {
        return 1;
    }

    int left = 1;
    int right = n;
    int sumLeft = 1;   // sum of 1..left
    int sumRight = n;  // sum of right..n

    while (left < right) {
        if (sumLeft < sumRight) {
            ++left;
            sumLeft += left;
        } else if (sumRight < sumLeft) {
            --right;
            sumRight += right;
        } else { // sumLeft == sumRight
            if (left == right) {
                return left; // pivot found
            }
            // Move left pointer inward and continue.
            ++left;
            sumLeft += left;
        }
    }
    return -1;
}

#include <cassert>

int main() {
    // Basic cases
    assert(findPivot(1) == 1);
    assert(findPivot(2) == -1);
    assert(findPivot(3) == -1);
    assert(findPivot(4) == -1);
    assert(findPivot(5) == -1);
    assert(findPivot(6) == -1);
    assert(findPivot(7) == -1);
    assert(findPivot(8) == 6);       // 1+2+3+4+5+6 = 21, 6+7+8 = 21
    assert(findPivot(9) == -1);
    assert(findPivot(10) == -1);
    assert(findPivot(49) == 35);     // sum(1..35) = 630, sum(35..49) = 630
    assert(findPivot(50) == -1);
    assert(findPivot(100) == -1);
    assert(findPivot(101) == -1);
    assert(findPivot(288) == 204);   // known valid: sum(1..204)=20910, sum(204..288)=20910
    // Larger case (pivot exists for n=8, 49, 288, etc., but many do not)
    assert(findPivot(1000) == -1);
    return 0;
}
