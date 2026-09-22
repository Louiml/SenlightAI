/*
Given a set of \(N\) points in the 2D plane with integer coordinates, and two special points \(A\) and \(B\), define the Chebyshev distance between \(A\) and \(B\) as \(R = \max(|x_A - x_B|, |y_A - y_B|)\). A point \(P\) is said to be "reachable" if it lies on any of the four axis-aligned lines (in the original coordinate system) that pass through \(A\) or \(B\) and are at horizontal or vertical distance exactly \(R\) from them—specifically, any point whose \(x\) coordinate equals \(x_A \pm R\) or \(x_B \pm R\), or whose \(y\) coordinate equals \(y_A \pm R\) or \(y_B \pm R\). Starting from the two given points \(A\) and \(B\), you may repeatedly visit any reachable point that is within Chebyshev distance \(R\) from any already visited point, and then all points that share that same line (same \(x\) or same \(y\) as the visited point) and are within distance \(R\) also become visited. Write a C++ function `long long countReachablePairs(int N, int A, int B, const vector<pair<int,int>>& pts)` that takes \(N\) (the number of points, 1-indexed), the 1-indexed IDs of \(A\) and \(B\), and the list of points (each with integer coordinates), and returns the total number of unordered pairs \((u,v)\) such that both \(u\) and \(v\) are among the visited points (including \(A\) and \(B\)) and their Chebyshev distance is exactly \(R\). The points have distinct IDs but may share coordinates. The function must handle up to \(N = 100,000\) points efficiently.
*/

#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <cstdint>

struct Point2D {
    int u, v, id;
    Point2D(int _u = 0, int _v = 0, int _id = 0) : u(_u), v(_v), id(_id) {}
};

struct Node2D {
    int coord, id;
    Node2D(int _coord = 0, int _id = 0) : coord(_coord), id(_id) {}
    bool operator<(const Node2D& other) const {
        return coord < other.coord;
    }
};

