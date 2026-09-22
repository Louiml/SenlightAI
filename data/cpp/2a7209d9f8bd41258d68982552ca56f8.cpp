/*
Write a C++ function `double maxDriveRatio(const std::vector<double>& frontGears, const std::vector<double>& rearGears)` that, given two non-empty vectors of positive gear sizes (front sprockets and rear sprockets), computes the maximum ratio between any two consecutive values when all possible gear ratios `rear / front` are sorted in ascending order. The function should return this maximum ratio. If there is only one gear ratio (i.e., product of sizes equals 1), the function should return `0.0`. The function must handle inputs where the number of front and rear gears may differ and where gear sizes can be non-integer double values.
*/

#include <vector>
#include <algorithm>
#include <stdexcept>

// Compute the maximum ratio between any two consecutive sorted gear ratios.
// @param frontGears: front sprocket sizes (positive doubles)
// @param rearGears: rear sprocket sizes (positive doubles)
// @return: maximum consecutive ratio, or 0.0 if fewer than two ratios exist.
// @throws: std::invalid_argument if either vector is empty.
double maxDriveRatio(const std::vector<double>& frontGears, const std::vector<double>& rearGears) {
    if (frontGears.empty() || rearGears.empty()) {
        throw std::invalid_argument("Gear vectors must not be empty");
    }
    
    std::vector<double> ratios;
    ratios.reserve(frontGears.size() * rearGears.size());
    
    // Compute all possible ratios: rear / front
    for (double rear : rearGears) {
        for (double front : frontGears) {
            ratios.push_back(rear / front);
        }
    }
    
    if (ratios.size() < 2) {
        return 0.0;
    }
    
    std::sort(ratios.begin(), ratios.end());
    
    double maxRatio = 0.0;
    for (size_t i = 1; i < ratios.size(); ++i) {
        double consecutiveRatio = ratios[i] / ratios[i - 1];
        if (consecutiveRatio > maxRatio) {
            maxRatio = consecutiveRatio;
        }
    }
    return maxRatio;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the solution function (already defined above)
double maxDriveRatio(const std::vector<double>& frontGears, const std::vector<double>& rearGears);

int main() {
    // Example from the snippet: front [10, 20, 30], rear [1, 2] -> ratios [0.033, 0.05, 0.0667, 0.1, 0.2, 0.0667]? Let's test actual computation.
    // Ratios sorted: 1/30=0.0333, 1/20=0.05, 1/10=0.1, 2/30=0.0667, 2/20=0.1, 2/10=0.2 → sorted: 0.0333, 0.05, 0.0667, 0.1, 0.1, 0.2
    // Max consecutive: 0.2/0.1=2.0, 0.1/0.0667=1.5, 0.0667/0.05=1.333, 0.05/0.0333=1.5 → max is 2.0
    std::vector<double> front{10, 20, 30};
    std::vector<double> rear{1, 2};
    assert(std::fabs(maxDriveRatio(front, rear) - 2.0) < 1e-9);
    
    // Single gear each: only one ratio, return 0.0
    assert(maxDriveRatio({5.0}, {10.0}) == 0.0);
    
    // Simple case: ratios are [1, 2] -> max consecutive ratio = 2
    assert(std::fabs(maxDriveRatio({2.0}, {2.0, 4.0}) - 2.0) < 1e-9);
    
    // Duplicate ratios: front {1,2}, rear {2,4} -> ratios: 2,4,1,2 → sorted [1,2,2,4] → consecutive: 2,1,2 → max=2
    std::vector<double> f2{1.0, 2.0};
    std::vector<double> r2{2.0, 4.0};
    assert(std::fabs(maxDriveRatio(f2, r2) - 2.0) < 1e-9);
    
    // All ratios equal: front {1,2}, rear {2,4} gives ratios 2,4,1,2 – not all equal. Use front {1}, rear {5} → only one ratio, returns 0.0 already tested.
    // Test with two ratios equal: front {1}, rear {2,2} → ratios 2,2 → sorted [2,2] → consecutive ratio = 1.0
    assert(std::fabs(maxDriveRatio({1.0}, {2.0, 2.0}) - 1.0) < 1e-9);
    
    // Larger example: front {1,2,4}, rear {1,3} → ratios: 1,3, 0.5,1.5, 0.25,0.75 → sorted: 0.25,0.5,0.75,1,1.5,3 → consecutive max = 3/1.5=2.0, also 1.5/1=1.5, 1/0.75=1.333, etc. Max is 2.0
    std::vector<double> f3{1.0, 2.0, 4.0};
    std::vector<double> r3{1.0, 3.0};
    assert(std::fabs(maxDriveRatio(f3, r3) - 2.0) < 1e-9);
    
    return 0;
}

// The problem is a direct adaptation of the provided snippet. The algorithm computes all `m * n` ratios by iterating over every rear gear and every front gear, storing them in a vector. After sorting the vector in ascending order, the maximum ratio between consecutive elements is found by iterating from index 1 to the end and taking `max(current / previous)`. Important edge cases: (1) If the total count of ratios is less than 2, there are no consecutive pairs, so return `0.0` as a sentinel (the original snippet would have undefined behavior because maxD stays -1, but we improve it). (2) All ratios are positive since gear sizes are positive, so division is safe without zero division. (3) The input vectors may be empty, but the task guarantees non-empty input. Time complexity is \(O(n \cdot m \log(n \cdot m))\) due to sorting, and space complexity is \(O(n \cdot m)\) for storing ratios.
