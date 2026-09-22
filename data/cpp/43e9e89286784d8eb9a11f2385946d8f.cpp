You are given an integer `n` followed by `n` lines, each containing an integer `age` and a string `name`. Write a C++ function `stable_sort_by_age` that takes a `std::vector<std::pair<int, std::string>>` (where the first element is the age, second is the name) and returns a new vector sorted by age in ascending order. The sorting must be **stable**, meaning that if two entries have the same age, their original relative order must be preserved. The function must not modify the input vector and must work for any size (including empty) and any age values (including negative and duplicate). The returned vector should contain the same pairs as the input but reordered according to the rule above. You may use any standard library facilities, but the core logic must be your own (i.e., implement the stable sort explicitly, not by calling `std::stable_sort`). Assume `name` contains no whitespace.
// The main algorithm is a stable sort. A stable sort preserves the relative order of equal elements. The simplest way to implement this is to use an insertion sort, which is naturally stable: iterate over the input vector from left to right, and for each element, move it left past any elements that are strictly greater (so equal elements are not moved). This works in O(n²) worst-case time and O(1) auxiliary space (or O(n) if we create a copy of the input and sort in place, which is fine). Alternatively, a merge sort is also stable and runs in O(n log n), but insertion sort is simpler and matches the typical educational context of the snippet. Edge cases: empty vector returns empty; negative ages sort normally; duplicate ages maintain original order; single element is trivially sorted. The function should take the vector by const reference and return a new vector sorted, leaving input unchanged. Time complexity for insertion sort is O(n²) in the worst case, space complexity O(1) extra (excluding the output vector).
#include <vector>
#include <utility>
#include <string>

// Stable sort a vector of (age, name) pairs by age (ascending).
// Returns a new sorted vector; does not modify the input.
std::vector<std::pair<int, std::string>> stable_sort_by_age(const std::vector<std::pair<int, std::string>>& input) {
    std::vector<std::pair<int, std::string>> result = input; // copy

    // Insertion sort – naturally stable because we only shift strictly greater elements.
    for (size_t i = 1; i < result.size(); ++i) {
        std::pair<int, std::string> key = result[i];
        size_t j = i;
        while (j > 0 && result[j - 1].first > key.first) {
            result[j] = result[j - 1];
            --j;
        }
        result[j] = key;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Include the solution function here or link it.

int main() {
    // Basic sort
    std::vector<std::pair<int, std::string>> v1 = {{30, "Alice"}, {20, "Bob"}, {10, "Charlie"}};
    auto r1 = stable_sort_by_age(v1);
    assert(r1.size() == 3);
    assert(r1[0] == std::make_pair(10, "Charlie"));
    assert(r1[1] == std::make_pair(20, "Bob"));
    assert(r1[2] == std::make_pair(30, "Alice"));

    // Stability with duplicates
    std::vector<std::pair<int, std::string>> v2 = {{25, "Zeta"}, {25, "Alpha"}, {25, "Beta"}};
    auto r2 = stable_sort_by_age(v2);
    assert(r2[0] == std::make_pair(25, "Zeta"));
    assert(r2[1] == std::make_pair(25, "Alpha"));
    assert(r2[2] == std::make_pair(25, "Beta"));

    // Negative ages and interleaved duplicates
    std::vector<std::pair<int, std::string>> v3 = {{-2, "neg2"}, {5, "pos"}, {-2, "neg2b"}, {0, "zero"}, {5, "pos2"}};
    auto r3 = stable_sort_by_age(v3);
    assert(r3[0] == std::make_pair(-2, "neg2"));
    assert(r3[1] == std::make_pair(-2, "neg2b"));
    assert(r3[2] == std::make_pair(0, "zero"));
    assert(r3[3] == std::make_pair(5, "pos"));
    assert(r3[4] == std::make_pair(5, "pos2"));

    // Empty vector
    std::vector<std::pair<int, std::string>> v4;
    auto r4 = stable_sort_by_age(v4);
    assert(r4.empty());

    // Single element
    std::vector<std::pair<int, std::string>> v5 = {{1, "one"}};
    auto r5 = stable_sort_by_age(v5);
    assert(r5.size() == 1 && r5[0] == std::make_pair(1, "one"));

    // Input not modified
    std::vector<std::pair<int, std::string>> v6 = {{3, "c"}, {1, "a"}, {2, "b"}};
    auto r6 = stable_sort_by_age(v6);
    assert(v6[0] == std::make_pair(3, "c"));
    assert(v6[1] == std::make_pair(1, "a"));
    assert(v6[2] == std::make_pair(2, "b"));

    return 0;
}
