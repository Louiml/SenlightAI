/*
Write a C++ function `long long countPointsOnLine(long long a, long long b, long long c, long long x1, long long x2, long long y1, long long y2)` that counts the number of integer coordinate points \((x,y)\) satisfying \(a x + b y + c = 0\) with \(x_1 \le x \le x_2\) and \(y_1 \le y \le y_2\). The function must handle all coefficients and bounds within the range of a signed 64-bit integer, including cases where one or both of \(a\) and \(b\) are zero, and when the linear equation has infinitely many solutions (both coefficients zero and \(c=0\)). Return the total count as a 64-bit integer. If no point satisfies the constraints, return 0.
*/

#include <algorithm>
#include <cstdlib>

using int64 = long long;

// Extended Euclidean: returns gcd(a,b) and sets x,y such that a*x + b*y = gcd(a,b)
int64 exgcd(int64 a, int64 b, int64& x, int64& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int64 x1, y1;
    int64 d = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return d;
}

// Count integer points on line a*x + b*y + c = 0 within the rectangle [x1,x2] x [y1,y2]
int64 countPointsOnLine(int64 a, int64 b, int64 c, int64 x1, int64 x2, int64 y1, int64 y2) {
    c = -c; // now equation is a*x + b*y = c

    // Both coefficients zero
    if (a == 0 && b == 0) {
        if (c == 0) {
            return (x2 - x1 + 1) * (y2 - y1 + 1);
        } else {
            return 0;
        }
    }

    // Only b nonzero
    if (a == 0) {
        if (c % b != 0) return 0;
        int64 y = c / b;
        if (y < y1 || y > y2) return 0;
        return x2 - x1 + 1;
    }

    // Only a nonzero
    if (b == 0) {
        if (c % a != 0) return 0;
        int64 x = c / a;
        if (x < x1 || x > x2) return 0;
        return y2 - y1 + 1;
    }

    // Both nonzero, use extended Euclidean
    int64 x0, y0;
    int64 d = exgcd(a, b, x0, y0);
    if (c % d != 0) return 0;

    int64 scale = c / d;
    int64 xp = x0 * scale;
    int64 yp = y0 * scale;

    // General solution: x = xp + (b/d)*t, y = yp - (a/d)*t
    int64 tx = b / d;
    int64 ty = -a / d;

    // To make interval computation simpler, ensure both step coefficients are positive.
    // For tx < 0: flip the sign of t and also flip x bounds.
    if (tx < 0) {
        tx = -tx;
        xp = -xp;
        std::swap(x1, x2);
        x1 = -x1;
        x2 = -x2;
    }
    if (ty < 0) {
        ty = -ty;
        yp = -yp;
        std::swap(y1, y2);
        y1 = -y1;
        y2 = -y2;
    }

    // Compute range of t for x-interval: x1 <= xp + tx*t <= x2
    int64 t_min_x, t_max_x;
    // Lower bound: need tx*t >= x1 - xp  => t >= ceil((x1-xp)/tx)
    int64 num = x1 - xp;
    if (num >= 0) {
        t_min_x = (num + tx - 1) / tx;
    } else {
        t_min_x = num / tx; // division truncates toward zero, but num negative so this is floor, which equals ceil for negative
    }
    // Upper bound: need tx*t <= x2 - xp => t <= floor((x2-xp)/tx)
    num = x2 - xp;
    if (num >= 0) {
        t_max_x = num / tx;
    } else {
        t_max_x = -((-num + tx - 1) / tx); // ceil for negative => floor of negative
    }

    // Compute range of t for y-interval: y1 <= yp + ty*t <= y2
    int64 t_min_y, t_max_y;
    num = y1 - yp;
    if (num >= 0) {
        t_min_y = (num + ty - 1) / ty;
    } else {
        t_min_y = num / ty;
    }
    num = y2 - yp;
    if (num >= 0) {
        t_max_y = num / ty;
    } else {
        t_max_y = -((-num + ty - 1) / ty);
    }

    int64 t_min = std::max(t_min_x, t_min_y);
    int64 t_max = std::min(t_max_x, t_max_y);
    if (t_min > t_max) return 0;
    return t_max - t_min + 1;
}

#include <cassert>

