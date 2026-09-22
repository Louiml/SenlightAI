// You are given an `n x n` grid of characters, where each cell contains either `'.'` (empty) or `'*'` (blocked). Write a C++ function `int largestBlockRadius(const std::vector<std::string>& grid)` that returns the largest non-negative integer radius `r` such that there exists at least one empty cell `(i, j)` (0-indexed) for which every cell within Euclidean distance `≤ r` from `(i, j)` (including the center) is empty. The center cell itself must also be empty. A cell is considered within distance `r` if the squared Euclidean distance (computed as `(i - a)^2 + (j - b)^2`) is `≤ r^2`. The grid size `n` satisfies `1 ≤ n ≤ 50`. If no empty cell exists, return `-1`. You must implement the function without using global variables, and the solution must handle the fact that the radius can be at most the maximum number of steps to any edge from the center (i.e., `max(0, min(i, j, n-1-i, n-1-j))`) for any candidate center.

#include <cassert>
#include <vector>
#include <string>

int largestBlockRadius(const std::vector<std::string>& grid); // forward declaration

int main() {
    // Single cell empty grid
    assert(largestBlockRadius({"."}) == 0);

    // Single cell blocked
    assert(largestBlockRadius({"*"}) == -1);

    // All empty 3x3 grid: max radius is 1 (center can reach all 8 neighbors)
    assert(largestBlockRadius({"...", "...", "..."}) == 1);

    // 5x5 all empty: radius 2 works from center (2,2)
    assert(largestBlockRadius({".....", ".....", ".....", ".....", "....."}) == 2);

    // Blocked cell at (0,1) prevents radius 1 from center (1,1) but radius 0 works
    assert(largestBlockRadius({"..", ".*"}) == 0);

    // 4x4 with center at (1,1) allowed radius 1, but (2,2) blocked prevents radius 2
    assert(largestBlockRadius({"....", "....", "..*.", "...."}) == 1);

    // Asymmetric: 5x5 with all clear except a single blocked at (0,0) still allows radius 2 from (2,2)
    assert(largestBlockRadius({"*....", ".....", ".....", ".....", "....."}) == 2);

    // All blocked except one empty at corner: radius 0
    assert(largestBlockRadius({"*.", "**"}) == 0);

    // 2x2 all empty: max radius 0 because any non‑zero radius goes off‑grid
    assert(largestBlockRadius({"..", ".."}) == 0);

    // 6x6 with a central blocked cell forcing radius 1 instead of 2
    assert(largestBlockRadius({"......", "......", "..***.", "......", "......", "......"}) == 1);
}

#include <vector>
#include <string>
#include <algorithm>

// Returns the largest radius r such that there exists an empty center cell
// whose entire radius-r disk (within the grid) contains no '*'.
int largestBlockRadius(const std::vector<std::string>& grid) {
    const int n = static_cast<int>(grid.size());
    if (n == 0) return -1;

    int best = -1; // no valid center yet

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == '*') continue; // center must be empty

            // Maximum possible radius before leaving the grid.
            int maxR = std::min({i, j, n - 1 - i, n - 1 - j});

            // Binary search for the largest feasible radius.
            int low = 0, high = maxR, ans = 0;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                bool valid = true;

                // Check all cells inside the grid within Euclidean distance <= mid.
                for (int a = 0; a < n && valid; ++a) {
                    for (int b = 0; b < n; ++b) {
                        int dx = a - i;
                        int dy = b - j;
                        if (dx * dx + dy * dy <= mid * mid) {
                            if (grid[a][b] == '*') {
                                valid = false;
                                break;
                            }
                        }
                    }
                }

                if (valid) {
                    ans = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }

            best = std::max(best, ans);
        }
    }

    return best;
}

// The straightforward approach is to iterate over every cell as a potential center. For each empty cell, we binary‑search the maximum radius `r` that satisfies the condition. The search space for `r` is `[0, maxR]` where `maxR = min(i, j, n-1-i, n-1-j)` because any larger radius would necessarily include a cell outside the grid (out‑of‑bounds) which is invalid. For a given candidate radius `m`, we must check whether every cell `(a,b)` inside the grid with `(a-i)^2+(b-j)^2 ≤ m^2` is empty. If any such cell is blocked, the candidate is invalid. Since `n ≤ 50`, a brute‑force check over all `n^2` cells per candidate is acceptable. Binary search reduces the number of checks to `O(log n)` per center. The total time complexity is `O(n^2 * log n * n^2) = O(n^4 log n)`, which is fine for `n=50` (about 6.25 million checks per center worst case, but binary search cuts it). Edge cases: if the grid has no empty cells, return `-1`. If an empty cell exists, radius `0` is always valid, so the answer is at least `0`. For a single‑cell grid, the only radius is `0`. The distance condition uses squared distance to avoid floating‑point issues. Space complexity is `O(1)` auxiliary beyond the input grid.
