/*
Write a C++ function `std::vector<int> findIncreasingTriplet(const std::vector<int>& arr)` that, given a non-empty vector of integers, returns a vector containing three indices (not values) `i < j < k` such that `arr[i] < arr[j] < arr[k]`. If no such triplet exists, return an empty vector. If multiple triplets exist, return the one with the smallest possible `i`, and among those, the smallest possible `j` (i.e., lexicographically smallest index triplet). The function must not modify the input vector and must handle duplicate values correctly (strictly increasing is required). The vector may contain negative numbers and up to 10^5 elements.
*/

#include <vector>
#include <stack>

// Return indices {i, j, k} with i < j < k and arr[i] < arr[j] < arr[k].
// If no such triplet exists, return an empty vector.
std::vector<int> findIncreasingTriplet(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    std::vector<int> result;
    if (n < 3) return result;

    std::stack<int> st;  // stores elements to the right, maintained in decreasing order from top to bottom
    st.push(arr[n - 1]);

    int best_i = -1;

    // Scan from right to left to find the smallest i such that there exist two strictly larger elements to the right
    for (int i = n - 2; i >= 0; --i) {
        // Remove all elements <= arr[i] because they cannot form a strictly increasing triplet with arr[i] as the first
        while (!st.empty() && st.top() <= arr[i]) {
            st.pop();
        }

        // If there are at least two elements in the stack, arr[i] can be the smallest of a valid triplet
        if (st.size() >= 2) {
            best_i = i;
            // Since we scan from right to left, the first (smallest i) found is the answer
            break;
        }

        st.push(arr[i]);
    }

    if (best_i == -1) return result;

    // Construct the triplet by scanning forward from best_i
    result.push_back(best_i);
    int prev = arr[best_i];
    for (int i = best_i + 1; i < n; ++i) {
        if (arr[i] > prev) {
            result.push_back(i);
            prev = arr[i];
            if (result.size() == 3) break;
        }
    }

    // In case we somehow didn't find 3 elements (shouldn't happen if best_i found), return empty
    if (result.size() != 3) result.clear();
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic increasing triplet
    std::vector<int> a1 = {1, 2, 3, 4};
    assert(findIncreasingTriplet(a1) == std::vector<int>({0, 1, 2}));

    // Triplet not at the beginning
    std::vector<int> a2 = {5, 1, 4, 2, 3};
    assert(findIncreasingTriplet(a2) == std::vector<int>({1, 2, 4}));

    // No triplet exists (strictly decreasing)
    std::vector<int> a3 = {9, 8, 7, 6};
    assert(findIncreasingTriplet(a3).empty());

    // Duplicate values prevent strict increase
    std::vector<int> a4 = {1, 2, 2, 3};
    assert(findIncreasingTriplet(a4) == std::vector<int>({0, 1, 3}));

    // Negative numbers
    std::vector<int> a5 = {-5, -1, 0, 3};
    assert(findIncreasingTriplet(a5) == std::vector<int>({0, 1, 2}));

    // Smallest i (lexicographically smallest triplet)
    std::vector<int> a6 = {2, 1, 3, 4, 5};
    assert(findIncreasingTriplet(a6) == std::vector<int>({1, 2, 3}));

    // Only two elements
    std::vector<int> a7 = {1, 2};
    assert(findIncreasingTriplet(a7).empty());

    // Large values and negative
    std::vector<int> a8 = {0, -100, 50, 60, 7};
    assert(findIncreasingTriplet(a8) == std::vector<int>({1, 2, 3}));

    // Triplet at the very end
    std::vector<int> a9 = {10, 20, 30, 40, 1, 2, 3};
    assert(findIncreasingTriplet(a9) == std::vector<int>({0, 1, 2}));

    // All equal values
    std::vector<int> a10 = {7, 7, 7, 7};
    assert(findIncreasingTriplet(a10).empty());

    return 0;
}

// The key idea is to process the array from right to left while maintaining a monotonic stack that keeps a decreasing sequence of candidate "middle" and "largest" elements. For each element `arr[i]` when scanning from right, we pop from the stack all elements that are less than or equal to `arr[i]` because those cannot form a strictly increasing triplet with `arr[i]` as the smallest (since they need to be larger than `arr[i]` for a valid triplet, and strictly greater is required). After popping, the size of the stack tells us how many larger elements exist to the right that can serve as `j` and `k`. When the stack size becomes 2, we have found a valid `i` such that there exist two distinct elements to the right that are strictly greater than `arr[i]` and also satisfy the increasing order between themselves (because the stack keeps them sorted increasingly from bottom to top). We record the first (rightmost) such `i` encountered during the scan — that will give the smallest `i` because we scan from right to left, and the first time we see a size-2 stack, that `i` is the smallest index satisfying the condition. Then, to construct the triplet, we start with `arr[i]` and scan forward from `i+1` to find the first element greater than the current value, update the current value, and repeat twice. This yields the lexicographically smallest `j` and `k` among those with the minimal `i`. Edge cases: if no element produces a stack size of 2, return empty. Duplicate values are handled by the `<=` condition in the popping loop, which ensures strict inequality. Time complexity is O(N) because each element is pushed and popped at most once, and the final forward scan is O(N). Space complexity is O(N) in the worst case for the stack.
