/*
Write a C++ function named `spiralValue` that takes two integer coordinates `(row, col)` and returns the integer value stored at that position in a square spiral pattern that starts at coordinates `(0, 0)` with the value `1` and proceeds outward in a clockwise direction (first moving right to `(0, 1)` with value `2`, then up to `(-1, 1)` with value `3`, then left, then down, and so on, forming concentric square rings). The function must compute the value directly using arithmetic, without constructing the grid. For example, `spiralValue(0, 0)` returns `1`, `spiralValue(0, 1)` returns `2`, `spiralValue(-1, 1)` returns `3`, and `spiralValue(1, -1)` returns `9`. The coordinates can be any integers (positive, negative, or zero), and the function must handle large coordinates by using 64-bit integers internally to avoid overflow. After completing the function, write a test program that verifies it against a known pattern for several points, including the four corners of a ring and a few interior points.
*/

#include <algorithm>  // for std::max, std::abs (actually abs for long long is in <cstdlib> or <cmath>, but <cstdlib> is fine; using <cstdlib> or just rely on <algorithm>? Better include <cstdlib> for std::llabs, or use custom abs. We'll use <cstdlib> and <algorithm>? We'll just use <cstdlib> for std::llabs and <algorithm> for std::max? Actually std::max is in <algorithm>. We'll include both.)

// Return the value at (row, col) in a clockwise square spiral starting at (0,0)=1.
long long spiralValue(long long row, long long col) {
    long long n = std::max(std::llabs(row), std::llabs(col));
    long long maxVal = (2 * n + 1) * (2 * n + 1);  // value at (n, n)
    long long cst = 2 * n;

    if (row == n) {
        // Bottom side: from (n, n) leftwards to (n, -n)
        return maxVal - (n - col);
    }
    maxVal -= cst;
    if (col == -n) {
        // Left side: from (n-1, -n) upwards to (-n, -n)
        return maxVal - (n - row);
    }
    maxVal -= cst;
    if (row == -n) {
        // Top side: from (-n, -n+1) rightwards to (-n, n)
        return maxVal - (col + n);
    }
    maxVal -= cst;
    // Right side: from (-n+1, n) downwards to (n-1, n)
    return maxVal - (row + n);
}

int main() {
    // Basic points
    assert(spiralValue(0, 0) == 1);
    assert(spiralValue(0, 1) == 2);
    assert(spiralValue(-1, 1) == 3);
    assert(spiralValue(1, 0) == 8);
    assert(spiralValue(1, 1) == 9);

    // Ring 1 corners and edges
    assert(spiralValue(1, -1) == 7);
    assert(spiralValue(0, -1) == 6);
    assert(spiralValue(-1, -1) == 5);
    assert(spiralValue(-1, 0) == 4);

    // Ring 2 points
    assert(spiralValue(1, 2) == 10);   // start of ring 2
    assert(spiralValue(2, 2) == 25);   // max of ring 2
    assert(spiralValue(-2, 2) == 13);  // top-right corner
    assert(spiralValue(-2, -2) == 17); // top-left corner
    assert(spiralValue(2, -2) == 21);  // bottom-left corner
    assert(spiralValue(2, 0) == 23);   // bottom side interior
    assert(spiralValue(0, -2) == 19);  // left side interior

    // Larger ring 3 sample
    assert(spiralValue(3, 3) == 49);   // (2*3+1)^2 = 49
    assert(spiralValue(2, 3) == 48);
    assert(spiralValue(-3, 3) == 31);
    assert(spiralValue(0, 0) == 1);    // still 1

    // Negative coordinates consistent
    assert(spiralValue(-1, 1) == 3);
    assert(spiralValue(1, -1) == 7);

    return 0;
}

