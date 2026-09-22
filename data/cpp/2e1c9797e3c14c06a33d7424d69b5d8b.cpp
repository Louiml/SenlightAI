Write a C++ function that simulates a simplified version of the given code's core logic. You are given three arrays: `heights` (a vector of distinct integers representing heights), `tastes` (a vector of integers with the same size, representing taste values), and `indices` (a vector of integers representing the original 1-based positions, parallel to `heights`). The function `processHeightsAndTastes` takes these three parallel vectors along with a list of queries. Each query is a tuple of three integers `(type, x, y)`. If `type == 1`, update `tastes[x-1]` to `y` (since `x` is a 1-based index into the original arrays). If `type == 2`, treat `x` and `y` as height values (not indices); find the first position in the combined sorted-by-height list where the height equals `x` and the first position where height equals `y` (using the sorted order of heights, but note the function must internally sort a copy of the height-index pairs by height, then find these positions). For each such query of type 2, output (or accumulate) the pair `(height, original_index)` for both found positions, in that order. If either height is not found, use `(-1, -1)` for that result. The function should return a vector of strings, each string representing the results for a type-2 query in the format `"h1 i1 h2 i2"` with spaces, where `h1 i1` are the height and original index of the first found pair, and `h2 i2` are for the second. The queries are processed in order, and updates to `tastes` persist across queries. Heights are distinct, but the function must handle cases where a queried height is not present.

The core task is to maintain a mapping from height to original index, but the sorting must happen once on an initial copy of the height-index pairs, not on the mutable array. First, create a vector of pairs `(height, index)` from the parallel input vectors, and sort it by height (default pair sorting sorts by first element, which is height). Since heights are distinct, we can use `std::lower_bound` on the sorted vector to find the position for a given height in O(log n) time. However, the given snippet uses `find_if` which is O(n) per query, but we can improve to O(log n) by using `lower_bound` on the sorted vector for type-2 queries. The `tastes` array is updated in place for type-1 queries; no sorting of tastes is needed. Edge cases: if a height is not found, `lower_bound` will return an iterator to a value greater than the target or the end, so we must check if the iterator is valid and if the height matches; otherwise return `(-1, -1)`. Also, the indices passed in queries for type-1 are 1-based, so subtract 1. For type-2, the values are heights, not indices. The function processes each query sequentially, accumulating results for type-2 queries. Time complexity: building the sorted copy takes O(n log n), each type-1 update is O(1), each type-2 query uses two binary searches O(log n). Overall for q queries O(n log n + q log n). Space complexity O(n) for the sorted copy and the output.

#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <sstream>

