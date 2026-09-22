Write a C++ function `computeGroupEnergies` that takes a vector of `double` values representing subband signal powers and a vector of group border indices, and returns a vector of `double` containing the total energy (sum of values) for each group. The function should handle the case where the input vector may be empty, in which case it returns an empty vector. The group borders are given as a vector of indices where group `i` covers elements from `borders[i]` to `borders[i+1] - 1` inclusive, with the last border equal to the size of the input vector. Assume borders are valid (non-decreasing, start at 0, end at size). The function should copy the group energy values into the output vector in order.
The solution iterates through each group defined by consecutive pairs of border indices. For each group, it sums the element values in the range `[borders[i], borders[i+1])` (since the last border equals the vector size, this covers all elements). The main edge case is an empty input vector, which should return an empty result without dereferencing invalid memory. Another edge case is when borders might have only one element (indicating no groups), which should also return an empty vector. The time complexity is O(n + m) where n is the size of the input vector and m is the number of groups, because each element is visited exactly once across all groups. The space complexity is O(m) for storing the result, excluding the input and output vectors themselves.
#include <vector>

// Compute the sum of values in each group defined by border indices.
std::vector<double> computeGroupEnergies(const std::vector<double>& values,
                                         const std::vector<size_t>& borders) {
    std::vector<double> groupEnergies;
    
    // No groups if the values vector is empty or borders has fewer than 2 entries.
    if (values.empty() || borders.size() < 2) {
        return groupEnergies;
    }
    
    // For each group defined by borders[i] to borders[i+1]-1
    for (size_t group = 0; group + 1 < borders.size(); ++group) {
        double sum = 0.0;
        for (size_t idx = borders[group]; idx < borders[group + 1]; ++idx) {
            sum += values[idx];
        }
        groupEnergies.push_back(sum);
    }
    
    return groupEnergies;
}
#include <cassert>
#include <vector>

int main() {
    // Basic grouping
    {
        std::vector<double> values = {1.0, 2.0, 3.0, 4.0};
        std::vector<size_t> borders = {0, 2, 4};
        std::vector<double> result = computeGroupEnergies(values, borders);
        assert(result.size() == 2);
        assert(result[0] == 3.0);
        assert(result[1] == 7.0);
    }
    
    // Single group covering all values
    {
        std::vector<double> values = {5.0, 5.0, 5.0};
        std::vector<size_t> borders = {0, 3};
        std::vector<double> result = computeGroupEnergies(values, borders);
        assert(result.size() == 1);
        assert(result[0] == 15.0);
    }
    
    // Empty values vector
    {
        std::vector<double> values;
        std::vector<size_t> borders = {0, 0};
        std::vector<double> result = computeGroupEnergies(values, borders);
        assert(result.empty());
    }
    
    // Borders with a single entry (no groups)
    {
        std::vector<double> values = {1.0, 2.0};
        std::vector<size_t> borders = {0};
        std::vector<double> result = computeGroupEnergies(values, borders);
        assert(result.empty());
    }
    
    // Multiple groups with one element each
    {
        std::vector<double> values = {1.0, 2.0, 3.0};
        std::vector<size_t> borders = {0, 1, 2, 3};
        std::vector<double> result = computeGroupEnergies(values, borders);
        assert(result.size() == 3);
        assert(result[0] == 1.0);
        assert(result[1] == 2.0);
        assert(result[2] == 3.0);
    }
    
    // Negative and zero values
    {
        std::vector<double> values = {-1.0, 0.0, -2.0, 3.0};
        std::vector<size_t> borders = {0, 2, 4};
        std::vector<double> result = computeGroupEnergies(values, borders);
        assert(result.size() == 2);
        assert(result[0] == -1.0);  // -1.0 + 0.0
        assert(result[1] == 1.0);   // -2.0 + 3.0
    }
    
    return 0;
}
