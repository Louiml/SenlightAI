Write a C++ function that takes a reference to a `std::vector<int>` representing a stack (where the last element is the top) and returns a new `std::vector<int>` containing the original stack's elements in the order they would be printed by the given snippet's `print()` method (from top to bottom), but with all duplicate values removed. The function must preserve the relative order of the first occurrence of each value when scanning from the top of the stack downward. For example, if the stack is {5, 3, 5, 2, 3} (top to bottom: 3, 2, 5, 3, 5), the output should be {3, 2, 5}. Ensure the function does not modify the input vector.
#include <cassert>
#include <vector>

int main() {
    std::vector<int> empty;
    assert(uniqueStackFromTop(empty) == std::vector<int>{});

    std::vector<int> single = {42};
    assert(uniqueStackFromTop(single) == std::vector<int>{42});

    std::vector<int> allDuplicates = {7, 7, 7};
    assert(uniqueStackFromTop(allDuplicates) == std::vector<int>{7});

    std::vector<int> mixed = {5, 3, 5, 2, 3};
    assert(uniqueStackFromTop(mixed) == std::vector<int>{3, 2, 5});

    std::vector<int> noDuplicates = {1, 2, 3};
    assert(uniqueStackFromTop(noDuplicates) == std::vector<int>{3, 2, 1});

    std::vector<int> larger = {10, -3, 10, 4, -3, 0, 4};
    assert(uniqueStackFromTop(larger) == std::vector<int>{4, 0, -3, 10});
}
#include <vector>
#include <unordered_set>

// Return a vector of unique values from the input stack (last element is top),
// in order from top to bottom, preserving first occurrence from the top.
std::vector<int> uniqueStackFromTop(const std::vector<int>& stack) {
    std::vector<int> result;
    std::unordered_set<int> seen;
    // Scan from top (last element) down to bottom (first element)
    for (int i = static_cast<int>(stack.size()) - 1; i >= 0; --i) {
        int value = stack[i];
        if (seen.find(value) == seen.end()) {
            seen.insert(value);
            result.push_back(value);
        }
    }
    return result;
}
// The task simulates processing a stack from its top downward while removing duplicates. The main algorithm is a simple linear scan from the last element of the input vector to the first (since the last element is the stack's top). For each element, check whether it has already been seen; if not, append it to a result vector and mark it as seen. Because the order matters only by first occurrence from the top, scanning in reverse guarantees this. Use a `std::unordered_set<int>` for O(1) average-time duplicate checks, or a `std::set` for O(log n) but simpler deterministic behavior. Important edge cases: an empty vector (return empty), a vector with all duplicates (return one element), and a vector with no duplicates (return the same elements in reverse order). Time complexity is O(n) with an unordered_set (average) or O(n log n) with a set, where n is the number of elements. Auxiliary space is O(n) for the result and the set.
