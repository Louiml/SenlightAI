/*
Write a C++ function `printAllSubarrays` that takes a constant reference to a vector of integers and prints every contiguous subarray, one per line, in the order determined by fixing the starting index (from 0 to n-1) and then increasing the ending index (from start to n-1) for each start. Each subarray should be printed as its elements separated by spaces, with no trailing space. For example, for the input `{1, 2, 3}`, the output must be exactly: `1` then `1 2` then `1 2 3` then `2` then `2 3` then `3`, each on its own line. If the vector is empty, print nothing. The function must not return any value, but must produce the correct output via `std::cout`. Ensure proper handling of single-element and empty arrays.
*/
#include <vector>
#include <iostream>

// Prints all contiguous subarrays of the input vector, each on a new line.
// Elements within a subarray are separated by a single space.
void printAllSubarrays(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            for (int k = i; k <= j; ++k) {
                std::cout << arr[k];
                if (k != j) {
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }
    }
}
#include <cassert>
#include <sstream>

int main() {
    // Redirect cout to a string stream for testing
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    // Test 1: Empty array -> no output
    printAllSubarrays({});
    assert(oss.str() == "");

    // Test 2: Single element
    oss.str("");
    printAllSubarrays({7});
    assert(oss.str() == "7\n");

    // Test 3: Three elements
    oss.str("");
    printAllSubarrays({1, 2, 3});
    assert(oss.str() == "1\n1 2\n1 2 3\n2\n2 3\n3\n");

    // Test 4: Two elements with negative numbers
    oss.str("");
    printAllSubarrays({-1, 0});
    assert(oss.str() == "-1\n-1 0\n0\n");

    // Test 5: Four elements
    oss.str("");
    printAllSubarrays({5, 6, 7, 8});
    assert(oss.str() == "5\n5 6\n5 6 7\n5 6 7 8\n6\n6 7\n6 7 8\n7\n7 8\n8\n");

    // Restore original cout
    std::cout.rdbuf(old_cout);
    return 0;
}
// The algorithm is a direct triple-nested loop: the outermost loop selects the starting index `i` (from 0 to n-1), the middle loop selects the ending index `j` (from i to n-1), and the innermost loop iterates from `i` to `j`, printing each element followed by a space only if it is not the last element in that subarray (or simply print the element and then a space if it's not the last, and print the newline after the inner loop ends). Edge cases: empty vector — no output (the loops naturally do nothing). Single element — prints just that element and a newline. The time complexity is O(n³) because there are n(n+1)/2 subarrays and each subarray has an average length proportional to n; in the worst case, this is O(n³). Space complexity is O(1) auxiliary, as no extra data structures are used besides loop counters. The solution must use `const std::vector<int>&` to avoid copying and should print each element separated by a space, ensuring correct formatting with no trailing spaces.
