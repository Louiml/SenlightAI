Write a C++ function `int findCircleWinner(int n, int k)` that simulates the Josephus problem: there are `n` players labeled 1 through `n` arranged in a circle. Starting at player 1, you count `k` players clockwise (including the starting player as the first count), remove the `k`-th player, and continue from the next player with the same counting rule. The process repeats until only one player remains, and the function must return that winner's label. Guarantee `n >= 1` and `k >= 1`. The function must not modify any external state and must be safe to call multiple times.
#include <cassert>

int main() {
    // Basic cases.
    assert(findCircleWinner(1, 1) == 1);
    assert(findCircleWinner(2, 1) == 2);
    assert(findCircleWinner(2, 2) == 1);
    assert(findCircleWinner(3, 2) == 3);
    assert(findCircleWinner(4, 2) == 1);
    assert(findCircleWinner(5, 3) == 4);
    // Larger n, arbitrary k.
    assert(findCircleWinner(7, 3) == 4);
    assert(findCircleWinner(10, 4) == 5);
    assert(findCircleWinner(100, 7) == 50);
    // k can be larger than n.
    assert(findCircleWinner(5, 17) == 4);
    return 0;
}
#include <queue>

// Simulate the Josephus game and return the winner's label (1-based).
int findCircleWinner(int n, int k) {
    std::queue<int> circle;
    for (int i = 1; i <= n; ++i) {
        circle.push(i);
    }
    while (circle.size() > 1) {
        // Rotate the first k-1 players to the back.
        for (int i = 1; i < k; ++i) {
            int front = circle.front();
            circle.pop();
            circle.push(front);
        }
        // Eliminate the k-th player.
        circle.pop();
    }
    return circle.front();
}
// The classic simulation uses a queue to represent the circle. Push all labels 1..n into a queue. Repeatedly, until the queue size is 1, move the first `k-1` players to the back (simulating counting but not removing them), then pop the front (the `k`-th counted player) to eliminate them. The final remaining element is the winner. Edge cases: when `k == 1`, the loop `for(int i=1;i<k;i++)` never runs, so we just pop the front each iteration, correctly eliminating players in order 1,2,...,n-1, leaving n as winner. For `n == 1`, the while loop condition fails immediately and the function returns 1. Complexity: each of the `n-1` eliminations requires `k-1` rotations plus one pop, so worst-case time is `O(n*k)` using a `std::queue`. Space is `O(n)` for storing the queue. An alternative mathematical solution exists but the simulation is straightforward and matches the given snippet.
