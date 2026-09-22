You are given `N` positions in a line numbered from `1` to `N`, initially uncolored. There are `Q` update operations, each described by three integers `L`, `R`, and `C`. Each operation means: color every currently uncolored position in the inclusive range `[L, R]` with color `C`, but only if it has not been colored before (i.e., once a position is assigned a color, it can never be changed). The operations are given in chronological order (first to last), but you must process them in reverse order to determine the final color of each position. After all operations, output the color assigned to each position from `1` to `N`, one per line; if a position is never colored, output `0`. Write a C++ function that takes `N`, a vector of operations (each as a triple `(L, R, C)`), and returns a `std::vector<int>` of length `N+1` (index `0` unused) where index `i` holds the final color of position `i`. The function must handle cases where ranges overlap arbitrarily, and each operation can cover many positions.
// The key observation is that processing operations in reverse order guarantees that the first time a position is encountered, it receives its final color—because any later operation (in original order) would have overwritten it, but since we go backwards, the first assignment is the last operation that touched it. We maintain a disjoint-set union (DSU) where each set represents a contiguous block of already-colored positions. For each position, we store a `next` pointer to the next uncolored position (or `N+1` if none). Initially, every position is its own parent, and `next[i] = i`. When we process an operation `(L, R, C)` in reverse, we walk through all uncolored positions in `[L, R]` using the DSU `find` to skip already-colored ones. For each such position, we set its answer color to `C`, then union it with the next position, effectively removing it from the set of uncolored positions. To optimize, each position is only processed once, so the total work across all operations is `O(N α(N))` where `α` is the inverse Ackermann function. Edge cases: ranges may extend to `N`, and we need a sentinel position `N+1` to avoid out-of-bounds; positions never touched remain `0`. The space complexity is `O(N + Q)` for the DSU arrays and the answer vector.
#include <vector>
#include <numeric>

// Union-Find with path compression and size heuristic, but we use it to skip colored positions.
class NextDSU {
private:
    std::vector<int> parent;
    std::vector<int> size;

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path halving
            x = parent[x];
        }
        return x;
    }

public:
    explicit NextDSU(int n) : parent(n + 2), size(n + 2, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    // Return the smallest index >= x that is still uncolored (i.e., root of its set).
    int get_next(int x) {
        return find(x);
    }

    // Mark position x as colored by merging it with x+1. x must be <= N.
    void remove(int x) {
        int a = find(x);
        int b = find(x + 1);
        if (a == b) return;
        // We always attach the larger root under the smaller root so that the root becomes the next uncolored position.
        if (size[a] < size[b]) {
            parent[a] = b;
            size[b] += size[a];
        } else {
            parent[b] = a;
            size[a] += size[b];
        }
    }
};

// Given N and operations (L, R, C), return final color per position (index 1..N).
// Process operations in reverse order to assign each position exactly once.
std::vector<int> processRangeColorings(int N, const std::vector<std::vector<int>>& operations) {
    std::vector<int> answer(N + 1, 0);
    NextDSU dsu(N + 1); // sentinel N+1

    // Iterate from last operation to first
    for (int i = static_cast<int>(operations.size()) - 1; i >= 0; --i) {
        int L = operations[i][0];
        int R = operations[i][1];
        int C = operations[i][2];
        int cur = dsu.get_next(L);
        while (cur <= R) {
            answer[cur] = C;
            dsu.remove(cur);
            cur = dsu.get_next(L);
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

// The solution function is declared above (include the header or paste it here).

int main() {
    // Example 1: Simple disjoint ranges
    std::vector<std::vector<int>> ops1 = {{1, 2, 5}, {4, 5, 7}};
    std::vector<int> res1 = processRangeColorings(5, ops1);
    assert(res1[1] == 5 && res1[2] == 5 && res1[3] == 0 && res1[4] == 7 && res1[5] == 7);

    // Example 2: Overlapping ranges; later operation should win (but processed in reverse)
    std::vector<std::vector<int>> ops2 = {{1, 5, 1}, {2, 4, 2}, {3, 3, 3}};
    std::vector<int> res2 = processRangeColorings(5, ops2);
    // Reverse order: op3 (3,3,3) => pos3=3; op2 (2,4,2) => pos2 and pos4 get 2 (pos3 already colored); op1 (1,5,1) => pos1 and pos5 get 1
    assert(res2[1] == 1 && res2[2] == 2 && res2[3] == 3 && res2[4] == 2 && res2[5] == 1);

    // Example 3: All positions covered by first operation, later operations irrelevant
    std::vector<std::vector<int>> ops3 = {{1, 3, 9}, {2, 2, 0}}; // 0 is a valid color here
    std::vector<int> res3 = processRangeColorings(3, ops3);
    assert(res3[1] == 9 && res3[2] == 0 && res3[3] == 9);

    // Example 4: No operations
    std::vector<std::vector<int>> ops4;
    std::vector<int> res4 = processRangeColorings(4, ops4);
    assert(res4[1] == 0 && res4[2] == 0 && res4[3] == 0 && res4[4] == 0);

    // Example 5: Single position range repeated
    std::vector<std::vector<int>> ops5 = {{2, 2, 10}, {2, 2, 20}};
    std::vector<int> res5 = processRangeColorings(5, ops5);
    assert(res5[2] == 20 && res5[1] == 0 && res5[3] == 0);

    // Example 6: Large range covering all, then a small later range
    std::vector<std::vector<int>> ops6 = {{1, 10, 1}, {5, 7, 2}};
    std::vector<int> res6 = processRangeColorings(10, ops6);
    for (int i = 1; i <= 10; ++i) {
        if (i >= 5 && i <= 7) assert(res6[i] == 2);
        else assert(res6[i] == 1);
    }
    return 0;
}
