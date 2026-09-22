// Write a standalone C++ function named `simulateRobotMoves` that simulates the movement of multiple robots on a rectangular grid. The function takes as input: the grid width `maxX` and height `maxY` (both positive integers), the number of robots `N`, the number of commands `M`, a vector of robot initial states (each with integer x-coordinate 1..maxX, integer y-coordinate 1..maxY, and direction character `N`, `E`, `S`, or `W`), and a vector of commands (each command contains a robot index 1..N, an action character `L` (turn left 90°), `R` (turn right 90°), or `F` (move forward one cell per step), and a repetition count). Robots move simultaneously but commands are executed sequentially: each command is fully processed before the next. The grid is 1-indexed in input, but internally treat coordinates as 0-indexed with y increasing upward (e.g., input y=1 is at the bottom row). When a robot moves forward, it checks for collisions: if the next cell would be outside the grid, return a string `"Robot X crashes into the wall"` where X is the robot number (1-based); if the next cell contains another robot, return `"Robot X crashes into robot Y"` where Y is the number of the robot in that cell. If no crash occurs, update the grid and robot positions. If multiple commands are given, stop at the first crash and return the corresponding message. If all commands complete without collision, return the string `"OK"`. The grid cell can hold at most one robot at a time. The function must not modify the input vectors (use `const` references), and must handle arbitrary command counts (including zero). Assume the initial robot positions are distinct and within bounds.

The solution uses a 2D grid (e.g., vector of vector<int>) to track which robot occupies each cell, with 0 meaning empty and a positive integer representing robot number (1-based). Store robot positions and directions in a vector of structs. For each command, apply the action: for `L` or `R`, rotate the direction by reducing count modulo 4 to avoid redundant loops (since four turns return to original direction). For `F`, iterate over each step (1 to count). For each step, compute the next cell based on current direction. Check if the next cell is outside the grid boundaries: if so, return the wall crash message. Check if the next cell is occupied: if so, return the robot collision message. If both checks pass, update the grid by setting the old cell to 0 and the new cell to the robot number, and update the robot's position. After all commands, return `"OK"`. Edge cases include: turning commands never cause collisions; a robot may move through multiple cells in one command (must check each intermediate cell); the wall check should be done before occupancy check to prioritize wall crash (though the original code checks wall first). Important: the input coordinates are 1-based and y increases upward, so convert to internal 0-based with y = maxY - inputY and x = inputX - 1; this makes the north direction decrease y, south increase y, west decrease x, east increase x. Time complexity is O((N + M * maxSteps) * 1) where maxSteps is the maximum repetition count, because each step processes constant work; space complexity is O(maxX * maxY) for the grid plus O(N) for robot storage.

#include <string>
#include <vector>
#include <utility>

struct RobotState {
    int x; // 0-indexed internal
    int y; // 0-indexed internal
    char dir; // 'N', 'E', 'S', 'W'
};

struct Command {
    int robotIndex; // 1-based
    char action;    // 'L', 'R', 'F'
    int count;
};

// Simulate robot movements on a grid.
// Returns "OK" if no crashes, otherwise a crash message.
std::string simulateRobotMoves(int maxX, int maxY, int N, int M,
                               const std::vector<RobotState>& initialRobots,
                               const std::vector<Command>& commands) {
    // Initialize grid with 0 (empty), size maxY rows x maxX columns.
    std::vector<std::vector<int>> grid(maxY, std::vector<int>(maxX, 0));
    
    // Copy robots to mutable local vector.
    std::vector<RobotState> robots = initialRobots;
    
    // Place robots on the grid.
    for (int i = 0; i < N; ++i) {
        grid[robots[i].y][robots[i].x] = i + 1; // robot numbers are 1-based
    }
    
    // Process commands.
    for (const auto& cmd : commands) {
        int idx = cmd.robotIndex - 1; // 0-based index
        char action = cmd.action;
        int cnt = cmd.count;
        
        if (action == 'L' || action == 'R') {
            // Rotate direction; count modulo 4 avoids redundant loops.
            int rotations = cnt % 4;
            if (rotations == 0) {
                continue; // no change
            }
            for (int r = 0; r < rotations; ++r) {
                if (action == 'L') {
                    // N->W, W->S, S->E, E->N
                    if (robots[idx].dir == 'N') robots[idx].dir = 'W';
                    else if (robots[idx].dir == 'W') robots[idx].dir = 'S';
                    else if (robots[idx].dir == 'S') robots[idx].dir = 'E';
                    else if (robots[idx].dir == 'E') robots[idx].dir = 'N';
                } else { // 'R'
                    // N->E, E->S, S->W, W->N
                    if (robots[idx].dir == 'N') robots[idx].dir = 'E';
                    else if (robots[idx].dir == 'E') robots[idx].dir = 'S';
                    else if (robots[idx].dir == 'S') robots[idx].dir = 'W';
                    else if (robots[idx].dir == 'W') robots[idx].dir = 'N';
                }
            }
        } else if (action == 'F') {
            // Move forward step by step.
            for (int step = 1; step <= cnt; ++step) {
                int nextX = robots[idx].x;
                int nextY = robots[idx].y;
                if (robots[idx].dir == 'N') nextY -= 1;
                else if (robots[idx].dir == 'E') nextX += 1;
                else if (robots[idx].dir == 'S') nextY += 1;
                else if (robots[idx].dir == 'W') nextX -= 1;
                
                // Check wall collision.
                if (nextX < 0 || nextX >= maxX || nextY < 0 || nextY >= maxY) {
                    return "Robot " + std::to_string(idx + 1) + " crashes into the wall";
                }
                // Check robot collision.
                if (grid[nextY][nextX] != 0) {
                    return "Robot " + std::to_string(idx + 1) + " crashes into robot " + std::to_string(grid[nextY][nextX]);
                }
                // Move is safe: update grid and robot position.
                grid[robots[idx].y][robots[idx].x] = 0;
                robots[idx].x = nextX;
                robots[idx].y = nextY;
                grid[robots[idx].y][robots[idx].x] = idx + 1;
            }
        }
        // Unknown action is ignored (per spec only L, R, F).
    }
    return "OK";
}

