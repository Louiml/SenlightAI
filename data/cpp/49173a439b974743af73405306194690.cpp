Write a C++ function `int maxCandiesEaten(vector<int>& candyType)` that determines the maximum number of different types of candies a person can eat, given that the person can eat at most `n/2` candies (where `n` is the total number of candies). The input vector contains integers representing candy types; some may be negative values, and types are guaranteed to be in the range [-100000, 100000]. The function should return the maximum possible number of distinct candy types the person can eat, which is the minimum between the total number of distinct types and `n/2`. Do not modify the input vector; use `const` correctness where appropriate.

The problem is a classic "maximum distinct count with a limit" scenario. The key observation is that the answer is the smaller of two numbers: (1) the total number of distinct candy types present, and (2) the maximum number of candies the person is allowed to eat, which is `n/2`. To count distinct types efficiently, use a boolean or integer array of size 200001 (since types range from -100000 to 100000). For each candy type `t`, if `t >= 0`, map it to index `t`; if `t < 0`, map to index `-t + 100000` to avoid negative indexing. Mark that index as visited if not already, and increment a distinct counter. Early exit if the distinct counter reaches `n/2`, since we can never eat more than that. Edge cases: an empty vector (though the problem likely guarantees non-empty, handle it gracefully by returning 0), all candies of the same type (answer is 1 if n/2 >=1), and negative types which require offset mapping. Time complexity is O(n) since we traverse the array once, and space complexity is O(1) extra (a fixed-size array of 200001 ints is constant memory relative to problem constraints).

#include <vector>
#include <cstring>

// Returns the maximum number of distinct candy types that can be eaten,
// given that at most candyType.size()/2 candies can be consumed.
int maxCandiesEaten(const std::vector<int>& candyType) {
    const int n = candyType.size();
    if (n == 0) return 0;

    const int maxAllowed = n / 2;
    int distinctCount = 0;

    // Array to mark visited types; indices 0..100000 for non-negative,
    // indices 100001..200000 for negative types (using offset 100000).
    bool seen[200001] = {false};

    for (int type : candyType) {
        if (distinctCount == maxAllowed) {
            return distinctCount; // Early exit if we've reached the limit
        }

        int idx;
        if (type >= 0) {
            idx = type;
        } else {
            idx = -type + 100000;
        }

        if (!seen[idx]) {
            seen[idx] = true;
            ++distinctCount;
        }
    }

    return distinctCount;
}

#include <cassert>
#include <vector>

// The solution function is defined above.
int main() {
    // Basic case with duplicate types and limit
    std::vector<int> v1 = {1, 1, 2, 2, 3, 3};
    assert(maxCandiesEaten(v1) == 3); // distinct = 3, n/2 = 3

    // All same type, n/2 = 1 (since n=2)
    std::vector<int> v2 = {5, 5};
    assert(maxCandiesEaten(v2) == 1);

    // Negative and positive mixed, n=4 -> limit 2
    std::vector<int> v3 = {-1, -1, 2, 3};
    assert(maxCandiesEaten(v3) == 2); // distinct = 3, but limit = 2

    // Zero cases: n=0 (though unusual, handle gracefully)
    std::vector<int> v4 = {};
    assert(maxCandiesEaten(v4) == 0);

    // Negative boundary values
    std::vector<int> v5 = {-100000, 100000, -99999, 99999};
    assert(maxCandiesEaten(v5) == 2); // distinct = 4, limit = 2

    // Limit exceeded by distinct count: n=6, distinct=5 -> answer=3
    std::vector<int> v6 = {1, 2, 3, 4, 5, 1};
    assert(maxCandiesEaten(v6) == 3);

    // Large n with all distinct, dictinct > n/2
    std::vector<int> v7 = {10, 20, 30, 40, 50, 60};
    assert(maxCandiesEaten(v7) == 3);

    // Single element
    std::vector<int> v8 = {42};
    assert(maxCandiesEaten(v8) == 0); // n/2 = 0

    return 0;
}
