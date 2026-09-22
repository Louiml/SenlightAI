/*
Write a C++ function `repeatedCharacter` that takes a single lowercase letter and a positive integer `n` as parameters, and returns a string containing that letter repeated `n` times. The function must handle the edge case where `n` is 0 by returning an empty string, and it must work correctly for any lowercase letter from `'a'` to `'z'`. Additionally, the function must be `const`-correct and use only standard C++ libraries without external dependencies.
*/
#include <string>

// Returns a string consisting of the given character repeated `count` times.
// If `count` is 0, an empty string is returned.
std::string repeatedCharacter(const char ch, const int count) {
    // Using the std::string constructor that takes a count and a character
    // is both clear and efficient, handling the count == 0 case inherently.
    return std::string(count, ch);
}
#include <cassert>
#include <string>

// Declaration (must match the solution exactly)
std::string repeatedCharacter(const char ch, const int count);

int main() {
    // Basic repetition
    assert(repeatedCharacter('a', 5) == "aaaaa");
    
    // Single repetition
    assert(repeatedCharacter('z', 1) == "z");
    
    // Zero repetitions returns empty string
    assert(repeatedCharacter('b', 0) == "");
    
    // Different letters and counts
    assert(repeatedCharacter('m', 3) == "mmm");
    assert(repeatedCharacter('x', 7) == "xxxxxxx");
    
    // Larger count
    assert(repeatedCharacter('q', 10) == "qqqqqqqqqq");
    
    // Edge: lowercase letter at boundary
    assert(repeatedCharacter('a', 2) == "aa");
    assert(repeatedCharacter('z', 2) == "zz");
    
    // Ensure function is const-correct (callable on const contexts, no modification of inputs)
    const char c = 'k';
    const int n = 4;
    std::string result = repeatedCharacter(c, n);
    assert(result == "kkkk");
    
    return 0;
}
// The solution approach is straightforward: create a result string and append the given character `n` times using a loop or the `std::string` constructor with count and character arguments. The key edge case is `n == 0`, which should return an empty string. Since the problem guarantees a lowercase letter, no validation is required, but for robustness, the function could check the range. The time complexity is O(n) because the string must contain `n` characters, and the space complexity is O(n) for the result string itself. Using `std::string(n, ch)` is the most efficient and idiomatic way, as it pre-allocates the exact required memory.
