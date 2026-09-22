Write a C++ function `int minTiltsToEscape(int n, int m, const std::vector<std::string>& board)` that determines the minimum number of tile-sliding moves needed to get a red ball (`'R'`) into the hole (`'O'`) on a grid board, without the blue ball (`'B'`) falling into the hole first. The board is `n` rows by `m` columns, consisting of `'.'` (empty), `'#'` (wall), `'O'` (hole, exactly one), `'R'` (red ball, exactly one), and `'B'` (blue ball, exactly one). A move consists of tilting the entire board in one of four directions (up, down, left, right). Both balls slide simultaneously as far as possible in that direction until they hit a wall, another ball, or fall into the hole, with the ball closer to the direction of motion moving first. If the blue ball falls into the hole at any point, the attempt is invalid (even if red also falls in the same move). The red ball must reach the hole without blue falling into it, and the maximum allowed moves is 10. If it is impossible within 10 moves, return -1. The function must be `const`‑correct and handle any board dimensions up to 10×10.
// The problem is a classic BFS/DFS state‑space search. Each state consists of the positions of the red and blue balls. Since the board is at most 10×10 and we have 2 balls, the total states are at most 100*100 = 10,000, which is small. We run BFS from the initial positions, expanding each state by the 4 directions. For each direction, we simulate the movement carefully: determine which ball moves first based on its coordinate along that axis (e.g., for moving up, the ball with smaller row moves first). Then slide the first ball until it hits a wall, another ball (but the other ball hasn't moved yet), or falls into the hole. Then slide the second ball similarly, but now the first ball's final position blocks it. During simulation, if the blue ball falls into the hole, that move is invalid and we discard the resulting state. If the red ball falls into the hole and the blue does not, we have found a solution and the current depth+1 is the answer. We restrict the search to at most 10 moves; if BFS returns no solution within depth 10, return -1. Key edge cases: both balls might fall in the same move, but that is invalid because blue falls; also, the red ball might fall but blue might also fall later in the same move (since blue moves second), so we must check after both balls have moved. Time complexity is O(states × 4 × (N+M)) for simulation, with states ≤ 10,000, so it’s very efficient. Space complexity is O(states) for BFS queue and visited set.
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <utility>

struct State {
    int rR, cR, rB, cB;
    bool operator<(const State& other) const {
        if (rR != other.rR) return rR < other.rR;
        if (cR != other.cR) return cR < other.cR;
        if (rB != other.rB) return rB < other.rB;
        return cB < other.cB;
    }
};

// Move both balls in a given direction (0=up,1=down,2=left,3=right)
// Returns true if red fell into hole and blue did not; also outputs new state.
bool simulateMove(const std::vector<std::string>& board, int n, int m,
                  State cur, int dir, State& next, bool& redFell, bool& blueFell) {
    // Determine which ball moves first based on direction
    bool redFirst;
    if (dir == 0) redFirst = (cur.rR < cur.rB);      // up
    else if (dir == 1) redFirst = (cur.rR > cur.rB); // down
    else if (dir == 2) redFirst = (cur.cR < cur.cB); // left
    else redFirst = (cur.cR > cur.cB);               // right

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    // Temporary map to simulate; start with current ball positions
    std::vector<std::string> tmp = board;
    tmp[cur.rR][cur.cR] = 'R';
    tmp[cur.rB][cur.cB] = 'B';

    redFell = false;
    blueFell = false;

    // Move first ball
    State first = redFirst ? State{cur.rR, cur.cR, 0, 0} : State{cur.rB, cur.cB, 0, 0};
    int r = first.rR, c = first.cR;
    while (true) {
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (tmp[nr][nc] == '#') break;
        if (tmp[nr][nc] == 'R' || tmp[nr][nc] == 'B') break;
        if (tmp[nr][nc] == 'O') {
            tmp[r][c] = '.';
            r = nr; c = nc;
            if (redFirst) redFell = true;
            else blueFell = true;
            break;
        }
        // empty
        tmp[nr][nc] = tmp[r][c];
        tmp[r][c] = '.';
        r = nr; c = nc;
    }
    // Update first ball's position (if not fell, it's at r,c; if fell, it's at O but we don't place)
    if (!(redFirst ? redFell : blueFell)) {
        if (redFirst) { first.rR = r; first.cR = c; }
        else { first.rB = r; first.cB = c; }
    }

    // Move second ball
    State second = redFirst ? State{cur.rB, cur.cB, 0, 0} : State{cur.rR, cur.cR, 0, 0};
    r = second.rR; c = second.cR;
    while (true) {
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (tmp[nr][nc] == '#') break;
        if (tmp[nr][nc] == 'R' || tmp[nr][nc] == 'B') break;
        if (tmp[nr][nc] == 'O') {
            tmp[r][c] = '.';
            r = nr; c = nc;
            if (redFirst) blueFell = true;
            else redFell = true;
            break;
        }
        tmp[nr][nc] = tmp[r][c];
        tmp[r][c] = '.';
        r = nr; c = nc;
    }
    if (!(redFirst ? blueFell : redFell)) {
        if (redFirst) { second.rB = r; second.cB = c; }
        else { second.rR = r; second.cR = c; }
    }

    // Set next state from the two ball positions
    if (redFirst) {
        next = redFell ? State{0,0, second.rB, second.cB} : State{first.rR, first.cR, second.rB, second.cB};
    } else {
        next = blueFell ? State{second.rR, second.cR, 0,0} : State{second.rR, second.cR, first.rB, first.cB};
    }
    // If blue fell, the move is invalid regardless
    return redFell && !blueFell;
}

