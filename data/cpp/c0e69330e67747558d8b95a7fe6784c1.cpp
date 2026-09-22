Write a standalone C++ function that, given an array of 3D points represented as a vector of `std::array<double,3>` (or a user-defined struct with `double x, y, z`), removes exact duplicate points (where all three coordinates are bitwise identical) and returns a `std::vector` of unique points. The function must also produce a cross-reference (mapping) from each original index to the index of the first occurrence of the same point in the output vector. The function should preserve the relative order of first occurrences (i.e., output points appear in the order of their first appearance in the input). Handle empty input gracefully, and ensure that the algorithm is efficient for large inputs (e.g., up to 1 million points) by using sorting or hashing rather than naive O(n²) duplicate checks. The function signature should be: `std::pair<std::vector<Point>, std::vector<size_t>> removeDuplicatePoints(const std::vector<Point>& points);` where `Point` is a struct with three `double` members. The function must be `const`-correct and should not modify the input.
// The solution approach is to sort the indices of the points based on the tuple of their coordinates, then iterate through the sorted indices and compare each point to the previous unique point (using exact equality of all three `double` components). If a point differs from the previously kept point, it is appended to the output unique vector, and its new index (which is the current size of the unique vector minus one) is assigned to the cross-reference for that original index. If it matches the previous unique point, the cross-reference is set to the index of that previous unique point. Key edge cases: (1) Empty input – return empty vectors. (2) All duplicates – the unique vector will have one element, and all cross-references point to 0. (3) Exact floating-point comparison – since the task specifies bitwise identical duplicates, using direct `==` comparison on `double` is safe; no tolerance is needed. (4) To preserve order of first appearance, we sort indices, but the output is built by scanning sorted indices, which naturally groups identical points together. However, since we only output a point when it first appears in sorted order, the order of first appearance in the original array is not automatically preserved—for example, if point A appears at index 0 and point B appears at index 5, but B sorts before A, then B would be output first. To fix this, we need to map each unique point to its first original index, then sort the unique points by that first original index. Alternative approach: use an unordered_map with a custom hash (e.g., hash the three doubles as strings or combine their bit patterns) to store the first occurrence index, then output in original order. That is simpler and preserves order naturally, but the task explicitly says "efficient" and suggests sorting or hashing—hashing is acceptable. However, to match the spirit of the snippet (which uses radix sort on float bits), I'll present a sorting-based solution that preserves order by storing first occurrence indices and then sorting unique points by those indices. Time complexity: Sorting indices is O(n log n), scanning is O(n), and final sorting of unique points is O(m log m) where m ≤ n, so overall O(n log n). Space is O(n) for sorted indices, cross-reference, and temporary storage. Hashing would be O(n) average time but requires a robust hash; to keep the solution self-contained and avoid hash collisions, sorting is preferred.
#include <utility>
#include <vector>
#include <algorithm>
#include <array>
#include <cstddef>

