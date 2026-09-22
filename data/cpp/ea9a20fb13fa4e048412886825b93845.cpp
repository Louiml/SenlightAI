Write a standalone C++ function named `sortPackagesByPriority` that takes a vector of package records (each containing a `type` character, an integer `height`, and an integer `ordinal` priority value) and returns a new vector with the packages sorted according to the following rules: first, group packages by descending priority value (`ordinal`), with higher ordinal values appearing earlier; within each priority group, sort by descending `height` (higher height first). The input vector may contain up to 25 packages, and duplicates are allowed. The function must not modify the input vector and must use `const` references appropriately. Assume the `Package` struct is defined globally as: `struct Package { char type; int height; int ordinal; };`. The sorting should be stable in the sense that packages with identical `ordinal` and `height` retain their original relative order (though this is not strictly required by the primary rule, it is a good practice). The output vector should contain all elements from the input, just reordered.

// The solution requires a multi-key sorting: primary key is `ordinal` (descending), secondary key is `height` (descending). The simplest approach is to create a copy of the input vector and then sort it using a custom comparator that compares `ordinal` first, then `height`. Since the standard `std::sort` is not stable, to preserve relative order for equal keys, we can either use `std::stable_sort` or add a tie-breaking on the original index. For simplicity and to meet the specification exactly, we can use `std::stable_sort` with a lambda that compares `ordinal` descending, then `height` descending. The time complexity is O(n log n) for sorting, where n is the number of packages (max 25, so negligible). The space complexity is O(n) for the copy, plus O(n) for the internal sort stack. Edge cases: empty input returns empty vector; all packages have same ordinal and height (order unchanged if using stable sort); negative heights or ordinals are handled naturally by the comparator because we only compare values, not absolute magnitudes. The function should be declared as `std::vector<Package> sortPackagesByPriority(const std::vector<Package>& packages)`.

#include <vector>
#include <algorithm>

// Represents a package to be sorted by priority (ordinal) then height.
struct Package {
    char type;
    int height;
    int ordinal;
};

// Returns a new sorted vector of packages.
// Sorting rules:
//   1. Group by descending ordinal (higher ordinal first).
//   2. Within each ordinal group, sort by descending height (higher height first).
// The input vector is not modified.
std::vector<Package> sortPackagesByPriority(const std::vector<Package>& packages) {
    // Create a copy to avoid modifying the input.
    std::vector<Package> sorted = packages;
    
    // Use stable_sort to preserve relative order for equal ordinal and height.
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const Package& a, const Package& b) {
            if (a.ordinal != b.ordinal) {
                return a.ordinal > b.ordinal; // higher ordinal first
            }
            return a.height > b.height;       // higher height first
        });
    
    return sorted;
}

#include <cassert>
#include <vector>

// Package struct is already defined in the solution.

int main() {
    // Test 1: Basic mixed ordinals and heights.
    std::vector<Package> input1 = {
        {'A', 10, 5},
        {'B', 20, 10},
        {'C', 15, 5},
        {'D', 30, 10}
    };
    std::vector<Package> expected1 = {
        {'B', 20, 10},
        {'D', 30, 10},
        {'C', 15, 5},
        {'A', 10, 5}
    };
    assert(sortPackagesByPriority(input1) == expected1);

    // Test 2: Same ordinal, different heights (descending height).
    std::vector<Package> input2 = {
        {'X', 5, 7},
        {'Y', 9, 7},
        {'Z', 2, 7}
    };
    std::vector<Package> expected2 = {
        {'Y', 9, 7},
        {'X', 5, 7},
        {'Z', 2, 7}
    };
    assert(sortPackagesByPriority(input2) == expected2);

    // Test 3: Empty input.
    std::vector<Package> input3;
    assert(sortPackagesByPriority(input3).empty());

    // Test 4: All identical keys – order should be preserved (stable sort).
    std::vector<Package> input4 = {
        {'P', 100, 1},
        {'Q', 100, 1},
        {'R', 100, 1}
    };
    std::vector<Package> expected4 = {
        {'P', 100, 1},
        {'Q', 100, 1},
        {'R', 100, 1}
    };
    assert(sortPackagesByPriority(input4) == expected4);

    // Test 5: Negative ordinal and height values.
    std::vector<Package> input5 = {
        {'M', -5, -1},
        {'N', -2, -1},
        {'O', -10, -3}
    };
    std::vector<Package> expected5 = {
        {'N', -2, -1},
        {'M', -5, -1},
        {'O', -10, -3}
    };
    assert(sortPackagesByPriority(input5) == expected5);

    // Test 6: Larger list with mixed priorities and heights, including duplicates.
    std::vector<Package> input6 = {
        {'A', 3, 2},
        {'B', 8, 3},
        {'C', 5, 2},
        {'D', 8, 3},
        {'E', 1, 1}
    };
    std::vector<Package> expected6 = {
        {'B', 8, 3},
        {'D', 8, 3},
        {'C', 5, 2},
        {'A', 3, 2},
        {'E', 1, 1}
    };
    assert(sortPackagesByPriority(input6) == expected6);

    return 0;
}
