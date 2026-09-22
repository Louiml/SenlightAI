Write a C++ function `bool canMeasureWater(int x, int y, int target)` that determines whether it is possible to measure exactly `target` liters of water using two jugs with capacities `x` liters and `y` liters. You may fill either jug completely, empty either jug completely, or pour water from one jug into the other until either the source jug is empty or the desticnation jug is full. The function should return `true` if the target amount can be obtained in either jug (or the sum of both) at any step, otherwise `false`. Assume `x` and `y` are non-negative integers, `target` is a non-negative integer, and all inputs fit within a 32-bit signed integer. Note: The direct mathematical condition is that `target` must be divisible by the greatest common divisor (GCD) of `x` and `y`, and `target <= x + y`, but you must implement a simulation-based approach that models the state transitions.
The problem is a classic two-jug puzzle. The simplest and most correct solution uses number theory: measuring is possible iff `target` is a multiple of `gcd(x, y)` and `target <= x + y`. This works because the set of all achievable amounts in either jug is exactly the set of multiples of `gcd(x, y)` up to `x + y`. Edge cases include: `target == 0` (always possible because both jugs start empty), either `x` or `y` being `0` (then only amounts that are 0 or the other capacity are achievable), and when `x + y < target` (impossible). However, the task explicitly asks for a simulation-based approach that models pouring operations. A standard BFS over all possible states `(a, b)` where `a` is water in the first jug and `b` in the second, with edges representing fill, empty, and pour operations, works correctly. The state space is bounded by `(x+1)*(y+1)`, so for typical inputs it is small. We check every state; if `a == target`, `b == target`, or `a + b == target`, we return true. Time complexity is `O(x*y)` per call, space `O(x*y)` for the visited set. Handling zero capacities carefully avoids infinite loops.
#include <vector>
#include <queue>
#include <utility>

// Determine whether measuring exactly 'target' liters is possible
// using two jugs with capacities 'x' and 'y' via filling, emptying,
// and pouring operations. Simulation via BFS over all reachable states.
bool canMeasureWater(int x, int y, int target) {
    if (target < 0 || target > x + y) return false;
    if (target == 0) return true;
    if (x < 0 || y < 0) return false;
    
    // Use a visited matrix (size (x+1) x (y+1)) to avoid revisiting states.
    std::vector<std::vector<bool>> visited(x + 1, std::vector<bool>(y + 1, false));
    std::queue<std::pair<int, int>> q;
    
    // Start with both jugs empty.
    q.push({0, 0});
    visited[0][0] = true;
    
    while (!q.empty()) {
        auto [a, b] = q.front();
        q.pop();
        
        // Check if target is achieved in either jug or their sum.
        if (a == target || b == target || a + b == target) {
            return true;
        }
        
        // Generate all possible next states.
        // 1. Fill first jug completely.
        if (!visited[x][b]) { visited[x][b] = true; q.push({x, b}); }
        // 2. Fill second jug completely.
        if (!visited[a][y]) { visited[a][y] = true; q.push({a, y}); }
        // 3. Empty first jug.
        if (!visited[0][b]) { visited[0][b] = true; q.push({0, b}); }
        // 4. Empty second jug.
        if (!visited[a][0]) { visited[a][0] = true; q.push({a, 0}); }
        // 5. Pour from first to second until first empty or second full.
        int pour1 = std::min(a, y - b);
        if (pour1 > 0 && !visited[a - pour1][b + pour1]) {
            visited[a - pour1][b + pour1] = true;
            q.push({a - pour1, b + pour1});
        }
        // 6. Pour from second to first until second empty or first full.
        int pour2 = std::min(b, x - a);
        if (pour2 > 0 && !visited[a + pour2][b - pour2]) {
            visited[a + pour2][b - pour2] = true;
            q.push({a + pour2, b - pour2});
        }
    }
    
    return false;
}
#include <cassert>

int main() {
    // Basic cases from classic puzzle.
    assert(canMeasureWater(3, 5, 4) == true);   // 3 and 5 jugs -> can measure 4
    assert(canMeasureWater(2, 6, 5) == false);  // gcd=2, 5 not divisible by 2, impossible
    assert(canMeasureWater(0, 5, 5) == true);   // one jug of capacity 5, target 5
    assert(canMeasureWater(0, 5, 3) == false);  // only 0 or 5 achievable
    assert(canMeasureWater(1, 1, 2) == true);   // fill both, sum = 2
    assert(canMeasureWater(1, 1, 3) == false);  // target > capacity sum
    assert(canMeasureWater(0, 0, 0) == true);   // target 0 always possible
    assert(canMeasureWater(7, 11, 6) == true);  // gcd 1, target <=18, possible
    assert(canMeasureWater(4, 6, 8) == true);   // gcd 2, target 8 divisible, but 8 <= 10, possible via sum
    assert(canMeasureWater(4, 6, 7) == false);  // gcd 2, 7 not divisible
    return 0;
}
