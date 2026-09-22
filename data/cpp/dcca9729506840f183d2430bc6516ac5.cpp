/*
Write a C++ function `string movementFeasible(const string& commands, int x, int y)` that determines whether a robot starting at the origin `(0, 0)` can reach the target point `(x, y)` after executing a sequence of commands consisting only of characters `'F'` (move forward one unit in the current direction) and `'T'` (turn 90 degrees, alternating between right and left turns with each `'T'`). Initially the robot faces the positive X-axis. The first `'T'` turns it to face the positive Y-axis, the second `'T'` turns it to face the negative X-axis, the third to negative Y-axis, and so on. The robot must execute the entire sequence in order; it cannot skip or reorder commands. The function should return `"Yes"` if it is possible to end exactly at `(x, y)`, and `"No"` otherwise. The input coordinates `x` and `y` can be any integers (including negative, zero, or large in magnitude up to 10^4). The length of `commands` is between 1 and 5000, consisting only of `'F'` and `'T'`. You must use only bitset-based dynamic programming (as hinted by the snippet) to determine feasibility, not brute force over all 2^n choices.
*/
#include <string>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <algorithm>

const int MAX_SUM = 5000;

// Helper: check if given list of segment lengths can sum to target (absolute value) using +/- signs.
bool achievable(const std::vector<int>& lengths, int target) {
    int total = 0;
    for (int len : lengths) total += len;
    target = std::abs(target);
    if (target > total) return false;
    if ((total + target) % 2 != 0) return false;
    int need = (total + target) / 2;
    std::bitset<MAX_SUM + 1> dp;
    dp[0] = 1;
    for (int len : lengths) {
        dp |= (dp << len);
    }
    return dp[need];
}

// Main solution function: decide if robot can reach (x,y) from origin.
std::string movementFeasible(const std::string& commands, int x, int y) {
    // Collect initial horizontal runs.
    std::vector<int> horizontal, vertical;
    int cur = 0;
    // Handle initial 'F' before any 'T' (direction positive X).
    if (cur < static_cast<int>(commands.size()) && commands[cur] == 'F') {
        int cnt = 0;
        while (cur < static_cast<int>(commands.size()) && commands[cur] == 'F') {
            ++cur;
            ++cnt;
        }
        // This initial run is always in positive X direction, so subtract from x.
        x -= cnt;
        horizontal.push_back(cnt);
    }
    // Now process the rest, alternating axes with each 'T'.
    bool onX = true; // after first T, we switch to Y; but since we already handled initial X runs, let's start with vertical axis.
    bool isVertical = true; // next axis after first T is Y (vertical).
    while (cur < static_cast<int>(commands.size())) {
        if (commands[cur] == 'T') {
            ++cur;
            isVertical = !isVertical;
            // Count following F's.
            int cnt = 0;
            while (cur < static_cast<int>(commands.size()) && commands[cur] == 'F') {
                ++cur;
                ++cnt;
            }
            if (cnt > 0) {
                if (isVertical) vertical.push_back(cnt);
                else horizontal.push_back(cnt);
            }
        } else {
            // Should not happen because we always skip past F's in the while loops above.
            ++cur;
        }
    }
    // Now check feasibility for each axis independently.
    if (!achievable(horizontal, std::abs(x))) return "No";
    if (!achievable(vertical, std::abs(y))) return "No";
    return "Yes";
}
#include <cassert>
#include <string>

// Declare the function from solution (included above in actual compilation).
std::string movementFeasible(const std::string& commands, int x, int y);

