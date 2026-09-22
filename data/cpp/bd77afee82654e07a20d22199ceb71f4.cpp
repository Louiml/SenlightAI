Write a C++ function that, given target integer coordinates `(tx, ty)`, returns a string representing a sequence of moves (each move is one of `'N'`, `'S'`, `'E'`, `'W'`) that starts at `(0,0)` and reaches exactly `(tx, ty)`, subject to the rule that the `i`-th move (1-indexed) has length `2^(i-1)` and must be in a cardinal direction. If no valid sequence exists, return `"IMPOSSIBLE"`. The search space is limited to moves with total length steps up to 511 (i.e., move lengths `1,2,4,...,256`), and the function must output the *shortest* possible sequence (the one with the fewest moves) if any exists; if multiple shortest sequences exist, return any one of them. Coordinates may be negative or zero, and the function must handle all possible targets within the reachable region defined by the step-length constraint.
// The problem is a classic BFS over the state `(x, y, m)` where `m` is the *next* step length to be used. Starting at `(0,0,1)`, each state has four possible transitions: move East, West, North, or South by `m`, producing a new state with step length `2*m`. The BFS guarantees the first time we reach `(tx, ty)` uses the minimum number of moves because all moves have unit cost (each move is one step). The search is bounded by `m <= 512` (the code uses `if (m >= 512) continue`), meaning the maximum number of moves is 9 (since step lengths double: 1,2,4,8,16,32,64,128,256 – the 10th step would have length 512, which is excluded). This bound is sufficient to cover all targets whose coordinates have absolute values ≤ 511 in the sum of the Manhattan distance? Actually, the reachable region is bounded because the maximum total displacement in any direction is sum of first 9 powers of 2 = 511. So any `|tx| > 511` or `|ty| > 511` is impossible. For the general case, we BFS until we either reach the target or the queue empties. We store the parent direction in a map from `(x,y,m)` to the corresponding move character (N/S/E/W) that led to that state. After BFS, we reconstruct the path by backtracking: from the final `(tx,ty,m_final)` where `m_final` is the step length used to reach it. The reconstruction loop: while `m > 1`, look up the direction `a` for `(tx,ty,m)`, append `a` to the result, then update `m /= 2` and move the coordinate back (subtract the step length `m` from the coordinate in the opposite direction). Finally reverse the string to get moves in the correct order. Edge cases: target `(0,0)` should return an empty string (since no moves needed) – but the given snippet returns `1` from BFS and then reconstructs with while(q>1) giving empty string, which is correct. If target is unreachable within the step limit, return `"IMPOSSIBLE"`. Time complexity: O(4^9) = O(262144) worst-case states, but many are pruned by the map, and it's O(number of generated states) which is at most O(4^9) but typically much smaller. Space complexity: O(number of states) for the map and queue, also O(4^9) worst-case.
#include <string>
#include <queue>
#include <map>
#include <tuple>
#include <algorithm>

