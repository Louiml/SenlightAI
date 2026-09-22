/*
Write a C++ function named `generateCombinations` that takes two integers `n` and `k` (with `1 ≤ k ≤ n ≤ 100`) and returns a `std::vector<std::string>` containing all combinations of `k` distinct integers chosen from the set `{1, 2, ..., n}`, where each combination is represented as a string of its `k` integers concatenated in increasing order (no separators, e.g., for `n=4, k=2`, one element is `"12"`). The combinations must be generated in lexicographic (dictionary) order, and the function must use the iterative successor algorithm (not recursion) based on the provided snippet's `Sinh()` logic. The returned vector must be empty if `k` is 0 or if `k > n`. The function should handle the initial combination as `[1, 2, ..., k]` and stop after generating the last combination `[n-k+1, ..., n]`. Note: The output order must match exactly the order produced by repeatedly applying the successor rule: find the rightmost position `i` where `a[i] < n-k+i`; if none exists, stop; otherwise increment `a[i]` and set all positions after `i` to be `a[i]+1, a[i]+2, ...`.
*/

#include <vector>
#include <string>

// Generate all k-combinations of {1,...,n} in lexicographic order as concatenated strings.
std::vector<std::string> generateCombinations(int n, int k) {
    std::vector<std::string> result;
    if (k <= 0 || k > n) {
        return result;
    }

    std::vector<int> a(k + 1, 0);  // 1-indexed array for current combination
    for (int i = 1; i <= k; ++i) {
        a[i] = i;
    }

    bool stop = false;
    while (!stop) {
        // Convert current combination to a string
        std::string combo;
        for (int i = 1; i <= k; ++i) {
            combo += std::to_string(a[i]);
        }
        result.push_back(combo);

        // Generate next combination using the successor rule
        int i = k;
        while (i >= 1 && a[i] == n - k + i) {
            --i;
        }
        if (i < 1) {
            stop = true;
        } else {
            ++a[i];
            for (int j = i + 1; j <= k; ++j) {
                a[j] = a[j - 1] + 1;
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Function under test is declared here; in a real test it would be included.
std::vector<std::string> generateCombinations(int n, int k);

int main() {
    // Test basic case: n=3, k=2
    std::vector<std::string> c1 = generateCombinations(3, 2);
    assert(c1 == std::vector<std::string>({"12", "13", "23"}));

    // Test n=4, k=3
    std::vector<std::string> c2 = generateCombinations(4, 3);
    assert(c2 == std::vector<std::string>({"123", "124", "134", "234"}));

    // Test when k == n (single combination)
    std::vector<std::string> c3 = generateCombinations(5, 5);
    assert(c3 == std::vector<std::string>({"12345"}));

    // Test when k == 1 (all single digits)
    std::vector<std::string> c4 = generateCombinations(4, 1);
    assert(c4 == std::vector<std::string>({"1", "2", "3", "4"}));

    // Test when k > n -> empty
    assert(generateCombinations(3, 4).empty());

    // Test when k == 0 -> empty
    assert(generateCombinations(3, 0).empty());

    // Test full set n=4, k=2 (lexicographic order)
    std::vector<std::string> c5 = generateCombinations(4, 2);
    assert(c5 == std::vector<std::string>({"12", "13", "14", "23", "24", "34"}));

    // Test n=5, k=2 (first few and last)
    std::vector<std::string> c6 = generateCombinations(5, 2);
    assert(c6.front() == "12");
    assert(c6.back() == "45");
    assert(c6.size() == 10);

    // Test n=6, k=3 count = 20, verify first and last
    std::vector<std::string> c7 = generateCombinations(6, 3);
    assert(c7.size() == 20);
    assert(c7.front() == "123");
    assert(c7.back() == "456");

    // Test n=1, k=1
    std::vector<std::string> c8 = generateCombinations(1, 1);
    assert(c8 == std::vector<std::string>({"1"}));

    return 0;
}

// The solution uses an array `a` of size `k+1` (1-indexed) to hold the current combination. Initialize `a[i] = i` for `i=1..k`. In each iteration, copy the current combination into a string by concatenating each integer's decimal representation (using `std::to_string`) and push it to the result vector. Then apply the successor algorithm: start from the last index `i = k`; while `i >= 1` and `a[i] == n - k + i`, decrement `i`. If `i < 1`, all combinations are exhausted and the loop terminates. Otherwise, increment `a[i]` by 1, then for each subsequent index from `i+1` to `k`, set `a[pos] = a[pos-1] + 1` to maintain increasing order. This ensures lexicographic order. Edge cases: if `k == 0` or `k > n`, return an empty vector. If `k == n`, there is exactly one combination `"12...n"`; the loop runs once and then stops because `i` becomes 0 on the next call. Time complexity: The number of combinations is C(n, k), and each generation takes O(k) time to build the string and O(k) time for the successor step, so O(k * C(n, k)) overall. Space complexity: O(k) for the array plus O(k * C(n, k)) for the output strings (which is inherent to the result). The iterative approach avoids recursion stack overhead.