#include <cassert>
#include <string>
#include <vector>

// Include the solution function declaration here (or copy the function above).

int main() {
    // Test case 1: Simple movement, no collision.
    {
        int maxX = 5, maxY = 5, N = 2, M = 2;
        std::vector<RobotState> robots = {{0, 4, 'N'}, {4, 4, 'S'}}; // internal: bottom-left? Actually (x=0, y=4) is top-left corner in internal.
        std::vector<Command> cmds = {{1, 'F', 1}, {2, 'F', 1}};
        // Converting from input 1-based: robot1 at (1,1) -> internal x=0, y=MaxY-1=4; robot2 at (5,5) -> internal x=4, y=0.
        // But let's test with direct internal coordinates for simplicity.
        // We'll test with valid internal coordinates.
        // Actually, better to test with a fresh correct setup.
    }
    // Direct test with internal coordinates consistent with grid.
    // Grid 5x5, robot1 at (x=1, y=1) internal, facing 'E', robot2 at (x=3, y=3) internal.
    {
        int maxX = 5, maxY = 5, N = 1, M = 1;
        std::vector<RobotState> robots = {{1, 1, 'E'}};
        std::vector<Command> cmds = {{1, 'F', 2}};
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "OK");
    }
    // Wall crash.
    {
        int maxX = 3, maxY = 3, N = 1, M = 1;
        std::vector<RobotState> robots = {{0, 2, 'N'}}; // internal top-left, facing north
        std::vector<Command> cmds = {{1, 'F', 1}};
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "Robot 1 crashes into the wall");
    }
    // Robot collision.
    {
        int maxX = 5, maxY = 5, N = 2, M = 1;
        std::vector<RobotState> robots = {{0, 0, 'E'}, {1, 0, 'N'}}; // robot1 at (0,0), robot2 at (1,0)
        std::vector<Command> cmds = {{1, 'F', 1}}; // robot1 moves east to (1,0) where robot2 is
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "Robot 1 crashes into robot 2");
    }
    // Turning only, no movement.
    {
        int maxX = 4, maxY = 4, N = 1, M = 2;
        std::vector<RobotState> robots = {{2, 2, 'N'}};
        std::vector<Command> cmds = {{1, 'L', 1}, {1, 'R', 1}};
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "OK");
    }
    // Multiple forward steps into same robot.
    {
        int maxX = 4, maxY = 4, N = 2, M = 1;
        std::vector<RobotState> robots = {{0, 0, 'E'}, {2, 0, 'S'}}; // robot1 at (0,0), robot2 at (2,0)
        std::vector<Command> cmds = {{1, 'F', 2}}; // move 2 steps: (1,0) then (2,0) collision at second step
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "Robot 1 crashes into robot 2");
    }
    // Long rotation count modulo.
    {
        int maxX = 2, maxY = 2, N = 1, M = 1;
        std::vector<RobotState> robots = {{0, 0, 'N'}};
        std::vector<Command> cmds = {{1, 'L', 5}}; // 5 left turns = 1 left turn, becomes 'W'
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "OK");
    }
    // Zero commands.
    {
        int maxX = 3, maxY = 3, N = 1, M = 0;
        std::vector<RobotState> robots = {{1, 1, 'E'}};
        std::vector<Command> cmds = {};
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "OK");
    }
    // Robot moves off grid after turning.
    {
        int maxX = 3, maxY = 3, N = 1, M = 2;
        std::vector<RobotState> robots = {{2, 2, 'S'}};
        std::vector<Command> cmds = {{1, 'L', 1}, {1, 'F', 1}}; // S -> E, then move east from x=2 to x=3 which is out of bounds (maxX=3 so x indices 0,1,2)
        std::string result = simulateRobotMoves(maxX, maxY, N, M, robots, cmds);
        assert(result == "Robot 1 crashes into the wall");
    }
    return 0;
}
