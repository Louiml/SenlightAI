/*
Write a standalone C++ function `findShortestRun` that accepts a C-style array of integers (via pointer and size), a target integer `value`, and a reference parameter `runLength` to store the length of the shortest contiguous run (consecutive sequence) of exactly `value` in the array. The function must return the starting index (0-based) of that shortest run. If `value` does not appear in the array at all, set `runLength` to 0 and return `-1`. If multiple runs have the same minimal length, return the starting index of the first such run encountered. Handle edge cases including empty arrays (size 0), runs at the very beginning or end, runs of length 1, and cases where the entire array consists of a single run. The function must be `const`-correct (accept a pointer to const data) and must not modify the array.
*/

#include <cstddef> // for size_t

// Finds the shortest contiguous run of 'value' in arr[0..size-1].
// Sets 'runLength' to the length of that run (0 if none) and
// returns its starting index, or -1 if 'value' does not occur.
int findShortestRun(const int* arr, size_t size, int value, int& runLength) {
    runLength = 0;
    if (size == 0) {
        return -1;
    }

    int currentLen = 0;
    int bestLen = 0;
    int bestStart = -1;

    for (size_t i = 0; i < size; ++i) {
        if (arr[i] == value) {
            ++currentLen;
            // If we're not at the end, continue to extend the run.
            if (i < size - 1) {
                continue;
            }
        }

        // We reached a non-matching element OR the end of the array.
        // currentLen holds the length of the run that just ended.
        if (currentLen > 0) {
            if (bestLen == 0 || currentLen < bestLen) {
                bestLen = currentLen;
                // The run started at (i - currentLen + 1).
                bestStart = static_cast<int>(i) - currentLen + 1;
            }
            currentLen = 0;
        }
    }

    if (bestLen == 0) {
        return -1; // value not found
    }

    runLength = bestLen;
    return bestStart;
}

#include <cassert>

int main() {
    int arr1[] = {4,4,3,3,3,1,45,3,3,25,3,3,3,3,4,4,4,3};
    int len1 = 0;
    assert(findShortestRun(arr1, 18, 3, len1) == 8 && len1 == 2); // run at indices 8-9
    assert(findShortestRun(arr1, 18, 4, len1) == 14 && len1 == 3); // run at indices 14-16
    len1 = 0;
    assert(findShortestRun(arr1, 18, 17, len1) == -1 && len1 == 0); // not present

    int arr2[] = {5,5,5,2,5,5};
    len1 = 0;
    assert(findShortestRun(arr2, 6, 5, len1) == 4 && len1 == 2); // second run is shorter

    int arr3[] = {1};
    len1 = 0;
    assert(findShortestRun(arr3, 1, 1, len1) == 0 && len1 == 1); // single element

    int arr4[] = {2,2,2,2};
    len1 = 0;
    assert(findShortestRun(arr4, 4, 2, len1) == 0 && len1 == 4); // entire array is one run

    int arr5[] = {7,7,7,3,7,7,7,7,7};
    len1 = 0;
    assert(findShortestRun(arr5, 9, 7, len1) == 4 && len1 == 5); // middle run

    int arr6[] = {9,8,7};
    len1 = 0;
    assert(findShortestRun(arr6, 3, 9, len1) == 0 && len1 == 1); // run at start
    assert(findShortestRun(arr6, 3, 7, len1) == 2 && len1 == 1); // run at end

    int empty[] = {};
    len1 = 0;
    assert(findShortestRun(empty, 0, 4, len1) == -1 && len1 == 0); // empty array
}

// The core idea is to scan the array once, identifying contiguous segments where consecutive elements equal `value`. Maintain a `currentRunLength` counter that resets whenever a non-matching element is encountered. Track the smallest run length found so far in `minLength`, initialized to 0 (meaning "none found yet"), and the corresponding starting index `bestStart`. When a run ends (either by reaching a non-matching element or the end of the array), if `currentRunLength > 0` and either `minLength == 0` or `currentRunLength < minLength`, update `minLength` and compute the start as `endIndex - currentRunLength + 1`. Important edge cases: (1) empty array → return `-1` with `runLength=0`; (2) value not present → after full scan, if `minLength` remains 0, return `-1`; (3) a single-element run at index 0 or `size-1` is handled because the run-ending check occurs both before a mismatch and after the loop for a trailing run. The algorithm runs in `O(n)` time with `O(1)` auxiliary space, using only integer variables.
