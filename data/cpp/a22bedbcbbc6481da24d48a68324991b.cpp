// Write a C++ function that takes two integers `n` and `k` and a vector of `n` integers, and prints all unique combinations of size `k` from the set of distinct values in the input vector, in lexicographically sorted order (based on the sorted distinct values). Each combination must be printed on a separate line with elements separated by single spaces. The input may contain duplicate values, but the output must treat the distinct values only. The function should handle cases where `k` is greater than the number of distinct values by producing no output.
// The problem requires generating combinations without repetition from the set of unique elements. Since duplicates in the input are ignored, first they are removed by inserting all values into a `std::map`, which also sorts them in ascending order. The distinct values are stored in a vector for index-based access. A typical backtracking approach is used: maintain an array `a` of size `k+1` (1-indexed) where `a[i]` stores the 0-based index (into the distinct-values vector) of the element chosen for position `i`. At each recursive step `i`, we iterate `j` from `a[i-1]+1` to `n - k + i` (where `n` now denotes the number of distinct values). This ensures strictly increasing indices to avoid permutations of the same combination. When `i` reaches `k`, we print the corresponding values. Edge cases: if `k` is 0, there is exactly one empty combination (print an empty line); if `k` > number of distinct values, the loop bounds will be invalid and no combination is printed. The time complexity is \(O(C(m,k) \cdot k)\) where \(m\) is the number of distinct values (up to `n`), because each generated combination of size `k` requires `k` operations to print. The space complexity is \(O(m + k)\) for the map, vector, and recursion stack depth `k`.
#include <vector>
#include <map>
#include <iostream>

// Print all unique combinations of size k from the distinct values in input.
// Each combination is printed on one line, values separated by spaces.
// The combinations are generated in lexicographic order of the sorted distinct values.
// If k is 0, prints an empty line (representing the empty combination).
// If k > number of distinct values, prints nothing.
void printUniqueCombinations(const std::vector<int>& input, int k) {
    // Build a sorted map of distinct values.
    std::map<int, int> distinctMap; // value -> count (count not needed but appends sorting)
    for (int value : input) {
        distinctMap[value] = 1;
    }

    // Extract distinct values into a vector (1-indexed for convenience in recursion).
    std::vector<int> values;
    values.push_back(0); // dummy at index 0
    for (const auto& pair : distinctMap) {
        values.push_back(pair.first);
    }

    int m = static_cast<int>(values.size()) - 1; // number of distinct values
    if (k > m) {
        return; // No combinations possible
    }

    if (k == 0) {
        std::cout << "\n";
        return;
    }

    // Recursive lambda to generate combinations.
    std::vector<int> chosen(k + 1, 0); // chosen[i] = index into values[], 1-based positions
    std::function<void(int)> generate = [&](int pos) {
        // pos is the current position (1..k) to fill
        for (int j = chosen[pos - 1] + 1; j <= m - (k - pos); j++) {
            chosen[pos] = j;
            if (pos == k) {
                // Print one combination
                for (int i = 1; i <= k; i++) {
                    std::cout << values[chosen[i]];
                    if (i < k) std::cout << " ";
                }
                std::cout << "\n";
            } else {
                generate(pos + 1);
            }
        }
    };

    chosen[0] = 0; // sentinel
    generate(1);
}
#include <cassert>
#include <sstream>
#include <iostream>

// Helper to capture output of printUniqueCombinations into a string.
std::string captureOutput(const std::vector<int>& input, int k) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printUniqueCombinations(input, k);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Test 1: basic unique values
    assert(captureOutput({1, 2, 3}, 2) == "1 2\n1 3\n2 3\n");

    // Test 2: duplicates in input should be ignored
    assert(captureOutput({3, 1, 3, 2, 1}, 2) == "1 2\n1 3\n2 3\n");

    // Test 3: k=0 prints empty line
    assert(captureOutput({5, -1, 3}, 0) == "\n");

    // Test 4: k > number of distinct values -> no output
    assert(captureOutput({1, 2}, 3) == "");

    // Test 5: single distinct value, k=1
    assert(captureOutput({7, 7, 7}, 1) == "7\n");

    // Test 6: negative numbers sorted lexicographically
    assert(captureOutput({-3, -10, 0, 5}, 2) == "-10 -3\n-10 0\n-10 5\n-3 0\n-3 5\n0 5\n");

    // Test 7: k=1 with multiple distinct values
    assert(captureOutput({4, 2, 9, 2}, 1) == "2\n4\n9\n");

    // Test 8: all values equal, k=2
    assert(captureOutput({1, 1}, 2) == "");

    // Test 9: larger combination count
    assert(captureOutput({1, 2, 3, 4}, 3) == "1 2 3\n1 2 4\n1 3 4\n2 3 4\n");

    // Test 10: empty input
    assert(captureOutput({}, 1) == "");

    return 0;
}
