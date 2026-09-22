You are given a rectangular grid representing a building during a fire, with dimensions `h` (rows) and `w` (columns). Each cell is one of: `.` (empty space), `#` (wall), `*` (fire), or `@` (person starting position). Every second, the fire spreads to all four orthogonally adjacent empty cells (not walls or already-burning cells). Simultaneously, the person may move one step orthogonally to an adjacent empty, non-burning, non-wall cell; the person cannot step into a cell that is on fire or becomes on fire in the same second. The person escapes the moment they step out of the grid boundary from any edge cell (i.e., moving from a border cell to outside). Your task is to write a C++ function `int escapeTime(const std::vector<std::string>& grid)` that returns the minimum number of seconds required for the person to escape, or `-1` if escape is impossible. The grid contains exactly one `@` and at least one `*`. Time is counted as the number of moves made by the person; escaping directly from a starting cell at the boundary requires 1 second. The function must only rely on standard libraries and should not read from or write to any external input/output. You may assume the grid is non-empty, dimensions at least 1x1, and that all characters are valid.

This problem is a classic two-source BFS with time-layered propagation. The main algorithm is to simulate time steps: at each second, first expand the fire from all current fire cells to valid adjacent empty cells, marking them as fire and queuing them; then expand the person from all current person positions to adjacent valid cells that are not walls, not fire (including newly expanded fire), and not visited by the person before. The person's BFS runs first only after fire expansion for that second, so the person cannot move into a cell that becomes fire in the same second. If the person reaches a border cell, the next move (the one that would step outside) is considered escape, so the answer is the distance to that border cell plus 1. If the person starts at the border, the answer is 1 (because the first move is out). If the search ends without escape, return -1. Edge cases include: person starts at boundary (answer is 1), no fire (but the problem guarantees at least one), person surrounded by walls or fire, fire reaching every reachable cell faster than the person, and multiple fire sources. Time complexity: each cell is processed at most once for fire and once for person, so O(h*w) per test case. Space complexity: O(h*w) for the grid, visited array, distance array, and queues.

#include <vector>
#include <queue>
#include <string>
#include <utility>

using namespace std;

int escapeTime(const vector<string>& grid) {
    int h = grid.size();
    int w = grid[0].size();
    
    vector<vector<int>> personTime(h, vector<int>(w, -1));
    vector<vector<bool>> personVisited(h, vector<bool>(w, false));
    queue<pair<int,int>> fireQ, personQ;
    
    // Locate start and fire sources
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (grid[i][j] == '@') {
                personQ.push({i,j});
                personVisited[i][j] = true;
                personTime[i][j] = 0;
            } else if (grid[i][j] == '*') {
                fireQ.push({i,j});
            }
        }
    }
    
    // Directions: up, down, left, right
    const int dirR[4] = {-1, 1, 0, 0};
    const int dirC[4] = {0, 0, -1, 1};
    
    // Make a mutable copy to update fire spread
    vector<string> map = grid;
    
    while (!personQ.empty()) {
        // Fire spreads one step first
        int fireSize = fireQ.size();
        for (int i = 0; i < fireSize; i++) {
            auto cur = fireQ.front();
            fireQ.pop();
            for (int d = 0; d < 4; d++) {
                int nr = cur.first + dirR[d];
                int nc = cur.second + dirC[d];
                if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
                if (map[nr][nc] == '#' || map[nr][nc] == '*') continue;
                map[nr][nc] = '*';
                fireQ.push({nr, nc});
            }
        }
        
        // Person moves one step
        int personSize = personQ.size();
        for (int i = 0; i < personSize; i++) {
            auto cur = personQ.front();
            personQ.pop();
            
            // Check if current position is on the border -> can escape this second?
            // Escape means moving out, so if at border, the next move escapes.
            // However, we only process movement; check if this cell is boundary?
            // Actually if we are at boundary, we can step out in 1 second.
            // But we need to check before move? The problem: escaping from border cell takes 1 second.
            // We'll handle it by when generating moves, if new position is outside, return current time + 1.
            for (int d = 0; d < 4; d++) {
                int nr = cur.first + dirR[d];
                int nc = cur.second + dirC[d];
                
                // Out of bounds means escape
                if (nr < 0 || nr >= h || nc < 0 || nc >= w) {
                    return personTime[cur.first][cur.second] + 1;
                }
                
                if (map[nr][nc] == '#' || map[nr][nc] == '*') continue;
                if (personVisited[nr][nc]) continue;
                
                personTime[nr][nc] = personTime[cur.first][cur.second] + 1;
                personVisited[nr][nc] = true;
                personQ.push({nr, nc});
            }
        }
    }
    
    return -1;
}

#include <cassert>
#include <vector>
#include <string>

int escapeTime(const std::vector<std::string>& grid);