int main() {
    // Simple line through origin: x - y = 0 (a=1,b=-1,c=0), range 0..3,0..3
    assert(countPointsOnLine(1, -1, 0, 0, 3, 0, 3) == 4); // (0,0),(1,1),(2,2),(3,3)

    // Line x=2 (vertical): a=1,b=0,c=-2, range x 0..4, y 0..4
    assert(countPointsOnLine(1, 0, -2, 0, 4, 0, 4) == 5);

    // Line y=1 (horizontal): a=0,b=1,c=-1, range x -2..2, y 0..2
    assert(countPointsOnLine(0, 1, -1, -2, 2, 0, 2) == 5);

    // No solution because gcd(4,6)=2 does not divide -7
    assert(countPointsOnLine(4, 6, 7, -10, 10, -10, 10) == 0);

    // Infinite solutions: a=0,b=0,c=0 inside a 3x3 square
    assert(countPointsOnLine(0, 0, 0, 0, 2, 0, 2) == 9);

    // Infinite solutions but c!=0: no solutions
    assert(countPointsOnLine(0, 0, 5, 0, 2, 0, 2) == 0);

    // Line x+y=5 in rectangle 0..5 squared: points (0,5),(1,4),(2,3),(3,2),(4,1),(5,0)
    assert(countPointsOnLine(1, 1, -5, 0, 5, 0, 5) == 6);

    // Line 2x+4y=6, gcd=2, particular solution (3,0), general x=3+2t, y=0-t
    // Range x 0..10, y 0..10: t from -1 (x=1,y=1) to 0 (x=3,y=0) gives 2 points, plus t=-1? check: t=-1 → (1,1) inside; t=0 → (3,0) inside; t=1 → (5,-1) not in y range
    assert(countPointsOnLine(2, 4, -6, 0, 10, 0, 10) == 2);

    // Negative coefficients: line -x+y=0 in range -5..5,-5..5, points where x=y
    assert(countPointsOnLine(-1, 1, 0, -5, 5, -5, 5) == 11);

    // Bounds where no point fits but line exists
    assert(countPointsOnLine(1, 1, 0, 6, 10, 6, 10) == 0);

    // Single point: line x=3 and y=3 range only that point
    assert(countPointsOnLine(1, 0, -3, 3, 3, 3, 3) == 1);

    // Large numbers to ensure 64-bit correctness (simple line through origin)
    assert(countPointsOnLine(1000000000LL, 1000000000LL, 0, -2000000000LL, 2000000000LL, -2000000000LL, 2000000000LL) == 4000000001LL);
}

// The equation \(a x + b y + c = 0\) can be rewritten as \(a x + b y = -c\). First handle degenerate cases: if \(a=0\) and \(b=0\), then the equation is either always true (if \(c=0\), all integer points in the rectangle are solutions, count = \((x_2-x_1+1)(y_2-y_1+1)\)) or never true (if \(c\neq 0\), return 0). If \(a=0\) but \(b\neq0\), then the equation imposes \(y = -c/b\); if \(-c\) is divisible by \(b\) and that integer lies in \([y_1,y_2]\), the count is \(x_2-x_1+1\), else 0. Symmetrically for \(b=0\). For \(a\neq0\) and \(b\neq0\), use the extended Euclidean algorithm to find one particular solution \((x_0,y_0)\) to \(a x + b y = d\), where \(d = \gcd(a,b)\). If \(d\) does not divide \(-c\), return 0. Otherwise, scale the particular solution by \(k = -c/d\) to get one solution \((x_p, y_p)\). The general integer solution is \(x = x_p + (b/d) t\), \(y = y_p - (a/d) t\) for integer \(t\). To make interval calculations simpler, ensure the coefficient for \(t\) in the \(x\)-expression is positive; if \(b/d < 0\), we can multiply the whole parametrization by \(-1\) and adjust the bounds. Similarly for the \(y\)-expression, ensure \(a/d\) is positive by symmetry. Then for each coordinate we can compute the range of \(t\) that keeps the coordinate within its interval, using careful integer division that correctly handles negative numbers. The intersection of the two ranges for \(x\) and \(y\) yields the valid \(t\) interval; the count is the number of integers in that intersection. Time complexity is \(O(\log \min(|a|,|b|))\) due to the extended Euclidean algorithm, and constant extra space.
