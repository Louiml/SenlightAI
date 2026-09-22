// Write a C++ function `findClosestPair` that takes two sorted integer arrays (`first` and `second`), their respective lengths, and a target `sum`. The function should output (using `std::cout`) the pair consisting of one element from the first array and one element from the second array such that the absolute difference between their sum and the target `sum` is minimized. If multiple pairs have the same minimal difference, output the one that appears first when scanning the arrays in row-major order (i.e., with the outer loop over the first array and the inner loop over the second array). You may assume both arrays are non-empty and sorted in ascending order. The function should not return a value but instead print the result in the exact format: `The closest pair is [first_element, second_element]`.

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the solution function (already defined above in the solution section).
void findClosestPair(const int first[], const int second[], int lenFirst, int lenSecond, int sum);

// Helper to capture printed output from findClosestPair
std::string captureOutput(const int first[], const int second[], int lenFirst, int lenSecond, int sum) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    findClosestPair(first, second, lenFirst, lenSecond, sum);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

int main() {
    // Test case 1: from the problem statement
    int first1[] = {1, 8, 10, 12};
    int second1[] = {2, 4, 9, 15};
    assert(captureOutput(first1, second1, 4, 4, 11) == "The closest pair is [1, 9]");

    // Test case 2: from the problem statement
    int first2[] = {10, 12, 15, 18, 20};
    int second2[] = {1, 4, 6, 8};
    assert(captureOutput(first2, second2, 5, 4, 22) == "The closest pair is [18, 4]");

    // Test case 3: exact match
    int first3[] = {1, 5, 10};
    int second3[] = {2, 6, 9};
    assert(captureOutput(first3, second3, 3, 3, 11) == "The closest pair is [5, 6]");

    // Test case 4: multiple equidistant pairs; should pick first in row-major order
    int first4[] = {0, 10};
    int second4[] = {0, 10};
    // Pairs: (0,0)=0, (0,10)=10, (10,0)=10, (10,10)=20. Target=10 → first minimal is (0,10)
    assert(captureOutput(first4, second4, 2, 2, 10) == "The closest pair is [0, 10]");

    // Test case 5: single-element arrays
    int first5[] = {7};
    int second5[] = {3};
    assert(captureOutput(first5, second5, 1, 1, 10) == "The closest pair is [7, 3]");

    // Test case 6: negative numbers
    int first6[] = {-5, -1, 4};
    int second6[] = {-10, 2, 8};
    // Target=0 → best is (-1,2) sum=1 diff=1 or (4,-10) sum=-6 diff=6, so (-1,2)
    assert(captureOutput(first6, second6, 3, 3, 0) == "The closest pair is [-1, 2]");

    // Test case 7: large difference arrays
    int first7[] = {100, 200};
    int second7[] = {300, 400};
    assert(captureOutput(first7, second7, 2, 2, 1000) == "The closest pair is [200, 400]");

    // Test case 8: sum smaller than any possible pair sum
    int first8[] = {10, 20};
    int second8[] = {30, 40};
    assert(captureOutput(first8, second8, 2, 2, 1) == "The closest pair is [10, 30]");

    // Test case 9: duplicates within array
    int first9[] = {5, 5, 5};
    int second9[] = {1, 2};
    // Target=7 → (5,2) diff=0 is first found: i=0,j=1
    assert(captureOutput(first9, second9, 3, 2, 7) == "The closest pair is [5, 2]");

    // Test case 10: sum exactly in middle
    int first10[] = {1, 2, 3};
    int second10[] = {4, 5, 6};
    assert(captureOutput(first10, second10, 3, 3, 8) == "The closest pair is [2, 6]");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>
#include <cstdlib>

// Prints the pair from first and second whose sum is closest to the target sum.
// Arrays are sorted and non-empty. Time: O(n*m), Space: O(1).
void findClosestPair(const int first[], const int second[], int lenFirst, int lenSecond, int sum) {
    int bestI = 0;
    int bestJ = 0;
    for (int i = 0; i < lenFirst; ++i) {
        for (int j = 0; j < lenSecond; ++j) {
            if (std::abs(first[i] + second[j] - sum) < std::abs(first[bestI] + second[bestJ] - sum)) {
                bestI = i;
                bestJ = j;
            }
        }
    }
    std::cout << "The closest pair is [" << first[bestI] << ", " << second[bestJ] << "]";
}

// The problem is a classic two-array pair-sum search. Since the arrays are sorted, a more efficient two-pointer approach can be used, but the task explicitly mentions a brute-force solution with O(n·m) time (where n = lenFirst, m = lenSecond) and O(1) space. The algorithm initializes `k = 0` and `l = 0` to represent the best pair found so far. It then iterates over every combination of indices `i` (from first array) and `j` (from second array). For each pair, it computes `abs(first[i] + second[j] - sum)` and compares it with the current best value `abs(first[k] + second[l] - sum)`. If the new pair is strictly closer, update `k` and `l`. Because the comparison uses `<` and not `<=`, the first minimal pair in row-major order (outer loop over first, inner over second) is retained. Edge cases: empty arrays are not allowed per task; if all pairs are equidistant, the initial pair (first[0], second[0]) is printed. Time complexity is O(n·m), space is O(1) plus the output buffer. The solution uses `std::abs` from `<cstdlib>` and `std::cout` from `<iostream>`. Const correctness: the function accepts arrays as const pointers or const references, but since they are C-style arrays, we pass pointers and lengths; we mark them `const` to prevent modification.