long long countReachablePairs(int N, int A, int B, const std::vector<std::pair<int,int>>& pts) {
    // pts is 0-indexed, but IDs are 1..N; A and B are 1-indexed
    std::vector<Point2D> P(N + 1);
    for (int i = 1; i <= N; ++i) {
        int x = pts[i-1].first;
        int y = pts[i-1].second;
        P[i] = Point2D(x - y, x + y, i);
    }

    // Compute Chebyshev distance in original coordinates; same as in (u,v)
    int R = std::max(std::abs(P[A].u - P[B].u), std::abs(P[A].v - P[B].v));

    // Maps from u coordinate to set of (v, id) and v coordinate to set of (u, id)
    std::map<int, std::set<Node2D>> sU, sV;

    // Sorted vectors for counting later
    std::map<int, std::vector<Node2D>> vecU, vecV;

    for (int i = 1; i <= N; ++i) {
        if (i != A && i != B) {
            sU[P[i].u].insert(Node2D(P[i].v, i));
            sV[P[i].v].insert(Node2D(P[i].u, i));
        }
        vecU[P[i].u].push_back(Node2D(P[i].v, i));
        vecV[P[i].v].push_back(Node2D(P[i].u, i));
    }

    // Sort the vectors for binary search
    for (auto& kv : vecU) {
        std::sort(kv.second.begin(), kv.second.end());
    }
    for (auto& kv : vecV) {
        std::sort(kv.second.begin(), kv.second.end());
    }

    // BFS
    std::queue<int> q;
    std::vector<int> visitedIds;
    q.push(A);
    q.push(B);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visitedIds.push_back(u);

        // Check line u + R
        auto itMap = sU.find(P[u].u + R);
        if (itMap != sU.end()) {
            auto& s = itMap->second;
            auto it = s.lower_bound(Node2D(P[u].v - R, 0));
            for (auto it2 = it; it2 != s.end() && it2->coord <= P[u].v + R; ) {
                q.push(it2->id);
                // Remove from other set
                auto& otherSet = sV[P[it2->id].v];
                otherSet.erase(Node2D(P[it2->id].u, it2->id));
                it2 = s.erase(it2);
            }
        }

        // Check line u - R
        itMap = sU.find(P[u].u - R);
        if (itMap != sU.end()) {
            auto& s = itMap->second;
            auto it = s.lower_bound(Node2D(P[u].v - R, 0));
            for (auto it2 = it; it2 != s.end() && it2->coord <= P[u].v + R; ) {
                q.push(it2->id);
                auto& otherSet = sV[P[it2->id].v];
                otherSet.erase(Node2D(P[it2->id].u, it2->id));
                it2 = s.erase(it2);
            }
        }

        // Check line v + R
        itMap = sV.find(P[u].v + R);
        if (itMap != sV.end()) {
            auto& s = itMap->second;
            auto it = s.lower_bound(Node2D(P[u].u - R, 0));
            for (auto it2 = it; it2 != s.end() && it2->coord <= P[u].u + R; ) {
                q.push(it2->id);
                auto& otherSet = sU[P[it2->id].u];
                otherSet.erase(Node2D(P[it2->id].v, it2->id));
                it2 = s.erase(it2);
            }
        }

        // Check line v - R
        itMap = sV.find(P[u].v - R);
        if (itMap != sV.end()) {
            auto& s = itMap->second;
            auto it = s.lower_bound(Node2D(P[u].u - R, 0));
            for (auto it2 = it; it2 != s.end() && it2->coord <= P[u].u + R; ) {
                q.push(it2->id);
                auto& otherSet = sU[P[it2->id].u];
                otherSet.erase(Node2D(P[it2->id].v, it2->id));
                it2 = s.erase(it2);
            }
        }
    }

    // Count unordered pairs of visited points with Chebyshev distance exactly R
    long long ans = 0;
    for (int u : visitedIds) {
        // For u-lines: include points with v diff exactly R (i.e., entire interval)
        if (vecU.count(P[u].u + R)) {
            const auto& vec = vecU[P[u].u + R];
            auto l = std::lower_bound(vec.begin(), vec.end(), Node2D(P[u].v - R, 0));
            auto r = std::upper_bound(vec.begin(), vec.end(), Node2D(P[u].v + R, 0));
            ans += (long long)(r - l);
        }
        if (vecU.count(P[u].u - R)) {
            const auto& vec = vecU[P[u].u - R];
            auto l = std::lower_bound(vec.begin(), vec.end(), Node2D(P[u].v - R, 0));
            auto r = std::upper_bound(vec.begin(), vec.end(), Node2D(P[u].v + R, 0));
            ans += (long long)(r - l);
        }
        // For v-lines: exclude points with u diff exactly R to avoid double counting
        if (vecV.count(P[u].v + R)) {
            const auto& vec = vecV[P[u].v + R];
            auto l = std::lower_bound(vec.begin(), vec.end(), Node2D(P[u].u - R + 1, 0));
            auto r = std::upper_bound(vec.begin(), vec.end(), Node2D(P[u].u + R - 1, 0));
            ans += (long long)(r - l);
        }
        if (vecV.count(P[u].v - R)) {
            const auto& vec = vecV[P[u].v - R];
            auto l = std::lower_bound(vec.begin(), vec.end(), Node2D(P[u].u - R + 1, 0));
            auto r = std::upper_bound(vec.begin(), vec.end(), Node2D(P[u].u + R - 1, 0));
            ans += (long long)(r - l);
        }
    }
    return ans / 2;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined above.

