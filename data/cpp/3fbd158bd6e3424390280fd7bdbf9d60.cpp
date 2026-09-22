/*
Given a rectangular grid of size \(n \times m\) containing only the characters '.' (empty) and '*' (blocked), write a C++ function `std::vector<std::string> makeUniqueTiling(int n, int m, const std::vector<std::string>& grid)` that returns a vector of strings of the same dimensions. For each empty cell, if a valid domino tiling exists and is **unique**, replace each '.' with a directional arrow (`'^'`, `'v'`, `'<'`, `'>'`) indicating which adjacent empty cell its matched domino partner lies in. If no valid tiling exists or the tiling is not unique, return a vector containing a single string `"Not unique"`. The function must not modify the input grid; treat blocked cells as permanently unavailable. Constraint: \(1 \le n, m \le 2000\), total empty cells up to \(4 \times 10^6\).
*/
#include <vector>
#include <string>
#include <queue>

// Returns a unique domino tiling of empty cells as directional arrows,
// or a vector with a single string "Not unique" if no unique tiling exists.
std::vector<std::string> makeUniqueTiling(int n, int m, const std::vector<std::string>& grid) {
    const int N = n * m;
    std::vector<int> degree(N, 0);
    std::vector<int> match(N, -1);
    
    // Helper to convert (i,j) to flattened index; assumes 0-based (i,j)
    auto id = [m](int i, int j) { return i * m + j; };

    // Initialize degrees for all empty cells
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != '.') continue;
            int deg = 0;
            if (i > 0 && grid[i-1][j] == '.') ++deg;
            if (i+1 < n && grid[i+1][j] == '.') ++deg;
            if (j > 0 && grid[i][j-1] == '.') ++deg;
            if (j+1 < m && grid[i][j+1] == '.') ++deg;
            degree[id(i,j)] = deg;
        }
    }

    // Queue of vertices with degree 1
    std::queue<int> leaves;
    for (int v = 0; v < N; ++v) {
        int i = v / m, j = v % m;
        if (grid[i][j] == '.' && degree[v] == 1) {
            leaves.push(v);
        }
    }

    // Process forced matches
    auto neighbor_id = [&](int i, int j, int dir) -> int {
        switch (dir) {
            case 0: return (i > 0) ? id(i-1,j) : -1; // up
            case 1: return (i+1 < n) ? id(i+1,j) : -1; // down
            case 2: return (j > 0) ? id(i,j-1) : -1; // left
            default: return (j+1 < m) ? id(i,j+1) : -1; // right
        }
    };

    while (!leaves.empty()) {
        int v = leaves.front();
        leaves.pop();
        if (match[v] != -1 || degree[v] != 1) continue; // already matched or no longer leaf
        int vi = v / m, vj = v % m;
        if (grid[vi][vj] != '.') continue;

        // Find the unique unmatched neighbor
        int u = -1;
        for (int d = 0; d < 4; ++d) {
            int cand = neighbor_id(vi, vj, d);
            if (cand != -1 && grid[cand/m][cand%m] == '.' && match[cand] == -1) {
                u = cand;
                break;
            }
        }
        if (u == -1) continue; // shouldn't happen if degree==1, but safety

        // Match v and u
        match[v] = u;
        match[u] = v;
        degree[v] = 0;
        degree[u] = 0;

        // Decrease degree of all neighbors of u (except v)
        int ui = u / m, uj = u % m;
        for (int d = 0; d < 4; ++d) {
            int cand = neighbor_id(ui, uj, d);
            if (cand != -1 && cand != v && match[cand] == -1 && grid[cand/m][cand%m] == '.') {
                --degree[cand];
                if (degree[cand] == 1) {
                    leaves.push(cand);
                }
            }
        }
    }

    // Check if all empty cells are matched
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '.' && match[id(i,j)] == -1) {
                return {"Not unique"};
            }
        }
    }

    // Build result with arrows
    std::vector<std::string> result = grid;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != '.') continue;
            int v = id(i, j);
            int u = match[v];
            if (u == -1) continue; // should not happen
            int ui = u / m, uj = u % m;
            if (ui == i - 1) result[i][j] = '^'; // partner above
            else if (ui == i + 1) result[i][j] = 'v'; // partner below
            else if (uj == j - 1) result[i][j] = '<'; // partner left
            else if (uj == j + 1) result[i][j] = '>'; // partner right
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// (the solution function is assumed to be defined above)

