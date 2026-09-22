/*
Given N points on a 2D plane, write a C++ function `long long minimalManhattanDistance(int n, const std::vector<int>& xs, const std::vector<int>& ys)` that returns the minimum possible total Manhattan distance from all points to a single point (x, y), where x and y can be any real numbers (but the optimal choices are always integer coordinates). The function takes the number of points and two vectors containing the x- and y-coordinates respectively (both of length n, with n ≥ 1). The Manhattan distance is defined as |x_i - x| + |y_i - y| for each point i. The function must return the minimal sum of these distances over all choices of x and y.
*/
#include <vector>
#include <algorithm>
#include <cstdlib>

// Given n points with coordinates xs[i] and ys[i], return the minimum total
// Manhattan distance from all points to a single point (x, y).
long long minimalManhattanDistance(int n, const std::vector<int>& xs, const std::vector<int>& ys) {
    // Work on copies to sort without modifying the original data.
    std::vector<int> sorted_xs = xs;
    std::vector<int> sorted_ys = ys;
    std::sort(sorted_xs.begin(), sorted_xs.end());
    std::sort(sorted_ys.begin(), sorted_ys.end());

    // The median index works for both odd and even n (n/2 integer division).
    int median_x = sorted_xs[n / 2];
    int median_y = sorted_ys[n / 2];

    long long total_distance = 0;
    for (int i = 0; i < n; ++i) {
        total_distance += std::llabs(static_cast<long long>(xs[i]) - median_x);
        total_distance += std::llabs(static_cast<long long>(ys[i]) - median_y);
    }
    return total_distance;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include its definition in the same file.

int main() {
    // Example 1: Single point.
    {
        std::vector<int> xs = {0};
        std::vector<int> ys = {0};
        assert(minimalManhattanDistance(1, xs, ys) == 0);
    }

    // Example 2: Two points.
    {
        std::vector<int> xs = {1, 5};
        std::vector<int> ys = {2, 6};
        // Median of xs: 5 (or any between 1 and 5), median of ys: 6 (or between 2 and 6).
        // Minimal sum = |1-1| + |5-1| + |2-2| + |6-2| = 4? No: wait, choose x=5,y=6 works: |1-5|+|5-5|+|2-6|+|6-6|=4+0+4+0=8. Actually choose x=1,y=2 gives 0+4+0+4=8. Any point in rectangle gives sums: |1-x|+|5-x| minimal is 4, |2-y|+|6-y| minimal is 4, total=8.
        assert(minimalManhattanDistance(2, xs, ys) == 8);
    }

    // Example 3: Three points forming a line.
    {
        std::vector<int> xs = {-3, 0, 4};
        std::vector<int> ys = {0, 0, 0};
        // Median of xs is 0, median of ys is 0. Sum = |-3-0|+|0-0|+|4-0| = 3+0+4=7.
        assert(minimalManhattanDistance(3, xs, ys) == 7);
    }

    // Example 4: Duplicate coordinates.
    {
        std::vector<int> xs = {2, 2, 2};
        std::vector<int> ys = {10, 10, 10};
        assert(minimalManhattanDistance(3, xs, ys) == 0);
    }

    // Example 5: Negative coordinates.
    {
        std::vector<int> xs = {-5, -1, -10};
        std::vector<int> ys = {3, -2, 7};
        // xs sorted: -10,-5,-1 median -5; ys sorted: -2,3,7 median 3.
        // Sum = |-5+5|+|-1+5|+|-10+5| + |3-3|+|-2-3|+|7-3| = 0+4+5 + 0+5+4 = 18.
        assert(minimalManhattanDistance(3, xs, ys) == 18);
    }

    // Example 6: Even number of points with mixed coordinates.
    {
        std::vector<int> xs = {0, 10, 20, 30};
        std::vector<int> ys = {0, 0, 0, 0};
        // Median index 2: xs[2]=20, ys[2]=0. Sum = |0-20|+|10-20|+|20-20|+|30-20| = 20+10+0+10=40.
        assert(minimalManhattanDistance(4, xs, ys) == 40);
    }

    // Example 7: Large values to check 64-bit.
    {
        std::vector<int> xs = {1000000, -1000000};
        std::vector<int> ys = {1000000, -1000000};
        // Medians: xs[1] = 1000000, ys[1] = 1000000? Actually sorted xs: -1000000,1000000; index1=1000000. Sum = |-1000000-1000000|+|1000000-1000000| + same for y = 2000000+0+2000000+0=4000000.
        assert(minimalManhattanDistance(2, xs, ys) == 4000000LL);
    }

    // Example 8: Mixed order, unsorted input.
    {
        std::vector<int> xs = {7, 1, 9, 3};
        std::vector<int> ys = {5, 2, 8, 4};
        // xs sorted: 1,3,7,9 median index2=7; ys sorted: 2,4,5,8 median index2=5.
        // Sum = |7-7|+|1-7|+|9-7|+|3-7| + |5-5|+|2-5|+|8-5|+|4-5| = 0+6+2+4 + 0+3+3+1 = 12+7=19.
        assert(minimalManhattanDistance(4, xs, ys) == 19);
    }

    return 0;
}
// The key observation is that the x and y coordinates are independent. The total cost is the sum of costs for the x-dimension: Σ|x_i - x|, plus the sum for the y-dimension: Σ|y_i - y|. Each one-dimensional problem is minimized when the chosen coordinate is a median of the respective coordinate list. For an even number of points, any value between the two middle elements (inclusive) gives the same minimal sum; choosing the element at index n/2 (0-based) works in all cases. For an odd number, the median is unique. Therefore, sort both coordinate arrays, pick the element at index n/2 (integer division) for each, then compute the sum of absolute differences. Edge cases: n=1 gives 0; duplicate coordinates are naturally handled; negative coordinates work fine with abs. Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (ignoring input storage). The answer may exceed 32-bit int, so use a 64-bit integer (long long).
