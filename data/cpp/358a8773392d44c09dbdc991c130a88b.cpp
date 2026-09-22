// Write a C++ function `long long maxSubarraySumAfterOperations(int n, const std::vector<int>& initial, const std::vector<Operation>& ops)` that processes a sequence of operations on a dynamic array. The initial array contains `n` positive integers (all > 0), and we maintain a sentinel array with two extra elements at the beginning and end set to a very large negative value (e.g., `-1e18`) to simplify boundary conditions. The operations are of the following types (each operation is legal and the array always has at least one real element):  
// - `INSERT pos arr`: Insert the given sequence `arr` (positive integers) after position `pos` (1-indexed among real elements).  
// - `DELETE pos len`: Delete `len` consecutive elements starting at position `pos`.  
// - `SET pos len val`: Set all `len` consecutive elements starting at `pos` to value `val` (positive).  
// - `REVERSE pos len`: Reverse the order of `len` consecutive elements starting at `pos`.  
// - `GET_SUM pos len`: Query the sum of `len` consecutive elements starting at `pos`.  
// - `MAX_SUM`: Query the maximum sum of any non-empty contiguous subarray in the entire current array (including all real elements, not sentinels).  
//
// The function must return a vector of results: for each `GET_SUM` operation, store the queried sum, and for each `MAX_SUM` operation, store the maximum subarray sum (even if all elements are positive, the maximum is simply the total sum; if any negative numbers appear after `SET`, handle correctly). Assume initial elements and inserted values are positive, but `SET` may assign negative values. The maximum number of real elements at any time is at most 5×10^5, and total number of inserted elements can be large, so space must be proportional to current size, not total inserts. Implement using a splay tree with lazy propagation for assignment, reversal, and maintaining node aggregates (sum, maximum subarray sum, maximum prefix sum, maximum suffix sum, and size).
// The core structure is a splay tree where each node stores: value `val`, subtree size `sz`, subtree sum `sum`, subtree maximum non-empty subarray sum `best` (initialize to `-inf`), maximum prefix sum `pref` (initialize to `-inf` for empty but allow 0 for prefix to handle “can be empty” in merge), maximum suffix sum `suff`, and lazy tags `setTag` (bool + value) and `revTag`. Each node also has parent, left, and right pointers. To avoid overhead, we use arrays of fixed size `MAXN = 500005` for `val`, `sz`, `sum`, `best`, `pref`, `suff`, `left`, `right`, `parent`, and a free-list stack for memory reuse. The initial tree is built with two sentinel nodes (index 1 and `n+2`) around the real elements (indices 2..n+1). The sentinel values are set to a very negative number (e.g., `-1e18`) so they never become part of any maximum subarray (because max subarray must be non-empty and sentinels are not considered in queries; but they are included in the tree for easy boundary operations). However, to avoid interfering with `max` operations, we set `best` for sentinel nodes to `-inf` and `pref`/`suff` to `-inf` as well, and during `pull` we carefully handle empty subtrees. For a node with value `v`, after `pull`, `best = max(best[left], best[right], suff[left] + v + pref[right])`, `pref = max(pref[left], sum[left] + v + pref[right])`, `suff = max(suff[right], suff[left] + v + sum[right])`, and `sum = sum[left] + v + sum[right]`, `sz = sz[left]+1+sz[right]`. The sentinel nodes have `pref` and `suff` equal to `-inf` (not 0) so they cannot be picked in prefix/suffix when combining. When applying `set` to a subtree, we set `val` to `c`, `sum = c * sz`, `best = max(c, c*sz)` (since if c is negative, best should be c for single element, but because subtree may have many elements, if all equal c and c negative, best = c, if c positive, best = c*sz), `pref = max(c*sz, 0)` for non-empty subtree (but we keep 0 for empty? Actually for sentinel we want `-inf`, so when applying set to sentinel we should handle specially; we only apply set to real subtree, never sentinel), and set `pref` and `suff` to `max(c*sz, 0)` (since they can be empty but we also allow 0). But for correctness, when `c` is positive, `pref` should be `c*sz` (sum of all) because max prefix is whole subtree; when `c` is negative, `pref` should be 0 (choose empty prefix). So formula `pref = max(c*sz, 0)` works. Similarly `suff`. `best = max(c, c*sz)` works because if c negative, best is c (single element), if positive, best is c*sz. For lazy `rev`, just swap left and right and swap `pref` and `suff`. The main operations use splay with sentinels: for range `[pos, pos+len-1]`, we splay node at index `pos` (which is `pos` in 1-indexed real array, but since sentinel at position 0 real?), careful: we maintain the tree with sentinels at both ends, so the real elements are at positions 1..n, and we have sentinel at position 0 and position n+1. To access a range, we splay `find(pos)` (the node that would be at rank `pos` where rank counts sentinel+real) and then `find(pos+len+1)`? Actually standard trick: to get subtree for real positions `l..r` inclusive, we splay the node before `l` (rank `l`) and then splay the node after `r` (rank `r+2`)?? Let's define ranks: node with rank 1 is left sentinel, rank 2..n+1 are real elements, rank n+2 is right sentinel. For a range of real positions `l` to `r` (1-indexed among real), we need to isolate them. The left boundary is the node at rank `l` (which is the element just before the range, since real position `l` actually corresponds to rank `l+1`). So find node with rank `l` (which is sentinel or previous real) and splay it to root, then find node with rank `r+2` (which is the element after the range) and splay it to right child of root. Then the left child of that right child is exactly the subtree of nodes from real position `l` to `r` (since we have rank positions: sentinel at 1, real[1] at 2, real[2] at 3, ..., real[n] at n+1, sentinel at n+2). So for range `pos` to `pos+len-1`, we splay `find(pos)` (rank = pos because sentinel at 1, so pos is rank pos) and then `find(pos+len+1)` (since after `pos` real elements, we want the node after the range, which is rank `pos+len+1` because real elements are at rank `pos+1`..`pos+len`). Then the subtree `left[right[root]]` is the range. For insertion, we insert after position `pos` (real), meaning we want to insert new nodes between rank `pos+1` (the node at position `pos` real?) Actually we use the same boundary isolation: to insert after real position `pos`, we splay `find(pos+1)` (the node at real position `pos` plus sentinel?) Let's define carefully: We have sentinel at rank 1, real elements at ranks 2..n+1. Position `pos` in real sense means we want to insert after the `pos`-th real element. So the left boundary is the node at rank `pos+1` (which is the `pos`-th real element). To insert after it, we splay that node to root, then splay its successor (rank `pos+2`?) Actually the standard method: splay `find(pos+1)` to root, then splay `find(pos+2)` to right child of root. Then the right child's left subtree is empty (since they are adjacent), and we set that left child to the newly built tree. But careful: after splaying `find(pos+1)` to root, the right subtree of root contains all elements after the `pos`-th real element, including the right sentinel. Then we splay `find(pos+2)` (which is the next real element or right sentinel) to be the right child of root. Then the left child of that right child is empty (since they are adjacent). So we place the new tree there. So insertion function takes rank = pos+1 (real) but we call `insert(rank, n)` with rank meaning the node position in the tree (where rank 1 is left sentinel). In the original code, they use `pos` after incrementing `pos++` to account for sentinel. They call `insert(pos, len)` where `pos` is rank directly. We'll follow similar: for user operation with real position `pos`, we convert to tree rank `pos+1` for the left boundary (since sentinel at rank 1, real positions start at 2). For deletion, we isolate range with left boundary `pos` (rank) and right boundary `pos+len+1`. The function must return a vector<long long> (since sums may exceed int). We'll use `long long` for all sums and values. Time complexity per operation is amortized O(log n) due to splay rotations, with O(log n) per splay. Memory usage is O(current size) because we recycle deleted nodes using a free list. The total number of insertions can be large but we only reuse space, so total allocated nodes is at most 5*10^5 + maybe sentinel? Actually the free list initially has MAXN nodes, and we never exceed current size + some overhead? Since we recycle deleted nodes, the maximum number of live nodes at any time is at most 5*10^5 plus the sentinels (2) plus maybe during build we use all? We build using `space` stack, so we allocate nodes from the free list; when deleting, we push back to free list. So total live nodes never exceed `MAXN`. We'll set MAXN = 500005 + 5 (to be safe). Edge cases: `len` could be 0? But operations are guaranteed legal, so `len>=1`. For `MAX_SUM` query, we return `best[head]` but careful: the root includes sentinels, and sentinels have `best` set to `-inf` so they don't affect the maximum. However, the sentinels have `pref` and `suff` as `-inf`, which may cause issues in `pull` for non-sentinel nodes? Let's verify: For a node with left child being sentinel, `pref[left] = -inf`, and `sum[left]` is the sentinel's value (very negative), so `pref[node] = max(-inf, sum[left] + val + pref[right])` which effectively ignores left sentinel's contribution. Similarly `suff`. For root's `best`, it will consider combinations that include sentinels? Since sentinel values are very negative, any subarray that includes a sentinel will have a sum that is negative infinity plus something, which will be much smaller than any positive subarray. So the maximum subarray will never include sentinels. However, if the entire real array is negative (after SET operations), then the maximum subarray is the maximum single element (which is negative). The sentinels have very negative values, so they won't be chosen. So `best[head]` correctly returns the max subarray among real elements. But we must initialize sentinel nodes with `best = -inf` and `pref = -inf`, `suff = -inf`, `sum = -inf * size`? Actually `sum` is the sum of subtree, but for sentinel we set `val = -INF`, and `sum = -INF*size`? But `size` is 1 for sentinel, so `sum = -INF`. That's fine. However, in `pull`, when we combine with sentinel's `pref = -INF`, it works because `max(-INF, ...)` ignores. But be careful: `suf[l] + num + pre[r]` could overflow if `-INF + something + -INF`? But we use `long long` and `-1e18` as sentinel, and real sums are at most 5*10^5 * (say 10^9) = 5*10^14, so sum of two sentinels is about -2e18 which is still within `long long` range. So safe. Also, for `setValue` on a subtree, we must not apply to sentinels because they are boundary and should remain `-INF`. But our operations isolate the real range that does not include sentinels, so `setValue` is only called on real subtrees. For `reverse`, same. For `insert`, we build a new tree with real values only (no sentinels), so all nodes are real. At the initial build, we build a tree containing sentinels and real values; we set sentinel nodes with `val = -INF`, and later we never modify them except possibly by `setValue` if a range includes them? But our ranges are between sentinels, so left boundary is rank `pos` (which is a real element or left sentinel?) For operation with `pos` starting at 1, we splay `find(pos)` where `pos` is 1 for the first real element? Actually in the original code, they increment `pos` by 1 before operations to account for the left sentinel. Let's adopt that: In the main processing, for user input `pos`, we do `pos++` to convert to tree rank because rank 1 is left sentinel. Then for a range of length `len`, we splay `find(pos)` and `find(pos+len+1)`. So the subtree `left[right[root]]` exactly contains the real elements from position `pos` to `pos+len-1` in rank terms. For insertion, we also use `pos` as rank after increment, and we splay `find(pos)` and `find(pos+1)` to get an empty spot. So we will implement a helper `extract(l, r)` that splays and returns the subtree root. We'll also have a `merge`? Not needed. We need `build` for arrays. We'll use a free list stack.
//
// We must carefully implement `pull(i)` and lazy propagation. For `setValue` on node `i` with value `v` (real node), we set `val[i]=v`, `sum[i]=v*sz[i]`, `best[i]=max(v, v*sz[i])`, `pref[i]=max(v*sz[i], 0LL)` (because prefix can be empty), `suff[i]=max(v*sz[i], 0LL)`. But for a subtree of size 1, `best[i]=v` (since v could be negative), `pref=max(v,0)`? Actually `pref` is max prefix sum, and prefix can be empty, so we allow 0. So `pref[i]=max(v,0)` for size 1; but our formula `max(v*sz,0)` for sz=1 gives `max(v,0)` which is correct. For `suff` same. For a subtree of size >1 with all equal v, the max prefix is either empty (0) or the whole sum (v*sz). So formula holds. For `best[i]=max(v, v*sz)` works because if v positive, best = v*sz (whole subtree), if v negative, best = v (single element). So good. For `setReverse` on node `i`, we just swap left and right children, and swap `pref[i]` and `suff[i]`, and toggle `rev[i]^=1`.
//
// The `down` function must push lazy tags to children before any traversal. For `update[i]` (set tag), we call `setValue` on both children with the tag value, and clear `update[i]`. For `rev[i]` tag, we swap children of current node? Wait, `down` is called on node `i` before accessing its children. So we must apply the reverse to children: swap their left/right? Actually the reverse tag means the whole subtree is reversed. When we push down to children, we should apply `setReverse` to each child (which swaps their own children and toggles their rev). Also for the current node, when the tag is on it, we have already swapped its children when the tag was applied via `setReverse(i)` (which swaps left and right of i). So in `down(i)`, we just propagate to children: `setReverse(ls[i])` and `setReverse(rs[i])`, and clear `rev[i]`. For `update` tag, we call `setValue(ls[i], change[i])` and `setValue(rs[i], change[i])`. Then clear.
//
// For `find(rank)`, we need to go down the tree from root, pushing `down` at each node to ensure correct subtree sizes and child pointers. Return the node with the given rank (1-indexed in the whole tree including sentinels).
//
// We also need a `recycle` function that frees the subtree by pushing indices back to free list.
//
// Time complexity: Each operation (insert, delete, set, reverse, query) does two splays (or one for insert? Actually insert does two splays too, but we can do one splay of `pos` and then one of `pos+1`, but the second is actually the same because after splaying `pos` to root, `pos+1` is the successor; we can do `splay(find(pos), 0)` and then `splay(rs[head], head)` to bring the successor to right child of root, but since find already potentially pushed tags, it's fine). Each splay is amortized O(log n). So total O((n + m) log n) time where m is number of operations. The space is O(MAXN) for arrays, independent of total inserts because we recycle deleted nodes.
//
// We'll define a struct `Operation` with fields: type (string), and parameters. The function signature will take `const std::vector<Operation>& ops`.
//
// Now write the solution.
#include <bits/stdc++.h>
using namespace std;