int main() {
    // 1. Simple 1x2 fully empty: unique
    {
        std::vector<std::string> g = {".."};
        auto r = makeUniqueTiling(1, 2, g);
        assert(r.size() == 1);
        assert(r[0] == "<>" || r[0] == "><"); // either orientation accepted
    }
    // 2. 2x2 fully empty: not unique (two possible tilings)
    {
        std::vector<std::string> g = {"..", ".."};
        auto r = makeUniqueTiling(2, 2, g);
        assert(r.size() == 1 && r[0] == "Not unique");
    }
    // 3. L-shaped 3 cells (1x3 minus one) is impossible -> not unique
    {
        std::vector<std::string> g = {".*", ".."}; // 3 cells, odd count -> impossible
        auto r = makeUniqueTiling(2, 2, g);
        assert(r.size() == 1 && r[0] == "Not unique");
    }
    // 4. Single empty cell isolated -> not unique
    {
        std::vector<std::string> g = {"*."};
        auto r = makeUniqueTiling(1, 2, g);
        assert(r.size() == 1 && r[0] == "Not unique");
    }
    // 5. Empty grid (no dots) -> returns original grid unchanged
    {
        std::vector<std::string> g = {"***", "***"};
        auto r = makeUniqueTiling(2, 3, g);
        assert(r == g);
    }
    // 6. Vertical 2x1 -> unique
    {
        std::vector<std::string> g = {".", "."};
        auto r = makeUniqueTiling(2, 1, g);
        assert(r.size() == 2);
        assert(r[0] == "v" && r[1] == "^");
    }
    // 7. 3x3 with a unique spiral tiling – construct forced dominoes
    // Pattern:
    // . . *
    // * . .
    // . . .
    // Known unique tiling: (0,0)-(0,1), (1,1)-(2,1), (2,0)-(2,2)? No, that leaves odd. Let's use a known unique shape.
    // Instead, test a simple 2x3 zigzag:
    // . * .
    // . . .
    // This tilings? Cells: (0,0),(0,2),(1,0),(1,1),(1,2) = 5 -> odd, impossible.
    // Use a 2x4 with unique tiling: a path of 8 cells in a snake shape.
    // Let's skip complex because uniqueness is hard to verify by hand; just test that output is consistent.
    // We'll test a 2x2 with a blocked cell that forces unique match.
    // Grid: ". ." / "* ." -> cells: (0,0),(0,1),(1,1) = 3 odd -> impossible.
    // Use a 2x3 with blocked middle: 4 cells in a square? Actually 2x2 with one blocked gives 3 cells -> impossible.
    // Use 1x4 empty: two possible tilings? (horizontal pairs) actually exactly 1 tiling for even length: all horizontal pairs. So unique.
    {
        std::vector<std::string> g = {"...."};
        auto r = makeUniqueTiling(1, 4, g);
        assert(r.size() == 1);
        // Only tiling: (0,0)-(0,1) and (0,2)-(0,3) -> arrows: "><><"? Wait each cell points to neighbor:
        // cell0->1 right '>', cell1->0 left '<', cell2->3 '>', cell3->2 '<' -> string "><><"
        assert(r[0] == "><><");
    }
    // 8. Larger test: 2x2 with one blocked corner -> 3 cells impossible
    {
        std::vector<std::string> g = {".*", ".."};
        auto r = makeUniqueTiling(2, 2, g);
        assert(r.size() == 1 && r[0] == "Not unique");
    }
    std::cout << "All tests passed.\n";
    return 0;
}
// This is a bipartite matching uniqueness problem on a grid, where empty cells are vertices and edges connect orthogonally adjacent empty cells. The grid is naturally bipartite by parity of \((i+j)\): even parity cells on the left, odd on the right. A valid tiling corresponds to a perfect matching in this bipartite graph. The key insight for uniqueness: if there exists a cell with degree 1 (a "forced" leaf), its only neighbor must be its match. We can process leaves in a queue-like fashion: when a leaf `v` is matched to its sole neighbor `u`, remove both vertices from the graph, and for every neighbor of `u`, decrement its degree; if that degree becomes 1, it becomes a new leaf. This greedy propagation either finds a unique perfect matching (if all empty vertices get matched) or fails (if some vertices remain unmatched). After processing all forced matches, any remaining unmatched empty vertex means no unique tiling. We must also consider the possibility that no perfect matching exists at all — the greedy algorithm leaves some vertices unmatched, so we output "Not unique" (since a perfect matching is required for a unique tiling). Edge cases: isolated empty cells (degree 0) immediately make a tiling impossible; a grid with no empty cells returns an empty vector of strings? Actually the grid has at least one cell, but if there are zero empty cells, the tiling is trivially unique? The problem statement says tiling exists if all empty cells are covered by dominoes; with zero empty cells, the empty tiling is unique, so we should return the grid with no arrows? The original code does not handle zero empty cells specially; it would output the grid as-is (since all cells are '*', match is zero, ok becomes true? Wait, the code checks `match[codif(i,j)] == 0 and el[i][j] == '.'` — if no '.' cells, ok stays true, and prints the grid unchanged. So our function should return the grid unchanged when there are no '.' cells. We'll incorporate that. Time complexity: each vertex and edge is processed a constant number of times. Total vertices up to \(4 \times 10^6\), edges roughly \(2 \times\) adjacency, so \(O(nm)\) time and \(O(nm)\) space. We must be careful with memory: storing adjacency lists for up to 4 million vertices with up to 4 edges each is fine (about 32MB for vectors of ints). But we can optimize by not storing all adjacency lists explicitly; instead, for each cell, compute neighbors on the fly when needed. Since the greedy algorithm needs to iterate over neighbors of a matched vertex, we can compute them from coordinates. But we also need to track degrees and whether a vertex is already matched. We'll store `degree` as an int array (size \(n \times m\), 4 bytes each = 16MB for 4M), `match` as an int array (similar), plus a queue for leaves. We'll flatten coordinates to a single index `id = i*m + j`. For each empty cell, compute degree by checking four directions. Then process leaves: for each leaf, find its only unmatched neighbor, set match both ways, and for every neighbor of that neighbor, decrement their degree. Continue until queue is empty. Finally, check all empty cells: if any has match == -1, return "Not unique"; otherwise, build the arrow grid. Important: when processing a leaf, its only neighbor might already be matched? But we ensure leaf has degree 1 and is unmatched, so its neighbor is unmatched. Also, after matching, we must skip updating the degree of the leaf itself or the matched partner; we only decrement neighbors of the matched partner that are not the leaf and not already matched. Edge case: two adjacent leaves that are only neighbors of each other — they match; their degrees both become 0 after decrement? Actually, initial degree of each is 2? No, if two cells are adjacent and each has only that one neighbor (e.g., two isolated cells in a 1x2 grid), each degree is 1, so both are leaves. We process one leaf, match it to the other, then when we decrement neighbors of the other, we skip the leaf (already matched), and no other neighbors. The second leaf is now matched and removed. That works. But to avoid double-processing, we only add a vertex to the queue when its degree becomes exactly 1 and it's not matched. We also need to handle the case where a vertex initially has degree 0 — immediately not unique. We'll check that at the start or when processing queue? Actually if any empty cell has degree 0, it can never be matched, so return "Not unique". So we can pre-check.
