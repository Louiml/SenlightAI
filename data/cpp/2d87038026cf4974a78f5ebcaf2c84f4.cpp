// Given a grid of height `h` and width `w` cells, and a list of `n` black cells specified by 1-based coordinates `(a, b)`, write a C++ function `countBlackCenters` that returns a `std::vector<long long>` of length 10. The first element (index 0) must contain the number of all possible 3×3 subgrids centered at cells `(x, y)` (where `1 ≤ x ≤ h-2`, `1 ≤ y ≤ w-2`) that contain exactly 0 black cells. Each subsequent element (indexes 1..9) must contain the number of such subgrids that contain exactly 1, 2, ..., 9 black cells respectively. Note that a black cell can belong to multiple overlapping 3×3 subgrids if it lies within the interior region (not on the border of the whole grid). The function receives `h`, `w`, and a vector of pairs of black cell coordinates. The grid dimensions can be up to `1e9`, but the number of black cells `n` is at most `10^5`.
// Since the grid is enormous but only a few cells are black, we cannot iterate over all possible centers. Instead, observe that a black cell `(a,b)` only affects centers `(x,y)` such that the 3×3 subgrid centered at `(x,y)` includes `(a,b)`. That means `x` must be in `{a-1, a, a+1}` and `y` must be in `{b-1, b, b+1}`. Also, valid centers must satisfy `1 ≤ x ≤ h-2` and `1 ≤ y ≤ w-2`. So for each black cell, we consider up to 9 candidate centers (bounded by grid borders). For each valid candidate center, we increment a counter in an unordered_map keyed by that center's coordinates. After processing all black cells, the map's keys are exactly the centers that contain at least one black cell, and each value is the number of black cells in that 3×3 subgrid. The total number of possible centers is `(h-2)*(w-2)`, which might overflow 32-bit int, so use `long long`. The number of centers with exactly 0 black cells is total centers minus the number of distinct keys in the map (since every key has at least one black cell). Then we iterate through the map values, count frequencies for counts 1..9 (values cannot exceed 9 because a 3×3 subgrid has 9 cells). Edge cases: when `h` or `w` is less than 3, there are no valid centers, so the first element is 0 and every other is 0. Also, when a black cell is near the border, some candidate centers may be out of bounds and must be skipped. Time complexity: O(n * 9) for processing, plus O(k) for counting the map keys, where k ≤ 9n, so O(n) overall. Space: O(n) for the map.
#include <vector>
#include <unordered_map>
#include <utility>

// Returns a vector of 10 elements:
// index 0: number of 3x3 subgrids with 0 black cells
// indices 1..9: number of 3x3 subgrids with exactly 1..9 black cells
std::vector<long long> countBlackCenters(int h, int w, const std::vector<std::pair<int,int>>& blackCells) {
    // Total possible centers
    long long totalCenters = 0;
    if (h >= 3 && w >= 3) {
        totalCenters = static_cast<long long>(h - 2) * (w - 2);
    }

    // Map from a center (x,y) encoded as a single long long to the count of black cells in its 3x3 area
    std::unordered_map<long long, int> centerCount;
    // Encode x and y (both up to 1e9) using base = 1e9 + 10 (same as Bound)
    const long long base = 1000000010LL;

    for (const auto& cell : blackCells) {
        int a = cell.first;
        int b = cell.second;
        // Candidate centers: x from a-1 to a+1, y from b-1 to b+1
        for (int dx = -1; dx <= 1; ++dx) {
            int x = a + dx;
            if (x < 1 || x > h - 2) continue; // invalid center x
            for (int dy = -1; dy <= 1; ++dy) {
                int y = b + dy;
                if (y < 1 || y > w - 2) continue; // invalid center y
                long long key = static_cast<long long>(x) * base + y;
                centerCount[key]++;
            }
        }
    }

    // Result vector: index 0 for zero black cells, 1..9 for exact counts
    std::vector<long long> result(10, 0);
    // Number of centers that have at least one black cell
    long long nonZeroCenters = static_cast<long long>(centerCount.size());
    result[0] = totalCenters - nonZeroCenters;

    for (const auto& entry : centerCount) {
        int cnt = entry.second; // between 1 and 9
        if (cnt >= 1 && cnt <= 9) {
            result[cnt]++;
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (already provided above)
std::vector<long long> countBlackCenters(int h, int w, const std::vector<std::pair<int,int>>& blackCells);

int main() {
    // Example 1: 3x3 grid, one black cell at center (2,2)
    // Only one center (1,1) with 1 black cell -> result[1]=1, result[0]=0
    {
        auto res = countBlackCenters(3, 3, {{2,2}});
        assert(res.size() == 10);
        assert(res[0] == 0);
        assert(res[1] == 1);
        for (int i = 2; i < 10; ++i) assert(res[i] == 0);
    }

    // Example 2: 3x3 grid, no black cells -> total centers = 1, result[0]=1
    {
        auto res = countBlackCenters(3, 3, {});
        assert(res[0] == 1);
        for (int i = 1; i < 10; ++i) assert(res[i] == 0);
    }

    // Example 3: 5x5 grid, black cells at (1,1) and (3,3)
    // Centers: (1,1)..(3,3) -> 9 centers total
    // (1,1) affects centers (1,1) only? Actually (1,1) is corner, affects centers (1,1) and (1,2) and (2,1) and (2,2)? Let's compute manually.
    // For (1,1): candidate centers x=1 (valid), y=1 (valid) => center (1,1) gets+1. Also x=1,y=2 => center (1,2)+1. x=2,y=1 => center (2,1)+1. x=2,y=2 => center (2,2)+1. (x=0 invalid, y=0 invalid)
    // For (3,3): candidate centers x=2,3 (valid), y=2,3 (valid) => centers (2,2),(2,3),(3,2),(3,3) each +1
    // So counts: (1,1):1, (1,2):1, (2,1):1, (2,2):2, (2,3):1, (3,2):1, (3,3):1 -> 7 centers with any black, 2 centers with 0 (total 9)
    // Counts of 1:5 (centers with 1), count of 2:1 (center with 2)
    {
        auto res = countBlackCenters(5, 5, {{1,1},{3,3}});
        assert(res[0] == 2); // no black
        assert(res[1] == 5); // exactly 1 black
        assert(res[2] == 1); // exactly 2 black
        for (int i = 3; i < 10; ++i) assert(res[i] == 0);
    }

    // Example 4: h or w less than 3 -> no centers
    {
        auto res = countBlackCenters(2, 10, {{1,5}});
        assert(res[0] == 0);
        for (int i = 1; i < 10; ++i) assert(res[i] == 0);
    }

    // Example 5: Large grid with one black cell not near border
    // h=10, w=10, black at (5,5) affects centers (4,4)..(6,6) 9 centers each with 1 black
    // total centers = 8*8=64, so result[0]=55, result[1]=9
    {
        auto res = countBlackCenters(10, 10, {{5,5}});
        assert(res[0] == 55);
        assert(res[1] == 9);
        for (int i = 2; i < 10; ++i) assert(res[i] == 0);
    }

    return 0;
}
