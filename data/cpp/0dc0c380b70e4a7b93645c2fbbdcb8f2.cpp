Write a C++ function that maintains a dynamic sequence of integers supporting two operations: range reversal (reversing the order of elements between 1-based indices `a` and `b`) and range sum query (summing elements between 1-based indices `a` and `b`). The function should process a list of initial `n` integers followed by `m` operations, where operation type `1 a b` indicates reversal of the subarray from `a` to `b`, and operation type `2 a b` requests the sum of elements in that range. The function must return a `std::vector<long long>` containing the outputs of all sum queries in the order they appear. Input is provided in the following format: first line contains `n` and `m`, second line contains `n` integers (the initial sequence), followed by `m` lines each containing an operation. The sequence indices are 1-based, and all intermediate arithmetic must use 64-bit integers.

The optimal solution uses an implicit treap (a randomized binary search tree) where each node stores a value, a subtree size, a subtree sum, a lazy reversal flag, and random priority for balancing. The treap supports split by position (cutting into left `[0,k)` and right `[k,size)` parts) and merge in `O(log n)` expected time due to random priorities. For reversal, split the treap into three parts: `A` (before `a`), `B` (range `[a,b]`), and `C` (after `b`), then toggle the lazy flag on `B`, and merge back. For sum query, split into `A`, `B`, `C` similarly, take `sum(B)`, then merge back. Since splitting and merging preserve subtree sums and lazy propagation is handled during pushes, both operations run in `O(log n)` expected time. The total complexity is `O((n+m) log n)` time and `O(n)` space. Edge cases include `a == b` (reversal has no effect, sum is the single value), and range covering the whole sequence; the treap naturally handles these. Lazy propagation must occur before any split or merge to ensure children are correct when accessing sizes and sums. The final answer is a vector of all sum results in order.

#include <bits/stdc++.h>
using namespace std;

struct TreapNode {
    TreapNode *left, *right;
    long long val, sum;
    int priority, size;
    bool rev;
    
    TreapNode(long long v) : left(nullptr), right(nullptr), val(v), sum(v), 
                             priority(rand()), size(1), rev(false) {}
};

int getSize(TreapNode* t) {
    return t ? t->size : 0;
}

long long getSum(TreapNode* t) {
    return t ? t->sum : 0;
}

void push(TreapNode* t) {
    if (t && t->rev) {
        t->rev = false;
        swap(t->left, t->right);
        if (t->left) t->left->rev ^= true;
        if (t->right) t->right->rev ^= true;
    }
}

void pull(TreapNode* t) {
    if (t) {
        t->size = 1 + getSize(t->left) + getSize(t->right);
        t->sum = t->val + getSum(t->left) + getSum(t->right);
    }
}

void split(TreapNode* t, TreapNode*& left, TreapNode*& right, int k) {
    if (!t) {
        left = right = nullptr;
        return;
    }
    push(t);
    if (getSize(t->left) < k) {
        split(t->right, t->right, right, k - getSize(t->left) - 1);
        left = t;
    } else {
        split(t->left, left, t->left, k);
        right = t;
    }
    pull(t);
}

void merge(TreapNode*& t, TreapNode* left, TreapNode* right) {
    push(left);
    push(right);
    if (!left || !right) {
        t = left ? left : right;
    } else if (left->priority > right->priority) {
        merge(left->right, left->right, right);
        t = left;
    } else {
        merge(right->left, left, right->left);
        t = right;
    }
    pull(t);
}

vector<long long> processQueries(int n, int m, const vector<long long>& initial, 
                                 const vector<tuple<int,int,int>>& queries) {
    TreapNode* root = nullptr;
    for (long long x : initial) {
        TreapNode* newNode = new TreapNode(x);
        merge(root, root, newNode);
    }
    
    vector<long long> results;
    results.reserve(m);
    
    for (auto [type, a, b] : queries) {
        TreapNode *A, *B, *C;
        // Split into: A = [0, a-1], B = [a-1, b], C = [b, size)
        split(root, A, B, a - 1);
        split(B, B, C, b - a + 1);
        
        if (type == 1) {
            B->rev ^= true;
            merge(root, A, B);
            merge(root, root, C);
        } else {
            results.push_back(getSum(B));
            merge(root, A, B);
            merge(root, root, C);
        }
    }
    return results;
}

#include <bits/stdc++.h>
#include <cassert>

// Prototype of the solution function (copy from Solution section)
vector<long long> processQueries(int n, int m, const vector<long long>& initial, 
                                 const vector<tuple<int,int,int>>& queries);

