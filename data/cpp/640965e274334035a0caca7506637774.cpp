// Given a vector of integers representing instruction IDs in a scheduling DAG, write a C++ function that groups adjacent elements into "clusters" of consecutive values, but only when the cluster contains at least 3 elements. The function should return a vector of vectors, where each inner vector contains the original consecutive integers in their original order. Elements that do not belong to any cluster of size 3 or more should be preserved as singleton vectors in their original relative order. For example, given `[1, 2, 3, 5, 6, 7, 8, 10]`, the output should be `[[1, 2, 3], [5, 6, 7, 8], [10]]` because 1-3 and 5-8 form runs of length 3 and 4 respectively, while 10 is a singleton. The input vector may be empty, and all integers are assumed to be unique and strictly increasing (no duplicates, no sorting required).
// The solution requires a single pass over the input vector. We maintain a current run (a vector of integers) and track the last processed value. Start with the first element as the beginning of a run. For each subsequent element, check if it equals the previous value plus one; if so, append it to the current run. If not, the current run ends. At the end of each run, if its length is at least 3, push the entire run as a cluster; otherwise, push each element as a singleton vector. After the loop, flush the final run. Complexity is O(n) in time (each element visited once, and each element is copied exactly once into the result) and O(n) additional space for the result. Edge cases: an empty input returns an empty vector; a single element or two consecutive elements are singletons; the entire input may be one large cluster; and runs can be of any length. No modification of the input is performed, and the function uses const references for efficiency.
#include <vector>

// Group consecutive integers into clusters of size at least 3.
// Preserves order for non-clustered elements as singletons.
std::vector<std::vector<int>> clusterConsecutiveIntegers(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    if (nums.empty()) {
        return result;
    }

    std::vector<int> currentRun;
    currentRun.push_back(nums[0]);

    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] == nums[i - 1] + 1) {
            currentRun.push_back(nums[i]);
        } else {
            // Flush the completed run.
            if (currentRun.size() >= 3) {
                result.push_back(currentRun);
            } else {
                for (int val : currentRun) {
                    result.push_back({val});
                }
            }
            currentRun.clear();
            currentRun.push_back(nums[i]);
        }
    }

    // Flush the final run.
    if (currentRun.size() >= 3) {
        result.push_back(currentRun);
    } else {
        for (int val : currentRun) {
            result.push_back({val});
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Function declaration (assumed from solution).
std::vector<std::vector<int>> clusterConsecutiveIntegers(const std::vector<int>& nums);

int main() {
    // Empty input.
    assert(clusterConsecutiveIntegers({}) == std::vector<std::vector<int>>{});
    
    // Single element -> singleton.
    assert(clusterConsecutiveIntegers({5}) == std::vector<std::vector<int>>{{5}});
    
    // Two consecutive -> two singletons.
    assert(clusterConsecutiveIntegers({3, 4}) == std::vector<std::vector<int>>{{3}, {4}});
    
    // One cluster of three.
    assert(clusterConsecutiveIntegers({1, 2, 3}) == std::vector<std::vector<int>>{{1, 2, 3}});
    
    // One cluster of four.
    assert(clusterConsecutiveIntegers({10, 11, 12, 13}) == std::vector<std::vector<int>>{{10, 11, 12, 13}});
    
    // Mixed: cluster, singleton, cluster.
    assert(clusterConsecutiveIntegers({1, 2, 3, 5, 6, 7, 8, 10}) ==
           std::vector<std::vector<int>>({{1, 2, 3}, {5, 6, 7, 8}, {10}}));
    
    // All singletons (no run of 3).
    assert(clusterConsecutiveIntegers({1, 3, 5}) == std::vector<std::vector<int>>{{1}, {3}, {5}});
    
    // Leading small run then large run.
    assert(clusterConsecutiveIntegers({100, 101, 200, 201, 202, 203}) ==
           std::vector<std::vector<int>>({{100}, {101}, {200, 201, 202, 203}}));
    
    // Run of exactly 3 at start and end.
    assert(clusterConsecutiveIntegers({3, 4, 5, 9, 10, 11}) ==
           std::vector<std::vector<int>>({{3, 4, 5}, {9, 10, 11}}));
    
    return 0;
}
