Write a C++ function `printCombinations` that takes a vector of distinct integers and an integer `r`, and returns a vector of vectors, where each inner vector is a unique combination of `r` elements from the input, preserving the original order of elements within each combination. Combinations should be generated in lexicographic (increasing) order based on the input order. If `r` is greater than the size of the input, or if `r` is less than or equal to zero, return an empty vector. The input vector is guaranteed to contain no duplicates.
#include <cassert>
#include <vector>

// Including the solution function here for testing purposes.
// (In an actual separate compilation, you'd include the header.)
std::vector<std::vector<int>> printCombinations(const std::vector<int>& arr, int r) { /* ... */ }

int main() {
    // Test 1: Basic case with 5 elements, r=3.
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    auto res1 = printCombinations(arr1, 3);
    assert(res1.size() == 10);
    assert(res1[0] == std::vector<int>({1, 2, 3}));
    assert(res1[9] == std::vector<int>({3, 4, 5}));

    // Test 2: r=1 gives single-element combinations.
    auto res2 = printCombinations(arr1, 1);
    assert(res2.size() == 5);
    assert(res2 == std::vector<std::vector<int>>({{1}, {2}, {3}, {4}, {5}}));

    // Test 3: r=n gives only the full combination.
    auto res3 = printCombinations(arr1, 5);
    assert(res3.size() == 1);
    assert(res3[0] == arr1);

    // Test 4: r greater than n returns empty.
    auto res4 = printCombinations(arr1, 6);
    assert(res4.empty());

    // Test 5: r = 0 returns empty.
    auto res5 = printCombinations(arr1, 0);
    assert(res5.empty());

    // Test 6: r negative returns empty.
    auto res6 = printCombinations(arr1, -2);
    assert(res6.empty());

    // Test 7: Empty input with any valid r returns empty.
    auto res7 = printCombinations(std::vector<int>{}, 1);
    assert(res7.empty());

    // Test 8: Two elements, r=2.
    auto res8 = printCombinations(std::vector<int>{7, 9}, 2);
    assert(res8.size() == 1);
    assert(res8[0] == std::vector<int>({7, 9}));

    return 0;
}
#include <vector>

// Return all combinations of size r from the input vector, preserving input order.
std::vector<std::vector<int>> printCombinations(const std::vector<int>& arr, int r) {
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(arr.size());
    
    // Invalid r: return empty result.
    if (r <= 0 || r > n) {
        return result;
    }
    
    std::vector<int> current;
    
    // Backtracking helper to build combinations.
    void backtrack(int start) {
        // Base case: combination of size r is complete.
        if (static_cast<int>(current.size()) == r) {
            result.push_back(current);
            return;
        }
        
        // Try each remaining element, starting from index 'start'.
        for (int i = start; i < n; ++i) {
            current.push_back(arr[i]);
            backtrack(i + 1);
            current.pop_back(); // Backtrack
        }
    }
    
    backtrack(0);
    return result;
}
// The problem is to generate all combinations of size `r` from `n` distinct elements. The classic backtracking approach works well: at each step, choose an element from the current position onward, add it to a temporary combination, and recurse to fill the remaining positions. When the temporary combination reaches size `r`, store a copy in the result. After the recursion returns, remove the chosen element (backtrack) and try the next element. Since we only pick elements from indices greater than or equal to the current start, we avoid permutations and ensure each combination is generated once. The base case is when the combination is complete. An important edge case is when `r` is outside `[1, n]`: we should return an empty vector immediately. The recursion depth is at most `r`, and the total number of combinations is C(n, r). Each combination is stored, so the space complexity is O(C(n, r) * r) for the output, plus O(r) for the temporary storage. The time complexity is O(C(n, r) * r) because for each combination we iterate over its elements to copy them.
