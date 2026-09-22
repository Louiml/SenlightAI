Write a C++ function `int josephusElimination(int n, int k)` that simulates the classic Josephus problem using a queue. Given `n` people numbered `1` through `n` arranged in a circle, the elimination process starts at person 1 and counts `k` people around the circle, removing the `k`-th person each time. Counting resumes from the next person after the removed one. The function should return the number of the last remaining person. Assume `n >= 1` and `k >= 1`. The function must use a queue (e.g., `std::queue<int>`) to simulate the process, not mathematical shortcuts.

#include <cassert>

int main() {
    // Basic cases
    assert(josephusElimination(5, 2) == 3); // Classic: people 1..5, every 2nd eliminated -> survivor 3
    assert(josephusElimination(1, 5) == 1); // Single person survives
    assert(josephusElimination(7, 1) == 7); // k=1 eliminates sequentially, last remains
    assert(josephusElimination(10, 3) == 4); // Known result for n=10, k=3
    assert(josephusElimination(6, 2) == 5); // Even n with k=2 -> survivor is odd pattern
    // Larger n sanity check: survivor must be in range [1, n]
    for (int n = 1; n <= 20; ++n) {
        for (int k = 1; k <= 5; ++k) {
            int survivor = josephusElimination(n, k);
            assert(survivor >= 1 && survivor <= n);
        }
    }
    return 0;
}

#include <queue>

// Simulate the Josephus elimination process using a queue.
// Returns the number of the last remaining person.
int josephusElimination(int n, int k) {
    std::queue<int> people;
    for (int i = 1; i <= n; ++i) {
        people.push(i);
    }

    while (people.size() > 1) {
        // Rotate the queue so the k-th person is at the front.
        for (int step = 1; step < k; ++step) {
            int frontPerson = people.front();
            people.pop();
            people.push(frontPerson);
        }
        // Eliminate the k-th person.
        people.pop();
    }
    return people.front();
}

// The solution uses a queue to model the circular arrangement. Initially, all numbers `1` through `n` are pushed into the queue in order. The simulation continues while more than one person remains. In each round, we rotate the queue `k-1` times: for each of those rotations, we move the front element to the back by pushing it and popping it, so that the `k`-th person ends up at the front. Then we pop the front element to eliminate that person. After the loop exits, the only remaining element in the queue is the survivor, which is returned. Important edge cases: if `k == 1`, the loop simply pops the front each time, eliminating people in order `1, 2, 3, ...`, leaving `n` as the survivor; if `n == 1`, the queue already contains one person, so the while loop never runs and returns `1`. The time complexity is `O(n * k)` in the worst case because for each of `n-1` eliminations we perform `k-1` queue operations, and each queue operation is `O(1)`. Space complexity is `O(n)` for the queue storing all `n` people.
