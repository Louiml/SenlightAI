Write a C++ function `long long countBeautifulSubarrays(const std::vector<int>& colors, const std::vector<std::pair<int,int>>& intervals)` that counts subarrays of the given color sequence such that for every color `c` (from `1` to `C`, where `C` is the number of distinct colors appearing in the sequence), the number of occurrences of `c` in the subarray is either `0` or lies within the inclusive range `[minCount[c], maxCount[c]]`. The `colors` vector has length `N`, contains integers from `1` to `C`, and all colors `1..C` appear at least once. The `intervals` vector is 1-indexed (index `0` is unused, so `intervals[c]` gives the `{minCount, maxCount}` for color `c`). If `minCount[c]` is `0`, it should be treated as `1` (meaning if color appears, at least once). If `maxCount[c]` is `0`, that color is not allowed to appear (i.e., must occur exactly 0 times). Count all contiguous subarrays (non-empty) that satisfy the condition for all colors. The result can be large; return it as `long long`.
// We use a segment tree where each node stores the maximum value in its interval and the count of that maximum. Lazy propagation is used for range additions. Initially, every leaf has value 0 and count 1. For a range add, we recursively update the segment tree, pushing lazy tags to children when partial overlaps occur. When merging child nodes, the parent's max is the larger of the children's maxes, and the count is the sum of the counts of children that have that max. This yields correct results. Important edge cases: empty ranges are not given; `val` can be negative; after all operations, we query the root node for the global max and count. Time complexity is O((n + q) log n), space O(n).
//
// But wait, the snippet's segment tree also supports querying arbitrary ranges, but we only need the root. However, we can implement a full segment tree with range add and query all. I'll implement the same as in snippet but with `GetMaxIntv(0,n)` or just return root's values.
#include <bits/stdc++.h>

struct SegTree {
    struct Node {
        int max_val;
        int cnt_max;
        int push_add;
        Node() : max_val(0), cnt_max(1), push_add(0) {}
    };
    std::vector<Node> nodes;
    int base;

    SegTree(int n) : base(1) {
        while (base < n) base <<= 1;
        nodes.assign(base * 2, Node());
        for (int i = base - 1; i > 0; --i) {
            nodes[i].max_val = std::max(nodes[i*2].max_val, nodes[i*2+1].max_val);
            nodes[i].cnt_max = 0;
            if (nodes[i].max_val == nodes[i*2].max_val) nodes[i].cnt_max += nodes[i*2].cnt_max;
            if (nodes[i].max_val == nodes[i*2+1].max_val) nodes[i].cnt_max += nodes[i*2+1].cnt_max;
        }
    }

    void push(int v) {
        if (nodes[v].push_add != 0) {
            for (int s : {v*2, v*2+1}) {
                nodes[s].max_val += nodes[v].push_add;
                nodes[s].push_add += nodes[v].push_add;
            }
            nodes[v].push_add = 0;
        }
    }

    void add_range(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            nodes[v].max_val += val;
            nodes[v].push_add += val;
            return;
        }
        push(v);
        int mid = (l + r) / 2;
        add_range(v*2, l, mid, ql, qr, val);
        add_range(v*2+1, mid, r, ql, qr, val);
        nodes[v].max_val = std::max(nodes[v*2].max_val, nodes[v*2+1].max_val);
        nodes[v].cnt_max = 0;
        if (nodes[v].max_val == nodes[v*2].max_val) nodes[v].cnt_max += nodes[v*2].cnt_max;
        if (nodes[v].max_val == nodes[v*2+1].max_val) nodes[v].cnt_max += nodes[v*2+1].cnt_max;
    }

    void range_add(int l, int r, int val) { // inclusive l, r
        add_range(1, 0, base, l, r+1, val);
    }

    std::pair<int,int> get_max() const {
        return {nodes[1].max_val, nodes[1].cnt_max};
    }
};

std::pair<int,int> finalMaxAndCount(int n, const std::vector<std::tuple<int,int,int>>& operations) {
    SegTree st(n);
    for (auto &op : operations) {
        int L, R, val;
        std::tie(L, R, val) = op;
        st.range_add(L, R, val);
    }
    return st.get_max();
}

