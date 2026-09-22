/*
Write a C++ function named `majorityResult` that takes a vector of integers and returns an integer indicating the majority outcome according to the following rule: if the vector contains at least one `1`, return `-1`; otherwise (i.e., all elements are `0`), return `1`. This is a simplified abstraction of a voting or control-flow problem where the presence of any special flag flips the result. The function should handle both empty and non-empty vectors gracefully: for an empty vector, assume the "all zeros" case and return `1`. You may not use any standard library algorithms other than simple iteration; the function must be self-contained with only the necessary headers.
*/
#include <vector>

// Return -1 if vector contains any 1, otherwise return 1.
int majorityResult(const std::vector<int>& values) {
    for (const int value : values) {
        if (value == 1) {
            return -1;
        }
    }
    return 1;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (must be visible to main)
int majorityResult(const std::vector<int>& values);

int main() {
    // Contains a 1 -> returns -1
    assert(majorityResult({1}) == -1);
    // All zeros -> returns 1
    assert(majorityResult({0, 0, 0}) == 1);
    // Mixed with at least one 1 -> returns -1
    assert(majorityResult({0, 0, 1, 0}) == -1);
    // Empty vector treated as all zeros -> returns 1
    assert(majorityResult({}) == 1);
    // Single zero -> returns 1
    assert(majorityResult({0}) == 1);
    // Large vector with 1 at end -> returns -1
    assert(majorityResult({0, 0, 0, 0, 0, 1}) == -1);
    // Multiple ones -> returns -1
    assert(majorityResult({1, 0, 1}) == -1);
    // No ones but other values (should be treated as not 1)
    assert(majorityResult({2, 3, 0}) == 1);
}
// The algorithm is straightforward: iterate through the entire vector once, checking each element for a value of `1`. As soon as a `1` is found, immediately return `-1` without inspecting remaining elements, because the presence of any `1` determines the result. If the loop completes without finding any `1`, return `1`. This covers edge cases: an empty vector (no `1`s, so return `1`), vectors with only zeros (return `1`), vectors with at least one `1` among zeros (return `-1`), and even negative numbers or other values—but per the task specification, only `0` and `1` are expected, so other values are treated as "not `1`" and do not affect the condition. Time complexity is \(O(n)\) in the worst case (when no `1` exists), but \(O(1)\) on average if a `1` appears early; space complexity is \(O(1)\) auxiliary, as no extra containers are used.
