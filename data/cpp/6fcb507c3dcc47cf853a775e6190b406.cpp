/*
Write a C++ function named `countExceedingSuffixMin` that, given a non-empty vector of integers, returns the number of elements (excluding the last element) that are strictly greater than the smallest value appearing to their right in the vector. For example, for the vector `{3, 1, 4, 2, 5}`, the smallest value to the right of `3` is `1` (so 3 > 1 → count), to the right of `1` is `2` (1 is not > 2 → no count), to the right of `4` is `2` (count), to the right of `2` is `5` (2 is not > 5 → no count), and the last element `5` is never considered. The result would be `2`. The input vector may contain duplicate values, negative numbers, and any size from 1 upward. The function must not modify the input vector and must be const-correct.
*/
#include <vector>
#include <algorithm>

// Return the number of elements (excluding the last) that are strictly
// greater than the smallest value appearing to their right in the vector.
int countExceedingSuffixMin(const std::vector<int>& values) {
    if (values.size() <= 1) {
        return 0;
    }

    int count = 0;
    int currentMinimum = values.back(); // suffix min for the last element is itself

    // Iterate from second-to-last down to the first element
    for (int i = static_cast<int>(values.size()) - 2; i >= 0; --i) {
        if (values[i] > currentMinimum) {
            ++count;
        } else {
            // currentMinimum remains unchanged if values[i] is not smaller
        }
        currentMinimum = std::min(currentMinimum, values[i]);
    }

    return count;
}
#include <cassert>
#include <vector>

// Declaration (expected to be provided by the solution header)
int countExceedingSuffixMin(const std::vector<int>& values);

int main() {
    // Basic case with mixed numbers
    assert(countExceedingSuffixMin({3, 1, 4, 2, 5}) == 2);

    // Single element: no comparisons possible
    assert(countExceedingSuffixMin({7}) == 0);

    // All elements are equal: none is strictly greater than suffix min
    assert(countExceedingSuffixMin({4, 4, 4, 4}) == 0);

    // Strictly increasing: every element except last is greater than its suffix min
    assert(countExceedingSuffixMin({1, 2, 3, 4}) == 3);

    // Strictly decreasing: no element is greater than the smallest to its right (the last)
    assert(countExceedingSuffixMin({5, 4, 3, 2}) == 0);

    // Negative numbers and duplicates
    assert(countExceedingSuffixMin({-3, -1, -2, -1, 0}) == 3); // -3 > -1? no; -1 > -2? yes; -2 > -1? no; -1 > 0? no → count=1? wait recompute: 
    // Let's compute: suffix min after index4=0; i=3: -1 > 0? no; min=min(0,-1)=-1; i=2: -2 > -1? no; min=min(-1,-2)=-2; i=1: -1 > -2? yes count=1; min=min(-2,-1)=-2; i=0: -3 > -2? no; total count=1. So assert should be 1, not 3. Correct the assertion.
    assert(countExceedingSuffixMin({-3, -1, -2, -1, 0}) == 1);

    // Larger vector with duplicates and negatives
    assert(countExceedingSuffixMin({10, 5, 5, 1, 1, 2}) == 2); // 10>1? yes (count1); 5>1? yes (count2); 5>1? yes? but 5>1 yes? actually suffix min after index4 is 1; 5>1 yes but wait: Let's trace: suffix min after last=2; i=4:1>2? no; min=min(2,1)=1; i=3:1>1? no; min=1; i=2:5>1? yes count=1; min=min(1,5)=1; i=1:5>1? yes count=2; min=1; i=0:10>1? yes count=3. So result=3. Adjust assert to 3.

    assert(countExceedingSuffixMin({10, 5, 5, 1, 1, 2}) == 3);

    // Two elements: check both cases
    assert(countExceedingSuffixMin({3, 2}) == 0); // 3>2? yes! Actually 3>2 yes → count=1. Wait: last is 2, previous is 3, 3>2 yes → count=1. Correct to 1.
    assert(countExceedingSuffixMin({3, 2}) == 1);
    assert(countExceedingSuffixMin({2, 3}) == 0); // 2>3? no

    return 0;
}
// The key observation is that for each element except the last, we need to compare it with the minimum of all elements strictly to its right. Instead of recomputing the suffix minimum for each element (which would be O(n²)), we can traverse the vector from right to left while maintaining the current suffix minimum. Start with `currentMinimum` initialized to the value of the last element. Then iterate from the second‑to‑last element down to the first. For each element, if it is strictly greater than `currentMinimum`, we increment the counter; otherwise, we leave the counter unchanged. After the comparison, we update `currentMinimum` to be the minimum of its current value and the current element (i.e., `currentMinimum = std::min(currentMinimum, a[k])`). This ensures that for the next element to the left, `currentMinimum` holds the smallest value among all elements to its right. Important edge cases: if the vector has only one element, there are no elements to compare, so the result is 0. Duplicate values work naturally because we use strictly greater; for example, `{5,5}` gives 0 because the first 5 is not greater than the suffix minimum (which is the second 5). Negative numbers also work because comparisons operate on integer values. Time complexity is O(n) for a vector of size n, and auxiliary space is O(1) because we only use a few integer variables. The algorithm is optimal as it processes each element once.
