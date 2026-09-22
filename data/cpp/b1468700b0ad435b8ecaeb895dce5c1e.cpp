Given a line of evenly spaced robots moving along a straight line, where each robot is labeled either `R` (moving to the right) or `L` (moving to the left) and has a distinct integer position, write a C++ function `int earliestCollisionTime(const std::vector<int>& positions, const std::string& directions)` that returns the earliest integer time (in seconds) at which any two adjacent robots collide head‑on, or `-1` if no collision can ever occur. Robots move at one unit per second, and collisions happen only when a robot moving right is immediately followed (in position) by a robot moving left; the collision time is the integer floor of half the distance between them. The input arrays are the same length, positions are strictly increasing and already sorted, and directions is a string of `'R'` and `'L'`. If there are multiple possible collisions, return the smallest time. Also, if the minimum computed time is not an integer (i.e., the distance is odd), still return the integer floor (since the problem assumes positions are integers and the collision happens at a half‑second boundary, but the answer is the floor of that half‑distance in seconds as per the original snippet’s logic). The positions and directions correspond element‑wise: the i‑th robot has position `positions[i]` and direction `directions[i]`.

The key observation is that a collision can only happen between two consecutive robots in the sorted position order when the left robot is moving right (`'R'`) and the right robot is moving left (`'L'`). Since all robots move at the same speed, the collision time for such a pair is exactly half the distance between them, i.e., `(positions[i] - positions[i-1]) / 2` (integer division, which is the floor of the real half‑distance). We iterate through all adjacent pairs from index 1 to n‑1, check the direction condition, and compute the candidate collision time. We take the minimum over all valid candidates. If no valid pair exists, the answer is `-1`. Edge cases: (1) an empty or single‑element vector should return `-1` because no adjacent pair exists; (2) the direction string may have mixed cases, but we assume only `'R'` and `'L'`; (3) positions are guaranteed sorted and distinct, so no sorting is needed; (4) the division is integer division, which automatically floors the result, matching the snippet’s behavior. Time complexity is `O(n)` and space complexity is `O(1)` beyond input storage.

#include <string>
#include <vector>
#include <algorithm>
#include <climits>

// Returns the earliest integer collision time between any two adjacent robots,
// or -1 if no such collision occurs. Positions are sorted increasing.
int earliestCollisionTime(const std::vector<int>& positions, const std::string& directions) {
    const int n = static_cast<int>(positions.size());
    if (n < 2) {
        return -1;
    }
    
    int minTime = INT_MAX;
    for (int i = 1; i < n; ++i) {
        // Check if the left robot moves right and the right robot moves left.
        if (directions[i - 1] == 'R' && directions[i] == 'L') {
            int distance = positions[i] - positions[i - 1];
            int candidate = distance / 2; // integer floor of half‑distance
            minTime = std::min(minTime, candidate);
        }
    }
    
    return (minTime == INT_MAX) ? -1 : minTime;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Example from snippet: n=4, s="RLRL", positions=[1,5,6,7] => only pair (pos1=1,R) and (pos2=5,L) gives (5-1)/2=2
    assert(earliestCollisionTime({1,5,6,7}, "RLRL") == 2);
    
    // No collision: all moving right
    assert(earliestCollisionTime({1,2,3,4}, "RRRR") == -1);
    
    // No collision: all moving left
    assert(earliestCollisionTime({1,2,3,4}, "LLLL") == -1);
    
    // Single robot: no adjacent pair
    assert(earliestCollisionTime({5}, "R") == -1);
    
    // Empty input
    assert(earliestCollisionTime({}, "") == -1);
    
    // Multiple collisions, pick smallest time
    // Positions: 0(R), 2(L) -> time=1; 3(R), 5(L) -> time=1; earliest=1
    assert(earliestCollisionTime({0,2,3,5}, "RLRL") == 1);
    
    // Odd distance: floor half works, e.g., distance=3 => floor=1
    assert(earliestCollisionTime({0,3}, "RL") == 1);
    
    // Even distance: exact half
    assert(earliestCollisionTime({0,4}, "RL") == 2);
    
    // No collision because pattern is LR (moving away from each other)
    assert(earliestCollisionTime({0,4}, "LR") == -1);
    
    // Mixed pattern: first pair qualifies, second pair does not
    assert(earliestCollisionTime({0,2,10}, "RRL") == 1); // pair (0,R)-(2,R) no; pair (2,R)-(10,L) => (10-2)/2=4, but pair (0,R)-(2,R) no, so answer=4? Actually check: positions 0(R),2(R),10(L) => only (2,R)-(10,L) qualifies => (10-2)/2=4
    assert(earliestCollisionTime({0,2,10}, "RRL") == 4);
    
    return 0;
}
