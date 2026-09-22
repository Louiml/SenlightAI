// Write a C++ function `bool hasQuadrupleSum(const std::vector<int>& numbers, int target)` that determines whether there exist four indices `a, b, c, d` (not necessarily distinct) such that `numbers[a] + numbers[b] + numbers[c] + numbers[d] == target`. The function should return `true` if such a combination exists, and `false` otherwise. The input vector may contain up to 100 integers, and values may be negative, zero, or positive. The original code used a naive quadruple loop, but your solution should be efficient enough for the given constraints.

The task asks to check if any four values (allowing repetition of the same index) sum to a target. A naive O(n^4) approach works for very small n, but the typical constraint (up to 100) makes it too slow (100^4 = 10^8, borderline but possible, but better to optimize). A common approach is to reduce the problem: first compute all possible sums of two elements (including same index twice), storing them in a hash set (or sorted vector for two-pointer). Then for each pair sum `s`, check if `target - s` exists in the set. Since indices can repeat, we must be careful: a pair sum that uses the same two indices as another pair is allowed, because the four indices in the original sum can all be the same or repeat. So we can simply compute all n^2 pair sums (including duplicates) and store them in a set for O(1) lookup. Then iterate over all pair sums again and check the complement. This yields O(n^2) time and O(n^2) space. Edge cases: n=0 or n<4? Even if n<4, we can still use repetition (e.g., n=1, use the same element four times), so the algorithm works for any n ≥ 1 because we allow repeated indices. For n=0 (empty vector), we should return `false` because we cannot pick any indices. The algorithm handles that naturally since no pair sums exist. Complexity: O(n^2) time and O(n^2) space.

#include <vector>
#include <unordered_set>

// Check if there exist four indices (possibly repeated) whose values sum to target.
bool hasQuadrupleSum(const std::vector<int>& numbers, int target) {
    const int n = static_cast<int>(numbers.size());
    if (n == 0) {
        return false;
    }

    std::unordered_set<int> pairSums;
    pairSums.reserve(n * n);

    // Compute all possible sums of two values (indices can be same).
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pairSums.insert(numbers[i] + numbers[j]);
        }
    }

    // For each pair sum, check if the complementary sum exists.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int currentSum = numbers[i] + numbers[j];
            int needed = target - currentSum;
            if (pairSums.find(needed) != pairSums.end()) {
                return true;
            }
        }
    }

    return false;
}

#include <cassert>
#include <vector>

// Assuming the function above is declared before main.
// We'll include the header or just define it inline for testing.

int main() {
    // Basic true case
    std::vector<int> v1 = {1, 2, 3, 4};
    assert(hasQuadrupleSum(v1, 10) == true); // 1+2+3+4=10

    // Basic false case
    std::vector<int> v2 = {1, 2, 3, 4};
    assert(hasQuadrupleSum(v2, 100) == false);

    // Using same index repeatedly
    std::vector<int> v3 = {5};
    assert(hasQuadrupleSum(v3, 20) == true); // 5+5+5+5=20

    // Single element cannot make a sum other than 4*it
    std::vector<int> v4 = {5};
    assert(hasQuadrupleSum(v4, 19) == false);

    // Negative numbers
    std::vector<int> v5 = {-1, 2, -3, 4};
    assert(hasQuadrupleSum(v5, 0) == true); // -1 + -3 + 2 + 2? Actually -1+2-3+4=2, but find combination: -3+2+? Let's test with target 2: -1+2+? Better: -1+2+? Actually 2+2-1-1 =2 but indices can repeat. So assert true for target 2.
    assert(hasQuadrupleSum(v5, 2) == true);
    assert(hasQuadrupleSum(v5, 100) == false);

    // Empty vector
    std::vector<int> v6;
    assert(hasQuadrupleSum(v6, 0) == false);

    // Large vector with duplicate values
    std::vector<int> v7(100, 1);
    assert(hasQuadrupleSum(v7, 4) == true); // 1+1+1+1=4
    assert(hasQuadrupleSum(v7, 5) == false);

    // Mixed values where target is achieved by repeated index combinations
    std::vector<int> v8 = {2, 3};
    assert(hasQuadrupleSum(v8, 10) == true); // 2+2+2+3? actually 2+2+3+3=10
    assert(hasQuadrupleSum(v8, 11) == true); // 2+3+3+3=11

    return 0;
}
