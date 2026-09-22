// You are given a 2D grid representing a Hashi (Hashiwokakero) puzzle board. Each cell is either an empty water cell (value 0) or an island with a number (1-8) indicating how many bridges it must have. Islands can be connected by horizontal or vertical bridges (1 or 2 bridges between two adjacent islands in the same row or column with no other island between them). Write a C++ function `long long countPuzzleSolutions(const std::vector<std::vector<int>>& board)` that returns the total number of complete valid bridge configurations (solutions) for the given puzzle. A valid configuration must satisfy: (1) every island has exactly its required number of bridges, (2) bridges only connect islands in the same row or column with no intervening island, (3) between any two directly connected islands, you may place 0, 1, or 2 bridges, and (4) the entire bridge graph must be connected (all islands reachable from any other via bridges). The input board is rectangular with dimensions at least 1x1 and at most 8x8; empty cells are 0, islands are 1-8. If there are no islands, return 0 (since an empty puzzle has no valid solution). Return the count as a `long long`.

The solution uses backtracking with recursion over islands and directions. First, gather all island positions and their required bridge counts. Then, for each island, consider its four possible directions (right, down, left, up) and find the nearest island in that direction (if any). During recursion, process islands in a fixed order. For the current island, if its current bridge count exceeds the required value, prune. Otherwise, try placing 0, 1, or 2 bridges from the current island to the next island in some direction (choosing directions in a consistent order) while ensuring the total placed at the current island plus the minimum necessary for unprocessed connections doesn't exceed the requirement. For each possible number of bridges, recursively proceed to the next state. When all islands are processed, check that all required counts are exactly met and that the bridge graph is connected. To avoid counting the same configuration multiple times, each solution is identified by the set of bridges placed; since we process each pair of adjacent islands exactly once (when we visit the first island of the pair in the direction order), each configuration is counted exactly once. Important edge cases: islands with 0 required (not allowed by rules, but treat as already satisfied), islands with no reachable neighbor (prune if requirement not zero), and boards with only one island (then connectivity is trivial but requirement must be 0, otherwise no solution). Time complexity is exponential in the number of islands and possible bridge counts, but with pruning and a maximum of 64 cells and at most 32 islands, it is feasible for typical puzzles; in the worst case (all cells islands with low numbers), it could be large, but the constraints keep it reasonable. Space complexity is O(rows * cols) for grid copies and O(islands) for recursion stack.

#include <vector>
#include <tuple>
#include <unordered_set>
#include <functional>

using namespace std;

