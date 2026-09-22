Given three distinct points in the plane with integer coordinates between 0 and 1000 inclusive, write a C++ function `minimalPathPoints` that takes a `vector<pair<int,int>>` of exactly three points and returns a `set<pair<int,int>>` representing the smallest possible connected set of points that contains at least one path between each pair of the three points. The set must be connected in the usual four-directional grid sense (each point in the set must be reachable from every other via up/down/left/right moves staying within the set). The returned set should have minimum cardinality; if multiple optimal sets exist, any is acceptable. The function must work for all inputs with coordinates 0..1000 inclusive and distinct points.
The problem is to minimally connect three grid points. A key insight is that any minimal connected set containing three points must be a tree, and in a grid, the optimal solution is formed by first connecting two of the points with a shortest path (which is an L-shaped path — either go horizontal then vertical, or vertical then horizontal), then connecting the third point to that path with a shortest path. Since a shortest path between two aligned points is a Manhattan path, we have exactly two possible “L-shaped” paths for the first pair: one that goes horizontally first then vertically, and the other vertically first then horizontally. The third point is then connected by a shortest path to any point on that chosen path; this is equivalent to running a BFS from all points on the initial path to find the shortest distance to the third point and reconstructing that shortest path. We also must try all three possible orderings of the three points for the first pair, because the optimal tree might have one point as a central junction, and the smallest overall set among all six combinations (three choices of the first pair × two L-shapes) is the answer. Implementation details: BFS on a 1001×1001 grid is feasible; we mark all points of the initial L-path as distance 0, then run a standard BFS until we reach the third point, recording parents to reconstruct the path. The total set is the union of the initial path and the BFS path. Edge cases: points may be collinear, making the two L-paths sometimes identical; the BFS will still produce a correct minimal path. Also, note that the third point may already lie on the initial path, in which case BFS immediately finds it with distance 0 and no extra points are added. Time complexity: each BFS visits up to O(1000×1000) cells, and we run up to 6 BFS runs (3 orderings × 2 L-shapes) in the worst case, giving O(6×10^6) which is acceptable. Space: O(1000×1000) for distance and parent arrays in each BFS.
#include <vector>
#include <set>
#include <queue>
#include <tuple>
#include <algorithm>
#include <utility>
#include <cstring>

// Solve for a given ordering of the three points: the first two are connected by an L-path,
// then the third is connected by shortest path to that L-path.
static std::set<std::pair<int,int>> solveOrder(
        const std::pair<int,int>& a,
        const std::pair<int,int>& b,
        const std::pair<int,int>& c,
        bool horizontalFirst) {

    // Build the L-shaped path from a to b.
    std::set<std::pair<int,int>> initialPath;
    initialPath.insert(a);
    initialPath.insert(b);
    if (horizontalFirst) {
        int dx = (a.first < b.first) ? 1 : -1;
        for (int x = a.first; x != b.first; x += dx) {
            initialPath.insert({x, a.second});
        }
        // Now go vertical from (b.first, a.second) to b
        int dy = (a.second < b.second) ? 1 : -1;
        for (int y = a.second; y != b.second; y += dy) {
            initialPath.insert({b.first, y});
        }
    } else { // verticalFirst
        int dy = (a.second < b.second) ? 1 : -1;
        for (int y = a.second; y != b.second; y += dy) {
            initialPath.insert({a.first, y});
        }
        int dx = (a.first < b.first) ? 1 : -1;
        for (int x = a.first; x != b.first; x += dx) {
            initialPath.insert({x, b.second});
        }
    }

    // BFS from all points in initialPath to find shortest path to c.
    const int N = 1001;
    static int dist[N][N];
    static std::pair<int,int> parent[N][N];
    // Initialize dist to a large value.
    memset(dist, 0x3f, sizeof(dist));
    std::queue<std::pair<int,int>> q;
    for (const auto& p : initialPath) {
        dist[p.first][p.second] = 0;
        q.push(p);
    }

    // Directions: up, down, left, right.
    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    bool found = false;
    while (!q.empty() && !found) {
        auto cur = q.front(); q.pop();
        int i = cur.first, j = cur.second;
        if (cur == c) {
            found = true;
            break;
        }
        for (int d = 0; d < 4; ++d) {
            int ni = i + dx[d];
            int nj = j + dy[d];
            if (ni < 0 || ni > 1000 || nj < 0 || nj > 1000) continue;
            if (dist[ni][nj] > dist[i][j] + 1) {
                dist[ni][nj] = dist[i][j] + 1;
                parent[ni][nj] = cur;
                q.push({ni, nj});
            }
        }
    }

    // Reconstruct path from c back to the initial path.
    std::set<std::pair<int,int>> result = initialPath;
    std::pair<int,int> cur = c;
    result.insert(cur);
    while (dist[cur.first][cur.second] != 0) {
        cur = parent[cur.first][cur.second];
        result.insert(cur);
    }
    return result;
}

