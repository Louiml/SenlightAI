// Given three sorted arrays of integers `a`, `b`, and `c` with sizes `n`, `m`, and `k` respectively (each array may contain duplicate values), write a C++ function `long long minimumSquaredDistance(const std::vector<long long>& a, const std::vector<long long>& b, const std::vector<long long>& c)` that returns the minimum possible value of the expression `(x - y)^2 + (y - z)^2 + (z - x)^2` where `x` is chosen from array `a`, `y` from array `b`, and `z` from array `c`. The input vectors are not necessarily sorted, and the function must handle arrays of size at least 1. The result fits in a signed 64-bit integer. The function should be self-contained, efficient, and correct for arbitrary large inputs up to 100,000 elements per array.
// The core insight is that the expression `(x - y)^2 + (y - z)^2 + (z - x)^2` depends on the differences between the three chosen numbers. To minimize this sum of squared differences, we want the three numbers to be as close together as possible. A naive triple loop over all combinations would be O(n·m·k), which is too slow for large arrays. Instead, we sort each array and then for each element in one array, we use binary search (`lower_bound`) to locate the closest elements in the other two arrays, checking only a few candidates around the best match.
//
// The approach works as follows: sort all three arrays. Then iterate over each element `x` in array `a`. For this fixed `x`, we want to find `y` in `b` and `z` in `c` that are near `x`. We use `lower_bound` to find the first position in `b` where the value is >= `x`, then check the two or three closest indices around that position (previous, equal, next) — because the optimal `y` will be among these candidates. Similarly, for each candidate `y`, we use `lower_bound` in `c` to find `z` near the average of `x` and `y` (since the optimal point that minimizes squared differences is roughly the midpoint), checking a small constant number of candidates around that point. We repeat this for each pair of arrays to ensure we don't miss a combination where the optimal `x` is not the fixed element. Specifically, we fix each array as the "base" and search in the other two. Edge cases: arrays may have duplicates, so `lower_bound` returns the first occurrence; we must clamp indices to valid range and check multiple neighbors (like -2 to +2) to be safe. Time complexity: Sorting takes O(n log n + m log m + k log k). The main loop iterates over all elements in all three arrays (O(n + m + k)) and performs a constant number of binary searches (each O(log m) or O(log k)), so overall O((n+m+k) log(max(n,m,k))). Space complexity: O(1) auxiliary, excluding the storage of the input vectors.
#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <cmath>