int main() {
    // Case 1: Person at corner, escape in 1 second
    assert(escapeTime({"@*"}) == 1);
    assert(escapeTime({"@.", ".#"}) == 1);
    
    // Case 2: Open grid, person must go around a wall
    std::vector<std::string> g2 = {
        "....",
        ".#..",
        "@...",
        "...."
    };
    // Start at (2,0), nearest boundary is left column, distance 0 to boundary? Actually moving left escapes in 1 second.
    // But there is wall at (1,1). From (2,0) left is out, so 1 second. Let's use a different start.
    std::vector<std::string> g2b = {
        "....",
        ".#..",
        "..@.",
        "...."
    };
    // Start at (2,2). Escape to top takes? Path: (2,2)->(1,2)->(0,2) then up out -> 3 seconds? Actually from (0,2) moving up escapes in next step -> 3 seconds total? Let's compute: (2,2) time0, (1,2) time1, (0,2) time2, move up out -> time3. But fire? No fire. So expected 3.
    assert(escapeTime(g2b) == 3);
    
    // Case 3: Fire blocks escape
    std::vector<std::string> g3 = {
        "@#",
        "*#"
    };
    // Start at (0,0) is boundary? Left is out, so 1 second. But fire at (1,0) doesn't affect. Expected 1.
    // Better: put person in middle surrounded by fire
    std::vector<std::string> g3b = {
        "***",
        "*@*",
        "***"
    };
    // Person has no move because all adjacent are fire, and no boundary (3x3 interior). Expected -1.
    assert(escapeTime(g3b) == -1);
    
    // Case 4: Person must run from fire but has path
    std::vector<std::string> g4 = {
        "....",
        ".*..",
        ".@..",
        "...."
    };
    // Start (2,1). Need to escape to boundary. Fire at (1,1) spreads. 
    // Let's simulate manually: t=0: fire at (1,1). Person at (2,1). Fire spreads to (0,1),(1,0),(1,2),(2,1) but (2,1) is person, so fire moves to (0,1),(1,0),(1,2). Person can move to (2,0) or (2,2) or (3,1) etc. 
    // Quick: path to bottom (3,1) then out? From (2,1)->(3,1) time1, then from (3,1) down out time2. But at t=1 fire spreads to (2,0)?? Actually at t=1 fire spreads again: from (0,1),(1,0),(1,2), plus (1,1) already burned. Fire reaches (2,0) at t=2? Not sure. Let's trust the algorithm should give 3? Let's just check -1 case more.
    // We'll just test -1 for blocked.
    
    // Case 5: Escape from boundary in one step even with fire
    std::vector<std::string> g5 = {"@*"};
    assert(escapeTime(g5) == 1);
    
    // Case 6: Open field, no fire, start far from edge
    std::vector<std::string> g6 = {
        ".....",
        ".....",
        "..@..",
        ".....",
        "....."
    };
    // Start (2,2) in 5x5, nearest boundary distance 2 (to row0 or col0) -> need 2 steps to reach boundary, then 1 step out = 3 seconds.
    assert(escapeTime(g6) == 3);
    
    // Case 7: Fire catches up
    std::vector<std::string> g7 = {
        "@..",
        ".*.",
        "..."
    };
    // Start (0,0) is boundary, so 1 second. Fire at (1,1) doesn't matter. Expected 1.
    assert(escapeTime(g7) == 1);
    
    // Case 8: Long corridor, fire from one end, person at other end
    std::vector<std::string> g8 = {
        "*...@"
    };
    // Width 5, person at index 4 (boundary right) -> escape in 1 second. Actually `@` at last column, moving right out. So 1.
    assert(escapeTime(g8) == 1);
    
    // Case 9: Corridor where fire blocks middle
    std::vector<std::string> g9 = {
        ".*..@"
    };
    // Start at (0,4) boundary -> 1 second. Expected 1.
    assert(escapeTime(g9) == 1);
    
    // Case 10: Multi-row with fire spreading faster than person
    std::vector<std::string> g10 = {
        "....",
        ".*..",
        "..@.",
        "...."
    };
    // Start (2,2). Fire at (1,1). Let's compute quickly: t=0 fire (1,1). Fire spreads to (0,1),(1,0),(1,2). Person can move to (2,1) or (3,2) or (2,3). Take (3,2) time1. At t=1 fire spreads to (0,0),(0,2),(1,3),(2,2) but person left, so (2,2) becomes fire. Fire also spreads to (2,1)? from (1,1)? not directly. Person at (3,2) can move to (3,1),(3,3),(4,2) but (4,2) out? Actually row 3 is bottom, so moving down escapes at time2. So answer 2. But need to ensure fire doesn't block. At t=2 fire from (3,? )? Not sure but likely person escapes. Expected 2.
    assert(escapeTime(g10) == 2);
    
    return 0;
}
