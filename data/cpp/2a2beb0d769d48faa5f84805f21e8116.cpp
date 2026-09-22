// Write a C++ function `vector<int> goodPoints(const vector<vector<int>>& points)` that takes a list of \(n\) points in 5-dimensional Euclidean space (each point is a vector of 5 integers, with indices 1-based in the original list but 0-based in the vector), and returns the indices (1-based) of all points \(P_i\) such that for every pair of other points \(P_j, P_k\) with \(j < k\), the angle at \(P_i\) formed by vectors \(P_j - P_i\) and \(P_k - P_i\) is **not acute**, i.e., the dot product \((P_j - P_i) \cdot (P_k - P_i) \leq 0\). Return the indices in increasing order. The input may contain duplicate points, and \(n\) can be as small as 1. If \(n=1\) or \(n=2\), all points are good because the condition vacuously holds (no pair to test). For performance, note that the original snippet uses a naive \(O(n^3)\) approach, but you may implement any correct algorithm; still, the expected solution should work for \(n \leq 100\) within reasonable time (original is $O(n^3 \cdot d)$ with \(d=5\)). Ensure your function is self-contained and does not rely on global state.
#include <cassert>
#include <vector>

// Function declaration from solution
std::vector<int> goodPoints(const std::vector<std::vector<int>>& points);

int main() {
    // Single point: vacuous, all good.
    std::vector<std::vector<int>> p1 = {{0,0,0,0,0}};
    assert(goodPoints(p1) == std::vector<int>{1});

    // Two points: no pair to test, both good.
    std::vector<std::vector<int>> p2 = {{1,2,3,4,5}, {5,4,3,2,1}};
    assert(goodPoints(p2) == std::vector<int>{1,2});

    // Three points on a line: middle point has two vectors opposite, dot negative -> good.
    std::vector<std::vector<int>> p3 = {{0,0,0,0,0}, {1,0,0,0,0}, {2,0,0,0,0}};
    // For i=0 (point 1): vectors to j=1 and k=2 both positive -> dot positive -> bad.
    // For i=1: vectors to j=0 and k=2 are opposite -> dot negative -> good.
    // For i=2: vectors to j=0 and k=1 both negative -> dot positive -> bad.
    assert(goodPoints(p3) == std::vector<int>{2});

    // Three points forming a right angle at the origin: origin good, others? check.
    std::vector<std::vector<int>> p4 = {{0,0,0,0,0}, {1,0,0,0,0}, {0,1,0,0,0}};
    // i=0: vectors (1,0,..) and (0,1,..) dot=0 -> not acute -> good.
    // i=1: vectors to j=0 (-1,0,..) and k=2 (-1,1,..) dot = (-1)*(-1)+0*1=1>0 -> bad.
    // i=2 similarly bad.
    assert(goodPoints(p4) == std::vector<int>{1});

    // Four points: square in 2D embedded in 5D, all four should be good (right angles).
    std::vector<std::vector<int>> p5 = {
        {0,0,0,0,0}, {1,0,0,0,0}, {0,1,0,0,0}, {1,1,0,0,0}
    };
    // For each vertex, all pairs of other points: some dot = 0, some negative? Let's test.
    // Origin: vectors to (1,0) and (0,1) dot=0; to (1,0) and (1,1) dot=1>0 -> bad? Actually (1,0)·(1,1)=1>0 -> origin bad.
    // So not all good. Let's use a regular simplex? Simpler: a line with 4 points works.
    std::vector<std::vector<int>> p6 = {{0,0,0,0,0}, {1,0,0,0,0}, {2,0,0,0,0}, {3,0,0,0,0}};
    // Only the two middle points are good (i=1 and i=2) because endpoints have same direction pairs.
    assert(goodPoints(p6) == std::vector<int>{2,3});

    // Duplicate points: all identical, any candidate has zero vectors, dot=0 -> all good.
    std::vector<std::vector<int>> p7 = {{1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1}};
    assert(goodPoints(p7) == std::vector<int>{1,2,3});

    // Larger n, random test: manually check simple case with 4 points on a circle? Use 2D.
    // Let's do a simple known: a regular tetrahedron in 3D embedded in 5D.
    // But to keep it simple, test with 5 random points and compare with naive brute force inline.
    std::vector<std::vector<int>> p8 = {{2,3,5,7,11}, {13,17,19,23,29}, {31,37,41,43,47}, {53,59,61,67,71}};
    // Compute expected via direct loop in test.
    std::vector<int> expected;
    int n = p8.size();
    for (int i=0;i<n;i++) {
        bool bad=false;
        for (int j=0;j<n && !bad;j++) if(i!=j)
            for (int k=j+1;k<n && !bad;k++) if(i!=k) {
                int dot=0;
                for (int d=0; d<5; d++) dot+=(p8[k][d]-p8[i][d])*(p8[j][d]-p8[i][d]);
                if(dot>0) { bad=true; break; }
            }
        if(!bad) expected.push_back(i+1);
    }
    assert(goodPoints(p8) == expected);
}
#include <vector>
#include <algorithm>

// Returns 1-based indices of all points that never form an acute angle at themselves
// with any pair of other points. Points are in 5D, given as vector<int> of size 5.
std::vector<int> goodPoints(const std::vector<std::vector<int>>& points) {
    const int n = static_cast<int>(points.size());
    const int dim = 5;
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        bool bad = false;
        for (int j = 0; j < n && !bad; ++j) {
            if (i == j) continue;
            for (int k = j + 1; k < n && !bad; ++k) {
                if (i == k) continue;
                int dot = 0;
                for (int d = 0; d < dim; ++d) {
                    dot += (points[k][d] - points[i][d]) * (points[j][d] - points[i][d]);
                }
                if (dot > 0) {
                    bad = true;
                    break;
                }
            }
        }
        if (!bad) {
            result.push_back(i + 1);
        }
    }
    return result;
}
// The problem asks to find all points that never lie at the vertex of an acute angle formed by two other points. For each candidate point \(P_i\), we need to check every pair of other points \((P_j, P_k)\), \(j<k\), and compute the dot product of the two vectors from \(P_i\) to \(P_j\) and \(P_i\) to \(P_k\). If any dot product is strictly positive, then the angle at \(P_i\) is acute, so \(P_i\) is rejected. If no pair produces a positive dot product, \(P_i\) is kept. This is a direct translation of the condition. The naive triple nested loop over \(i, j, k\) runs in \(O(n^3)\) time, and each dot product takes \(O(5)=O(1)\) time, so total \(O(n^3)\). Space is \(O(n)\) for the answer. Edge cases: \(n=1\) or \(n=2\) automatically satisfy the condition because there are no pairs \((j,k)\) with both distinct from \(i\) and \(j<k\). Duplicate points are allowed, and they are treated as normal points; if a duplicate point is the candidate, the vectors to other points might be zero? Actually, vectors are computed as difference, so if candidate equals another point, vector length zero, but the dot product with any other vector is zero, so it does not cause acute angle. However, if two other points are the same as the candidate, then the vector from candidate to them is zero, dot product zero, still not positive. So duplicates don't cause false rejection. The algorithm is straightforward.