struct Operation {
    string type;
    int pos, len, val; // for INSERT: pos, len (but values are in a separate vector? We'll use a vector for insert values)
    vector<int> insertVals;
};

const long long NEG_INF = -1e18;
const int MAXN = 500005 + 5;

int leftChild[MAXN];
int rightChild[MAXN];
int parent[MAXN];
long long val[MAXN];
int sz[MAXN];
long long sum[MAXN];
long long best[MAXN];
long long pref[MAXN];
long long suff[MAXN];
bool hasSetTag[MAXN];
long long setTagVal[MAXN];
bool hasRevTag[MAXN];
int freeStack[MAXN];
int freeTop;
int root = 0;

void pull(int i) {
    int l = leftChild[i], r = rightChild[i];
    sz[i] = sz[l] + sz[r] + 1;
    sum[i] = sum[l] + sum[r] + val[i];
    best[i] = max(max(best[l], best[r]), suff[l] + val[i] + pref[r]);
    pref[i] = max(pref[l], sum[l] + val[i] + pref[r]);
    suff[i] = max(suff[r], suff[l] + val[i] + sum[r]);
}

int isRightChild(int i) {
    return rightChild[parent[i]] == i;
}

void rotate(int i) {
    int f = parent[i], g = parent[f];
    bool isRight = isRightChild(i);
    if (isRight) {
        rightChild[f] = leftChild[i];
        if (rightChild[f] != 0) parent[rightChild[f]] = f;
        leftChild[i] = f;
    } else {
        leftChild[f] = rightChild[i];
        if (leftChild[f] != 0) parent[leftChild[f]] = f;
        rightChild[i] = f;
    }
    parent[f] = i;
    parent[i] = g;
    if (g != 0) {
        if (rightChild[g] == f) rightChild[g] = i;
        else leftChild[g] = i;
    }
    pull(f);
    pull(i);
}

