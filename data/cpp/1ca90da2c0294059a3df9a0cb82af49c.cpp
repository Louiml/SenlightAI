Write a C++ function `minimizeDiscontent` that takes as input: a vector of 7 strings representing character names (always the same seven: "Anka", "Chapay", "Cleo", "Troll", "Dracul", "Snowy", "Hexadecimal"), a 7x7 adjacency matrix `likes` where `likes[i][j]` counts how many times character `i` explicitly likes character `j`, and a vector `xp` of exactly 3 positive integers representing experience points for three groups. The function must partition the 7 characters into 3 non-empty groups (each character belongs to exactly one group). For a partition, define the "dissatisfaction" as the maximum over groups of `xp[k] / group_size(k)` minus the minimum over groups of `xp[k] / group_size(k)` (using integer division, i.e., truncating toward zero). Define the "positive sentiment count" as the total number of ordered pairs `(a,b)` where `a` and `b` are in the same group and `likes[a][b] > 0` (i.e., sum over all groups of sum over all ordered pairs within that group of the exact integer entries in the matrix, counting duplicates). The goal is to choose the partition that first minimizes dissatisfaction; among partitions with equal minimal dissatisfaction, maximize the positive sentiment count. The function should return a `pair<int,int>` where the first is the minimal dissatisfaction and the second is the maximum positive sentiment count achievable with that minimal dissatisfaction. Use a brute-force search over all 3^7 = 2187 assignments, ignoring invalid ones where any group is empty, and keep track of the best pair using lexicographic comparison with the negative sentiment count trick as in the original code.
// The problem reduces to enumerating every possible assignment of each of the 7 characters to one of 3 groups. There are exactly 3^7 assignments; for each, we check that all three groups are non-empty (otherwise skip). For a valid assignment, we compute the positive sentiment count by iterating over all ordered pairs within each group and summing `likes[i][j]`. Simultaneously, compute the dissatisfaction as the range (max-min) of `xp[k] / group_size`, where group_size is the number of characters in that group. Use integer division as in C++. We maintain the best partition as a pair `(dissatisfaction, -positiveSentiment)` so that `std::min` naturally prefers smaller dissatisfaction, and for equal dissatisfaction, larger positive sentiment (since we negate it). Avoid overflow: dissatisfaction is at most around 1e9 and positive sentiment count is at most 7*7*? = at most 49 per pair? Actually `likes` entries are small, but the sum for a group of size m is at most m*m*maxEntry; with maxEntry possibly large but bounded by input constraints, still fits in int. Edge cases: exactly one group has all characters? That would make two groups empty -> invalid. The minimum valid group size is 1. The initial best pair should be initialized to a very large first value, e.g., `{INT_MAX, INT_MAX}`. Time complexity: O(3^7 * 7^2) = about 2187*49 ≈ 107k operations, very fast. Space complexity: O(7*7) for the matrix plus O(7) for the assignment.
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <climits>

