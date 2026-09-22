// Write a C++ function `josephusPermutation(int n, int k)` that simulates the Josephus problem and returns a `std::vector<int>` containing the order in which people are eliminated. There are `n` people numbered from 1 to `n` arranged in a circle. Starting at person 1, you repeatedly count `k` people (including the starting person), remove that person from the circle, and continue counting from the next person. Return the elimination order as a vector. For example, `josephusPermutation(7, 3)` returns `{3, 6, 2, 7, 5, 1, 4}`. Assume `n ≥ 1` and `k ≥ 1`. The function must not use external libraries beyond standard headers and must be const-correct where applicable.
// The standard approach is to use an array or boolean vector to mark whether a person is still alive. Initialize a vector `alive` of size `n` with all `true`, and an index `idx = 0` representing the current position (0-based). For each of the `n` rounds, we need to advance `k` steps among alive people. Since counting starts at the current position, we loop until we have counted `k` alive people: while the counted counter is less than `k`, if the current index is at the end, wrap around to 0; if the person at `idx` is alive, increment the counter; then increment `idx` (modulo n). After the loop, `idx` will point to the person just after the one to eliminate, so we store the eliminated person as `idx-1` (handling wrap-around if `idx == 0`, then it's `n-1`). Mark that person as dead, push its number (index+1) into the result, and continue. Edge cases: when `n == 1`, the function returns `{1}` regardless of `k` because the only person is eliminated after counting `k` times but since there are no others, it works correctly with the loop. If `k` is large, the while loop will still correctly skip dead people because it only increments the counter on alive ones. Time complexity: Each round scans at most `k` alive people, and there are `n` rounds, so worst-case O(n*k). Space complexity: O(n) for the alive array and the result. An alternative is to use a linked list for O(n) time, but the simple array method is sufficient and clearer for this task.
#include <vector>

// Simulates the Josephus problem and returns the elimination order.
// n: number of people (>=1), k: step count (>=1)
std::vector<int> josephusPermutation(int n, int k) {
    std::vector<bool> alive(n, true);
    std::vector<int> result;
    result.reserve(n);
    int idx = 0;  // current position (0-based)
    for (int round = 0; round < n; ++round) {
        int counted = 0;
        while (counted < k) {
            if (idx == n) idx = 0;  // wrap around
            if (alive[idx]) counted++;
            idx++;
        }
        // After the loop, idx points to the next position after the eliminated one
        int eliminated = (idx == 0) ? n - 1 : idx - 1;
        result.push_back(eliminated + 1);  // convert to 1-based
        alive[eliminated] = false;
    }
    return result;
}
#include <cassert>
#include <vector>

// Include the solution function here (e.g., by copy-pasting or including the header)

int main() {
    assert(josephusPermutation(1, 1) == std::vector<int>({1}));
    assert(josephusPermutation(2, 1) == std::vector<int>({1, 2}));
    assert(josephusPermutation(5, 2) == std::vector<int>({2, 4, 1, 5, 3}));
    assert(josephusPermutation(7, 3) == std::vector<int>({3, 6, 2, 7, 5, 1, 4}));
    assert(josephusPermutation(4, 5) == std::vector<int>({1, 3, 2, 4})); // large k
    assert(josephusPermutation(3, 1) == std::vector<int>({1, 2, 3}));
    return 0;
}
