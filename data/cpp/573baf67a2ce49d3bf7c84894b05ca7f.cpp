Implement a C++ function `performSequenceOperations(int n, const std::vector<int>& initial, const std::vector<Operation>& ops)` that processes a sequence of integers using a balanced binary tree (implicit treap) supporting the following operations: insert (a block of numbers after a given position), delete a contiguous segment, assign a common value to a segment, reverse a segment, query the sum of a segment, and query the maximum subarray sum of the entire sequence. The function should return a `std::vector<int>` containing the results of all sum queries (in order) followed by the final maximum subarray sum at the end (if there is at least one operation, otherwise return an empty vector). Define the `Operation` struct with fields: `type` (an integer where 1=insert, 2=delete, 3=assign, 4=reverse, 5=sum, 6=maxSum), `pos` (1-based position for operations that need it), `tot` (length of segment), `value` (used for insert data or assign value), and `data` (a vector of integers for insert operations). All positions are 1-based and refer to the current sequence. For insert, `pos` is the number of elements to skip (0 means insert at beginning, `currentSize` means append at end); for delete/sum/assign/reverse, the segment is from `pos` to `pos+tot-1` (inclusive). Assumptions: initial sequence non-empty, operations are valid, no operation leaves the sequence empty, and at most one `maxSum` operation is present (it can be anywhere). The implementation must handle up to \(10^5\) elements and \(10^5\) operations efficiently.
The core is an implicit treap (randomized BST) where each node stores a value and maintains subtree aggregate information: `size` (number of nodes), `sum` (sum of values), `leftMax` (maximum prefix sum), `rightMax` (maximum suffix sum), and `maxSum` (maximum subarray sum). Each node also has lazy propagation flags for assignment (`modifyTag` and `modifyVal`) and reversal (`reverseTag`). The `update` function recomputes aggregates from children and the current node; careful handling is needed for missing children (treat as neutral: size 0, sum 0, leftMax/rightMax/maxSum = -infinity for empty, but for the `maxSum` we must consider that an empty segment is allowed, so for missing children we treat them as 0-length with sum 0 but for maxSum we don't add from empty; equivalently, we use sentinel values). The `split` function splits a tree into two by rank (number of nodes in left part) while pushing down lazy tags. The `merge` function combines two trees respecting heap priorities. All operations are broken into `split`/`merge` pairs: for insert, split at `pos`, build a new treap from given numbers using recursive building and merging, then merge everything back; for delete, split out the segment and discard it; for assign/reverse, split out the segment, apply the operation to its root (which lazy-propagates), then merge back; for sum, split out the segment and read its `sum`; for maxSum, read the root's `maxSum`. Because each split/merge visits O(log n) nodes, and each operation does a constant number of them, total time is O((n + m) log n) building and each operation O(log n). Space is O(n + m log n) for nodes and recursion, but effectively O(n + m) since treap size grows with insertions. Edge cases: empty segments (should not occur), single-node trees, when both children missing, when only one child exists, and when values are all negative (maximum subarray must be the maximum single element, not 0). The `maxSum` for empty segment is defined as -infinity; for the whole tree, we compute max(0, leftMax)+val+max(0, rightMax) in the crossing case, but we must ensure `maxSum` is never 0 unless there is a positive element; we use negative infinity for empty children's `maxSum` and `leftMax`/`rightMax` but also handle the case of a single node correctly. In `update`, for missing children, treat their `size` as 0, `sum` as 0, `leftMax`/`rightMax` as -INF (but for the combination, we use max(0, child's leftMax) which becomes 0), and `maxSum` as -INF. But careful: for the overall `maxSum`, we take max of left's maxSum, right's maxSum, and crossing sum; if a child is missing, its maxSum is -INF so it is ignored. The `modify` function sets all values in the subtree, recomputes sum, and sets leftMax/rightMax/maxSum: if value > 0, all three are value*size; if value <= 0, all three are value (since the best subarray is the single maximum element). The `reverse` function swaps left/right children and swaps leftMax/rightMax, sets reverseTag. PushDown must apply modify before reverse to avoid ordering issues (the provided code does modify first). For the test, implement the function exactly as described and test with sample operations.
#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int type;          // 1=insert, 2=delete, 3=assign, 4=reverse, 5=sum, 6=maxSum
    int pos;           // 1-based position (for insert: number of elements before)
    int tot;           // length of segment (for delete, assign, reverse, sum)
    int value;         // for assign: value to set; for insert: not used
    vector<int> data;  // for insert: numbers to insert
};

