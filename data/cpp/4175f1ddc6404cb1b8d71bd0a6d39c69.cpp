// Given an odd number of points on a 2D plane represented by their integer x and y coordinates, where all but one point appear an exact even number of times, write a C++ function `std::pair<long long, long long> findMissingPoint(const std::vector<std::pair<long long, long long>>& points)` that returns the coordinates of the point that appears an odd number of times. The input vector contains `4*size - 1` points (where `size` is a positive integer), meaning exactly one coordinate value appears an odd count among the x-coordinates, and exactly one coordinate value appears an odd count among the y-coordinates. The function should determine and return that missing point's x and y coordinates. You may assume the input is always valid (i.e., there is exactly one odd-occurring x and one odd-occurring y coordinate). The coordinates are within the range of a 64-bit signed integer, and the vector size can be up to 10^5 points.
The key observation is that for each axis independently, all coordinates appear in pairs except for the missing point. Since pairs cancel out when considering parity, we can use the XOR operation to find the odd-occurring coordinate on each axis. XOR is associative, commutative, and `x ^ x = 0`, so XORing all x-coordinates together will cancel all paired values and leave only the odd-occurring x value. The same applies to the y-coordinates. The algorithm simply iterates once through the input vector, XORing all x values into one variable and all y values into another. After the loop, these two variables contain the missing point's coordinates. This works with negative numbers because XOR is bitwise and independent of sign interpretation. Edge cases include an empty vector (though the problem guarantees at least one point) and very large coordinate values; since we use `long long`, overflow does not occur with XOR. Time complexity is O(n) where n is the number of points, and space complexity is O(1) besides the input vector.
#include <vector>
#include <utility>

// Given a vector of points where every coordinate pair appears an even number of times except for one point,
// return that point's coordinates (the one appearing an odd number of times).
std::pair<long long, long long> findMissingPoint(const std::vector<std::pair<long long, long long>>& points) {
    long long xXor = 0;
    long long yXor = 0;

    for (const auto& p : points) {
        xXor ^= p.first;
        yXor ^= p.second;
    }

    return {xXor, yXor};
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above in the Solution section.
// This main function tests the function.
int main() {
    // Basic case with size = 1, missing point (5,5)
    std::vector<std::pair<long long, long long>> points1 = {{1,2}, {3,4}, {5,5}};
    std::pair<long long, long long> result1 = findMissingPoint(points1);
    assert(result1.first == 5 && result1.second == 5);

    // Case with multiple pairs and negative coordinates
    std::vector<std::pair<long long, long long>> points2 = {{-1,0}, {2,3}, {-1,0}, {2,4}, {7,7}};
    // Odd x is 7, odd y is 7 (because 0 and 3/4 appear even? Let's check:
    // x: -1,2,-1,2,7 -> XOR = (-1^2^-1^2^7) = 7
    // y: 0,3,0,4,7 -> XOR = (0^3^0^4^7) = 0^3^0^4^7 = 3^4^7 = 7^7 = 0? wait compute: 0^3=3, 3^0=3, 3^4=7, 7^7=0 => So y odd is 0? This is wrong because we need exactly one odd y. Let's adjust.
    // Better test: ensure exactly one odd on each axis.
    
    // Constructing a valid test: all pairs except missing.
    // Let's make missing point (10,20), and pairs: (1,2) twice, (3,4) twice, so total 1+4=5 points? Actually 4*size -1 with size=1 gives 3 points. So for size=1, only 3 points: one missing and one pair? Wait if size=1, 4*1-1=3. So 3 points means one pair and one odd. So my first test is fine. For larger test, use size=2 -> 7 points: one odd and three pairs. Let's do that.
    // Three pairs: (1,2), (3,4), (5,6), each appears twice. Missing (7,8). Total 7 points.
    std::vector<std::pair<long long, long long>> points2_valid = {{1,2},{3,4},{5,6},{1,2},{3,4},{5,6},{7,8}};
    std::pair<long long, long long> result2 = findMissingPoint(points2_valid);
    assert(result2.first == 7 && result2.second == 8);

    // Test with negative coordinates: pairs: (-1,0) twice, (2,-3) twice, missing (-5, -5)
    std::vector<std::pair<long long, long long>> points3 = {{-1,0},{2,-3},{-1,0},{2,-3},{-5,-5}};
    std::pair<long long, long long> result3 = findMissingPoint(points3);
    assert(result3.first == -5 && result3.second == -5);

    // Large values within long long range
    long long big = 1LL << 60;
    std::vector<std::pair<long long, long long>> points4 = {{big, 0}, {big, 0}, {42, big}, {42, big}, {99, 99}};
    // XOR of x: big^big^42^42^99 = 99; XOR of y: 0^0^big^big^99 = 99
    std::pair<long long, long long> result4 = findMissingPoint(points4);
    assert(result4.first == 99 && result4.second == 99);

    // Single point only (size should be 1? Actually 4*1-1=3, so not possible. But we can still test odd vector of size 1 (invalid by spec but function works)
    std::vector<std::pair<long long, long long>> points5 = {{0,0}};
    std::pair<long long, long long> result5 = findMissingPoint(points5);
    assert(result5.first == 0 && result5.second == 0);

    return 0;
}