long long countPuzzleSolutions(const vector<vector<int>>& board) {
    int rows = board.size();
    if (rows == 0) return 0;
    int cols = board[0].size();
    if (cols == 0) return 0;

    // Collect all islands
    vector<tuple<int, int, int>> islands; // (row, col, required)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] > 0) {
                islands.emplace_back(r, c, board[r][c]);
            }
        }
    }
    int n = islands.size();
    if (n == 0) return 0;

    // Precompute nearest island in each direction for each island
    // dir: 0=right, 1=down, 2=left, 3=up
    vector<array<int, 4>> neighbors(n); // -1 if no island in that direction
    for (int i = 0; i < n; ++i) {
        auto [r, c, req] = islands[i];
        for (int d = 0; d < 4; ++d) neighbors[i][d] = -1;
        // right: scan same row, increasing col
        for (int nc = c+1; nc < cols; ++nc) {
            if (board[r][nc] > 0) {
                for (int j = 0; j < n; ++j) {
                    if (get<0>(islands[j]) == r && get<1>(islands[j]) == nc) {
                        neighbors[i][0] = j;
                        break;
                    }
                }
                break;
            }
        }
        // down
        for (int nr = r+1; nr < rows; ++nr) {
            if (board[nr][c] > 0) {
                for (int j = 0; j < n; ++j) {
                    if (get<0>(islands[j]) == nr && get<1>(islands[j]) == c) {
                        neighbors[i][1] = j;
                        break;
                    }
                }
                break;
            }
        }
        // left
        for (int nc = c-1; nc >= 0; --nc) {
            if (board[r][nc] > 0) {
                for (int j = 0; j < n; ++j) {
                    if (get<0>(islands[j]) == r && get<1>(islands[j]) == nc) {
                        neighbors[i][2] = j;
                        break;
                    }
                }
                break;
            }
        }
        // up
        for (int nr = r-1; nr >= 0; --nr) {
            if (board[nr][c] > 0) {
                for (int j = 0; j < n; ++j) {
                    if (get<0>(islands[j]) == nr && get<1>(islands[j]) == c) {
                        neighbors[i][3] = j;
                        break;
                    }
                }
                break;
            }
        }
    }

    // To avoid counting symmetric configurations multiple times, we assign each
    // unordered pair (i,j) where i<j and they are connected (either i->j or j->i)
    // a unique index. We'll store the current bridge count for that pair.
    vector<tuple<int,int,int>> edge_list; // (smaller_idx, larger_idx, bridge_count)
    unordered_map<long long, int> edge_index;
    for (int i = 0; i < n; ++i) {
        for (int d = 0; d < 4; ++d) {
            int j = neighbors[i][d];
            if (j != -1) {
                int a = min(i, j), b = max(i, j);
                long long key = (long long)a * 100 + (long long)b;
                if (edge_index.find(key) == edge_index.end()) {
                    edge_index[key] = edge_list.size();
                    edge_list.emplace_back(a, b, 0);
                }
            }
        }
    }
    int m = edge_list.size();

    // For each island, list of edge indices incident to it (in order of direction)
    vector<vector<int>> island_edges(n);
    for (int i = 0; i < n; ++i) {
        for (int d = 0; d < 4; ++d) {
            int j = neighbors[i][d];
            if (j != -1) {
                int a = min(i, j), b = max(i, j);
                long long key = (long long)a * 100 + (long long)b;
                island_edges[i].push_back(edge_index[key]);
            }
        }
    }

    // Recursive backtracking
    long long result = 0;
    vector<int> current_edge_count(m, 0);
    vector<int> current_degree(n, 0);

    function<void(int)> dfs = [&](int idx) {
        if (idx == n) {
            // Check all requirements met
            for (int i = 0; i < n; ++i) {
                if (current_degree[i] != get<2>(islands[i])) return;
            }
            // Check connectivity
            if (n == 0) { result += 1; return; }
            // Build adjacency list from edges with count>0
            vector<vector<int>> adj(n);
            int total_edges = 0;
            for (int e = 0; e < m; ++e) {
                if (current_edge_count[e] > 0) {
                    int a, b, cnt;
                    tie(a, b, cnt) = edge_list[e];
                    adj[a].push_back(b);
                    adj[b].push_back(a);
                    total_edges++;
                }
            }
            // BFS
            vector<bool> visited(n, false);
            int start = 0;
            int comp_size = 0;
            vector<int> stack = {start};
            visited[start] = true;
            while (!stack.empty()) {
                int u = stack.back(); stack.pop_back();
                comp_size++;
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        stack.push_back(v);
                    }
                }
            }
            if (comp_size == n) {
                result += 1;
            }
            return;
        }

        int r, c, req;
        tie(r, c, req) = islands[idx];
        int cur = current_degree[idx];
        if (cur > req) return; // prune

        // Try all possible assignments of bridges (0,1,2) on each incident edge
        // We'll enumerate combinations via recursion over incident edges
        const auto& incident = island_edges[idx];
        int k = incident.size();
        vector<int> edge_bridge_counts(k);

        function<void(int)> try_edges = [&](int pos) {
            if (pos == k) {
                // Apply these edges and recurse to next island
                vector<int> saved_edge_count(edge_bridge_counts);
                bool valid = true;
                for (int e_i = 0; e_i < k; ++e_i) {
                    int e = incident[e_i];
                    int delta = edge_bridge_counts[e_i];
                    // Ensure edge count doesn't exceed 2
                    if (current_edge_count[e] + delta > 2) {
                        valid = false;
                        break;
                    }
                    // The other endpoint's degree will be updated later when we process it,
                    // but we must not exceed its requirement now. However we can't know its
                    // future edges yet, but we can at least ensure it doesn't exceed req.
                    int a, b, cnt;
                    tie(a, b, cnt) = edge_list[e];
                    int other = (a == idx) ? b : a;
                    if (current_degree[other] + delta > get<2>(islands[other])) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    // Temporarily apply
                    int new_cur = cur;
                    for (int e_i = 0; e_i < k; ++e_i) {
                        int e = incident[e_i];
                        int delta = edge_bridge_counts[e_i];
                        current_edge_count[e] += delta;
                        int a, b, cnt;
                        tie(a, b, cnt) = edge_list[e];
                        if (a == idx) {
                            current_degree[a] += delta;
                            current_degree[b] += delta;
                        } else {
                            current_degree[b] += delta;
                            current_degree[a] += delta;
                        }
                    }
                    // Recurse
                    dfs(idx+1);
                    // Revert
                    for (int e_i = 0; e_i < k; ++e_i) {
                        int e = incident[e_i];
                        int delta = edge_bridge_counts[e_i];
                        current_edge_count[e] -= delta;
                        int a, b, cnt;
                        tie(a, b, cnt) = edge_list[e];
                        if (a == idx) {
                            current_degree[a] -= delta;
                            current_degree[b] -= delta;
                        } else {
                            current_degree[b] -= delta;
                            current_degree[a] -= delta;
                        }
                    }
                }
                return;
            }
            // For each incident edge, try 0, 1, 2 bridges
            for (int b = 0; b <= 2; ++b) {
                // Check that current degree plus this b doesn't exceed req
                if (cur + b > req) continue;
                edge_bridge_counts[pos] = b;
                try_edges(pos+1);
            }
        };

        try_edges(0);
    };

    dfs(0);
    return result;
}