// Processes queries on parallel arrays. Returns strings for each type-2 query.
// Parameters:
//   heights : vector of distinct integers (original order aligned with tastes and indices)
//   tastes  : vector of integers (same size as heights)
//   indices : vector of 1-based original positions (same size as heights)
//   queries : vector of tuples (type, x, y)
// Returns: vector of strings in order of type-2 queries, format "h1 i1 h2 i2"
std::vector<std::string> processHeightsAndTastes(
    const std::vector<int>& heights,
    std::vector<int> tastes,
    const std::vector<int>& indices,
    const std::vector<std::tuple<int, int, int>>& queries) {

    // Build sorted copy of (height, original_index) pairs
    std::vector<std::pair<int, int>> sortedPairs;
    sortedPairs.reserve(heights.size());
    for (std::size_t i = 0; i < heights.size(); ++i) {
        sortedPairs.push_back({heights[i], indices[i]});
    }
    std::sort(sortedPairs.begin(), sortedPairs.end());

    std::vector<std::string> results;

    // Helper lambda to find pair by height, returns (-1,-1) if not found
    auto findPairByHeight = [&](int height) -> std::pair<int, int> {
        auto it = std::lower_bound(sortedPairs.begin(), sortedPairs.end(), height,
            [](const std::pair<int, int>& p, int value) { return p.first < value; });
        if (it != sortedPairs.end() && it->first == height) {
            return *it;
        }
        return {-1, -1};
    };

    for (const auto& q : queries) {
        int type = std::get<0>(q);
        int x = std::get<1>(q);
        int y = std::get<2>(q);

        if (type == 1) {
            // Update taste at 1-based index x
            if (x >= 1 && x <= static_cast<int>(tastes.size())) {
                tastes[x - 1] = y;
            }
        } else if (type == 2) {
            // Find heights x and y in sortedPairs
            auto p1 = findPairByHeight(x);
            auto p2 = findPairByHeight(y);
            std::ostringstream oss;
            oss << p1.first << " " << p1.second << " " << p2.first << " " << p2.second;
            results.push_back(oss.str());
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <tuple>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Test 1: Basic case with two heights and queries
    {
        std::vector<int> heights = {10, 20, 30};
        std::vector<int> tastes = {1, 2, 3};
        std::vector<int> indices = {1, 2, 3};
        std::vector<std::tuple<int, int, int>> queries = {
            {2, 10, 30},
            {2, 20, 25}
        };
        auto result = processHeightsAndTastes(heights, tastes, indices, queries);
        assert(result.size() == 2);
        assert(result[0] == "10 1 30 3");
        assert(result[1] == "20 2 -1 -1");
    }

    // Test 2: Type-1 updates do not affect height sorting, but taste updates persist
    {
        std::vector<int> heights = {5, 15, 25};
        std::vector<int> tastes = {100, 200, 300};
        std::vector<int> indices = {1, 2, 3};
        std::vector<std::tuple<int, int, int>> queries = {
            {1, 2, 999},   // update taste of index 2 (height 15) to 999
            {2, 5, 25},    // heights 5 and 25 exist
            {2, 15, 5}     // heights 15 and 5 exist
        };
        auto result = processHeightsAndTastes(heights, tastes, indices, queries);
        assert(result.size() == 2);
        assert(result[0] == "5 1 25 3");
        assert(result[1] == "15 2 5 1");
    }

    // Test 3: Distinct heights but unsorted input order
    {
        std::vector<int> heights = {42, 7, 13};
        std::vector<int> tastes = {0, 0, 0};
        std::vector<int> indices = {1, 2, 3};
        std::vector<std::tuple<int, int, int>> queries = {
            {2, 13, 42},
            {2, 1, 7}   // 1 not present
        };
        auto result = processHeightsAndTastes(heights, tastes, indices, queries);
        assert(result.size() == 2);
        assert(result[0] == "13 3 42 1");
        assert(result[1] == "-1 -1 7 2");
    }

    // Test 4: Single element, both queries same height
    {
        std::vector<int> heights = {100};
        std::vector<int> tastes = {5};
        std::vector<int> indices = {1};
        std::vector<std::tuple<int, int, int>> queries = {
            {2, 100, 100},
            {2, 200, 100}
        };
        auto result = processHeightsAndTastes(heights, tastes, indices, queries);
        assert(result.size() == 2);
        assert(result[0] == "100 1 100 1");
        assert(result[1] == "-1 -1 100 1");
    }

    // Test 5: More complex sequence with updates and missing heights
    {
        std::vector<int> heights = {3, 1, 2};
        std::vector<int> tastes = {10, 20, 30};
        std::vector<int> indices = {1, 2, 3};
        std::vector<std::tuple<int, int, int>> queries = {
            {1, 3, 99},   // update taste at index 3 (height 2) to 99
            {2, 1, 2},
            {1, 1, 77},   // update taste at index 1 (height 3)
            {2, 3, 1},
            {2, 4, 1}    // 4 missing
        };
        auto result = processHeightsAndTastes(heights, tastes, indices, queries);
        assert(result.size() == 3);
        assert(result[0] == "1 2 2 3");
        assert(result[1] == "3 1 1 2");
        assert(result[2] == "-1 -1 1 2");
    }

    return 0;
}
