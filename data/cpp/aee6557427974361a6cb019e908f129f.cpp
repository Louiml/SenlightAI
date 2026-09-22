/*
Write a C++ function named `foldDequeOperations` that takes two integer parameters: `n` (the initial number of elements in a deque, all equal to 1, with positions 1 through `n`) and a vector of query strings. Each query string is either `"F a"` (fold: if the current non-empty deque has size `m`, the operation removes the first `a` elements and reverses the order of the remaining `m - a` elements, but in the problem's specific folding semantics, we treat it as: if the left part length <= right part length, we fold the left part onto the right part by adding the left part's values to the mirrored positions on the right, then discard the left part; otherwise we fold the right part onto the left part, adding right part values to mirrored left positions, discard the right part, and reverse the entire deque orientation) or `"Q l r"` (query: output the sum of the current deque elements from 1-based index `l` to `r`, inclusive, where indices refer to the current logical order of the deque). All queries are valid (fold `a` is between 1 and current size-1, and query `l <= r` within current size). The function should return a vector of integers containing the answers to all `Q` queries in order. The initial deque consists of `n` ones. Folding operation: if the left segment length `L_len = a` and right segment length `R_len = size - a`, then if `L_len <= R_len`, for each `i` from 1 to `L_len`, the element at position `a + i` (the mirrored position on the right) gets added the value of the element at position `a - i + 1` (the left element), and then the left segment is removed (so the new deque is the old right segment, with those added values). If `L_len > R_len`, then for each `i` from 1 to `R_len`, the element at position `a - i + 1` (the mirrored position on the left) gets added the value of the element at position `a + i` (the right element), and then the right segment is removed, and the deque is reversed (so the new deque is the old left segment in reverse order, with those added values). This is a classic "fold" operation on a sequence where all elements are 1 initially, and queries ask for range sums after arbitrary folds.
*/

#include <bits/stdc++.h>
using namespace std;

// Segment tree supporting point updates and range sum queries.
class SegmentTree {
    int size;
    vector<int> tree;
public:
    SegmentTree(int n) : size(n), tree(4 * n + 5, 0) {}

