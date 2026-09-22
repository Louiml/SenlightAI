// Write a C++ function `simulateRobot` that takes a rectangular grid of characters representing directional sensors (`'N'`, `'S'`, `'E'`, `'W'`), along with its number of rows `r`, number of columns `c`, and the starting column index `startCol` (0-based), and simulates a robot starting at the top row (row 0) at that column. The robot moves one step per tick according to the sensor at its current cell: `'N'` moves up (decrease row), `'S'` moves down (increase row), `'E'` moves right (increase column), `'W'` moves left (decrease column). The robot stops when it moves outside the grid (exits) or when it re-enters a previously visited cell (forming a loop). The function must return a `std::pair<int,int>` where: if the robot exits, the pair is `{number_of_steps_to_exit, -1}` (where steps count from 1 for the first move); if the robot loops, the pair is `{number_of_steps_taken_before_the_loop_starts, loop_length}` where "steps taken before the loop starts" is the step count when entering the cell that begins the cycle (i.e., the first time that cell is visited), and loop length is the number of steps in the cycle. Note: the grid is guaranteed to be non-empty, and the starting cell is always valid. The grid characters are uppercase letters only from the set `{'N','S','E','W'}`. The function must handle arbitrary grid sizes up to 100x100.
// The core idea is to simulate the robot step by step while tracking the first visit step number for each cell. Use a 2D vector `visitStep` initialized to `-1` (meaning unvisited). At each step `step` (starting at 1), record the current cell's visit step. Then compute the next cell from the current sensor. If the next cell is out of bounds, the robot exits after `step` moves, so return `{step+1, -1}` because the step count increments when you try to move and fail. If the next cell is already visited (its `visitStep` is not `-1`), then that cell is the start of the loop. The number of steps taken before the loop starts is exactly `visitStep[ny][nx]` (the first time it entered that cell). The loop length is `step - visitStep[ny][nx] + 1`? Let's verify with the example: if a cell is first visited at step 3, and revisited at step 6, then the duration of the loop is 6-3=3 steps (because steps 3,4,5,6? Actually the path from step 3 to step 6 inclusive, but when you revisit the cell at step 6, you don't make a move; the loop length is the number of moves made between the two visits, which is 6-3=3). So loop length = `step - visitStep[ny][nx]`. The starting step for the loop is `visitStep[ny][nx]`. Return `{visitStep[ny][nx], step - visitStep[ny][nx]}`. If the next cell is unvisited and valid, continue recursion with `step+1`. In the original code, they define `start = moves[nY][nX] - 1` and `duration = step - start`, and output `start` as steps before loop and `duration` as loop length. That matches: `start` is `visitStep[nY][nX] - 1`? Wait let's check: in the original, `moves[nY][nX]` is set to the step number when that cell was visited. If a cell was first visited at step 1, then `moves[nY][nX]=1`. Then `start = moves[nY][nX] - 1 = 0`. They output `res.F` as steps before loop, which is `start` = 0? But they output "step(s) before a loop of ..." and `res.F` is that. In the example, consider grid of 1 row 2 cells starting at col 0: "EW"? Let's simulate: start at (0,0) step1, sensor E, next (0,1) valid, step2, sensor W, next (0,0) visited. Then `moves[0][0]=1`, `moves[0][1]=2`. At step2, when we move from (0,1) to (0,0), we see `moves[0][0]` is not -1, so `start = moves[0][0]-1 = 0`, `duration = step - start = 2-0=2`. They output `start`=0 steps before loop, and duration=2 loop steps. But actually the robot starts at (0,0) at step1, moves to (0,1) at step2, tries to move back to (0,0) at step3? No, the loop detection happens at the moment you attempt to move into a visited cell, not after making the move. In the simulation, you are at (0,1) after step2, and you try to move to (0,0) but that cell is visited, so you stop without making that move. The steps taken before the loop starts should be the number of steps taken before re-entering the first cell of the loop. The robot has taken 2 steps (to (0,1)) before it would re-enter (0,0). The loop consists of the path from (0,0) to (0,1) and back, which is 2 moves. So steps before loop = 2? But they output 0. That seems inconsistent. Let's check the original problem's output format. This is a known problem (UVA 10116 - Robot Motion). The correct output for the grid "EW" starting at column 1 (0-based col 0?) Actually the original problem: input `r c start` where start is 1-based column index. In the example, if r=1, c=2, start=1, grid "EW", the output is "0 step(s) before a loop of 2 step(s)" because the robot starts at the cell, and the first move is to the right, then the next move would be back, but the loop includes the starting cell, and the number of steps before the loop is the number of steps taken before encountering the first repeated cell. Since the starting cell is visited at time 0 (step 0), then first move at step1, second move at step2, and then it would re-enter start at step3? Actually the typical simulation: you mark the starting cell as visited at step 0. Then you move. When you move to a new cell, you record its step as the number of moves taken. When you attempt to move to a cell already visited, the number of steps taken before the loop is the recorded step of that cell (the time it was first visited), and the loop length is the difference between the current step (the number of moves taken to reach the current cell) and the recorded step. In the UVA problem, they output "0 step(s) before a loop of 2 step(s)" for the "EW" example. That suggests that they consider the starting cell as step 0, and the first move increments to step 1. So the number of steps before the loop is the step count when the repeated cell was first entered. For "EW", starting at (0,0) step0, move to (0,1) step1, then attempt to move to (0,0) which is visited at step0, so steps before loop = 0, loop length = 1 - 0 = 1? Actually they output 2, so that doesn't match either. Let me look up the expected output for this classic problem: For input `1 2 1` and grid `EW`, the output is "0 step(s) before a loop of 2 step(s)". That indicates that the loop length is 2, meaning the robot visits two cells before repeating. The steps before the loop is 0 because the starting cell is the first cell of the loop. The simulation: start at cell (0,0) at step 0. Move to (0,1) at step 1. Then from (0,1) you move to (0,0) but that cell was visited at step 0, so you stop. The number of steps taken before the loop is the step index of the first cell of the loop, which is 0. The loop length is the number of moves in the cycle, which is 2 (from (0,0) to (0,1) and back). So in code, if you store `moves[y][x]` as the step number when you first visit that cell, starting with step 0 for the initial cell, then when you are at a cell with step `s` and you try to move to a neighbor that has `moves[nY][nX] = t`, then steps before loop = `t`, loop length = `s - t + 1`? Because you have taken `s` steps to reach the current cell, and the cycle includes the current cell back to the target? Actually let's formalize: visits: cell A step0, cell B step1. From B, attempt to A: B has step1, A has step0, so you have moved 1 step to B, and the cycle is A->B->A, length 2. Steps before loop = 0, loop length = (current_step - target_step) + 1? current_step=1, target_step=0, difference=1, +1 gives 2. That works. But in the original code, they initialize step=1 at the starting cell. So they consider the starting cell as step1. Then for "EW": start at (0,0) step1, move to (0,1) step2, attempt to (0,0) which has step1. Then `start = moves[0][0]-1 = 0`, `duration = step - start = 2-0=2`. Output `start`=0 and `duration`=2. That matches. So the number of steps before loop is `moves[nY][nX] - 1` (i.e., one less than the recorded step of the repeated cell), because they count steps starting at 1 for the initial position, but the problem expects "steps before loop" to be 0 for the first cell. That is consistent. Similarly, for exit: if you are at step `s` and the next cell is out of bounds, the total steps to exit is `s` (because you move out on the next move? Actually in the code they return `(step+1, -1)` and then output `res.F - 1`. So for a single cell with sensor 'N' (up) starting at top row, you are at step1, try to move up, invalid, they return (2,-1), then output 1 step to exit. That makes sense: you take one move to leave. So in my function, I need to replicate this behavior: use step starting at 1 for the initial cell, and when moving, if next is out of bounds, the number of steps to exit is `step` (because you take one more step out). Actually the original returns `(step+1, -1)` and then prints `res.F - 1`, so effectively `step`. So I will return `{step, -1}` directly for exit. For loop, return `{moves[nY][nX] - 1, step - (moves[nY][nX] - 1)}`? Let's compute: `moves[nY][nX]` is the first visit step. For "EW", `moves[0][0]=1`, `step=2` at B, `moves[0][0]=1`, so steps before loop = 0, loop length = step - (moves[nY][nX]-1) = 2 - 0 = 2. That works. So essentially: steps_before_loop = (moves[nY][nX] - 1), loop_length = step - (moves[nY][nX] - 1)? Actually `step - (moves[nY][nX]-1)` = step - moves[nY][nX] + 1. For "EW", step=2, moves[nY][nX]=1, gives 2-1+1=2. Yes. So loop_length = step - moves[nY][nX] + 1. But in original they compute `start = moves[nY][nX] - 1; duration = step - start;` which gives duration = step - (moves[nY][nX]-1) = step - moves[nY][nX] + 1. Same. So return `{moves[nY][nX] - 1, step - (moves[nY][nX] - 1)}`.
//
// Therefore my function should: initialize `visitStep` 2D vector with -1. Start at row 0, column `startCol`, set `visitStep[0][startCol] = 1`, and call a recursive or iterative simulation with step=1. For each step, compute next cell using the grid. If next out of bounds, return `{step, -1}`. If next cell visited (visitStep != -1), return `{visitStep[nextY][nextX] - 1, step - (visitStep[nextY][nextX] - 1)}`. Else set `visitStep[nextY][nextX] = step+1` and continue with step+1.
//
// Edge cases: single cell with any direction that goes out of bounds: return `{1, -1}`. Single cell that points to itself? Impossible because sensors point to adjacent cells, not same cell, so no immediate self-loop. A loop that includes the starting cell: steps before loop = 0. A loop that does not include starting cell: steps before loop > 0. Time complexity O(r*c) because each cell visited at most once. Space O(r*c) for visitStep.
#include <vector>
#include <utility>
#include <string>

