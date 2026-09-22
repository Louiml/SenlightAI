// Write a C++ function `int catMouseGame(const vector<vector<int>>& graph)` that determines the winner of a game played on an undirected graph with nodes numbered 0, 1, ..., n-1. Node 0 is the hole, node 1 is the mouse's start, and node 2 is the cat's start. The mouse moves first, then players alternate turns. The mouse can move to any adjacent node (including the hole). The cat can move to any adjacent node except the hole (the cat cannot enter the hole). The mouse wins if it reaches the hole before being caught. The cat wins if it lands on the mouse's current node or the mouse is forced to move onto the cat. The game can end in a draw if both players can avoid losing indefinitely. Return `1` if the mouse wins, `2` if the cat wins, and `0` if the game is a draw. The graph is given as an adjacency list; it is connected and simple (no self-loops or multiple edges).

#include <cassert>
#include <vector>

int catMouseGame(const std::vector<std::vector<int>>& graph); // declaration

int main() {
    // Example 1: graph = [[2,3],[2],[0,1],[0]]
    // Expected: Mouse wins (1)
    std::vector<std::vector<int>> g1 = {{2,3},{2},{0,1},{0}};
    assert(catMouseGame(g1) == 1);

    // Example 2: graph = [[1,3],[0],[3],[0,2]]
    // Expected: Cat wins (2)
    std::vector<std::vector<int>> g2 = {{1,3},{0},{3},{0,2}};
    assert(catMouseGame(g2) == 2);

    // Example 3: graph = [[2,3],[2],[0,1,3],[0,2]]
    // Expected: Draw (0)
    std::vector<std::vector<int>> g3 = {{2,3},{2},{0,1,3},{0,2}};
    assert(catMouseGame(g3) == 0);

    // Edge case: cat starts adjacent to mouse, mouse can quickly go to hole?
    // g4 = [[1,2],[0],[0]] -> mouse at 1, cat at 2, both connected to 0 and each other? Actually graph[1]={0}, graph[2]={0}. Mouse moves first: can move to 0 and win. So mouse wins.
    std::vector<std::vector<int>> g4 = {{1,2},{0},{0}};
    assert(catMouseGame(g4) == 1);

    // Edge case: cat immediately catches? Mouse at 1, cat at 2, but graph[1]={2}, graph[2]={1}. Mouse must move to 2 and lose.
    std::vector<std::vector<int>> g5 = {{},{2},{1}}; // node 0 isolated but graph is simple connected? Actually this is not connected. But for testing, make node 0 connected to both? Let's do [[2,3],[2],[0,1],[0]] already tested. Instead, let's craft a direct: nodes 0=hole,1=mouse,2=cat, edges: 1-2, 0-1, 0-2. Mouse moves first: can go to 0 and win, so mouse wins.
    std::vector<std::vector<int>> g6 = {{1,2},{0,2},{0,1}};
    assert(catMouseGame(g6) == 1);

    // A cycle of 3 nodes: 0-1-2-0, mouse at 1, cat at 2. Mouse can go to 0 and win immediately.
    std::vector<std::vector<int>> g7 = {{1,2},{0,2},{0,1}};
    // Same as g6, mouse wins.

    // Larger graph where cat can force a draw? Possibly but we'll trust known examples.

    return 0;
}

#include <vector>
#include <queue>
#include <tuple>
#include <unordered_set>

