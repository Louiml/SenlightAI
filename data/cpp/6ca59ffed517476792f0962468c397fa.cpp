// Write a C++ function `minDeletionSize` that takes a vector of strings of equal length and returns the minimum number of columns that must be deleted so that each remaining column is sorted in non-decreasing lexicographic order from top to bottom. A column is defined as a vertical sequence of characters taken from each string at the same index. If a column is not sorted (i.e., there exists an adjacent pair where the upper character is greater than the lower character), it must be deleted. The input strings contain only lowercase English letters.
// The solution iterates column by column (by index from 0 to length-1). For each column, it checks all adjacent rows (from index 1 to n-1) to see if `strs[i][col] < strs[i-1][col]`; if any such violation is found, the column is unsorted, so we increment the deletion count and break out of the inner loop (no need to check further rows for this column). Since all strings have equal length, we can safely access `strs[i][col]`. Edge cases: if the vector is empty or strings are empty, there are no columns, so return 0; if there is only one string, every column is trivially sorted (no adjacent pairs), so return 0. Time complexity is O(n * m), where n is number of strings and m is the common string length. Space complexity is O(1) extra space.
#include <vector>
#include <string>

// Given a vector of equal-length strings, return the minimum number of columns
// that must be deleted so that each remaining column is non-decreasing vertically.
int minDeletionSize(const std::vector<std::string>& strs) {
    if (strs.empty() || strs[0].empty()) return 0;

    const int rows = static_cast<int>(strs.size());
    const int cols = static_cast<int>(strs[0].length());
    int deletions = 0;

    for (int col = 0; col < cols; ++col) {
        for (int row = 1; row < rows; ++row) {
            if (strs[row][col] < strs[row - 1][col]) {
                ++deletions;
                break;
            }
        }
    }

    return deletions;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Example from problem statement
    assert(minDeletionSize({"cba", "daf", "ghi"}) == 1);
    assert(minDeletionSize({"a", "b"}) == 0);
    assert(minDeletionSize({"zyx", "wvu", "tsr"}) == 3);
    // Edge cases
    assert(minDeletionSize({}) == 0);
    assert(minDeletionSize({"abc"}) == 0);
    assert(minDeletionSize({"a"}) == 0);
    // Single column with descending order
    assert(minDeletionSize({"z", "a", "m"}) == 1);
    // Mixed sorted and unsorted columns
    assert(minDeletionSize({"abc", "ade", "afg"}) == 0);
    assert(minDeletionSize({"abc", "aac", "aaa"}) == 1); // second and third columns fail?
    // Actually check: col0: 'a','a','a' sorted; col1: 'b','a','a' fails (b>a) -> delete; col2: 'c','c','a' fails (c>a) -> delete => 2
    assert(minDeletionSize({"abc", "aac", "aaa"}) == 2);
    // Equal strings
    assert(minDeletionSize({"aaa", "aaa", "aaa"}) == 0);
    // Large test with many rows, one bad column
    std::vector<std::string> large(1000, "aaaa");
    large[500] = "aaab"; // column 3 fails at row 500 (b>a)
    assert(minDeletionSize(large) == 1);

    return 0;
}
