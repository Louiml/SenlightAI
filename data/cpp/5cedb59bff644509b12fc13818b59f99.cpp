/*
Implement a C++ function that simulates a subset of the implicit treap operations from the given code snippet. Specifically, write a function `treap_operations` that receives a vector of integers, followed by a vector of operation strings, and returns a vector of results for all query operations. The supported operations are: `insert pos val` (insert val at index pos, 0-indexed), `erase l r` (remove elements from index l to r inclusive), `query l r` (return sum of elements from l to r), `update l r val` (add val to all elements from l to r), `reverse l r` (reverse the subarray from l to r), `replace l r val` (set all elements from l to r to val), `cyclic_shift l r k` (right-rotate the subarray from l to r by k positions, if k is negative then left-rotate by |k|), and `get_val pos` (return value at index pos). The initial vector represents the starting array. For each query, `query` should return the sum, and `get_val` should return the value; all other operations produce no output. If an index is out of range for a query or get_val, return `-1` for queries or `INT_MIN` for get_val. All indices are 0-based and operations are guaranteed valid for insert and erase (insert at position equal to size is allowed; erase has valid range). The function must be self-contained, not relying on external libraries beyond standard C++.
*/

#include <bits/stdc++.h>
using namespace std;

// Helper to generate random priorities (xorshift)
static unsigned long long rng_state = 123456789;
static unsigned int rng() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return (unsigned int)(rng_state & 0xFFFFFFFF);
}

// Implicit treap node
struct TreapNode {
    long long val, sum, mx, mn;
    int sz, prior;
    long long lazy_add;
    bool has_repl, rev;
    long long repl_val;
    TreapNode *l, *r;

    TreapNode(long long v) : val(v), sum(v), mx(v), mn(v), sz(1), prior((int)rng()),
        lazy_add(0), has_repl(false), rev(false), repl_val(0), l(nullptr), r(nullptr) {}
};

int getSize(TreapNode* t) {
    return t ? t->sz : 0;
}

long long getSum(TreapNode* t) {
    return t ? t->sum : 0;
}

long long getMax(TreapNode* t) {
    return t ? t->mx : LLONG_MIN;
}

long long getMin(TreapNode* t) {
    return t ? t->mn : LLONG_MAX;
}

void push(TreapNode* t) {
    if (!t) return;
    if (t->has_repl) {
        t->val = t->repl_val;
        t->sum = t->repl_val * t->sz;
        t->mx = t->repl_val;
        t->mn = t->repl_val;
        if (t->l) {
            t->l->has_repl = true;
            t->l->repl_val = t->repl_val;
            // reset lazy_add for children? No, replace overrides add, but we must clear their add
            t->l->lazy_add = 0;
        }
        if (t->r) {
            t->r->has_repl = true;
            t->r->repl_val = t->repl_val;
            t->r->lazy_add = 0;
        }
        t->has_repl = false;
    }
    if (t->lazy_add != 0) {
        long long add = t->lazy_add;
        t->val += add;
        t->sum += add * t->sz;
        t->mx += add;
        t->mn += add;
        if (t->l) t->l->lazy_add += add;
        if (t->r) t->r->lazy_add += add;
        t->lazy_add = 0;
    }
    if (t->rev) {
        swap(t->l, t->r);
        if (t->l) t->l->rev ^= true;
        if (t->r) t->r->rev ^= true;
        t->rev = false;
    }
}

void pull(TreapNode* t) {
    if (!t) return;
    push(t);
    t->sz = 1 + getSize(t->l) + getSize(t->r);
    t->sum = t->val + getSum(t->l) + getSum(t->r);
    t->mx = max(t->val, max(getMax(t->l), getMax(t->r)));
    t->mn = min(t->val, min(getMin(t->l), getMin(t->r)));
}

void split(TreapNode* t, TreapNode*& l, TreapNode*& r, int k) {
    // split so that l has first k elements (0..k-1), r has rest
    if (!t) { l = r = nullptr; return; }
    push(t);
    int leftSize = getSize(t->l);
    if (k <= leftSize) {
        split(t->l, l, t->l, k);
        r = t;
    } else {
        split(t->r, t->r, r, k - leftSize - 1);
        l = t;
    }
    pull(t);
}

