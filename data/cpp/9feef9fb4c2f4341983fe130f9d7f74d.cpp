// Given a vector of 2D integer points where each point is expressed as `{x, y}`, write a C++ function `int countTrapezoids(const std::vector<std::vector<int>>& points)` that returns the number of distinct axis-aligned trapezoids (with two parallel horizontal sides and two non-parallel slanted sides) that can be formed using these points as vertices, modulo `1e9+7`. A trapezoid is defined as a quadrilateral with at least one pair of parallel sides; here we specifically consider those with exactly one pair of parallel horizontal sides (i.e., the top and bottom sides are horizontal, but the left and right sides are not vertical). The four vertices must be distinct points, and the top and bottom sides must lie on two different horizontal lines (different y-values). For each horizontal line that contains at least two points, you can choose any two points on that line to form one horizontal side. Two different horizontal lines (with different y-values) are required to form the top and bottom sides. The other two vertices are simply the other endpoints of those chosen pairs. The order of choosing lines does not matter (choosing line A for the top and line B for the bottom is the same as the reverse), so each set of two distinct horizontal lines with at least two points contributes the product of the numbers of pairs on those two lines to the total count. The output must be modulo `1e9+7`. The input may contain duplicate points, and those duplicates are treated as separate points for selection purposes. If fewer than two distinct y-values have at least two points, return 0.
The problem reduces to counting the number of ways to select two distinct horizontal lines (y-values) such that each selected line has at least two points, and then for each pair of lines, multiplying the number of ways to choose two points on the first line by the number of ways to choose two points on the second line. The key observation is that we only care about the frequency of points on each y-value. Let `freq[y]` be the number of points with that y-coordinate. For each line with `c = freq[y] >= 2`, the number of ways to pick two points on that line is `c choose 2 = c*(c-1)/2`. Then the total number of trapezoids is the sum over all unordered pairs of distinct lines `(i, j)` with both having at least two points, of `(ways_i * ways_j)`. Since `(sum_{i} ways_i)^2 = sum_{i} ways_i^2 + 2 * sum_{i<j} ways_i * ways_j`, we can compute the answer efficiently: sum over all contributions from unordered pairs is `(S^2 - sum(ways_i^2)) / 2`, but we must handle modulo and division modulo `1e9+7` (which is prime, so use modular inverse of 2). However, the original code uses a prefix sum approach: it sorts the lines by some order (e.g., by y) and for each line `i`, it adds `ways_i * (sum of ways for all subsequent lines)`. This avoids division and directly accumulates the sum of products over ordered pairs where the first index is less than the second. The implementation must handle the modulo carefully: because `c*(c-1)/2` involves integer division, but since `c*(c-1)` is always even, we can safely compute integer division before taking modulo, but to be safe we can compute `( ( (c-1) % mod) * (c % mod) / 2 ) % mod` but integer division after multiplication might overflow, so better compute `(c % mod) * ((c-1) % mod) % mod * inv2 % mod` where `inv2` is the modular inverse of 2, or just compute `(long long)(c) * (c-1) / 2` directly and then take modulo. Since `c <= number of points` which fits in int, this is safe. Edge cases: if there are fewer than two distinct y-values with at least two points, return 0; duplicate points are allowed and each duplicate contributes to frequency. Time complexity: O(n) to count frequencies via hash map, plus O(k log k) if we sort the y-values, or O(k) if we iterate through map in any order (the order doesn't affect the sum because we only need sum of ways for all subsequent in some arbitrary order; as long as we treat each pair once). Using a prefix sum over the list of `ways` values (not over original frequencies) works. Specifically, let `ways` be a vector of all `c choose 2` for lines with `c >= 2`. Sort `ways` (or keep in any order) and compute prefix sums. For each index `i`, add `ways[i] * (total_ways_of_all_after_i)` to answer. This ensures each unordered pair is counted exactly once. Space complexity O(k) where k is number of distinct y-values.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Count trapezoids modulo 1e9+7
int countTrapezoids(const std::vector<std::vector<int>>& points) {
    const long long MOD = 1000000007LL;
    std::unordered_map<int, int> freq;
    for (const auto& p : points) {
        freq[p[1]]++;  // y-coordinate is p[1]
    }

    // Collect number of ways to pick two points on each horizontal line with at least two points
    std::vector<long long> ways;
    for (const auto& kv : freq) {
        long long c = kv.second;
        if (c >= 2) {
            // c choose 2, using 64-bit to avoid overflow
            long long w = c * (c - 1) / 2;
            ways.push_back(w);
        }
    }

    if (ways.size() < 2) return 0;

    // Compute prefix sums so that for each i, sum_after[i] = sum of ways[j] for j > i
    std::vector<long long> prefix(ways.size() + 1, 0);
    for (size_t i = 0; i < ways.size(); ++i) {
        prefix[i + 1] = (prefix[i] + ways[i]) % MOD;
    }

    long long ans = 0;
    for (size_t i = 0; i < ways.size(); ++i) {
        long long sum_after = (prefix.back() - prefix[i + 1] + MOD) % MOD;
        ans = (ans + (ways[i] % MOD) * sum_after) % MOD;
    }
    return static_cast<int>(ans);
}
#include <cassert>
#include <vector>

// Function declaration (implementation above)
int countTrapezoids(const std::vector<std::vector<int>>& points);

int main() {
    // Example: two lines with 2 points each => 1 trapezoid
    std::vector<std::vector<int>> p1 = {{0,0},{1,0},{2,1},{3,1}};
    assert(countTrapezoids(p1) == 1);

    // One line has 3 points, another has 2 => 3 ways on first line * 1 way on second = 3
    std::vector<std::vector<int>> p2 = {{0,0},{1,0},{2,0},{3,1},{4,1}};
    assert(countTrapezoids(p2) == 3);

    // Two lines with 3 points each => 3*3 = 9
    std::vector<std::vector<int>> p3 = {{0,0},{1,0},{2,0},{3,1},{4,1},{5,1}};
    assert(countTrapezoids(p3) == 9);

    // Only one horizontal line with >=2 points => 0
    std::vector<std::vector<int>> p4 = {{0,0},{1,0},{2,2}};
    assert(countTrapezoids(p4) == 0);

    // Duplicate points: (0,0) appears twice, (1,0) appears once, (2,1) twice, (3,1) twice
    // Line y=0 has 3 points => 3 ways; line y=1 has 4 points => 6 ways => total 18
    std::vector<std::vector<int>> p5 = {{0,0},{0,0},{1,0},{2,1},{2,1},{3,1},{3,1}};
    assert(countTrapezoids(p5) == 18);

    // Three lines each with 2 points => choose any two lines: 3 pairs * 1*1 = 3
    std::vector<std::vector<int>> p6 = {{0,0},{1,0},{2,1},{3,1},{4,2},{5,2}};
    assert(countTrapezoids(p6) == 3);

    // Large count to test modulo: 4 lines each with 100 points => each line has 4950 ways
    // Sum over all pairs = 6 * 4950^2 = 147,015,000, which mod 1e9+7 is 147015000
    std::vector<std::vector<int>> p7;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 100; ++x) {
            p7.push_back({x, y});
        }
    }
    assert(countTrapezoids(p7) == 147015000);

    return 0;
}
