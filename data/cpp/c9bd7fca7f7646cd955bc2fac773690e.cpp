/*
Write a C++ function named `printElements` that takes a `const std::vector<T>&` (where `T` is a generic type) and prints each element on a separate line to `std::cout`. The function must be a template so it works with any data type that supports the output stream operator `<<`. The printing format is exactly one element per line, with no extra spaces or blank lines. The function should return `void` and avoid modifying the input vector (use `const` reference). The function should work correctly for vectors of fundamental types (e.g., `int`, `double`) and standard library types (e.g., `std::string`). Assume the vector is non-empty but handle edge cases like a vector with `0` elements gracefully (print nothing). The function must not use global variables or rely on any external state besides `std::cout`.
*/
#include <iostream>
#include <vector>

/**
 * Prints each element of a generic vector on a new line.
 * @tparam T The type of elements in the vector.
 * @param vec A const reference to the vector to print.
 */
template<typename T>
void printElements(const std::vector<T>& vec) {
    for (const auto& element : vec) {
        std::cout << element << '\n';
    }
}
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

// Forward declaration of the solution function (replace with actual include if needed)
template<typename T>
void printElements(const std::vector<T>& vec);

int main() {
    // Helper to capture output
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());

    // Test 1: vector of integers
    std::vector<int> ints = {1, 2, 3};
    printElements(ints);
    assert(buffer.str() == "1\n2\n3\n");
    buffer.str(""); buffer.clear();

    // Test 2: vector of strings
    std::vector<std::string> strs = {"hello", "world"};
    printElements(strs);
    assert(buffer.str() == "hello\nworld\n");
    buffer.str(""); buffer.clear();

    // Test 3: empty vector
    std::vector<double> empty;
    printElements(empty);
    assert(buffer.str() == "");
    buffer.str(""); buffer.clear();

    // Test 4: vector with a single element
    std::vector<char> single = {'Z'};
    printElements(single);
    assert(buffer.str() == "Z\n");
    buffer.str(""); buffer.clear();

    // Test 5: vector with duplicate values
    std::vector<int> duplicates = {5, 5, 5};
    printElements(duplicates);
    assert(buffer.str() == "5\n5\n5\n");
    buffer.str(""); buffer.clear();

    // Test 6: vector with negative numbers
    std::vector<int> negatives = {-1, -2, -3};
    printElements(negatives);
    assert(buffer.str() == "-1\n-2\n-3\n");
    buffer.str(""); buffer.clear();

    // Test 7: vector of doubles
    std::vector<double> doubles = {3.14, 2.718};
    printElements(doubles);
    assert(buffer.str() == "3.14\n2.718\n");
    buffer.str(""); buffer.clear();

    // Restore original cout
    std::cout.rdbuf(old_cout);

    return 0;
}
// The solution is straightforward: define a function template with a single template parameter `typename T`. The function accepts a `const std::vector<T>&` to avoid copying and to prevent modification. Then iterate over the vector using a range-based `for` loop (or an index loop). For each element, output it to `std::cout` followed by a newline (`'\n'` or `std::endl` — `'\n'` is preferred to avoid unnecessary flushing). The generic type `T` does not require any special constraints because the only operation used is `operator<<` which is already defined for all built‑in types and many standard types. Edge cases: an empty vector results in zero output lines; the loop simply does not execute. Duplicate values are printed as they appear. Time complexity is \(O(n)\) because each element is visited exactly once. Space complexity is \(O(1)\) auxiliary space because no extra data structures are created; the function uses the existing vector (by reference).
