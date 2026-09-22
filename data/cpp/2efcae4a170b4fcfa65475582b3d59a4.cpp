// Write a C++ function `bool areInSameSector(long long m, long long n, int q, const vector<tuple<int, long long, int, long long>>& queries)` that, given positive integers `m` and `n` representing the dimensions of a grid divided vertically into `m` equal parts along one axis and `n` equal parts along the other, and for each query consisting of two points `(type1, coord1)` and `(type2, coord2)`, determines whether the two points fall into the same sector. Here, `type` is 1 if the coordinate is measured along the first axis (range `[1, m]`) and 2 if along the second axis (range `[1, n]`). The grid is split into `d = gcd(m, n)` equal diagonal "sectors" such that a point `(t, v)` maps to sector index: if `t == 1`, sector = `(v - 1) / (m / d)`, otherwise sector = `(v - 1) / (n / d)`. Return `true` if both points in the query map to the same sector, else `false`. The function should not read from standard input; instead it receives all query data as a vector of tuples. Assume `m, n >= 1`, `1 <= v <=` corresponding dimension, and `q >= 0`. The function must be const-correct (parameters by const reference where appropriate) and include a descriptive comment.
// The problem is a direct extraction from the snippet: the core logic computes `d = gcd(m, n)`, then reduces each point's coordinate to a "block index" by dividing the coordinate minus one by the step size (`m/d` if type 1, `n/d` if type 2). Two points are in the same sector iff their block indices match. This works because the grid is divided into `d` equal diagonal strips: along the first axis each strip spans `m/d` units, and along the second axis `n/d` units. The gcd ensures the strips align at integer boundaries for both axes. Edge cases: if `d=1`, there is only one sector, so every pair is in the same sector. Coordinates are 1-indexed, so subtracting 1 before integer division correctly maps ranges of equal size to the same index. For each query, time is O(1) with O(q) total, and space is O(1) auxiliary beyond the input vector. The function must handle large values (64-bit) as the snippet uses `long long`.
#include <vector>
#include <tuple>
#include <numeric> // for std::gcd (C++17)
#include <cstdint>

// Determine if two points, each given as (type, coordinate), belong to the same sector
// in a grid divided into d = gcd(m, n) equal diagonal sectors.
bool areInSameSector(
    long long m,
    long long n,
    const std::vector<std::tuple<int, long long, int, long long>>& queries
) {
    // Compute number of sectors
    long long d = std::gcd(m, n);
    long long step1 = m / d; // size along axis1
    long long step2 = n / d; // size along axis2

    // Process each query
    for (const auto& q : queries) {
        int t1 = std::get<0>(q);
        long long v1 = std::get<1>(q);
        int t2 = std::get<2>(q);
        long long v2 = std::get<3>(q);

        // Convert each point to sector index
        long long sector1 = (t1 == 1) ? (v1 - 1) / step1 : (v1 - 1) / step2;
        long long sector2 = (t2 == 1) ? (v2 - 1) / step1 : (v2 - 1) / step2;

        if (sector1 != sector2) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>
#include <tuple>

// Declaration of the function under test
bool areInSameSector(long long m, long long n, const std::vector<std::tuple<int, long long, int, long long>>& queries);

int main() {
    // Example: m=4, n=6 => d=2, step1=2, step2=3
    // Sector 0: axis1 [1,2], axis2 [1,3]
    // Sector 1: axis1 [3,4], axis2 [4,6]
    std::vector<std::tuple<int, long long, int, long long>> q1 = {
        {1, 1, 2, 2},   // both sector 0
        {1, 3, 2, 5},   // both sector 1
        {1, 2, 2, 4}    // first sector 0, second sector 1
    };
    assert(areInSameSector(4, 6, q1) == false); // last query differs

    // All queries same sector
    std::vector<std::tuple<int, long long, int, long long>> q2 = {
        {1, 1, 1, 2},
        {2, 1, 2, 3},
        {1, 2, 2, 3}
    };
    assert(areInSameSector(4, 6, q2) == true);

    // m=n (d=m, step=1) each point unique sector
    std::vector<std::tuple<int, long long, int, long long>> q3 = {
        {1, 1, 2, 1},
        {1, 2, 2, 2}
    };
    assert(areInSameSector(3, 3, q3) == false);

    // m=1 => d=1, single sector
    std::vector<std::tuple<int, long long, int, long long>> q4 = {
        {1, 1, 2, 100}
    };
    assert(areInSameSector(1, 100, q4) == true);

    // Empty query list returns true
    std::vector<std::tuple<int, long long, int, long long>> q5;
    assert(areInSameSector(10, 15, q5) == true);

    return 0;
}
