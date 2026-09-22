// Write a C++ function `findFirstOdd` that accepts a `std::vector<int>` and returns a `std::optional<int>` representing the first odd number found in the vector (i.e., the value, not the iterator). If no odd number exists, the function should return `std::nullopt`. The vector may be empty, may contain only even numbers, may contain duplicate odds, and may include negative values (consider negative odd numbers, e.g., -3, as odd). The function must handle all these cases correctly and must not modify the input vector.
#include <cassert>
#include <vector>
#include <optional>

// Function under test (declared here for clarity; assume it is included)
std::optional<int> findFirstOdd(const std::vector<int>& values);

int main() {
    // Basic case: first odd appears mid-list
    std::vector<int> v1 = {2, 10, 4, 5, 1, 6};
    assert(findFirstOdd(v1).value() == 5);

    // Odd is first element
    std::vector<int> v2 = {7, 2, 4};
    assert(findFirstOdd(v2).value() == 7);

    // Odd is last element
    std::vector<int> v3 = {2, 4, 6, 9};
    assert(findFirstOdd(v3).value() == 9);

    // Negative odd number
    std::vector<int> v4 = {-2, -4, -3, 6};
    assert(findFirstOdd(v4).value() == -3);

    // Duplicate odds: returns only the first
    std::vector<int> v5 = {4, 9, 9, 1};
    assert(findFirstOdd(v5).value() == 9);

    // No odd numbers present
    std::vector<int> v6 = {2, 4, 6, 8};
    assert(!findFirstOdd(v6).has_value());

    // Empty vector
    std::vector<int> v7 = {};
    assert(!findFirstOdd(v7).has_value());

    // Single even element
    std::vector<int> v8 = {10};
    assert(!findFirstOdd(v8).has_value());

    // Single odd element
    std::vector<int> v9 = {-5};
    assert(findFirstOdd(v9).value() == -5);

    return 0;
}
#include <vector>
#include <optional>

// Returns the first odd number in the vector, or std::nullopt if none exists.
std::optional<int> findFirstOdd(const std::vector<int>& values) {
    for (int value : values) {
        if (value % 2 != 0) {  // captures both positive and negative odds
            return value;
        }
    }
    return std::nullopt;
}
// The solution iterates through the vector in order (0 to n-1) and returns the first element for which `value % 2 != 0` (or `value % 2 == 1` for positive values, but using `!= 0` also captures negative odds). Since we need to return the value itself, we can store it in an `std::optional<int>`. If the loop completes without finding an odd, we return `std::nullopt`. Important edge cases: empty vector → `nullopt`; all even → `nullopt`; negative odds (e.g., -3) → should be returned; duplicate odds → only the first one is returned. Time complexity is O(n) in the worst case (if no odd exists) and O(1) in the best case (if the first element is odd). Space complexity is O(1) because only an optional and an iterator are used.