// Returns the minimal connected set of grid points containing all three given points.
std::set<std::pair<int,int>> minimalPathPoints(const std::vector<std::pair<int,int>>& points) {
    // points must have size 3.
    std::vector<std::pair<int,int>> v = points;
    std::set<std::pair<int,int>> best;
    bool first = true;

    // Try all 6 permutations of the three points as the first pair and the third.
    // We keep the original order for the first, then swap to get all permutations.
    std::sort(v.begin(), v.end());
    do {
        auto a = v[0], b = v[1], c = v[2];
        // Two possible L-shapes for a-b.
        auto candidate1 = solveOrder(a, b, c, true);
        auto candidate2 = solveOrder(a, b, c, false);
        if (first) {
            best = candidate1.size() < candidate2.size() ? candidate1 : candidate2;
            first = false;
        } else {
            if (candidate1.size() < best.size()) best = candidate1;
            if (candidate2.size() < best.size()) best = candidate2;
        }
    } while (std::next_permutation(v.begin(), v.end()));

    return best;
}
#include <cassert>
#include <vector>
#include <set>
#include <utility>

// The solution function declaration is assumed to be available.
std::set<std::pair<int,int>> minimalPathPoints(const std::vector<std::pair<int,int>>& points);

int main() {
    // Test 1: Three collinear points on the same line.
    std::vector<std::pair<int,int>> p1 = {{0,0}, {5,0}, {10,0}};
    auto r1 = minimalPathPoints(p1);
    assert(r1.size() == 11); // points 0..10 on x-axis.

    // Test 2: Points forming a right triangle.
    std::vector<std::pair<int,int>> p2 = {{0,0}, {3,0}, {0,4}};
    auto r2 = minimalPathPoints(p2);
    // The minimal set is the L path from (0,0) to (3,0) to (3,4) or similar.
    // The shortest tree is to connect (0,0)-(3,0)-(3,4) or (0,0)-(0,4)-(3,4), both size 3+1+4? Actually:
    // Path (0,0)->(3,0)->(3,4) has 3 horizontal + 4 vertical + 1 for the corner = 8 points? Let's count:
    // Points: (0,0),(1,0),(2,0),(3,0),(3,1),(3,2),(3,3),(3,4) = 8.
    // Alternative: (0,0),(1,0),(2,0),(3,0),(3,1),(3,2),(3,3),(3,4) same. So size 8.
    assert(r2.size() == 8);

    // Test 3: Points far apart.
    std::vector<std::pair<int,int>> p3 = {{0,0}, {1000,1000}, {0,1000}};
    auto r3 = minimalPathPoints(p3);
    // The minimal tree is either go along two sides of the square: (0,0)->(1000,0)->(1000,1000) and then (0,1000) connects to (0,0)? Actually the three points form a right triangle with legs 1000 each. The minimal Steiner tree is to connect (0,0) to (1000,1000) via (0,1000) or (1000,0). That would be 1000+1000+1=2001 points? Let's calculate: path (0,0)->(0,1000)->(1000,1000) has 1000 vertical + 1000 horizontal + 1 corner = 2001 points. But also we could connect (0,0) to (1000,1000) via (1000,0) similar. Actually the minimal is 2001. So assert size == 2001.
    assert(r3.size() == 2001);

    // Test 4: Two points same line, third off to the side.
    std::vector<std::pair<int,int>> p4 = {{2,2}, {2,6}, {5,4}};
    auto r4 = minimalPathPoints(p4);
    // The optimal is likely to connect (2,2)-(2,6) (vertical line of 5 points) then connect (5,4) to the nearest point on that line, which is (2,4) with a horizontal path of 3 points (2,4),(3,4),(4,4),(5,4) so total 5+3 = 8? Actually the line has points (2,2),(2,3),(2,4),(2,5),(2,6) = 5, then add (3,4),(4,4),(5,4) = 3, total 8. But also could connect via (2,5) or (2,3) giving different distances (2,5) to (5,4) is 3+1=4? Wait (2,5)->(2,4)->(3,4)->(4,4)->(5,4) is 4 steps, so total 5+4=9. So optimal is 8. Assert size 8.
    assert(r4.size() == 8);

    // Test 5: Points already in a straight vertical line with gaps.
    std::vector<std::pair<int,int>> p5 = {{0,0}, {0,3}, {0,7}};
    auto r5 = minimalPathPoints(p5);
    // The minimal set is all points from (0,0) to (0,7) inclusive, size 8.
    assert(r5.size() == 8);

    // Test 6: Zero coordinates.
    std::vector<std::pair<int,int>> p6 = {{0,0}, {0,1}, {1,0}};
    auto r6 = minimalPathPoints(p6);
    // The minimal tree is a 2x2 square minus one corner? Actually the three points form an L shape: (0,0)-(0,1)-(1,0) requires either (0,0)-(0,1)-(1,1)-(1,0) or (0,0)-(1,0)-(1,1)-(0,1). Both have 4 points. So size 4.
    assert(r6.size() == 4);

    // Test 7: Duplicates not allowed per problem, but test that it works if accidentally passed? We'll skip.

    // Test 8: Points all far apart but forming a line with a missing middle.
    std::vector<std::pair<int,int>> p7 = {{0,0}, {1,0}, {3,0}};
    auto r7 = minimalPathPoints(p7);
    // Need to fill (2,0) to connect, so size 4.
    assert(r7.size() == 4);

    return 0;
}
