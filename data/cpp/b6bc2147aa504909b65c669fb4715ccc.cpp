Given a series of test cases where each test case consists of an integer `n` followed by `n` integers, write a C++ function `areAlternatingParities(const std::vector<int>& arr)` that returns `true` if all elements at even indices (0-based) have the same parity (all even or all odd), and all elements at odd indices also have the same parity (which may differ from the even-index parity). The function should handle arrays of length 0 or 1 appropriately: length 0 returns `true` (vacuously), length 1 returns `true` (only even index). Return `false` otherwise.

#include <cassert>
#include <vector>

// Solution function is declared above.

int main() {
    // Basic cases from typical tests
    assert(areAlternatingParities({1, 2, 3, 4}) == true);      // even idx: 1,3 odd; odd idx: 2,4 even
    assert(areAlternatingParities({2, 3, 4, 5}) == true);      // even idx: 2,4 even; odd idx: 3,5 odd
    assert(areAlternatingParities({1, 3, 2}) == false);        // even idx: 1,2 differ

    // Edge cases
    assert(areAlternatingParities({}) == true);                // empty
    assert(areAlternatingParities({7}) == true);               // single element
    assert(areAlternatingParities({4, 4}) == true);            // same parity both indices
    assert(areAlternatingParities({4, 5, 4, 5}) == true);      // alternating exactly
    assert(areAlternatingParities({1, 2, 1}) == false);        // odd index only one element, ok
    assert(areAlternatingParities({2, 2, 3}) == false);        // even index 2 and 3 differ

    // Larger test
    std::vector<int> arr = {1, 2, 1, 2, 1, 2};
    assert(areAlternatingParities(arr) == true);
    arr[4] = 4;
    assert(areAlternatingParities(arr) == false);

    return 0;
}

#include <vector>

// Return true if all even-indexed elements share the same parity and all odd-indexed elements share the same parity.
bool areAlternatingParities(const std::vector<int>& arr) {
    const size_t n = arr.size();
    if (n == 0) return true;

    const int evenParity = arr[0] % 2;

    // If n == 1, no odd indices exist, so condition is satisfied.
    if (n == 1) return true;

    const int oddParity = arr[1] % 2;

    // Check all even indices.
    for (size_t i = 0; i < n; i += 2) {
        if (arr[i] % 2 != evenParity) return false;
    }

    // Check all odd indices.
    for (size_t i = 1; i < n; i += 2) {
        if (arr[i] % 2 != oddParity) return false;
    }

    return true;
}

// The solution tracks the required parity for even-indexed positions using the first element's parity (`arr[0] % 2`), and for odd-indexed positions using the second element's parity (`arr[1] % 2`) only if `n > 1`. Then iterate through all even indices starting at 0 and check that each element's parity matches the predefined even parity; similarly iterate odd indices starting at 1 and check against the odd parity. If any mismatch is found, return `false` immediately. Edge cases: empty vector → `true`; single-element vector → `true` because only even index exists and it trivially matches itself. The algorithm runs in O(n) time and uses O(1) auxiliary space, making it efficient for large inputs.