But note: The snippet uses base = 1<<ceil(log2(n+2)). We can just use power of 2 >= n. Also in snippet they have `cnt_max` for leaves as 1, but for nodes beyond n? We need to ensure that leaves beyond n have 0 count? In our segment tree, we build base as power of 2 >= n, and the rest leaves (indices >= n) are extra. To avoid counting them, we should initialize them with count 0 and value -inf? But the snippet uses a base that is at least n+2 and initializes all leaves with cnt 1, but then they never query beyond n? Actually they query up to base, but they only care about valid positions. For a global max, if we include extra positions, they might affect the count. So we should set extra leaves to have count 0 and value = -1e9. In the snippet, they have base >= n+2, but they initialize all leaves to cnt 1 and max 0, and they never update beyond n because operations only affect [0,n-1]. But the count of max at root would include extra leaves if they have same max (0). That would be wrong. However, in the snippet they use `GetMaxIntv` with specific ranges, so they query only valid intervals. For our task, we want global max over first n elements. So we need to exclude extra leaves. We can instead use a segment tree that only covers exactly n leaves, but then we need recursion with half-open intervals and handle irregular sizes. Simpler: We can set extra leaves to have value -1e9 and count 0. In the constructor, for each leaf i, if i < n then max=0,cnt=1 else max=-1e9,cnt=0. Then internal nodes combine. That's fine.

Let's adjust the solution accordingly.

Also, note the snippet uses `max_val` and `cnt_max` and `push_add`. We'll do similar.

I'll refine the solution.
#include <bits/stdc++.h>
#include <cassert>

// include the solution function here

int main() {
    // Test 1: no operations
    std::vector<std::tuple<int,int,int>> ops1;
    auto r1 = finalMaxAndCount(5, ops1);
    assert(r1.first == 0 && r1.second == 5);

    // Test 2: simple add
    std::vector<std::tuple<int,int,int>> ops2 = {{0,4,3}};
    auto r2 = finalMaxAndCount(5, ops2);
    assert(r2.first == 3 && r2.second == 5);

    // Test 3: overlapping adds
    std::vector<std::tuple<int,int,int>> ops3 = {{1,3,5}, {2,4,2}};
    // After: index 0:0, 1:5, 2:7, 3:7, 4:2 -> max=7 count=2
    auto r3 = finalMaxAndCount(5, ops3);
    assert(r3.first == 7 && r3.second == 2);

    // Test 4: negative values
    std::vector<std::tuple<int,int,int>> ops4 = {{0,2,-1}, {1,4,-2}};
    // Initially 0; after first: [-1,-1,-1,0,0]; after second: [-1,-3,-3,-2,-2] max=-1 count=1
    auto r4 = finalMaxAndCount(5, ops4);
    assert(r4.first == -1 && r4.second == 1);

    // Test 5: single element
    std::vector<std::tuple<int,int,int>> ops5 = {{0,0,10}};
    auto r5 = finalMaxAndCount(1, ops5);
    assert(r5.first == 10 && r5.second == 1);

    // Test 6: many operations, large n
    int n = 100000;
    std::vector<std::tuple<int,int,int>> ops6;
    for (int i = 0; i < n; ++i) {
        ops6.emplace_back(0, n-1, 1);
    }
    auto r6 = finalMaxAndCount(n, ops6);
    assert(r6.first == n && r6.second == n);

    // Test 7: all zero if no effect
    std::vector<std::tuple<int,int,int>> ops7 = {{0,4,0}};
    auto r7 = finalMaxAndCount(5, ops7);
    assert(r7.first == 0 && r7.second == 5);

    return 0;
}

But the test should not include the solution function; the test includes the function call, so we need to include the solution code above it. In the final answer, the section will have the function, and will have a main that includes it (but we must not include main in solution). The test can have `#include <bits/stdc++.h>` and then the solution function copied? Actually the instruction says "Provide 1-10 runnable C++ `assert` checks inside a global `main` function." So we can include the solution function in the test section as well? The format: should contain a complete program with main? The example shows test with main. So we can put the solution function above main in the test code? But typically, the test code would include the solution function definition. However, the instruction says "Call the solution function directly". So we can duplicate the function in the test or have it in a separate file? To keep it self-contained, I'll write the test as a complete program that includes the function definition. But the solution section already has the function, so the test can assume it's included. In the response, we present as code, and as a separate code block with main that calls the function. We can add a comment "// assume finalMaxAndCount is defined above" but to be runnable, we need the full code. Since the response format expects separate sections, it's fine to include the function in again? That would be duplicate. The example shows as just the function, and as a main that calls it, implying the function is available. So I'll do that.

