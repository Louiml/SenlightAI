Write a C++ function `countGoodPairs` that takes two integers `N` and `K`, followed by a vector of `N` strings representing student names (each name length between 1 and 20 characters), and returns the number of pairs `(i, j)` with `i < j` such that `j - i <= K` and the two names have equal length. The function should handle large `N` (up to 300,000) and `K` (up to `N`), and must return the result as a `long long` to avoid overflow. The names contain only lowercase English letters. For example, with `N=5, K=2` and names `"a", "bb", "a", "ccc", "bb"`, valid pairs are `(1,3)` (both length 1) and `(2,5)` (both length 2), but not `(1,? )` with index 4 because length differs; total is 2.
#include <cassert>
#include <vector>
#include <string>

// Declaration (from solution)
long long countGoodPairs(int N, int K, const std::vector<std::string>& names);

int main() {
    // Example from task
    std::vector<std::string> names1 = {"a", "bb", "a", "ccc", "bb"};
    assert(countGoodPairs(5, 2, names1) == 2);

    // All same length, K large -> all pairs
    std::vector<std::string> names2 = {"x", "y", "z"};
    assert(countGoodPairs(3, 10, names2) == 3);  // pairs: (1,2), (1,3), (2,3)

    // All same length, K=1 -> only adjacent pairs
    assert(countGoodPairs(3, 1, names2) == 2);  // (1,2), (2,3)

    // No pairs because lengths differ and K=1
    std::vector<std::string> names3 = {"a", "bb", "c"};
    assert(countGoodPairs(3, 1, names3) == 0);

    // Single name
    std::vector<std::string> names4 = {"hello"};
    assert(countGoodPairs(1, 5, names4) == 0);

    // Two names same length but far apart
    std::vector<std::string> names5 = {"ab", "cd", "ef", "gh"};
    assert(countGoodPairs(4, 2, names5) == 3);  // (1,2), (2,3), (3,4) but (1,3) not because distance =2? Actually distance (1,3)=2 <=2 yes, so (1,2), (1,3), (2,3), (2,4), (3,4) all valid? Let's compute: distances: (1,2)=1, (1,3)=2, (1,4)=3>2 invalid, (2,3)=1, (2,4)=2, (3,4)=1 -> total 5. So let's just assert 5.

    assert(countGoodPairs(4, 2, names5) == 5);
    // Test boundary: K=0 -> no pairs
    assert(countGoodPairs(3, 0, names2) == 0);

    // Large N stress (small scale)
    std::vector<std::string> names6(10, "a");
    // All pairs among 10 elements = 10*9/2 = 45
    assert(countGoodPairs(10, 100, names6) == 45);

    return 0;
}
#include <vector>
#include <queue>
#include <string>

// Count pairs (i, j) with i < j, j - i <= K, and equal name lengths.
long long countGoodPairs(int N, int K, const std::vector<std::string>& names) {
    std::queue<int> q[21];  // queues for lengths 1..20 (index 0 unused)
    long long cnt = 0;
    for (int i = 1; i <= N; ++i) {
        int len = static_cast<int>(names[i - 1].size());
        // Remove indices too far back from current index i.
        while (!q[len].empty() && i - q[len].front() > K) {
            q[len].pop();
        }
        // All remaining indices in q[len] are within distance K and have same length.
        cnt += static_cast<long long>(q[len].size());
        q[len].push(i);
    }
    return cnt;
}
// The solution uses 21 separate queues (one per possible name length, since max length is 20). We process each name in input order (1-indexed). For each name of length `L`, before counting pairs with previous same-length names, we remove from the queue `q[L]` all indices that are too far back (i.e., `i - q[L].front() > K`). After cleaning, the number of elements remaining in `q[L]` equals the number of previous indices with the same length that are within distance `K`. We add that count to the answer, then push the current index `i` into `q[L]`. This ensures each pair is counted exactly once when the later element is processed. Edge cases include: names of different lengths never form a pair; duplicate names count as separate pairs; when `K` overlaps all previous same-length names, we count them all. Time complexity is O(N) because each index is pushed once and popped at most once. Space complexity is O(N) in the worst case if many names share the same length and `K` is large, but bounded by `N` and 21 queues.