int minTiltsToEscape(int n, int m, const std::vector<std::string>& board) {
    State start;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (board[i][j] == 'R') { start.rR = i; start.cR = j; }
            if (board[i][j] == 'B') { start.rB = i; start.cB = j; }
        }
    }

    std::queue<std::pair<State, int>> q; // state, depth
    std::set<State> visited;
    q.push({start, 0});
    visited.insert(start);

    while (!q.empty()) {
        auto [cur, depth] = q.front();
        q.pop();
        if (depth == 10) continue; // cannot move further

        for (int dir = 0; dir < 4; ++dir) {
            State next;
            bool redFell, blueFell;
            bool success = simulateMove(board, n, m, cur, dir, next, redFell, blueFell);
            if (!success) continue; // blue fell or both fell
            if (redFell && !blueFell) return depth + 1; // red escaped
            if (visited.count(next)) continue;
            visited.insert(next);
            q.push({next, depth + 1});
        }
    }
    return -1;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link it)

int main() {
    // Test 1: from problem statement
    std::vector<std::string> b1 = {
        "#####",
        "#..B#",
        "#.#.#",
        "#RO.#",
        "#####"
    };
    assert(minTiltsToEscape(5, 5, b1) == 1);

    // Test 2: sample from problem
    std::vector<std::string> b2 = {
        "#######",
        "#...RB#",
        "#.#####",
        "#.....#",
        "#####.#",
        "#O....#",
        "#######"
    };
    assert(minTiltsToEscape(7, 7, b2) == 5);

    // Test 3: impossible
    std::vector<std::string> b3 = {
        "##########",
        "#R#...##B#",
        "#...#.##.#",
        "#####.##.#",
        "#......#.#",
        "#.######.#",
        "#.#....#.#",
        "#.#.#.#..#",
        "#...#.O#.#",
        "##########"
    };
    assert(minTiltsToEscape(10, 10, b3) == -1);

    // Test 4: simple direct
    std::vector<std::string> b4 = {
        "#######",
        "#R.O.B#",
        "#######"
    };
    assert(minTiltsToEscape(3, 7, b4) == 1);

    // Test 5: red is blocked but can go around
    std::vector<std::string> b5 = {
        "##########",
        "#R#...##B#",
        "#...#.##.#",
        "#####.##.#",
        "#......#.#",
        "#.######.#",
        "#.#....#.#",
        "#.#.##...#",
        "#O..#....#",
        "##########"
    };
    assert(minTiltsToEscape(10, 10, b5) == 7);

    // Test 6: impossible because both would fall
    std::vector<std::string> b6 = {
        "##########",
        "#.O....RB#",
        "##########"
    };
    assert(minTiltsToEscape(3, 10, b6) == -1);

    // Test 7: more than 10 moves needed
    std::vector<std::string> b7 = {
        "#######",
        "#.....#",
        "#.#####",
        "#.....#",
        "#####B#",
        "#O..R.#",
        "#######"
    };
    assert(minTiltsToEscape(7, 7, b7) == -1); // needs more than 10

    // Test 8: red falls immediately, blue does not
    std::vector<std::string> b8 = {
        "###",
        "#RO",
        "###"
    };
    // But there is no blue? Actually need blue. Let's put blue far away.
    std::vector<std::string> b8b = {
        "######",
        "#R...O",
        "#B....",
        "######"
    };
    assert(minTiltsToEscape(4, 6, b8b) == 1); // move right

    // Test 9: red and blue both fall in same move -> invalid
    std::vector<std::string> b9 = {
        "#####",
        "#RB O", // but careful: put both in line with hole
        "#####"
    };
    // Better: a line with hole at end
    std::vector<std::string> b9b = {
        "#####",
        "#RB.O",
        "#####"
    };
    // Tilt right: both move right, both fall, so invalid -> -1
    assert(minTiltsToEscape(3, 5, b9b) == -1);

    // Test 10: blue falls first, but red could escape later? No, blue falls first invalidates.
    std::vector<std::string> b10 = {
        "######",
        "#B..RO",
        "#....#",
        "######"
    };
    // Move right: red moves first? Actually red is right of blue, so moving right, red has smaller column? red col 4, blue col 1, so red moves second? For right, ball with larger column moves first -> red first. Red moves right into O and falls, blue then slides right but not into O? Actually blue is at col1, moving right, it will slide to col3 or so, not into O (hole at col5). So red escapes, blue does not -> answer 1.
    assert(minTiltsToEscape(4, 6, b10) == 1);

    return 0;
}
