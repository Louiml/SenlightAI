// You are given an API function `bool isBadVersion(int version)` that returns `true` if a particular version of a product is defective (all versions after the first defective one are also defective) and `false` otherwise. Write a standalone C++ function named `firstBadVersion` (as a free function, not a class method) that takes an integer `n` (the total number of versions, numbered from 1 to `n`) and returns the lowest version number which is defective. The function must assume `isBadVersion` is already declared and accessible globally (you may declare it yourself for compilation, but do not define it). The product has at least one defective version, and `n` can be as large as 2^31 - 1. Your function must be efficient and must not rely on any linear scan over all versions. Ensure the function signature is `int firstBadVersion(int n)` and that it returns the correct index (1-based) of the first bad version.
The problem is equivalent to finding the first element in a boolean sequence `[false, false, ..., true, true, ...]` where all elements after the first `true` are `true`. Since the predicate `isBadVersion` is monotonic (once it becomes `true`, it stays `true`), binary search is optimal. We maintain a search interval `[low, high]` where `low` is a candidate for the first bad version (initially 1) and `high` is the upper bound (initially `n`). The loop invariant is that the first bad version lies in `[low, high]`. At each step, we compute `mid = low + (high - low) / 2` (using this form avoids integer overflow that could occur in `(low + high) / 2` for very large `n`). If `isBadVersion(mid)` returns `true`, then `mid` is a valid candidate, so we set `high = mid` to narrow the search to the left half. If it returns `false`, then all versions up to `mid` are good, so we set `low = mid + 1`. This process continues until `low == high`, at which point both pointers point to the first bad version. Edge cases include `n = 1` (immediately returns 1, but binary search handles it trivially), all versions good (but problem guarantees at least one bad), and the first bad version being exactly `n`. Time complexity is O(log n) because each iteration halves the search range. Space complexity is O(1) as we only use a few integer variables.
// Forward declaration of the API (assumed to be provided externally).
bool isBadVersion(int version);

// Returns the lowest version (between 1 and n) that is defective.
// Precondition: n >= 1 and at least one version in [1, n] is defective.
int firstBadVersion(int n) {
    int low = 1;
    int high = n;
    while (low < high) {
        // Avoid potential overflow of (low + high) by using this form.
        int mid = low + (high - low) / 2;
        if (isBadVersion(mid)) {
            // mid is defective, so the first bad version is at or before mid.
            high = mid;
        } else {
            // mid is good, so the first bad version must be after mid.
            low = mid + 1;
        }
    }
    return low; // low == high at termination
}
// For test purposes, we define a mock isBadVersion based on a global threshold.
static int badThreshold;

bool isBadVersion(int version) {
    return version >= badThreshold;
}

int main() {
    // Case 1: n=1, bad at 1
    badThreshold = 1;
    assert(firstBadVersion(1) == 1);

    // Case 2: n=5, bad at 4
    badThreshold = 4;
    assert(firstBadVersion(5) == 4);

    // Case 3: n=10, bad at 1 (first version is bad)
    badThreshold = 1;
    assert(firstBadVersion(10) == 1);

    // Case 4: n=10, bad at 10 (last version is bad)
    badThreshold = 10;
    assert(firstBadVersion(10) == 10);

    // Case 5: n=100, bad at 50
    badThreshold = 50;
    assert(firstBadVersion(100) == 50);

    // Case 6: n=2^31 - 1, bad at a very large value
    badThreshold = 2000000000;
    assert(firstBadVersion(2147483647) == 2000000000);

    // Case 7: n=7, bad at 3 (odd interval sizes)
    badThreshold = 3;
    assert(firstBadVersion(7) == 3);

    // Case 8: n=8, bad at 8
    badThreshold = 8;
    assert(firstBadVersion(8) == 8);
}