struct Node {
    int val, priority, size;
    Node *leftSon, *rightSon;
    bool reverseTag, modifyTag;
    int modifyVal;
    int sum, leftMax, rightMax, maxSum;

    Node(int _val) : val(_val), priority(rand()), size(1),
                     leftSon(nullptr), rightSon(nullptr),
                     reverseTag(false), modifyTag(false) {
        updateInternal();
    }

    void updateInternal() {
        // Update this node's aggregates; assumes children are already up-to-date
        int leftSize = leftSon ? leftSon->size : 0;
        int rightSize = rightSon ? rightSon->size : 0;
        size = 1 + leftSize + rightSize;

        sum = val + (leftSon ? leftSon->sum : 0) + (rightSon ? rightSon->sum : 0);

        // Compute prefix max (leftMax) globally
        leftMax = val;
        if (leftSon) leftMax = max(leftMax, leftSon->sum + val);
        if (rightSon) leftMax = max(leftMax, leftSon->sum + val + rightSon->leftMax);
        // Also consider if leftSon exists and its leftMax
        if (leftSon) leftMax = max(leftMax, leftSon->leftMax);

        // Compute suffix max (rightMax) globally
        rightMax = val;
        if (rightSon) rightMax = max(rightMax, rightSon->sum + val);
        if (leftSon) rightMax = max(rightMax, rightSon->sum + val + leftSon->rightMax);
        if (rightSon) rightMax = max(rightMax, rightSon->rightMax);

        // Compute max subarray sum
        maxSum = val;
        if (leftSon) maxSum = max(maxSum, leftSon->maxSum);
        if (rightSon) maxSum = max(maxSum, rightSon->maxSum);
        int crossing = (leftSon ? max(0, leftSon->rightMax) : 0) + val +
                       (rightSon ? max(0, rightSon->leftMax) : 0);
        maxSum = max(maxSum, crossing);
    }

    void applyReverse() {
        reverseTag = !reverseTag;
        swap(leftSon, rightSon);
        swap(leftMax, rightMax);
    }

    void applyModify(int newVal) {
        modifyTag = true;
        modifyVal = newVal;
        val = newVal;
        sum = size * newVal;
        if (newVal > 0) {
            leftMax = rightMax = maxSum = newVal * size;
        } else {
            leftMax = rightMax = maxSum = newVal;
        }
    }

    void pushDown() {
        if (modifyTag) {
            modifyTag = false;
            if (leftSon) leftSon->applyModify(modifyVal);
            if (rightSon) rightSon->applyModify(modifyVal);
        }
        if (reverseTag) {
            reverseTag = false;
            if (leftSon) leftSon->applyReverse();
            if (rightSon) rightSon->applyReverse();
        }
    }
};

pair<Node*, Node*> split(Node* cur, int rank) {
    if (!cur) return {nullptr, nullptr};
    if (rank == 0) return {nullptr, cur};
    int leftSize = cur->leftSon ? cur->leftSon->size : 0;
    cur->pushDown();
    if (rank <= leftSize) {
        Node *l, *r;
        tie(l, r) = split(cur->leftSon, rank);
        cur->leftSon = r;
        cur->updateInternal();
        return {l, cur};
    } else {
        Node *l, *r;
        tie(l, r) = split(cur->rightSon, rank - leftSize - 1);
        cur->rightSon = l;
        cur->updateInternal();
        return {cur, r};
    }
}

Node* merge(Node* u, Node* v) {
    if (!u) return v;
    if (!v) return u;
    if (u->priority <= v->priority) {
        v->pushDown();
        v->leftSon = merge(u, v->leftSon);
        v->updateInternal();
        return v;
    } else {
        u->pushDown();
        u->rightSon = merge(u->rightSon, v);
        u->updateInternal();
        return u;
    }
}

Node* build(const vector<int>& data, int l, int r) {
    if (l > r) return nullptr;
    int mid = (l + r) / 2;
    return merge(build(data, l, mid - 1), merge(new Node(data[mid]), build(data, mid + 1, r)));
}

void clearTree(Node*& cur) {
    if (!cur) return;
    clearTree(cur->leftSon);
    clearTree(cur->rightSon);
    delete cur;
    cur = nullptr;
}