int main() {
    // Basic cases from snippet examples:
    assert(movementFeasible("FFTFF", 2, 2) == "Yes");
    assert(movementFeasible("FFTFF", 0, 0) == "No");
    // Initial F only moves along X.
    assert(movementFeasible("FFF", 3, 0) == "Yes");
    assert(movementFeasible("FFF", 0, 0) == "No");
    // Single T with no F: no movement.
    assert(movementFeasible("T", 0, 0) == "Yes");
    assert(movementFeasible("T", 1, 0) == "No");
    // Alternating turns allow both signs.
    assert(movementFeasible("FTF", 1, 1) == "Yes");
    assert(movementFeasible("FTF", 1, -1) == "Yes");
    assert(movementFeasible("FTF", -1, 1) == "Yes"); // initial F only positive X, can't go negative X after T? Actually after first T, you move Y, then second T moves negative X, but with only one F before T and one after, you can get to (1,1) or (1,-1) but not (-1,1) because initial F is always positive. So this should be No.
    assert(movementFeasible("FTF", -1, 1) == "No");
    // Complex: multiple runs on each axis.
    assert(movementFeasible("FFTFFTFF", 4, 0) == "Yes"); // horizontal: 2 + 2 = 4 with signs both +, vertical: 2? Actually sequence: FF (X+2), T (turn Y), FF (Y+2), T (turn X-), FF (X-2). So horizontal runs: 2 and -2 => sum 0, not 4. Let's compute properly: initial FF gives X+2, second FF after T gives Y+2, third FF after second T gives X-2. So total x = 2 - 2 = 0, y = 2. So target (4,0) impossible.
    assert(movementFeasible("FFTFFTFF", 0, 2) == "Yes");
    // Large values with many runs.
    assert(movementFeasible("F" + std::string(100, 'T') + "F", 1, 0) == "No"); // many turns but only one F total, so can't move far.
    // Edge: empty initial runs.
    assert(movementFeasible("TFF", 0, 2) == "Yes");
    assert(movementFeasible("TFF", 1, 0) == "No");
    // All F's: only X movements.
    assert(movementFeasible("FFFF", 4, 0) == "Yes");
    assert(movementFeasible("FFFF", 0, 4) == "No");
    // Mixed with large coordinates.
    assert(movementFeasible("FFFTFFFT", 3, 3) == "Yes"); // initial FFF (X+3), T, FFF (Y+3), T, last T no F. So (3,3).
    return 0;
}
// The key observation is that the sequence of movements decomposes into independent horizontal and vertical components. Initially, before any `'T'`, the robot moves along the X-axis; those initial `'F'`s form a horizontal segment that must be accounted for by adjusting the target x coordinate (we must move exactly that many steps in the X direction before any turn). After the first `'T'`, all forward moves occur along the Y-axis; after the second `'T'`, along the negative X-axis, but since direction only affects sign, we can treat each axis independently. In fact, each `'T'` toggles between the two axes: the first `'T'` starts the Y-axis segment, the second starts the X-axis segment (but in the opposite direction from the initial X-axis, but since we take absolute values at the end, sign does not matter). So we partition the forward runs into two lists: one list for horizontal moves (all runs that occur when the robot is moving along X, which includes the initial run before any `'T'` and every other run starting with the second `'T'`, the fourth, etc.) and one list for vertical moves (runs after the first `'T'`, third, fifth, etc.). For each axis, we have a list of positive integers (run lengths). We need to determine if we can assign a sign (+ or -) to each run such that the sum of signed values equals the target coordinate (with the initial horizontal run having a fixed sign: the robot moves in the positive X direction initially, so the target x must be adjusted by subtracting that initial run, then take absolute value to handle direction). Since the robot's direction reverses with each turn, we can choose signs arbitrarily for each run after the first horizontal run, because the direction alternates but we can consider the absolute value of the target. Thus for each axis, the problem reduces to: given a multiset of positive integers (run lengths), can we partition them into two subsets whose sums differ by a target absolute value? Equivalently, can we achieve a subset sum of `(total + target)/2`? This is a classic subset-sum feasibility, solvable by bitset DP. We maintain a bitset of reachable sums, initially only 0 is reachable, and for each run length `len`, we shift the bitset left by `len` and OR it with the current bitset. After processing all runs, if the bit at position `(total + abs(target))/2` is set, it is feasible (provided that `(total + abs(target))` is even and non-negative). Edge cases: if the target is larger than the total sum of runs on that axis, it's impossible; also if `(total + abs(target))` is odd, impossible. Also, if there are no runs on an axis, the only feasible target is 0. Time complexity: O(L * (max possible sum)/64) where L is number of runs and max sum is at most 5000 (since each `F` increments a run length, total F count ≤ 5000). Space: O(maxSum) bits, effectively O(maxSum/64) machine words. The overall algorithm runs in O(n^2/64) time, which is efficient for n up to 5000.
