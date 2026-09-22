// Write a C++ function `generatePermutations` that takes a positive integer `n` and returns a `std::vector<std::vector<int>>` containing all permutations of the integers from 1 to `n` in lexicographical order. The function must use a recursive depth-first search (DFS) with a `std::vector<bool>` for visited status and a `std::vector<int>` for the current permutation. The returned vector must have exactly `n!` permutations, each of length `n`. If `n` is 0 or negative, return an empty vector.

// The solution uses standard backtracking. We maintain a `std::vector<bool> used(n+1, false)` where `used[i]` indicates whether integer `i` is already placed in the current permutation. We also maintain a `std::vector<int> current` that builds one permutation as we recurse. At each recursion level `depth` (0-indexed), we iterate through integers `1..n`, and for each unused integer, we place it at `current[depth]`, mark it used, recurse to the next depth, then unmark it (backtrack). When `depth == n`, we have a complete permutation, so we push a copy of `current` into the result. Because we iterate integers in increasing order, the generated permutations are naturally in lexicographical order. Edge cases: `n <= 0` should return an empty vector; `n == 1` returns `{{1}}`. Time complexity is `O(n * n!)` because there are `n!` permutations and each is copied into the result (each copy takes O(n)). Space complexity is `O(n)` for the recursion stack and auxiliary arrays, plus `O(n * n!)` for the result storage.

#include <vector>

// Generate all permutations of 1..n in lexicographical order using DFS backtracking.
// Returns an empty vector if n <= 0.
std::vector<std::vector<int>> generatePermutations(int n) {
    std::vector<std::vector<int>> result;
    if (n <= 0) {
        return result;
    }

    std::vector<bool> used(n + 1, false);
    std::vector<int> current(n);

    // Recursive lambda for DFS
    std::function<void(int)> dfs = [&](int depth) {
        if (depth == n) {
            result.push_back(current);
            return;
        }
        for (int value = 1; value <= n; ++value) {
            if (!used[value]) {
                current[depth] = value;
                used[value] = true;
                dfs(depth + 1);
                used[value] = false; // backtrack
            }
        }
    };

    dfs(0);
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // n = 0 and negative -> empty
    assert(generatePermutations(0).empty());
    assert(generatePermutations(-3).empty());

    // n = 1 -> single permutation
    std::vector<std::vector<int>> p1 = generatePermutations(1);
    assert(p1.size() == 1);
    assert(p1[0] == std::vector<int>{1});

    // n = 2 -> {1,2}, {2,1}
    std::vector<std::vector<int>> p2 = generatePermutations(2);
    assert(p2.size() == 2);
    assert(p2[0] == std::vector<int>{1,2});
    assert(p2[1] == std::vector<int>{2,1});

    // n = 3 -> 6 permutations in lexicographical order
    std::vector<std::vector<int>> p3 = generatePermutations(3);
    assert(p3.size() == 6);
    assert(p3[0] == std::vector<int>({1,2,3}));
    assert(p3[1] == std::vector<int>({1,3,2}));
    assert(p3[2] == std::vector<int>({2,1,3}));
    assert(p3[3] == std::vector<int>({2,3,1}));
    assert(p3[4] == std::vector<int>({3,1,2}));
    assert(p3[5] == std::vector<int>({3,2,1}));

    // n = 4 -> check size and that each permutation is a valid permutation of 1..4
    std::vector<std::vector<int>> p4 = generatePermutations(4);
    assert(p4.size() == 24);
    for (const auto& perm : p4) {
        assert(perm.size() == 4);
        std::vector<int> sorted = perm;
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({1,2,3,4}));
    }
    // Also verify lexicographical order using std::is_sorted
    assert(std::is_sorted(p4.begin(), p4.end()));

    return 0;
}
