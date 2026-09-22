Write a C++ function that takes a vector of employee records, where each record contains a name (std::string) and a salary (int), along with a minimum salary threshold. The function should return a new vector containing only the employees whose salary is strictly greater than the threshold, sorted first by descending salary, and for employees with equal salary, sorted alphabetically by name in ascending order. The original vector must remain unmodified. Assume all names are non-empty and contain only lowercase letters, and all salaries are positive integers. If no employee meets the threshold, return an empty vector.

The solution involves three main steps: filtering, sorting, and returning the result. First, iterate through the input vector and copy only elements whose salary is greater than the given threshold into a new vector. Second, sort this filtered vector using a custom comparator: if two salaries are equal, compare names lexicographically (ascending order); otherwise, compare salaries in descending order. This can be implemented using `std::sort` with a lambda or a free function. Edge cases include an empty input vector (returns empty vector), all salaries at or below the threshold (returns empty vector), and duplicate names/salaries (sort handles them naturally; duplicates with same salary and name are indistinguishable but remain in the vector). Time complexity is O(n log n) due to sorting, where n is the number of employees whose salary exceeds the threshold (worst case all n). Space complexity is O(n) for the result vector, excluding the input storage.

#include <vector>
#include <string>
#include <algorithm>

// Sort by descending salary, then ascending name.
bool compareEmployees(const std::pair<std::string, int>& a, 
                      const std::pair<std::string, int>& b) {
    if (a.second == b.second) {
        return a.first < b.first;
    }
    return a.second > b.second;
}

// Return employees with salary > threshold, sorted by salary descending, then name ascending.
std::vector<std::pair<std::string, int>> filterAndSortEmployees(
    const std::vector<std::pair<std::string, int>>& employees,
    int threshold) {
    std::vector<std::pair<std::string, int>> result;
    
    // Filter: keep only salaries strictly greater than threshold.
    for (const auto& emp : employees) {
        if (emp.second > threshold) {
            result.push_back(emp);
        }
    }
    
    // Sort the filtered result using the custom comparator.
    std::sort(result.begin(), result.end(), compareEmployees);
    
    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Function declaration (provided from solution).
std::vector<std::pair<std::string, int>> filterAndSortEmployees(
    const std::vector<std::pair<std::string, int>>& employees,
    int threshold);

int main() {
    // Test 1: Basic filtering and sorting.
    std::vector<std::pair<std::string, int>> employees1 = {
        {"alice", 5000}, {"bob", 7000}, {"carol", 5000}, {"dave", 3000}
    };
    std::vector<std::pair<std::string, int>> result1 = filterAndSortEmployees(employees1, 4000);
    std::vector<std::pair<std::string, int>> expected1 = {
        {"bob", 7000}, {"alice", 5000}, {"carol", 5000}
    };
    assert(result1 == expected1);

    // Test 2: All employees below or equal threshold → empty vector.
    std::vector<std::pair<std::string, int>> employees2 = {
        {"x", 100}, {"y", 200}
    };
    std::vector<std::pair<std::string, int>> result2 = filterAndSortEmployees(employees2, 200);
    assert(result2.empty());

    // Test 3: Empty input → empty vector.
    std::vector<std::pair<std::string, int>> employees3;
    std::vector<std::pair<std::string, int>> result3 = filterAndSortEmployees(employees3, 0);
    assert(result3.empty());

    // Test 4: Equal salaries sorted by name ascending.
    std::vector<std::pair<std::string, int>> employees4 = {
        {"charlie", 1000}, {"alpha", 1000}, {"bravo", 1000}
    };
    std::vector<std::pair<std::string, int>> result4 = filterAndSortEmployees(employees4, 500);
    std::vector<std::pair<std::string, int>> expected4 = {
        {"alpha", 1000}, {"bravo", 1000}, {"charlie", 1000}
    };
    assert(result4 == expected4);

    // Test 5: All salaries equal to threshold → empty.
    std::vector<std::pair<std::string, int>> employees5 = {
        {"a", 500}, {"b", 500}
    };
    std::vector<std::pair<std::string, int>> result5 = filterAndSortEmployees(employees5, 500);
    assert(result5.empty());

    // Test 6: Only one employee passes threshold, others below.
    std::vector<std::pair<std::string, int>> employees6 = {
        {"low", 10}, {"high", 100}, {"mid", 50}
    };
    std::vector<std::pair<std::string, int>> result6 = filterAndSortEmployees(employees6, 60);
    std::vector<std::pair<std::string, int>> expected6 = {{"high", 100}};
    assert(result6 == expected6);

    // Test 7: Large salaries and mixed order, verify original unchanged.
    std::vector<std::pair<std::string, int>> employees7 = {
        {"c", 3000}, {"a", 9000}, {"b", 3000}
    };
    std::vector<std::pair<std::string, int>> original7 = employees7;
    std::vector<std::pair<std::string, int>> result7 = filterAndSortEmployees(employees7, 2000);
    assert(employees7 == original7); // Input not modified.
    std::vector<std::pair<std::string, int>> expected7 = {
        {"a", 9000}, {"b", 3000}, {"c", 3000}
    };
    assert(result7 == expected7);

    return 0;
}