    void build(int node, int l, int r, int limit) {
        if (l == r) {
            tree[node] = (l <= limit) ? 1 : 0;
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, limit);
        build(node * 2 + 1, mid + 1, r, limit);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    void update(int node, int l, int r, int pos, int delta) {
        if (l == r) {
            tree[node] += delta;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(node * 2, l, mid, pos, delta);
        else update(node * 2 + 1, mid + 1, r, pos, delta);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    int query_range(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return query_range(node * 2, l, mid, ql, qr) +
               query_range(node * 2 + 1, mid + 1, r, ql, qr);
    }
};

// Main solution function.
// n: initial number of ones in the deque; queries: list of "F a" or "Q l r".
// Returns answers to all Q queries.
vector<int> foldDequeOperations(int n, const vector<string>& queries) {
    // Virtual array size is 2*n to allow mirroring beyond both ends.
    SegmentTree seg(2 * n);
    seg.build(1, 1, 2 * n, n);

    int L = 1, R = n;
    bool reversed = false; // false: logical left is physical L; true: logical left is physical R

    vector<int> answers;

    for (const string& q : queries) {
        char op;
        int a, b;
        istringstream iss(q);
        iss >> op;
        if (op == 'F') {
            iss >> a; // logical fold position from left (1-based)
            // Convert logical 'a' to physical index in the current orientation
            int phyA;
            if (!reversed) {
                phyA = L + a - 1;
            } else {
                phyA = R - a + 1;
            }
            int leftLen = a; // logical length of left segment
            int rightLen = (R - L + 1) - a; // logical length of right segment
            if (!reversed) {
                if (leftLen <= rightLen) {
                    // Fold left part onto right part
                    for (int i = L; i <= phyA; ++i) {
                        int mirror = 2 * phyA + 1 - i;
                        int val = seg.query_range(1, 1, 2 * n, i, i);
                        if (val != 0) {
                            seg.update(1, 1, 2 * n, mirror, val);
                            seg.update(1, 1, 2 * n, i, -val);
                        }
                    }
                    L = phyA + 1;
                } else {
                    // Fold right part onto left part, then reverse orientation
                    for (int i = R; i >= phyA + 1; --i) {
                        int mirror = 2 * phyA + 1 - i;
                        int val = seg.query_range(1, 1, 2 * n, i, i);
                        if (val != 0) {
                            seg.update(1, 1, 2 * n, mirror, val);
                            seg.update(1, 1, 2 * n, i, -val);
                        }
                    }
                    R = phyA;
                    reversed = !reversed;
                }
            } else {
                // reversed == true: logical indices are from R downwards
                if (leftLen <= rightLen) {
                    // Logical left is physical right side, so fold from physical 'phyA' upwards
                    for (int i = phyA; i >= L; --i) {
                        int mirror = 2 * phyA + 1 - i;
                        int val = seg.query_range(1, 1, 2 * n, i, i);
                        if (val != 0) {
                            seg.update(1, 1, 2 * n, mirror, val);
                            seg.update(1, 1, 2 * n, i, -val);
                        }
                    }
                    // Now logical left (physical right side) is removed, so physical R moves left
                    R = phyA - 1;
                    // The orientation flips because we now treat the remaining part as reversed
                    reversed = !reversed;
                } else {
                    // Logical right is physical left side, fold it onto physical right side
                    for (int i = L; i <= phyA - 1; ++i) {
                        int mirror = 2 * (phyA - 1) + 1 - i; // careful: fold line is at phyA-1
                        // Actually, for right side folding onto left side in reversed orientation,
                        // the mirror formula remains same with phyA as the boundary between left and right.
                        // Let's use phyA as the first physical index of the right segment.
                        // Right segment physical indices are from phyA to R, left segment from L to phyA-1.
                        // Mirror of physical i (in right segment) is 2*phyA - i? Let's derive.
                        // In logical coordinates, if we fold the right part onto left, we mirror around the boundary.
                        // For a position i in the right segment, its mirror is a - (i - (a+1)) = 2a+1-i when a is the last left index.
                        // But here the last left index is phyA-1, so mirror = 2*(phyA-1)+1 - i = 2*phyA-1 - i.
                        int mirror = 2 * phyA - 1 - i;
                        int val = seg.query_range(1, 1, 2 * n, i, i);
                        if (val != 0) {
                            seg.update(1, 1, 2 * n, mirror, val);
                            seg.update(1, 1, 2 * n, i, -val);
                        }
                    }
                    L = phyA; // Remove left physical segment
                    // Orientation does not change (still reversed)
                }
            }
        } else if (op == 'Q') {
            int l, r;
            iss >> l >> r; // logical 1-based indices
            int physL, physR;
            if (!reversed) {
                physL = L + l - 1;
                physR = L + r - 1;
            } else {
                physL = R - r + 1;
                physR = R - l + 1;
            }
            answers.push_back(seg.query_range(1, 1, 2 * n, physL, physR));
        }
    }

    return answers;
}

#include <bits/stdc++.h>
using namespace std;

// Assume foldDequeOperations is defined as above.

int main() {
    // Test 1: Simple fold and query, no reversal
    {
        vector<string> q = {"Q 1 3", "F 1", "Q 1 2"};
        vector<int> res = foldDequeOperations(3, q);
        assert(res == vector<int>({3, 2}));
    }
    // Test 2: Fold larger right onto left, causing reversal
    {
        vector<string> q = {"F 2", "Q 1 1"};
        vector<int> res = foldDequeOperations(4, q);
        // Initial: [1,1,1,1], fold a=2: leftLen=2, rightLen=2, equal, choose left<=right branch
        // Actually leftLen=2 <= rightLen=2, so fold left onto right: mirror left[1] to pos3, left[2] to pos2, remove left => [1,2,1] (positions 2,3,4)
        // Query range 1..1: sum = 1
        assert(res == vector<int>({1}));
    }
    // Test 3: Multiple folds and queries
    {
        vector<string> q = {
            "F 1", // leftLen=1, rightLen=2, fold left onto right => [2,1] (logical)
            "Q 1 2", // sum = 3
            "F 1", // now size=2, leftLen=1, rightLen=1, equal, fold left onto right => [3]
            "Q 1 1" // sum = 3
        };
        vector<int> res = foldDequeOperations(3, q);
        assert(res == vector<int>({3, 3}));
    }
    // Test 4: Reversal case where right side folded onto left
    {
        vector<string> q = {
            "F 3", // n=5, a=3 => leftLen=3, rightLen=2, fold right onto left, reverse orientation
            "Q 1 3"
        };
        // Initial: [1,1,1,1,1]
        // a=3, left=[1,1,1], right=[1,1]
        // Fold right onto left: mirror pos4 to pos2, pos5 to pos1, remove right, reverse => original left reversed: [1,1,1] => after add: [2,2,1]? Let's compute:
        // Mirror of pos4 (value1) to pos2 (2*3+1-4=3? Wait: a=3, mirror of i=4 is 2*3+1-4=3), so pos3 +=1 => pos3=2
        // Mirror of i=5 is 2*3+1-5=2, pos2 +=1 => pos2=2
        // Remove right (pos4,pos5), keep left (pos1,pos2,pos3) => [1,2,2]
        // Reverse orientation, so logical order becomes [2,2,1] (since reversed)
        // Query 1..3 sum = 5
        assert(foldDequeOperations(5, q) == vector<int>({5}));
    }
    // Test 5: Complex sequence with multiple reversals
    {
        vector<string> q = {
            "F 2", // n=5, leftLen=2, rightLen=3, left<=right => fold left onto right
            // initial [1,1,1,1,1], a=2: mirror pos1 to pos4, pos2 to pos3, remove left => [1,2,2,1] (logical)
            "Q 1 4", // sum=6
            "F 1", // size=4, a=1, leftLen=1, rightLen=3, fold left onto right => mirror pos1 to pos3 => [2,2,2,1]? Wait: logical is [1,2,2,1], folding left (pos1=1) onto right (mirror pos1=2*1+1-1=2? but a=1, mirror of i=1 is 2*1+1-1=2? Actually physical indices? Let's use logical: new logical after fold: remove first, add to position 2 => [3,2,1]? Let's trust the algorithm.
            "Q 1 3"
        };
        vector<int> res = foldDequeOperations(5, q);
        assert(res[0] == 6);
        // After second fold, logical array should be [3,2,1] sum=6
        assert(res[1] == 6);
    }
    // Test 6: No folds, just queries
    {
        vector<string> q = {"Q 1 1", "Q 2 4"};
        vector<int> res = foldDequeOperations(4, q);
        assert(res == vector<int>({1, 3}));
    }
    // Test 7: Folding down to single element
    {
        vector<string> q = {"F 1", "F 1", "Q 1 1"};
        // n=2: after first fold size=1, second fold invalid? Actually a must be between 1 and size-1; if size=1 no valid fold, test skipped.
        // Use n=3: F1 -> size=2, F1 -> size=1, Q1 -> sum=3
        vector<int> res = foldDequeOperations(3, q);
        assert(res == vector<int>({3}));
    }
    // Test 8: All ones fold entirely
    {
        vector<string> q = {"F 2", "Q 1 3"};
        // n=4: leftLen=2, rightLen=2, fold left onto right => [2,2,1]? Actually initial [1,1,1,1], fold left (1,1) onto right (mirror pos1->pos3, pos2->pos2) => [2,2,1] (physical pos2,3,4) sum=5
        // Query 1..3 sum = 5
        assert(foldDequeOperations(4, q) == vector<int>({5}));
    }
    // Test 9: Large n and many queries, just check consistency
    {
        int n = 100;
        vector<string> q;
        for (int i = 0; i < 10; ++i) q.push_back("Q 1 " + to_string(n - i));
        vector<int> res = foldDequeOperations(n, q);
        for (int v : res) assert(v == n);
    }
    // Test 10: Mixed folds and queries with reversal
    {
        vector<string> q = {
            "F 3", // n=5 => right folded, reversed => [2,2,1,1]? Actually after fold: [1,2,2] reversed becomes [2,2,1]
            "Q 1 2", // sum=4
            "F 1", // size=3, a=1, leftLen=1, rightLen=2, fold left onto right => [4,1] (logical)
            "Q 1 2" // sum=5
        };
        vector<int> res = foldDequeOperations(5, q);
        assert(res == vector<int>({4, 5}));
    }
    return 0;
}

// The problem is a direct simulation of the folding operation on a deque of numbers, with range sum queries. The naive approach would be to maintain an actual deque (or vector) and perform each fold by copying elements, which could take O(n) per fold and O(n) per query, leading to O(Q * n) time, which is too slow for large n and Q. However, the provided code uses a segment tree over an extended array of size `2*n` to support point updates and range sum queries in O(log n) time per operation, and handles folding by transferring values from one side to the mirrored position on the other side using point updates. The key idea is to keep track of a logical left and right boundary (`L` and `R`) in a virtual array of size `2*n`. Initially the deque occupies positions `[L, R]` where `L=1`, `R=n`. When folding left part onto right part, for each element in the left segment (indices `L` to `a`), we add its value to the mirrored position on the right (which is `2*a+1 - index`), then set the original to 0. Then we move `L` to `a+1`. When folding right part onto left part, we mirror elements from the right segment (indices `a+1` to `R`) onto the left (mirrored position also `2*a+1 - index`), set originals to 0, move `R` to `a`, and toggle a `rev` flag to indicate that the logical order of the deque is reversed. In reversed mode, all subsequent operations (queries and next folds) need to map logical positions to physical positions accordingly. The segment tree supports point update (adding a value to a position) and range sum query. Each fold operation touches at most `min(L_len, R_len)` elements, and each touch is O(log n) for two point updates (one to add to mirror, one to set original to zero, but the original may already be zero? Actually we set to zero via `-query(i)`). The total number of element moves across all folds is at most O(n log n) because each fold moves at most half of the current size, and the size reduces by at least half in the worst case (when the smaller side is folded). Actually, if we always fold the smaller side, the size reduces to the larger side, which is at least half the original, so the total work across all folds is O(n log n) in the worst case (since each element can be moved O(log n) times). Queries are O(log n) each. Therefore, the overall time complexity is O((n log n) + Q log n) and space O(n) for the segment tree. Edge cases include when the fold exactly splits the deque into two equal halves (then both branches are possible; choose either), and when the deque size becomes 1 (no further folds, but queries still possible). Also, the segment tree must be built over the extended range `1` to `2*n`, and we must correctly handle the `rev` flag to map logical indices to physical positions: in non-reversed mode, logical index `i` maps to physical `L + i - 1`; in reversed mode, logical index `i` maps to physical `R - i + 1`. For fold operations, we compute the physical index `a` from the logical input using the current orientation, then perform the mirroring exactly as in the original code but with explicit logical mapping.