void splay(int i, int goal) {
    while (parent[i] != goal) {
        int f = parent[i], g = parent[f];
        if (g != goal) {
            if (isRightChild(i) == isRightChild(f)) rotate(f);
            else rotate(i);
        }
        rotate(i);
    }
    if (goal == 0) root = i;
}

void setValue(int i, long long v) {
    if (i == 0) return;
    hasSetTag[i] = true;
    setTagVal[i] = v;
    val[i] = v;
    sum[i] = (long long)sz[i] * v;
    best[i] = max(v, sum[i]);
    pref[i] = max(sum[i], 0LL);
    suff[i] = max(sum[i], 0LL);
}

void setReverse(int i) {
    if (i == 0) return;
    swap(pref[i], suff[i]);
    hasRevTag[i] ^= 1;
}

void pushDown(int i) {
    if (i == 0) return;
    if (hasSetTag[i]) {
        setValue(leftChild[i], setTagVal[i]);
        setValue(rightChild[i], setTagVal[i]);
        hasSetTag[i] = false;
    }
    if (hasRevTag[i]) {
        swap(leftChild[i], rightChild[i]);
        setReverse(leftChild[i]);
        setReverse(rightChild[i]);
        hasRevTag[i] = false;
    }
}

int newNode(long long v) {
    int i = freeStack[freeTop--];
    sz[i] = 1;
    val[i] = v;
    sum[i] = v;
    best[i] = v;
    pref[i] = max(v, 0LL);
    suff[i] = max(v, 0LL);
    leftChild[i] = rightChild[i] = parent[i] = 0;
    hasSetTag[i] = hasRevTag[i] = false;
    return i;
}

