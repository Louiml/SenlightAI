/*
Write a C++ function named `sortAndDisplaySteps` that performs a bubble sort on an integer array and returns a string containing every intermediate array state produced during the sorting process, including the initial unsorted array and each state after every comparison-and-swap step (but not the final sorted array, which is also produced as the last step). The function should accept a non-const integer array and its size, mutate the array in-place to sorted order, and return a single string where each state is a line with elements separated by spaces and lines separated by newline characters. The input array may contain duplicates, negative numbers, and any non-zero size; handle size 0 or 1 by returning the array's initial state as the only line. The final sorted array must be the last line of the returned string.
*/

#include <string>
#include <vector>
#include <sstream>

// Performs bubble sort on arr (in-place) and returns a string with each
// intermediate array state after every inner-loop step, starting from the
// initial unsorted array and ending with the fully sorted array.
// Each state is on a separate line, with elements separated by spaces.
std::string sortAndDisplaySteps(int arr[], int n) {
    std::ostringstream result;

    // Helper lambda to append the current array state to result.
    auto appendState = [&result, arr, n]() {
        for (int i = 0; i < n; ++i) {
            if (i > 0) result << ' ';
            result << arr[i];
        }
        result << '\n';
    };

    // Always include the initial state, even for n=0 or n=1.
    appendState();

    // Bubble sort: outer loop reduces the effective end from n down to 1.
    int currentLength = n;
    while (currentLength > 1) {
        // Inner loop compares adjacent pairs from index 0 to currentLength-2.
        for (int j = 0; j < currentLength - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
            // Record the state after every inner iteration.
            appendState();
        }
        --currentLength;
    }

    return result.str();
}

#include <cassert>
#include <string>

// The solution function is assumed to be declared above (or included here).

int main() {
    // Test 1: Basic sort with multiple states.
    {
        int arr[] = {5, 1, 4};
        std::string output = sortAndDisplaySteps(arr, 3);
        assert(arr[0] == 1 && arr[1] == 4 && arr[2] == 5);
        // Expected states: 
        // initial: 5 1 4
        // after j=0: 1 5 4
        // after j=1: 1 4 5
        // after j=0 (next outer): 1 4 5
        std::string expected = "5 1 4\n1 5 4\n1 4 5\n1 4 5\n";
        assert(output == expected);
    }

    // Test 2: Already sorted array still records all states.
    {
        int arr[] = {1, 2, 3};
        std::string output = sortAndDisplaySteps(arr, 3);
        // initial, j=0, j=1, j=0 — all same sorted
        std::string expected = "1 2 3\n1 2 3\n1 2 3\n1 2 3\n";
        assert(output == expected);
        assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3);
    }

    // Test 3: Single element returns just one line.
    {
        int arr[] = {42};
        std::string output = sortAndDisplaySteps(arr, 1);
        assert(output == "42\n");
        assert(arr[0] == 42);
    }

    // Test 4: Empty array (n=0) returns just an empty line.
    {
        int* arr = nullptr;
        std::string output = sortAndDisplaySteps(arr, 0);
        assert(output == "\n");
    }

    // Test 5: Array with duplicates.
    {
        int arr[] = {3, 1, 3, 2};
        std::string output = sortAndDisplaySteps(arr, 4);
        assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3 && arr[3] == 3);
        // Manually compute first few states: initial "3 1 3 2"
        // j=0: 1 3 3 2
        // j=1: 1 3 2 3 (swap)
        // j=2: 1 3 2 3 (no swap)
        // j=0 (next): 1 3 2 3 (no swap)
        // j=1: 1 2 3 3 (swap)
        // j=2: 1 2 3 3 (no swap)
        // j=0: 1 2 3 3 (no swap)
        // j=1: 1 2 3 3 (no swap)
        // j=2: 1 2 3 3 (no swap)
        // Total 10 lines including initial? Actually total states = 1 + 3 + 2 + 1 = 7? Let's count: outer passes: length=3 (j=0..2) -> 3 states after each, length=2 (j=0..1) -> 2 states, length=1 loop ends. So total states = initial + 3 + 2 = 6 lines. Check manually:
        // initial: 3 1 3 2
        // pass1 j0: 1 3 3 2
        // pass1 j1: 1 3 2 3
        // pass1 j2: 1 3 2 3
        // pass2 j0: 1 3 2 3
        // pass2 j1: 1 2 3 3
        // pass3 (length=1) doesn't run. But wait, outer loop runs while currentLength>1: after pass2, currentLength becomes 1, loop stops. So total states = 1 + 3 + 2 = 6 lines.
        std::string expected = "3 1 3 2\n1 3 3 2\n1 3 2 3\n1 3 2 3\n1 3 2 3\n1 2 3 3\n";
        assert(output == expected);
    }

    // Test 6: Negative numbers.
    {
        int arr[] = {-1, -5, 0};
        std::string output = sortAndDisplaySteps(arr, 3);
        assert(arr[0] == -5 && arr[1] == -1 && arr[2] == 0);
        // initial: -1 -5 0
        // j0: -5 -1 0
        // j1: -5 -1 0 (no swap)
        // j0: -5 -1 0
        std::string expected = "-1 -5 0\n-5 -1 0\n-5 -1 0\n-5 -1 0\n";
        assert(output == expected);
    }

    return 0;
}

// The solution replicates the exact behavior of the provided snippet's `display` calls inside the `bubbleSort` function. The algorithm performs standard bubble sort with an outer loop decreasing the number of comparisons from n-1 down to 1. For each inner iteration j from 0 to n-1 (where n is the current outer-loop value), we compare adjacent elements and swap if left > right. After every inner iteration—regardless of whether a swap occurred—we record the entire current array state into a string with spaces between elements and a newline after each state. Note that the final sorted array state is recorded when the last inner iteration completes, which matches the snippet's final `display` call inside the loop; the snippet then calls `display` again after sort, but that would produce a duplicate final line, so our function deliberately returns exactly the states recorded during the inner loop, ending with the sorted array. Edge cases: for size 0 or 1, the while loop body never executes, so we must explicitly append the initial (and final) array state as the only line. Time complexity is O(n²) comparisons and swaps in the worst case (reverse-sorted), O(n) best case (already sorted) because the loop still runs but no swaps happen, but we still record all states regardless. Space complexity is O(n²) for the returned string in the worst case because each state has up to n elements and we store up to n*(n-1)/2 states (each inner iteration produces a state), so the string size is O(n³) if we count characters? Actually each element is an integer with up to 10 digits, so total characters are O(n² * digits) which is O(n²) for typical small n; but theoretically O(n³) if counting all characters? Let's be precise: there are O(n²) states (since inner loop runs n-1 + n-2 + ... + 1 = n(n-1)/2 times), each state has n integers, each integer up to 10 digits, so total characters O(n * n²) = O(n³). But for practical n it's fine. We'll state O(n²) states and O(n) space for the temp buffer per state.
