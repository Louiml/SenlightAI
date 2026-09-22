// Given a set of \(n\) points in a 2D plane (where \(1 \le n \le 300000\)) and \(q\) axis-aligned rectangle queries (where \(1 \le q \le 300000\)), each specified by its lower-left corner \((x_1, y_1)\) and upper-right corner \((x_2, y_2)\), write a C++ function `countPointsInRectangles` that takes the list of points and the list of queries (each represented as a `Rectangle` struct with four integers) and returns a vector of integers, where the \(i\)-th result is the number of points that lie strictly inside or on the boundary of the \(i\)-th rectangle. Coordinates are integers in the range \([-10^9, 10^9]\). You must process all queries efficiently, avoiding the \(O(nq)\) brute force. The function should return a `std::vector<int>` containing the count for each query in order. Assume all points are unique. You may use standard library sorting and binary search. For this task, you need to implement the function yourself without relying on external geometry libraries.

The key is to pre-sort the points twice: once by x-coordinate (with y as tiebreaker) and once by y-coordinate (with x as tiebreaker). For each query, we must count points satisfying \(x_1 \le x \le x_2\) and \(y_1 \le y \le y_2\). A naive check of every point is too slow. However, we can decompose the count using inclusion–exclusion with prefix counts: count points with \(x \le X\) and \(y \le Y\) for four corners \((x_2, y_2)\), \((x_1-1, y_2)\), \((x_2, y_1-1)\), \((x_1-1, y_1-1)\), then combine as \(F(x_2,y_2) - F(x_1-1,y_2) - F(x_2,y_1-1) + F(x_1-1,y_1-1)\), where \(F(X,Y)\) is the number of points with \(x \le X\) and \(y \le Y\). To compute \(F(X,Y)\) efficiently for arbitrary \(X,Y\) per query (without precomputing a 2D grid), we sort points by x, and for a given \(X\) we find the prefix of points with \(x \le X\) using binary search; among that prefix, we need to count how many have \(y \le Y\). But that would be slow if done naively for each query. Instead, we can use a subdivision approach: maintain the points sorted by x, and for each point, we can use a Fenwick tree or a merge-sort tree. However, a simpler and standard approach for static 2D orthogonal range counting with many queries is the "sweep-line with a Fenwick tree" or "offline processing". But the snippet provided uses a different technique: it processes each query by four range counts using two sorted arrays: `py` sorted by y, and `px` sorted by x. For each y-boundary (y1 and y2), it finds the index range in `py` where y equals that boundary, then within that range it counts how many x values lie between x1 and x2 by binary-searching in that subrange. Similarly, for each x-boundary (x1 and x2), it finds the range in `px` where x equals that boundary, and within that range counts y values strictly between y1 and y2 (exclusive) to avoid double-counting corners. The total for a rectangle is the sum of counts on the bottom edge, top edge, left edge (excluding corners), and right edge (excluding corners). This works because all points are unique and we count boundary points exactly once. The algorithm per query does a constant number of binary searches (at most 8), each O(log n), so O(log n) per query. Preprocessing sorts two arrays in O(n log n). Total time: O(n log n + q log n). Space: O(n). Edge cases: when x1 > x2 or y1 > y2? The problem statement likely guarantees x1 ≤ x2 and y1 ≤ y2, but we can handle that by swapping or returning 0. Also, when there are no points on a boundary, the binary search returns empty ranges. When y1 == y2, the bottom and top edges are the same; the algorithm counts the bottom edge fully and the top edge fully, which would double-count points on that line. To avoid that, we can either handle the case of y1 == y2 and x1 == x2 specially (a single point) or adjust: when counting bottom edge, include points with y1 and x in [x1,x2]; counting top edge, if y2 != y1, include points with y2 and x in [x1,x2]; if y1==y2, we only need one of them. Similarly for vertical edges. The snippet avoids double counting by using exclusive bounds on the left/right edges (y1+1 to y2-1), so if y1==y2, vertical edges count nothing, and horizontal edges count bottom and top separately, which would double count the same line. Actually, the snippet adds both bottom and top edges unconditionally, which would double count when y1==y2. But the problem might guarantee y1 < y2 and x1 < x2? The snippet does not validate. We'll design our reference to handle any rectangle, including degenerate ones, correctly. We'll implement a function that uses inclusion-exclusion with an offline BIT (Fenwick tree) approach: sort points by x, sort queries by X (for the four prefix queries), and process with a BIT over y-coordinates. This is cleaner, handles degenerate rectangles, and is standard for static orthogonal counting. We'll compress y-coordinates of all points and all query y-boundaries (y1-1, y2, etc.) to fit in a BIT. For each query, we compute four prefix counts: F(x2,y2), F(x1-1,y2), F(x2,y1-1), F(x1-1,y1-1). We'll create four "prefix request" events per query, each with (X, Y, queryIndex, sign). Sort points by x, sort events by X, then sweep X from low to high: add points with x <= current X to BIT at their compressed y; at each event, answer the prefix sum of y <= Y. Then combine results by sign. Complexity: O((n + 4q) log(n + 4q)) for sorting, O((n+4q) log m) for BIT operations where m is number of unique y coordinates. This is standard and robust.

#include <vector>
#include <algorithm>
#include <cstdint>

struct Rectangle {
    int x1, y1, x2, y2; // lower-left and upper-right corners inclusive
};

struct Point {
    int x, y;
};

