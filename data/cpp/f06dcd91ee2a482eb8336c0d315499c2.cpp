/*
Write a C++ function `collectSpanningCells` that takes a vector of triples, where each triple represents a cell with a column span (`colSpan`), row index (`row`), and column index (`col`). The function must group these cells by their column span and return them in ascending order of column span. For each unique column span, all cells with that span should be listed in their original relative order (i.e., the order they appeared in the input vector, but grouped by span). The function should handle spans from 2 to 1000000, but only cells with span values in the inclusive range [2, 1000000] should be considered; any cell with a span outside this range is ignored. The output should be a `vector<pair<int, vector<pair<int, int>>>>` where each pair contains a column span and a vector of (row, col) pairs for that span, ordered by increasing span. The input vector may contain duplicate spans and arbitrary row/column values. If no valid cells exist, return an empty vector.
*/
#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>

// Groups cells by column span (span from 2 to 1000000 inclusive)
// and returns them sorted by ascending span. For each span, cells
// are in their original relative order from the input.
// Input: vector of (colSpan, row, col). Output: vector of (span, vector of (row, col)).
std::vector<std::pair<int, std::vector<std::pair<int, int>>>> collectSpanningCells(
    const std::vector<std::tuple<int, int, int>>& input) {
    
    // Map from span to vector of (row, col) preserving insertion order.
    std::unordered_map<int, std::vector<std::pair<int, int>>> groups;
    
    for (const auto& tuple : input) {
        int colSpan = std::get<0>(tuple);
        int row = std::get<1>(tuple);
        int col = std::get<2>(tuple);
        
        // Only consider spans in [2, 1000000].
        if (colSpan < 2 || colSpan > 1000000) {
            continue;
        }
        
        groups[colSpan].emplace_back(row, col);
    }
    
    // Sort the distinct spans ascending.
    std::vector<int> spans;
    spans.reserve(groups.size());
    for (const auto& entry : groups) {
        spans.push_back(entry.first);
    }
    std::sort(spans.begin(), spans.end());
    
    // Build the result in sorted order.
    std::vector<std::pair<int, std::vector<std::pair<int, int>>>> result;
    result.reserve(spans.size());
    for (int span : spans) {
        result.emplace_back(span, std::move(groups[span]));
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <tuple>
#include <utility>

// Assume the solution function is provided above.

int main() {
    // Test 1: Basic grouping with unsorted spans.
    std::vector<std::tuple<int, int, int>> input1 = {
        {5, 1, 1}, {2, 0, 0}, {5, 2, 3}, {3, 0, 2}, {2, 1, 4}
    };
    auto result1 = collectSpanningCells(input1);
    assert(result1.size() == 3);
    assert(result1[0].first == 2);
    assert(result1[0].second == std::vector<std::pair<int,int>>({{0,0},{1,4}}));
    assert(result1[1].first == 3);
    assert(result1[1].second == std::vector<std::pair<int,int>>({{0,2}}));
    assert(result1[2].first == 5);
    assert(result1[2].second == std::vector<std::pair<int,int>>({{1,1},{2,3}}));

    // Test 2: Ignore out-of-range spans.
    std::vector<std::tuple<int, int, int>> input2 = {
        {1, 0, 0}, {1000001, 0, 1}, {2, 1, 1}, {1000000, 2, 2}
    };
    auto result2 = collectSpanningCells(input2);
    assert(result2.size() == 2);
    assert(result2[0].first == 2);
    assert(result2[0].second == std::vector<std::pair<int,int>>({{1,1}}));
    assert(result2[1].first == 1000000);
    assert(result2[1].second == std::vector<std::pair<int,int>>({{2,2}}));

    // Test 3: Empty input.
    std::vector<std::tuple<int, int, int>> input3;
    auto result3 = collectSpanningCells(input3);
    assert(result3.empty());

    // Test 4: Duplicate spans and many entries.
    std::vector<std::tuple<int, int, int>> input4 = {
        {4, 0, 0}, {4, 0, 1}, {4, 1, 2}, {2, 9, 9}, {4, 2, 3}
    };
    auto result4 = collectSpanningCells(input4);
    assert(result4.size() == 2);
    assert(result4[0].first == 2);
    assert(result4[0].second == std::vector<std::pair<int,int>>({{9,9}}));
    assert(result4[1].first == 4);
    assert(result4[1].second == std::vector<std::pair<int,int>>({{0,0},{0,1},{1,2},{2,3}}));

    // Test 5: All cells ignored.
    std::vector<std::tuple<int, int, int>> input5 = {
        {0, 0, 0}, { -5, 1, 2}, {2000000, 3, 4}
    };
    auto result5 = collectSpanningCells(input5);
    assert(result5.empty());

    // Test 6: Single valid cell.
    std::vector<std::tuple<int, int, int>> input6 = {
        {1000000, 7, 8}
    };
    auto result6 = collectSpanningCells(input6);
    assert(result6.size() == 1);
    assert(result6[0].first == 1000000);
    assert(result6[0].second == std::vector<std::pair<int,int>>({{7,8}}));

    return 0;
}
// The solution groups cells by their column span while preserving the relative order of cells within each span. Since the output requires spans sorted ascending, and the span range is large (2 to 1,000,000), we cannot use a fixed-size array for all spans. Instead, use a hash map (e.g., `std::unordered_map<int, std::vector<std::pair<int,int>>>`) where the key is the span and the value is a vector of (row,col) pairs. Iterate over the input vector once, skipping any cell whose span is outside [2, 1000000]. For each valid cell, append to the vector corresponding to its span. After processing all input, collect the keys from the map, sort them ascending, and build the result by iterating the sorted keys and moving the corresponding vectors into the output. This preserves the insertion order within each span because we append in the order cells appear. Time complexity is O(n + k log k) where n is the number of input triples and k is the number of distinct valid spans. Space complexity is O(n) for storing all cells, plus O(k) for the map keys and output structure. Edge cases: span values exactly 2 and exactly 1000000 are included; span values below 2 or above 1000000 are ignored; empty input yields empty result; duplicate spans are handled naturally by grouping.
