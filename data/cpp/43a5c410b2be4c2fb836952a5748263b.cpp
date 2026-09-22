// You are given a sorted array of `n` distinct integers (`1 ≤ n ≤ 10^5`), a maximum jump length `l`, and `q` queries (`1 ≤ q ≤ 10^5`). For a query `(a, b)`, find the minimum number of jumps required to go from position `a` to position `b` (inclusive, 1-indexed) in the array, where each jump from position `i` can land on any position `j > i` such that `x[j] - x[i] ≤ l`. You may not jump to a position beyond `b`, and if `a == b` the answer is `0`. Write a C++ function `int minJumps(vector<int>& x, int l, int a, int b)` that preprocesses the array once (using a class or static data) and answers each query efficiently. The preprocessing must handle up to `10^5` elements, and each query must be answered in `O(sqrt(n))` time. Divide the array into blocks of size `sqrt(n)`, precompute the farthest position reachable after exactly one block of jumps, and then for each query, jump block-by-block followed by single jumps. Handle the case where `a > b` by swapping them. Return the minimum number of jumps.
// The solution uses a block decomposition technique. First, for each position `i`, compute `nxt[i]` as the farthest index reachable in exactly one jump from `i` (using binary search on the sorted array since `x` is sorted). Then, choose block size `B = sqrt(n)`. Precompute `jump[i]` as the index reached after performing `B` jumps starting from `i`, using a loop that applies `nxt` `B` times (or stops early at `n`). For each query `(a, b)`, if `a == b` return 0. Otherwise, repeatedly jump using `jump[a]` as long as `jump[a] <= b`, each time adding `B` to the answer. After that, perform single jumps (`a = nxt[a]`) until `a` reaches `b`, incrementing the answer by 1. Because each block jump covers `B` single jumps, and the number of block jumps is at most `n/B`, and the final single jumps are at most `B`, the time per query is `O(n/B + B) = O(sqrt(n))`. Preprocessing is `O(n log n)` for the binary search to compute `nxt` plus `O(n * B)` for the block jumps, which is `O(n sqrt(n))` total. Edge cases: `a` may already be at `n`, and `jump` for position `n` is defined as a large sentinel. Also ensure that `jump[a]` might exceed `b` even if `a` is far, which is fine. When `a > b`, swap them to make `a ≤ b`. The time complexity for `q` queries is `O(q sqrt(n))`, and preprocessing is `O(n sqrt(n))` with `O(n)` memory.
#include <vector>
#include <algorithm>
#include <cmath>

// Class to answer range jump queries on a sorted array.
class JumpRangeQueries {
private:
    int n;
    int blockSize;
    std::vector<int> x;       // sorted array (1-indexed internally, but we shift)
    std::vector<int> nxt;     // nxt[i] = farthest index reachable in one jump from i
    std::vector<int> metaJump; // metaJump[i] = index after blockSize jumps from i

public:
    // Constructor: takes a 1-indexed array 'input' (size n+1, input[0] unused).
    JumpRangeQueries(const std::vector<int>& input, int l) {
        n = input.size() - 1;
        x = input;
        blockSize = static_cast<int>(std::sqrt(n)) + 1;
        nxt.assign(n + 1, n);
        metaJump.assign(n + 1, n + 1);

        // Precompute nxt[i] using binary search
        for (int i = 1; i <= n; ++i) {
            int lo = i, hi = n;
            while (lo < hi) {
                int mid = (lo + hi + 1) / 2;
                if (x[mid] - x[i] <= l) {
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }
            nxt[i] = lo;
        }

        // Precompute metaJump[i] = position after blockSize jumps
        for (int i = 1; i <= n; ++i) {
            int cur = i;
            for (int step = 0; step < blockSize; ++step) {
                if (cur == n) {
                    cur = n + 1; // sentinel
                    break;
                }
                cur = nxt[cur];
            }
            metaJump[i] = cur;
        }
        // Sentinel for position n is already set as n+1
    }

    // Answer query for positions a and b (1-indexed), returns min jumps.
    int minJumps(int a, int b) const {
        if (a > b) std::swap(a, b);
        if (a == b) return 0;

        int ans = 0;
        int cur = a;
        // Jump block-wise as long as the metaJump target is within bounds
        while (cur <= n && metaJump[cur] <= b) {
            cur = metaJump[cur];
            ans += blockSize;
        }
        // Final single jumps
        while (cur < b) {
            cur = nxt[cur];
            ++ans;
        }
        return ans;
    }
};

// Required function signature: returns min jumps for a query.
int minJumps(const std::vector<int>& x, int l, int a, int b) {
    // The input x is 1-indexed (x[0] unused), but we'll adapt if needed.
    // We'll assume x already has size n+1 with x[0] irrelevant.
    static JumpRangeQueries solver(x, l);
    return solver.minJumps(a, b);
}
#include <cassert>
#include <vector>

int main() {
    // Example 1: n=5, l=3, array [_,1,3,5,8,10]
    std::vector<int> x1 = {0, 1, 3, 5, 8, 10};
    int l1 = 3;
    assert(minJumps(x1, l1, 1, 5) == 4);
    assert(minJumps(x1, l1, 2, 4) == 2);
    assert(minJumps(x1, l1, 3, 3) == 0);
    assert(minJumps(x1, l1, 5, 1) == 4); // swapped

    // Example 2: n=3, l=1, array [_,1,2,3]
    std::vector<int> x2 = {0, 1, 2, 3};
    int l2 = 1;
    assert(minJumps(x2, l2, 1, 3) == 2);
    assert(minJumps(x2, l2, 1, 2) == 1);

    // Example 3: Large jump length, direct jump always possible
    std::vector<int> x3 = {0, 1, 5, 9, 13, 17};
    int l3 = 20;
    assert(minJumps(x3, l3, 1, 5) == 1);
    assert(minJumps(x3, l3, 2, 4) == 1);

    // Example 4: Single element
    std::vector<int> x4 = {0, 7};
    int l4 = 10;
    assert(minJumps(x4, l4, 1, 1) == 0);

    // Example 5: Increasing difficulty, l=2, array [1,3,5,7,9]
    std::vector<int> x5 = {0, 1, 3, 5, 7, 9};
    int l5 = 2;
    assert(minJumps(x5, l5, 1, 5) == 4);
    assert(minJumps(x5, l5, 1, 4) == 3);
}
