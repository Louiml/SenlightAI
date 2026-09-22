// Given a grid with dimensions \(n\) rows and \(m\) columns, where you may move only right or down, and a set of \(k\) "special" cells, write a C++ function `int countPaths(int n, int m, const vector<pair<int,int>>& special)` that returns the number of paths from the top-left cell \((1,1)\) to the bottom-right cell \((n,m)\) that do **not** pass through any special cell. The result must be modulo \(10^9+7\). Coordinates are 1-indexed, and special cells are guaranteed to be distinct and not equal to \((1,1)\) or \((n,m)\). All input values fit in `int`, but intermediate calculations must use `long long` to avoid overflow. A valid path consists of exactly \(n-1\) down moves and \(m-1\) right moves in any order.

The solution uses the principle of inclusion–exclusion via dynamic programming over the special cells, sorted by row then column. First, precompute factorials and inverse factorials up to \(n+m\) modulo \(10^9+7\) to calculate combination counts in \(O(1)\). The total number of paths from \((a,b)\) to \((c,d)\) (only moving right/down, with \(a\le c\), \(b\le d\)) is \(\binom{(c-a)+(d-b)}{c-a}\). For each special cell \(i\) (sorted), compute `dp[i]` = number of paths from \((1,1)\) to that cell that avoid all previously processed special cells. This is obtained by starting with the total paths from \((1,1)\) to \(p_i\), then subtracting for every earlier special cell \(j\) with \(p_j.r \le p_i.r\) (and \(p_j.l \le p_i.l\) automatically by sorting) the product `dp[j] * number_of_paths_from(p_j to p_i)`. The answer is the total paths from start to finish minus, for every special cell `i`, `dp[i] * number_of_paths_from(p_i to (n,m))`. Edge cases: if a special cell is unreachable from the start (i.e., `dp[i]` becomes 0 naturally), it contributes nothing; the sort ensures we process cells in topological order, so dependencies are already computed. Complexity is \(O(k^2 + n+m)\) time and \(O(n+m+k)\) space, dominated by the pairwise subtraction loop when \(k\) is large.

#include <vector>
#include <algorithm>
#include <utility>

const int MOD = 1000000007;

// Equality-comparable custom struct for special cells
struct Cell {
    int row, col;
    bool operator<(const Cell& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

// Return number of paths from (r1,c1) to (r2,c2) modulo MOD.
// Assumes r1<=r2 and c1<=c2.
inline long long pathsBetween(int r1, int c1, int r2, int c2,
                              const std::vector<long long>& fact,
                              const std::vector<long long>& invFact) {
    int dr = r2 - r1;
    int dc = c2 - c1;
    int total = dr + dc;
    // C(total, dr) = fact[total] * invFact[dr] * invFact[dc] % MOD
    return fact[total] * invFact[dr] % MOD * invFact[dc] % MOD;
}

// Count valid paths from (1,1) to (n,m) avoiding all special cells.
int countPaths(int n, int m, const std::vector<std::pair<int,int>>& special) {
    int k = (int)special.size();

    // Precompute factorials and inverse factorials
    int maxTotal = n + m;
    std::vector<long long> fact(maxTotal + 1), invFact(maxTotal + 1);
    fact[0] = 1;
    for (int i = 1; i <= maxTotal; ++i) fact[i] = fact[i-1] * i % MOD;

    // Fermat's little theorem: a^(MOD-2) is inverse mod MOD
    invFact[maxTotal] = 1;
    long long base = fact[maxTotal], exponent = MOD - 2;
    while (exponent) {
        if (exponent & 1) invFact[maxTotal] = invFact[maxTotal] * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    for (int i = maxTotal - 1; i >= 0; --i) {
        invFact[i] = invFact[i+1] * (i+1) % MOD;
    }

    // Build sorted list of cells
    std::vector<Cell> cells(k);
    for (int i = 0; i < k; ++i) {
        cells[i].row = special[i].first;
        cells[i].col = special[i].second;
    }
    std::sort(cells.begin(), cells.end());

    // dp[i] = number of paths from start to cell i avoiding all earlier cells
    std::vector<long long> dp(k, 0);
    for (int i = 0; i < k; ++i) {
        dp[i] = pathsBetween(1, 1, cells[i].row, cells[i].col, fact, invFact);
        for (int j = 0; j < i; ++j) {
            if (cells[j].row <= cells[i].row && cells[j].col <= cells[i].col) {
                long long sub = dp[j] * pathsBetween(cells[j].row, cells[j].col,
                                                     cells[i].row, cells[i].col,
                                                     fact, invFact) % MOD;
                dp[i] = (dp[i] - sub + MOD) % MOD;
            }
        }
    }

    // Total paths without restrictions
    long long answer = pathsBetween(1, 1, n, m, fact, invFact);

    // Subtract paths that pass through each special cell as first special cell
    for (int i = 0; i < k; ++i) {
        long long subtract = dp[i] * pathsBetween(cells[i].row, cells[i].col,
                                                  n, m, fact, invFact) % MOD;
        answer = (answer - subtract + MOD) % MOD;
    }

    return (int)answer;
}

#include <cassert>
#include <vector>
#include <utility>

// Forward declaration of the solution function
int countPaths(int n, int m, const std::vector<std::pair<int,int>>& special);

int main() {
    // Simple 2x2 grid with no obstacles: all paths = C(2,1)=2
    assert(countPaths(2, 2, {}) == 2);

    // 1x1 grid: only one empty path
    assert(countPaths(1, 1, {}) == 1);

    // 2x2 with one obstacle in the middle blocks all paths
    assert(countPaths(2, 2, {{1,2}}) == 1);  // the only path going right first is blocked, but down-then-right works
    assert(countPaths(2, 2, {{2,1}}) == 1);  // symmetric

    // Block both possible intermediate cells on a 2x2: no paths
    assert(countPaths(2, 2, {{1,2},{2,1}}) == 0);

    // 3x3 grid, all 6 paths. Block (2,2) which forces detour but still 2 paths exist
    // Total paths = C(4,2)=6. Paths through (2,2) = C(2,1)*C(2,1)=4, so answer=2
    assert(countPaths(3, 3, {{2,2}}) == 2);

    // Same 3x3 but block a corner-adjacent cell (1,2): all paths that go right first are blocked,
    // those that go down first (3 paths) remain valid, so answer=3
    assert(countPaths(3, 3, {{1,2}}) == 3);

    // Larger test: 5x5 no obstacles = C(8,4)=70
    assert(countPaths(5, 5, {}) == 70);

    // 5x5 with a single obstacle at (3,3): total 70 minus paths through (3,3)
    // Paths to (3,3)=C(4,2)=6, paths from (3,3) to (5,5)=C(4,2)=6 => subtract 36 => 34
    assert(countPaths(5, 5, {{3,3}}) == 34);

    // Edge case with obstacle at (2,2) in 2x3 grid
    // Total paths = C(3,1)=3. Only path blocked is the one through (2,2)
    // Paths through (2,2) = C(2,1)*C(1,0)=2, so answer=1
    assert(countPaths(2, 3, {{2,2}}) == 1);

    // Many obstacles: 4x4 with a diagonal of obstacles blocking all paths except one forced route
    // Place obstacles along (1,3),(2,2),(3,1) — then only path is down-down-right-right? Let's compute
    // Total = C(6,3)=20. But we trust the algorithm; just test consistency
    assert(countPaths(4, 4, {{1,3},{2,2},{3,1}}) == 0); // no path avoids all three

    return 0;
}
