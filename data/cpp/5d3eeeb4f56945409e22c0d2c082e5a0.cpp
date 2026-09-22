Given four positive integer side lengths, write a C++ function that returns an integer result: `-1` if no triangle can be formed (impossible), `0` if a non-degenerate triangle can be formed, and `1` if only a degenerate triangle (a segment, where the largest side equals the sum of the other two) can be formed. The function must consider all four possible choices of three sides from the four given lengths (order does not matter for each triple). If any triple forms a non-degenerate triangle, return `0` immediately. Otherwise, if any triple forms a degenerate triangle (strictly satisfying the triangle inequality with equality), return `1`. If neither case occurs for any triple, return `-1`. The input values are distinct or not, and all are positive integers.

// The core algorithm sorts all four side lengths first to simplify enumeration, but the easiest robust approach is to generate all combinations of 3 indices from 4 (there are exactly 4 combinations) and for each triple compute the minimum, maximum, and the middle value. For a triple `(a, b, c)` with `a <= b <= c`, the triangle inequality reduces to checking `a + b > c`. If `a + b > c`, a non-degenerate triangle exists, so return `0`. If `a + b == c`, a degenerate triangle (segment) exists, so mark the result as `1` but continue checking other triples in case a strictly valid one appears. After all four triples are checked, return `1` if any equality was found, otherwise return `-1`. The time complexity is constant: sorting takes `O(4 log 4)` and checking 4 triples is constant, so overall `O(1)` time and `O(1)` space.

#include <algorithm>
#include <vector>

// Return -1 if no triangle (non-degenerate or degenerate) can be formed,
// 0 if at least one non-degenerate triangle exists,
// 1 if only a degenerate triangle (segment) is possible.
int classifyTriangles(const std::vector<int>& sides) {
    // Ensure we have exactly 4 sides.
    std::vector<int> s = sides;
    std::sort(s.begin(), s.end());

    int best = -1; // -1 impossible, 1 segment, 0 triangle
    // Generate all combinations of 3 out of 4 indices.
    for (int i = 0; i < 4; ++i) {
        int a = -1, b = -1, c = -1;
        // Pick the three sides excluding index i.
        int count = 0;
        for (int j = 0; j < 4; ++j) {
            if (j == i) continue;
            if (count == 0) a = s[j];
            else if (count == 1) b = s[j];
            else c = s[j];
            ++count;
        }
        // a, b, c are not sorted, but we can sort them locally.
        int arr[3] = {a, b, c};
        std::sort(arr, arr + 3);
        if (arr[0] + arr[1] > arr[2]) {
            return 0; // strict triangle found
        }
        if (arr[0] + arr[1] == arr[2]) {
            best = 1; // segment possible
        }
    }
    return best;
}

#include <cassert>
#include <vector>

// The function is declared above; here we test it.
int main() {
    // 1 1 2 2: triples (1,1,2) -> degenerate segment, (1,2,2) -> triangle, so 0
    assert(classifyTriangles({1,1,2,2}) == 0);
    
    // 1 2 3 4: (1,2,3) degenerate, (1,2,4) impossible, (1,3,4) triangle, so 0
    assert(classifyTriangles({1,2,3,4}) == 0);
    
    // 1 1 1 10: only (1,1,1) is triangle, so 0
    assert(classifyTriangles({1,1,1,10}) == 0);
    
    // 1 2 3 5: (1,2,3) degenerate, others impossible, so 1
    assert(classifyTriangles({1,2,3,5}) == 1);
    
    // 1 2 4 8: none satisfy a+b >= c, so -1
    assert(classifyTriangles({1,2,4,8}) == -1);
    
    // 2 2 2 2: any triple is an equilateral triangle, so 0
    assert(classifyTriangles({2,2,2,2}) == 0);
    
    // 1 1 2 3: (1,1,2) degenerate, (1,2,3) degenerate, so 1
    assert(classifyTriangles({1,1,2,3}) == 1);
    
    // 3 4 5 10: (3,4,5) triangle, so 0
    assert(classifyTriangles({3,4,5,10}) == 0);
    
    // 1 10 10 10: (10,10,10) triangle, so 0
    assert(classifyTriangles({1,10,10,10}) == 0);
    
    // 1 2 3 6: only (1,2,3) degenerate, so 1
    assert(classifyTriangles({1,2,3,6}) == 1);
    
    return 0;
}
