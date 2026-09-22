Write a C++ function `bool canMeasureWater(int jug1Capacity, int jug2Capacity, int targetCapacity)` that determines whether it is possible to measure exactly `targetCapacity` units of water using two jugs with capacities `jug1Capacity` and `jug2Capacity`. You may perform any sequence of the following operations: fill a jug completely, empty a jug completely, or pour water from one jug into the other until the source jug is empty or the destination jug is full. The function should return `true` if the target amount can be achieved, and `false` otherwise. Assume all capacities and the target are non-negative integers. The solution must work for capacities up to 1,000,000 and handle edge cases such as zero capacities, target equal to zero, and target larger than the sum of both jugs.

#include <cassert>

int main() {
    // Basic cases
    assert(canMeasureWater(3, 5, 4) == true);
    assert(canMeasureWater(2, 6, 5) == false);
    assert(canMeasureWater(1, 2, 3) == true);

    // Zero target always true
    assert(canMeasureWater(3, 5, 0) == true);
    assert(canMeasureWater(0, 0, 0) == true);

    // Target larger than sum of capacities
    assert(canMeasureWater(3, 5, 9) == false);

    // One jug has zero capacity
    assert(canMeasureWater(0, 5, 5) == true);
    assert(canMeasureWater(0, 5, 3) == false);
    assert(canMeasureWater(7, 0, 7) == true);
    assert(canMeasureWater(7, 0, 4) == false);

    // Larger values
    assert(canMeasureWater(13, 11, 6) == true);
    assert(canMeasureWater(13, 11, 1) == true);
    assert(canMeasureWater(13, 11, 12) == true);
    assert(canMeasureWater(13, 11, 25) == false);

    // Same capacity jugs
    assert(canMeasureWater(4, 4, 8) == true);
    assert(canMeasureWater(4, 4, 4) == true);
    assert(canMeasureWater(4, 4, 2) == false);
}

#include <queue>
#include <vector>

// Determines whether exactly targetCapacity units can be measured
// using two jugs with capacities jug1Capacity and jug2Capacity.
// Operations allowed: fill, empty, pour between jugs.
bool canMeasureWater(int jug1Capacity, int jug2Capacity, int targetCapacity) {
    if (targetCapacity == 0) return true;
    const int total = jug1Capacity + jug2Capacity;
    if (total < targetCapacity) return false;
    if (total == 0) return false; // both capacities zero, target > 0
    if (jug1Capacity == 0) return jug2Capacity == targetCapacity;
    if (jug2Capacity == 0) return jug1Capacity == targetCapacity;

    const int steps[4] = {jug1Capacity, -jug1Capacity, jug2Capacity, -jug2Capacity};
    std::vector<bool> visited(total + 1, false);
    std::queue<int> q;

    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        const int current = q.front();
        q.pop();

        if (current == targetCapacity) return true;

        for (int i = 0; i < 4; ++i) {
            const int next = current + steps[i];
            if (next >= 0 && next <= total && !visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
    return false;
}

// The problem is equivalent to checking whether the target can be expressed as a linear combination of the two jug capacities, since each operation changes the total water amount by either +jug1, -jug1, +jug2, or -jug2 (though physically the state might be bounded). However, a more straightforward and robust approach is to model the problem as a graph traversal over all possible water levels in one jug (say jug1), where each state represents the amount in jug1, and the amount in jug2 is implicitly determined if we fix the total? Actually, a simpler state representation is to track the amount of water in jug1 only, because for any given amount in jug1, operations can be simulated. But the provided code uses a BFS over all possible total amounts from 0 to jug1+jug2, applying the four possible changes. This works because the set of reachable total water amounts is exactly the set of numbers that can be formed by adding/subtracting multiples of gcd(jug1, jug2) within the range [0, jug1+jug2], and the BFS explores all reachable totals. Edge cases: if targetCapacity == 0, return true (since we start with both jugs empty). If jug1+jug2 < target, return false because we can never have more water than the sum of capacities. If either jug capacity is 0, handle specially: the only reachable totals are 0 and the non-zero capacity (if it exists). The BFS ensures correctness for all non-negative inputs. Time complexity is O(jug1+jug2) since the visited array has that many entries, and each node is processed once. Space complexity is also O(jug1+jug2) for the visited array and the queue. This is acceptable given the constraints.
