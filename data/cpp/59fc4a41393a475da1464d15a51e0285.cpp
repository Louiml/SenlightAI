Write a standalone C++ function named `snakeCollisionOrWall` that simulates the core movement and collision logic from the provided Snake game snippet, but without any platform-specific dependencies (no conio.h, windows.h, or terminal output). The function takes as input: the current head x and y coordinates, the current direction (a character `'w'`, `'a'`, `'s'`, or `'d'` representing up, left, down, right), the snake length (an integer ≥ 1), and a vector of `std::pair<int,int>` representing the snake body segments from head (index 0) to tail (index length-1). The function should return an `enum class MoveResult { Success, WallHit, SelfCollision }`. It should compute the new head position by moving one step in the given direction, applying boundary constraints of `x` in [1, 50] and `y` in [1, 25] (inclusive). If the new head position is outside these bounds, return `WallHit` without modifying anything. If the new head position coincides with any body segment other than the tail (which will move away in the next step), return `SelfCollision` (treat a snake of length 1 as never colliding with itself). Otherwise, update the body vector by shifting all segments toward the tail and setting the head to the new position, then return `Success`. The function must not allocate memory or use global state.
The solution simulates the snake's movement by first computing the prospective head position based on the direction. The bounds check (x between 1 and 50, y between 1 and 25) is straightforward; if the new head goes out of bounds, return `WallHit` immediately. For self-collision, compare the new head position against every body segment except the last one (the tail). This is because in the real snake movement, the tail segment moves away in the same tick, so it cannot cause a collision—except when length is 1, where no collision can occur. If a match is found, return `SelfCollision`. Otherwise, shift the body segments: for `i` from length-1 down to 1, copy `body[i-1]` into `body[i]`, then set `body[0]` to the new head position. The complexity is O(n) time due to the shift and collision check (both linear in snake length), and O(1) auxiliary space since we modify the input vector in place. Edge cases include length=1 (no self-collision), moving into the tail position (allowed because tail moves), and boundary extremes (x=1 moving left causes wall hit, y=25 moving down causes wall hit).
#include <vector>
#include <utility>

enum class MoveResult { Success, WallHit, SelfCollision };

// Simulates one step of snake movement with collision detection.
// Modifies the body vector on success; leaves it unchanged on failure.
MoveResult snakeCollisionOrWall(
    int& x, int& y,
    char direction,
    std::vector<std::pair<int,int>>& body
) {
    const int maxX = 50;
    const int maxY = 25;

    int newX = x;
    int newY = y;

    if (direction == 'w') newY -= 1;
    else if (direction == 's') newY += 1;
    else if (direction == 'a') newX -= 1;
    else if (direction == 'd') newX += 1;
    else return MoveResult::Success; // invalid direction, no movement

    if (newX < 1 || newX > maxX || newY < 1 || newY > maxY) {
        return MoveResult::WallHit;
    }

    const int length = static_cast<int>(body.size());
    if (length > 1) {
        // Check collision with all segments except the tail (last)
        for (int i = 0; i < length - 1; ++i) {
            if (body[i].first == newX && body[i].second == newY) {
                return MoveResult::SelfCollision;
            }
        }
    }

    // Shift body segments toward tail
    for (int i = length - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    body[0] = std::make_pair(newX, newY);

    x = newX;
    y = newY;
    return MoveResult::Success;
}
#include <cassert>
#include <vector>
#include <utility>

// forward declaration of the solution function
enum class MoveResult { Success, WallHit, SelfCollision };
MoveResult snakeCollisionOrWall(int&, int&, char, std::vector<std::pair<int,int>>&);

int main() {
    // Test 1: Basic success movement
    int x = 5, y = 5;
    std::vector<std::pair<int,int>> body = {{5,5}, {4,5}, {3,5}};
    MoveResult r = snakeCollisionOrWall(x, y, 'd', body);
    assert(r == MoveResult::Success);
    assert(x == 6 && y == 5);
    assert(body[0] == std::make_pair(6,5));
    assert(body[1] == std::make_pair(5,5));
    assert(body[2] == std::make_pair(4,5));

    // Test 2: Wall hit at left boundary
    x = 1; y = 10;
    body = {{1,10}, {2,10}};
    r = snakeCollisionOrWall(x, y, 'a', body);
    assert(r == MoveResult::WallHit);
    assert(x == 1 && y == 10); // unchanged
    assert(body.size() == 2);

    // Test 3: Wall hit at top boundary
    x = 20; y = 1;
    body = {{20,1}, {20,2}};
    r = snakeCollisionOrWall(x, y, 'w', body);
    assert(r == MoveResult::WallHit);

    // Test 4: Self collision (head runs into body segment)
    x = 5; y = 5;
    body = {{5,5}, {4,5}, {5,4}};
    r = snakeCollisionOrWall(x, y, 's', body); // moving down into (5,4) which exists
    assert(r == MoveResult::SelfCollision);
    assert(body[0] == std::make_pair(5,5)); // unchanged

    // Test 5: Allowed to move into tail position (length > 1)
    x = 3; y = 3;
    body = {{3,3}, {4,3}, {4,4}};
    r = snakeCollisionOrWall(x, y, 'a', body); // move left to (2,3) not tail
    assert(r == MoveResult::Success);

    // Test with tail at (4,3) moving right: head would go to (4,3) which is tail, allowed
    x = 3; y = 3;
    body = {{3,3}, {4,3}, {4,4}};
    r = snakeCollisionOrWall(x, y, 'd', body); // new head (4,3) is tail segment
    assert(r == MoveResult::Success);
    assert(x == 4 && y == 3);
    // After shift, tail moves away, head occupies (4,3)
    assert(body[1] == std::make_pair(3,3));

    // Test 6: Length 1 cannot self collide
    x = 10; y = 10;
    body = {{10,10}};
    r = snakeCollisionOrWall(x, y, 'd', body);
    assert(r == MoveResult::Success);
    assert(x == 11 && y == 10);

    // Test 7: Invalid direction does nothing
    x = 5; y = 5;
    body = {{5,5}};
    r = snakeCollisionOrWall(x, y, 'x', body);
    assert(r == MoveResult::Success);
    assert(x == 5 && y == 5);

    // Test 8: Boundary bottom and right
    x = 50; y = 10;
    body = {{50,10}, {49,10}};
    r = snakeCollisionOrWall(x, y, 'd', body);
    assert(r == MoveResult::WallHit);

    x = 30; y = 25;
    body = {{30,25}, {30,24}};
    r = snakeCollisionOrWall(x, y, 's', body);
    assert(r == MoveResult::WallHit);

    return 0;
}
