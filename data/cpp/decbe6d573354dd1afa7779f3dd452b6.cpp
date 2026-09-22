// Given an array of integers, write a C++ function `int countMaxOccurrences(const std::vector<int>& arr)` that returns the number of times the maximum value in the array appears. The function should handle arrays of any positive length, including negative and duplicate values. For example, for input `{3, 5, 2, 5, 5}`, the maximum is 5, and it appears 3 times, so the function returns 3. If the array has only one element, that element is the maximum, and the count is 1. Do not modify the input array.

#include <cassert>
#include <vector>

// Solution function is declared above, but for testing we include it here.
int countMaxOccurrences(const std::vector<int>& arr);

int main() {
    std::vector<int> test1 = {3, 5, 2, 5, 5};
    assert(countMaxOccurrences(test1) == 3);

    std::vector<int> test2 = {7};
    assert(countMaxOccurrences(test2) == 1);

    std::vector<int> test3 = {-1, -3, -2, -1};
    assert(countMaxOccurrences(test3) == 2); // max is -1

    std::vector<int> test4 = {4, 4, 4, 4};
    assert(countMaxOccurrences(test4) == 4);

    std::vector<int> test5 = {0, -5, 0, 10, 10, 10};
    assert(countMaxOccurrences(test5) == 3);

    std::vector<int> test6 = {100, 99, 98};
    assert(countMaxOccurrences(test6) == 1);

    std::vector<int> test7 = {};
    assert(countMaxOccurrences(test7) == 0);
}

#include <vector>
#include <algorithm>

// Count the number of occurrences of the maximum value in the array.
int countMaxOccurrences(const std::vector<int>& arr) {
    if (arr.empty()) return 0;

    // Find the maximum value in the array.
    int maxVal = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }

    // Count how many times the maximum appears.
    int count = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == maxVal) {
            ++count;
        }
    }
    return count;
}

// The original snippet tries to find the maximum value (by a flawed nested loop that sets a flag when any element is smaller than another, essentially detecting if the current element is the maximum), then counts its occurrences. The approach is inefficient and incorrect for some cases (e.g., when multiple maxima exist, it only sets `number` to the last maximum, which is fine, but the flag logic can be buggy if the array has a single element because `flag` remains 0 and `number` remains uninitialized). A clean solution is to first scan the array once to find the maximum value (initializing with the first element), then scan again to count how many times that maximum appears. This runs in O(n) time and O(1) auxiliary space. Edge cases: empty array is not allowed per task constraints, but if it were, we could return 0; all elements equal means count equals array size; negative numbers are handled naturally because we compare with `<` to update the maximum. The function should take a `const std::vector<int>&` to avoid copying and guarantee the input isn't modified.
