/*
Write a C++ function `int compressedValue(const std::vector<int>& heights)` that takes a list of building heights (positive integers) and returns the sum of values obtained by applying a sequence of interval compression operations. The operations are: for a given range `[left, right]` (1-indexed inclusive), all distinct heights currently present among positions within that range are replaced by a single representative height equal to `(left+right)/2` (integer division). The function should return the total value (sum of heights) after processing all positions, where each position's final height is the result of all compression operations that affected it. The input heights are guaranteed to be between 1 and 10^9, and positions are 1-indexed.

For example, if heights = [5, 3, 1] and the only operation is on range [1,3] (left=1, right=3), then all three heights (5,3,1) are distinct, so they all become 2 (since (1+3)/2 = 2), and the sum becomes 6. If we had heights [2,2,2] and apply the same operation, since all heights are already equal, no change occurs, and sum remains 6.
*/
#include <bits/stdc++.h>
using namespace std;

// Structure for a height node that may be redirected.
struct HeightNode;
using NodePtr = shared_ptr<HeightNode>;

struct HeightNode {
    // Either holds an explicit value or a redirect to another node.
    variant<int, NodePtr> data;

    explicit HeightNode(int v) : data(v) {}
    HeightNode(NodePtr other) : data(move(other)) {}
};

// Find the root node for a given node, with path compression.
int getValue(const NodePtr& node) {
    if (holds_alternative<int>(node->data)) {
        return get<int>(node->data);
    }
    NodePtr& redirected = get<NodePtr>(node->data);
    redirected = make_shared<HeightNode>(getValue(redirected)); // compress path
    return getValue(redirected);
}

// Function to compute sum after applying compression operations.
int compressedValue(const vector<int>& heights, const vector<pair<int,int>>& ranges) {
    int n = heights.size();
    vector<NodePtr> position(n);
    set<NodePtr, function<bool(const NodePtr&, const NodePtr&)>> active(
        [](const NodePtr& a, const NodePtr& b) {
            return get<int>(a->data) < get<int>(b->data);
        }
    );

    // Initialize nodes for each height.
    for (int i = 0; i < n; ++i) {
        NodePtr node = make_shared<HeightNode>(heights[i]);
        if (active.find(node) == active.end()) {
            active.insert(node);
        }
        position[i] = node;
    }

    // Process each range compression.
    for (auto [left, right] : ranges) {
        // left and right are 1-indexed inclusive.
        int targetValue = (left + right) / 2;

        // Create a dummy node for lower bound search.
        NodePtr leftDummy = make_shared<HeightNode>(left);
        auto it = active.lower_bound(leftDummy);

        vector<NodePtr> toRemove;
        while (it != active.end() && get<int>((*it)->data) <= right) {
            toRemove.push_back(*it);
            ++it;
        }

        NodePtr targetNode = make_shared<HeightNode>(targetValue);
        if (active.find(targetNode) == active.end()) {
            // target might already be in set; if not, we'll insert later.
        }

        for (NodePtr oldNode : toRemove) {
            // Redirect old node to target node.
            oldNode->data = targetNode;
            active.erase(oldNode);
        }

        // Insert target node if not already present.
        active.insert(targetNode);
    }

    // Compute sum of final values.
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += getValue(position[i]);
    }
    return (int)sum;
}
#include <cassert>
#include <vector>
using namespace std;

// Include the solution function here or link it.

