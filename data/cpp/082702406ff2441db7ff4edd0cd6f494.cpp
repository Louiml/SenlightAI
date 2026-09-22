Write a C++ function named `arrayRank` that takes no arguments and returns a `std::string` representing the rank (number of dimensions) of a fixed-size array type, using the `std::rank` trait. The function should output, in order, the rank of `int[10]`, `char[10][10]`, and `std::string[10][10][10]`, each followed by a newline. The function must use the `<type_traits>` header and return a single string with the three ranks separated by newlines. Ensure the function is `const`-qualified (though it doesn't access mutable state) and uses `std::rank<T>::value` to obtain each rank. The expected output is:
```
1
2
3
```
// The task requires using the C++ standard library type trait `std::rank<T>`, which provides a static member `value` equal to the number of dimensions of array type `T`. For a non-array type, `value` is 0. For `int[10]`, it’s 1; for `char[10][10]` (a 2D array), it’s 2; for `std::string[10][10][10]` (a 3D array), it’s 3. The solution simply constructs a string by concatenating the three values with `\n` separators. Since `std::to_string` is available from `<string>`, we convert each `size_t` value to a string. The main algorithm is trivial: evaluate each `std::rank<T>::value`, convert to string, and concatenate. Edge cases: none, as the types are fixed and known at compile time. Time complexity is O(1) since it’s purely compile-time computation and string construction of constant size. Space complexity is O(1) for the returned string (constant length). The function should be `const` because it does not modify any state, and it returns a `std::string` by value.
#include <string>
#include <type_traits>

// Return a string containing the ranks of three fixed array types, one per line.
std::string arrayRank() const {
    std::string result;
    result += std::to_string(std::rank<int[10]>::value) + "\n";
    result += std::to_string(std::rank<char[10][10]>::value) + "\n";
    result += std::to_string(std::rank<std::string[10][10][10]>::value) + "\n";
    return result;
}
#include <cassert>
#include <string>

// Declaration for testing (normally provided by the solution)
std::string arrayRank();

int main() {
    assert(arrayRank() == "1\n2\n3\n");
    // Additional checks to ensure correctness: the string is well-formed and ends with newline.
    assert(arrayRank().front() == '1');
    assert(arrayRank().back() == '\n');
    assert(arrayRank().find('\n') != std::string::npos);
    assert(arrayRank().size() == 6); // "1\n2\n3\n" has length 6
}
