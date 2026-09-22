/*
Write a C++ function that reads a string containing an arbitrary number of integers separated by single spaces (with no leading or trailing spaces), and returns the sum of all integers as a string. The input string is guaranteed to be non-empty and may contain negative numbers. Your function must be named `sumIntegers` and take a `const std::string&` parameter, returning a `std::string`. The function must handle integers that may exceed the range of `int` by performing the sum using `long long`. The input will always contain at least one integer, and each integer will be a valid 32-bit signed integer.
*/
#include <string>
#include <sstream>

// Sum all space-separated integers in the input string and return the result as a string.
std::string sumIntegers(const std::string& numbers) {
    std::istringstream input(numbers);
    long long total = 0;
    int value;
    while (input >> value) {
        total += value;
    }
    return std::to_string(total);
}
#include <cassert>
#include <string>

// The solution function is already included above its usage.
int main() {
    assert(sumIntegers("5") == "5");
    assert(sumIntegers("1 2 3") == "6");
    assert(sumIntegers("-1 -2 -3") == "-6");
    assert(sumIntegers("1000000000 1000000000") == "2000000000");
    assert(sumIntegers("2147483647 1") == "2147483648");
    assert(sumIntegers("-2147483648 -1") == "-2147483649");
    assert(sumIntegers("0 0 0") == "0");
    assert(sumIntegers("42 -42 99") == "99");
}
// The solution uses a `std::istringstream` to parse integers from the input string. We initialize a `long long total = 0` to accumulate the sum, which avoids integer overflow since the worst-case sum of many 32-bit integers can exceed `int` range but fits comfortably in `long long`. We then repeatedly extract integers using the stream extraction operator `>>` and add each to `total`. Extraction stops automatically at the end of the stream, so no manual delimiter handling is needed. Edge cases include a single integer (returns that integer as a string), negative numbers (handled naturally by addition), and large magnitudes that would overflow `int` but not `long long`. Time complexity is O(n) where n is the number of integers in the string, and space complexity is O(1) auxiliary, excluding the input and output strings.