void merge(TreapNode*& t, TreapNode* l, TreapNode* r) {
    if (!l || !r) { t = l ? l : r; return; }
    if (l->prior > r->prior) {
        push(l);
        merge(l->r, l->r, r);
        t = l;
    } else {
        push(r);
        merge(r->l, l, r->l);
        t = r;
    }
    pull(t);
}

// Main function: given initial array and operations, return results for QUERY and GET_VAL
vector<long long> treap_operations(vector<int> initial, vector<string> ops) {
    TreapNode* root = nullptr;
    for (int v : initial) {
        TreapNode* node = new TreapNode(v);
        merge(root, root, node);
    }

    vector<long long> results;

    for (const string& op : ops) {
        istringstream iss(op);
        string cmd;
        iss >> cmd;
        if (cmd == "insert") {
            int pos, val;
            iss >> pos >> val;
            TreapNode* newNode = new TreapNode(val);
            TreapNode *l, *r;
            split(root, l, r, pos);
            TreapNode* mid;
            merge(mid, l, newNode);
            merge(root, mid, r);
        } else if (cmd == "erase") {
            int l, r;
            iss >> l >> r;
            TreapNode *a, *b, *c;
            split(root, a, b, l);
            split(b, b, c, r - l + 1);
            // b is the erased part, we ignore it (memory leak acceptable)
            merge(root, a, c);
        } else if (cmd == "query") {
            int l, r;
            iss >> l >> r;
            if (l < 0 || r >= getSize(root) || l > r) {
                results.push_back(-1);
                continue;
            }
            TreapNode *a, *b, *c;
            split(root, a, b, l);
            split(b, b, c, r - l + 1);
            results.push_back(getSum(b));
            merge(root, a, merge(a, b, c));
            // Actually we need to merge correctly: re-merge a, b, c
            TreapNode* temp;
            merge(temp, a, b);
            merge(root, temp, c);
        } else if (cmd == "update") {
            int l, r, val;
            iss >> l >> r >> val;
            TreapNode *a, *b, *c;
            split(root, a, b, l);
            split(b, b, c, r - l + 1);
            if (b) b->lazy_add += val;
            TreapNode* temp;
            merge(temp, a, b);
            merge(root, temp, c);
        } else if (cmd == "reverse") {
            int l, r;
            iss >> l >> r;
            TreapNode *a, *b, *c;
            split(root, a, b, l);
            split(b, b, c, r - l + 1);
            if (b) b->rev ^= true;
            TreapNode* temp;
            merge(temp, a, b);
            merge(root, temp, c);
        } else if (cmd == "replace") {
            int l, r, val;
            iss >> l >> r >> val;
            TreapNode *a, *b, *c;
            split(root, a, b, l);
            split(b, b, c, r - l + 1);
            if (b) {
                b->has_repl = true;
                b->repl_val = val;
                b->lazy_add = 0; // replace clears any pending adds
            }
            TreapNode* temp;
            merge(temp, a, b);
            merge(root, temp, c);
        } else if (cmd == "cyclic_shift") {
            int l, r, k;
            iss >> l >> r >> k;
            int len = r - l + 1;
            if (len <= 1) continue;
            if (k < 0) k = len - (abs(k) % len);
            k %= len;
            if (k == 0) continue;
            TreapNode *a, *b, *c;
            split(root, a, b, l);
            split(b, b, c, len);
            // Now b is the subarray
            int splitPoint = len - k; // put first (len-k) elements after k elements
            TreapNode *p1, *p2;
            split(b, p1, p2, splitPoint);
            // New b = p2 + p1
            TreapNode* newB;
            merge(newB, p2, p1);
            TreapNode* temp;
            merge(temp, a, newB);
            merge(root, temp, c);
        } else if (cmd == "get_val") {
            int pos;
            iss >> pos;
            if (pos < 0 || pos >= getSize(root)) {
                results.push_back(INT_MIN);
                continue;
            }
            // Query single element
            TreapNode *a, *b, *c;
            split(root, a, b, pos);
            split(b, b, c, 1);
            push(b);
            results.push_back(b->val);
            TreapNode* temp;
            merge(temp, a, b);
            merge(root, temp, c);
        }
    }

    return results;
}

#include <bits/stdc++.h>
using namespace std;

// Include the solution here (for brevity, declare the function)
vector<long long> treap_operations(vector<int> initial, vector<string> ops);

