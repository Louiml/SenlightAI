// Given a tree with N nodes, each node initially has a population count (number of people living there) and a happiness value. A node is considered "happy" if its happiness equals the number of happy people minus the number of sad people residing in its subtree (including itself and all descendants). People can change mood between happy and sad, but the total number of people in a subtree is fixed. The happiness value for a node is given as `h_i` and population as `p_i`. Write a C++ function `bool possibleTree(const vector<int>& population, const vector<int>& happiness, const vector<vector<int>>& adj)` that returns `true` if there exists an assignment of each person as happy (value +1) or sad (value -1) such that for every node `v`, the sum of assigned values in its subtree equals its given happiness `h[v]`. The tree is rooted at node 1 (1-indexed) and adjacency list is provided.
#include <cassert>
#include <bits/stdc++.h>
using namespace std;

// (Include the solution function here)

int main() {
    // Test 1: single node, population 1, happiness 1 -> one happy person
    vector<int> pop1 = {1};
    vector<int> hap1 = {1};
    vector<vector<int>> adj1 = { {} };
    assert(possibleTree(pop1, hap1, adj1) == true);

    // Test 2: single node, population 1, happiness 0 -> impossible (must be +1 or -1)
    vector<int> pop2 = {1};
    vector<int> hap2 = {0};
    vector<vector<int>> adj2 = { {} };
    assert(possibleTree(pop2, hap2, adj2) == false);

    // Test 3: chain of 2 nodes, both population 1, h=[2,0] impossible because root sum=2 but h=2 requires all happy children, but child h=0 impossible.
    vector<int> pop3 = {1,1};
    vector<int> hap3 = {2,0};
    vector<vector<int>> adj3 = { {1}, {0} };
    assert(possibleTree(pop3, hap3, adj3) == false);

    // Test 4: chain of 2 nodes, pop [1,1], h=[0,0] → root sum=2, h=0 means 1 happy 1 sad, child h=0 also needs 1 happy 1 sad and has only 1 person -> impossible.
    vector<int> pop4 = {1,1};
    vector<int> hap4 = {0,0};
    vector<vector<int>> adj4 = { {1}, {0} };
    assert(possibleTree(pop4, hap4, adj4) == false);

    // Test 5: star with root pop2 h0, leaves pop1 h1 each. Root sum=4, h0 -> happyTotal=2, each leaf gives 1 happy (since h1 with pop1 => happy). childHappySum=2, neededFromNode=0 <=2, feasible.
    vector<int> pop5 = {2,1,1};
    vector<int> hap5 = {0,1,1};
    vector<vector<int>> adj5 = { {1,2}, {0}, {0} };
    assert(possibleTree(pop5, hap5, adj5) == true);

    // Test 6: same star but root h=-2? Not possible because abs(-2)>sumPop4? No sumPop4=4, h=-2 => happyTotal=1, children give 2 happy -> impossible.
    vector<int> pop6 = {2,1,1};
    vector<int> hap6 = {-2,1,1};
    vector<vector<int>> adj6 = { {1,2}, {0}, {0} };
    assert(possibleTree(pop6, hap6, adj6) == false);

    // Test 7: example from problem statement: n=4, populations [1,2,1,2], h=[-1,1,1,1], edges 1-2,2-3,2-4 → feasible.
    vector<int> pop7 = {1,2,1,2};
    vector<int> hap7 = {-1,1,1,1};
    vector<vector<int>> adj7 = { {1}, {0,2,3}, {1}, {1} };
    assert(possibleTree(pop7, hap7, adj7) == true);

    // Test 8: same but change root h to -3 → sumPop=6, |h|>6? no, |h|=3 <=6, parity (6-(-3))=9 odd -> false.
    vector<int> pop8 = {1,2,1,2};
    vector<int> hap8 = {-3,1,1,1};
    vector<vector<int>> adj8 = { {1}, {0,2,3}, {1}, {1} };
    assert(possibleTree(pop8, hap8, adj8) == false);

    // Test 9: two-node tree, pop [0,1], h=[0,1] -> root sum=1, h=0 impossible (population 0 node cannot provide happy).
    vector<int> pop9 = {0,1};
    vector<int> hap9 = {0,1};
    vector<vector<int>> adj9 = { {1}, {0} };
    assert(possibleTree(pop9, hap9, adj9) == false);

    // Test 10: larger tree with 5 nodes, all pop1, h=[1,1,-1,1,1], edges 1-2,1-3,2-4,2-5. sum=5, root h=1 -> happyTotal=3, children 2 and 3 each contribute? Node2 subtree sum=3, h=1 -> happyTotal=2, children 4,5 each h=1 give 1 happy each, neededFromNode=0, feasible. Node3 h=-1 with pop1 -> happyTotal=0, gives 0 happy. childHappySum=2, neededFromRoot=1 <= root pop1, feasible.
    vector<int> pop10 = {1,1,1,1,1};
    vector<int> hap10 = {1,1,-1,1,1};
    vector<vector<int>> adj10 = { {1,2}, {0,3,4}, {0}, {1}, {1} };
    assert(possibleTree(pop10, hap10, adj10) == true);

    return 0;
}
#include <bits/stdc++.h>

