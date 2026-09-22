/*
Write a C++ function named `minimumVehicles` that solves the following problem: Given a list of points \((x, y)\) on a 2D plane, a vehicle starts at any point and can move from point A to point B if and only if \(A.x \leq B.x\) and \(A.y \leq B.y\). A single vehicle can visit a sequence of such points following this reachability rule. Find the minimum number of vehicles needed to visit all given points (each point must be visited by exactly one vehicle, and each vehicle starts at a point it visits first). Points are given in a delimited format: multiple test cases, each ending with a line `0 0` (the points themselves have positive coordinates). The end of all input is marked by `-1 -1`. For each test case, return the minimum number of vehicles required. The function should take input as a single string with all tokens (integers) separated by whitespace, and output the answers for each test case as a vector of integers, where each integer is the answer for that test case in order.
*/

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Point {
    int x, y;
    bool used;
};

// Given a whitespace-separated string with multiple test cases,
// return the minimum number of vehicles for each test case.
vector<int> minimumVehicles(const string& input) {
    istringstream iss(input);
    int x, y;
    vector<int> answers;
    while (true) {
        iss >> x >> y;
        if (x == -1 && y == -1) break;
        vector<Point> points;
        while (!(x == 0 && y == 0)) {
            points.push_back({x, y, false});
            iss >> x >> y;
        }
        // Greedy: sort by x then by y.
        sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });
        int vehicles = 0;
        for (size_t i = 0; i < points.size(); ++i) {
            if (points[i].used) continue;
            vehicles++;
            points[i].used = true;
            int currentY = points[i].y;
            for (size_t j = i + 1; j < points.size(); ++j) {
                if (!points[j].used && points[j].y >= currentY) {
                    points[j].used = true;
                    currentY = points[j].y;
                }
            }
        }
        answers.push_back(vehicles);
    }
    return answers;
}

#include <cassert>
#include <vector>
#include <string>

using namespace std;

// Include the solution function here or put it above in a header.

int main() {
    // Single test case: (1,1) then (2,2), end with 0 0 and -1 -1.
    vector<int> ans = minimumVehicles("1 1 2 2 0 0 -1 -1");
    assert(ans.size() == 1 && ans[0] == 1); // One vehicle can go (1,1)->(2,2)

    // Two points incomparable: (1,3) and (3,1) end with 0 0 and -1 -1.
    ans = minimumVehicles("1 3 3 1 0 0 -1 -1");
    assert(ans.size() == 1 && ans[0] == 2); // Need two vehicles

    // Duplicate points: (2,2) twice, cannot chain.
    ans = minimumVehicles("2 2 2 2 0 0 -1 -1");
    assert(ans.size() == 1 && ans[0] == 2);

    // Multiple test cases: first test (1,1)(2,2), second test (5,5) only.
    ans = minimumVehicles("1 1 2 2 0 0 5 5 0 0 -1 -1");
    assert(ans.size() == 2 && ans[0] == 1 && ans[1] == 1);

    // Empty test case: just a single point.
    ans = minimumVehicles("7 9 0 0 -1 -1");
    assert(ans.size() == 1 && ans[0] == 1);

    // More complex case: (1,1),(1,2),(2,1),(3,3) 
    // Greedy: sort: (1,1),(1,2),(2,1),(3,3)
    // First vehicle: (1,1)->(1,2)->(3,3)
    // Second vehicle: (2,1) -> total 2
    ans = minimumVehicles("1 1 1 2 2 1 3 3 0 0 -1 -1");
    assert(ans.size() == 1 && ans[0] == 2);

    return 0;
}

// The key insight is that this problem is equivalent to finding the minimum number of chains (paths) needed to cover all points in a partially ordered set, where point A precedes point B if \(A.x \leq B.x\) and \(A.y \leq B.y\). By Dilworth's theorem, the minimum number of chains equals the size of the largest antichain, but a more direct approach is to model it as a minimum path cover in a directed acyclic graph (DAG). Build a bipartite graph where the left and right sides each represent all points. Add an edge from left point \(i\) to right point \(j\) (for \(i \neq j\)) if point \(i\) can reach point \(j\). Then the minimum path cover number equals \(n - \text{maximum matching size}\). Alternatively, a greedy algorithm works because the points are partially ordered by coordinate dominance: sort points by \(x\), then by \(y\); then repeatedly pick the next point with the smallest \(y\) that is \(\geq\) the current point's \(y\), marking it used. This greedy yields the minimum number of vehicles. Since coordinates are positive and the input ends with `0` or `-1` markers, parse test cases by collecting points until `0 0`; the final `-1 -1` stops all processing. For each test case, compute the answer using the greedy algorithm: sort points, then for each unvisited point, start a new vehicle, then scan forward and greedily take the next point with \(y \geq\) current \(y\). Time complexity: \(O(n^2)\) per test case due to nested scanning, but with sorting \(O(n \log n)\). Space complexity: \(O(n)\). Edge cases: single point per test case yields 1; all points same coordinates yields \(n\) because no point can reach another (since strict inequality in at least one coordinate is needed for movement; the condition \(A.x \leq B.x\) and \(A.y \leq B.y\) allows equality, but if two points are identical, neither can reach the other, so they must be different vehicles).
