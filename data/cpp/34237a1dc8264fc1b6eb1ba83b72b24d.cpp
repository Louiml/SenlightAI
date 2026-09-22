/*
Write a standalone C++ function named `processPairs` that takes a vector of integer pairs and performs the following operations inspired by the snippet: for each pair, it calculates the sum of the two integers, tracks the pair with the maximum second component, and counts how many pairs have a first component equal to their second component. The function should return a `std::string` formatted exactly as: `"MaxSecond:<value> EqualCount:<count> Sums:<sum1>,<sum2>,..."` where sums are listed in the order of the input pairs, and the max second value and count are integers. The input vector will contain at least one pair, and all integers are within the range of a 32-bit signed int. The output string must not have trailing spaces or commas.
*/

#include <string>
#include <vector>
#include <sstream>
#include <climits>

// Process a vector of integer pairs and return a formatted summary string.
std::string processPairs(const std::vector<std::pair<int,int>>& pairs) {
    int maxSecond = INT_MIN;
    int equalCount = 0;
    std::vector<int> sums;
    sums.reserve(pairs.size());

    for (const auto& p : pairs) {
        // Track maximum second component
        if (p.second > maxSecond) {
            maxSecond = p.second;
        }
        // Count equal pairs
        if (p.first == p.second) {
            ++equalCount;
        }
        // Record sum
        sums.push_back(p.first + p.second);
    }

    // Build the output string
    std::ostringstream oss;
    oss << "MaxSecond:" << maxSecond
        << " EqualCount:" << equalCount
        << " Sums:";
    for (size_t i = 0; i < sums.size(); ++i) {
        if (i > 0) oss << ",";
        oss << sums[i];
    }
    return oss.str();
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>

int main() {
    // Basic test with mixed pairs
    std::vector<std::pair<int,int>> v1 = {{1,2}, {3,3}, {5,1}};
    assert(processPairs(v1) == "MaxSecond:3 EqualCount:1 Sums:3,6,6");

    // Single pair
    std::vector<std::pair<int,int>> v2 = {{-4, -4}};
    assert(processPairs(v2) == "MaxSecond:-4 EqualCount:1 Sums:-8");

    // No equal pairs
    std::vector<std::pair<int,int>> v3 = {{1,10}, {2,20}, {3,30}};
    assert(processPairs(v3) == "MaxSecond:30 EqualCount:0 Sums:11,22,33");

    // Negative values and ties for max second
    std::vector<std::pair<int,int>> v4 = {{-5, -1}, {-2, -1}, {0, 5}};
    assert(processPairs(v4) == "MaxSecond:5 EqualCount:0 Sums:-6,-3,5");

    // All equal pairs
    std::vector<std::pair<int,int>> v5 = {{7,7}, {7,7}};
    assert(processPairs(v5) == "MaxSecond:7 EqualCount:2 Sums:14,14");

    // Large numbers (within int range)
    std::vector<std::pair<int,int>> v6 = {{100000, 200000}, {300000, 400000}};
    assert(processPairs(v6) == "MaxSecond:400000 EqualCount:0 Sums:300000,700000");

    // Max second occurs in first element
    std::vector<std::pair<int,int>> v7 = {{1,100}, {2,3}};
    assert(processPairs(v7) == "MaxSecond:100 EqualCount:0 Sums:101,5");

    // Zero and negative mixed
    std::vector<std::pair<int,int>> v8 = {{0,0}, {-1,-1}, {2,3}};
    assert(processPairs(v8) == "MaxSecond:3 EqualCount:2 Sums:0,-2,5");

    // Single pair with unequal values
    std::vector<std::pair<int,int>> v9 = {{5, 10}};
    assert(processPairs(v9) == "MaxSecond:10 EqualCount:0 Sums:15");

    // Sums can be negative
    std::vector<std::pair<int,int>> v10 = {{-10, -20}, {-30, -40}};
    assert(processPairs(v10) == "MaxSecond:-20 EqualCount:0 Sums:-30,-70");

    return 0;
}

// The solution approach is straightforward: iterate through the vector once, maintaining three pieces of state. First, update a maximum variable holding the largest second component encountered so far. Second, increment a counter whenever the two components of a pair are equal. Third, accumulate the sum of each pair into a vector of integers. After the loop, format these results into a string using a `std::ostringstream` for efficient concatenation. Edge cases: when only one pair exists, the maximum is that pair’s second value, the equal count is either 0 or 1, and the sums list has exactly one element. No special handling is needed for zero or negative values. Time complexity is O(n) for n pairs, and space complexity is O(n) for storing the sum list (which is necessary to produce the output). The function should be `const`-correct by taking the vector by `const&`, and should not modify the input.
