Write a C++ function that takes a vector of integers and returns a new vector containing only the elements that appear exactly once in the original vector, preserving their original relative order. The function should be named `uniqueElements` and must handle empty input gracefully (returning an empty vector). You must implement this without using any standard library containers other than `std::vector` and without sorting the input. For example, given `{1, 2, 3, 2, 4, 1}`, the output should be `{3, 4}` because 1 and 2 appear more than once. Use only `std::vector` and basic loops; avoid using `std::map`, `std::unordered_map`, or any other associative containers.
The key challenge is counting occurrences of each element without using associative containers. Since the input is a vector of integers, we can use a nested loop approach: for each element at index `i`, count how many times it appears in the entire vector. If the count is exactly 1, append it to the result. This is O(n^2) time in the worst case because for each element we scan the whole vector. Space complexity is O(n) for the output vector, but O(1) extra auxiliary space besides the output. Edge cases include empty input (return empty), all duplicates (return empty), all unique (return the same vector), and negative numbers (handled naturally by integer comparison). We must carefully avoid duplicate additions if the same value appears once but at multiple positions—but since each position is checked independently, a value that appears exactly once will only be at one position, so it's fine. If a value appears multiple times, none of its positions will be added because the count is >1 for each. This ensures each unique single-occurrence value appears exactly once.
#include <vector>

// Return a vector containing only the elements that appear exactly once in the input,
// preserving their original relative order. Empty input returns an empty vector.
std::vector<int> uniqueElements(const std::vector<int>& input) {
    std::vector<int> result;
    for (std::size_t i = 0; i < input.size(); ++i) {
        int count = 0;
        for (std::size_t j = 0; j < input.size(); ++j) {
            if (input[j] == input[i]) {
                ++count;
            }
        }
        if (count == 1) {
            result.push_back(input[i]);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 2, 3, 2, 4, 1};
    std::vector<int> r1 = uniqueElements(v1);
    assert(r1 == std::vector<int>({3, 4}));

    std::vector<int> v2 = {5, 5, 5};
    std::vector<int> r2 = uniqueElements(v2);
    assert(r2.empty());

    std::vector<int> v3 = {};
    assert(uniqueElements(v3).empty());

    std::vector<int> v4 = {-1, -2, -1, 3};
    std::vector<int> r4 = uniqueElements(v4);
    assert(r4 == std::vector<int>({-2, 3}));

    std::vector<int> v5 = {7};
    assert(uniqueElements(v5) == std::vector<int>({7}));

    std::vector<int> v6 = {0, 0, 0, 1, 1, 2};
    std::vector<int> r6 = uniqueElements(v6);
    assert(r6 == std::vector<int>({2}));

    std::vector<int> v7 = {1, 2, 3, 4};
    assert(uniqueElements(v7) == v7);

    std::vector<int> v8 = {1, 1, 2, 2, 3, 3};
    assert(uniqueElements(v8).empty());
}