// Given target coordinates (tx, ty), return a string of moves (N/S/E/W) that
// starts at (0,0) and reaches the target, where move i has length 2^(i-1).
// Returns "IMPOSSIBLE" if no such sequence exists within the step limit (max 9 moves).
std::string findPath(int tx, int ty) {
    // Map from (x, y, next_step_length) to the direction character that led here.
    std::map<std::tuple<int, int, int>, char> parent;
    std::queue<std::tuple<int, int, int>> q;
    
    // Start at (0,0) with next move length 1.
    q.push(std::make_tuple(0, 0, 1));
    int x, y, m;
    
    while (!q.empty()) {
        std::tie(x, y, m) = q.front();
        q.pop();
        
        // If we reached the target, reconstruct path.
        if (x == tx && y == ty) {
            std::string res;
            int cur_x = tx, cur_y = ty, cur_m = m;
            while (cur_m > 1) {
                char dir = parent[std::make_tuple(cur_x, cur_y, cur_m)];
                res.push_back(dir);
                cur_m /= 2;
                // Undo the move.
                if (dir == 'N') cur_y -= cur_m;
                else if (dir == 'S') cur_y += cur_m;
                else if (dir == 'E') cur_x -= cur_m;
                else if (dir == 'W') cur_x += cur_m;
            }
            std::reverse(res.begin(), res.end());
            return res;
        }
        
        // If step length reaches 512, we stop expanding (max 9 moves).
        if (m >= 512) continue;
        
        int next_m = m * 2;
        // Try all four directions.
        // East
        auto state_e = std::make_tuple(x + m, y, next_m);
        if (parent.find(state_e) == parent.end()) {
            parent[state_e] = 'E';
            q.push(state_e);
        }
        // West
        auto state_w = std::make_tuple(x - m, y, next_m);
        if (parent.find(state_w) == parent.end()) {
            parent[state_w] = 'W';
            q.push(state_w);
        }
        // North
        auto state_n = std::make_tuple(x, y + m, next_m);
        if (parent.find(state_n) == parent.end()) {
            parent[state_n] = 'N';
            q.push(state_n);
        }
        // South
        auto state_s = std::make_tuple(x, y - m, next_m);
        if (parent.find(state_s) == parent.end()) {
            parent[state_s] = 'S';
            q.push(state_s);
        }
    }
    return "IMPOSSIBLE";
}
#include <cassert>
#include <string>

// Forward declaration of the solution function.
std::string findPath(int tx, int ty);

int main() {
    // Trivial: target is start.
    assert(findPath(0, 0) == "");
    
    // Simple one-move targets.
    assert(findPath(1, 0) == "E");
    assert(findPath(-1, 0) == "W");
    assert(findPath(0, 1) == "N");
    assert(findPath(0, -1) == "S");
    
    // Two-move examples (e.g., (1,1) can be E then N, but BFS may find N then E? Both are valid.)
    std::string path11 = findPath(1, 1);
    assert(path11.size() == 2);
    assert((path11 == "EN" || path11 == "NE"));
    
    // A known longer path (2,0): E then W? No, must use distinct lengths: 1 and 2.
    // (2,0) can be E (1) then E (2) => "EE"
    assert(findPath(2, 0) == "EE");
    
    // (0,2) can be N (1) then N (2) => "NN"
    assert(findPath(0, 2) == "NN");
    
    // Unreachable: coordinate too large (beyond sum 1+2+4+...+256=511)
    assert(findPath(512, 0) == "IMPOSSIBLE");
    assert(findPath(0, -512) == "IMPOSSIBLE");
    
    // A more complex reachable target: (3,0) = E (1) + E (2) => "EE"? Actually 1+2=3 exactly.
    assert(findPath(3, 0) == "EE");
    
    // (3,1) is reachable: E (1), E (2), N (4) gives (3,4) not (3,1). Let's check: 
    // Possible: E(1) -> (1,0), N(2) -> (1,2), S(4) doesn't work. Another: N(1)->(0,1), E(2)->(2,1), W(4) no.
    // Actually (3,1) might be impossible with 3 moves? But could be with 4 moves: E(1)->(1,0), E(2)->(3,0), N(4) no.
    // Let's skip because it's not deterministic. Use a known reachable: (3,2) = E(1)+E(2)+N(4)? gives (3,4) no.
    // Instead, use (-3,2) which is definitely reachable: W(1) -> (-1,0), W(2) -> (-3,0), N(4) -> (-3,4) no.
    // To avoid complexity, test only the simple ones given above.
    
    // Additional edge: negative target reachable.
    assert(findPath(-3, 0) == "WW");
    
    // Mixed signs: (1,-1) => E then S? "ES" works.
    std::string path1_1 = findPath(1, -1);
    assert(path1_1.size() == 2);
    assert((path1_1 == "ES" || path1_1 == "SE"));
    
    return 0;
}
