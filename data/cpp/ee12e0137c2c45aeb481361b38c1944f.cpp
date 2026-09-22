// You are given a set of \( n \) axis-aligned rectangles on a 2D plane, each described by its lower-left corner \((x1_i, y1_i)\) and upper-right corner \((x2_i, y2_i)\), where \(x1_i < x2_i\) and \(y1_i < y2_i\). Write a C++ function `std::pair<int,int> findCommonIntersectionPoint(const std::vector<Rectangle>& rects)` that, given a non-empty vector of such rectangles, returns a single integer point \((x, y)\) that lies inside the intersection of all rectangles **except at most one** of them. In other words, you must remove exactly one rectangle so that the remaining \(n-1\) rectangles have a non-empty common intersection, and then return the lower-left corner of that intersection (the point with the minimum \(x\) and minimum \(y\) among the intersection region). It is guaranteed that such a point always exists. The input rectangles are distinct (no two have exactly the same four coordinates). The function should return the point as a pair of integers; if multiple valid points exist, return the one with the smallest x, and if tie, smallest y (which is already the lower-left corner).

#include <cassert>
#include <vector>
#include <utility>
// Assume the solution's struct and function are defined above.

int main() {
    // Test 1: Simple two rectangles, remove one.
    std::vector<Rectangle> rects1 = { {0,0,10,10}, {5,5,15,15} };
    assert(findCommonIntersectionPoint(rects1) == std::make_pair(5,5));
    
    // Test 2: Three rectangles, all intersect at a point, removing any one works.
    std::vector<Rectangle> rects2 = { {0,0,3,3}, {1,1,4,4}, {2,2,5,5} };
    assert(findCommonIntersectionPoint(rects2) == std::make_pair(2,2));
    
    // Test 3: One rectangle doesn't overlap with the others, must remove it.
    std::vector<Rectangle> rects3 = { {0,0,1,1}, {0,0,1,1}, {10,10,11,11} };
    // After removing the third, intersection of first two is {0,0} to {1,1}, lower-left (0,0).
    assert(findCommonIntersectionPoint(rects3) == std::make_pair(0,0));
    
    // Test 4: Single rectangle.
    std::vector<Rectangle> rects4 = { {2,3,8,9} };
    assert(findCommonIntersectionPoint(rects4) == std::make_pair(2,3));
    
    // Test 5: All share same lower-left, but one has shifted upper-right far away.
    std::vector<Rectangle> rects5 = { {1,1,2,2}, {1,1,5,5}, {1,1,3,3} };
    assert(findCommonIntersectionPoint(rects5) == std::make_pair(1,1));
    
    // Test 6: Removing first rectangle yields non-empty, but removing others fails.
    std::vector<Rectangle> rects6 = { {0,0,1,1}, {2,2,3,3}, {2,2,3,3} };
    // Remove the first, remaining two intersect at {2,2} to {3,3}.
    assert(findCommonIntersectionPoint(rects6) == std::make_pair(2,2));
    
    // Test 7: Larger coordinates and many rectangles.
    std::vector<Rectangle> rects7;
    for (int i = 0; i < 100; ++i) rects7.push_back({i, -i, i+100, -i+100});
    // All intersect at some region; removing any one still yields non-empty.
    auto p7 = findCommonIntersectionPoint(rects7);
    assert(p7.first >= 99 && p7.second >= -99 && p7.first <= 100 && p7.second <= 100);
    
    // Test 8: Degenerate where exactly one removal makes intersection a single point.
    std::vector<Rectangle> rects8 = { {0,0,2,2}, {1,1,3,3}, {2,0,4,1} };
    // Remove the third rectangle, intersection of first two is [1,1]-[2,2] -> point (1,1).
    assert(findCommonIntersectionPoint(rects8) == std::make_pair(1,1));
    
    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

struct Rectangle {
    int x1, y1, x2, y2;
    
    Rectangle(int x1_, int y1_, int x2_, int y2_) 
        : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
};

// Returns a point (x, y) that lies in the common intersection of all rectangles
// except one. The point is the lower-left corner of that intersection.
// It is guaranteed such a point exists.
std::pair<int, int> findCommonIntersectionPoint(const std::vector<Rectangle>& rects) {
    int n = static_cast<int>(rects.size());
    
    // Special case: single rectangle -> its lower-left corner is trivially valid.
    if (n == 1) {
        return {rects[0].x1, rects[0].y1};
    }
    
    std::vector<int> prefMaxX1(n), prefMaxY1(n), prefMinX2(n), prefMinY2(n);
    std::vector<int> suffMaxX1(n), suffMaxY1(n), suffMinX2(n), suffMinY2(n);
    
    // Prefix arrays.
    prefMaxX1[0] = rects[0].x1;
    prefMaxY1[0] = rects[0].y1;
    prefMinX2[0] = rects[0].x2;
    prefMinY2[0] = rects[0].y2;
    for (int i = 1; i < n; ++i) {
        prefMaxX1[i] = std::max(prefMaxX1[i-1], rects[i].x1);
        prefMaxY1[i] = std::max(prefMaxY1[i-1], rects[i].y1);
        prefMinX2[i] = std::min(prefMinX2[i-1], rects[i].x2);
        prefMinY2[i] = std::min(prefMinY2[i-1], rects[i].y2);
    }
    
    // Suffix arrays.
    suffMaxX1[n-1] = rects[n-1].x1;
    suffMaxY1[n-1] = rects[n-1].y1;
    suffMinX2[n-1] = rects[n-1].x2;
    suffMinY2[n-1] = rects[n-1].y2;
    for (int i = n-2; i >= 0; --i) {
        suffMaxX1[i] = std::max(suffMaxX1[i+1], rects[i].x1);
        suffMaxY1[i] = std::max(suffMaxY1[i+1], rects[i].y1);
        suffMinX2[i] = std::min(suffMinX2[i+1], rects[i].x2);
        suffMinY2[i] = std::min(suffMinY2[i+1], rects[i].y2);
    }
    
    // Try removing each rectangle.
    for (int i = 0; i < n; ++i) {
        int maxX1 = INT_MIN, maxY1 = INT_MIN, minX2 = INT_MAX, minY2 = INT_MAX;
        
        // Combine prefix (0..i-1) and suffix (i+1..n-1).
        if (i > 0) {
            maxX1 = std::max(maxX1, prefMaxX1[i-1]);
            maxY1 = std::max(maxY1, prefMaxY1[i-1]);
            minX2 = std::min(minX2, prefMinX2[i-1]);
            minY2 = std::min(minY2, prefMinY2[i-1]);
        }
        if (i < n-1) {
            maxX1 = std::max(maxX1, suffMaxX1[i+1]);
            maxY1 = std::max(maxY1, suffMaxY1[i+1]);
            minX2 = std::min(minX2, suffMinX2[i+1]);
            minY2 = std::min(minY2, suffMinY2[i+1]);
        }
        
        // Check if intersection is non-empty.
        if (maxX1 <= minX2 && maxY1 <= minY2) {
            return {maxX1, maxY1};
        }
    }
    
    // Should never reach here due to problem guarantee.
    assert(false);
    return {0, 0};
}

// The key observation is that the intersection of a set of axis-aligned rectangles is itself an axis-aligned rectangle (possibly degenerate), whose lower-left corner is \((\max(x1_i), \max(y1_i))\) and upper-right corner is \((\min(x2_i), \min(y2_i))\). The intersection is non-empty if and only if \(\max(x1_i) \le \min(x2_i)\) and \(\max(y1_i) \le \min(y2_i)\). Our goal is to find an index \(i\) such that removing rectangle \(i\) leaves a valid intersection. We can do this by scanning each rectangle as a candidate to remove. For each candidate, we compute the max of all \(x1\) and \(y1\) and the min of all \(x2\) and \(y2\) excluding that rectangle. This can be done efficiently in \(O(n)\) per candidate if we naively recompute, resulting in \(O(n^2)\) total time, which is acceptable for moderate \(n\) but could be improved to \(O(n)\) using prefix/suffix arrays. Since the constraints are not given, we provide an \(O(n^2)\) solution for clarity; a better solution uses four arrays: prefix max for \(x1\)/\(y1\) and prefix min for \(x2\)/\(y2\), and similarly suffix arrays, then for each \(i\) combine prefix up to \(i-1\) and suffix from \(i+1\) to get the extremes excluding \(i\). This yields \(O(n)\) time and \(O(n)\) extra space. The point to return is the lower-left corner \((\max\_x1, \max\_y1)\) of the valid intersection. Edge cases include when \(n=1\), the single remaining rectangle is the intersection of zero rectangles? Actually if \(n=1\), we must remove one rectangle leaving zero rectangles, which trivially has an empty intersection? But the problem statement guarantees a point exists, so for \(n=1\) we can simply return its lower-left corner (since we can skip removing the only one and consider the empty intersection as everything? Actually the problem says remove exactly one rectangle so that the remaining \(n-1\) have non-empty intersection. For \(n=1\), \(n-1=0\) and an empty intersection is usually considered unbounded? But to be consistent, we treat the case where \(n=1\) as we remove the only rectangle and the remaining "intersection" is entire plane, but we return the lower-left of the original rectangle? The guarantee ensures it works. In our implementation, we handle the case where after removal of one rectangle the remaining set is empty by returning that rectangle's lower-left corner. But the sample code only works for \(n\ge 2\)? Actually the original snippet uses multisets and checks for each removal; for \(n=1\) after removal all multisets are empty and `*rbegin` on empty multiset is undefined. So we assume \(n\ge 2\) or handle specially. We'll handle \(n=1\) explicitly. Time complexity: \(O(n)\) with prefix/suffix method, space \(O(n)\). For an \(O(n^2)\) solution, space \(O(1)\) and time \(O(n^2)\). We'll present the \(O(n)\) solution in the reference.
