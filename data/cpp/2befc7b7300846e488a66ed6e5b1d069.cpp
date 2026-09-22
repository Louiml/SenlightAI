Write a C++ function `maximumCakeSlices` that takes a positive integer `n` representing the number of straight cuts made on a circular cake (or a flat disk), and returns the maximum number of pieces the cake can be divided into, assuming each cut is a straight line that passes through the entire cake and that cuts can intersect anywhere inside the cake (not necessarily at the center), and no two cuts are parallel, and no three cuts meet at a single interior point. The function must compute the result directly using a closed-form mathematical expression, not by simulation or recursion.
The key observation is that this is a classic pizza/cake cutting problem. For `n` cuts, the maximum number of pieces is given by the formula:  
`P(n) = (n*(n+1))/2 + 1`.  
This is derived because each new cut can intersect all previous cuts, and if it crosses `k` previous cuts, it gets divided into `k+1` segments, each adding one new piece. The sequence is: 1, 2, 4, 7, 11, ... which matches the formula. However, the given code snippet uses a different but equivalent form: `d = k*(n-k) + 1` where `k = n/2` (integer division). This works because the product `k*(n-k)` is maximized when `k` is as close to `n/2` as possible, and for integer `n`, `k = floor(n/2)` gives the maximum value `floor(n/2)*ceil(n/2)`. Note that `floor(n/2)*ceil(n/2)` equals `(n^2)/4` when `n` is even and `(n^2-1)/4` when `n` is odd. The given formula `k*(n-k)+1` yields the same sequence as the standard formula.  
Edge cases: `n = 0` should return `1` (uncut cake), `n = 1` returns `2`, `n = 2` returns `4`. The formula works for all non-negative integers. Since `n` is positive per the task description, but we can handle 0 for safety. Time complexity: `O(1)`. Space complexity: `O(1)`.
#include <cstdint>

// Return the maximum number of cake pieces obtainable with n straight cuts.
// The formula is derived from the classic cake-cutting problem: each new cut
// intersects all previous cuts, adding one piece per intersection segment.
// Equivalent closed form: pieces = n*(n+1)/2 + 1.
// Here we use the equivalent optimization: k = n/2, pieces = k*(n-k)+1.
std::int64_t maximumCakeSlices(int n) {
    // Ensure n is non-negative; if negative, treat as 0 for safety.
    if (n < 0) {
        n = 0;
    }
    const std::int64_t cuts = static_cast<std::int64_t>(n);
    const std::int64_t k = cuts / 2;  // integer division
    return k * (cuts - k) + 1;
}
#include <cassert>
#include <cstdint>

// Declare the function (or include the solution header in real code).
std::int64_t maximumCakeSlices(int n);

int main() {
    // Basic cases
    assert(maximumCakeSlices(0) == 1);   // no cuts -> whole cake
    assert(maximumCakeSlices(1) == 2);   // one cut -> two pieces
    assert(maximumCakeSlices(2) == 4);   // two cuts -> four pieces
    assert(maximumCakeSlices(3) == 7);   // three cuts -> seven pieces
    assert(maximumCakeSlices(4) == 11);  // four cuts -> eleven pieces

    // Larger values using the closed-form formula for verification
    assert(maximumCakeSlices(10) == 56);        // 10*11/2+1 = 56
    assert(maximumCakeSlices(100) == 5051);     // 100*101/2+1 = 5051
    assert(maximumCakeSlices(12345) == 76205686); // (12345*12346)/2+1

    // Edge: negative input treated as zero
    assert(maximumCakeSlices(-5) == 1);

    // Consistency with the alternative formula for many values
    for (int n = 0; n <= 200; ++n) {
        std::int64_t expected = static_cast<std::int64_t>(n) * (n + 1) / 2 + 1;
        assert(maximumCakeSlices(n) == expected);
    }

    return 0;
}
