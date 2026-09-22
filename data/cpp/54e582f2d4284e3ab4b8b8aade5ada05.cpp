Implement a C++ function `adjustAndReport` that takes a reference to a `std::vector<float>` representing average commute times (in minutes) and another `std::vector<float>` representing fuel efficiencies (in miles per gallon), both of the same size. The function must apply the following operations in sequence: first increase the time at index 4 by 0.6 and at index 2 by 0.9; then decrease the time at index 6 by 0.6 and at index 1 by 0.9; then decrease the fuel at index 3 by 0.6 and at index 5 by 0.9. After each group of operations (time adjustments, then fuel adjustments), the function must output the current time array and fuel array, each on separate lines, with values space-separated. Finally, the function must compute and return the count of elements whose index is strictly before the element with value 5 (i.e., indices of elements less than the index of the first occurrence of 5), and also the count of elements strictly after the element with value 5 (i.e., indices greater than the index of the first occurrence of 5). If the value 5 does not exist in the time array, return -1 for both counts. The function must handle arrays of any positive size, apply the operations only if the indices are valid (i.e., within bounds), and output the arrays after each phase.

The solution begins by verifying that both input vectors are non-empty and of equal size; if not, the function should return immediately (or handle error). For each operation, check that the target index is within `0` to `vec.size()-1` before applying the modification. After the time adjustments, print the time vector followed by the fuel vector (each space-separated on its own line). Then apply the fuel adjustments, and again print both vectors. To compute the counts relative to the element with value 5, first find the index of the first occurrence of 5 in the time vector using `std::find`. If not found, return `{-1, -1}`. Otherwise, the count before is simply the index `pos` (since indices 0..pos-1 are before), and the count after is `size - pos - 1`. Return these as a `std::pair<int,int>`. Time complexity is O(n) for finding the element and O(1) for adjustments (if we assume index checks are constant), with O(n) space for printing (if output is considered). Edge cases include arrays smaller than index 7 or 5, missing 5, and single-element arrays.

#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>

// Applies time and fuel adjustments, prints arrays after each phase, and returns counts before/after element 5.
std::pair<int, int> adjustAndReport(std::vector<float>& times, std::vector<float>& fuels) {
    if (times.empty() || times.size() != fuels.size()) {
        return {-1, -1};
    }
    const size_t n = times.size();
    
    // Time adjustments
    if (4 < n) times[4] += 0.6f;
    if (2 < n) times[2] += 0.9f;
    if (6 < n) times[6] -= 0.6f;
    if (1 < n) times[1] -= 0.9f;
    
    // First output: print both arrays
    for (size_t i = 0; i < n; ++i) std::cout << times[i] << " ";
    std::cout << "\n";
    for (size_t i = 0; i < n; ++i) std::cout << fuels[i] << " ";
    std::cout << "\n";
    
    // Fuel adjustments
    if (3 < n) fuels[3] -= 0.6f;
    if (5 < n) fuels[5] -= 0.9f;
    
    // Second output: print both arrays again
    for (size_t i = 0; i < n; ++i) std::cout << times[i] << " ";
    std::cout << "\n";
    for (size_t i = 0; i < n; ++i) std::cout << fuels[i] << " ";
    std::cout << "\n";
    
    // Find index of value 5 in times
    auto it = std::find(times.begin(), times.end(), 5.0f);
    if (it == times.end()) {
        return {-1, -1};
    }
    size_t pos = std::distance(times.begin(), it);
    return {static_cast<int>(pos), static_cast<int>(n - pos - 1)};
}

#include <cassert>
#include <vector>
#include <sstream>
#include <iostream>

// Declare the function (in the same translation unit)
std::pair<int, int> adjustAndReport(std::vector<float>&, std::vector<float>&);

int main() {
    // Test 1: Example from problem
    {
        std::vector<float> times = {93, 96, 97, 95, 100, 99, 101, 92, 98, 102};
        std::vector<float> fuels = {194, 186, 156, 193, 120, 138, 113, 200, 148, 102};
        std::ostringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        auto result = adjustAndReport(times, fuels);
        std::cout.rdbuf(old);
        // Verify adjustments on times: index4 +0.6 => 100.6, index2 +0.9 => 97.9, index6 -0.6 => 100.4, index1 -0.9 => 95.1
        assert(fabs(times[4] - 100.6f) < 1e-5);
        assert(fabs(times[2] - 97.9f) < 1e-5);
        assert(fabs(times[6] - 100.4f) < 1e-5);
        assert(fabs(times[1] - 95.1f) < 1e-5);
        // Fuel: index3 -0.6 => 192.4, index5 -0.9 => 137.1
        assert(fabs(fuels[3] - 192.4f) < 1e-5);
        assert(fabs(fuels[5] - 137.1f) < 1e-5);
        // No 5 in times, so counts should be -1
        assert(result.first == -1 && result.second == -1);
    }
    
    // Test 2: Contains 5 at index 2
    {
        std::vector<float> times = {10, 20, 5, 30, 40};
        std::vector<float> fuels = {1, 2, 3, 4, 5};
        std::ostringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        auto result = adjustAndReport(times, fuels);
        std::cout.rdbuf(old);
        // Index of 5 is 2, before=2, after=5-2-1=2
        assert(result.first == 2 && result.second == 2);
        // Adjusted times: index4 doesn't exist (only 5 elements, index4 exists), index1 +0.9 => 20.9, etc. But not necessary for counts.
    }
    
    // Test 3: 5 at first position
    {
        std::vector<float> times = {5, 3, 7};
        std::vector<float> fuels = {1, 1, 1};
        std::ostringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        auto result = adjustAndReport(times, fuels);
        std::cout.rdbuf(old);
        assert(result.first == 0 && result.second == 2);
    }
    
    // Test 4: 5 at last position
    {
        std::vector<float> times = {1, 2, 3, 4, 5};
        std::vector<float> fuels = {1, 2, 3, 4, 5};
        std::ostringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        auto result = adjustAndReport(times, fuels);
        std::cout.rdbuf(old);
        assert(result.first == 4 && result.second == 0);
    }
    
    // Test 5: Empty vectors
    {
        std::vector<float> times;
        std::vector<float> fuels;
        auto result = adjustAndReport(times, fuels);
        assert(result.first == -1 && result.second == -1);
    }
    
    return 0;
}
