// Write a C++ function named `strictlyIncreasingPairs` that takes a constant reference to a vector of integers and returns a vector of strings. For every adjacent pair of elements `(arr[i], arr[i-1])` starting at index 1, the output string must be `"Yes"` if `arr[i] > arr[i-1]` (strictly greater), and `"No"` otherwise. The function should handle an empty input vector by returning an empty vector, and for a vector with one element it should also return an empty vector (since there are no adjacent pairs). Preserve the order of outputs exactly as they appear in the vector. Do not modify the input vector.

// The core algorithm is a simple linear scan that iterates through the vector starting from index 1 (the second element). For each index `i`, compare `arr[i]` with `arr[i-1]`. If `arr[i]` is strictly greater, push `"Yes"` into the result vector; otherwise, push `"No"`. Edge cases: if the input vector has fewer than two elements, there are no adjacent pairs, so the result vector must be empty. Duplicate values produce `"No"` because the requirement is strict greater than. Negative numbers and large integers are handled correctly because the comparison works on the values directly. Time complexity is O(n) where n is the number of elements in the input vector. Space complexity is O(n) for the output vector (which is required to store the results). The input vector itself is not copied, and we only use constant extra space for iteration.

#include <vector>
#include <string>

// Returns "Yes" for each position i>=1 if arr[i] > arr[i-1], otherwise "No".
// Returns an empty vector if input has fewer than 2 elements.
std::vector<std::string> strictlyIncreasingPairs(const std::vector<int>& arr) {
    std::vector<std::string> result;
    if (arr.size() < 2) {
        return result;
    }
    result.reserve(arr.size() - 1);
    for (std::size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > arr[i - 1]) {
            result.push_back("Yes");
        } else {
            result.push_back("No");
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be declared above.)

int main() {
    std::vector<int> empty;
    assert(strictlyIncreasingPairs(empty).empty());

    std::vector<int> single = {42};
    assert(strictlyIncreasingPairs(single).empty());

    std::vector<int> increasing = {1, 2, 3, 4};
    assert(strictlyIncreasingPairs(increasing) == std::vector<std::string>({"Yes", "Yes", "Yes"}));

    std::vector<int> decreasing = {5, 4, 3, 2};
    assert(strictlyIncreasingPairs(decreasing) == std::vector<std::string>({"No", "No", "No"}));

    std::vector<int> mixed = {3, 5, 5, 2, 8};
    assert(strictlyIncreasingPairs(mixed) == std::vector<std::string>({"Yes", "No", "No", "Yes"}));

    std::vector<int> negatives = {-3, -1, -2, 0, -4};
    assert(strictlyIncreasingPairs(negatives) == std::vector<std::string>({"Yes", "No", "Yes", "No"}));

    std::vector<int> large = {1000000, -1000000, 2000000};
    assert(strictlyIncreasingPairs(large) == std::vector<std::string>({"No", "Yes"}));
}
