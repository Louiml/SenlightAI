// You are given an integer `n` representing a sequence of versions numbered `1` through `n`, where each version is built on the previous one. A quality check reveals that from some unknown first version onward, all subsequent versions are defective. You have access to a hidden boolean function `isBadVersion(int version)` (already implemented elsewhere) that returns `true` if the given version is defective and `false` otherwise. Write a C++ function `int firstBadVersion(int n)` that returns the smallest version number that is defective. Your implementation must minimize the number of calls to `isBadVersion`; specifically, you must use a binary search approach. Assume `n >= 1` and that at least one version is defective (so a valid answer always exists). The function should be self-contained, have no side effects, and avoid using recursive calls to prevent stack overhead.

#include <cassert>

// Mock isBadVersion: in test code we define a static threshold.
static int bad_threshold;

bool isBadVersion(int version) {
    return version >= bad_threshold;
}

int main() {
    // Case 1: n=1, first bad is 1
    bad_threshold = 1;
    assert(firstBadVersion(1) == 1);

    // Case 2: n=5, first bad is 3
    bad_threshold = 3;
    assert(firstBadVersion(5) == 3);

    // Case 3: n=10, first bad is 1
    bad_threshold = 1;
    assert(firstBadVersion(10) == 1);

    // Case 4: n=10, first bad is 10 (last version)
    bad_threshold = 10;
    assert(firstBadVersion(10) == 10);

    // Case 5: Large n=1000000, first bad is 500001
    bad_threshold = 500001;
    assert(firstBadVersion(1000000) == 500001);

    // Case 6: Consecutive bad from early point: n=7, first bad is 2
    bad_threshold = 2;
    assert(firstBadVersion(7) == 2);

    // Case 7: All good except last: n=6, first bad is 6
    bad_threshold = 6;
    assert(firstBadVersion(6) == 6);

    return 0;
}

#include <cstddef>  // not strictly needed but kept for clarity

// Predicate: returns true if version is defective. In a real setting this is
// provided externally. Here we declare it for linkage.
bool isBadVersion(int version);

// Return the first (smallest) version number that is defective.
// Uses binary search to minimize calls to isBadVersion.
int firstBadVersion(int n) {
    int low = 1;
    int high = n;
    while (low < high) {
        // Compute mid without overflow: low + (high - low) / 2
        int mid = low + (high - low) / 2;
        if (isBadVersion(mid)) {
            // This version is bad, so the first bad is at or before mid.
            high = mid;
        } else {
            // This version is good, so the first bad is after mid.
            low = mid + 1;
        }
    }
    // low == high is the first bad version.
    return low;
}

// The problem is a classic binary search for the leftmost element satisfying a predicate that is monotonic: once `isBadVersion(k)` becomes `true`, it remains `true` for all `k+1, k+2, ... n`. The standard approach is to maintain two indices `low = 1` and `high = n`, and repeatedly examine the middle element. If the middle is bad, then the first bad version must be at or before that middle, so we move `high = mid`. If the middle is good, the first bad version is strictly after it, so we move `low = mid + 1`. The loop continues while `low < high`. When `low == high`, that index is the first bad version. This variant avoids the subtle off‑by‑one errors that occur when using inclusive bounds with `mid = low + (high - low)/2` and adjusting `high = mid` instead of `mid - 1`. The algorithm guarantees that the invariant "low is always a good version (or 1) and high is always a bad version" is maintained, but the simpler invariant `low` is a candidate and `high` is a candidate with the property that all versions before `low` are good and all versions after `high` are bad. When `low == high`, it must be the boundary. For `n=1`, the loop does not run and returns `1`, which is correct because we assume at least one bad version exists. For `n=2`, if version 1 is bad, the loop finds `low=1, high=2`, `mid=1`, condition `isBad(1)` true → high=1, loop ends, returns 1. If version 1 is good and 2 is bad, `mid=1` false → low=2, returns 2. Each iteration halves the search interval, so the number of `isBadVersion` calls is `O(log n)`. The space complexity is `O(1)` (only a few integer variables).
