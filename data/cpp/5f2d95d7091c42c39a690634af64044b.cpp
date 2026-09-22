Given a rooted tree stored in an adjacency structure where node 0 is the root and every other node has a parent, with each node having a value, write a C++ function `std::vector<int> findCutEdges(int n, const std::vector<std::pair<int,int>>& parentAndValue)`. The input `parentAndValue` is a 1-indexed list (element i corresponds to node i): each pair gives the parent of node i and the value of node i. The total sum of all node values is guaranteed to be divisible by 3 when valid. The function should return a vector of exactly two node indices (the two cut points) such that removing the edges from these nodes to their parents splits the tree into three connected components each having equal total sum. If no such two cuts exist, return an empty vector. The tree structure is defined by parent pointers (parent may be 0 for nodes directly connected to root). Note: cut points must be distinct and neither can be the root (node 0). The root's value is 0 and is not counted in any component sum.
The problem requires splitting the tree into three parts of equal sum. First compute the total sum of all node values. If total sum is 0 or not divisible by 3, no solution. Let target = total/3. Perform a post-order DFS from the root (node 0). For each subtree, compute its sum. If a subtree's sum equals target, that subtree can be one of the required components; we record that subtree's root as a cut point and treat that subtree as removed (i.e., its contribution to ancestors becomes 0). We need exactly two such independent subtrees whose sums are each target. Key edge cases: (1) A single node with value target can be a cut point; (2) Two subtrees might overlap? No, because once we find a subtree sum = target, we set its returned sum to 0, so ancestors won't count it again, preventing overlapping cuts. (3) The root itself cannot be a cut point, even if its total sum equals target (which would mean total sum is target, only possible if total is 0, already excluded). We stop after finding two cuts. If after DFS we have fewer than two cuts, return empty. Time complexity: O(n) for DFS, O(n) space for recursion stack and adjacency lists.
#include <vector>
#include <algorithm>

// Compute cuts to split tree into three equal-sum components.
// n: number of nodes (indices 1..n). parentAndValue[i-1] = {parent of i, value of i}.
// Returns vector of two cut node indices, or empty if impossible.
std::vector<int> findCutEdges(int n, const std::vector<std::pair<int,int>>& parentAndValue) {
    // Build children adjacency.
    std::vector<std::vector<int>> children(n + 1);
    long long total = 0;
    for (int i = 1; i <= n; ++i) {
        int parent = parentAndValue[i - 1].first;
        int val = parentAndValue[i - 1].second;
        children[parent].push_back(i);
        total += val;
    }
    if (total == 0 || total % 3 != 0) {
        return {};
    }
    const long long target = total / 3;

    std::vector<int> cuts;
    // values array: index 0 is root with value 0.
    std::vector<int> nodeValue(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        nodeValue[i] = parentAndValue[i - 1].second;
    }

    // Post-order DFS function (lambda for recursion).
    // Returns sum of the subtree (0 if this subtree has already been cut).
    std::function<long long(int)> dfs = [&](int node) -> long long {
        long long sum = nodeValue[node];
        for (int child : children[node]) {
            sum += dfs(child);
        }
        if (sum == target && node != 0 && cuts.size() < 2) {
            cuts.push_back(node);
            return 0; // subtree removed, contributes nothing upward
        }
        return sum;
    };

    dfs(0);
    if (cuts.size() == 2) {
        return cuts;
    }
    return {};
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or include the header).
// For brevity, the code is assumed to be above.

