// Write a C++ function `countLinearDiophantineSolutions(long long a, long long b, long long d)` that returns the number of non-negative integer solutions \((x, y)\) to the linear Diophantine equation \(a x + b y = d\). The inputs are positive integers \(a, b\) and a non-negative integer \(d\). The function must handle large values (up to \(10^{18}\)) and return `0` if no solution exists. If \(d = 0\), there is exactly one trivial solution \((0,0)\). Use the extended Euclidean algorithm to find a particular solution, then count how many integer shifts keep both \(x\) and \(y\) non-negative. The result fits in a 64-bit signed integer.

#include <cassert>
#include <cstdint>

// The solution function is declared above

int main() {
    // Simple cases
    assert(countLinearDiophantineSolutions(2, 3, 5) == 1);  // (1,1)
    assert(countLinearDiophantineSolutions(2, 3, 6) == 2);  // (3,0), (0,2)
    assert(countLinearDiophantineSolutions(2, 4, 6) == 1);  // (1,1)
    assert(countLinearDiophantineSolutions(2, 4, 5) == 0);  // no solution
    assert(countLinearDiophantineSolutions(1, 1, 5) == 6);  // (0..5,5..0)

    // Edge cases
    assert(countLinearDiophantineSolutions(3, 5, 0) == 1);  // (0,0)
    assert(countLinearDiophantineSolutions(100, 200, 1) == 0); // gcd 100 not divide 1
    assert(countLinearDiophantineSolutions(7, 11, 77) == 1); // (11,0) and (0,7)? check: 7*11=77 -> x=11,y=0; another? 7*0+11*7=77 -> (0,7) => 2? let's verify: a=7,b=11,d=77, gcd=1, a_r=7,b_r=11,d_r=77. Extended: tri.x=-3, tri.y=2 (since 7*(-3)+11*2=1). x0=-231, y0=154. lower = ceil(231/11)=21, upper=154/7=22 => t=21,22 => 2 solutions. So assert 2)
    assert(countLinearDiophantineSolutions(7, 11, 77) == 2);
    // Large test: 1e18 values, but count fits in int64
    assert(countLinearDiophantineSolutions(1, 1, 1000000000000000000LL) == 1000000000000000001LL);
}

#include <cstdint>
#include <numeric>

struct Triple {
    int64_t gcd;
    int64_t x;
    int64_t y;
};

// Extended Euclidean algorithm returning gcd and Bezout coefficients
Triple extendedEuclid(int64_t a, int64_t b) {
    if (b == 0) {
        return {a, 1, 0};
    }
    Triple res = extendedEuclid(b, a % b);
    int64_t new_x = res.y;
    int64_t new_y = res.x - (a / b) * res.y;
    return {res.gcd, new_x, new_y};
}

// Count non-negative integer solutions to a*x + b*y = d
int64_t countLinearDiophantineSolutions(int64_t a, int64_t b, int64_t d) {
    if (d == 0) {
        return 1;  // only (0,0)
    }
    int64_t g = std::gcd(a, b);
    if (d % g != 0) {
        return 0;
    }
    // Reduce equation
    int64_t a_r = a / g;
    int64_t b_r = b / g;
    int64_t d_r = d / g;

    // Find one solution to a_r * x + b_r * y = d_r
    Triple tri = extendedEuclid(a_r, b_r);
    // tri.x and tri.y satisfy a_r*tri.x + b_r*tri.y = 1
    __int128 x0 = (__int128)tri.x * d_r;
    __int128 y0 = (__int128)tri.y * d_r;

    // General solution: x = x0 + t * b_r, y = y0 - t * a_r
    // Need t such that x >= 0 and y >= 0
    // Compute lower bound for t: ceil(-x0 / b_r)
    __int128 lower = (-x0 + b_r - 1) / b_r;  // ceil division for possibly negative
    if (x0 >= 0) lower = -x0 / b_r;          // if x0 non-negative, ceil(-x0/b) = floor(-x0/b)
    // More precisely: lower = ceil(-x0 / b_r)
    // Use floor division for negative:
    lower = (-x0 + b_r - 1) / b_r;           // works for any sign as long as b_r>0

    // Upper bound: floor(y0 / a_r)
    __int128 upper = y0 / a_r;

    if (lower > upper) {
        return 0;
    }
    // Number of integers t in [lower, upper]
    return static_cast<int64_t>(upper - lower + 1);
}

// The equation \(a x + b y = d\) has integer solutions iff \(g = \gcd(a, b)\) divides \(d\). Dividing all coefficients by \(g\), we get \(a' x + b' y = d'\) where \(\gcd(a', b') = 1\). Using the extended Euclidean algorithm, we find one solution \((x_0, y_0)\) to \(a' x + b' y = 1\), then multiply by \(d'\) to get a particular solution \((X_0, Y_0)\). The general integer solution is:
// \[
// x = X_0 + t \cdot b', \quad y = Y_0 - t \cdot a'
// \]
// for any integer \(t\). To have both non-negative, we need \(t \ge \lceil -X_0 / b' \rceil\) and \(t \le \lfloor Y_0 / a' \rfloor\). The number of integer \(t\) values in this interval is \(\max(0, \text{upper} - \text{lower} + 1)\). Special cases: if \(d=0\), return 1 (only (0,0)). If \(d\) not divisible by \(g\), return 0. Also handle when one coefficient becomes zero after division (not possible since \(a,b>0\)). The inverse modulo technique in the snippet is simpler but relies on \(b'\) being invertible modulo \(a'\), which is true because they are coprime; however, we can use the general formula directly to avoid modular inverse edge cases. Time complexity is \(O(\log(\min(a,b)))\) for Euclidean algorithm; space \(O(\log n)\) for recursion stack.