int main() {
    // Test 1: two points directly at distance R
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {2,0}, {1,1}};
        // A=1, B=2, R=2. Reachable: A(0,0) and B(2,0) only? Check point (1,1): Chebyshev distance from A is 1, from B is 1, not exactly 2? Actually R=2, but (1,1) is not on any line at distance 2 from A or B. So visited = {A,B}. Pairs: (A,B) distance 2 -> 1 pair.
        long long res = countReachablePairs(3, 1, 2, pts);
        assert(res == 1);
    }

    // Test 2: four points forming a square
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {2,0}, {0,2}, {2,2}};
        // A=1, B=2, R=2. BFS: A(0,0) and B(2,0). A can reach points on lines x=0 and x=2? Actually check: u = x-y, v=x+y.
        // Points: (0,0)->u=0,v=0; (2,0)->u=2,v=2; (0,2)->u=-2,v=2; (2,2)->u=0,v=4. R between A and B: max(|0-2|,|0-2|)=2. From A, line u+2=2 -> points with u=2: only B (already). line u-2=-2 -> point (0,2) has u=-2? yes, v=2, within [0-2,0+2]? v=2 in [-2,2]? yes, so (0,2) visited. Also v+2=2 -> points with v=2: (2,0) and (0,2) both? Actually v=2 line includes (2,0) and (0,2). (0,2) already? Actually (0,2) not visited yet but will be via u-2. Then from (0,2), it can reach (2,2) because u+2=0? Actually (2,2) has u=0, v=4. From (0,2) u=-2, v=2, u+2=0 -> point with u=0: (0,0) and (2,2) both? (2,2) u=0, v=4, v within [2-2,2+2]=[0,4]? yes, so visited. So all four visited. Pairs with dist 2: all 6 pairs? Check (0,0)-(2,2) distance=2, (0,0)-(0,2) dist=2, etc. Actually all pairs have distance 2 because max differences are 2. So 6 pairs.
        long long res = countReachablePairs(4, 1, 2, pts);
        assert(res == 6);
    }

    // Test 3: R=0, same point? But A and B distinct, so R>0. Test when A and B are adjacent.
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {2,0}};
        // A=1(0,0), B=2(1,0), R=1. From A, line u+1=1? u=0+1=1, point B has u=1? For B(1,0): u=1,v=1? Actually u=1-0=1, v=1+0=1. So B. Also from A line u-1=-1 none. v+1=1? v=0+1=1, points with v=1: B only. v-1=-1 none. From B, similarly. Point (2,0): u=2,v=2. From B, u+1=2 -> line u=2, v within [1-1,1+1]=[0,2], point (2,0) v=2 in range, so visited. So visited all three. Pairs with distance 1: (A,B) yes, (B,C) yes, (A,C) distance 2 not. So 2 pairs.
        long long res = countReachablePairs(3, 1, 2, pts);
        assert(res == 2);
    }

    // Test 4: isolated points not reachable
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {5,5}, {1,0}};
        // A=1, B=3? Let's set A=1(0,0), B=3(1,0), R=1. Point (5,5) not reachable. Visited: A and B and maybe point? From A, u+1=1? none. v+1=1? none. So only A,B. Pairs: dist 1 -> 1.
        long long res = countReachablePairs(3, 1, 3, pts);
        assert(res == 1);
    }

    // Test 5: duplicate coordinates with different IDs
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {2,0}, {0,0}, {2,0}};
        // A=1(0,0), B=2(2,0), R=2. Points 3 and 4 same coords. All four will be visited? From A, u+2=2 -> points with u=2: B(2,0) and point 4 (2,0) both v=2. So they are visited. From B, u-2=0 -> A and point3. So all visited. Pairs with dist 2: any pair among the four? All pairs have distance either 0 (same coords) or 2. So pairs with dist 2: pairs (1,2),(1,4),(3,2),(3,4) = 4 pairs? Also (1,2) and (1,4) etc. Actually (1,3) dist 0, (2,4) dist 0. So 4 unordered pairs.
        long long res = countReachablePairs(4, 1, 2, pts);
        assert(res == 4);
    }

    // Test 6: large R with a chain
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {3,0}, {6,0}, {9,0}};
        // A=1(0,0), B=2(3,0), R=3. From A, line u+3=3 -> B only. v lines: v+3=3? v=0+3=3, points with v=3? B(3,0) v=3? u=3,v=3 yes. From B, can reach point 3 (6,0) because u+3=6? B u=3, +3=6, v=6, within [3-3,3+3]=[0,6]? yes. Then point4 similarly. So all visited. Pairs with dist 3: (1,2),(2,3),(3,4) = 3 pairs. Also (1,3) dist 6 not.
        long long res = countReachablePairs(4, 1, 2, pts);
        assert(res == 3);
    }

    // Test 7: points forming a diamond shape
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,1}, {2,0}, {1,-1}};
        // A=1(0,0), B=2(1,1), R=1. Chebyshev distance between (0,0) and (1,1) = 1. From A, u=0, v=0. u+1=1 -> points with u=1: B and? (1,1) u=0? Actually (1,1): u=0, v=2. So no. v+1=1 -> points with v=1? none. u-1=-1 none. v-1=-1 none. From B, similarly. Point (2,0): u=2,v=2. From B, u+1=2? B u=0? B u=1-1=0? Actually B(1,1): u=0, v=2. u+1=1? not. So not reachable. Point (1,-1): u=2,v=0. Not reachable. So only A,B. Pairs: dist 1 -> 1.
        long long res = countReachablePairs(4, 1, 2, pts);
        assert(res == 1);
    }

    // Test 8: all points on same line
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {2,0}, {4,0}, {6,0}};
        // A=1, B=2, R=2. From A, u+2=2 -> B only. From B, u+2=4 -> point3, then point4. All visited. Pairs with dist 2: (1,2),(2,3),(3,4) = 3 pairs.
        long long res = countReachablePairs(4, 1, 2, pts);
        assert(res == 3);
    }

    // Test 9: zero points? not allowed N>=2. Test with N=2 only.
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {3,4}};
        // A=1, B=2, R=5. Only two points, visited both. Pair distance = 5, so 1.
        long long res = countReachablePairs(2, 1, 2, pts);
        assert(res == 1);
    }

    // Test 10: random small brute check
    {
        // Brute force check with 5 points
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {2,0}, {0,1}, {2,1}};
        // A=1(0,0), B=3(2,0), R=2. BFS: from A, u+2=2 -> B? B u=2,v=2 yes. Also v+2=2? v=0+2=2, points with v=2: B? also (2,1) u=1,v=3? no. So B visited. From B, u-2=0 -> A only. v-2=0? v=2-2=0, points with v=0: A and (1,0) u=1,v=1? no v=1. Actually (1,0) v=1. So no. So visited {A,B}. Pairs with dist 2: only (A,B) dist 2. Also (A,(2,1)) dist 2? A(0,0) to (2,1) chebyshev=2, but (2,1) not visited because not on lines? Is (2,1) reachable? Not from A or B because its u=1, v=3; not on u=2 or u=-2 or v=2 or v=-2? For A, u+2=2, v in [-2,2]? v of (2,1) is 3, not in range. From B, u-2=0? (2,1) u=1, no. So not. So ans=1.
        long long res = countReachablePairs(5, 1, 3, pts);
        assert(res == 1);
    }

    return 0;
}

