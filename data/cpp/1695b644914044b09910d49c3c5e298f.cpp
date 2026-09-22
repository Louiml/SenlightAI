/*
Write a C++ function that reconstructs a queue given a list of people represented as `[height, k]` pairs, where `k` is the number of people in front of this person who have a height greater than or equal to their height. The input is a vector of vectors of integers, and the function must return the reconstructed queue order that satisfies all `k` constraints. The function should handle duplicate heights correctly, prioritize sorting by descending height then ascending `k`, and insert each person into the result vector at the position indicated by their `k` value. Assume the input always has a valid solution. The function should be named `reconstructQueue` and take a `std::vector<std::vector<int>>&` (or `const` reference if you copy internally) and return the same type.
*/

#include <vector>
#include <algorithm>

// Reconstruct the queue according to height and k constraints.
std::vector<std::vector<int>> reconstructQueue(std::vector<std::vector<int>>& people) {
    // Sort by height descending, then by k ascending.
    std::sort(people.begin(), people.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  if (a[0] != b[0]) return a[0] > b[0];
                  return a[1] < b[1];
              });
    
    std::vector<std::vector<int>> result;
    result.reserve(people.size());
    
    // Insert each person at index equal to their k value.
    for (const auto& person : people) {
        result.insert(result.begin() + person[1], person);
    }
    
    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Test case 1: Basic example.
    std::vector<std::vector<int>> people1 = {{7,0},{4,4},{7,1},{5,0},{6,1},{5,2}};
    std::vector<std::vector<int>> expected1 = {{5,0},{7,0},{5,2},{6,1},{4,4},{7,1}};
    auto result1 = reconstructQueue(people1);
    assert(result1 == expected1);

    // Test case 2: All same height, k increasing.
    std::vector<std::vector<int>> people2 = {{5,0},{5,1},{5,2}};
    std::vector<std::vector<int>> expected2 = {{5,0},{5,1},{5,2}};
    auto result2 = reconstructQueue(people2);
    assert(result2 == expected2);

    // Test case 3: Single person.
    std::vector<std::vector<int>> people3 = {{3,0}};
    std::vector<std::vector<int>> expected3 = {{3,0}};
    auto result3 = reconstructQueue(people3);
    assert(result3 == expected3);

    // Test case 4: Descending heights with k=0 for all.
    std::vector<std::vector<int>> people4 = {{9,0},{8,0},{7,0}};
    std::vector<std::vector<int>> expected4 = {{9,0},{8,0},{7,0}};
    auto result4 = reconstructQueue(people4);
    assert(result4 == expected4);

    // Test case 5: Duplicate heights with mixed k.
    std::vector<std::vector<int>> people5 = {{6,0},{6,2},{5,0},{5,1}};
    std::vector<std::vector<int>> expected5 = {{6,0},{5,0},{5,1},{6,2}};
    auto result5 = reconstructQueue(people5);
    assert(result5 == expected5);

    // Test case 6: Larger random-like case.
    std::vector<std::vector<int>> people6 = {{2,1},{1,0},{3,0},{2,0}};
    std::vector<std::vector<int>> expected6 = {{1,0},{2,0},{3,0},{2,1}};
    auto result6 = reconstructQueue(people6);
    assert(result6 == expected6);

    return 0;
}

// The solution sorts the people by height in descending order, and for equal heights, by `k` in ascending order. This ensures that when we process people from tallest to shortest, all people taller or equal to the current person have already been placed in the result. For a person with height `h` and `k`, the `k` value tells exactly how many already-placed people (taller or equal height) must be in front of them, so inserting them at index `k` in the result vector automatically satisfies the constraint. Because equal-height people are processed in increasing `k`, a person with smaller `k` is inserted before a person with larger `k`, maintaining stability. Edge cases include duplicate heights (handled by secondary sort on `k`), a person with `k = 0` (inserted at the front), and the last person (inserted at the end). The algorithm runs in `O(n^2)` time due to `vector::insert` shifting elements, and uses `O(n)` auxiliary space for the result vector.
