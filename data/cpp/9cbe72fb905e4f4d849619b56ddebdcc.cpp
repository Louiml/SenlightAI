// Write a C++ function that simulates a random walk of `m` distinct balls placed in `n` circular bowls, where each bowl is labeled from `0` to `n-1`. The function should take as input the number of bowls `n`, the number of balls `m`, and a positive integer `steps` representing how many times to repeat the following operation: uniformly choose a random ball and move it to an adjacent bowl, with equal probability of moving clockwise (index `+1 mod n`) or anticlockwise (index `-1 mod n`). The function should return a `std::vector<int>` of size `n` representing the final number of balls in each bowl after all steps. The initial placement must also be random: each ball starts in a uniformly random bowl. Note that the operation moves exactly one ball per step; if `steps` is zero, the function returns the initial bowl counts. Assume `n >= 1`, `m >= 0`, and `steps >= 0`. Use a fixed random seed (e.g., `std::mt19937` seeded with a constant) inside the function to make the behavior deterministic for testing purposes.

The solution requires three main phases: (1) initialize random ball positions, (2) build the initial bowl occupancy counts, and (3) perform the specified number of random ball moves while updating both the ball locations and bowl counts. The key data structures are a `std::vector<int> balls` of size `m` storing each ball’s current bowl index, and a `std::vector<int> bowls` of size `n` storing the count of balls in each bowl. Initialization uses a uniform distribution over `0` to `n-1` to assign each ball, incrementing the corresponding bowl count. For each simulation step, we choose a random ball index uniformly from `0` to `m-1`, decrement its current bowl’s count, then choose a random direction (0 or 1). For clockwise direction, the new bowl index is `(current + 1) % n`; for anticlockwise, we must handle negative modulo carefully—using `(current - 1 + n) % n` ensures a valid non-negative index. After moving, increment the new bowl’s count and update the ball’s position. Edge cases: if `m == 0`, the `balls` vector is empty and no moves can be made, so return all zeros; if `steps == 0`, return initial counts. The time complexity is `O(m + steps)`, since initialization is `O(m)` and each step is `O(1)`. Space complexity is `O(m + n)` for storage. The use of a fixed seed ensures reproducibility for tests.

#include <vector>
#include <random>
#include <cstddef>

// Simulate random ball movement and return final bowl occupancy counts.
// n: number of bowls (>=1), m: number of balls (>=0), steps: number of moves (>=0).
std::vector<int> simulate_ball_walk(int n, int m, int steps) {
    // Fixed seed for deterministic behavior.
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> bowl_dist(0, n - 1);
    std::uniform_int_distribution<int> ball_dist(0, m - 1);
    std::uniform_int_distribution<int> dir_dist(0, 1);

    // Initialize balls and bowl counts.
    std::vector<int> balls(m);
    std::vector<int> bowls(n, 0);
    for (int i = 0; i < m; ++i) {
        int bowl = bowl_dist(gen);
        balls[i] = bowl;
        bowls[bowl] += 1;
    }

    // Perform the specified number of moves.
    for (int step = 0; step < steps; ++step) {
        if (m == 0) break; // No balls to move.
        int ball = ball_dist(gen);
        int current = balls[ball];
        bowls[current] -= 1;

        int direction = dir_dist(gen);
        int new_bowl;
        if (direction == 1) {
            // Clockwise.
            new_bowl = (current + 1) % n;
        } else {
            // Anticlockwise, avoid negative modulo.
            new_bowl = (current - 1 + n) % n;
        }

        bowls[new_bowl] += 1;
        balls[ball] = new_bowl;
    }

    return bowls;
}

#include <cassert>
#include <vector>
#include <numeric>

int main() {
    // Test 1: Zero steps returns initial random counts (should sum to m).
    std::vector<int> r1 = simulate_ball_walk(5, 10, 0);
    assert(r1.size() == 5);
    int sum1 = 0;
    for (int c : r1) sum1 += c;
    assert(sum1 == 10);

    // Test 2: Single bowl, any balls, any steps -> all balls in bowl 0.
    std::vector<int> r2 = simulate_ball_walk(1, 7, 100);
    assert(r2 == std::vector<int>{7});

    // Test 3: No balls, any bowls/steps -> all zeros.
    std::vector<int> r3 = simulate_ball_walk(4, 0, 50);
    assert(r3 == std::vector<int>(4, 0));

    // Test 4: Deterministic with seed: two calls produce identical results.
    std::vector<int> a = simulate_ball_walk(3, 5, 20);
    std::vector<int> b = simulate_ball_walk(3, 5, 20);
    assert(a == b);

    // Test 5: Sum of counts always equals m after any steps.
    std::vector<int> r5 = simulate_ball_walk(6, 3, 17);
    int sum5 = std::accumulate(r5.begin(), r5.end(), 0);
    assert(sum5 == 3);

    // Test 6: For n=2, alignment of ball moves: final counts sum to m.
    std::vector<int> r6 = simulate_ball_walk(2, 4, 1);
    int sum6 = std::accumulate(r6.begin(), r6.end(), 0);
    assert(sum6 == 4);
}