int main() {
    // Basic operations
    {
        vector<int> init = {10, 20, 30, 40, 50};
        vector<string> ops = {
            "query 0 2",       // 10+20+30 = 60
            "get_val 1",       // 20
            "update 1 3 5",    // add 5 to 20,30,40 -> 25,35,45
            "query 0 4",       // 10+25+35+45+50 = 165
            "replace 2 3 100", // set index 2,3 to 100 -> 10,25,100,100,50
            "query 2 4",       // 100+100+50 = 250
            "reverse 0 4",     // 50,100,100,25,10
            "query 0 4",       // 50+100+100+25+10 = 285
            "cyclic_shift 0 4 2", // right shift by 2 -> 25,10,50,100,100
            "query 3 4",       // 100+100 = 200
            "get_val 0",       // 25
            "erase 1 2",       // remove 10,50 -> 25,100,100
            "query 0 2"        // 225
        };
        auto res = treap_operations(init, ops);
        assert(res[0] == 60);
        assert(res[1] == 20);
        assert(res[2] == 165);
        assert(res[3] == 250);
        assert(res[4] == 285);
        assert(res[5] == 200);
        assert(res[6] == 25);
        assert(res[7] == 225);
    }

    // Edge cases: empty initial, insert at end, out-of-range query
    {
        vector<int> init = {};
        vector<string> ops = {
            "insert 0 42",
            "insert 1 7",
            "insert 0 99",
            "get_val 0",
            "get_val 1",
            "get_val 2",
            "query 0 0",
            "query 0 2",
            "query 3 4",       // out of range
            "get_val 5"        // out of range
        };
        auto res = treap_operations(init, ops);
        assert(res[0] == 99);
        assert(res[1] == 42);
        assert(res[2] == 7);
        assert(res[3] == 99);
        assert(res[4] == 148);
        assert(res[5] == -1);
        assert(res[6] == INT_MIN);
    }

    // Negative cyclic shift (left shift)
    {
        vector<int> init = {1, 2, 3, 4, 5};
        vector<string> ops = {
            "cyclic_shift 0 4 -2", // left shift by 2 -> 3,4,5,1,2
            "query 0 4",           // 15
            "get_val 0"            // 3
        };
        auto res = treap_operations(init, ops);
        assert(res[0] == 15);
        assert(res[1] == 3);
    }

    // Combined lazy operations: update then replace should discard update
    {
        vector<int> init = {10, 20, 30};
        vector<string> ops = {
            "update 0 2 100",   // 110,120,130
            "replace 0 2 5",    // 5,5,5 (add discarded)
            "query 0 2",        // 15
            "get_val 1"         // 5
        };
        auto res = treap_operations(init, ops);
        assert(res[0] == 15);
        assert(res[1] == 5);
    }

    // Reverse and then query, ensure lazy propagation works
    {
        vector<int> init = {1, 2, 3, 4};
        vector<string> ops = {
            "reverse 1 2",     // 1,3,2,4
            "get_val 1",       // 3
            "get_val 2",       // 2
            "reverse 0 3",     // 4,2,3,1
            "query 0 3"        // 10
        };
        auto res = treap_operations(init, ops);
        assert(res[0] == 3);
        assert(res[1] == 2);
        assert(res[2] == 10);
    }

    return 0;
}

// The core challenge is to implement an implicit treap (a randomized binary search tree where the inorder traversal represents the array order) with lazy propagation for range additions, range replacements, and range reversals. Each node stores its value `val`, subtree size `sz`, sum `sum`, max `mx`, min `mn`, and lazy tags: `lazy` for addition, `repl_flag`/`repl` for replacement, and `rev` for reverse. The `split` operation divides the treap into two parts: the first `k+1` nodes (indices 0..k) and the rest, using the size of left subtree. The `merge` operation combines two treaps based on random priorities. Lazy tags are pushed down during splits, merges, and queries to ensure correctness. For range operations, we split the treap into three parts: left, middle (the target range), and right; apply the operation to the middle; then merge back. For `cyclic_shift`, we split the middle into two parts and swap them. For `get_pos` we could use parent pointers, but since we only need `get_val`, we directly query the sum of a single-element range (which equals the value after lazy propagation). Edge cases include empty ranges (should not occur for valid inputs), single-element ranges, and updates that need to be composed correctly (e.g., replace after update, or reverse after replace). Time complexity is O(log n) per operation on average due to treap height being logarithmic with high probability; space is O(n). The solution uses random priorities via a simple xorshift RNG to avoid platform-dependence.