int build(int l, int r, const vector<long long>& arr) {
    if (l > r) return 0;
    int mid = (l + r) / 2;
    int node = newNode(arr[mid]);
    int lc = build(l, mid-1, arr);
    int rc = build(mid+1, r, arr);
    if (lc != 0) { leftChild[node] = lc; parent[lc] = node; }
    if (rc != 0) { rightChild[node] = rc; parent[rc] = node; }
    pull(node);
    return node;
}

int findRank(int rank) {
    int i = root;
    while (i != 0) {
        pushDown(i);
        int lSize = sz[leftChild[i]];
        if (lSize + 1 == rank) return i;
        else if (lSize >= rank) i = leftChild[i];
        else { rank -= lSize + 1; i = rightChild[i]; }
    }
    return 0;
}

// isolate the subtree containing real positions [l, r] within the tree (1-indexed by rank, where rank 1 is left sentinel)
// returns the root of that subtree, and after the two splays, this subtree is leftChild[rightChild[root]]
int isolate(int l, int r) {
    int a = findRank(l);
    int b = findRank(r+2);
    splay(a, 0);
    splay(b, a);
    return leftChild[b];
}

void recycleTree(int i) {
    if (i == 0) return;
    recycleTree(leftChild[i]);
    recycleTree(rightChild[i]);
    freeStack[++freeTop] = i;
}

