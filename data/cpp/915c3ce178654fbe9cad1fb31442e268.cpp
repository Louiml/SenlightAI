Write a C++ function named `countOccurrencesInFixedArray` that takes an integer value `target` and returns the number of times that value appears in a fixed 19-element integer array initialized with the values `{12, 3, 445, 56, 69, 56, 4, 334, 5, 6, 54, 45, 76, 65, 23, 434, 56, 34, 999}`. The function must use `const` correctly (marking the array as `const` to prevent modification) and return the total count of matches. The function should work for any valid integer input, including values that do not appear in the array (returning 0) and values that appear multiple times (returning the correct multiplicity). Do not use global variables; the array must be defined inside the function as a local `const` array. The function should not perform any input/output operations—only return the count.

int main() {
    assert(countOccurrencesInFixedArray(12) == 1);
    assert(countOccurrencesInFixedArray(56) == 3);
    assert(countOccurrencesInFixedArray(999) == 1);
    assert(countOccurrencesInFixedArray(0) == 0);
    assert(countOccurrencesInFixedArray(-1) == 0);
    assert(countOccurrencesInFixedArray(334) == 1);
    assert(countOccurrencesInFixedArray(445) == 1);
    assert(countOccurrencesInFixedArray(34) == 1);
    assert(countOccurrencesInFixedArray(1000) == 0);
    assert(countOccurrencesInFixedArray(3) == 1);
    return 0;
}

// Returns the number of times target appears in a fixed 19-element array.
int countOccurrencesInFixedArray(int target) {
    const int age[] = {12, 3, 445, 56, 69, 56, 4, 334, 5, 6, 54, 45, 76, 65, 23, 434, 56, 34, 999};
    int count = 0;
    for (int value : age) {
        if (value == target) {
            ++count;
        }
    }
    return count;
}

// The solution is straightforward: define a local `const int` array with the exact 19 elements from the spec. Then iterate through the array using a range-based for loop or index-based loop, comparing each element with the `target` parameter. For each match, increment a counter. After the loop, return the counter. Edge cases: if the target is not present, the counter remains at 0—this is correct. If the target appears multiple times (e.g., 56 appears three times), the counter correctly accumulates all occurrences. No special handling is needed for negative numbers, zero, or very large values, since integer comparison is exact. The time complexity is O(n) with n = 19 (constant, since the array size is fixed), and the space complexity is O(1) aside from the fixed array itself. The use of `const` on the array ensures we do not accidentally mutate it, and passing `target` by value is appropriate for a simple integer.
