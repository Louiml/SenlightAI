// Given a grid of characters with `n` rows and `m` columns, and an array of `m` positive integers `po` (points per column), write a C++ function `long long totalColumnScore(const vector<string>& grid, const vector<long long>& points)` that returns the sum over all columns of `(number of most frequent character in that column) * (points for that column)`. For each column independently, count the occurrences of each character in that column, find the maximum count, multiply it by the corresponding points value, and add to the total. The grid contains only lowercase English letters. The number of rows `n` and columns `m` can be up to 1000, and points values are positive and fit in a 64-bit integer. The function should not modify the input and should use `const` references where appropriate.
The solution processes each column independently. For every column index `i` from 0 to `m-1`, we count the frequency of each character across all `n` rows of that column. Since the characters are lowercase only, we can use a fixed-size array of 26 integers for counting, initialized to zero for each column. We then iterate through all rows for that column, incrementing the count for the character at `grid[row][i]`, and track the current maximum count as we go. After processing all rows of a column, the maximum count (`maxFreq`) is multiplied by `points[i]` and added to the answer. This avoids the need for a map or clearing a large structure each time; the 26-element array is fast and has constant size. Edge cases include `n=1` (each column has exactly one character, so maxFreq is always 1) and `m=1` (just one column). The total time complexity is O(n*m) because we scan every cell exactly once, and the space complexity is O(1) auxiliary (only the frequency array of size 26). All arithmetic should use `long long` to avoid overflow since the maximum score is 1000 * 1000 * large points, well within 64-bit range.
#include <vector>
#include <string>
#include <cstdint>

// Sum over each column of (most frequent character count) * points[column]
long long totalColumnScore(const std::vector<std::string>& grid, const std::vector<long long>& points) {
    int n = grid.size();
    int m = grid[0].size();
    long long answer = 0;

    for (int col = 0; col < m; ++col) {
        int freq[26] = {0};
        int maxFreq = 0;

        for (int row = 0; row < n; ++row) {
            int idx = grid[row][col] - 'a';
            ++freq[idx];
            if (freq[idx] > maxFreq) {
                maxFreq = freq[idx];
            }
        }

        answer += static_cast<long long>(maxFreq) * points[col];
    }

    return answer;
}
#include <cassert>
#include <vector>
#include <string>

// Declare the solution function (prototype)
long long totalColumnScore(const std::vector<std::string>& grid, const std::vector<long long>& points);

int main() {
    // Test 1: Basic case with distinct columns
    std::vector<std::string> g1 = {"abc", "def", "ghi"};
    std::vector<long long> p1 = {1, 2, 3};
    assert(totalColumnScore(g1, p1) == (1*1 + 1*2 + 1*3)); // each column max count=1

    // Test 2: Repeated characters in each column
    std::vector<std::string> g2 = {"aaa", "bbb", "ccc"};
    std::vector<long long> p2 = {5, 5, 5};
    assert(totalColumnScore(g2, p2) == (3*5 + 3*5 + 3*5)); // each column max count=3

    // Test 3: Mixed frequencies and points
    std::vector<std::string> g3 = {"ab", "ab", "aa"};
    std::vector<long long> p3 = {3, 4};
    // col0: 'a' appears 3 times, 'b' 0 -> max=3 -> 3*3=9
    // col1: 'b' appears 2 times, 'a' 1 -> max=2 -> 2*4=8
    assert(totalColumnScore(g3, p3) == 17);

    // Test 4: Single row, multiple columns
    std::vector<std::string> g4 = {"hello"};
    std::vector<long long> p4 = {1, 1, 1, 1, 1};
    assert(totalColumnScore(g4, p4) == 5); // each column max=1

    // Test 5: Single column, multiple rows
    std::vector<std::string> g5 = {"a", "b", "a"};
    std::vector<long long> p5 = {10};
    assert(totalColumnScore(g5, p5) == 20); // max 'a'=2 -> 2*10

    // Test 6: Large points and all same characters
    std::vector<std::string> g6 = {"zz", "zz"};
    std::vector<long long> p6 = {1000000000LL, 1000000000LL};
    assert(totalColumnScore(g6, p6) == 2000000000LL * 2); // 2*1e9 + 2*1e9

    // Test 7: Empty grid edge (n=0, m=0) should return 0
    std::vector<std::string> g7;
    std::vector<long long> p7;
    assert(totalColumnScore(g7, p7) == 0);

    return 0;
}
