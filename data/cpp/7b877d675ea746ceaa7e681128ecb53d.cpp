Write a C++ function `averageOfRange` that takes two random-access iterators (e.g., pointers into an array) representing a half-open range `[first, last)` and returns the arithmetic mean of all elements in that range as a `double`. The function must work for both `int` and `char` arrays (including the case where the range contains characters from a C-string, with the null terminator excluded). The input range is guaranteed to be non-empty. Your implementation must be generic (template-based) and handle potential type promotion correctly (e.g., summing `char` values as integers before dividing by the count). You are allowed to use `std::iterator_traits` to deduce the value type, but the return type must always be `double`. Do not assume the iterators are random-access other than the fact that they support `operator*`, `operator++`, and `operator!=`; but the function will only be tested with pointers into arrays. However, for generality, implement it as a template that works with any forward iterator (since the range is half-open, you only need forward iteration).
The core idea is to iterate over the range from `first` to `last`, accumulating the sum of all elements as a `double` to avoid integer overflow and to correctly handle character promotions (e.g., a `char` with value `'t'` is promoted to an integer ASCII value). Initialize a counter to zero, then for each element, add `static_cast<double>(*it)` to the sum and increment the counter. After the loop, return `sum / count`. Since the range is non-empty, the count is at least 1, so division by zero is not an issue. Edge cases: if the range contains negative values (e.g., signed chars with values like -5), the sum and average correctly reflect that. If the range is exactly one element, the average equals that element's numeric value. Time complexity is O(n) where n is the number of elements in the range, and space complexity is O(1) auxiliary, as we only use a few local variables.
#include <iterator>   // for std::iterator_traits
#include <cstddef>    // for std::size_t

// Compute the arithmetic mean of a non-empty range [first, last).
// Works for any forward iterator; returns the average as a double.
template <typename ForwardIterator>
double averageOfRange(ForwardIterator first, ForwardIterator last) {
    using ValueType = typename std::iterator_traits<ForwardIterator>::value_type;
    
    double sum = 0.0;
    std::size_t count = 0;
    
    for (ForwardIterator it = first; it != last; ++it) {
        sum += static_cast<double>(*it);
        ++count;
    }
    
    return sum / static_cast<double>(count);
}
#include <cassert>
#include <cstddef>

int main() {
    // Test with integer array
    int nums[] = {1, 2, 3, 4, 5};
    assert(averageOfRange(nums, nums + 5) == 3.0);

    // Test with single element
    int single[] = {7};
    assert(averageOfRange(single, single + 1) == 7.0);

    // Test with negative values
    int negs[] = {-10, -20, -30};
    assert(averageOfRange(negs, negs + 3) == -20.0);

    // Test with char array (ASCII values)
    char name[] = "templates"; // length 9 without null
    std::size_t len = sizeof(name) - 1;
    // ASCII sum of "templates": t=116, e=101, m=109, p=112, l=108, a=97, t=116, e=101, s=115 -> sum = 975
    double avgChar = averageOfRange(name, name + len);
    assert((avgChar - 108.33333333333333) < 1e-9);  // 975/9 = 108.333...

    // Test with mixed positive/negative chars (signed char)
    char mixed[] = {5, -5, 10, -10};
    assert(averageOfRange(mixed, mixed + 4) == 0.0);

    // Test with duplicate values
    int dups[] = {3, 3, 3, 3};
    assert(averageOfRange(dups, dups + 4) == 3.0);

    // Test larger range to verify precision
    int large[] = {1000000, 2000000, 3000000};
    assert(averageOfRange(large, large + 3) == 2000000.0);

    // Test with float array (implicit conversion to double)
    float floats[] = {1.5f, 2.5f, 3.0f};
    assert(averageOfRange(floats, floats + 3) == 2.3333333333333335);
}
