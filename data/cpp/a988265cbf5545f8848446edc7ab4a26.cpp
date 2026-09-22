// Write a C++ function `bool pathExists(int maze[MAX_ROWS][MAX_COLS], int rows, int cols, int startRow, int startCol, int goalValue)` that determines whether a robot can move from a starting cell (containing value 2) to any cell containing the goal value (3) in a rectangular maze. The maze is a grid where 0 represents a wall (impassable), 1 represents a passable open cell, 2 is the robot's starting position (also passable), and 3 is the goal cell (also passable). The robot can move only left, right, up, or down one cell at a time and cannot leave the grid or pass through walls (value 0). The function should return `true` if there exists any path from the start to a goal, and `false` otherwise. The maze dimensions are at most 10×10. Do not use recursion—implement an iterative depth-first search (DFS) using a stack of states, where each state stores the robot's position. The function must handle cases where the start is already on a goal, where no path exists, and where the start position is invalid (should return false). Assume the input maze always contains exactly one cell with value 2 and at least one cell with value 3.

#include <assert.h>
#include <iostream>

#define MAX_ROWS 10
#define MAX_COLS 10

// Include the solution function here (or link it)

int main() {
    // Test 1: Simple maze where path exists
    int maze1[MAX_ROWS][MAX_COLS] = {
        {2, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 3}
    };
    assert(pathExists(maze1, 4, 4, 0, 0) == true);

    // Test 2: Maze where no path exists (goal blocked by walls)
    int maze2[MAX_ROWS][MAX_COLS] = {
        {2, 1, 0},
        {0, 1, 0},
        {0, 0, 3}
    };
    assert(pathExists(maze2, 3, 3, 0, 0) == false);

    // Test 3: Start already on a goal (treat start as any value)
    int maze3[MAX_ROWS][MAX_COLS] = {
        {3, 0},
        {0, 1}
    };
    assert(pathExists(maze3, 2, 2, 0, 0) == true);

    // Test 4: Invalid start coordinates (out of bounds)
    int maze4[MAX_ROWS][MAX_COLS] = {
        {2, 1},
        {1, 3}
    };
    assert(pathExists(maze4, 2, 2, 5, 5) == false);

    // Test 5: Start on a wall (value 0)
    int maze5[MAX_ROWS][MAX_COLS] = {
        {0, 1},
        {1, 3}
    };
    assert(pathExists(maze5, 2, 2, 0, 0) == false);

    // Test 6: Maze with multiple goals and multiple paths
    int maze6[MAX_ROWS][MAX_COLS] = {
        {2, 1, 3},
        {0, 1, 1},
        {0, 0, 3}
    };
    assert(pathExists(maze6, 3, 3, 0, 0) == true);

    // Test 7: Single cell maze with start equal to goal
    int maze7[MAX_ROWS][MAX_COLS] = {
        {3}
    };
    assert(pathExists(maze7, 1, 1, 0, 0) == true);

    // Test 8: Single cell maze with only a start, no goal
    int maze8[MAX_ROWS][MAX_COLS] = {
        {2}
    };
    assert(pathExists(maze8, 1, 1, 0, 0) == false);

    // Test 9: Larger maze where path is a long corridor
    int maze9[MAX_ROWS][MAX_COLS] = {
        {2, 1, 1, 1},
        {0, 0, 0, 1},
        {1, 1, 1, 1},
        {0, 0, 0, 3}
    };
    assert(pathExists(maze9, 4, 4, 0, 0) == true);

    // Test 10: Maze with no open cells except start and blocked goal
    int maze10[MAX_ROWS][MAX_COLS] = {
        {2, 0},
        {0, 3}
    };
    assert(pathExists(maze10, 2, 2, 0, 0) == false);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <stack>
#include <cstring>

#define MAX_ROWS 10
#define MAX_COLS 10

// Determine if a path exists from the start (value 2) to any goal (value 3) in the maze.
// Values: 0=wall, 1=open, 2=start, 3=goal. Iterative DFS using a stack.
bool pathExists(int maze[MAX_ROWS][MAX_COLS], int rows, int cols, int startRow, int startCol, int goalValue = 3) {
    // Validate start position
    if (startRow < 0 || startRow >= rows || startCol < 0 || startCol >= cols) {
        return false;
    }
    // The start should be a passable cell (value 2), but we allow any passable value for flexibility
    if (maze[startRow][startCol] == 0) {
        return false;
    }

    // Visited grid to avoid revisiting cells
    bool visited[MAX_ROWS][MAX_COLS];
    memset(visited, false, sizeof(visited));

    // Stack of positions as pairs (row, col) encoded as a single int: row * MAX_COLS + col
    std::stack<int> stack;
    stack.push(startRow * MAX_COLS + startCol);
    visited[startRow][startCol] = true;

    // Four directions: up, down, left, right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!stack.empty()) {
        int pos = stack.top();
        stack.pop();
        int r = pos / MAX_COLS;
        int c = pos % MAX_COLS;

        // Check if this cell is a goal
        if (maze[r][c] == goalValue) {
            return true;
        }

        // Explore all four neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            // Check bounds and whether the neighbor is passable (not 0) and not visited
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (maze[nr][nc] != 0 && !visited[nr][nc]) {
                    visited[nr][nc] = true;
                    stack.push(nr * MAX_COLS + nc);
                }
            }
        }
    }

    return false; // No path found
}

// The solution treats the maze as a graph where cells are nodes and edges exist between adjacent passable cells (values 1, 2, or 3). Use an iterative DFS from the starting position. Maintain a stack of visited positions and a separate visited set (e.g., a boolean grid) to avoid revisiting and infinite loops. At each step, pop a position, check if it's a goal (value 3) and return true, otherwise push all valid neighboring cells (not walls, within bounds, not visited) onto the stack and mark them visited. Edge cases: if start coordinates are out of bounds or the start cell is not value 2, return false. If the start is already a goal (which shouldn't normally happen because start is 2, but handle any value), return true. Time complexity is O(rows × cols) because each cell is visited at most once. Space complexity is O(rows × cols) for the visited grid and the stack (worst case all cells are passable). No recursion avoids call stack overflow for larger grids (though here max 100 cells).