// Returns a pair (minDissatisfaction, maxPositiveSentimentCount) over all partitions
// of the 7 characters into 3 non-empty groups.
std::pair<int,int> minimizeDiscontent(
    const std::vector<std::string>& names,
    const std::vector<std::vector<int>>& likes,
    const std::vector<int>& xp) {
    // Map names to indices 0..6
    std::map<std::string,int> id;
    for (int i = 0; i < 7; ++i) id[names[i]] = i;

    // Convert likes matrix to index-based (already given as 7x7)
    // xp has size 3

    std::vector<int> p(7);  // group assignment for each character

    std::pair<int,int> best = {INT_MAX, INT_MAX}; // best.first = dissatisfaction, best.second = -positiveSentiment

    // Helper lambda to evaluate current assignment p
    auto evaluate = [&]() {
        // Build groups
        std::vector<int> groups[3];
        for (int i = 0; i < 7; ++i) groups[p[i]].push_back(i);

        // Check all groups non-empty
        for (int k = 0; k < 3; ++k) if (groups[k].empty()) return;

        // Compute positive sentiment count
        int positive = 0;
        for (int k = 0; k < 3; ++k) {
            int sz = groups[k].size();
            for (int a = 0; a < sz; ++a) {
                for (int b = 0; b < sz; ++b) {
                    positive += likes[groups[k][a]][groups[k][b]];
                }
            }
        }

        // Compute dissatisfaction
        int maxVal = 0, minVal = INT_MAX;
        for (int k = 0; k < 3; ++k) {
            int val = xp[k] / (int)groups[k].size();
            maxVal = std::max(maxVal, val);
            minVal = std::min(minVal, val);
        }
        int diss = maxVal - minVal;

        // Compare with best: smaller diss, larger positive
        std::pair<int,int> candidate = {diss, -positive};
        if (candidate < best) best = candidate;
    };

    // DFS over assignments
    std::function<void(int)> dfs = [&](int u) {
        if (u == 7) {
            evaluate();
            return;
        }
        for (int g = 0; g < 3; ++g) {
            p[u] = g;
            dfs(u + 1);
        }
    };

    dfs(0);

    return {best.first, -best.second};
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration (from solution)
std::pair<int,int> minimizeDiscontent(
    const std::vector<std::string>& names,
    const std::vector<std::vector<int>>& likes,
    const std::vector<int>& xp);

int main() {
    std::vector<std::string> names = {"Anka", "Chapay", "Cleo", "Troll", "Dracul", "Snowy", "Hexadecimal"};

    // Example 1: No likes, all xp=1000 each.
    // Best partition with 1,1,5 or 2,2,3 etc. Minimizing range: e.g., sizes (3,2,2) gives xp/group: 333,500,500 -> range 167; (2,2,3) same; (1,3,3) -> 1000,333,333 range 667; so best diss=167, positive=0
    {
        std::vector<std::vector<int>> likes(7, std::vector<int>(7, 0));
        std::vector<int> xp = {1000,1000,1000};
        auto res = minimizeDiscontent(names, likes, xp);
        assert(res.first == 167);
        assert(res.second == 0);
    }

    // Example 2: One directed like: Anka likes Cleo (0->2).
    // Best diss same 167; positive should be 1 if we can put 0 and 2 in same group.
    {
        std::vector<std::vector<int>> likes(7, std::vector<int>(7, 0));
        likes[0][2] = 1;
        std::vector<int> xp = {1000,1000,1000};
        auto res = minimizeDiscontent(names, likes, xp);
        assert(res.first == 167);
        assert(res.second == 1);
    }

    // Example 3: All 7 in one group invalid; all groups must be non-empty.
    // Very skewed xp: 1,100,10000. Best partition likely sizes (1,1,5) to balance? Let's manually check:
    // size1 group gets 1/1=1, size1 gets 100, size5 gets 10000/5=2000 -> range 1999
    // size (2,2,3): 1/2=0, 100/2=50, 10000/3=3333 -> range 3333 worse
    // Actual best via brute force: maybe (1,2,4) gives 1,50,2500 -> range 2499; (1,1,5) gives 1999; (1,3,3) gives 1,33,3333 -> range 3332; So best diss = 1999.
    // Positive = 0 since no likes.
    {
        std::vector<std::vector<int>> likes(7, std::vector<int>(7, 0));
        std::vector<int> xp = {1,100,10000};
        auto res = minimizeDiscontent(names, likes, xp);
        assert(res.first == 1999);
        assert(res.second == 0);
    }

    // Example 4: All likes present (every ordered pair has 1). Then positive count is always
    // sum_{groups} size^2. To maximize positive with min diss, choose partition that gives highest sum of squares.
    // For diss=167 (sizes 2,2,3): sum squares = 4+4+9=17. For diss=167 with sizes (2,3,2) same. No other diss is lower.
    // Actually diss 167 is minimal; among those, maximize sum of squares. Possible size distributions with diss 167:
    // (2,2,3) permutations, (3,2,2), (2,3,2). Also (1,3,3) gives diss=667 worse. So max sum squares=17.
    {
        std::vector<std::vector<int>> likes(7, std::vector<int>(7, 1));
        std::vector<int> xp = {1000,1000,1000};
        auto res = minimizeDiscontent(names, likes, xp);
        assert(res.first == 167);
        assert(res.second == 17);
    }

    // Example 5: Only one character; but here we have 7 characters so not applicable.
    // Test that duplicates in likes are counted (e.g., likes[0][0] = 5 adds 5 for ordered pair within same group)
    {
        std::vector<std::vector<int>> likes(7, std::vector<int>(7, 0));
        likes[0][0] = 5; // self-like
        std::vector<int> xp = {1000,1000,1000};
        auto res = minimizeDiscontent(names, likes, xp);
        // With diss=167, we must put char0 in some group; positive includes self pair if same group.
        // Maximizing positive: we can put char0 alone in a group? But need all groups non-empty. If char0 alone, that group size=1 gives xp/1=1000 -> diss maybe large. So best to put char0 with others.
        // Minimal diss 167 requires sizes 2,2,3. Char0 can be in any group of size >=2, so self-like counted. So positive at least 5.
        // Could we get diss lower? No. Could we get positive >5? Only self-like exists; no other likes. So max positive=5.
        assert(res.first == 167);
        assert(res.second == 5);
    }

    return 0;
}
