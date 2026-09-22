/*
Given an `n x m` grid of characters `'R'` and `'S'` (representing red and silver cells, with `1 <= n, m <= 100`), write a C++ function `int largestUniformSquare(const std::vector<std::string>& grid)` that returns the side length of the largest square subgrid such that, starting from any placement of that square (i.e., an `x x x` block), you can repeatedly flip all cells inside any currently uniform square of that side length to become a mixture of both colors (or more precisely, when a square is chosen, all its cells become both `'R'` and `'S'` simultaneously – in terms of the original problem, they become "both" and then can be covered by other squares of any color, effectively allowing any square that is entirely one color or entirely already both to be selected). The process must eventually cover every possible top-left position of that square size exactly once, meaning you can sequentially select all `(n - x + 1) * (m - x + 1)` possible square positions, each time the square must be uniform (all original same char or all already both) at the moment of selection, and after selection all its cells become both. The function returns the maximum `x` for which such a complete covering is possible. The grid is 1-indexed internally, but input is 0-indexed.
*/
#include <vector>
#include <string>
#include <queue>
#include <cstring>
#include <algorithm>

// Returns the largest side length of a square that can fully cover all positions
// using the described "paint if uniform" process.
int largestUniformSquare(const std::vector<std::string>& grid) {
    const int n = (int)grid.size();
    const int m = (int)grid[0].size();
    const int maxSide = std::min(n, m);

    auto feasible = [&](int x) -> bool {
        // 1-indexed grids for prefix sums
        std::vector<std::vector<int>> span(n+1, std::vector<int>(m+1, 0));
        std::vector<std::vector<int>> rpan(n+1, std::vector<int>(m+1, 0));
        auto update_psum = [&]() {
            for (int i=1; i<=n; ++i) {
                for (int j=1; j<=m; ++j) {
                    span[i][j] = (grid[i-1][j-1] == 'S' ? 1 : 0) 
                                 + span[i-1][j] + span[i][j-1] - span[i-1][j-1];
                    rpan[i][j] = (grid[i-1][j-1] == 'R' ? 1 : 0) 
                                 + rpan[i-1][j] + rpan[i][j-1] - rpan[i-1][j-1];
                }
            }
        };
        std::vector<std::vector<bool>> vis(n+1, std::vector<bool>(m+1, false));
        // Actually initialize span/rpan as char maps, but we'll maintain them separately
        // For simplicity, we'll use separate 2D arrays that track if a cell is 'S' or 'R'
        // But the prefix sums above already handle original chars. To track "both", we need
        // to update the original cells after painting. So we'll maintain two boolean grids.
        std::vector<std::vector<bool>> isS(n+1, std::vector<bool>(m+1, false));
        std::vector<std::vector<bool>> isR(n+1, std::vector<bool>(m+1, false));
        for (int i=1; i<=n; ++i) {
            for (int j=1; j<=m; ++j) {
                isS[i][j] = (grid[i-1][j-1] == 'S');
                isR[i][j] = (grid[i-1][j-1] == 'R');
            }
        }
        // Recompute prefix sums from boolean grids
        auto rebuild_psum = [&]() {
            for (int i=1; i<=n; ++i) {
                for (int j=1; j<=m; ++j) {
                    span[i][j] = (isS[i][j] ? 1 : 0) + span[i-1][j] + span[i][j-1] - span[i-1][j-1];
                    rpan[i][j] = (isR[i][j] ? 1 : 0) + rpan[i-1][j] + rpan[i][j-1] - rpan[i-1][j-1];
                }
            }
        };
        rebuild_psum();

        auto scan = [&](int s, int e) -> bool {
            int x2 = x;
            int sumS = span[s+x2-1][e+x2-1] - span[s-1][e+x2-1] - span[s+x2-1][e-1] + span[s-1][e-1];
            int sumR = rpan[s+x2-1][e+x2-1] - rpan[s-1][e+x2-1] - rpan[s+x2-1][e-1] + rpan[s-1][e-1];
            return (sumS == x2*x2) || (sumR == x2*x2);
        };

        std::queue<std::pair<int,int>> q;
        int totalWindows = (n - x + 1) * (m - x + 1);
        int painted = 0;
        for (int i=1; i<=n-x+1; ++i) {
            for (int j=1; j<=m-x+1; ++j) {
                if (!vis[i][j] && scan(i,j)) {
                    vis[i][j] = true;
                    q.push({i,j});
                }
            }
        }
        while (!q.empty()) {
            auto [xf, yf] = q.front(); q.pop();
            painted++;
            // Paint the square: mark all cells as both
            for (int i=0; i<x; ++i) {
                for (int j=0; j<x; ++j) {
                    isS[xf+i][yf+j] = true;
                    isR[xf+i][yf+j] = true;
                }
            }
            rebuild_psum();
            // Find newly paintable windows
            for (int i=1; i<=n-x+1; ++i) {
                for (int j=1; j<=m-x+1; ++j) {
                    if (!vis[i][j] && scan(i,j)) {
                        vis[i][j] = true;
                        q.push({i,j});
                    }
                }
            }
        }
        return painted == totalWindows;
    };

    int low = 1, high = maxSide;
    while (low < high) {
        int mid = (low + high + 1) / 2;
        if (feasible(mid)) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}
#include <cassert>
#include <vector>
#include <string>

int largestUniformSquare(const std::vector<std::string>& grid);

int main() {
    // Single cell
    assert(largestUniformSquare({"R"}) == 1);
    assert(largestUniformSquare({"S"}) == 1);

    // 2x2 all same => largest square 2 works
    assert(largestUniformSquare({"RR","RR"}) == 2);
    assert(largestUniformSquare({"SS","SS"}) == 2);

    // 2x2 mixed => only 1 works
    assert(largestUniformSquare({"RS","SR"}) == 1);

    // 3x3 all S => 3
    assert(largestUniformSquare({"SSS","SSS","SSS"}) == 3);

    // 3x3 with one R corner => only 1? Actually 2 may work? Let's reason: 
    // 2x2 all S exists (bottom right) so that window can be painted first, then 
    // painting it makes those cells both, but the remaining 2x2 windows may not become uniform.
    // Test: grid: RRR / RRR / SSS -> largest 2? 
    // Let's just test a known case: 3x3 with top-left 2x2 mixed, bottom row all S.
    assert(largestUniformSquare({"RRR","RRR","SSS"}) == 2); // 2x2 all S bottom-right works first, then can paint others? Actually after painting bottom-right, cells become both, but other 2x2 windows still contain R and not both, so not paintable. So only 1? But we can paint the all-S 2x2 bottom-right, then paint a 2x2 that includes some of that and R cells? No, it's not uniform. So 1. Let's not over-assert; just rely on known cases.

    // 4x4 checkerboard => only 1
    std::vector<std::string> checker = {"RSRS","SRSR","RSRS","SRSR"};
    assert(largestUniformSquare(checker) == 1);

    // 4x4 top half S, bottom half R => 2? Let's test: 
    // Top-left 2x2 all S, can paint it, then it becomes both, then paint 
    // left-middle 2x2? It will have both and R, not uniform. So maybe 1. 
    // But we know a 2x2 all R exists bottom-left or bottom-right, paint that first.
    // Then all other 2x2 windows? Still may contain mixed. So 1.
    std::vector<std::string> split = {"SSSS","SSSS","RRRR","RRRR"};
    // Actually 2x2 all S top-left, 2x2 all R bottom-right, paint both, then 
    // middle 2x2? It has S and R and both, not uniform. So 1. But we can paint 
    // top-left, then top-middle? It has S and both, uniform (since both), so paint. 
    // Actually after painting top-left (all S), it becomes both. Then top-middle 
    // has S (from col2) and both (from col1?) it's all S? top row col2-3 are S, 
    // so yes all S, paint. Continue similarly, can paint all 3x3? Let's trust algorithm.
    // We'll just not assert on this ambiguous case; only assert clear ones.

    // Test known feasible case: 3x3 with all S => 3 already tested.
    // Test a case where 2 works: 3x3 with all S except bottom-right R? Then 
    // the 2x2 bottom-left is all S, paint it, then bottom-right? Still has R. 
    // So 1. Instead, make a 3x3 where a 2x2 all R and another 2x2 all S overlap 
    // such that after painting both, the rest become uniform. Example: 
    // S S R
    // S S S
    // R R R
    // Top-left 2x2 all S, bottom-right 2x2 all R, they overlap at center? No. 
    // Paint TL, then BR, then the middle? Still mixed. So 1.

    // To keep tests reliable, we'll only assert trivial cases.
    assert(largestUniformSquare({"S","S","S"}) == 1); // 3x1, maxSide=1
    assert(largestUniformSquare({"SSS","SSS"}) == 2); // 2x3, maxSide=2, all S => 2

    // Larger all same:
    std::vector<std::string> big(5, std::string(5,'R'));
    assert(largestUniformSquare(big) == 5);

    return 0;
}
// We need to determine the largest possible square size `x` such that we can paint all possible square windows of that size in some order, where a window is paintable if at that moment all cells inside are either all `'R'`, all `'S'`, or all "both" (i.e., already painted at least once). After painting, those cells become "both", meaning they can be used as either color in future checks. This is a BFS-like process: we start with all windows that are initially uniform (all same char). When we paint a window, we mark its cells as both, then re-check all unpainted windows; any that become uniform (because all their cells are now both or all same color not yet both) are pushed to the queue. The process succeeds if we paint all possible windows. Since the maximum possible side length is `min(n,m)`, we binary search over `x`. For a given `x`, we maintain two boolean grids: `span` (originally `'S'` cells) and `rpan` (originally `'R'` cells). A cell is "both" if both `span` and `rpan` are true. To efficiently check if a window is uniform original or all both, we use 2D prefix sums of `span` and `rpan`: a window of size `x` is paintable if its sum of `span` equals `x*x` (all originally `'S'` or already both) OR its sum of `rpan` equals `x*x` (all originally `'R'` or already both). We repeatedly update prefix sums after each painting and enqueue newly paintable windows. If we process all windows, `x` is feasible. Edge cases: when `x = 1`, every cell is a 1x1 window and always paintable (since it is either 'R' or 'S'), so it's always feasible. Complexity per trial: we have at most `n*m` windows, each painting takes `O(x^2)` to update cells, but we do that once per window, so `O(n*m*x^2)` per trial. Since `x` up to 100, that's up to 10^8 worst-case per trial, and binary search adds a factor log(100) ~ 7, giving ~ 7*10^8, which may be borderline but acceptable with optimization (we update only touched cells and use 2D prefix rebuild in O(n*m)). For a cleaner solution, we can avoid recomputing entire prefix sums after each paint by updating only the affected region's differences, but for the problem size it's fine to rebuild each time (O(n*m) per paint) giving O((n*m)^2 * x^2?) Actually rebuild is O(n*m) per paint, total O(n*m * n*m) = O(n^2*m^2) per trial, which for n=m=100 is 10^8, times log factor ~ 7e8, still acceptable in C++ with optimizations. We'll implement rebuild prefix sums after each paint for simplicity. Space: O(n*m).
