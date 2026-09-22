/*
Write a C++ function that takes a 2D array (represented as a pointer to pointer of integers), along with its row and column counts, and returns a struct or pair containing the sum of all even elements and the sum of all odd elements. Access the array elements only through pointer arithmetic (i.e., using `*(*(array + i) + j)`), not subscript notation. The function must handle empty arrays (zero rows or columns) gracefully, returning zero sums. The sums may exceed the range of `int` if the array contains many large values, but for the scope of this task assume the total sum fits within a 64-bit signed integer. Use `long long` for the sums to be safe. Apply `const` correctness by marking the array pointer as `const int* const*` in the function signature (or equivalently `const int** const`), so the function does not modify the array.
*/

#include <cstddef>

struct EvenOddSums {
    long long even;
    long long odd;
};

// Compute sums of even and odd elements in a 2D array using pointer notation only.
// The array is passed as a pointer to const pointers to const int (not modified).
EvenOddSums sumEvenAndOdd(const int* const* array, std::size_t rows, std::size_t cols) {
    long long evenSum = 0;
    long long oddSum = 0;

    for (std::size_t i = 0; i < rows; ++i) {
        // array[i] is a pointer to the first element of row i. Using *(array+i) gets that pointer.
        const int* row = *(array + i);
        for (std::size_t j = 0; j < cols; ++j) {
            // Access element via pointer arithmetic: *(row + j) or equivalently *(*(array + i) + j)
            int value = *(row + j);
            if (value % 2 == 0) {
                evenSum += value;
            } else {
                // value % 2 could be -1 for negative odd, so use != 0
                oddSum += value;
            }
        }
    }

    return {evenSum, oddSum};
}

#include <cassert>

int main() {
    // Test case 1: Basic mixed array
    int row1[] = {1, 2, 3};
    int row2[] = {4, 5, 6};
    int row3[] = {7, 8, 9};
    int* rows1[] = {row1, row2, row3};
    const int* const* arr1 = rows1;
    EvenOddSums res1 = sumEvenAndOdd(arr1, 3, 3);
    assert(res1.even == 20); // 2+4+6+8 = 20
    assert(res1.odd == 25);  // 1+3+5+7+9 = 25

    // Test case 2: Negative numbers (odd detection must handle negatives)
    int nrow1[] = {-3, -2};
    int nrow2[] = {-1, 0};
    int* nrows[] = {nrow1, nrow2};
    const int* const* arr2 = nrows;
    EvenOddSums res2 = sumEvenAndOdd(arr2, 2, 2);
    assert(res2.even == -2); // -2 + 0 = -2
    assert(res2.odd == -4);  // -3 + -1 = -4

    // Test case 3: Empty array (0 rows)
    const int* const* arr3 = nullptr;
    EvenOddSums res3 = sumEvenAndOdd(arr3, 0, 5);
    assert(res3.even == 0);
    assert(res3.odd == 0);

    // Test case 4: Zero columns (non-null rows) – loops do not run
    int single[] = {10};
    int* rowArr[] = {single};
    const int* const* arr4 = rowArr;
    EvenOddSums res4 = sumEvenAndOdd(arr4, 1, 0);
    assert(res4.even == 0);
    assert(res4.odd == 0);

    // Test case 5: Single element even
    int e1[] = {42};
    int* eRow[] = {e1};
    const int* const* arr5 = eRow;
    EvenOddSums res5 = sumEvenAndOdd(arr5, 1, 1);
    assert(res5.even == 42);
    assert(res5.odd == 0);

    // Test case 6: Single element odd
    int o1[] = {99};
    int* oRow[] = {o1};
    const int* const* arr6 = oRow;
    EvenOddSums res6 = sumEvenAndOdd(arr6, 1, 1);
    assert(res6.even == 0);
    assert(res6.odd == 99);

    // Test case 7: Large values (fits in long long)
    int big1[] = {1000000000, 1000000000};
    int big2[] = {1000000000, 1000000000};
    int* bigRows[] = {big1, big2};
    const int* const* arr7 = bigRows;
    EvenOddSums res7 = sumEvenAndOdd(arr7, 2, 2);
    assert(res7.even == 4000000000LL); // 4 billion
    assert(res7.odd == 0);

    // Test case 8: All zeros (even)
    int zero1[] = {0, 0};
    int* zeroRows[] = {zero1};
    const int* const* arr8 = zeroRows;
    EvenOddSums res8 = sumEvenAndOdd(arr8, 1, 2);
    assert(res8.even == 0);
    assert(res8.odd == 0);

    return 0;
}

// The solution iterates over every cell of the 2D array using nested loops. For each element, we access it via pointer arithmetic: `*(*(array + i) + j)`. We check if the element is even using the modulo operator (`% 2 == 0`); for odd, we check `% 2 != 0` (or `% 2 == 1`, but this fails for negative odd numbers because in C++ negative odd numbers like -3 give `-3 % 2 == -1`). Thus, use `% 2 != 0` for odd detection to be correct for negatives. Edge cases: if `r > 0` and `l == 0` (or vice versa), the loops simply do not execute, sums remain zero. If both are zero, also zero. The time complexity is O(r*l) since each element is visited exactly once. Space complexity is O(1) beyond the input array, as we only store two accumulators. Use `long long` to avoid overflow.