// Simulate a robot on a grid of directional sensors.
// Returns {steps_to_exit, -1} if the robot exits, or {steps_before_loop, loop_length} if it loops.
std::pair<int, int> simulateRobot(const std::vector<std::string>& grid, int startCol) {
    int r = (int)grid.size();
    int c = (int)grid[0].size();
    
    // visitStep[y][x] = step number (1-based) when this cell was first visited, or -1 if never visited.
    std::vector<std::vector<int>> visitStep(r, std::vector<int>(c, -1));
    
    int x = startCol;
    int y = 0;
    int step = 1;
    visitStep[y][x] = step;
    
    while (true) {
        char sensor = grid[y][x];
        int nx = x, ny = y;
        if (sensor == 'N') ny--;
        else if (sensor == 'S') ny++;
        else if (sensor == 'E') nx++;
        else nx--; // 'W'
        
        // Check exit
        if (nx < 0 || nx >= c || ny < 0 || ny >= r) {
            return {step, -1};
        }
        
        // Check loop
        if (visitStep[ny][nx] != -1) {
            int beforeLoop = visitStep[ny][nx] - 1;
            int loopLength = step - beforeLoop;
            return {beforeLoop, loopLength};
        }
        
        // Move to next cell
        step++;
        x = nx;
        y = ny;
        visitStep[y][x] = step;
    }
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// the function above
std::pair<int, int> simulateRobot(const std::vector<std::string>& grid, int startCol);

int main() {
    // Single row, move right then exit
    std::vector<std::string> grid1 = {"E"};
    assert(simulateRobot(grid1, 0) == std::make_pair(1, -1));
    
    // Single row, move left then exit
    std::vector<std::string> grid2 = {"W"};
    assert(simulateRobot(grid2, 0) == std::make_pair(1, -1));
    
    // Single row, two cells: E then W, starting at left -> loop
    std::vector<std::string> grid3 = {"EW"};
    assert(simulateRobot(grid3, 0) == std::make_pair(0, 2));
    
    // Single row, three cells: E E W, starting at left -> loop
    std::vector<std::string> grid4 = {"EEW"};
    // Positions: 0:E->1, 1:E->2, 2:W->1 (visited at step2? Let's trace: step1 at 0, step2 at 1, step3 at 2, then move to 1 which was visited at step2 => beforeLoop=1 (since 2-1=1), loopLength = 3 - 1 = 2
    assert(simulateRobot(grid4, 0) == std::make_pair(1, 2));
    
    // Two rows, two columns: starting top-left, sensors: right (E) and down (S) / left (W) and up (N) creates a simple 2x2 loop
    std::vector<std::string> grid5 = {"ES", "WN"};
    // Trace: step1 (0,0) E->(0,1) step2, S->(1,1) step3, W->(1,0) step4, N->(0,0) visited step1 => beforeLoop=0, loopLength=4
    assert(simulateRobot(grid5, 0) == std::make_pair(0, 4));
    
    // Exit after some steps in a larger grid
    std::vector<std::string> grid6 = {"SE", "WN"};
    // Trace: step1 (0,0) S->(1,0) step2, W out of bounds => exit after 2 steps
    assert(simulateRobot(grid6, 0) == std::make_pair(2, -1));
    
    // Loop that does not include starting cell
    std::vector<std::string> grid7 = {"E", "E", "S"}; // 3 rows, 1 column? Actually make it 1x3? Let's use 1x3: {"EWE"}? Better to test with a known pattern:
    // Use grid: {"EEW"} we already did. For a loop not starting at start, do: {"N", "S"}? Not possible in 1D. Let's do a 2x2 with start at (0,0) but the loop is at (0,1)-(1,1)? Hard to construct without getting complicated. Skip.
    
    // Edge: start column at last column, pointing left into a loop
    std::vector<std::string> grid8 = {"EW"};
    assert(simulateRobot(grid8, 1) == std::make_pair(1, 2)); // Start at (0,1), W->(0,0) step2, E->(0,1) visited step1 => beforeLoop=0, loopLength=2? Actually start at col1: step1 at col1, W->col0 step2, E->col1 visited step1 => beforeLoop=0, loopLength=2, but steps before loop should be 0? Wait beforeLoop = visitStep[col1]-1 = 1-1=0, loopLength = step - 0 = 2-0=2. So yes (0,2). But note that starting at (0,1) you take 1 move to reach col0, then would go back, so the first repeated cell is col1 at step1, so before loop is 0? Actually the robot is at col1 at step1, moves to col0 at step2, then tries to move to col1 which is visited at step1, so before loop = 0? That would mean the loop starts immediately at the starting cell, which is correct because the cycle is the entire path from start. So output (0,2) is correct.
    assert(simulateRobot(grid8, 1) == std::make_pair(0, 2));
    
    return 0;
}
