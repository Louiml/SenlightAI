// Write a C++ function that accepts a positive integer `n` representing the number of elements to analyze, and returns a `std::vector<float>` containing the first `n` even numbers (starting from 2, i.e., 2, 4, 6, ...) but with every element whose index (0-based) is a multiple of 5 replaced by its negative value. The function must validate that `n` is between 1 and 10 (inclusive); if invalid, return an empty vector. The function must be `const`-correct and not use global variables.
// The solution generates a vector of size `n` and fills it sequentially. For each index `i` from 0 to `n-1`, the raw even value is `(i+1)*2`, which matches the pattern from the snippet. If `i % 5 == 0`, replace the value with its negation (e.g., at index 0 value becomes -2, at index 5 value becomes -12). Validation: if `n < 1` or `n > 10`, return an empty vector immediately. No other edge cases exist because the function only deals with integers within the valid range; for invalid `n`, no allocation is performed. Time complexity is O(n) (filling the vector) and space complexity is O(n) (storing the vector), both linear in `n`. The function uses only constant auxiliary variables besides the returned vector.
#include <vector>

// Returns a vector of the first n even numbers starting from 2,
// where elements at indices divisible by 5 are negated.
// Returns an empty vector if n is not in [1, 10].
std::vector<float> generateEvenNumbers(int n) {
    if (n < 1 || n > 10) {
        return {};
    }

    std::vector<float> result;
    result.reserve(n);

    for (int i = 0; i < n; ++i) {
        float value = static_cast<float>((i + 1) * 2);
        if (i % 5 == 0) {
            value = -value;
        }
        result.push_back(value);
    }

    return result;
}
#include <cassert>
#include <vector>

std::vector<float> generateEvenNumbers(int n); // declaration

int main() {
    // n = 1: only index 0 is multiple of 5, so -2
    assert((generateEvenNumbers(1) == std::vector<float>{-2.0f}));
    // n = 2: index 0 negated, index 1 normal
    assert((generateEvenNumbers(2) == std::vector<float>{-2.0f, 4.0f}));
    // n = 5: indices 0 and 5? here only index 0 negated
    assert((generateEvenNumbers(5) == std::vector<float>{-2.0f, 4.0f, 6.0f, 8.0f, 10.0f}));
    // n = 6: index 5 negated (value 12 -> -12)
    assert((generateEvenNumbers(6) == std::vector<float>{-2.0f, 4.0f, 6.0f, 8.0f, 10.0f, -12.0f}));
    // n = 10: indices 0 and 5 negated
    assert((generateEvenNumbers(10) == std::vector<float>{-2.0f, 4.0f, 6.0f, 8.0f, 10.0f, -12.0f, 14.0f, 16.0f, 18.0f, 20.0f}));
    // invalid n
    assert(generateEvenNumbers(0).empty());
    assert(generateEvenNumbers(11).empty());
}