long long minimumSquaredDistance(const std::vector<long long>& a,
                                 const std::vector<long long>& b,
                                 const std::vector<long long>& c) {
    std::vector<long long> A = a, B = b, C = c;
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    std::sort(C.begin(), C.end());

    long long ans = std::numeric_limits<long long>::max();

    auto update = [&](long long x, long long y, long long z) {
        long long d1 = x - y;
        long long d2 = y - z;
        long long d3 = z - x;
        ans = std::min(ans, d1*d1 + d2*d2 + d3*d3);
    };

    // Helper to check candidates around a position in a sorted vector
    auto check = [&](const std::vector<long long>& vec, long long target, int pos, long long x, long long y) {
        // Adjust pos to be a valid index and check a few neighbors
        for (int diff = -2; diff <= 2; ++diff) {
            int idx = pos + diff;
            if (idx >= 0 && idx < (int)vec.size()) {
                update(x, y, vec[idx]);
            }
        }
    };

    // Fix x in A, search y in B, z in C
    for (long long x : A) {
        int posB = std::lower_bound(B.begin(), B.end(), x) - B.begin();
        for (int db = -2; db <= 2; ++db) {
            int idxB = posB + db;
            if (idxB >= 0 && idxB < (int)B.size()) {
                long long y = B[idxB];
                // For fixed x and y, find z near average
                long long target = (x + y) / 2;
                int posC = std::lower_bound(C.begin(), C.end(), target) - C.begin();
                check(C, target, posC, x, y);
            }
        }
    }

    // Fix y in B, search x in A, z in C
    for (long long y : B) {
        int posC = std::lower_bound(C.begin(), C.end(), y) - C.begin();
        for (int dc = -2; dc <= 2; ++dc) {
            int idxC = posC + dc;
            if (idxC >= 0 && idxC < (int)C.size()) {
                long long z = C[idxC];
                long long target = (y + z) / 2;
                int posA = std::lower_bound(A.begin(), A.end(), target) - A.begin();
                check(A, target, posA, A[std::max(0, std::min((int)A.size()-1, posA))], y); // placeholder, will be overwritten
                // Better: manually check neighbors for x
                for (int da = -2; da <= 2; ++da) {
                    int idxA = posA + da;
                    if (idxA >= 0 && idxA < (int)A.size()) {
                        update(A[idxA], y, z);
                    }
                }
            }
        }
    }

    // Fix z in C, search x in A, y in B
    for (long long z : C) {
        int posA = std::lower_bound(A.begin(), A.end(), z) - A.begin();
        for (int da = -2; da <= 2; ++da) {
            int idxA = posA + da;
            if (idxA >= 0 && idxA < (int)A.size()) {
                long long x = A[idxA];
                long long target = (x + z) / 2;
                int posB = std::lower_bound(B.begin(), B.end(), target) - B.begin();
                for (int db = -2; db <= 2; ++db) {
                    int idxB = posB + db;
                    if (idxB >= 0 && idxB < (int)B.size()) {
                        update(x, B[idxB], z);
                    }
                }
            }
        }
    }

    return ans;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Include the solution function here (or link it)

int main() {
    // Basic test
    std::vector<long long> a1 = {1, 5, 10};
    std::vector<long long> b1 = {2, 6, 11};
    std::vector<long long> c1 = {3, 7, 12};
    assert(minimumSquaredDistance(a1, b1, c1) == 3);

    // Single element each
    std::vector<long long> a2 = {0};
    std::vector<long long> b2 = {0};
    std::vector<long long> c2 = {0};
    assert(minimumSquaredDistance(a2, b2, c2) == 0);

    // Negative numbers
    std::vector<long long> a3 = {-10, -5, 0};
    std::vector<long long> b3 = {-8, -6, -4};
    std::vector<long long> c3 = {-7, -5, -3};
    assert(minimumSquaredDistance(a3, b3, c3) == 2);

    // Duplicates and larger range
    std::vector<long long> a4 = {1, 1, 100};
    std::vector<long long> b4 = {2, 2, 3};
    std::vector<long long> c4 = {3, 3, 4};
    assert(minimumSquaredDistance(a4, b4, c4) == 3);

    // Widely separated arrays: best is choose closest from each
    std::vector<long long> a5 = {0, 10, 20};
    std::vector<long long> b5 = {100, 110, 120};
    std::vector<long long> c5 = {200, 210, 220};
    // Choose x=20, y=100, z=200: (80^2 + 100^2 + 180^2) = 6400 + 10000 + 32400 = 48800
    // but maybe x=10, y=100, z=200 gives (90^2 + 100^2 + 190^2) = 8100+10000+36100=54200
    // x=20, y=100, z=200 is best? Actually check x=0, y=100, z=200: (100^2+100^2+200^2)=10000+10000+40000=60000
    // So min is 48800
    assert(minimumSquaredDistance(a5, b5, c5) == 48800);

    // Random stress small check (not exhaustive but a sanity check)
    std::vector<long long> a6 = {3, 1, 2};
    std::vector<long long> b6 = {6, 5, 4};
    std::vector<long long> c6 = {9, 8, 7};
    // Best: x=3, y=4, z=7? (1^2+3^2+4^2)=1+9+16=26
    // x=2,y=4,z=7? (4+9+25)=38
    // x=3,y=5,z=7? (4+4+16)=24
    // x=3,y=4,z=8? (1+16+25)=42
    // x=2,y=5,z=7? (9+4+25)=38
    // So min is 24
    assert(minimumSquaredDistance(a6, b6, c6) == 24);

    return 0;
}
