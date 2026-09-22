Write a C++ function `printIndexedVector(const std::vector<int>& data, int itemsPerLine)` that prints each vector element as `index:value` pairs, separating pairs by a comma and a space, and inserts a newline every `itemsPerLine` pairs. The first pair must be preceded by `[` and the last pair followed by `]`. If `itemsPerLine` is less than 1, treat it as 1. If the vector is empty, print `[]`. The function returns `void` and must be `const`-correct, taking the vector by const reference.

#include <cassert>
#include <sstream>
#include <vector>

// Forward declaration to avoid including the solution twice.
void printIndexedVector(const std::vector<int>& data, int itemsPerLine);

int main() {
    // Capture output from std::cout for testing.
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Test 1: empty vector
    printIndexedVector({}, 5);
    assert(buffer.str() == "[]\n");
    buffer.str("");

    // Test 2: single element, itemsPerLine=3
    printIndexedVector({42}, 3);
    assert(buffer.str() == "[0:42]\n");
    buffer.str("");

    // Test 3: four elements, itemsPerLine=2
    printIndexedVector({10, 20, 30, 40}, 2);
    assert(buffer.str() == "[0:10, 1:20\n2:30, 3:40]\n");
    buffer.str("");

    // Test 4: three elements, itemsPerLine=1
    printIndexedVector({7, 8, 9}, 1);
    assert(buffer.str() == "[0:7\n1:8\n2:9]\n");
    buffer.str("");

    // Test 5: itemsPerLine=0 (should be treated as 1) with two elements
    printIndexedVector({1, 2}, 0);
    assert(buffer.str() == "[0:1\n1:2]\n");
    buffer.str("");

    // Test 6: five elements, itemsPerLine=3 (line break after third, then after fifth? No, after last no break)
    printIndexedVector({5, 6, 7, 8, 9}, 3);
    assert(buffer.str() == "[0:5, 1:6, 2:7\n3:8, 4:9]\n");
    buffer.str("");

    // Restore original std::cout
    std::cout.rdbuf(old);

    return 0;
}

#include <vector>
#include <iostream>

// Print vector elements as "index:value" pairs, with lines broken every itemsPerLine pairs.
void printIndexedVector(const std::vector<int>& data, int itemsPerLine) {
    // Treat non-positive itemsPerLine as 1 to avoid division by zero.
    const int perLine = (itemsPerLine < 1) ? 1 : itemsPerLine;

    std::cout << "[";
    const std::size_t n = data.size();
    for (std::size_t i = 0; i < n; ++i) {
        // Print separator before each element except the first.
        if (i != 0) {
            std::cout << ", ";
        }

        std::cout << i << ":" << data[i];

        // Insert a newline after every perLine pairs, except after the very last element.
        if ((i + 1) % perLine == 0 && i != n - 1) {
            std::cout << '\n';
        }
    }
    std::cout << "]\n";
}

// The solution iterates once through the vector, maintaining a running count of printed pairs. For each element at index `i`, print the formatted pair `i:value`. Before printing a pair other than the first, print a comma and space if the previous pair was not the last on its line; but simpler: print the comma before every pair except the first overall. After each pair, if the number of pairs printed so far is a multiple of `itemsPerLine` (and we haven't just printed the final element), print a newline. Edge cases: empty vector → print `[]` and return; `itemsPerLine` ≤ 0 → clamp to 1 so newlines still occur. Careful with the placement of newline: it should occur after every `itemsPerLine` pairs, but not after the last element if it completes a line? The spec says "inserts a newline every `itemsPerLine` pairs" – so after every multiple of `itemsPerLine` pairs, including the last if it happens to be a multiple. But typical output would not add a trailing newline after the closing `]`. So we must avoid printing newline after the closing bracket. Approach: print opening bracket, then loop over elements, print comma separator if not first element, then `index:value`, and if (i+1) % itemsPerLine == 0 and i != last index, print newline. After loop, print closing bracket. Time complexity O(n), space O(1). The main complexity is handling line breaks correctly without an extra newline after the last line.
