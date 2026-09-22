// Write a C++ function that takes four integers `a`, `b`, `c`, and `n` as input, representing: `a` students who solved problem A, `b` students who solved problem B, `c` students who solved both problems A and B, and `n` total students who solved at least one problem. The function should return the number of students who solved neither problem A nor B, or return `-1` if the given data is logically inconsistent (e.g., `c` cannot exceed `a` or `b`, or the computed answer is non-positive). The function must be pure and not use any global variables.

// The solution is based on the principle of inclusion–exclusion. The total number of students who solved at least one problem is `(a + b - c)` because adding `a` and `b` counts the `c` students twice, so we subtract `c` once. The number of students who solved neither problem is then `n` minus that total, i.e., `f = n - (a + b - c)`.  
// The function must return `-1` if the data is inconsistent: first, if `c` is larger than either `a` or `b` (because you cannot have more students solving both than solving each individually), and second, if the computed `f` is less than or equal to zero (since a negative or zero count of "neither" is impossible if there are `n` total students and `n` represents all students—however, note that `n` is given as the number who solved at least one problem, so `f` should represent students who solved none; if `f <= 0`, no students solved none, which is invalid because `n` typically represents all students in the class, but per the problem logic, if the data leads to a non-positive count, it is considered inconsistent).  
// Edge cases: `c` can be zero (no overlap) or equal to `a` or `b`. Also ensure arithmetic overflow is not an issue since typical integer range suffices.  
// Time complexity: O(1) — constant number of operations. Space complexity: O(1) — only a few integer variables.

#include <cstdint>

// Given:
//   a = number of students who solved problem A
//   b = number of students who solved problem B
//   c = number of students who solved both A and B
//   n = total number of students (including those who solved at least one)
// Returns the number of students who solved neither, or -1 if inconsistent.
int neitherCount(int a, int b, int c, int n) {
    if (c > a || c > b) {
        return -1;
    }
    const int solvedAtLeastOne = a + b - c;
    const int neither = n - solvedAtLeastOne;
    if (neither <= 0) {
        return -1;
    }
    return neither;
}

#include <cassert>

int neitherCount(int a, int b, int c, int n);

int main() {
    // Basic valid case: a=10, b=5, c=3, n=15 -> neither = 15 - (10+5-3) = 3
    assert(neitherCount(10, 5, 3, 15) == 3);
    // No overlap: a=7, b=6, c=0, n=20 -> neither = 20 - 13 = 7
    assert(neitherCount(7, 6, 0, 20) == 7);
    // All solve both: a=8, b=8, c=8, n=8 -> neither = 8 - 8 = 0 -> invalid
    assert(neitherCount(8, 8, 8, 8) == -1);
    // c exceeds a
    assert(neitherCount(3, 5, 4, 12) == -1);
    // c exceeds b
    assert(neitherCount(6, 2, 3, 9) == -1);
    // Exactly one student solves neither: a=4, b=4, c=2, n=6 -> neither = 6 - 6 = 0 -> invalid (must be >0)
    assert(neitherCount(4, 4, 2, 6) == -1);
    // Large values
    assert(neitherCount(1000, 2000, 500, 3000) == 500);
    // c equal to a, but b larger: a=5, b=10, c=5, n=12 -> neither = 12 - 10 = 2
    assert(neitherCount(5, 10, 5, 12) == 2);
    // Edge: a=1, b=1, c=1, n=2 -> neither = 0 -> invalid
    assert(neitherCount(1, 1, 1, 2) == -1);
    // Valid but small positive result: a=2, b=1, c=1, n=3 -> neither = 1
    assert(neitherCount(2, 1, 1, 3) == 1);
    return 0;
}