// Counts points inside each rectangle (inclusive). Points are unique.
std::vector<int> countPointsInRectangles(const std::vector<Point>& points,
                                         const std::vector<Rectangle>& queries) {
    int n = points.size();
    int q = queries.size();

    // Coordinate compression for y values (points and all query y-boundaries)
    std::vector<int> ys;
    ys.reserve(n + 4*q);
    for (const auto& p : points) ys.push_back(p.y);
    for (const auto& r : queries) {
        ys.push_back(r.y1 - 1);
        ys.push_back(r.y2);
    }
    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    int m = ys.size();

    // Fenwick tree (BIT) for prefix sums over compressed y
    std::vector<int> bit(m + 1, 0);
    auto add = [&](int idx, int val) {
        for (int i = idx; i <= m; i += i & -i) bit[i] += val;
    };
    auto sum = [&](int idx) {
        int res = 0;
        for (int i = idx; i > 0; i -= i & -i) res += bit[i];
        return res;
    };

    // Sort points by x
    std::vector<Point> pts = points;
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    // Events: each is (X, Y_compressed, query_index, sign)
    // F(X,Y) = number of points with x <= X and y <= Y
    struct Event {
        int x;
        int yidx; // compressed y
        int qid;
        int sign;
    };
    std::vector<Event> events;
    events.reserve(4*q);
    auto getYIdx = [&](int y) {
        return std::lower_bound(ys.begin(), ys.end(), y) - ys.begin() + 1; // 1-based
    };

    for (int i = 0; i < q; ++i) {
        int x1 = queries[i].x1, y1 = queries[i].y1;
        int x2 = queries[i].x2, y2 = queries[i].y2;
        // Four prefix queries:
        events.push_back({x2, getYIdx(y2), i, +1});
        events.push_back({x1 - 1, getYIdx(y2), i, -1});
        events.push_back({x2, getYIdx(y1 - 1), i, -1});
        events.push_back({x1 - 1, getYIdx(y1 - 1), i, +1});
    }

    // Sort events by x (ascending)
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        return a.x < b.x;
    });

    // Sweep over x: for each event, we want all points with x <= event.x processed
    std::vector<int> results(q, 0);
    int pt_idx = 0;
    for (const auto& ev : events) {
        // Add points with x <= ev.x
        while (pt_idx < n && pts[pt_idx].x <= ev.x) {
            int yidx = getYIdx(pts[pt_idx].y);
            add(yidx, 1);
            ++pt_idx;
        }
        int prefix = sum(ev.yidx); // count of y <= compressed Y
        results[ev.qid] += ev.sign * prefix;
    }

    return results;
}

#include <cassert>
#include <vector>

// Assume the function from the solution is included above
// Define Rectangle and Point and the function as provided.

int main() {
    // Test 1: empty points
    std::vector<Point> pts1;
    std::vector<Rectangle> q1 = {{0,0,10,10}};
    auto res1 = countPointsInRectangles(pts1, q1);
    assert(res1 == std::vector<int>(1, 0));

    // Test 2: single point inside
    std::vector<Point> pts2 = {{3,4}};
    std::vector<Rectangle> q2 = {{1,1,5,5}};
    auto res2 = countPointsInRectangles(pts2, q2);
    assert(res2 == std::vector<int>(1, 1));

    // Test 3: point on boundary inclusive
    std::vector<Point> pts3 = {{5,5}};
    std::vector<Rectangle> q3 = {{5,5,5,5}};
    auto res3 = countPointsInRectangles(pts3, q3);
    assert(res3 == std::vector<int>(1, 1));

    // Test 4: multiple points, multiple queries, some outside
    std::vector<Point> pts4 = {{1,1}, {2,2}, {3,3}, {10,10}, {0,5}};
    std::vector<Rectangle> q4 = {{0,0,5,5}, {2,2,3,3}, {0,0,0,0}, {1,1,10,10}};
    auto res4 = countPointsInRectangles(pts4, q4);
    // Query 0: points (1,1),(2,2),(3,3),(0,5) => 4
    // Query 1: points (2,2),(3,3) => 2
    // Query 2: point (0,0) none => 0
    // Query 3: all 5 => 5
    assert(res4 == std::vector<int>({4, 2, 0, 5}));

    // Test 5: degenerate rectangle with x1==x2, y1==y2
    std::vector<Point> pts5 = {{7,7}, {8,8}};
    std::vector<Rectangle> q5 = {{7,7,7,7}, {8,8,8,8}, {9,9,9,9}};
    auto res5 = countPointsInRectangles(pts5, q5);
    assert(res5 == std::vector<int>({1,1,0}));

    // Test 6: larger set with negative coordinates
    std::vector<Point> pts6 = {{-5,-5}, {-1,0}, {0,0}, {2,-3}};
    std::vector<Rectangle> q6 = {{-10,-10,0,0}, {-6,-6,5,5}, {1,1,3,3}};
    auto res6 = countPointsInRectangles(pts6, q6);
    // q6[0]: points (-5,-5),(-1,0),(0,0) => 3
    // q6[1]: all 4
    // q6[2]: none => 0
    assert(res6 == std::vector<int>({3,4,0}));

    // Test 7: points exactly on rectangle corners
    std::vector<Point> pts7 = {{1,1}, {1,10}, {10,1}, {10,10}, {5,5}};
    std::vector<Rectangle> q7 = {{1,1,10,10}};
    auto res7 = countPointsInRectangles(pts7, q7);
    assert(res7 == std::vector<int>(1, 5));

    return 0;
}
