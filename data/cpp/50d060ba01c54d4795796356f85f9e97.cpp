/*
Given a set of N points on a 2D Cartesian plane, each point has a type ('H' for holstein or 'G' for guernsey) and integer coordinates (x, y) where 0 ≤ x, y ≤ 1000. Write a C++ function that determines, among all axis-aligned rectangles that contain at least one point of type H but NO points of type G in their interior, the maximum number of H-points such a rectangle can contain. If multiple rectangles achieve this maximum, return the one with the smallest area. The function should return a pair (maxCount, minArea) where area is computed as (maxX - minX) * (maxY - minY). Rectangles are defined by their bounding box of selected points; edges may pass through points of either type, but no G point may be strictly inside. All coordinates are integers.
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Returns the maximum number of H points in an axis-aligned rectangle that contains no G points,
// and the minimum area achieving that maximum.
// Input: vector of (x, y, isHolstein) where isHolstein is true for H, false for G.
pair<int, ll> maxHolsteinRect(const vector<tuple<int,int,bool>>& points) {
    const int MAX_COORD = 1001;
    int N = (int)points.size();
    
    // Build vector of (x, (type, y)) where type: 0=G, 1=H
    vector<pair<int, pair<int,int>>> vec;
    vec.reserve(N);
    for (auto& [x, y, isH] : points) {
        int type = isH ? 1 : 0;
        vec.push_back({x, {type, y}});
    }
    sort(vec.begin(), vec.end());  // sort by x, then type, then y
    
    int bestCount = 0;
    ll bestArea = LLONG_MAX;
    
    // For each left boundary
    for (int i = 0; i < N; ++i) {
        int yCount[MAX_COORD] = {0};     // H counts per y
        bool gPresent[MAX_COORD] = {false}; // G presence per y
        
        // Expand right boundary
        for (int j = i; j < N; ++j) {
            int xLeft = vec[i].first;
            int xRight = vec[j].first;
            int type = vec[j].second.first;
            int y = vec[j].second.second;
            
            if (type == 1) {
                yCount[y]++;
            } else {
                gPresent[y] = true;
            }
            
            // Skip processing until we finish all points with this xRight
            if (j + 1 < N && vec[j+1].first == xRight) {
                continue;
            }
            
            // Scan y from 0 to 1000
            int currentH = 0;
            int lowY = -1, highY = -1;
            int bestH = 0;
            int bestLow = -1, bestHigh = -1;
            
            for (int yCoords = 0; yCoords < MAX_COORD; ++yCoords) {
                if (gPresent[yCoords]) {
                    // Reset current segment
                    currentH = 0;
                    lowY = -1;
                    highY = -1;
                    continue;
                }
                if (yCount[yCoords] == 0) {
                    continue;
                }
                // Extend current segment
                currentH += yCount[yCoords];
                if (lowY == -1) {
                    lowY = highY = yCoords;
                } else {
                    highY = yCoords;
                }
                // Update best segment for this x-range
                if (currentH > bestH) {
                    bestH = currentH;
                    bestLow = lowY;
                    bestHigh = highY;
                } else if (currentH == bestH && bestH > 0) {
                    ll area = (ll)(xRight - xLeft) * (bestHigh - bestLow);
                    ll newArea = (ll)(xRight - xLeft) * (highY - lowY);
                    if (newArea < area) {
                        bestLow = lowY;
                        bestHigh = highY;
                    }
                }
            }
            
            // Update global ans
            if (bestH > bestCount) {
                bestCount = bestH;
                bestArea = (ll)(xRight - xLeft) * (bestHigh - bestLow);
            } else if (bestH == bestCount && bestH > 0) {
                bestArea = min(bestArea, (ll)(xRight - xLeft) * (bestHigh - bestLow));
            }
        }
    }
    if (bestCount == 0) bestArea = 0;  // no H points
    return {bestCount, bestArea};
}
#include <bits/stdc++.h>
#include <cassert>
#include <tuple>
using namespace std;

pair<int, ll> maxHolsteinRect(const vector<tuple<int,int,bool>>& points); // declare

int main() {
    // Single H point, no G
    auto res1 = maxHolsteinRect({{0,0,true}});
    assert(res1.first == 1 && res1.second == 0);
    
    // Two H points same x, no G
    auto res2 = maxHolsteinRect({{0,0,true},{0,5,true}});
    assert(res2.first == 2 && res2.second == 0);
    
    // H points separated by G same x -> cannot include both
    auto res3 = maxHolsteinRect({{0,0,true},{0,1,false},{0,2,true}});
    assert(res3.first == 1 && res3.second == 0);
    
    // Simple 2D case: 2 H on same y, G in middle x
    auto res4 = maxHolsteinRect({
        {0,0,true}, {1,0,false}, {2,0,true}
    });
    assert(res4.first == 1 && res4.second == 0); // cannot include both H because G inside
    
    // H on same row and column: 3 H in L-shape
    auto res5 = maxHolsteinRect({
        {0,0,true}, {1,0,true}, {0,1,true}
    });
    assert(res5.first == 3 && res5.second == 1); // area (1-0)*(1-0)=1
    
    // 2 H with one G blocking vertical, choose smaller area
    auto res6 = maxHolsteinRect({
        {0,0,true}, {0,2,true}, {0,1,false}, {5,0,true}
    });
    // Can take {0,0} and {5,0}? No, G at (0,1) not inside if rectangle y from 0 to 0, x from 0 to 5 -> valid, count=2, area=5*0=0
    // Also can take {0,2} alone ->1. So best is count=2 area=0
    assert(res6.first == 2 && res6.second == 0);
    
    // More complex: G blocks middle, but two H above and below
    auto res7 = maxHolsteinRect({
        {0,0,true}, {0,2,true}, {0,1,false}, {0,3,true}, {8,0,true}
    });
    // Best: take {0,0}, {8,0} -> count=2 area=0 ; or take {0,2} and {0,3}? without G? Actually G at (0,1) only affects y=1, so taking y=2..3 is fine -> count=2 area=0? No x range just one point, area=0. 
    // Also could take {0,0},{8,0},{0,2}? That rectangle x 0..8, y 0..2 includes G at y=1 -> invalid.
    // So best count=2 area=0
    assert(res7.first == 2 && res7.second == 0);
    
    // Test where area matters: two different rectangles with same count
    auto res8 = maxHolsteinRect({
        {0,0,true}, {10,0,true}, {0,10,true}, {10,10,true}
    });
    // Count=4 rectangle x 0..10, y 0..10 area=100; but also any subset? No G, so all 4 can be inside, area=100. Only one rectangle with count 4, area 100.
    assert(res8.first == 4 && res8.second == 100);
    
    // Same count but different areas: choose minimal area
    auto res9 = maxHolsteinRect({
        {0,0,true}, {3,0,true}, {0,3,true}, {2,2,true}
    });
    // Best count=4 rectangle x 0..3, y 0..3 area=9 (since all H in that box)
    assert(res9.first == 4 && res9.second == 9);
    
    // Test no H points
    auto res10 = maxHolsteinRect({{0,0,false}});
    assert(res10.first == 0 && res10.second == 0);
    
    cout << "All tests passed!" << endl;
    return 0;
}
// The solution leverages sorting and a two-pointer/sliding window technique over unique x-coordinates. First, sort all points by x, then by y (or type). For each left index i (point with type H) as the left boundary candidate, we consider expanding the right boundary j from i to N-1. For a fixed left boundary x-coordinate (equal to vec[i].x), we maintain counts of H-points per y-coordinate and boolean flags for G-points per y-coordinate. As we extend the right boundary, we add points that share the same x-coordinate together (since a vertical edge can pass through multiple points at the same x). At each unique right x-coordinate, we scan y from 0 to 1000, maintaining a running segment. Whenever we encounter a y with a G-point, we reset the current segment (because a G inside would invalidate the rectangle). Otherwise, if there is at least one H-point at that y, we extend the current segment by adding the count of H’s. We track the best (max H count, minimal area) across all valid vertical strips. Complexity: sorting O(N log N), then for each i (up to N) we iterate j from i to N and scan y-coordinates 0..1000, giving O(N^2 * 1000) worst-case but typically much less due to unique x grouping, with O(1) extra space aside from arrays of size 1001.