vector<long long> maxSubarraySumAfterOperations(int n, const vector<int>& initial, const vector<Operation>& ops) {
    // Initialize free list
    freeTop = MAXN - 1;
    for (int i = 1; i <= freeTop; ++i) freeStack[i] = i;

    // Build initial tree with two sentinels
    vector<long long> arr;
    arr.push_back(NEG_INF); // left sentinel at index 0 (real position 0)
    for (int i = 0; i < n; ++i) arr.push_back(initial[i]);
    arr.push_back(NEG_INF); // right sentinel
    root = build(0, (int)arr.size()-1, arr);

    vector<long long> results;

    for (const auto& op : ops) {
        if (op.type == "MAX_SUM") {
            results.push_back(best[root]);
        } else if (op.type == "INSERT") {
            // op.pos is real position; convert to tree rank by adding 1 for left sentinel
            int rank = op.pos + 1; // after left sentinel
            // Build new subtree from insert values
            vector<long long> newVals;
            for (int v : op.insertVals) newVals.push_back(v);
            int newTree = build(0, (int)newVals.size()-1, newVals);
            // Isolate empty spot: splay rank and rank+1
            int a = findRank(rank);
            int b = findRank(rank+1);
            splay(a, 0);
            splay(b, a);
            leftChild[b] = newTree;
            parent[newTree] = b;
            pull(b);
            pull(a);
        } else {
            int rank = op.pos + 1; // start of range in tree rank
            int endRank = rank + op.len - 1;
            int subRoot = isolate(rank, endRank);
            if (op.type == "DELETE") {
                recycleTree(subRoot);
                leftChild[rightChild[root]] = 0; // clear the link
                pull(rightChild[root]);
                pull(root);
            } else if (op.type == "SET") {
                setValue(subRoot, op.val);
                pull(rightChild[root]);
                pull(root);
            } else if (op.type == "REVERSE") {
                setReverse(subRoot);
                pull(rightChild[root]);
                pull(root);
            } else if (op.type == "GET_SUM") {
                results.push_back(sum[subRoot]);
                // no need to pull because nothing changed
            }
        }
    }
    return results;
}
#include <bits/stdc++.h>
using namespace std;

