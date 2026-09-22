Write a C++ function named `snakeGameDuration` that simulates the classic Snake game on an `n x n` board and returns the number of seconds (time steps) the snake survives before hitting a wall or its own body. The function takes four parameters: the board size `n`, a vector of apple positions (each given as a pair of 1-based row and column coordinates), a vector of direction-change commands (each given as a pair of time in seconds and a character `'L'` or `'D'` for left or right turn), and the starting direction (0 = right, 1 = down, 2 = left, 3 = up). The snake starts at the top-left cell (row 1, column 1) with length 1, moves one cell per second, and turns according to the commands at the exact second the command occurs (the turn is applied after moving that second). Apples are consumed when the snake's head reaches their cell, increasing the snake's length by 1 (the tail does not move on that second). The game ends when the head moves outside the board or onto a cell occupied by the snake's own body. The function should handle up to 100×100 board sizes, up to 1000 apples, and up to 1000 commands. Test cases include boards without apples, commands that cause a collision with the tail after it has moved, and multiple apples on the same row/column. Time complexity must be O(max(n², total actions)) and space O(n²).

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: No apples, no commands, snake goes right until wall
    assert(snakeGameDuration(4, {}, {}, 0) == 4);

    // Test 2: Snake eats apples in a line, grows, then hits wall
    std::vector<std::pair<int, int>> apples1 = {{1, 2}, {1, 3}, {1, 4}};
    assert(snakeGameDuration(4, apples1, {}, 0) == 5);

    // Test 3: Turn right at time 1, then go down to eat apple at (2,2)
    std::vector<std::pair<int, int>> apples2 = {{2, 2}};
    std::vector<std::pair<int, char>> commands1 = {{1, 'D'}};
    // Path: (1,1) -> (1,2) turn down -> (2,2) eat apple, then (3,2) ... hits bottom wall
    // After eating at (2,2), length = 2. At time 3 head at (3,2), tail moves from (2,1)? Actually no apple, tail moves.
    // Timeline: t1: (1,2) blank, tail pops (1,1), turn D. t2: (2,2) apple, no pop, length 2. t3: (3,2) blank, pop tail (1,2). t4: (4,2) blank, pop tail (2,2). t5: (5,2) out, return 5.
    assert(snakeGameDuration(4, apples2, commands1, 0) == 5);

    // Test 4: Self-collision after growing and turning
    // Board 3x3, apples at (1,2), (2,2), commands: D at t1, R at t2
    // t1: (1,2) apple, length=2
    // t2: turn D? Actually command at t1 is D, so direction down after t1 move. t2: head to (2,2) apple, length=3
    // t3: direction right? No, command at t2 is R, so after t2 move direction right. t3: head to (2,3) blank, tail moves from (1,1)
    // t4: head to (1,3) blank, tail moves from (1,2)
    // t5: head to (0,3) out, returns 5
    // Instead, let's force a real self-collision:
    // Board 3x3, apples at (1,2), (2,2), (2,1) maybe
    // Let's design: start right, t1 eat apple at (1,2), command D at t1
    // t2 eat apple at (2,2), command L at t2 -> direction left
    // t3 head to (2,1) blank, tail pops (1,1)
    // t4 head to (2,2) but that cell is snake body (from t2, still there because tail hasn't reached? Let's check: snake body after t3: head (2,1), then (2,2), (1,2) (tail). After t3, tail popped (1,1) so body cells: (2,1), (2,2), (1,2). At t4 head to (2,2) which is snake body, collision, return 4.
    std::vector<std::pair<int, char>> commands2 = {{1, 'D'}, {2, 'L'}};
    std::vector<std::pair<int, int>> apples3 = {{1, 2}, {2, 2}};
    assert(snakeGameDuration(3, apples3, commands2, 0) == 4);

    // Test 5: No apples, turn left immediately, snake survives longer
    std::vector<std::pair<int, char>> commands3 = {{1, 'L'}};
    // t1: (1,2) blank, tail pops (1,1), turn left -> direction up
    // t2: head to (0,2) out, return 2
    assert(snakeGameDuration(2, {}, commands3, 0) == 2);

    // Test 6: Commander at time 0? Not allowed, but check time 1 only
    std::vector<std::pair<int, char>> commands4 = {{1, 'D'}, {2, 'D'}};
    // Board 5, no apples. t1: (1,2), turn down. t2: (2,2), turn down. t3: (3,2) ... down until bottom wall at t6 (row indices 0..4, so row 4 at t5, t6 row5 out) Actually start (0,0) = time 0? We start moving at t1. Let's compute: t1 (0,1) turn D. t2 (1,1) turn D? command at t2 turns down again, same direction. t3 (2,1), t4 (3,1), t5 (4,1), t6 (5,1) out => 6.
    assert(snakeGameDuration(5, {}, commands4, 0) == 6);

    // Test 7: Large board, single apple at far corner, snake eats it and continues
    std::vector<std::pair<int, int>> apples4 = {{100, 100}};
    // Start right, need 99 steps to reach col 100 at row 1, then down 99 steps to row 100, total 99+99=198 steps to eat?, actually time to row1,col100 is 99 seconds (since start at col1). Then turn down at time 99? No command. So snake goes right until wall at t=100 (hits col101 out). So apple is at col100, but snake reaches col100 at t=99, eats it, then at t=100 goes to col101 out, returns 100. But test might be too large for assert? It's fine.
    assert(snakeGameDuration(100, {{100, 100}}, {}, 0) == 100);

    // Test 8: Command exactly at collision time, still collision
    std::vector<std::pair<int, char>> commands5 = {{4, 'D'}};
    // Board 4, start right. t1: (0,1), t2: (0,2), t3: (0,3), t4: compute head (0,4) out, command at t4 is after move, so collision at t4.
    assert(snakeGameDuration(4, {}, commands5, 0) == 4);

    return 0;
}

