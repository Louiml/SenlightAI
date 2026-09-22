Given an even number of distinct points on a 2D Cartesian plane, write a C++ function that finds two integer coefficients `(A, B)` defining a line through the origin `A*x + B*y = 0` such that exactly half of the points lie strictly on the positive side (where `A*x + B*y > 0`) and exactly half lie strictly on the negative side (where `A*x + B*y < 0`). No point may lie exactly on the line. The coefficients must each be within the range `[-500, 500]`. If multiple valid coefficient pairs exist, return any one pair as a `std::pair<int,int>`. If no such pair exists in the given coefficient range, return `{0,0}` (which is guaranteed invalid because it would classify every point as on the line). The number of points `n` is even, between 2 and 200 inclusive, and all point coordinates are integers with absolute values up to 10,000. The function should be efficient enough for at most 200 points and 1,002,001 coefficient combinations.
The natural brute-force approach is to iterate over all possible coefficient pairs `(i, j)` with `i` and `j` from -500 to 500, inclusive. For each pair, evaluate the linear form `h = i*x + j*y` for every point. If any `h` equals 0, that line is invalid (a point lies on it), so skip to the next pair. Otherwise, count how many points have `h < 0` (negative side) and how many have `h > 0` (positive side). If both counts equal `n/2`, we have found a valid pair. Because we search in a fixed order (outer loop on `i`, inner on `j`), the first valid pair found is returned. The coordinate bounds are small enough that the total combinations are about 1,002,001, and for each we perform `O(n)` work, giving `O(1,002,001 * n)` worst-case time, which is at most about 200 million operations—feasible in C++. Space complexity is `O(n)` for storing the points, or we can process them directly from input vector. An important edge case is that when `n` is zero or odd, the function should still work correctly (if `n` is odd, no valid split exists, return `{0,0}`). Also, when `n` is positive but `n/2` equals 0 (i.e., `n=0`), the function should return `{0,0}` because no points exist to split. When points are collinear with the origin, some coefficient pairs may be invalid, but the search will skip them. The coefficient pair `(0,0)` is never valid because every point would have `h=0`. The search range is inclusive and symmetric, so both positive and negative coefficients are tried.
#include <utility>
#include <vector>

// Find coefficients (A, B) such that A*x + B*y separates points into two equal halves.
std::pair<int, int> findSeparatingLine(const std::vector<std::pair<int, int>>& points) {
    const int n = static_cast<int>(points.size());
    if (n % 2 != 0 || n == 0) {
        return {0, 0};
    }
    
    const int half = n / 2;
    
    // Iterate over all integer coefficient pairs in the range [-500, 500].
    for (int a = -500; a <= 500; ++a) {
        for (int b = -500; b <= 500; ++b) {
            if (a == 0 && b == 0) {
                continue; // invalid: all points on the line
            }
            
            int negative = 0;
            int positive = 0;
            bool onLine = false;
            
            for (const auto& pt : points) {
                const int x = pt.first;
                const int y = pt.second;
                const long long h = static_cast<long long>(a) * x + static_cast<long long>(b) * y;
                
                if (h == 0) {
                    onLine = true;
                    break;
                } else if (h < 0) {
                    ++negative;
                } else {
                    ++positive;
                }
            }
            
            if (!onLine && negative == half && positive == half) {
                return {a, b};
            }
        }
    }
    
    return {0, 0}; // no valid pair found in the range
}
#include <cassert>
#include <utility>
#include <vector>

int main() {
    // Simple case: points on a straight line with origin not collinear.
    std::vector<std::pair<int,int>> p1 = {{1,1}, {2,2}, {-1,-1}, {-2,-2}};
    auto r1 = findSeparatingLine(p1);
    // Valid pairs exist; check that returned pair is non-zero and separates.
    assert(r1.first != 0 || r1.second != 0);
    // Check that the returned pair indeed splits correctly.
    int neg = 0, pos = 0;
    for (auto& pt : p1) {
        long long h = 1LL * r1.first * pt.first + 1LL * r1.second * pt.second;
        if (h != 0) {
            if (h < 0) ++neg; else ++pos;
        }
    }
    assert(neg == 2 && pos == 2);

    // Two points: any non-collinear line splits them.
    std::vector<std::pair<int,int>> p2 = {{3,4}, {-5,6}};
    auto r2 = findSeparatingLine(p2);
    assert(r2.first != 0 || r2.second != 0);

    // Odd number of points: no valid split.
    std::vector<std::pair<int,int>> p3 = {{1,0}, {0,1}, {1,1}};
    assert(findSeparatingLine(p3) == std::pair<int,int>(0,0));

    // Empty input: returns zero pair.
    std::vector<std::pair<int,int>> p4;
    assert(findSeparatingLine(p4) == std::pair<int,int>(0,0));

    // All points on one side of every line through origin (impossible for even split? actually possible if some lines split).
    // But test a case with all points having positive x coordinates and positive y coordinates, and another with negatives.
    std::vector<std::pair<int,int>> p5 = {{1,1}, {2,2}, {3,3}, {4,4}};
    auto r5 = findSeparatingLine(p5);
    // Check that a valid pair was found (e.g., (1, -1) would give x - y = 0 for all, so invalid; but other pairs).
    // We just ensure non-zero and proper split.
    assert(r5.first != 0 || r5.second != 0);
    int neg5 = 0, pos5 = 0;
    for (auto& pt : p5) {
        long long h = 1LL * r5.first * pt.first + 1LL * r5.second * pt.second;
        if (h != 0) {
            if (h < 0) ++neg5; else ++pos5;
        }
    }
    assert(neg5 == 2 && pos5 == 2);

    // Points that are symmetric about origin.
    std::vector<std::pair<int,int>> p6 = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    auto r6 = findSeparatingLine(p6);
    assert(r6.first != 0 || r6.second != 0);
    int neg6 = 0, pos6 = 0;
    for (auto& pt : p6) {
        long long h = 1LL * r6.first * pt.first + 1LL * r6.second * pt.second;
        if (h != 0) {
            if (h < 0) ++neg6; else ++pos6;
        }
    }
    assert(neg6 == 2 && pos6 == 2);

    return 0;
}