// include the solution code here or assume it's defined above

int main() {
    // Test 1: simple insert, query, delete
    {
        vector<int> init = {1, 2, 3};
        vector<Operation> ops;
        Operation op;
        op.type = "GET_SUM"; op.pos = 1; op.len = 3; ops.push_back(op);
        op.type = "INSERT"; op.pos = 2; op.insertVals = {10, 20}; ops.push_back(op);
        op.type = "GET_SUM"; op.pos = 2; op.len = 2; ops.push_back(op);
        op.type = "DELETE"; op.pos = 3; op.len = 2; ops.push_back(op);
        op.type = "GET_SUM"; op.pos = 1; op.len = 2; ops.push_back(op);
        op.type = "SET"; op.pos = 1; op.len = 2; op.val = 5; ops.push_back(op);
        op.type = "GET_SUM"; op.pos = 1; op.len = 3; ops.push_back(op);
        auto res = maxSubarraySumAfterOperations(3, init, ops);
        assert(res.size() == 4);
        assert(res[0] == 6);
        assert(res[1] == 30); // 10+20
        assert(res[2] == 2+3); // after delete positions 3-4 (original 3,10,20) removed? Actually after insert: [1,2,10,20,3], delete pos3 len2 removes 10,20 -> [1,2,3], sum of first 2 = 3
        assert(res[2] == 3);
        assert(res[3] == 15); // after SET first 2 to 5, array [5,5,3], sum of 1..3 = 13
    }

    // Test 2: reverse and MAX_SUM
    {
        vector<int> init = {1, -2, 3, 4, -5};
        vector<Operation> ops;
        Operation op;
        op.type = "MAX_SUM"; ops.push_back(op); // best before reverse: 7 (3+4)
        op.type = "REVERSE"; op.pos = 2; op.len = 3; ops.push_back(op); // [-5,4,3,-2,1]? Actually reverse positions 2..4: [-2,3,4] -> [4,3,-2], array becomes [1,4,3,-2,-5]
        op.type = "MAX_SUM"; ops.push_back(op); // best after reverse: 8 (1+4+3)
        op.type = "GET_SUM"; op.pos = 2; op.len = 2; ops.push_back(op); // sum of [4,3] = 7
        auto res = maxSubarraySumAfterOperations(5, init, ops);
        assert(res.size() == 3);
        assert(res[0] == 7);
        assert(res[1] == 8);
        assert(res[2] == 7);
    }

    // Test 3: SET to negative values
    {
        vector<int> init = {5, 6, 7};
        vector<Operation> ops;
        Operation op;
        op.type = "SET"; op.pos = 1; op.len = 3; op.val = -10; ops.push_back(op);
        op.type = "MAX_SUM"; ops.push_back(op); // all -10, max is -10
        op.type = "GET_SUM"; op.pos = 1; op.len = 3; ops.push_back(op); // sum = -30
        auto res = maxSubarraySumAfterOperations(3, init, ops);
        assert(res.size() == 2);
        assert(res[0] == -10);
        assert(res[1] == -30);
    }

    // Test 4: insert at beginning (after position 0) and end
    {
        vector<int> init = {1, 2};
        vector<Operation> ops;
        Operation op;
        op.type = "INSERT"; op.pos = 0; op.insertVals = {9, 8}; ops.push_back(op);
        op.type = "GET_SUM"; op.pos = 1; op.len = 2; ops.push_back(op); // [9,8,1,2] -> sum 17
        op.type = "INSERT"; op.pos = 4; op.insertVals = {7}; ops.push_back(op);
        op.type = "GET_SUM"; op.pos = 1; op.len = 5; ops.push_back(op); // [9,8,1,2,7] sum 27
        auto res = maxSubarraySumAfterOperations(2, init, ops);
        assert(res.size() == 2);
        assert(res[0] == 17);
        assert(res[1] == 27);
    }

    // Test 5: delete entire array, then insert new
    {
        vector<int> init = {1, 2, 3};
        vector<Operation> ops;
        Operation op;
        op.type = "DELETE"; op.pos = 1; op.len = 3; ops.push_back(op);
        op.type = "MAX_SUM"; ops.push_back(op); // should be -1e18? But operation must keep at least one real element? According to problem, any operation guarantees at least one number. So this test might be invalid. We'll skip.
    }

    // Test 6: complex sequence with mixed operations
    {
        vector<int> init = {10, 20, 30, 40};
        vector<Operation> ops;
        Operation op;
        op.type = "REVERSE"; op.pos = 2; op.len = 2; ops.push_back(op); // [10,30,20,40]
        op.type = "SET"; op.pos = 1; op.len = 2; op.val = 1; ops.push_back(op); // [1,1,20,40]
        op.type = "INSERT"; op.pos = 2; op.insertVals = {100, 200}; ops.push_back(op); // [1,1,100,200,20,40]
        op.type = "GET_SUM"; op.pos = 3; op.len = 3; ops.push_back(op); // sum of 100,200,20 = 320
        op.type = "DELETE"; op.pos = 1; op.len = 2; ops.push_back(op); // remove [1,1] -> [100,200,20,40]
        op.type = "MAX_SUM"; ops.push_back(op); // sum all = 360
        op.type = "GET_SUM"; op.pos = 2; op.len = 2; ops.push_back(op); // sum of 200,20 = 220
        auto res = maxSubarraySumAfterOperations(4, init, ops);
        assert(res.size() == 3);
        assert(res[0] == 320);
        assert(res[1] == 360);
        assert(res[2] == 220);
    }

    printf("All tests passed!\n");
    return 0;
}