// The spiral pattern is built from concentric square rings. For any point `(row, col)`, determine the ring number `n = max(abs(row), abs(col))`. The maximal value on that ring is at the outer corner `(n, n)`? Wait, careful: In the described clockwise spiral starting at `(0,0)` moving right, the ring `n` has side length `2n+1`. The largest value on ring `n` is at the top-right corner? Let's reason: For n=1, ring values from 1? No, ring 1 has values 2 to 9. The maximal value on ring n is at position `(n-1, n)`? Actually, the pattern: For n=0, value 1. For n=1, the ring: Starting from `(0,1)`=2, then up to `(-1,1)`=3, left to `(-1,0)`=4, `(-1,-1)`=5, down to `(0,-1)`=6, `(1,-1)`=7, right to `(1,0)`=8, `(1,1)`=9. So the ring n=1 has max 9 at `(1,1)`. For general n, the square ring has corners: `(-n, n)` top-left? Actually coordinate system: row is vertical (positive down?), but in the original code, `(x, y)` with `x` row, `y` col. The spiral starts at (0,0) move right (col+1), then up (row-1), left (col-1), down (row+1), etc. So ring n has four sides: top side (row = -n, col from n-1 down to -n+1?), but easier: The maximum value on ring n is at `(n, n)`? For n=1, max is 9 at (1,1) yes. For n=2, let's test: ring 2 starts after 9 at (0,2)=10? Actually after (1,1)=9, move right to (1,2)=10, then up to (0,2)=11, (-1,2)=12, (-2,2)=13, then left to (-2,1)=14, (-2,0)=15, (-2,-1)=16, (-2,-2)=17, then down to (-1,-2)=18, (0,-2)=19, (1,-2)=20, (2,-2)=21, then right to (2,-1)=22, (2,0)=23, (2,1)=24, (2,2)=25. So max 25 at (2,2). Yes, the maximum on ring n is at `(n, n)` and equals `(2n+1)^2`.
//
// Now, derive the value at any point on ring n. The ring n consists of cells with coordinates where `max(abs(row), abs(col)) == n`. There are `(2n+1)^2 - (2n-1)^2 = 8n` cells on ring n. The values on ring n range from `(2n-1)^2 + 1` to `(2n+1)^2`. We can compute the offset by traversing the ring in a consistent order. A common approach: Start from the top-right corner? Actually, based on the movement order: The ring starts at `(n-1, n)`? No, the ring n starts after the previous ring ends at `(n-1, n-1)`? Let's see: ring 1 starts at `(0,1)` which is `(n-1, n)` for n=1? For n=1, `(n-1, n) = (0,1)`, yes. For n=2, ring 2 starts at `(1,2)` which is `(n-1, n)`. So the first cell of ring n is at `(n-1, n)` with value `(2n-1)^2 + 1`. Then the ring proceeds: go up along the right side: row decreases from n-1 to -n, col = n, so cells `(n-1, n), (n-2, n), ..., (-n, n)`. That's `2n+1` cells? Actually from row n-1 down to -n inclusive: that's `(n-1) - (-n) + 1 = n-1+n+1 = 2n` cells? Let's count: rows n-1, n-2, ..., -n: that's (n-1 - (-n)) + 1 = 2n. But wait, the top-right corner? The corner at `(-n, n)` is included. Then next side: row = -n, col from n-1 down to -n: that's `(n-1) - (-n) + 1 = 2n` cells. Then third side: col = -n, row from -n+1 up to n: that's `n - (-n+1) + 1 = 2n` cells? Actually rows -n+1 to n inclusive: that's n - (-n+1) + 1 = n + n -1 +1 = 2n. Then fourth side: row = n, col from -n+1 to n-1: that's (n-1) - (-n+1) + 1 = n-1 + n -1 +1 = 2n-1? Wait, total should be 8n. Let's compute: side 1: right side excluding bottom? Actually my description is off. Better to use the original code's logic. The given snippet has `solved(x, y)` that computes exactly this using ring number `n` and then subtracts distances based on which side the point lies. We can adapt that logic.
//
// The original code: 
// ```
// int n = max(abs(x), abs(y));
// int cache = (2 * n + 1); cache *= cache; // (2n+1)^2
// int cst = 2 * n;
// if (x == n) return cache - (n - y); // bottom side? Actually x==n is bottom row, y from n-1 down to -n? Let's check: For x==n, cache is max value at (n,n). Then cache - (n-y) gives: when y=n, value = cache - 0 = max. When y decreases, value decreases. That matches bottom side going right? Actually row n is bottom row (since row positive is down?). In original code, they treat x as row, y as col. They use max(abs(x),abs(y)) so n is ring. If x == n, that means on bottom side, from (n,n) leftwards? Because y decreases from n to -n. So values: (n,n) max, (n,n-1) max-1, ..., (n,-n) max-2n. That's correct for bottom side. Then they subtract 2n and check if y==-n (left side), etc. This is a standard way.
//
// We'll implement a clean version. The algorithm: for given (r,c), let n = max(|r|,|c|). Let maxVal = (2n+1)^2. Let cst = 2n. The four sides are ordered: bottom (r == n), then left (c == -n), then top (r == -n), then right (c == n). Actually, from the max at (n,n), moving clockwise? The spiral goes right, up, left, down. At ring n, the max value is at (n,n) which is bottom-right corner? Because row increases downward? Let's define: In typical matrix, row positive goes down. The spiral starts at (0,0) and moves right (col+1), then up (row-1), so top is negative row. So (n,n) is bottom-right corner. Moving from max value, the next values go left along bottom edge to (n,-n), then up along left edge to (-n,-n), then right along top edge to (-n,n), then down along right edge to (n-1,n)? Actually after top-right corner (-n,n) then down to (n,n-1)? Wait, the original code: After bottom side (x==n), they subtract cst and check left side (y==-n). Then subtract cst and check top side (x==-n). Then subtract cst and return for right side. So the order: start from max at bottom-right corner (n,n), go left along bottom edge (y decreasing) up to bottom-left (n,-n). Then go up along left edge (x decreasing) up to top-left (-n,-n). Then go right along top edge (y increasing) up to top-right (-n,n). Then go down along right edge (x increasing) up to (n-1,n)? Actually the right edge from top-right to bottom-right? But bottom-right is max already, so the last part is from top-right down to (n, n-1)? Let's see original: if (x == n) return cache - (n - y); // bottom edge: y from n down to -n, so value decreases from max to max-2n. Then cache -= cst (now cache = max - 2n). if (y == -n) return cache - (n - x); // left edge: x from n-1 down to -n? Actually x from n-1 to -n? Because when x==n we already returned. So x ranges n-1 down to -n. For x = -n, y=-n gives value? cache - (n - (-n)) = max - 2n - 2n = max - 4n, which might be correct for the top-left corner? Let's test with n=1: max=9, cst=2. bottom edge: (1,1)->9, (1,0)->8, (1,-1)->7. Then cache=7. left edge: y==-1, x from 0 down to -1: (0,-1) -> 7 - (1-0)=6, (-1,-1) -> 7 - (1-(-1))=5. Then cache=5. top edge: x==-1, y from -1 to 1? Actually after left, cache=5. top edge: if (x==-n) return cache - (y + n) = 5 - (y+1). For y=-1: 5 - 0 =5 (already), y=0: 5-1=4, y=1: 5-2=3. Then cache=3. right edge: return cache - (x + n) = 3 - (x+1). For x from 0 to 1? But x==1 is bottom edge already. So x=0 gives 3-1=2, and that matches (0,1)=2? Wait (0,1) is right edge? Actually (0,1) is on right edge, and value 2. Yes. So this works.
//
// Thus, we can implement `spiralValue` following the same logic but using `long long` to avoid overflow.
//
// Edge cases: when n=0, i.e., (0,0): maxVal = 1, cst=0, x==n (0==0) return 1 - (0 - 0) = 1. Works. Also handle large coordinates: use long long for n and calculations. The function should be `long long spiralValue(int row, int col)` or better `long long spiralValue(long long row, long long col)`? The task says coordinates can be any integers, but to be safe use `long long` for both parameters and return. But the problem typically uses int for coordinates, but values can be huge (for n up to 100000, (2n+1)^2 ~ 4e10, fits in long long). So recommend `long long spiralValue(long long row, long long col)`.
//
// Time complexity O(1) per call. Space O(1).
//
// Now, produce the solution function.