int main() {
    // Test 1: Simple chain 1->2->3, all values 1, total=3, target=1.
    // Tree: root 0 has child 1 (value 1), 1 has child 2 (value 1), 2 has child 3 (value 1).
    std::vector<std::pair<int,int>> t1 = {{0,1}, {1,1}, {2,1}};
    std::vector<int> r1 = findCutEdges(3, t1);
    // Need two cuts: nodes 1 and 2 (since subtrees of 1 and 2 each sum to 1).
    assert(r1.size() == 2);
    assert((r1[0] == 1 && r1[1] == 2) || (r1[0] == 2 && r1[1] == 1));

    // Test 2: Impossible because total=5 not divisible by 3.
    std::vector<std::pair<int,int>> t2 = {{0,2}, {1,3}};
    assert(findCutEdges(2, t2).empty());

    // Test 3: A single node with value 3, total=3, target=1 -> impossible because one node's value 3 can't split.
    std::vector<std::pair<int,int>> t3 = {{0,3}};
    assert(findCutEdges(1, t3).empty());

    // Test 4: Root has three children each with value 2, total=6, target=2.
    // Children 1,2,3 each form a subtree sum 2. We need exactly two cuts, they can be any two.
    std::vector<std::pair<int,int>> t4 = {{0,2}, {0,2}, {0,2}};
    std::vector<int> r4 = findCutEdges(3, t4);
    assert(r4.size() == 2);
    // The two cut nodes must be among {1,2,3}
    assert((r4[0] == 1 && r4[1] == 2) || (r4[0] == 1 && r4[1] == 3) || (r4[0] == 2 && r4[1] == 3) ||
           (r4[0] == 2 && r4[1] == 1) || (r4[0] == 3 && r4[1] == 1) || (r4[0] == 3 && r4[1] == 2));

    // Test 5: A more complex tree: root 0 -> children 1 (value 1) and 2 (value 2).
    // Node 1 has child 3 (value 2). Total = 1+2+2=5, not divisible by 3 -> empty.
    std::vector<std::pair<int,int>> t5 = {{0,1}, {0,2}, {1,2}};
    assert(findCutEdges(3, t5).empty());

    // Test 6: Chain 1(2) -> 2(2) -> 3(2). Total=6, target=2. Node 3's subtree sum=2, node 2's subtree sum=4 not target.
    // Node 1's subtree sum=6 not target. Only one cut possible -> empty.
    std::vector<std::pair<int,int>> t6 = {{0,2}, {1,2}, {2,2}};
    assert(findCutEdges(3, t6).empty());

    // Test 7: Root 0 -> child 1 (value 4), and child 1 has two children 2 (value 1) and 3 (value 1). Total=6, target=2.
    // Subtrees: node 2 sum=1, node 3 sum=1, node 1 sum=4+1+1=6. No subtree sum=2 -> empty.
    std::vector<std::pair<int,int>> t7 = {{0,4}, {1,1}, {1,1}};
    assert(findCutEdges(3, t7).empty());

    // Test 8: Root 0 -> child 1 (value 3) and child 2 (value 3). Total=6, target=2? No, target=2, but each child sums=3.
    // No subtree sums 2 -> empty.
    std::vector<std::pair<int,int>> t8 = {{0,3}, {0,3}};
    assert(findCutEdges(2, t8).empty());

    // Test 9: total=0 case: node 1 value 0, parent 0 -> empty.
    std::vector<std::pair<int,int>> t9 = {{0,0}};
    assert(findCutEdges(1, t9).empty());

    // Test 10: A tree where two cuts are not directly under root but deeper.
    // root 0 -> 1(0) -> 2(1) and 1 also has 3(1). Also root has 4(1). Total=3, target=1.
    // Subtrees: 2 sum=1 (cut), 3 sum=1 (cut), 4 sum=1 but already two cuts. Must find two.
    // The DFS will find 2 and 3, valid.
    std::vector<std::pair<int,int>> t10 = {{0,0}, {1,1}, {1,1}, {0,1}};
    // Nodes: 1 parent 0 value 0; 2 parent 1 value 1; 3 parent 1 value 1; 4 parent 0 value 1.
    std::vector<int> r10 = findCutEdges(4, t10);
    assert(r10.size() == 2);
    // The cuts must be among {2,3,4} exactly two of them.
    bool ok = false;
    if ((r10[0]==2 && r10[1]==3) || (r10[0]==3 && r10[1]==2) ||
        (r10[0]==2 && r10[1]==4) || (r10[0]==4 && r10[1]==2) ||
        (r10[0]==3 && r10[1]==4) || (r10[0]==4 && r10[1]==3)) ok = true;
    assert(ok);

    return 0;
}
