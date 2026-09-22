Write a C++ function that calculates the minimum number of square tiles of side length `a` required to completely cover a rectangular floor of dimensions `n` by `m` meters, given that tiles may be cut and placed with their sides parallel to the floor's sides. The function should take three integer arguments `n`, `m`, and `a` (all greater than 0 and within the range of `long long`), and return the minimum number of tiles needed as a `long long`. The solution must compute the required tiles along each dimension by rounding up the division of the dimension by the tile side length, then multiply those two counts. Handle edge cases where `a` is larger than `n` or `m`, and where `n`, `m`, or `a` are very large (up to 10^9) to avoid overflow.
The problem reduces to determining how many tiles fit along each dimension independently because tiles must be axis-aligned. Along the `n`-dimension, the number of tiles needed is `ceil(n / a)`, which can be computed without floating-point by using integer arithmetic: `(n + a - 1) / a`. Similarly, along the `m`-dimension, the count is `(m + a - 1) / a`. The total number of tiles is the product of these two counts. Key edge cases: if `a` is greater than either dimension, the corresponding count is 1; if `n` or `m` are exact multiples of `a`, the ceiling expression works correctly (e.g., `n=6, a=3` yields `(6+2)/3 = 2`). Since `n` and `m` can be up to 10^9, the product `ceil(n/a) * ceil(m/a)` can be as large as 10^18, so the result must be stored in a `long long` to avoid overflow. The algorithm runs in O(1) time and O(1) auxiliary space.
#include <cstdint>

// Compute the minimum number of square tiles of side length a needed to cover an n x m rectangle.
// Tiles are axis-aligned; partial tiles are allowed.
long long minSquareTiles(long long n, long long m, long long a) {
    // Number of tiles along the n dimension (ceil division)
    const long long tiles_n = (n + a - 1) / a;
    // Number of tiles along the m dimension (ceil division)
    const long long tiles_m = (m + a - 1) / a;
    // Total tiles needed
    return tiles_n * tiles_m;
}
#include <cassert>

int main() {
    // Basic cases
    assert(minSquareTiles(6, 6, 4) == 4);  // 2*2
    assert(minSquareTiles(5, 7, 3) == 6);  // 2*3
    assert(minSquareTiles(1, 1, 1) == 1);
    assert(minSquareTiles(10, 10, 10) == 1);  // exact fit
    assert(minSquareTiles(10, 10, 11) == 1);  // tile larger than both dimensions
    assert(minSquareTiles(3, 9, 4) == 3);  // ceil(3/4)=1, ceil(9/4)=3 => 1*3
    // Edge with large values (within long long range)
    assert(minSquareTiles(1000000000LL, 1000000000LL, 2LL) == 250000000000000000LL);  // 500M*500M
    assert(minSquareTiles(1000000000LL, 1000000000LL, 1LL) == 1000000000000000000LL);  // 1e9*1e9
    // One dimension smaller than tile
    assert(minSquareTiles(5, 1, 10) == 1);  // ceil(5/10)=1, ceil(1/10)=1
    assert(minSquareTiles(2, 3, 5) == 1);   // both smaller than tile
    // Non-multiple dimensions
    assert(minSquareTiles(7, 3, 2) == 8);  // ceil(7/2)=4, ceil(3/2)=2 => 4*2
    return 0;
}