int catMouseGame(const std::vector<std::vector<int>>& graph) {
    const int n = static_cast<int>(graph.size());
    const int MOUSE = 1;
    const int CAT = 2;
    const int DRAW = 0;
    const int HOLE = 0;
    const int MOUSE_TURN = 0;
    const int CAT_TURN = 1;

    // degree[m][c][turn] = number of outgoing moves from state (m, c, turn)
    std::vector<std::vector<std::vector<int>>> degree(
        n, std::vector<std::vector<int>>(n, std::vector<int>(2)));

    std::unordered_set<int> ignore(graph[HOLE].begin(), graph[HOLE].end());
    for (int m = 0; m < n; ++m) {
        for (int c = 0; c < n; ++c) {
            degree[m][c][MOUSE_TURN] = static_cast<int>(graph[m].size());
            degree[m][c][CAT_TURN] = static_cast<int>(graph[c].size()) -
                                     (ignore.count(c) ? 1 : 0);
        }
    }

    // color[m][c][turn] = result if starting from that state
    std::vector<std::vector<std::vector<int>>> color(
        n, std::vector<std::vector<int>>(n, std::vector<int>(2, DRAW)));

    std::queue<std::tuple<int, int, int, int>> q; // mouse, cat, turn, result

    // Initialize terminal states
    for (int i = 0; i < n; ++i) {
        if (i == HOLE) continue;
        // Mouse at hole -> mouse wins, regardless of cat position and turn
        color[HOLE][i][CAT_TURN] = MOUSE;
        q.emplace(HOLE, i, CAT_TURN, MOUSE);
        // Also handle MOUSE_TURN? Actually if mouse is at hole and it's mouse's turn, the mouse already won on the previous move. But the state (HOLE, i, MOUSE_TURN) is unreachable because the game ends when mouse reaches hole. We'll still set it for completeness, but it's not needed.
        color[HOLE][i][MOUSE_TURN] = MOUSE;
        q.emplace(HOLE, i, MOUSE_TURN, MOUSE);

        // Mouse and cat at same node -> cat wins
        color[i][i][MOUSE_TURN] = CAT;
        color[i][i][CAT_TURN] = CAT;
        q.emplace(i, i, MOUSE_TURN, CAT);
        q.emplace(i, i, CAT_TURN, CAT);
    }

    while (!q.empty()) {
        auto [m, c, t, result] = q.front();
        q.pop();

        // Generate all predecessor states (states that can move to (m,c,t))
        // If t is MOUSE_TURN, that means it's mouse's turn, so the predecessor is a CAT turn state.
        // The predecessor state (nm, nc, nt) where nt is the other turn.
        // If t == MOUSE_TURN, the current state is reached after a mouse move, so the previous turn was CAT, and the previous cat position was nc = c, and the mouse moved from nm to m. So predecessors are (nm, c, CAT_TURN) for all nm in graph[m].
        // If t == CAT_TURN, the current state is reached after a cat move, so the previous turn was MOUSE, and the previous mouse position was nm = m, and the cat moved from nc to c. So predecessors are (m, nc, MOUSE_TURN) for all nc in graph[c] except nc == HOLE.

        if (t == MOUSE_TURN) {
            // Previous turn was CAT, previous state (nm, c, CAT_TURN)
            for (int nm : graph[m]) {
                if (color[nm][c][CAT_TURN] != DRAW) continue;
                if (CAT_TURN == result) { // If the current winning player is the one who moves in predecessor
                    color[nm][c][CAT_TURN] = result;
                    q.emplace(nm, c, CAT_TURN, result);
                } else {
                    --degree[nm][c][CAT_TURN];
                    if (degree[nm][c][CAT_TURN] == 0) {
                        color[nm][c][CAT_TURN] = result;
                        q.emplace(nm, c, CAT_TURN, result);
                    }
                }
            }
        } else { // t == CAT_TURN, previous turn was MOUSE, previous state (m, nc, MOUSE_TURN)
            for (int nc : graph[c]) {
                if (nc == HOLE) continue;
                if (color[m][nc][MOUSE_TURN] != DRAW) continue;
                if (MOUSE_TURN == result) {
                    color[m][nc][MOUSE_TURN] = result;
                    q.emplace(m, nc, MOUSE_TURN, result);
                } else {
                    --degree[m][nc][MOUSE_TURN];
                    if (degree[m][nc][MOUSE_TURN] == 0) {
                        color[m][nc][MOUSE_TURN] = result;
                        q.emplace(m, nc, MOUSE_TURN, result);
                    }
                }
            }
        }
    }

    // Return result for starting state: mouse at 1, cat at 2, mouse's turn
    return color[1][2][MOUSE_TURN];
}

// This problem is solved using retrograde analysis on the state space of the game, where each state is `(mouse_position, cat_position, turn)` with `turn` indicating whose turn it is (0 = mouse, 1 = cat). The number of states is `2 * n^2`, where `n` is the number of nodes. We precompute the "degree" of each state: for the mouse's turn, it has `graph[mouse].size()` outgoing moves; for the cat's turn, it has `graph[cat].size()` minus 1 if the hole is adjacent (since the cat cannot move to the hole). We then initialize known outcomes: states where the mouse is at the hole are wins for the mouse, and states where mouse and cat are at the same node are wins for the cat (but we must exclude the hole? Actually the hole state is handled separately). We push these known outcomes into a queue. Then we perform a reverse BFS: for each popped state with known result, we look at all predecessor states (states that can move into the current state). If a predecessor is on the turn of the player who wins in the popped state, then that predecessor is also a win for that player (because the player can move to a winning state). Otherwise, we decrement the predecessor's "remaining moves" counter; if it reaches zero, then all possible moves from that predecessor lead to a win for the opponent, so the predecessor is a win for the opponent. States not resolved by the end are draws.  
//
// Important edge cases: the cat cannot occupy the hole; the mouse can move to the hole and instantly wins. If the cat starts at the same node as the mouse (not possible here but the logic handles it), the cat wins immediately. The graph may have nodes not reachable from the start; the algorithm still processes all states. The time complexity is `O(n^3)` because for each of the `2*n^2` states, we may iterate over all edges of the current player (up to `n`), and the space is `O(n^2)` for the color and degree arrays.
