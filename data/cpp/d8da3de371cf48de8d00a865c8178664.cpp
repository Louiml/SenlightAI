// Write a C++ function `stableMatching` that takes two square matrices of size `n x n` (where `n` is a positive integer), `menPref` and `womenPref`, each row containing a permutation of `0..n-1`. The matrix `menPref[i]` lists women in order of preference for man `i` (index 0 is the most preferred), and `womenPref[j]` lists men similarly for woman `j`. The function must return a vector of length `n` where `result[i]` is the woman matched to man `i`. The matching must be stable: there is no pair (man `i`, woman `j`) such that both prefer each other over their assigned partners. Use the Gale-Shapley algorithm with men proposing to women. The function signature must be `std::vector<int> stableMatching(const std::vector<std::vector<int>>& menPref, const std::vector<std::vector<int>>& womenPref)`.
The stable matching problem is solved by the classic Gale-Shapley algorithm. Men propose to women in order of their preference lists. Each woman keeps the best proposer she has seen so far and rejects the rest; if she later receives a better proposal, she discards the previous match. The algorithm continues until every man is matched. A key implementation detail is that we need to efficiently check whether a woman prefers a new proposer over her current match. Given that `womenPref[j]` is a permutation, we can precompute a `rank` matrix where `rank[j][i]` is the position of man `i` in woman `j`’s list. Then comparing two men for a woman is just comparing their rank values. Since each man proposes at most `n` times, there are at most `n^2` proposals, and each proposal operation is O(1) after precomputing ranks. Thus total time is O(n^2) and space is O(n^2) for the rank matrix plus O(n) for the match arrays. Edge cases: when `n=1`, the only pair is matched; when preference lists are arbitrary permutations, the algorithm always terminates. The result is guaranteed to be a perfect matching (each man gets exactly one distinct woman, and every woman is matched to exactly one man).
#include <vector>
#include <queue>