vector<int> performSequenceOperations(int n, const vector<int>& initial,
                                     const vector<Operation>& ops) {
    srand(time(nullptr));
    Node* root = build(initial, 0, n - 1);
    vector<int> results;

    for (const auto& op : ops) {
        if (op.type == 1) { // insert
            Node* l, *r;
            tie(l, r) = split(root, op.pos);
            Node* newPart = build(op.data, 0, (int)op.data.size() - 1);
            root = merge(merge(l, newPart), r);
        } else if (op.type == 2) { // delete
            Node* l, *mid, *r;
            tie(l, mid) = split(root, op.pos - 1);
            tie(mid, r) = split(mid, op.tot);
            clearTree(mid);
            root = merge(l, r);
        } else if (op.type == 3) { // assign
            Node* l, *mid, *r;
            tie(l, mid) = split(root, op.pos - 1);
            tie(mid, r) = split(mid, op.tot);
            mid->applyModify(op.value);
            root = merge(merge(l, mid), r);
        } else if (op.type == 4) { // reverse
            Node* l, *mid, *r;
            tie(l, mid) = split(root, op.pos - 1);
            tie(mid, r) = split(mid, op.tot);
            mid->applyReverse();
            root = merge(merge(l, mid), r);
        } else if (op.type == 5) { // sum
            Node* l, *mid, *r;
            tie(l, mid) = split(root, op.pos - 1);
            tie(mid, r) = split(mid, op.tot);
            results.push_back(mid->sum);
            root = merge(merge(l, mid), r);
        } else if (op.type == 6) { // maxSum
            results.push_back(root->maxSum);
        }
    }
    // Optionally cleanup remaining tree
    clearTree(root);
    return results;
}
#include <bits/stdc++.h>
using namespace std;
// Assume the solution function and Operation struct are defined above.
int main() {
    // Test 1: Simple sequence, insert and sum
    {
        vector<int> init = {1, 2, 3};
        vector<Operation> ops;
        ops.push_back({1, 1, 0, 0, {4}}); // insert 4 after position 1 -> [1,4,2,3]
        ops.push_back({5, 2, 3, 0, {}});  // sum positions 2..4 -> 4+2+3=9
        auto res = performSequenceOperations(3, init, ops);
        assert(res.size() == 1 && res[0] == 9);
    }
    // Test 2: Reverse and maxSum
    {
        vector<int> init = {1, -2, 3, -4};
        vector<Operation> ops;
        ops.push_back({4, 2, 2, 0, {}}); // reverse positions 2..3 -> [1,3,-2,-4]
        ops.push_back({6, 0, 0, 0, {}}); // maxSum = 4 (from 1+3)
        auto res = performSequenceOperations(4, init, ops);
        assert(res.size() == 1 && res[0] == 4);
    }
    // Test 3: Assign and maxSum with all negative
    {
        vector<int> init = {5, -1, 3};
        vector<Operation> ops;
        ops.push_back({3, 1, 3, -2, {}}); // assign all to -2 -> [-2,-2,-2]
        ops.push_back({6, 0, 0, 0, {}});  // maxSum = -2
        auto res = performSequenceOperations(3, init, ops);
        assert(res.size() == 1 && res[0] == -2);
    }
    // Test 4: Delete and sum
    {
        vector<int> init = {10, 20, 30, 40, 50};
        vector<Operation> ops;
        ops.push_back({2, 2, 2, 0, {}}); // delete positions 2..3 -> [10,40,50]
        ops.push_back({5, 1, 3, 0, {}}); // sum all = 100
        auto res = performSequenceOperations(5, init, ops);
        assert(res.size() == 1 && res[0] == 100);
    }
    // Test 5: Multiple operations, mixed
    {
        vector<int> init = {1, -2, 3, -4};
        vector<Operation> ops;
        ops.push_back({5, 1, 4, 0, {}});      // sum all = -2
        ops.push_back({4, 1, 4, 0, {}});      // reverse all -> [-4,3,-2,1]
        ops.push_back({5, 1, 4, 0, {}});      // sum all still = -2
        ops.push_back({1, 4, 0, 0, {5, 6}});  // insert after 4 -> [-4,3,-2,1,5,6]
        ops.push_back({5, 4, 3, 0, {}});      // sum positions 4..6 = 1+5+6=12
        ops.push_back({6, 0, 0, 0, {}});      // maxSum = 12 (from 1+5+6)
        auto res = performSequenceOperations(4, init, ops);
        assert(res.size() == 3 && res[0] == -2 && res[1] == -2 && res[2] == 12);
    }
    // Test 6: Insert at beginning and end, then maxSum
    {
        vector<int> init = {2, -5};
        vector<Operation> ops;
        ops.push_back({1, 0, 0, 0, {10, 20}}); // insert at beginning -> [10,20,2,-5]
        ops.push_back({1, 4, 0, 0, {30}});    // insert after 4 elements -> [10,20,2,-5,30]
        ops.push_back({6, 0, 0, 0, {}});      // maxSum = 30+2+20+10=62? actually max is 10+20+2+ -5+30=57? Let's compute: 10+20+2=32, 2+(-5)+30=27, 30 alone=30, 20+2=22, 10+20+2=32, so max is 32. Wait 10+20+2=32, 20+2=22, -5+30=25, 30=30, so max 32. But 10+20+2-5+30=57? Let's compute carefully: sequence [10,20,2,-5,30], max subarray sum is 10+20+2=32, or 2-5+30=27, or 20+2-5+30=47, or 2-5=-3, so max is actually 20+2-5+30=47? Wait 20+2-5+30=47, 10+20+2=32, 20+2=22, -5+30=25, 30=30, so 47 is the max. Let's recalc: 20+2-5+30 = 47; 10+20+2-5+30 = 57, yes that sequence is contiguous and sum 57. So max=57.
        auto res = performSequenceOperations(2, init, ops);
        assert(res.size() == 1 && res[0] == 57);
    }
    // Test 7: Assign after reverse
    {
        vector<int> init = {1,2,3,4,5};
        vector<Operation> ops;
        ops.push_back({4, 2, 3, 0, {}}); // reverse positions 2..4 -> [1,4,3,2,5]
        ops.push_back({3, 1, 5, 0, {}}); // assign all 0 -> [0,0,0,0,0]
        ops.push_back({6, 0, 0, 0, {}}); // maxSum = 0
        auto res = performSequenceOperations(5, init, ops);
        assert(res.size() == 1 && res[0] == 0);
    }
    // Test 8: Sum after multiple splits
    {
        vector<int> init = {1,2,3,4,5,6};
        vector<Operation> ops;
        ops.push_back({5, 2, 4, 0, {}}); // sum 2..5 = 2+3+4+5=14
        ops.push_back({1, 5, 0, 0, {100}}); // insert 100 after 5 elements -> [1,2,3,4,5,100,6]
        ops.push_back({5, 6, 2, 0, {}}); // sum positions 6..7 = 100+6=106
        auto res = performSequenceOperations(6, init, ops);
        assert(res.size() == 2 && res[0] == 14 && res[1] == 106);
    }
    // Test 9: Delete all but one, then maxSum
    {
        vector<int> init = {7, -3, 9};
        vector<Operation> ops;
        ops.push_back({2, 1, 2, 0, {}}); // delete first two -> [9]
        ops.push_back({6, 0, 0, 0, {}}); // maxSum = 9
        auto res = performSequenceOperations(3, init, ops);
        assert(res.size() == 1 && res[0] == 9);
    }
    // Test 10: Complex combination
    {
        vector<int> init = { -5, 2, -1, 3, -2 };
        vector<Operation> ops;
        ops.push_back({1, 2, 0, 0, {4, -1, 5}}); // insert after 2 -> [-5,2,4,-1,5,-1,3,-2]
        ops.push_back({4, 3, 4, 0, {}}); // reverse positions 3..6 -> [-5,2,5,-1,4,-1,3,-2]
        ops.push_back({3, 2, 4, 10, {}}); // assign positions 2..5 to 10 -> [-5,10,10,10,10,-1,3,-2]
        ops.push_back({5, 1, 8, 0, {}}); // sum all = 35
        ops.push_back({6, 0, 0, 0, {}}); // maxSum = 35 (all positive except -5 and -1 -2, but contiguous sum 10+10+10+10-1+3-2 = 40? Let's compute: sequence: -5,10,10,10,10,-1,3,-2. Max subarray: 10+10+10+10 = 40, or 10+10+10+10-1+3=42, or -5+10+10+10+10-1+3=37, so max is 42. Wait 10+10+10+10-1+3 = 42, yes. So maxSum = 42.
        auto res = performSequenceOperations(5, init, ops);
        assert(res.size() == 2 && res[0] == 35 && res[1] == 42);
    }
    return 0;
}