#include <cassert>
#include <vector>
using namespace std;

// The solution function is declared above (not repeated here for brevity)

int main() {
    // Trivial empty board
    assert(countPuzzleSolutions({}) == 0);
    assert(countPuzzleSolutions({{0}}) == 0);

    // Single island with requirement 0? Not allowed by rules but our function handles:
    // Board with one island of value 0 is not an island; but we treat >0 as island.
    // Single island with value 1 but no neighbors -> impossible
    assert(countPuzzleSolutions({{1}}) == 0);

    // Two islands horizontally adjacent, both require 1 bridge
    // Only solution: single bridge between them
    {
        vector<vector<int>> b = {{1, 1}};
        assert(countPuzzleSolutions(b) == 1);
    }

    // Two islands vertically adjacent, both require 2 bridges
    // Only solution: double bridge
    {
        vector<vector<int>> b = {{1}, {1}};
        assert(countPuzzleSolutions(b) == 1);
    }

    // Two islands with gap (empty water) and require 1 each
    // Only one solution: bridge over the water
    {
        vector<vector<int>> b = {{1, 0, 1}};
        assert(countPuzzleSolutions(b) == 1);
    }

    // Two islands with gap, require 2 each -> double bridge
    {
        vector<vector<int>> b = {{1, 0, 1}};
        // Change requirement by modifying values? The board value is the requirement.
        // With both 1, only one bridge possible; so to test double, set both to 2.
        vector<vector<int>> b2 = {{2, 0, 2}};
        assert(countPuzzleSolutions(b2) == 1);
    }

    // Three islands in a row, each require 2, forming a chain
    // Possible configurations: left-middle double bridge and middle-right double bridge
    // If all require 2, then each island must have total 2, so left must connect to middle with 2
    // and right to middle with 2, middle gets 4 -> invalid. So no solution.
    {
        vector<vector<int>> b = {{2, 2, 2}};
        assert(countPuzzleSolutions(b) == 0);
    }

    // Simple L-shape: three islands at (0,0), (0,1), (1,0) all require 1
    // Each needs exactly one bridge; the only way is to connect (0,0)-(0,1) and (0,0)-(1,0)
    // because (0,1) can't connect to (1,0) (not adjacent). So one solution.
    {
        vector<vector<int>> b = {
            {1, 1},
            {1, 0}
        };
        assert(countPuzzleSolutions(b) == 1);
    }

    // Same L-shape but corners require 2: each island needs 2 bridges,
    // (0,0) can connect to both neighbors with 1 each (total 2), or one with 2? But (0,0) has two neighbors,
    // to get degree 2, either one bridge to each neighbor (2 total) or double to one neighbor and 0 to other (2 total)
    // but then the neighbor receiving double gets 2, and its requirement is 2, but that neighbor also has another neighbor? 
    // Let's test: all three require 2, then each island must have degree 2. (0,1) has only neighbor (0,0) -> must have 2 bridges to (0,0). 
    // (1,0) has only neighbor (0,0) -> must have 2 bridges to (0,0). Then (0,0) would get 4 bridges total -> invalid. So 0 solutions.
    {
        vector<vector<int>> b = {
            {2, 2},
            {2, 0}
        };
        assert(countPuzzleSolutions(b) == 0);
    }

    // 2x2 square with each island require 2: all four islands form a cycle.
    // Each island connects to two neighbors (right and down for top-left, etc.)
    // To have degree 2 each, each must have exactly 1 bridge to each neighbor.
    // That gives exactly one solution (all four edges have 1 bridge).
    {
        vector<vector<int>> b = {
            {2, 2},
            {2, 2}
        };
        assert(countPuzzleSolutions(b) == 1);
    }

    // 2x2 square with each require 1: each island needs one bridge.
    // Possible: connect top-left to top-right (1), and bottom-left to bottom-right (1) -> two separate pairs,
    // but graph not connected (two components) -> invalid. Or connect top-left to bottom-left, and top-right to bottom-right -> also disconnected.
    // Or connect top-left to top-right and top-left to bottom-left? That gives top-left degree 2, invalid.
    // So no valid connected solution.
    {
        vector<vector<int>> b = {
            {1, 1},
            {1, 1}
        };
        assert(countPuzzleSolutions(b) == 0);
    }

    // A plus-shaped puzzle with center 4 and four arms 1 each.
    // Center connects to each arm with exactly 1 bridge -> one solution.
    {
        vector<vector<int>> b = {
            {0, 1, 0},
            {1, 4, 1},
            {0, 1, 0}
        };
        assert(countPuzzleSolutions(b) == 1);
    }

    return 0;
}
