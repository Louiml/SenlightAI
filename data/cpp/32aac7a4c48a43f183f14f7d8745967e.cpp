// Write a C++ function `int minimumMovesToEqualize(int n, int m)` that simulates the following process. You have three numbers, initially all equal to `m`. In each move, you pick one of the three numbers in a cyclic order (starting with the first, then second, then third, then first, etc.). For the currently picked number, you replace it with the sum of the other two numbers minus 1, but if that sum exceeds `n`, you set the picked number to exactly `n` instead. The process stops when all three numbers have become exactly `n`. The function should return the total number of moves required. It is guaranteed that the process terminates for the given inputs. Assume `m` and `n` are positive integers with `1 ≤ m ≤ n ≤ 10^9`.
#include <cassert>

int main() {
    // Basic cases
    assert(minimumMovesToEqualize(1, 1) == 0);          // Already all equal
    assert(minimumMovesToEqualize(5, 5) == 0);          // Already all equal
    assert(minimumMovesToEqualize(5, 4) == 3);          // 4 4 4 -> 4 4 5 -> 4 5 5 -> 5 5 5
    assert(minimumMovesToEqualize(10, 1) == 7);         // Fibonacci-like growth
    assert(minimumMovesToEqualize(100, 1) == 10);       // Larger n
    assert(minimumMovesToEqualize(1000000000, 1) == 45); // Very large n
    // Asymmetric case: start with m close to n
    assert(minimumMovesToEqualize(10, 9) == 3);
    // Ensure no overflow in sum calculation for large n
    assert(minimumMovesToEqualize(1000000000, 1000000000) == 0);
    assert(minimumMovesToEqualize(1000000000, 500000000) == 3);
    // Additional check with n=2, m=1
    assert(minimumMovesToEqualize(2, 1) == 2);
    return 0;
}
#include <vector>
#include <algorithm>

// Simulate the cyclic update process and return the number of moves.
int minimumMovesToEqualize(int n, int m) {
    std::vector<int> values(3, m);
    int moves = 0;
    int index = 0;
    
    while (values[0] != n || values[1] != n || values[2] != n) {
        int otherA = (index + 1) % 3;
        int otherB = (index + 2) % 3;
        long long sumOthers = static_cast<long long>(values[otherA]) + values[otherB];
        long long candidate = sumOthers - 1;
        if (candidate >= n) {
            values[index] = n;
        } else {
            values[index] = static_cast<int>(candidate);
        }
        ++moves;
        index = (index + 1) % 3;
    }
    
    return moves;
}
// The key observation is that the process is deterministic and the order of picking is fixed (cyclic). The naive simulation could be very slow if `n` is large and `m` is small because each move only increases one value, but the increase can be large. However, direct simulation is still feasible for practical constraints if we bound the number of moves. Why? Because each move sets the current value to at least `m+1` (since the sum of the other two minus 1 is at least `m+m-1 = 2m-1 > m`), so values grow quickly. In fact, the number of moves is at most `O(log n)` when `m` is small, because values roughly double. But even if `m` is close to `n`, the process terminates in at most a few moves. So a straightforward simulation works. The algorithm: keep an array of three integers, initialize to `m`. Maintain an index `i=0` representing which of the three numbers is to be updated next. While not all equal to `n`, compute the sum of the other two. If that sum minus 1 is ≤ `n`, set the current to that; otherwise set to `n`. Increment move counter, then advance `i` cyclically. Edge cases: (1) If `m == n`, the loop never runs and return 0. (2) If the other two are already `n` and the third is not, then the sum minus 1 is `n+n-1 > n`, so it just sets to `n` in one move. (3) The process is guaranteed to terminate because each move strictly increases at least one value, and values are bounded above by `n`. Time complexity is `O(K)` where `K` is the number of moves; in the worst case, when `m=1` and `n` is large, the values grow like the Fibonacci sequence, so `K` is `O(log n)` (actually around `O(log n)` base φ). Space complexity is `O(1)`.
