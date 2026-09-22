Write a C++ function that simulates unlocking a 4-digit circular combination lock. The lock starts at "0000" and each of the four wheels can be rotated forward or backward by one position at a time (e.g., from '0' to '9' or '1', and from '9' to '0' or '8'). Given a vector of dead-end combinations (strings of exactly 4 digits) that must never be visited, and a target combination (also a 4-digit string), return the minimum number of total wheel rotations needed to reach the target, or -1 if it is unreachable. If the starting combination "0000" is a dead-end, return -1 immediately. The function should handle any valid inputs without assuming the dead-ends list is sorted or unique.
This problem is a classic shortest-path search on a state space of 10^4 possible lock positions. Each state (a 4-digit string) is a node, and edges exist between states that differ by exactly one wheel rotation (increment or decrement modulo 10). Since all edges have unit weight, breadth-first search (BFS) guarantees the minimum number of rotations when the target is first reached. We maintain a hash set of dead-ends for O(1) lookup and also mark visited states by inserting them into the same set once enqueued, preventing revisits and cycles. The BFS starts from "0000" with level 0; for each state popped, we generate its 8 neighbors (4 wheels × 2 directions). If a neighbor is not a dead-end and not visited, we enqueue it. The process terminates when the target is found (return current level) or the queue empties (return -1). Edge cases include: start being a dead-end (immediate -1), target equal to start (return 0), or target unreachable due to dead-ends blocking all paths. Time complexity is O(10^4) = O(1) in the worst case since there are only 10,000 possible states, but generally O(10^4) states each with 8 neighbors, giving O(10^4) time and O(10^4) space for the queue and dead set.
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

// Returns the minimum number of rotations to reach target from "0000",
// avoiding all deadends. Returns -1 if unreachable.
int openLock(const std::vector<std::string>& deadends, const std::string& target) {
    std::unordered_set<std::string> dead(deadends.begin(), deadends.end());
    const std::string start = "0000";

    if (dead.count(start)) {
        return -1;
    }
    if (start == target) {
        return 0;
    }

    std::queue<std::string> que;
    que.push(start);
    dead.insert(start); // mark as visited

    int level = 0;
    while (!que.empty()) {
        int size = que.size();
        while (size-- > 0) {
            std::string curr = que.front();
            que.pop();

            // Generate all 8 neighbors (4 wheels × 2 directions)
            for (int i = 0; i < 4; ++i) {
                char original = curr[i];

                // Decrement the wheel (wrap around)
                curr[i] = (original == '0') ? '9' : original - 1;
                if (curr == target) {
                    return level + 1;
                }
                if (!dead.count(curr)) {
                    dead.insert(curr);
                    que.push(curr);
                }

                // Increment the wheel (wrap around)
                curr[i] = (original == '9') ? '0' : original + 1;
                if (curr == target) {
                    return level + 1;
                }
                if (!dead.count(curr)) {
                    dead.insert(curr);
                    que.push(curr);
                }

                curr[i] = original; // restore
            }
        }
        ++level;
    }
    return -1;
}
#include <cassert>

int main() {
    // Basic reachable case
    assert(openLock({"0201", "0101", "0102", "1212", "2002"}, "0202") == 6);
    // Unreachable because start is blocked
    assert(openLock({"0000"}, "8888") == -1);
    // Target equals start
    assert(openLock({}, "0000") == 0);
    // Simple single step
    assert(openLock({}, "0001") == 1);
    // Wraparound decrement
    assert(openLock({}, "1000") == 1);
    // Wraparound increment from 9 to 0
    assert(openLock({}, "9999") == 4);
    // Blocked path leading to -1
    assert(openLock({"0001", "0010", "0100", "1000"}, "1111") == -1);
    // Multi-step with deadends that don't block
    assert(openLock({"8888"}, "0009") == 1);
    // Larger path
    assert(openLock({}, "1234") == 10); // each wheel needs at most 5 steps, sum = 1+2+3+4 = 10
    // Another reachable case
    assert(openLock({"1000", "9000", "0100", "0900"}, "0901") == 1);
    return 0;
}
