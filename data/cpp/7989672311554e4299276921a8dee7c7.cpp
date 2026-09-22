// You are given a rectangular grid of size \( n \times m \) where each cell is either a wall (`#`), empty (`.`), or initially occupied by one of \( p \) players (digits `'1'` to `'9'`). Each player \( i \) has a movement speed \( s[i] \) (a positive integer), meaning that in a single expansion round, player \( i \) may move up to \( s[i] \) steps outward in the four cardinal directions from all their currently owned cells simultaneously. Occupied cells cannot be revisited by any player, and walls cannot be crossed. The expansion process proceeds in rounds: in each round, players take turns in order \( 1, 2, \dots, p \). In a player’s turn, they expand from all their current territories up to their speed limit, claiming any reachable empty cells. If a player has no territories at the start of a round, they skip. The process continues until no player can expand further. Write a C++ function that, given the grid dimensions, the number of players, their speeds, and the initial grid, returns a `std::vector<int>` of length \( p \) where the \( i \)-th element is the total number of cells controlled by player \( i+1 \) (including their initial cells) after the process ends. The function signature is:
// ```cpp
// std::vector<int> castleConquest(int n, int m, int p, const std::vector<int>& speed, const std::vector<std::string>& grid);
// ```
// Ensure the function handles up to \( n, m \le 1000 \), \( p \le 9 \), with speeds up to \( 10^9 \), and the total empty cells can be large. Optimize so that you do not simulate each step of a large speed individually, but use a group‑expansion per turn (like BFS layers).
The given snippet uses a multi‑source BFS per player, but expands exactly `speed` steps per turn, which can be too slow if `speed` is huge (like 1e9) because each step is iterated individually even if there are no reachable cells after the first few steps. A better approach is to expand from each player’s queue in a wave‑by‑wave manner, but stop early if the queue becomes empty. However, even with that, if a player has a large speed and there are many empty cells, the BFS will naturally process each cell once, so the total work is proportional to the number of cells claimed, not the speed value. The key is to avoid looping `speed` times when the queue is empty; we already break if `qS == 0`. The original code iterates `for(int i=0;i<s[now];i++)` but breaks when the queue is empty, so it’s fine. But to be safe for large `speed`, we can use the same pattern: for each player, perform a BFS layer‑by‑layer, but limit to `speed` layers and stop if no new cells are claimed. Each cell is visited once, so time complexity is \( O(n \cdot m) \) overall because all BFS expansions together touch each empty cell at most once. The original code uses a queue per player and processes each turn for all players until no progress. That is correct. The main edge cases: initial cells already occupy some positions, walls are impassable, multiple players may start adjacent to each other – the first player in order gets to claim a cell if it’s empty at the time of their turn (since turns are sequential). Also, a player may have speed 0? Problem says positive integer, so assume ≥1. The function should return counts including initial cells. The solution below implements the BFS per player with a queue, and for each turn, we only expand up to `speed[i]` steps, but if no cells are available, we stop early. To avoid O(speed) per player per round even when queue is empty, we check queue emptiness before the inner loop. Also, to prevent infinite looping, we keep a global flag `changed` that becomes true if any player claimed new cells in the current round; if no player claims, we break. The original snippet uses a counter `ck` for empty queues but does not track changed, instead it repeatedly loops until all queues empty. That works because once a player can’t expand, their queue becomes empty and stays empty. So the loop condition is fine. The solution will mimic that. Time complexity: each cell is processed at most once per player? Actually each empty cell is claimed by exactly one player, so total BFS expansion across all players is O(n*m). There is overhead for checking each player per round, but number of rounds is at most max(n,m) because each round a player expands at least one step if possible, and the total steps across all players is limited by grid size. So overall O(n*m) time and O(n*m) space for the grid and queues.
#include <vector>
#include <string>
#include <queue>
#include <utility>