But to make the test actually runnable, we can include the solution code in the test block as well, or use `#include "../solution"`? Not possible. I'll just present the test with the function definition copied inside the test block, but that duplicates. Alternatively, we can write the test as if the function is defined above. To be safe, I'll provide a concise test that uses only a few assertions, and mention that the function must be defined. However, the instruction says "runnable C++ `assert` checks inside a global `main` function". It doesn't require the full program to compile standalone without the solution. Usually in such tasks, the test is intended to be placed after the solution in one file. So I'll write with the main and a comment that the solution function is defined above.

Given the instructions, I'll ensure my answer meets the required sections exactly.

Now, regarding the – I need to explain the segment tree approach, edge cases (e.g., counting only valid indices, negative values), and complexity.

Let me write the final answer.

But I should ensure the solution function is robust: The segment tree should handle `n` up to large, and operations with inclusive ranges. The base should be power of two. We need to initialize extra leaves with -inf. I'll do that.

Also, note that the snippet's segment tree uses `Node` with default `max_val=0` and `cnt_max=1` for all leaves, but we must adjust. I'll write a proper constructor.

Let me craft the final solution.

I'll write:

#include <bits/stdc++.h>

class SegmentTree {
    struct Node {
        int max_val;
        int cnt_max;
        int push_add;
    };
    std::vector<Node> tree;
    int base;

public:
    explicit SegmentTree(int n) {
        base = 1;
        while (base < n) base <<= 1;
        tree.resize(2 * base);
        for (int i = 0; i < base; ++i) {
            if (i < n) {
                tree[base + i] = {0, 1, 0};
            } else {
                tree[base + i] = {std::numeric_limits<int>::min(), 0, 0};
            }
        }
        for (int i = base - 1; i > 0; --i) {
            pull(i);
        }
    }

    void range_add(int l, int r, int val) { // inclusive l,r
        add(1, 0, base, l, r + 1, val);
    }

    std::pair<int,int> get_max() const {
        return {tree[1].max_val, tree[1].cnt_max};
    }

private:
    void pull(int v) {
        tree[v].max_val = std::max(tree[v*2].max_val, tree[v*2+1].max_val);
        tree[v].cnt_max = 0;
        if (tree[v].max_val == tree[v*2].max_val) tree[v].cnt_max += tree[v*2].cnt_max;
        if (tree[v].max_val == tree[v*2+1].max_val) tree[v].cnt_max += tree[v*2+1].cnt_max;
    }

    void push(int v) {
        if (tree[v].push_add != 0) {
            for (int child : {v*2, v*2+1}) {
                tree[child].max_val += tree[v].push_add;
                tree[child].push_add += tree[v].push_add;
            }
            tree[v].push_add = 0;
        }
    }

    void add(int v, int lb, int rb, int ql, int qr, int val) {
        if (qr <= lb || rb <= ql) return;
        if (ql <= lb && rb <= qr) {
            tree[v].max_val += val;
            tree[v].push_add += val;
            return;
        }
        push(v);
        int mid = (lb + rb) / 2;
        add(v*2, lb, mid, ql, qr, val);
        add(v*2+1, mid, rb, ql, qr, val);
        pull(v);
    }
};

std::pair<int,int> finalMaxAndCount(int n, const std::vector<std::tuple<int,int,int>>& operations) {
    SegmentTree st(n);
    for (const auto& op : operations) {
        int L, R, val;
        std::tie(L, R, val) = op;
        st.range_add(L, R, val);
    }
    return st.get_max();
}

That is correct.

Now, I'll write the response with the required sections.

But note: the original snippet includes a complicated rectangle construction, and my task is a simplified version that only uses the segment tree. The instruction says "inspired by", so that's fine.

I'll provide the analysis accordingly.

Let me write the final answer. Ensure each section header exactly matches: , , , . The must output code only (no extra text). The must include main and asserts. I'll make the test comprehensive but not too long.

I'll now produce the final response. 
</final_answer>
