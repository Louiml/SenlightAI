Write a standalone C++ function named `recursiveArraySum` that takes a `const std::vector<int>&` and returns the sum of all elements using recursion (no loops, no `std::accumulate`). The function should handle empty vectors by returning `0`, and must work correctly for negative values, zero, and large positive integers within `int` range. Use a private helper function or a default parameter to track the current index. The solution must be purely recursive, with the public function serving as the entry point that calls the recursive helper.

#include <cassert>
#include <vector>

// Assume the provided solution functions are included above.

int main() {
    std::vector<int> empty;
    assert(recursiveArraySum(empty) == 0);

    std::vector<int> single = {5};
    assert(recursiveArraySum(single) == 5);

    std::vector<int> mixed = {3, -1, 2, 4, 5};
    assert(recursiveArraySum(mixed) == 13);

    std::vector<int> negatives = {-1, -2, -3};
    assert(recursiveArraySum(negatives) == -6);

    std::vector<int> zeros = {0, 0, 0, 0};
    assert(recursiveArraySum(zeros) == 0);

    std::vector<int> large = {1000, 2000, -500, 1};
    assert(recursiveArraySum(large) == 2501);

    std::vector<int> alternating = {1, -1, 2, -2, 3, -3};
    assert(recursiveArraySum(alternating) == 0);

    return 0;
}

#include <vector>

// Recursive helper that sums elements from index idx to the end.
int recursiveSumHelper(const std::vector<int>& v, int idx) {
    if (idx == static_cast<int>(v.size())) {
        return 0;
    }
    return v[idx] + recursiveSumHelper(v, idx + 1);
}

// Public function: returns the sum of all elements in the vector.
// Uses recursion only; returns 0 for an empty vector.
int recursiveArraySum(const std::vector<int>& v) {
    return recursiveSumHelper(v, 0);
}

// The main algorithm is straightforward recursion: at each step, add the current element to the sum of the remaining elements, and stop when the index reaches the vector’s size. The recursive function signature `int recursiveSumHelper(const vector<int>& v, int idx)` returns the sum from `idx` to the end. Base case: if `idx == v.size()`, return `0`. Recursive case: return `v[idx] + recursiveSumHelper(v, idx+1)`. Edge cases include an empty vector (base case returns `0` directly), negative numbers (adding them works naturally), and large values that might overflow `int`—but we assume inputs stay within `int` range as specified. Time complexity is `O(n)` with `n` being the number of elements, because each element is visited exactly once. Space complexity is `O(n)` due to the recursion stack depth growing linearly with input size. The solution uses `const` references to avoid copying and to enforce read-only access.