int main() {
    // Test 1: simple range covers all distinct heights.
    {
        vector<int> h = {5, 3, 1};
        vector<pair<int,int>> ranges = {{1, 3}};
        assert(compressedValue(h, ranges) == 6);
    }

    // Test 2: all heights equal, compression does nothing.
    {
        vector<int> h = {2, 2, 2};
        vector<pair<int,int>> ranges = {{1, 3}};
        assert(compressedValue(h, ranges) == 6);
    }

    // Test 3: multiple ranges, overlapping.
    {
        vector<int> h = {10, 20, 30};
        vector<pair<int,int>> ranges = {{1, 2}, {2, 3}};
        // After first [1,2]: heights become 1,1,30 (since (1+2)/2=1)
        // After second [2,3]: range contains 1 and 30, both <=3, so both become (2+3)/2=2
        // Final: 2,2,2 sum=6
        assert(compressedValue(h, ranges) == 6);
    }

    // Test 4: no compression ranges.
    {
        vector<int> h = {4, 5, 6};
        vector<pair<int,int>> ranges = {};
        assert(compressedValue(h, ranges) == 15);
    }

    // Test 5: range doesn't cover any existing height.
    {
        vector<int> h = {1, 2, 3};
        vector<pair<int,int>> ranges = {{5, 6}}; // no height in [5,6]
        assert(compressedValue(h, ranges) == 6);
    }

    // Test 6: compression to a value already present.
    {
        vector<int> h = {1, 3, 5};
        vector<pair<int,int>> ranges = {{1, 3}}; // (1+3)/2=2, heights 1 and 3 become 2
        // Final: 2,2,5 sum=9
        assert(compressedValue(h, ranges) == 9);
    }

    // Test 7: single element.
    {
        vector<int> h = {42};
        vector<pair<int,int>> ranges = {{1, 1}}; // (1+1)/2=1
        assert(compressedValue(h, ranges) == 1);
    }

    // Test 8: larger test.
    {
        vector<int> h = {100, 200, 300, 400};
        vector<pair<int,int>> ranges = {{1, 4}, {2, 3}};
        // First [1,4]: all distinct, target=2, all become 2 -> sum=8
        // Second [2,3]: range [2,3] covers height 2, target=2, no change
        assert(compressedValue(h, ranges) == 8);
    }

    // Test 9: range with duplicate heights in positions but same node.
    {
        vector<int> h = {7, 7, 8};
        vector<pair<int,int>> ranges = {{1, 2}}; // (1+2)/2=1, only height 7 is in range, becomes 1
        // Final: 1,1,8 sum=10
        assert(compressedValue(h, ranges) == 10);
    }

    // Test 10: sequential compressions building.
    {
        vector<int> h = {3, 5, 7, 9};
        vector<pair<int,int>> ranges = {{1, 4}, {1, 2}, {3, 4}};
        // First [1,4]: all distinct, target=2 -> all become 2, sum=8
        // Second [1,2]: covers 2, target=1 -> all become 1, sum=4
        // Third [3,4]: covers 1, target=3 -> all become 3, sum=12
        assert(compressedValue(h, ranges) == 12);
    }

    return 0;
}
// The key challenge is efficiently applying range compression operations on a set of distinct height values. The original snippet uses a union-find-like structure where each unique height is represented as a node that can be redirected (merged) to another node. For each compression range, we need to find all distinct height values in that range and merge them into a single representative.
//
// We can model this with a `std::set` of active distinct heights (the "explicits"). For each position, we store a pointer to its current height node. When we perform a compression on `[left, right]`, we iterate over all active heights that fall strictly within `(left-1, right+1)` (i.e., all values `v` with `left <= v <= right`). For each such value, we redirect its node to a new node representing the target value (the midpoint), and remove it from the set. Finally, we insert the target value if it wasn't already in the set. This ensures that after compression, all positions whose heights were in that range now point to the same node with value `(left+right)/2`.
//
// Important edge cases: if no distinct heights fall in the range, the operation does nothing. When we redirect a node, we must also update all positions that pointed to it — but since we use shared pointers and the positions store the node directly, redirecting the node's internal variant updates all references automatically (due to shared ownership). However, to handle multiple redirects efficiently, we use path compression: when accessing a node's value, we recursively find its root and update the pointer to the root to avoid long chains.
//
// Time complexity: Each active height value is processed (redirected and removed) at most once per operation that covers it, but across all operations a value can be removed and re-inserted multiple times. In the worst case, each operation may scan many values, but since each removal removes a value from the set and a value can be re-added later, the total work is bounded by `O((number_of_height_changes) log N)` where `N` is the number of distinct values. For typical constraints, if the number of operations and positions is up to 10^5, this is acceptable. Space is `O(P + D)` for positions and distinct values.
//
// For the standalone function, we can simplify: instead of a full pointer-based structure, we can maintain an array `finalHeights` of size `n` and a set of currently active distinct heights. For each compression, we need to update all positions whose current height is in the range. That would require scanning all positions, which is too slow. So we must use the pointer-redirection approach as in the snippet. We'll implement it with `std::shared_ptr` and a variant.
