/*
Write a C++ function that simulates the movement and score calculation of a snake on a square grid without using graphics. You are given a series of directional commands (`up`, `down`, `left`, `right`) starting from an initial snake of length 3 located at the top-left corner of a `GRID_SIZE` × `GRID_SIZE` grid (with `GRID_SIZE = 10`). The snake's head is at grid cell (0,0), the body extends to (1,0) and (2,0) heading downward initially. Food appears at a fixed position (5,5) at the start. Each command moves the snake one cell in the given direction. If the head moves into a food cell, the snake grows by one cell at the tail (i.e., the tail does not shrink) and the score increases by 1; otherwise, the tail shrinks by one cell. The game ends if the head moves outside the grid bounds or collides with its own body (excluding the tail that is about to shrink). Implement a function `simulate_snake(const std::vector<std::string>& commands)` that returns a `std::pair<int, bool>` where the first element is the final score and the second is `true` if the snake survives all commands (no collision or out-of-bounds) or `false` if it dies before finishing. The commands are given in lowercase strings from the set {"up","down","left","right"}. The snake cannot reverse direction into itself (e.g., if moving down, the next command cannot be "up") — such a command is invalid and should be ignored (the snake continues in its current direction for that step). If the snake dies before all commands are processed, stop simulation immediately and return the current score and `false`. Assume the input vector may be empty, in which case return score 0 and `true`.
*/

#include <vector>
#include <string>
#include <utility>
#include <deque>
#include <set>

// Simulate snake movement and return {final_score, alive_at_end}
std::pair<int, bool> simulate_snake(const std::vector<std::string>& commands) {
    const int GRID_SIZE = 10;
    const std::pair<int,int> FOOD = {5,5};
    
    // Directions: down, up, left, right
    std::string current_dir = "down";
    
    // Snake initial body: head at (0,0), then (1,0), then (2,0)
    std::deque<std::pair<int,int>> snake;
    snake.push_back({0,0});
    snake.push_back({1,0});
    snake.push_back({2,0});
    
    // Occupied cells set
    std::set<std::pair<int,int>> occupied;
    for (const auto& cell : snake) occupied.insert(cell);
    
    int score = 0;
    bool alive = true;
    
    for (const auto& cmd : commands) {
        // Determine new direction: ignore invalid opposite commands
        std::string new_dir = cmd;
        if ((current_dir == "up" && cmd == "down") ||
            (current_dir == "down" && cmd == "up") ||
            (current_dir == "left" && cmd == "right") ||
            (current_dir == "right" && cmd == "left")) {
            new_dir = current_dir; // invalid, keep current
        }
        current_dir = new_dir;
        
        // Compute new head position
        int head_x = snake.front().first;
        int head_y = snake.front().second;
        if (current_dir == "up") head_x--;
        else if (current_dir == "down") head_x++;
        else if (current_dir == "left") head_y--;
        else if (current_dir == "right") head_y++;
        
        // Check out of bounds
        if (head_x < 0 || head_x >= GRID_SIZE || head_y < 0 || head_y >= GRID_SIZE) {
            alive = false;
            break;
        }
        
        // Determine if food is eaten before moving tail
        bool eats = (head_x == FOOD.first && head_y == FOOD.second);
        
        // Tail position (will move unless food eaten)
        auto tail = snake.back();
        
        // Check collision: with any occupied cell except the tail that will move away
        if (occupied.count({head_x, head_y}) > 0 && !(eats == false && std::make_pair(head_x, head_y) == tail)) {
            alive = false;
            break;
        }
        
        // Move head
        snake.push_front({head_x, head_y});
        occupied.insert({head_x, head_y});
        
        if (eats) {
            score++;
            // Do not remove tail, snake grows
        } else {
            // Remove tail
            occupied.erase(tail);
            snake.pop_back();
        }
    }
    
    return {score, alive};
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Declare the function (or include the solution above)
std::pair<int, bool> simulate_snake(const std::vector<std::string>&);

int main() {
    // Empty commands: survive, score 0
    assert(simulate_snake({}) == std::make_pair(0, true));
    
    // Move away from food, no growth
    assert(simulate_snake({"right", "right", "right"}) == std::make_pair(0, true));
    
    // Walk to food at (5,5): need 5 down then 5 right (no collisions)
    assert(simulate_snake({"down","down","down","down","down",
                           "right","right","right","right","right"}) == std::make_pair(1, true));
    
    // Attempt to reverse: ignored, no death
    assert(simulate_snake({"up"}) == std::make_pair(0, true)); // "up" opposite to "down" ignored
    
    // Move into wall: death
    assert(simulate_snake({"up","up"}) == std::make_pair(0, false)); // first up ignored, second up moves out of bounds
    
    // Collision with self: go down then right then up (head would hit body)
    assert(simulate_snake({"right","up","left"}) == std::make_pair(0, false));
    
    // Grow then immediately collide with new tail (pathological)
    // Not easy to construct quickly, but test survival after growth:
    assert(simulate_snake({"down","down","down","down","down",
                           "right","right","right","right","right",
                           "up","up"}) == std::make_pair(1, true));
    
    return 0;
}

// The solution models the snake as a queue of grid coordinates, with the head at the front and tail at the back. Maintain a set of occupied cells for O(1) collision checks. For each valid command (non-empty, not opposite to current direction, not causing immediate death), compute the next head position by moving in the given direction. If the new head is out of bounds or collides with a body cell that is not the tail (considering that the tail will move unless food is eaten), the snake dies. Otherwise, update the head: push it to the front of the queue and add to the occupied set. If the new head equals the food position, increment score and do not remove the tail; otherwise, remove the tail from the queue and set. The current direction is updated only if the command is valid (not opposite), otherwise keep current direction. The simulation stops early on death. Edge cases: empty command vector, commands that attempt to reverse, moving into the cell that will be vacated by the tail (allowable because tail moves away), and food eaten at the moment of death (if head lands on food but also would collide with body, death takes precedence). Time complexity is O(n) where n is the number of commands, as each step involves constant-time operations on a queue and set (amortized). Space complexity is O(L) where L is the maximum snake length, bounded by GRID_SIZE^2.
