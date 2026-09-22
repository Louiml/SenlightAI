Write a C++ function `canReachTarget` that takes a string `s` composed of characters `'F'` and `'T'`, and two integers `targetX` and `targetY`. The path starts at the origin `(0,0)` facing the positive x-direction. For each `'F'`, the robot moves one unit forward in its current direction; for each `'T'`, it turns 90 degrees to the right (clockwise) without moving. The function must return `true` if it is possible to choose, for each `'T'`, whether the robot turns right or left (instead of always turning right), so that after processing the entire string the robot ends exactly at `(targetX, targetY)`. The string length is at most 8000, and coordinates can be as large as 10^4 in absolute value. You may assume that the string always starts with at least one `'F'` and that there is at least one `'T'` between any two directional choices (i.e., the first segment is always along the positive x-axis). The function should be `bool canReachTarget(const std::string& s, int targetX, int targetY)`.
The key observation is that the movement can be separated into horizontal (x-direction) and vertical (y-direction) components. The first block of `'F'`s before the first `'T'` is forced along the positive x-axis and cannot be changed. After that, each subsequent block of `'F'`s corresponds to a move whose direction is determined by the parity of turns so far. When facing x-direction (even number of turns), moving can be positive or negative x depending on left/right choices; when facing y-direction (odd number of turns), moving can be positive or negative y. Therefore, we extract all block lengths. The first block goes to x directly. The even-indexed blocks (0-indexed after the initial) contribute to x, and odd-indexed blocks contribute to y. For each axis, the problem reduces to: given a set of block lengths, can we assign signs (+ or -) to each so that the sum equals a target? This is a classic subset-sum with signs, solvable by a DP over the cumulative sums. Because the total movement is at most 10^4*100 and coordinates bounded by 10^4, we shift indices by an offset to handle negative sums. We process each axis independently and return true only if both axes are achievable. Time complexity is O(N * S) where N is number of blocks and S is the range of possible sums (bounded by ~2*10^4). Space complexity O(S). Edge cases: a single block with no turns (all 'F's) must match targetX and targetY must be 0. Also, if there are no blocks on an axis, the target for that axis must be 0.
#include <string>
#include <vector>
#include <cstdlib>

// Returns true if robot can reach targetX, targetY by choosing left/right at each 'T'.
bool canReachTarget(const std::string& s, int targetX, int targetY) {
    // Extract block lengths of consecutive 'F's.
    std::vector<int> blocks;
    int cnt = 0;
    for (char c : s) {
        if (c == 'F') {
            ++cnt;
        } else {
            if (cnt > 0) {
                blocks.push_back(cnt);
                cnt = 0;
            }
        }
    }
    if (cnt > 0) blocks.push_back(cnt);

    // If no 'T' at all, only one block.
    if (blocks.empty()) {
        return targetX == 0 && targetY == 0;
    }

    // First block is fixed along +x.
    int initialX = blocks[0];

    // Collect lengths for horizontal moves (even indices after 0) and vertical moves (odd indices).
    std::vector<int> xBlocks, yBlocks;
    for (size_t i = 1; i < blocks.size(); ++i) {
        if (i % 2 == 0) {
            xBlocks.push_back(blocks[i]);
        } else {
            yBlocks.push_back(blocks[i]);
        }
    }

    // Helper lambda to check if a set of lengths can sum to target with +/- signs.
    auto canSum = [](const std::vector<int>& lengths, int target, int offset) {
        if (lengths.empty()) {
            return target == 0;
        }
        // Sum of all lengths, used to bound DP range.
        int total = 0;
        for (int len : lengths) total += std::abs(len);
        // dp[i] is true if sum (i - offset) is achievable after processing some lengths.
        // Use offset = total to make range [0, 2*total].
        int shift = total;
        std::vector<bool> dp(2*total + 1, false);
        dp[shift] = true; // sum 0 achievable initially.
        for (int len : lengths) {
            std::vector<bool> next(2*total + 1, false);
            for (int s = 0; s <= 2*total; ++s) {
                if (!dp[s]) continue;
                int current = s - shift;
                if (current + len >= -total && current + len <= total) {
                    next[current + len + shift] = true;
                }
                if (current - len >= -total && current - len <= total) {
                    next[current - len + shift] = true;
                }
            }
            dp.swap(next);
        }
        return (target >= -total && target <= total) && dp[target + shift];
    };

    // Horizontal target: initialX + sum of signed xBlocks.
    bool xOk = canSum(xBlocks, targetX - initialX, 0);
    bool yOk = canSum(yBlocks, targetY, 0);
    return xOk && yOk;
}
#include <cassert>
#include <string>

// Declaration of the function (should match the solution)
bool canReachTarget(const std::string& s, int targetX, int targetY);

int main() {
    // All F's: only forward along x.
    assert(canReachTarget("FFFF", 4, 0) == true);
    assert(canReachTarget("FFFF", 3, 0) == false);
    assert(canReachTarget("FFFF", 0, 0) == false);

    // One T: allows vertical choice.
    assert(canReachTarget("FTF", 1, 1) == true);  // F then T -> up or down? Actually T right initially: F(1,0), T turns right? But we assume can choose left, so up? Let's see: after first F at (1,0), we can turn left (up) or right (down). Then F to (1,1) or (1,-1). So (1,1) reachable.
    assert(canReachTarget("FTF", 1, -1) == true);
    assert(canReachTarget("FTF", 0, 1) == false);
    assert(canReachTarget("FTF", 2, 0) == false);

    // Multiple turns.
    assert(canReachTarget("FFTFT", 2, -1) == true); // F F (2,0), T, F (2,±1), T, F (2±1,±1) -> (1,1) or (3,1) etc. Need to check (2,-1) possible? Let's see: after FF at (2,0), choose T left? Actually after first T, direction is up or down. Then F to (2,±1). Then second T: can turn left or right from current vertical direction. If up and turn left -> left, then F -> (1,1). If up and turn right -> right, F -> (3,1). Similarly down. So (2,-1) not reachable. So should be false.
    assert(canReachTarget("FFTFT", 2, -1) == false);
    assert(canReachTarget("FFTFT", 1, 1) == true);
    assert(canReachTarget("FFTFT", 3, -1) == true);

    // Empty blocks on an axis.
    assert(canReachTarget("FT", 1, 0) == true); // No vertical moves, target y=0.
    assert(canReachTarget("FT", 1, 1) == false);
    assert(canReachTarget("FTTFF", 3, 0) == true); // First F (1,0), then two T's: direction becomes negative x? Actually after two turns, facing left. Then FF => (1-2= -1,0) or (3,0) depending on turns? Let's see: Starting (1,0) facing +x. T -> turn right/left? We choose left: facing +y. T again: from +y turn left -> +x? Actually turn left from +y gives -x? Wait, standard: turning left from +y gives -x, turning right from +y gives +x. So two turns can yield either +x or -x. So after two T's, facing either +x or -x. Then FF gives (1±2) = -1 or 3. So (3,0) reachable. So true.
    assert(canReachTarget("FTTFF", 3, 0) == true);

    // Larger example.
    assert(canReachTarget("FFTFTFTFF", 3, -2) == true);

    return 0;
}