// Expands all players' territories according to the described rules.
// Returns a vector of size p, ans[i] = number of cells controlled by player i+1.
std::vector<int> castleConquest(int n, int m, int p,
                                const std::vector<int>& speed,
                                const std::vector<std::string>& grid) {
    // Directions: up, down, left, right
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // Grid: 0 = empty, -1 = wall, positive = player id (1..p)
    std::vector<std::vector<int>> board(n, std::vector<int>(m, 0));
    std::queue<std::pair<int, int>> queues[10];  // p ≤ 9, so index 1..p
    std::vector<int> ans(p + 1, 0);              // 1-indexed for convenience

    // Initialize grid, queues, and answer counts
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char ch = grid[i][j];
            if (ch == '#') {
                board[i][j] = -1;
            } else if (ch >= '1' && ch <= '9') {
                int player = ch - '0';
                board[i][j] = player;
                queues[player].push({i, j});
                ans[player]++;
            }
        }
    }

    // Process expansion rounds until no player can move
    bool changed = true;
    while (changed) {
        changed = false;
        for (int player = 1; player <= p; ++player) {
            // If this player has no territories, skip
            if (queues[player].empty()) continue;
            
            // Expand up to speed[player] steps
            int steps = speed[player - 1];  // speed vector is 0-indexed
            for (int s = 0; s < steps; ++s) {
                int qSize = queues[player].size();
                if (qSize == 0) break;  // no more cells to expand from
                bool claimed = false;
                while (qSize--) {
                    int x = queues[player].front().first;
                    int y = queues[player].front().second;
                    queues[player].pop();
                    
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
                        if (board[nx][ny] != 0) continue;  // not empty
                        
                        board[nx][ny] = player;
                        queues[player].push({nx, ny});
                        ans[player]++;
                        claimed = true;
                    }
                }
                if (!claimed) {
                    // No new cells claimed in this step, so the remaining steps are useless
                    break;
                }
                changed = true;
            }
        }
    }

    // Build result vector (0-indexed for players 1..p)
    std::vector<int> result(p);
    for (int i = 1; i <= p; ++i) {
        result[i - 1] = ans[i];
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function declaration (assumed to be included above)

int main() {
    // Example 1: Simple 2x2, two players starting adjacent
    {
        int n = 2, m = 2, p = 2;
        std::vector<int> speed = {1, 1};
        std::vector<std::string> grid = {"12", ".."};
        auto res = castleConquest(n, m, p, speed, grid);
        assert(res == std::vector<int>({2, 2})); // '1' takes (0,1)? Actually initial: (0,0)='1', (0,1)='2'. Empty: (1,0),(1,1). Round: player1 claims (1,0), player2 claims (1,1). Final counts 2 each.
    }

    // Example 2: Wall blocks expansion
    {
        int n = 3, m = 3, p = 1;
        std::vector<int> speed = {5};
        std::vector<std::string> grid = {"1#.", ".#.", "..."};
        auto res = castleConquest(n, m, p, speed, grid);
        // Player1 can reach (0,2)? From (0,0) go right blocked by wall at (0,1). Down to (1,0) wall, so only (0,0). Actually grid row0: '1','#','.', so (0,0) is start, (0,2) is empty but wall at (0,1) blocks, so cannot reach. (2,0) is '.', can reach via (1,0) which is wall, so no. (2,2) '.' unreachable. So only 1.
        assert(res == std::vector<int>({1}));
    }

    // Example 3: Large speed but limited empty cells
    {
        int n = 1, m = 4, p = 2;
        std::vector<int> speed = {100, 100};
        std::vector<std::string> grid = {"1.2."};
        auto res = castleConquest(n, m, p, speed, grid);
        // Round1: player1 can take index1 (0,1), player2 can take index3 (0,3) from its start (0,2)? Actually start '2' at index2, empty at index3. So after round1: indices 0,1,2,3 all filled. Final counts: player1 has 2 (indices 0,1), player2 has 2 (2,3). But careful: player1 moves first, claims (0,1). Player2 then claims (0,3). So both have 2.
        assert(res == std::vector<int>({2, 2}));
    }

    // Example 4: Conflict resolution: player1 gets cell if they move first
    {
        int n = 1, m = 3, p = 2;
        std::vector<int> speed = {1, 1};
        std::vector<std::string> grid = {"1.2"};
        auto res = castleConquest(n, m, p, speed, grid);
        // Player1 moves first claims (0,1). Player2 cannot move to left because occupied. So player1 has 2, player2 has 1.
        assert(res == std::vector<int>({2, 1}));
    }

    // Example 5: All walls, no expansion
    {
        int n = 2, m = 2, p = 1;
        std::vector<int> speed = {3};
        std::vector<std::string> grid = {"1#", "#."};
        auto res = castleConquest(n, m, p, speed, grid);
        assert(res == std::vector<int>({1}));
    }

    // Example 6: Empty grid with no players? Not valid per problem but test p=0? We'll assume p≥1. Skip.

    return 0;
}
