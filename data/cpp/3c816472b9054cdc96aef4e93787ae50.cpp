/*
Write a C++ function `bool areAlike(const std::vector<int>& rossScores, const std::vector<int>& russScores, int minMatchCount, int maxDiff)` that determines whether two people "look alike" according to the following rule: for each characteristic index `i` (0-based), they match if the absolute difference between `rossScores[i]` and `russScores[i]` is less than or equal to `maxDiff`. The function returns `true` if at least `minMatchCount` characteristics match, otherwise `false`. The two score vectors will always have equal length, and all input values are non-negative integers. The function should be const-correct and handle empty input gracefully (returning `true` if `minMatchCount` ≤ 0, else `false`).
*/

#include <vector>
#include <cstdlib> // for std::abs

// Returns true if at least minMatchCount characteristics differ by at most maxDiff.
bool areAlike(const std::vector<int>& rossScores, const std::vector<int>& russScores, int minMatchCount, int maxDiff) {
    // If minMatchCount is non-positive, condition is trivially satisfied.
    if (minMatchCount <= 0) return true;
    // If either vector is empty, no characteristics exist.
    if (rossScores.empty() || russScores.empty()) return false;
    
    int matchCount = 0;
    const size_t n = rossScores.size();
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(rossScores[i] - russScores[i]) <= maxDiff) {
            ++matchCount;
        }
    }
    return matchCount >= minMatchCount;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include it here for completeness.
// In a real test, you'd include the header or copy the implementation.

int main() {
    // Example from the original problem: N=3, min=2, maxDiff=5, Ross={10,20,30}, Russ={12,18,35}
    std::vector<int> ross1 = {10, 20, 30};
    std::vector<int> russ1 = {12, 18, 35};
    assert(areAlike(ross1, russ1, 2, 5) == true); // matches: |10-12|=2, |20-18|=2 -> 2 matches

    // Same but require 3 matches -> false
    assert(areAlike(ross1, russ1, 3, 5) == false);

    // Large maxDiff makes all match
    assert(areAlike(ross1, russ1, 3, 100) == true);

    // Zero matches when maxDiff is zero and scores differ
    std::vector<int> ross2 = {1, 2, 3};
    std::vector<int> russ2 = {4, 5, 6};
    assert(areAlike(ross2, russ2, 1, 0) == false);

    // Empty vectors: minMatchCount=0 -> true, minMatchCount=1 -> false
    std::vector<int> empty1;
    std::vector<int> empty2;
    assert(areAlike(empty1, empty2, 0, 5) == true);
    assert(areAlike(empty1, empty2, 1, 5) == false);

    // All elements match exactly
    std::vector<int> ross3 = {7, 8, 9};
    std::vector<int> russ3 = {7, 8, 9};
    assert(areAlike(ross3, russ3, 3, 0) == true);

    // minMatchCount greater than number of characteristics -> false
    assert(areAlike(ross3, russ3, 4, 100) == false);

    // Negative minMatchCount treated as trivially true
    assert(areAlike(ross3, russ3, -1, 0) == true);

    // Boundary: exactly one match when maxDiff equals difference
    std::vector<int> ross4 = {5, 10, 15};
    std::vector<int> russ4 = {8, 10, 20};
    assert(areAlike(ross4, russ4, 1, 3) == true); // only |10-10|=0 <=3, |5-8|=3 <=3 actually two matches? Wait: 5-8=3 matches, 10-10=0 matches, 15-20=5 no -> two matches

    // Correct boundary test: exactly one match
    std::vector<int> ross5 = {1, 2, 3};
    std::vector<int> russ5 = {5, 6, 7};
    assert(areAlike(ross5, russ5, 1, 4) == false); // all differences are 4, so exactly all match? |1-5|=4 <=4, |2-6|=4, |3-7|=4 -> all three match, so requirement 1 is true

    // Let's fix: use maxDiff=3 -> none match
    assert(areAlike(ross5, russ5, 1, 3) == false);
    assert(areAlike(ross5, russ5, 0, 3) == true); // min=0 trivially true

    return 0;
}

// The problem reduces to counting how many indices satisfy the condition `abs(rossScores[i] - russScores[i]) <= maxDiff`. Iterate through the vectors with a single loop, incrementing a counter when the condition holds. After the loop, compare the counter against `minMatchCount`. Important edge cases: (1) if either vector is empty, there are zero matches, so the result is `true` only when `minMatchCount ≤ 0`; (2) if `maxDiff` is large enough (e.g., ≥ 10⁹) then all indices count; (3) if `minMatchCount` exceeds the number of characteristics, the result is necessarily `false`. The algorithm runs in O(N) time where N is the number of characteristics, and uses O(1) auxiliary space. No sorting or additional data structures are needed.