using namespace std;

// Verify if a valid assignment of happy/sad exists for the given tree.
bool possibleTree(const vector<int>& population, const vector<int>& happiness, const vector<vector<int>>& adj) {
    int n = population.size(); // nodes 0-based internally
    vector<int> sumPop(n, 0);
    vector<int> happyCount(n, 0);
    
    // DFS that returns whether subtree rooted at v is feasible.
    function<bool(int, int)> dfs = [&](int v, int parent) -> bool {
        sumPop[v] = population[v];
        int childHappySum = 0;
        for (int u : adj[v]) {
            if (u == parent) continue;
            if (!dfs(u, v)) return false;
            sumPop[v] += sumPop[u];
            childHappySum += happyCount[u];
        }
        // Total people in subtree
        int total = sumPop[v];
        int h = happiness[v];
        // Conditions:
        if (abs(h) > total) return false;
        if ((total - h) % 2 != 0) return false;
        int happyTotal = (total + h) / 2;
        // Children already provide some happy people; the rest must come from node v itself.
        if (childHappySum > happyTotal) return false;
        int neededFromNode = happyTotal - childHappySum;
        if (neededFromNode > population[v]) return false;
        happyCount[v] = happyTotal;
        return true;
    };
    
    return dfs(0, -1);
}
// This is a bottom-up tree dynamic programming / feasibility check problem. For a node `v`, after processing its children, we know the total population in its subtree `sumPop[v]` (sum of `population` of all nodes in subtree) and the sum of happiness values from its children subtrees `childSum` (sum of `happiness[u]` for each child `u`). The happiness of the subtree rooted at `v` must be `h[v]`, which equals `(number of happy people) - (number of sad people)` in that subtree. Meanwhile, total population `sumPop[v]` equals happy + sad. Therefore, we can solve: happy = (sumPop[v] + h[v])/2 and sad = (sumPop[v] - h[v])/2, both must be non-negative integers. Additionally, the happy count from children subtrees cannot exceed the happy count we need at `v`, because each child contributes some happy people, and the remaining happy people come from node `v` itself (which must be between 0 and `population[v]`). The condition becomes: let `childHappy` = number of happy people already produced by children = (sum of happy counts from each child). Let `totalHappyNeeded` = (sumPop[v] + h[v])/2. Then we need `childHappy <= totalHappyNeeded` and `(totalHappyNeeded - childHappy) <= population[v]` (since the rest must be supplied by node v's own people). Also `sumPop[v]` and `h[v]` must have the same parity (sumPop[v] - h[v] must be even). And for each child, we recursively verify feasibility. Edge cases: negative happiness, absolute value of h[v] greater than sumPop[v] (impossible), or parity mismatch. Time complexity O(N) because each edge visited once in DFS. Space O(N) for recursion stack and auxiliary arrays.
