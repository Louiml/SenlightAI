/*
Write a C++ function `vector<int> testExecutionOrder(int N, int M, const vector<int>& featurePriorities, const vector<vector<int>>& testCaseFeatures)` that determines the execution order of M test cases. Each test case covers a set of feature IDs (1-indexed), and its priority is the sum of the priorities of the features it covers (feature priorities are given as a vector of size N, where element i-1 corresponds to feature i). The execution order sorts test cases by priority in descending order; when priorities are equal, test case IDs (1-indexed) are sorted in ascending order. The function returns a vector containing the test case IDs (1..M) in the correct execution order. Input constraints: 0 < N ≤ 100, 0 < M ≤ 100; feature priorities are positive integers; test case feature lists contain distinct IDs within [1,N]; lists may be empty (priority 0). The solution must not modify the input vectors. The function should be efficient for the given constraints.
*/
#include <vector>
#include <algorithm>
#include <numeric>

// Compute test case execution order based on priority (sum of feature priorities) and ID.
// Feature priorities are 1-indexed; test case IDs are 1-indexed.
std::vector<int> testExecutionOrder(int N, int M,
                                    const std::vector<int>& featurePriorities,
                                    const std::vector<std::vector<int>>& testCaseFeatures) {
    // Store (priority, testCaseID) pairs
    std::vector<std::pair<int, int>> testCases;
    testCases.reserve(M);
    
    for (int i = 0; i < M; ++i) {
        int priority = 0;
        for (int featureID : testCaseFeatures[i]) {
            // featureID is 1-indexed, convert to 0-indexed for featurePriorities
            priority += featurePriorities[featureID - 1];
        }
        testCases.emplace_back(priority, i + 1); // ID is 1-indexed
    }
    
    // Sort by priority descending, then by ID ascending
    std::sort(testCases.begin(), testCases.end(),
              [](const auto& a, const auto& b) {
                  if (a.first != b.first) {
                      return a.first > b.first;
                  }
                  return a.second < b.second;
              });
    
    // Extract IDs in order
    std::vector<int> result;
    result.reserve(M);
    for (const auto& tc : testCases) {
        result.push_back(tc.second);
    }
    return result;
}
#include <cassert>
#include <vector>

// (Solution code would be placed here in a real integration)

int main() {
    // Example 1 from the problem
    {
        int N = 5, M = 4;
        std::vector<int> priorities = {1, 1, 2, 3, 5};
        std::vector<std::vector<int>> features = {
            {1, 2, 3},
            {1, 4},
            {3, 4, 5},
            {2, 3, 4}
        };
        std::vector<int> expected = {3, 4, 1, 2};
        assert(testExecutionOrder(N, M, priorities, features) == expected);
    }
    
    // Example 2: all equal priorities, sorted by ID
    {
        int N = 3, M = 3;
        std::vector<int> priorities = {3, 1, 5};
        std::vector<std::vector<int>> features = {
            {1, 2, 3},
            {1, 2, 3},
            {1, 2, 3}
        };
        std::vector<int> expected = {1, 2, 3};
        assert(testExecutionOrder(N, M, priorities, features) == expected);
    }
    
    // Edge: empty feature list gives priority 0
    {
        int N = 2, M = 2;
        std::vector<int> priorities = {10, 20};
        std::vector<std::vector<int>> features = {
            {},
            {1}
        };
        // T1 priority 0, T2 priority 10 -> T2 first, then T1
        std::vector<int> expected = {2, 1};
        assert(testExecutionOrder(N, M, priorities, features) == expected);
    }
    
    // Edge: single test case
    {
        int N = 1, M = 1;
        std::vector<int> priorities = {7};
        std::vector<std::vector<int>> features = {{1}};
        std::vector<int> expected = {1};
        assert(testExecutionOrder(N, M, priorities, features) == expected);
    }
    
    // Edge: priority ties with different priorities
    {
        int N = 3, M = 3;
        std::vector<int> priorities = {5, 5, 1};
        std::vector<std::vector<int>> features = {
            {1},    // priority 5
            {2},    // priority 5
            {3}     // priority 1
        };
        // T1 and T2 both priority 5, T1 ID smaller, then T3
        std::vector<int> expected = {1, 2, 3};
        assert(testExecutionOrder(N, M, priorities, features) == expected);
    }
    
    return 0;
}
// The core task is to compute a priority for each test case by summing the priorities of the features it covers. Since feature IDs are 1-indexed, map them to indices in `featurePriorities` (which is 0-indexed) by subtracting 1. For each test case i (0-indexed in the input list, but 1-indexed in the output), compute `priority = sum(featurePriorities[featureID-1] for each featureID in testCaseFeatures[i])`. Store the pair (priority, testCaseID). Then sort these pairs using a custom comparator: if priorities differ, sort by priority descending; if equal, sort by testCaseID ascending. Finally, extract the testCaseIDs in sorted order into the result vector.  
//
// **Edge cases:**  
// - Empty feature list → priority 0. Since priorities can be 0, ensure sorting handles zero correctly.  
// - Duplicate features in a test case are not allowed per problem statement, but if they occurred, summing would double-count; we assume input is valid.  
// - N and M can be as small as 1, and feature priorities can be equal.  
//
// **Time complexity:** Let F be the total number of feature references across all test cases. Computing priorities takes O(F) time. Sorting M elements takes O(M log M). Thus total O(F + M log M). Space complexity is O(M) for the vector of pairs and the result vector, plus input storage.