// Gale-Shapley stable matching algorithm (men propose to women).
// menPref[i] is a permutation of 0..n-1 representing man i's preference order for women.
// womenPref[j] is a permutation of 0..n-1 representing woman j's preference order for men.
// Returns vector result where result[i] = woman matched to man i, for all i.
std::vector<int> stableMatching(const std::vector<std::vector<int>>& menPref,
                                const std::vector<std::vector<int>>& womenPref) {
    int n = static_cast<int>(menPref.size());
    if (n == 0) return {};

    // rank[j][i] = position of man i in woman j's preference list.
    std::vector<std::vector<int>> rank(n, std::vector<int>(n));
    for (int j = 0; j < n; ++j) {
        for (int pos = 0; pos < n; ++pos) {
            int man = womenPref[j][pos];
            rank[j][man] = pos;
        }
    }

    // nextPropose[i] = index into menPref[i] for the next woman man i will propose to.
    std::vector<int> nextPropose(n, 0);

    // womanMatch[j] = current man matched to woman j, or -1 if unmatched.
    std::vector<int> womanMatch(n, -1);

    // menLeft contains all unmatched men initially.
    std::queue<int> freeMen;
    for (int i = 0; i < n; ++i) freeMen.push(i);

    while (!freeMen.empty()) {
        int man = freeMen.front();
        freeMen.pop();

        // Man proposes to his next preferred woman.
        int woman = menPref[man][nextPropose[man]];
        nextPropose[man]++;

        if (womanMatch[woman] == -1) {
            // Woman is free, she accepts.
            womanMatch[woman] = man;
        } else {
            int currentMan = womanMatch[woman];
            // If woman prefers new man over current, she switches.
            if (rank[woman][man] < rank[woman][currentMan]) {
                womanMatch[woman] = man;
                freeMen.push(currentMan); // current man becomes free
            } else {
                freeMen.push(man); // new proposer remains free
            }
        }
    }

    // Build result: result[man] = woman matched to man.
    std::vector<int> result(n);
    for (int j = 0; j < n; ++j) {
        result[womanMatch[j]] = j;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Include the solution function here (for brevity, we assume it is defined above).

int main() {
    // Test case 1: n=1
    {
        std::vector<std::vector<int>> menPref = {{0}};
        std::vector<std::vector<int>> womenPref = {{0}};
        std::vector<int> result = stableMatching(menPref, womenPref);
        assert(result == std::vector<int>({0}));
    }

    // Test case 2: n=2, simple case
    {
        std::vector<std::vector<int>> menPref = {{0,1}, {1,0}};
        std::vector<std::vector<int>> womenPref = {{0,1}, {1,0}};
        std::vector<int> result = stableMatching(menPref, womenPref);
        // Possibly (man0->woman0, man1->woman1) or (man0->woman1, man1->woman0); both stable.
        // Verify it's a permutation and stable.
        assert(result.size() == 2);
        std::vector<int> sorted = result;
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({0,1}));
        // Man0 gets his highest pick, Man1 gets his highest pick, so it's stable.
        assert(result[0] == 0 || result[0] == 1); // Just ensure valid.
    }

    // Test case 3: n=2 with conflicting preferences
    {
        std::vector<std::vector<int>> menPref = {{0,1}, {0,1}};
        std::vector<std::vector<int>> womenPref = {{0,1}, {1,0}};
        std::vector<int> result = stableMatching(menPref, womenPref);
        // Man0 proposes to woman0 first and wins her, man1 then takes woman1.
        assert(result[0] == 0);
        assert(result[1] == 1);
    }

    // Test case 4: n=3 known stable matching
    {
        std::vector<std::vector<int>> menPref = {
            {0,1,2},
            {1,0,2},
            {2,0,1}
        };
        std::vector<std::vector<int>> womenPref = {
            {0,1,2},
            {1,0,2},
            {2,0,1}
        };
        std::vector<int> result = stableMatching(menPref, womenPref);
        assert(result.size() == 3);
        std::vector<int> sorted = result;
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({0,1,2}));
        // With symmetric preferences, identity matching is stable.
        assert(result[0] == 0 && result[1] == 1 && result[2] == 2);
    }

    // Test case 5: n=4 random-like, check stability manually
    {
        std::vector<std::vector<int>> menPref = {
            {1,0,3,2},
            {2,1,0,3},
            {3,2,1,0},
            {0,3,2,1}
        };
        std::vector<std::vector<int>> womenPref = {
            {2,3,0,1},
            {0,1,2,3},
            {1,2,3,0},
            {3,0,1,2}
        };
        std::vector<int> result = stableMatching(menPref, womenPref);
        assert(result.size() == 4);
        std::vector<int> sorted = result;
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({0,1,2,3}));

        // Build rank for women to verify stability
        int n = 4;
        std::vector<std::vector<int>> rank(n, std::vector<int>(n));
        for (int j = 0; j < n; ++j) {
            for (int pos = 0; pos < n; ++pos) rank[j][womenPref[j][pos]] = pos;
        }
        // Build womanMatch
        std::vector<int> womanMatch(n);
        for (int i = 0; i < n; ++i) womanMatch[result[i]] = i;

        // Check no blocking pair
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                bool manPrefers = false;
                // Does man i prefer woman j over his current match result[i]?
                for (int pos = 0; pos < n; ++pos) {
                    if (menPref[i][pos] == j) { manPrefers = true; break; }
                    if (menPref[i][pos] == result[i]) break;
                }
                bool womanPrefers = false;
                for (int pos = 0; pos < n; ++pos) {
                    if (womenPref[j][pos] == i) { womanPrefers = true; break; }
                    if (womenPref[j][pos] == womanMatch[j]) break;
                }
                assert(!(manPrefers && womanPrefers));
            }
        }
    }

    // Test case 6: n=5, check all matched uniquely
    {
        int n = 5;
        std::vector<std::vector<int>> menPref(n, std::vector<int>(n));
        std::vector<std::vector<int>> womenPref(n, std::vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                menPref[i][j] = (i + j + 1) % n;
                womenPref[i][j] = (i + j + 2) % n;
            }
        }
        std::vector<int> result = stableMatching(menPref, womenPref);
        std::vector<int> sorted = result;
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({0,1,2,3,4}));
    }

    return 0;
}
