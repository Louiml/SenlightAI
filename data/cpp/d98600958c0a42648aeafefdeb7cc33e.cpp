// Write a C++ function named `generateNumberSequence` that takes two integer parameters: `start` (the first element of the sequence) and `count` (the number of elements to produce). The function must return a `std::vector<long long>` containing exactly `count` numbers, where the sequence is arithmetic with a constant difference equal to `start` (i.e., each subsequent element is the previous element plus `start`). For example, if `start = 3` and `count = 5`, the returned vector must be `{3, 6, 9, 12, 15}`. If `count` is 0, return an empty vector. If `count` is negative, treat it as 0 (return an empty vector). The function must be robust for negative `start` values (e.g., `start = -2, count = 3` should give `{-2, -4, -6}`), and must use `long long` internally to avoid overflow for moderately large `start` and `count`. No input/output is required; just return the vector.
The solution initializes an empty `std::vector<long long>` and creates a variable `current` storing the current term, initially set to `start` (converted to `long long`). A loop runs exactly `count` times (or zero times if `count <= 0`). In each iteration, push `current` onto the vector, then increment `current` by `start`. This directly constructs the arithmetic progression. Important edge cases: (1) `count == 0` or negative → return empty vector immediately (the loop naturally handles this if we use `for (int i = 0; i < count; ++i)` but we must guard against negative `count` to avoid infinite loop or undefined behavior, so we can set `count` to 0 if it is negative). (2) Negative `start` works because adding a negative number decrements the current value. (3) Overflows are mitigated by using `long long` for `current` and the vector elements. Time complexity is \(O(\text{count})\) because we perform a constant amount of work per element. Space complexity is \(O(\text{count})\) for storing the result vector (ignoring the input parameters).
#include <vector>

// Generate an arithmetic sequence of 'count' numbers starting at 'start',
// where the constant difference between consecutive terms is 'start'.
// If 'count' is non-positive, returns an empty vector.
std::vector<long long> generateNumberSequence(int start, int count) {
    std::vector<long long> result;
    if (count <= 0) {
        return result;
    }
    long long current = static_cast<long long>(start);
    const long long difference = static_cast<long long>(start);
    for (int i = 0; i < count; ++i) {
        result.push_back(current);
        current += difference;
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// For brevity, include the implementation here or rely on linking.
std::vector<long long> generateNumberSequence(int start, int count); // declaration

int main() {
    // Basic positive case
    assert((generateNumberSequence(3, 5) == std::vector<long long>{3, 6, 9, 12, 15}));
    // Single element
    assert((generateNumberSequence(7, 1) == std::vector<long long>{7}));
    // Zero count returns empty
    assert((generateNumberSequence(100, 0) == std::vector<long long>{}));
    // Negative count treated as zero
    assert((generateNumberSequence(100, -3) == std::vector<long long>{}));
    // Negative start
    assert((generateNumberSequence(-2, 4) == std::vector<long long>{-2, -4, -6, -8}));
    // Start=0, count>0
    assert((generateNumberSequence(0, 3) == std::vector<long long>{0, 0, 0}));
    // Larger values to demonstrate long long usage
    assert((generateNumberSequence(2000000000, 3) == std::vector<long long>{2000000000LL, 4000000000LL, 6000000000LL}));
    return 0;
}
