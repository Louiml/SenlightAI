Write a C++ function named `sortedUnion` that takes two sorted integer arrays (`left` and `right`), their respective sizes (`leftSize` and `rightSize`), and an output array `result` (which must be large enough to hold the union of all elements from both arrays, including duplicates that appear in both inputs only once). The function should compute the set union of the two sorted sequences and write the result into `result` in sorted order (non-decreasing). The function must not assume the arrays have distinct elements internally; duplicates within a single input array should appear only once in the output. The function returns nothing (void) and modifies `result` in place. For example, given `left = {1, 2, 2, 3}` and `right = {2, 3, 4}`, the output should be `{1, 2, 3, 4}`. You may assume both input arrays are already sorted in non-decreasing order.

#include <cassert>
#include <cstddef>

// assume sortedUnion is defined above
int main() {
    int left1[] = {1, 2, 2, 3};
    int right1[] = {2, 3, 4};
    int res1[4];
    sortedUnion(left1, 4, right1, 3, res1);
    int expected1[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; ++i) assert(res1[i] == expected1[i]);

    int left2[] = {5, 5, 5};
    int right2[] = {5, 5};
    int res2[1];
    sortedUnion(left2, 3, right2, 2, res2);
    assert(res2[0] == 5);

    int left3[] = {10, 20};
    int right3[] = {15, 30};
    int res3[4];
    sortedUnion(left3, 2, right3, 2, res3);
    int expected3[] = {10, 15, 20, 30};
    for (int i = 0; i < 4; ++i) assert(res3[i] == expected3[i]);

    int left4[] = {};
    int right4[] = {1, 2};
    int res4[2];
    sortedUnion(left4, 0, right4, 2, res4);
    assert(res4[0] == 1 && res4[1] == 2);

    int left5[] = {1, 1, 2, 2};
    int right5[] = {2, 2, 3, 3};
    int res5[3];
    sortedUnion(left5, 4, right5, 4, res5);
    assert(res5[0] == 1 && res5[1] == 2 && res5[2] == 3);

    return 0;
}

#include <cstddef>

// Computes the set union of two sorted integer arrays into result.
// Assumes left and right are sorted in non-decreasing order.
// result must have enough capacity to hold all distinct elements.
void sortedUnion(const int* left, std::size_t leftSize,
                 const int* right, std::size_t rightSize,
                 int* result) {
    std::size_t i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] == right[j]) {
            result[k++] = left[i];
            // Skip all duplicates of this value in left
            while (i < leftSize && left[i] == result[k - 1]) ++i;
            // Skip all duplicates of this value in right
            while (j < rightSize && right[j] == result[k - 1]) ++j;
        } else if (left[i] < right[j]) {
            result[k++] = left[i];
            while (i < leftSize && left[i] == result[k - 1]) ++i;
        } else {
            result[k++] = right[j];
            while (j < rightSize && right[j] == result[k - 1]) ++j;
        }
    }

    while (i < leftSize) {
        result[k++] = left[i];
        while (i < leftSize && left[i] == result[k - 1]) ++i;
    }

    while (j < rightSize) {
        result[k++] = right[j];
        while (j < rightSize && right[j] == result[k - 1]) ++j;
    }
}

// The solution uses a standard two-pointer merge technique similar to `std::set_union`. Initialize two indices `i` and `j` to 0 for `left` and `right`, and an output index `k` to 0. While both indices are within bounds, compare `left[i]` and `right[j]`. If they are equal, write that value once to `result`, then increment both `i` and `j` to skip duplicates from both arrays (but also skip any further consecutive duplicates within each array to handle internal duplicates). If `left[i] < right[j]`, write `left[i]`, increment `i`, and skip any consecutive duplicate values in `left`. Otherwise, write `right[j]`, increment `j`, and skip consecutive duplicates in `right`. After one array is exhausted, copy the remaining elements of the other array while skipping consecutive duplicates. This ensures that each distinct value appears exactly once. The algorithm runs in O(leftSize + rightSize) time because each index moves forward at most the size of its array, and uses O(1) auxiliary space (excluding the output array). Edge cases include empty arrays (which produce an empty result), arrays with all identical values, and arrays with overlapping ranges.
