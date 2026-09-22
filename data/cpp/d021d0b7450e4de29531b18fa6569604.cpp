/*
Write a C++ function that takes an array of student marks (as `const int[]` or `const int*` with a separate size parameter) and prints the indices (roll numbers, starting from 0) of students whose marks are strictly less than 35. The function should output these indices separated by spaces to the standard output, and should not print anything else. The array may contain any non-negative integer marks, and the size parameter is guaranteed to be at least 1. Handle the case of an empty array gracefully (print nothing). The function must be a free function, not a member of a class, and should be `const`-correct by taking the array as a pointer to const.
*/
#include <iostream>

// Prints indices (0-based) of students with marks strictly less than 35.
// Outputs indices separated by a single space, no trailing space.
// If there are no such students, prints nothing.
void printFailedRollNumbers(const int arr[], int size) {
    bool firstPrinted = false;
    for (int i = 0; i < size; ++i) {
        if (arr[i] < 35) {
            if (firstPrinted) {
                std::cout << ' ';
            }
            std::cout << i;
            firstPrinted = true;
        }
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Redirect cout to a stringstream for testing, then restore.
void runTest(const int arr[], int size, const std::string& expected) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printFailedRollNumbers(arr, size);
    std::cout.rdbuf(old);
    assert(buffer.str() == expected);
}

int main() {
    // Test 1: Example from original snippet
    int marks1[] = {99,96,95,85,31,36,32,45,32,30};
    runTest(marks1, 10, "4 6 8 9");

    // Test 2: No failed students
    int marks2[] = {35, 50, 100, 40};
    runTest(marks2, 4, "");

    // Test 3: All failed
    int marks3[] = {10, 20, 30, 34};
    runTest(marks3, 4, "0 1 2 3");

    // Test 4: Single element that fails
    int marks4[] = {0};
    runTest(marks4, 1, "0");

    // Test 5: Single element that passes (exactly 35)
    int marks5[] = {35};
    runTest(marks5, 1, "");

    // Test 6: Empty array (size 0) – should print nothing
    int marks6[] = {};
    runTest(marks6, 0, "");

    // Test 7: Marks with values far above and below
    int marks7[] = {100, -5, 34, 36, -1, 35};
    // Negative marks? Assumed non-negative, but test anyway: -5 and -1 fail
    runTest(marks7, 6, "1 2 4");

    // Test 8: Multiple fails with consecutive indices
    int marks8[] = {20, 21, 50, 22, 23};
    runTest(marks8, 5, "0 1 3 4");

    // Test 9: Mixed order, ensure no trailing space
    int marks9[] = {40, 30, 50, 20};
    runTest(marks9, 4, "1 3");

    // Test 10: Large size, spot check (not exhaustive but ensures no crash)
    int marks10[1000];
    for (int i = 0; i < 1000; ++i) {
        marks10[i] = (i % 2 == 0) ? 100 : 30; // odd indices fail
    }
    std::ostringstream expected10;
    for (int i = 1; i < 1000; i += 2) {
        if (i > 1) expected10 << ' ';
        expected10 << i;
    }
    runTest(marks10, 1000, expected10.str());

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution iterates through all elements of the array, checking if each mark is less than 35. If so, it prints the current index followed by a space. The main algorithm is simple linear scanning: for each position `i` from 0 to `size-1`, compare `arr[i]` with 35. For the output format, to avoid a trailing space when multiple indices are printed, we can track whether we've printed anything yet; before printing an index, if something has already been printed, print a leading space; otherwise print just the number. Alternatively, we can print all indices with a trailing space, but the problem statement implies "separated by spaces" so that's acceptable. However, a cleaner approach is to print a space before every index except the first one that is printed. Edge cases: if the array is empty (`size == 0`), we must do nothing. If no marks are below 35, we print nothing (no newline). If all marks are below 35, we print all indices from 0 to size-1. The time complexity is O(n) where n is the size of the array, since we visit each element exactly once. The space complexity is O(1), as we only use a few variables (loop counter and a boolean flag for space management). Const correctness is satisfied by declaring the parameter as `const int arr[]` or `const int* arr`, which prevents accidental modification of the marks inside the function.
