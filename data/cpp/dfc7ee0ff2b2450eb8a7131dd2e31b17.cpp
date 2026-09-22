// You are given an integer `n` representing the number of versions of a software product, labeled from `1` to `n`. There exists exactly one first "bad" version such that all versions after it are also bad, and all versions before it are good. You have access to a predefined function `bool isBadVersion(int version)` that returns `true` if the given version is bad and `false` otherwise. Write a C++ function named `firstBadVersion` that takes `n` as input, uses the least number of calls to `isBadVersion` possible, and returns the index of the first bad version. The function must handle the case where `n` can be as large as \(2^{31} - 1\), and you may assume that at least one bad version exists (i.e., `isBadVersion(n)` is `true`).
// The problem is a classic binary search on a monotonic predicate: the sequence of versions is sorted such that all good versions come before all bad versions. The goal is to find the leftmost index where `isBadVersion` returns `true`. We maintain two bounds: `left` (exclusive of any known bad) and `right` (inclusive of a known bad candidate). Initially, set `left = 0` and `right = n`, and keep a variable `result = n` as a fallback. In each iteration, compute `mid = left + (right - left) / 2` to avoid overflow. If `isBadVersion(mid)` is `true`, then `mid` is a candidate for the first bad version, so update `result = mid` and move `right = mid - 1` to search the left half. Otherwise, all versions up to `mid` are good, so move `left = mid + 1`. The loop continues while `left <= right`. At the end, `result` holds the smallest bad version found. Edge cases include `n = 1` (the only version is bad) and cases where the first bad version is `1`. The algorithm runs in \(O(\log n)\) time and uses \(O(1)\) auxiliary space, making only about \(\log_2 n\) calls to `isBadVersion`.
#include <cstdint>

// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

// Return the index of the first bad version in the range [1, n].
// Precondition: isBadVersion(n) == true.
int firstBadVersion(int n) {
    int left = 0;               // All versions <= left are known good
    int right = n;              // right is a candidate for the first bad
    int result = n;             // Default result if n itself is bad

    while (left <= right) {
        // Avoid overflow: (left + right) might overflow for large n
        int mid = left + (right - left) / 2;

        if (isBadVersion(mid)) {
            result = mid;       // mid is bad, so it might be the first
            right = mid - 1;    // Search the left half for an earlier bad
        } else {
            left = mid + 1;     // mid is good, so the first bad is to the right
        }
    }
    return result;
}
#include <cassert>

// Mock implementation of isBadVersion for testing.
// In real usage, this would be provided by the system.
static int first_bad = 0;

bool isBadVersion(int version) {
    return version >= first_bad;
}

int main() {
    // Test case 1: first bad is the last version (n = 10)
    first_bad = 10;
    assert(firstBadVersion(10) == 10);

    // Test case 2: first bad is version 1
    first_bad = 1;
    assert(firstBadVersion(1) == 1);
    assert(firstBadVersion(5) == 1);

    // Test case 3: first bad is in the middle
    first_bad = 4;
    assert(firstBadVersion(10) == 4);

    // Test case 4: first bad is near the start
    first_bad = 2;
    assert(firstBadVersion(100) == 2);

    // Test case 5: first bad is near the end with large n
    first_bad = 2147483647;
    assert(firstBadVersion(2147483647) == 2147483647);

    // Test case 6: n = 1 and version 1 is bad
    first_bad = 1;
    assert(firstBadVersion(1) == 1);

    // Test case 7: n = 2, first bad = 2
    first_bad = 2;
    assert(firstBadVersion(2) == 2);

    // Test case 8: n = 2, first bad = 1
    first_bad = 1;
    assert(firstBadVersion(2) == 1);

    // Test case 9: first bad is at position 7 in n=20
    first_bad = 7;
    assert(firstBadVersion(20) == 7);

    // Test case 10: first bad is at position 3 in n=3
    first_bad = 3;
    assert(firstBadVersion(3) == 3);

    return 0;
}
