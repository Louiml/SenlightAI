/*
Given an array of four strings representing products (e.g., `"apple"`, `"plum"`, `"tomato"`, `"potato"`) and an associated array of four integer codes, write a C++ function `lookupProductCode` that accepts a single string `key` and returns a string containing the product's code as a decimal number (e.g., `"145"`) if the key exactly matches one of the four product names; otherwise, return the exact string `"Don't find"`. The lookup must be case-sensitive, and the function must handle the case where the input string is empty or contains whitespace, treating any non-exact match as "Don't find". The function should be `const`-correct regarding its inputs and should not modify any global state; it must be self-contained, not relying on external files or a prebuilt hash table—just a simple linear scan.
*/
#include <string>
#include <array>

// Given a key string, return the code for a matching product name,
// or "Don't find" if no exact match exists.
// Case-sensitive, fixed-size lookup (4 products).
std::string lookupProductCode(const std::string& key) {
    // Fixed parallel arrays of product names and codes.
    const std::array<std::string, 4> products = {"apple", "plum", "tomato", "potato"};
    const std::array<int, 4> codes = {145, 134, 78, 65};

    // Linear scan; return on first exact match.
    for (size_t i = 0; i < products.size(); ++i) {
        if (key == products[i]) {
            return std::to_string(codes[i]);
        }
    }
    return "Don't find";
}
#include <cassert>
#include <string>

// Declaration of the function under test (must match the solution above).
std::string lookupProductCode(const std::string& key);

int main() {
    // All four existing products.
    assert(lookupProductCode("apple") == "145");
    assert(lookupProductCode("plum") == "134");
    assert(lookupProductCode("tomato") == "78");
    assert(lookupProductCode("potato") == "65");

    // Non-matching keys.
    assert(lookupProductCode("banana") == "Don't find");
    assert(lookupProductCode("Apple") == "Don't find"); // case-sensitive
    assert(lookupProductCode("") == "Don't find");
    assert(lookupProductCode(" apple") == "Don't find"); // leading space
    assert(lookupProductCode("apple ") == "Don't find"); // trailing space
    assert(lookupProductCode("potato") == "65"); // repeated check

    return 0;
}
// The solution approach is a straightforward linear search over a fixed-size array of four strings. For each input key, compare it directly with each product name using `==`. On the first exact match, convert the corresponding integer code (from the parallel array) to a string using `std::to_string` and return it immediately. If all four comparisons fail, return the literal string `"Don't find"`. Edge cases include: an empty string, a string with leading/trailing spaces (which would not match any product name), and duplicate product names in the list (though not present in the given data, the function would return the first match if duplicates existed). Time complexity is \(O(1)\) because the array size is fixed at four, so at most four comparisons are made. Space complexity is \(O(1)\) as only a temporary string is created for the result. The implementation should not use any global counters or state; each call is independent.