// The problem is based on simulating a BFS over points that are "aligned" in transformed coordinates. The key transformation is \(u = x - y\) and \(v = x + y\), because then the Chebyshev distance \(\max(|x_1 - x_2|, |y_1 - y_2|)\) becomes the Chebyshev distance in \((u,v)\) space, which is \(\max(|u_1 - u_2|, |v_1 - v_2|)\). The four "lines" containing reachable points become vertical or horizontal lines in \((u,v)\) space: points with \(u = x_A \pm R\) or \(u = x_B \pm R\), and points with \(v = y_A \pm R\) or \(v = y_B \pm R\). Because the transformation is bijective, two points are at Chebyshev distance \(R\) iff their \(u\)-coordinates differ by exactly \(R\) or their \(v\)-coordinates differ by exactly \(R\) (since in Chebyshev, if the difference is exactly \(R\), one coordinate difference must equal \(R\) while the other is at most \(R\)). The BFS visits all points that can be reached from \(A\) or \(B\) by repeatedly jumping to points that share the same \(u\) or \(v\) coordinate as a visited point and have coordinate difference exactly \(R\) in the other dimension. To do this efficiently, we maintain maps from each distinct \(u\) coordinate to a sorted set of \((v, id)\) pairs, and similarly for each \(v\) coordinate to set of \((u, id)\) pairs. When visiting a point, we look up the four possible lines: \(u \pm R\) and \(v \pm R\). For a line \(u = c\), we need all points with \(v\) in the interval \([v_{current} - R, v_{current} + R]\); we find the lower bound and iterate through the set, erasing visited points from both the \(u\)-set and the corresponding \(v\)-set to avoid reprocessing. This BFS ensures each point is visited at most once. After BFS, we have a list of visited point IDs. We then count the total number of ordered pairs of visited points (including possibly same point? The final answer divides by 2) that have Chebyshev distance exactly \(R\). For each visited point, we count points on the four lines \(u \pm R\) or \(v \pm R\) within the appropriate coordinate ranges using sorted vectors (precomputed) to count via binary search. We must be careful not to double-count pairs that appear on both a horizontal and vertical line (i.e., points that differ both in \(u\) and \(v\) by exactly \(R\)? But in Chebyshev distance exactly \(R\), both coordinate differences must be \(\le R\) and one must be exactly \(R\); if both are exactly \(R\), then that pair would be counted once via the \(u\)-difference and once via the \(v\)-difference. The original code handles this by using different boundaries: for \(v\)-lines it uses \(u \in [u - R + 1, u + R - 1]\), excluding those with \(u\) difference exactly \(R\), so the double-count is avoided. The total count is then divided by 2 because each unordered pair is counted twice. Edge cases include points coinciding with A or B (handled by excluding them from the sets initially), duplicate coordinates (but distinct IDs), and when R = 0, the BFS only visits A and B if they are identical? But A and B are distinct points usually. Time complexity: building sorted sets O(N log N), BFS visits each point once O(N log N) for set operations, and counting with binary search O(N log N) total. Space O(N).
