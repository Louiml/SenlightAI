/*
Write a C++ function `int countLitAfterFlips(int n, const std::vector<int>& flipPositions)` that models a row of `n` binary lights, all initially ON (`1`), indexed from `1` to `n`. For each flip position `d` in the given vector (in order), flip the state of every light from position `d+1` to `n` (inclusive) — that is, toggling each light in that suffix from ON to OFF or OFF to ON. After each flip, record the total number of lights that are currently ON. Return the total number of ON lights after the *last* flip operation. If the flip positions vector is empty, return `n` (all lights remain ON). The function must handle up to `n = 10^9` and up to `10^5` flips efficiently, using a dynamic segment tree with lazy propagation to avoid materializing the full range.
*/
#include <vector>

// Node for dynamic segment tree with lazy toggle.
struct SegNode {
    long long l, r;   // interval [l, r]
    long long v;      // number of ON lights in this interval
    int laz;          // lazy toggle flag (0 or 1)
    SegNode *L, *R;   // children (nullptr if not created)
    
    SegNode(long long a, long long b, long long c) : l(a), r(b), v(c), laz(0), L(nullptr), R(nullptr) {}
};

// Apply pending lazy toggle to this node and propagate to children if internal.
void push_lazy(SegNode* now) {
    if (now->laz) {
        now->v = (now->r - now->l + 1) - now->v;  // flip all lights in interval
        if (now->l != now->r) {
            long long mid = (now->l + now->r) >> 1;
            if (!now->L) now->L = new SegNode(now->l, mid, mid - now->l + 1);
            if (!now->R) now->R = new SegNode(mid + 1, now->r, now->r - mid);
            now->L->laz ^= now->laz;
            now->R->laz ^= now->laz;
        }
        now->laz = 0;
    }
}

// Toggle all positions in [a, b] within the tree rooted at now.
void update(SegNode* now, long long a, long long b) {
    push_lazy(now);
    if (now->r < a || b < now->l) return;
    if (a <= now->l && now->r <= b) {
        now->laz ^= 1;
        push_lazy(now);
        return;
    }
    long long mid = (now->l + now->r) >> 1;
    if (!now->L) now->L = new SegNode(now->l, mid, mid - now->l + 1);
    if (!now->R) now->R = new SegNode(mid + 1, now->r, now->r - mid);
    update(now->L, a, b);
    update(now->R, a, b);
    now->v = now->L->v + now->R->v;
}

// Main function: return total ON lights after applying all suffix flips.
int countLitAfterFlips(int n, const std::vector<int>& flipPositions) {
    SegNode* root = new SegNode(1, n, n);  // all ON initially
    
    for (int d : flipPositions) {
        long long start = static_cast<long long>(d) + 1;
        if (start > n) continue;  // empty range, no change
        update(root, start, n);
    }
    
    long long result = root->v;
    // Clean up dynamically allocated tree to avoid leaks (optional in contest environment)
    // For simplicity, we skip recursive deletion here; it's not required for correctness.
    return static_cast<int>(result);
}
#include <cassert>
#include <vector>

// Declaration of the function (in practice, include the solution header)
int countLitAfterFlips(int n, const std::vector<int>& flipPositions);

int main() {
    // Example 1: n=5, flips [1,2] -> after first flip: positions 2..5 toggled -> ON at 1, OFF at 2,3,4,5 => 1 ON; after second flip: positions 3..5 toggled -> ON at 1, OFF at 2, ON at 3,4,5 => 4 ON. So result 4.
    assert(countLitAfterFlips(5, {1, 2}) == 4);

    // Example 2: n=10, no flips -> all ON
    assert(countLitAfterFlips(10, {}) == 10);

    // Example 3: n=1, flips [0] -> start=1, flip position 1 -> OFF -> 0 ON
    assert(countLitAfterFlips(1, {0}) == 0);

    // Example 4: n=3, flip [3] -> start=4 > 3, no change -> all ON
    assert(countLitAfterFlips(3, {3}) == 3);

    // Example 5: n=4, flips [0,0,0] -> each flips suffix 1..4, three toggles -> net toggle (odd) -> all OFF
    assert(countLitAfterFlips(4, {0,0,0}) == 0);

    // Example 6: n=1000000000 (1e9), single flip at 0 -> flips everything -> 0 ON
    assert(countLitAfterFlips(1000000000, {0}) == 0);

    // Example 7: n=6, flips [2,2] -> first flip suffix 3..6 (4 lights) -> ON count = 2; second flip same suffix -> revert to 6 ON
    assert(countLitAfterFlips(6, {2,2}) == 6);

    // Example 8: n=8, flips [1,3,5] -> verify step by step manually -> after flips: suffix 2..8 toggled (OFF 7) -> ON=1; suffix 4..8 toggled (ON for those 5) -> ON=1+5=6? Let's compute: initial all ON (8). Flip 2..8: now ON at pos1, OFF at 2..8 => 1 ON. Flip 4..8: toggle pos4..8 from OFF to ON (5 become ON) => ON now at pos1, pos4..8 = 1+5=6. Flip 6..8: toggle pos6..8 from ON to OFF (3 become OFF) => ON count becomes 6-3=3. So result 3.
    assert(countLitAfterFlips(8, {1,3,5}) == 3);

    // Example 9: Large n and many flips but outcome known: n=7, flips [0,0] -> toggled twice -> all ON
    assert(countLitAfterFlips(7, {0,0}) == 7);

    // Example 10: n=2, flips [1] -> start=2..2, flip position 2 -> only position 2 OFF -> ON=1
    assert(countLitAfterFlips(2, {1}) == 1);

    return 0;
}
// The problem is a range toggle (XOR flip) on a suffix `[d+1, n]`, and we need the total sum of ON lights after each operation. Since `n` can be extremely large (up to 1e9), we cannot use a static array of size `n`. A dynamic segment tree is ideal: we only create nodes for ranges that are actually visited. Each node stores the number of ON lights in its interval (`v`) and a lazy toggle flag (`laz`). Initially the root represents `[1, n]` with all lights ON, so `v = n`. When we apply a toggle to a range, we use lazy propagation: if a node’s interval is fully covered, we invert its count (new `v` = interval length - old `v`) and toggle its lazy flag; if partially covered, we push the lazy flag to children (creating them if needed), recurse into children, then recompute `v = L->v + R->v`. The `push_lazy` function ensures that pending toggles are applied before any descent. Edge cases: if `d+1 > n` (i.e., `d >= n`), the update range is empty, so nothing changes; the answer remains the previous total. Also, when the vector is empty, we return `n`. Time complexity: each flip operation visits at most `O(log n)` nodes (with lazy propagation, amortized O(log n) per update, and each created node costs O(1) memory per distinct range visited). Since we only create nodes on the path to the updated range and their children, total memory is O(number of flips * log n). Space complexity is O(q log n) in the worst case, but typically much less.
