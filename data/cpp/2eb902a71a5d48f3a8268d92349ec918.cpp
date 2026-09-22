/*
Write a C++ function `computeHeightDifferences` that takes a vector of integers representing the heights of people (where each height is unique) and returns a vector of integers. For each person in the original order, the output should contain the difference between their height and the tallest height among all people, except that for the single tallest person, the difference should be computed against the second tallest height. If there are fewer than two people, return an empty vector. The function must not modify the input vector and should handle duplicate heights (in which case the "tallest" and "second tallest" are the same value, so each output element is `height - tallest`).
*/

#include <vector>
#include <algorithm>

// For each person, compute height - tallest, except the persons who are the tallest,
// for whom compute height - second_tallest. Returns empty vector for size < 2.
std::vector<int> computeHeightDifferences(const std::vector<int>& heights) {
    const int n = static_cast<int>(heights.size());
    if (n < 2) {
        return {};
    }
    
    // Make a sorted copy to find the tallest and second tallest.
    std::vector<int> sorted = heights;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    const int tallest = sorted[0];
    const int second_tallest = sorted[1];
    
    std::vector<int> result;
    result.reserve(n);
    for (const int h : heights) {
        if (h != tallest) {
            result.push_back(h - tallest);
        } else {
            // This includes the tallest person(s) — compute against second tallest.
            result.push_back(h - second_tallest);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic case with unique heights.
    std::vector<int> heights1 = {170, 180, 160, 175};
    std::vector<int> expected1 = {-10, 0, -20, -5};
    assert(computeHeightDifferences(heights1) == expected1);

    // Tallest person gets difference against second tallest (positive if second tallest is shorter).
    std::vector<int> heights2 = {150, 190, 170};
    std::vector<int> expected2 = {-40, 20, -20};
    assert(computeHeightDifferences(heights2) == expected2);

    // Duplicate tallest values: both map to height - second_tallest (which equals same as height - tallest).
    std::vector<int> heights3 = {180, 180, 170};
    std::vector<int> expected3 = {0, 0, -10};
    assert(computeHeightDifferences(heights3) == expected3);

    // Only two elements: tallest gets difference against second, second gets negative difference.
    std::vector<int> heights4 = {100, 200};
    std::vector<int> expected4 = {-100, 100};
    assert(computeHeightDifferences(heights4) == expected4);

    // Fewer than two elements → empty vector.
    std::vector<int> heights5 = {42};
    assert(computeHeightDifferences(heights5).empty());
    std::vector<int> heights6;
    assert(computeHeightDifferences(heights6).empty());

    // Larger case with many people.
    std::vector<int> heights7 = {155, 165, 175, 185, 195};
    std::vector<int> expected7 = {-40, -30, -20, -10, 10};
    assert(computeHeightDifferences(heights7) == expected7);

    return 0;
}

// The solution sorts a copy of the input to find the largest and second-largest values. Since duplicates are allowed, the largest and second-largest can be equal; this is handled by using values from the sorted descending array. For each element in the original array, if it is not equal to the largest value, output `height - largest`. If it equals the largest value (which might be multiple persons if duplicates exist), output `height - secondLargest`. Edge cases: if the vector has fewer than 2 elements, return empty vector (since there is no valid second-largest). Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(n)\) for the sorted copy and the output vector. The algorithm is straightforward: sort a copy descending, extract the top two values, then iterate over the original vector and compute differences as described.
