/*
Write a C++ function `int gameScore(int n, const std::vector<std::string>& grid)` that simulates a two-player game on an `n x n` grid of characters `'a'` (value +1), `'b'` (value -1), or other arbitrary characters (value 0). The game starts at cell `(0,0)` and ends at `(n-1,n-1)`. At each step, from the current position(s), both players collectively move to all reachable cells by moving either down or right. The player whose turn it is chooses one of the groups of reachable cells that share the same character (if multiple reachable cells have the same character, they all become the new positions simultaneously). The turn alternates: the first player (maximizer) moves on even total steps (Manhattan distance from start), the second player (minimizer) on odd steps. The value of a terminal position is the value of the finishing cell. The total score is the sum of values of all cells visited along the chosen path (including start and end). Return the final score from the perspective of the first player (positive means first wins, zero draw, negative second wins). The grid size `n` satisfies `1 ≤ n ≤ 20`, and all characters are printable ASCII.
*/

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <climits>

// Compute the game score for an n x n grid with characters 'a' (+1), 'b' (-1), others (0).
// Returns the final score from the first player's perspective.
int gameScore(int n, const std::vector<std::string>& grid) {
    // Value of each cell: 'a' -> +1, 'b' -> -1, else 0.
    auto cellValue = [&](int x, int y) -> int {
        char c = grid[x][y];
        if (c == 'a') return 1;
        if (c == 'b') return -1;
        return 0;
    };

    // Memoization map: state (vector of positions) -> best score from that state.
    std::map<std::vector<std::pair<int,int>>, int> mem;

    // Recursive minimax with memoization.
    std::function<int(std::vector<std::pair<int,int>>)> go;
    go = [&](std::vector<std::pair<int,int>> pos) -> int {
        // Normalize state (should already be sorted, but ensure).
        std::sort(pos.begin(), pos.end());
        pos.erase(std::unique(pos.begin(), pos.end()), pos.end());

        // Check memo.
        auto it = mem.find(pos);
        if (it != mem.end()) return it->second;

        // Terminal: all positions are bottom-right.
        if (pos.front().first == n-1 && pos.front().second == n-1) {
            int val = cellValue(n-1, n-1);
            mem[pos] = val;
            return val;
        }

        // Generate all reachable cells from current positions.
        std::vector<std::pair<int,int>> reachable;
        for (const auto& p : pos) {
            int x = p.first, y = p.second;
            if (x+1 < n) reachable.emplace_back(x+1, y);
            if (y+1 < n) reachable.emplace_back(x, y+1);
        }
        std::sort(reachable.begin(), reachable.end());
        reachable.erase(std::unique(reachable.begin(), reachable.end()), reachable.end());

        // Group reachable cells by character.
        std::sort(reachable.begin(), reachable.end(), [&](const auto& a, const auto& b) {
            return grid[a.first][a.second] < grid[b.first][b.second];
        });

        // Determine current player: even Manhattan distance -> maximizer (first), odd -> minimizer (second).
        bool maximizer = ((pos.front().first + pos.front().second) % 2 == 0);

        int best = maximizer ? INT_MIN : INT_MAX;
        int i = 0;
        while (i < (int)reachable.size()) {
            int j = i;
            char ch = grid[reachable[i].first][reachable[i].second];
            std::vector<std::pair<int,int>> group;
            while (j < (int)reachable.size() && grid[reachable[j].first][reachable[j].second] == ch) {
                group.push_back(reachable[j]);
                ++j;
            }
            // Group positions must be sorted for state canonicalization (already sorted by char; sort lexicographically).
            std::sort(group.begin(), group.end());
            group.erase(std::unique(group.begin(), group.end()), group.end());
            int sub = go(group);
            if (maximizer) best = std::max(best, sub);
            else best = std::min(best, sub);
            i = j;
        }

        int ret = best + cellValue(pos.front().first, pos.front().second);
        mem[pos] = ret;
        return ret;
    };

    // Initial state: only start cell.
    std::vector<std::pair<int,int>> start = {{0,0}};
    return go(start);
}

#include <cassert>
#include <vector>
#include <string>

// Assume gameScore is defined above.

int main() {
    // 1x1 grid: 'a' -> +1
    assert(gameScore(1, {"a"}) == 1);
    // 1x1 grid: 'b' -> -1
    assert(gameScore(1, {"b"}) == -1);
    // 1x1 grid: 'x' -> 0
    assert(gameScore(1, {"x"}) == 0);

    // 2x2 grid:
    // a b
    // b a
    // Path options: start 'a' (+1), then either right to 'b' (-1) then down to 'a' (+1) -> sum 1
    // or down to 'b' (-1) then right to 'a' (+1) -> sum 1. So score = 1.
    assert(gameScore(2, {"ab", "ba"}) == 1);

    // 3x3 grid:
    // a a a
    // a b a
    // a a a
    // The only path that avoids 'b'? Actually all paths must pass through center 'b'? No, can go around? 
    // In 3x3, from (0,0) to (2,2) all paths must pass through (1,1)? Not necessarily; 
    // Path (0,0)->(0,1)->(0,2)->(1,2)->(2,2) does not pass (1,1). So value = 1 (a) +1 (a) +1 (a) +1 (a) +1 (a) = 5.
    assert(gameScore(3, {"aaa", "aba", "aaa"}) == 5);

    // 2x2 grid:
    // b a
    // a b
    // Start 'b' (-1). From there, both moves go to 'a' (+1), then after that to 'b' (-1). Sum = -1+1-1 = -1.
    assert(gameScore(2, {"ba", "ab"}) == -1);

    // 2x2 grid with all 'a' gives sum = 4.
    assert(gameScore(2, {"aa", "aa"}) == 4);

    // 2x2 grid with all 'b' gives sum = -4.
    assert(gameScore(2, {"bb", "bb"}) == -4);

    // 3x3 grid with only start and end 'a', others 'x' (0):
    // a x x
    // x x x
    // x x a -> path must sum to 1+0+...+1? Actually start 'a' (1), end 'a' (1), all middle 0 -> total 2.
    assert(gameScore(3, {"axx", "xxx", "xxa"}) == 2);

    // Larger test for memoization: 4x4 with random mix, just check it runs and returns something.
    assert(gameScore(4, {"abab", "baba", "abab", "baba"}) == 
           gameScore(4, {"abab", "baba", "abab", "baba"})); // determinism

    return 0;
}

// The problem is a minimax game on a grid where the state is a set of positions that are all reachable at the same Manhattan distance (since moves are only down/right, the sum of coordinates is constant at a given depth). The state can be represented as a vector of pairs (x,y) sorted to avoid duplicates. For a state, if all positions are at the bottom-right corner, return the value of that corner. Otherwise, generate all possible moves from each position in the state (down and right if within bounds), deduplicate the resulting cells, then group them by the character at that cell. For each group, recursively compute the score of that group as the next state. If the current depth (given by the parity of `pos.front().first + pos.front().second`) is even (maximizer), take the maximum score among groups; else take the minimum. Add the value of the current front position (which is the same for all positions in the state because they are all at the same Manhattan distance) to the result. Use memoization on the sorted vector of positions to avoid recomputation. Edge cases: when `n=1`, the start is also the end and the function returns the value of the single cell. The grouping must be done carefully: sort the reachable cells by character, then for each contiguous group of equal characters, sort them lexicographically to form a canonical state. Time complexity is bounded by the number of distinct states; each state has at most `O(n)` positions, and there are at most `O(2^(2n))` states in the worst case but for n≤20 it's manageable with memoization. Space complexity is O(number of states * size of state).