// Define the point struct
struct Point {
    double x, y, z;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Helper to compare points using coordinate-wise ordering
bool pointLess(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    if (a.y != b.y) return a.y < b.y;
    return a.z < b.z;
}

// Main function: remove exact duplicates, preserve first-occurrence order,
// and return unique points plus cross-reference from original index to unique index.
std::pair<std::vector<Point>, std::vector<size_t>>
removeDuplicatePoints(const std::vector<Point>& points) {
    const size_t n = points.size();
    std::vector<size_t> xref(n);              // cross-reference for each original index
    if (n == 0) return { {}, {} };

    // Create an array of indices to sort
    std::vector<size_t> indices(n);
    for (size_t i = 0; i < n; ++i) indices[i] = i;

    // Sort indices based on point ordering
    std::sort(indices.begin(), indices.end(),
        [&points](size_t i, size_t j) { return pointLess(points[i], points[j]); });

    // First pass: gather unique points and record their first original index
    std::vector<Point> unique_points;
    std::vector<size_t> first_occurrence;   // for each unique point, the smallest original index
    unique_points.reserve(n);
    first_occurrence.reserve(n);

    size_t prev_orig = indices[0];
    unique_points.push_back(points[prev_orig]);
    first_occurrence.push_back(prev_orig);
    xref[prev_orig] = 0;

    for (size_t k = 1; k < n; ++k) {
        size_t orig = indices[k];
        if (!(points[orig] == unique_points.back())) {
            unique_points.push_back(points[orig]);
            first_occurrence.push_back(orig);
        }
        // Set cross-reference for this original index to the index of the last unique point
        size_t ui = unique_points.size() - 1;
        xref[orig] = ui;
    }

    // Now we need to order unique_points by their first_occurrence (which is the original order)
    const size_t m = unique_points.size();
    std::vector<size_t> order(m);
    for (size_t i = 0; i < m; ++i) order[i] = i;
    std::sort(order.begin(), order.end(),
        [&first_occurrence](size_t i, size_t j) { return first_occurrence[i] < first_occurrence[j]; });

    // Create the final unique vector in correct order
    std::vector<Point> final_unique(m);
    std::vector<size_t> remap(m);   // maps old unique index to new position
    for (size_t new_pos = 0; new_pos < m; ++new_pos) {
        size_t old_pos = order[new_pos];
        final_unique[new_pos] = unique_points[old_pos];
        remap[old_pos] = new_pos;
    }

    // Update cross-reference using remap
    for (size_t& idx : xref) idx = remap[idx];

    return { std::move(final_unique), std::move(xref) };
}
#include <cassert>
#include <iostream>

// Assume Point is defined as in the solution.

int main() {
    // Test 1: Empty input
    auto res0 = removeDuplicatePoints({});
    assert(res0.first.empty());
    assert(res0.second.empty());

    // Test 2: All distinct points
    std::vector<Point> pts1 = {{1.0,2.0,3.0}, {4.0,5.0,6.0}, {0.0,0.0,0.0}};
    auto res1 = removeDuplicatePoints(pts1);
    assert(res1.first.size() == 3);
    assert(res1.second == std::vector<size_t>({0,1,2}));
    assert(res1.first[0] == pts1[0]);
    assert(res1.first[1] == pts1[1]);
    assert(res1.first[2] == pts1[2]);

    // Test 3: All duplicates
    std::vector<Point> pts2 = {{1.0,2.0,3.0}, {1.0,2.0,3.0}, {1.0,2.0,3.0}};
    auto res2 = removeDuplicatePoints(pts2);
    assert(res2.first.size() == 1);
    assert(res2.second == std::vector<size_t>({0,0,0}));
    assert(res2.first[0] == pts2[0]);

    // Test 4: Mixed duplicates with interleaved original order
    std::vector<Point> pts3 = {{3.0,3.0,3.0}, {1.0,1.0,1.0}, {3.0,3.0,3.0}, {2.0,2.0,2.0}, {1.0,1.0,1.0}};
    auto res3 = removeDuplicatePoints(pts3);
    // Unique points should be in first-occurrence order: (3,3,3), (1,1,1), (2,2,2)
    assert(res3.first.size() == 3);
    assert(res3.first[0] == Point{3.0,3.0,3.0});
    assert(res3.first[1] == Point{1.0,1.0,1.0});
    assert(res3.first[2] == Point{2.0,2.0,2.0});
    // Cross-reference: indices 0->0, 1->1, 2->0, 3->2, 4->1
    assert(res3.second == std::vector<size_t>({0,1,0,2,1}));

    // Test 5: Negative and fractional coordinates, exact match
    std::vector<Point> pts4 = {{-1.0, 0.5, 2.0}, {-1.0, 0.5, 2.0}, {0.0,0.0,0.0}};
    auto res4 = removeDuplicatePoints(pts4);
    assert(res4.first.size() == 2);
    assert(res4.first[0] == Point{-1.0,0.5,2.0});
    assert(res4.first[1] == Point{0.0,0.0,0.0});
    assert(res4.second == std::vector<size_t>({0,0,1}));

    // Test 6: Large input with many duplicates (performance smoke test)
    std::vector<Point> pts5;
    pts5.reserve(100000);
    for (int i = 0; i < 100000; ++i) {
        if (i % 3 == 0) pts5.push_back({100.0, 200.0, 300.0});
        else if (i % 3 == 1) pts5.push_back({-5.0, 10.0, 20.0});
        else pts5.push_back({0.0, 0.0, 0.0});
    }
    auto res5 = removeDuplicatePoints(pts5);
    assert(res5.first.size() == 3);
    assert(res5.second.size() == 100000);
    // Verify cross-reference correctness for a few indices
    assert(res5.second[0] == 0);
    assert(res5.second[1] == 1);
    assert(res5.second[2] == 2);
    assert(res5.second[99999] == 2);

    // Test 7: Single point
    std::vector<Point> pts6 = {{42.0, 0.0, -1.0}};
    auto res6 = removeDuplicatePoints(pts6);
    assert(res6.first.size() == 1);
    assert(res6.second == std::vector<size_t>({0}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
