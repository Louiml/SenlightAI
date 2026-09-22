Write a C++ function named `findTownJudge` that, given an integer `n` (the number of people in a town, labeled from 1 to `n`) and a vector of trust relationships `trust` where each element is a pair `[a, b]` meaning person `a` trusts person `b`, returns the label of the town judge if one exists, or `-1` otherwise. The town judge is defined as the person who is trusted by everyone else (exactly `n-1` people) and who trusts nobody. The input `trust` may be empty, and the relationships are guaranteed to be unique (no duplicate pairs). The function should handle cases where `n` is at least 1 and up to any reasonable size, and must correctly identify the judge even when only one person exists (in that case, that person is the judge because they are trusted by zero others and trust nobody — the condition of being trusted by `n-1 = 0` people holds).

#include <cassert>
#include <vector>

// The function is declared above; this is the test harness.
int main() {
    // Example 1: n=2, 1 trusts 2 -> 2 is judge
    std::vector<std::vector<int>> t1 = {{1, 2}};
    assert(findTownJudge(2, t1) == 2);

    // Example 2: n=3, no trust -> no one is trusted by all, no judge
    std::vector<std::vector<int>> t2 = {};
    assert(findTownJudge(3, t2) == -1);

    // Example 3: n=3, all trust 2, 2 trusts nobody -> judge 2
    std::vector<std::vector<int>> t3 = {{1, 2}, {3, 2}};
    assert(findTownJudge(3, t3) == 2);

    // Example 4: n=3, 1 trusts 2, 2 trusts 3, 3 trusts 1 -> cycle, no judge
    std::vector<std::vector<int>> t4 = {{1, 2}, {2, 3}, {3, 1}};
    assert(findTownJudge(3, t4) == -1);

    // Example 5: n=1, empty trust -> person 1 is judge
    std::vector<std::vector<int>> t5 = {};
    assert(findTownJudge(1, t5) == 1);

    // Example 6: n=4, everyone trusts 3, but 3 trusts 1 -> no judge
    std::vector<std::vector<int>> t6 = {{1, 3}, {2, 3}, {4, 3}, {3, 1}};
    assert(findTownJudge(4, t6) == -1);

    // Example 7: n=4, everyone trusts 3, 3 trusts nobody -> judge 3
    std::vector<std::vector<int>> t7 = {{1, 3}, {2, 3}, {4, 3}};
    assert(findTownJudge(4, t7) == 3);

    // Example 8: n=2, both trust each other -> no judge
    std::vector<std::vector<int>> t8 = {{1, 2}, {2, 1}};
    assert(findTownJudge(2, t8) == -1);

    // Example 9: n=5, multiple trust graphs, judge is 4
    std::vector<std::vector<int>> t9 = {{1, 4}, {2, 4}, {3, 4}, {5, 4}, {2, 1}};
    assert(findTownJudge(5, t9) == 4);

    return 0;
}

#include <vector>
#include <cassert>

// Find the town judge: a person trusted by everyone else who trusts nobody.
// Returns the judge's label (1-based) or -1 if none exists.
int findTownJudge(int n, const std::vector<std::vector<int>>& trust) {
    std::vector<int> trustCount(n + 1, 0);      // count of people who trust each label
    std::vector<bool> trustsSomeone(n + 1, false); // whether each person trusts anyone

    for (const auto& pair : trust) {
        // pair[0] trusts pair[1]
        int a = pair[0];
        int b = pair[1];
        trustsSomeone[a] = true;
        ++trustCount[b];
    }

    for (int i = 1; i <= n; ++i) {
        if (trustCount[i] == n - 1 && !trustsSomeone[i]) {
            return i;
        }
    }
    return -1;
}

// The solution uses two data structures: a vector `trustCount` of size `n+1` to count how many people trust each person, and a boolean vector `trustsSomeone` of size `n+1` to mark whether each person actively trusts anyone. For each trust pair `[a, b]`, we increment `trustCount[b]` and set `trustsSomeone[a] = true`. After processing all pairs, we iterate over labels from 1 to `n`. A person `i` is the judge if two conditions hold simultaneously: `trustCount[i] == n-1` (they are trusted by everyone else) and `trustsSomeone[i] == false` (they trust nobody). If such a person is found, return `i`. If no such person exists, return `-1`. Edge cases: an empty trust vector with `n=1` correctly yields judge `1` because `trustCount[1] == 0 == n-1` and `trustsSomeone[1]` is false. If the graph has multiple candidates that satisfy the trust count condition but also trust someone, they are excluded. The algorithm runs in `O(n + m)` time where `m` is the number of trust pairs, and uses `O(n)` auxiliary space.