#include <vector>
#include <queue>
#include <utility>

// Simulate Snake game on an n x n board.
// board positions in apples and commands are 1-based (row, col)
// startDirection: 0=right, 1=down, 2=left, 3=up
// Returns number of seconds survived.
int snakeGameDuration(int n, const std::vector<std::pair<int, int>>& apples,
                      const std::vector<std::pair<int, char>>& commands,
                      int startDirection) {
    // board values: 0 = blank, 1 = snake body, 2 = apple
    std::vector<std::vector<int>> board(n, std::vector<int>(n, 0));

    for (const auto& apple : apples) {
        board[apple.first - 1][apple.second - 1] = 2;
    }

    // Snake representation: queue front = tail, back = head
    std::queue<std::pair<int, int>> snake;
    snake.push({0, 0});
    board[0][0] = 1;

    int direction = startDirection;
    int dy[4] = {0, 1, 0, -1};
    int dx[4] = {1, 0, -1, 0};

    int commandIndex = 0;

    for (int time = 1; ; ++time) {
        int newR = snake.back().first + dy[direction];
        int newC = snake.back().second + dx[direction];

        // Collision with wall or own body
        if (newR < 0 || newR >= n || newC < 0 || newC >= n ||
            board[newR][newC] == 1) {
            return time;
        }

        // If blank, remove tail; if apple, keep tail (growth)
        if (board[newR][newC] == 0) {
            board[snake.front().first][snake.front().second] = 0;
            snake.pop();
        }

        board[newR][newC] = 1;
        snake.push({newR, newC});

        // Apply direction change after the move, if scheduled at this time
        if (commandIndex < static_cast<int>(commands.size()) &&
            commands[commandIndex].first == time) {
            if (commands[commandIndex].second == 'L') {
                direction = (direction + 3) % 4; // left turn
            } else {
                direction = (direction + 1) % 4; // right turn
            }
            ++commandIndex;
        }
    }
}

// The solution uses a queue to represent the snake's body, where the front holds the tail and the back holds the head. A 2D vector of integers tracks the board state: 0 for blank, 1 for snake body, 2 for apple. Initialize the board with all cells blank, place apples, set the cell (0,0) to 1, and push the head coordinate into the queue. Maintain a direction index (`direction`) initialized from the parameter, and an index pointing into the command list. For each time step starting at 1, compute the next head position by adding the direction offsets. Check for out-of-bounds or collision with the snake body (board cell == 1); if so, return the current time. If the next cell is blank, remove the tail: set its board cell to 0 and pop from the queue. Then set the new head cell to 1 and push it. If the next cell is an apple, do not remove the tail (snake grows). After moving, if the current time matches the next command's time, update the direction: `'L'` subtracts 1 and `'D'` adds 1, using modulo 4 to keep in range. Finally, if no collision occurs, the loop ends when the time exceeds some large limit, but in practice the game always ends. Complexity is O(T) where T is the number of steps until death, which is at most O(n²) because the snake cannot grow beyond n² cells; plus O(k) for placing apples, so total O(n² + k + l). Space is O(n²) for the board and O(l) for commands, with the queue up to O(n²).
