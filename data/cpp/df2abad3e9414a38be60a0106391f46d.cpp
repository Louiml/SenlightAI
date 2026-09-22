/*
Write a C++ function that takes three integers: a square grid dimension `sz` (odd or even, but at least 1), a number of queries `cnt`, and then reads `cnt` pairs of 1-based coordinates `(x, y)` within the grid. For each query, the function must output one of the numbers 1, 2, or 3 based on the following rule: First, map the coordinate to its "mirror" position by reflecting across both the horizontal and vertical center lines (i.e., if `x > sz/2` then replace `x` with `sz - x + 1` when using 1-based indices, which is equivalent to the 0-based `sz - x - 1` after decrementing). Do the same for `y`. Then, order the two transformed coordinates so that the larger value is in the first variable and the smaller in the second (swap if needed). Finally, classify the smaller transformed coordinate `minVal` as follows: if `minVal % 3 == 0` return 1, if `minVal % 3 == 1` return 2, otherwise return 3. For example, for `sz = 5` and query `(1,5)`, after reflection (0-based: (0,4) -> both >= 2, so reflect to (0,0)), we get `minVal = 0`, output 1. The function should not read from standard input; instead, it should accept a 2D vector or a list of queries as parameters (e.g., a `std::vector<std::pair<int,int>>`), and return a `std::vector<int>` containing the result for each query in order.
*/
#include <vector>
#include <utility>
#include <algorithm>

// Given a square grid of dimension sz and a list of 1-based coordinate pairs,
// returns for each query the classification number (1, 2, or 3) based on the
// described reflection and modulo rule.
std::vector<int> classifyQueries(int sz, const std::vector<std::pair<int,int>>& queries) {
    std::vector<int> results;
    results.reserve(queries.size());
    
    for (const auto& query : queries) {
        int x = query.first - 1;  // convert to 0-based
        int y = query.second - 1;
        
        // Reflect across horizontal and vertical centers
        if (x >= sz / 2)
            x = sz - x - 1;
        if (y >= sz / 2)
            y = sz - y - 1;
        
        // Order so that first is larger, second is smaller
        if (x < y)
            std::swap(x, y);
        int minVal = y;  // smaller value
        
        if (minVal % 3 == 0)
            results.push_back(1);
        else if (minVal % 3 == 1)
            results.push_back(2);
        else
            results.push_back(3);
    }
    return results;
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function declaration here (or copy the code above)

int main() {
    // sz = 1, only coordinate (1,1) -> after reflection both 0, minVal=0 -> 1
    assert(classifyQueries(1, {{1,1}}) == std::vector<int>{1});
    
    // sz = 2, coordinates:
    // (1,1) -> (0,0) minVal=0 ->1
    // (1,2) -> (0,0) minVal=0 ->1 (since y=1>=1 -> reflect to 0)
    // (2,1) -> (0,0) same
    // (2,2) -> (0,0) same
    assert(classifyQueries(2, {{1,1},{1,2},{2,1},{2,2}}) == std::vector<int>({1,1,1,1}));
    
    // sz = 3, center (2,2) -> 0-based (1,1) -> minVal=1 -> 2
    assert(classifyQueries(3, {{2,2}}) == std::vector<int>{2});
    
    // sz = 3, corner (1,1) -> (0,0) minVal=0 ->1
    assert(classifyQueries(3, {{1,1}}) == std::vector<int>{1});
    
    // sz = 4, coordinate (1,4) -> 0-based (0,3) -> both >=2, reflect: x=0, y=0 -> minVal=0 ->1
    assert(classifyQueries(4, {{1,4}}) == std::vector<int>{1});
    
    // sz = 4, coordinate (3,2) -> 0-based (2,1) -> x>=2 reflect x=1, y=1 -> minVal=1 ->2
    assert(classifyQueries(4, {{3,2}}) == std::vector<int>{2});
    
    // sz = 5, coordinate (5,1) -> 0-based (4,0) -> x>=2 reflect x=0, y=0 -> minVal=0 ->1
    assert(classifyQueries(5, {{5,1}}) == std::vector<int>{1});
    
    // sz = 5, coordinate (3,3) -> 0-based (2,2) -> both < 2? no, 2 >= 2, reflect each to 2? 
    // Actually sz/2=2, so x=2 >=2 -> x=5-2-1=2, y same -> minVal=2 -> 2%3==2 -> 3
    assert(classifyQueries(5, {{3,3}}) == std::vector<int>{3});
    
    // sz = 7, coordinate (7,7) -> 0-based (6,6) -> both >=3, reflect to (0,0) -> minVal=0 ->1
    assert(classifyQueries(7, {{7,7}}) == std::vector<int>{1});
    
    // Multiple queries check
    auto results = classifyQueries(6, {{1,1},{2,2},{3,3},{6,6},{5,4}});
    // Compute expected manually:
    // (1,1)->(0,0) min=0 ->1
    // (2,2)->(1,1) min=1 ->2
    // (3,3)->(2,2) min=2 ->3
    // (6,6)->(5,5) reflect x=0,y=0 ->1
    // (5,4)->(4,3) x>=3 reflect x=1, y=3>=3 reflect y=2, sort: x=2,y=1 -> min=1 ->2
    assert(results == std::vector<int>({1,2,3,1,2}));
    
    return 0;
}
// The core idea is to reduce each query to a canonical "inner quadrant" coordinate by reflecting across both axes. Since reflections are involutive, after mapping `x` and `y` to their mirrored positions (0-based: if `coord >= sz/2`, set `coord = sz - coord - 1`), both coordinates become in `[0, sz/2]` (for odd `sz`, the middle row/column stays unchanged because `(sz/2)` is exclusive, so the middle element is less than `sz/2`). The order of the two transformed coordinates doesn't matter for the final output because we always take the smaller one, and the classification depends only on that smaller value modulo 3. Edge cases: `sz = 1` (only coordinate (0,0) after decrement, both map to 0, output 1). When `sz` is odd, the center coordinate `( (sz-1)/2, (sz-1)/2 )` remains unchanged and its smaller value is `(sz-1)/2`, which is classified accordingly. The algorithm processes each query in constant time, so for `cnt` queries, time complexity is `O(cnt)`, and space complexity `O(1)` auxiliary if we return a vector of results (size `cnt`). No integer overflow issues as long as `sz` fits in `int` (typically up to 2^31-1).
