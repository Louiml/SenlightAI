Write a C++ function `int cycleEndValue(int N, int P)` that computes the value `a[n] - a[n*N%P] + 1` as described in the snippet, where the sequence is defined by repeatedly applying the operation `n = (n * N) % P` starting from `n = (N * N) % P`. The function should simulate the process using an array of size `P` to track the first visit index for each residue. If `P` is 1, the only possible residue is `0`, and the result should be `1` (since the cycle has length 1). Assume `N` and `P` are positive integers, with `P` at most 100 (so a fixed-size array can be used). The function must return the length of the eventual cycle, including the first repeated element, as an integer.
// The snippet uses an array `a` initialized to zeros, which serves as a "visited" marker: `a[value]` stores the number of steps taken when that value was first reached. Starting from `n = N*N % P`, the loop computes `next = n*N % P` and checks if `next` has been visited. If not, it sets `a[next] = a[n] + 1` and moves to `next`. When a repeated value is encountered, the difference `a[n] - a[next] + 1` gives the cycle length, because `a[n]` is the step count of the current node, `a[next]` is the step count when that node was first seen, and the `+1` accounts for moving from `current` to `next`. Important edge cases: if `P=1`, then `N*N % P = 0` and `n*N % P = 0`, so the first `next` is 0, which has never been visited (since `a[0]` is 0, but we must treat 0 as "not visited" carefully). In the snippet, `while (!a[n*N%P])` would loop forever because `a[0]` remains 0. However, the intended logic is that `a[0]` is initially 0 but should be treated as unvisited; the cycle length is 1. Our implementation must handle this by explicitly checking if `P == 1` and returning 1. For `P>1`, we can use an array of int `dist` initialized to -1 (unvisited) to avoid ambiguity. The algorithm runs in O(P) time because the sequence must revisit a value within at most P steps (pigeonhole principle), and uses O(P) auxiliary space.
#include <vector>

// Compute the length of the cycle in the sequence n_{k+1} = (n_k * N) % P,
// starting with n_0 = (N * N) % P, stopping when a residue repeats.
int cycleEndValue(int N, int P) {
    if (P == 1) {
        return 1;  // only residue 0, cycle length 1
    }

    // dist[r] = step index when residue r was first visited, or -1 if never.
    std::vector<int> dist(P, -1);

    int n = (N * N) % P;
    int step = 0;
    dist[n] = step++;

    while (true) {
        int next = (n * N) % P;
        if (dist[next] != -1) {
            // Cycle detected: current n has step index dist[n],
            // next was first visited at dist[next].
            return dist[n] - dist[next] + 1;
        }
        dist[next] = step++;
        n = next;
    }
}
#include <cassert>

int main() {
    // edge cases
    assert(cycleEndValue(1, 1) == 1);
    assert(cycleEndValue(5, 1) == 1);

    // trivial: N multiple of P, starting residue 0, next 0 -> cycle length 1
    assert(cycleEndValue(4, 2) == 1);  // N*N%2=0, 0*4%2=0
    assert(cycleEndValue(10, 5) == 1); // 100%5=0

    // N=2, P=5: sequence: 4 -> 8%5=3 -> 6%5=1 -> 2 -> 4 (repeats after 4 steps)
    assert(cycleEndValue(2, 5) == 4);

    // N=3, P=7: sequence: 9%7=2 -> 6 -> 4 -> 5 -> 1 -> 3 -> 2 (repeats after 6 steps)
    assert(cycleEndValue(3, 7) == 6);

    // N=5, P=7: sequence: 25%7=4 -> 20%7=6 -> 30%7=2 -> 10%7=3 -> 15%7=1 -> 5 -> 4 (repeats after 6)
    assert(cycleEndValue(5, 7) == 6);

    // N=6, P=10: 36%10=6 -> 36%10=6 (immediate self-loop)
    assert(cycleEndValue(6, 10) == 1);

    // N=7, P=10: 49%10=9 -> 63%10=3 -> 21%10=1 -> 7%10=7 -> 49%10=9 (repeats after 4)
    assert(cycleEndValue(7, 10) == 4);

    // Larger random check: N=12, P=13: compute manually: 144%13=1 -> 12 -> 1 (cycle length 2)
    assert(cycleEndValue(12, 13) == 2);

    // N=2, P=3: 4%3=1 -> 2%3=2 -> 4%3=1 (repeats after 2 steps)
    assert(cycleEndValue(2, 3) == 2);
}
