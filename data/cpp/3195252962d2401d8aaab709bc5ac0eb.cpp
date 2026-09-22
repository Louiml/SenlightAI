// Write a C++ function that simulates a robot's movement on a 2D grid. The robot starts at given integer coordinates `(x, y)`. It receives four movement commands: `a` moves left, `b` moves right, `c` moves down, `d` moves up. The robot must stay inside an axis-aligned rectangle with lower-left corner `(x1, y1)` and upper-right corner `(x2, y2)`, inclusive. The function should return `true` if, after applying all movements (in the order: left `a` times, right `b` times, down `c` times, up `d` times), the final position is inside the rectangle, and also the robot never leaves the rectangle at any intermediate step. However, if the rectangle has zero width (i.e., `x1 == x2`), the robot is allowed to move horizontally only if the total left and right movements cancel out (i.e., `a + b == 0`), because otherwise it must leave. Similarly, if the rectangle has zero height (`y1 == y2`), vertical movements are allowed only if `c + d == 0`. The function signature is `bool canNavigate(ll x, ll y, ll a, ll b, ll c, ll d, ll x1, ll y1, ll x2, ll y2)` where `ll` is `long long`. Input values are all integers (possibly large), and coordinates can be negative. Return `true` if the robot can complete the movement, `false` otherwise.
// The key is to compute the final coordinates after applying all four directional moves. Since left moves subtract from `x` and right moves add, the net horizontal displacement is `b - a`, so final `x` is `x + b - a`. Similarly, down subtracts from `y` (if we interpret `c` as moving down, decreasing `y`), and up adds, so final `y` is `y + d - c`. The robot's path is monotonic in each axis because all left moves happen before all right moves, and all down moves before all up moves. However, the order within a direction does not matter because moving left `a` times always keeps `x` decreasing from the starting value, and moving right `b` times always keeps `x` increasing from the value after the left moves. The worst-case intermediate position occurs either at the start, after all left moves, after all right moves, or at the final position. Because left moves only decrease `x`, the minimum `x` along the horizontal path is `min(x, x - a, x - a + b)`. Similarly, the maximum `x` is `max(x, x - a, x - a + b)`. For the vertical direction, the minimum `y` is `min(y, y - c, y - c + d)` and maximum is `max(y, y - c, y - c + d)`. The robot stays inside the rectangle iff both the horizontal interval `[minX, maxX]` is within `[x1, x2]` and the vertical interval `[minY, maxY]` is within `[y1, y2]`. But there is a subtlety: the rectangle may be degenerate (zero width or height). If `x1 == x2`, the only way to stay inside is to never move horizontally, i.e., `a == 0` and `b == 0`, because any left or right move would immediately leave. Similarly for vertical. The provided code checks `(x2 > x1 || a + b == 0)` which is equivalent to: either there is positive width, or the total horizontal displacement is zero. But this condition alone is insufficient because even if `a+b==0`, moving left and then right might leave temporarily. For example, start at `x=0`, `x1=x2=0`, `a=1,b=1`: after left we are at -1 (outside), so condition `a+b==0` passes but the robot leaves. So a correct solution must check the actual intermediate positions. The correct approach: compute `minX`, `maxX`, `minY`, `maxY` as described, and check if `x1 <= minX && maxX <= x2 && y1 <= minY && maxY <= y2`. This handles all cases including degenerate rectangles, because if `x1==x2`, then `minX==maxX` must hold and equal `x1`. That forces `a==b==0` (unless `x` already equals `x1` and no horizontal move). The condition `(x2 > x1 || a + b == 0)` in the original code is actually a simplified heuristic that fails for cases like `a=1,b=1` with zero width. Our solution must be correct. Time complexity is O(1), space O(1).
#include <algorithm>
#include <cstdint>

using ll = long long;

bool canNavigate(ll x, ll y, ll a, ll b, ll c, ll d,
                 ll x1, ll y1, ll x2, ll y2) {
    // Final coordinates after all moves
    ll final_x = x + b - a;
    ll final_y = y + d - c;

    // Intermediate extremes for x: start, after left moves, after all horizontal moves
    ll min_x = std::min({x, x - a, x - a + b});
    ll max_x = std::max({x, x - a, x - a + b});

    // Intermediate extremes for y: start, after down moves, after all vertical moves
    ll min_y = std::min({y, y - c, y - c + d});
    ll max_y = std::max({y, y - c, y - c + d});

    // All intermediate positions must lie inside the rectangle
    bool inside_x = (min_x >= x1 && max_x <= x2);
    bool inside_y = (min_y >= y1 && max_y <= y2);

    return inside_x && inside_y;
}
#include <cassert>

int main() {
    // Basic moves inside a large rectangle
    assert(canNavigate(0, 0, 1, 1, 1, 1, -100, -100, 100, 100) == true);
    // Move left then right, stays inside wide rectangle
    assert(canNavigate(5, 5, 2, 2, 0, 0, 0, 0, 10, 10) == true);
    // Degenerate width rectangle, left then right would temporarily leave
    assert(canNavigate(0, 0, 1, 1, 0, 0, 0, -10, 0, 10) == false);
    // Degenerate width, no horizontal moves allowed
    assert(canNavigate(0, 0, 0, 0, 1, 1, 0, -10, 0, 10) == true);
    // Degenerate height, vertical moves cancel but temporarily leave
    assert(canNavigate(0, 0, 0, 0, 1, 1, -10, 0, 10, 0) == false);
    // Degenerate height, no vertical moves
    assert(canNavigate(5, 0, 2, 2, 0, 0, 0, 0, 10, 0) == true);
    // Move outside horizontally
    assert(canNavigate(5, 5, 6, 0, 0, 0, 0, 0, 10, 10) == false);
    // Move outside vertically
    assert(canNavigate(5, 5, 0, 0, 6, 0, 0, 0, 10, 10) == false);
    // Negative coordinates and large values
    assert(canNavigate(-100, -100, 50, 150, 50, 150, -200, -200, 200, 200) == true);
    // Zero-move case
    assert(canNavigate(3, 4, 0, 0, 0, 0, 0, 0, 10, 10) == false); // start outside
    return 0;
}