int main() {
    // Test 1: Basic sum and reversal
    {
        vector<long long> init = {1, 2, 3, 4, 5};
        vector<tuple<int,int,int>> q = {
            {2, 1, 5},   // sum = 15
            {1, 2, 4},   // reverse [2,4] -> [1,4,3,2,5]
            {2, 2, 4}    // sum = 9
        };
        vector<long long> res = processQueries(5, 3, init, q);
        assert(res == vector<long long>({15, 9}));
    }
    
    // Test 2: Full reversal and empty range edge case (a == b)
    {
        vector<long long> init = {10, 20, 30};
        vector<tuple<int,int,int>> q = {
            {2, 2, 2},   // sum = 20
            {1, 1, 3},   // reverse all -> [30,20,10]
            {2, 1, 1},   // sum = 30
            {2, 3, 3}    // sum = 10
        };
        vector<long long> res = processQueries(3, 4, init, q);
        assert(res == vector<long long>({20, 30, 10}));
    }
    
    // Test 3: Non-trivial reversal followed by sum
    {
        vector<long long> init = {1, 2, 3, 4};
        vector<tuple<int,int,int>> q = {
            {1, 2, 3},   // [1,3,2,4]
            {2, 1, 4},   // sum = 10
            {1, 1, 2},   // [3,1,2,4]
            {2, 3, 4}    // sum = 6
        };
        vector<long long> res = processQueries(4, 4, init, q);
        assert(res == vector<long long>({10, 6}));
    }
    
    // Test 4: Single element with multiple queries
    {
        vector<long long> init = {7};
        vector<tuple<int,int,int>> q = {
            {2, 1, 1},   // 7
            {1, 1, 1},   // no effect
            {2, 1, 1}    // 7
        };
        vector<long long> res = processQueries(1, 3, init, q);
        assert(res == vector<long long>({7, 7}));
    }
    
    // Test 5: All equal elements and repeated reversals
    {
        vector<long long> init = {5, 5, 5, 5};
        vector<tuple<int,int,int>> q = {
            {1, 1, 4},
            {1, 2, 3},
            {2, 1, 4},   // 20
            {2, 2, 2}    // 5
        };
        vector<long long> res = processQueries(4, 4, init, q);
        assert(res == vector<long long>({20, 5}));
    }
    
    // Test 6: Large sequence with mixed operations
    {
        vector<long long> init = {1, 2, 3, 4, 5, 6, 7, 8};
        vector<tuple<int,int,int>> q = {
            {2, 1, 8},   // 36
            {1, 3, 6},   // [1,2,6,5,4,3,7,8]
            {2, 3, 6},   // 18
            {1, 1, 4},   // [5,6,2,1,4,3,7,8]
            {2, 1, 4},   // 14
            {2, 5, 8}    // 22
        };
        vector<long long> res = processQueries(8, 6, init, q);
        assert(res == vector<long long>({36, 18, 14, 22}));
    }
    
    // Test 7: Adjacent reversal and sum after many reversals
    {
        vector<long long> init = {1, 2, 3, 4, 5};
        vector<tuple<int,int,int>> q = {
            {1, 1, 2},   // [2,1,3,4,5]
            {1, 4, 5},   // [2,1,3,5,4]
            {2, 1, 5},   // 15
            {1, 2, 3},   // [2,3,1,5,4]
            {2, 2, 4}    // 9
        };
        vector<long long> res = processQueries(5, 5, init, q);
        assert(res == vector<long long>({15, 9}));
    }
    
    // Test 8: Query range covering only one element after reversal
    {
        vector<long long> init = {10, 20, 30, 40};
        vector<tuple<int,int,int>> q = {
            {1, 2, 4},   // [10,40,30,20]
            {2, 2, 2},   // 40
            {2, 4, 4},   // 20
            {1, 1, 3},   // [30,40,10,20]
            {2, 1, 2}    // 70
        };
        vector<long long> res = processQueries(4, 5, init, q);
        assert(res == vector<long long>({40, 20, 70}));
    }
    
    // Test 9: Empty query list
    {
        vector<long long> init = {1, 2, 3};
        vector<tuple<int,int,int>> q = {};
        vector<long long> res = processQueries(3, 0, init, q);
        assert(res.empty());
    }
    
    // Test 10: Stress-like small random test (deterministic)
    {
        vector<long long> init = {1, 2, 3, 4, 5};
        vector<long long> current = init;
        vector<tuple<int,int,int>> q;
        vector<long long> expected;
        // Manual simulation
        q.push_back({2, 1, 5}); expected.push_back(15);
        q.push_back({1, 2, 4}); reverse(current.begin()+1, current.begin()+4);
        q.push_back({2, 2, 4}); expected.push_back(accumulate(current.begin()+1, current.begin()+4, 0LL));
        q.push_back({1, 1, 3}); reverse(current.begin(), current.begin()+3);
        q.push_back({2, 1, 5}); expected.push_back(accumulate(current.begin(), current.end(), 0LL));
        
        vector<long long> res = processQueries(5, 5, init, q);
        assert(res == expected);
    }
    
    return 0;
}
