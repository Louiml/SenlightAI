Given a whitespace-separated list of integers read from a settings file (simulated by an input string in this task), write a C++ function that parses the string and returns a comma-separated string containing: the first integer, then the sum of all integers, then the product of all integers (as a `long long` to avoid overflow for reasonable inputs), and finally the number of integers. The input may have leading/trailing/multiple spaces, and may be empty (in which case return `"0,0,0,0"`). The integers are guaranteed to fit within `int` range, but the product may exceed `int`, so use `long long`. Example: for `"3 -1 4"` the output should be `"3,6,-12,3"` because first=3, sum=3+(-1)+4=6, product=3*(-1)*4=-12, count=3.

// The solution uses a `std::istringstream` to extract integers one by one. Initialize variables: `first` as an optional (we need to know if any integer was read), `sum` as `long long` starting at 0, `product` as `long long` starting at 1, and `count` as `int` starting at 0. For each successfully extracted integer, if it's the first one, store it; otherwise continue. Add to sum, multiply product, and increment count. After all extractions, if count==0, return `"0,0,0,0"`; otherwise return a formatted string using `std::to_string` for each component. Edge cases: empty or whitespace-only strings; the first integer cannot be determined until we read at least one; product of zero integers is undefined so we return 0; negative numbers work naturally with integer arithmetic. Time complexity is O(n) where n is the number of integers (since each extraction is O(1) per integer, plus string tokenization). Space complexity is O(1) auxiliary, excluding the output string. Use `const` on the input parameter and mark the function `static` or put it in a namespace if desired.

#include <sstream>
#include <string>

// Parses a whitespace-separated string of integers and returns a comma-separated
// summary: first,sum,product,count. If input is empty, returns "0,0,0,0".
std::string summarizeIntegers(const std::string& input) {
    std::istringstream iss(input);
    int value;
    bool hasAny = false;
    int first = 0;
    long long sum = 0;
    long long product = 1;
    int count = 0;

    while (iss >> value) {
        if (!hasAny) {
            first = value;
            hasAny = true;
        }
        sum += value;
        product *= value;
        ++count;
    }

    if (count == 0) {
        return "0,0,0,0";
    }
    return std::to_string(first) + "," + std::to_string(sum) + ","
           + std::to_string(product) + "," + std::to_string(count);
}

#include <cassert>
#include <string>
#include <sstream>

// The function from Solution section is assumed to be defined above.
// For brevity, it's not repeated here.

int main() {
    // Basic mixed positive/negative
    assert(summarizeIntegers("3 -1 4") == "3,6,-12,3");
    // Single integer
    assert(summarizeIntegers("7") == "7,7,7,1");
    // All zeros
    assert(summarizeIntegers("0 0 0") == "0,0,0,3");
    // Leading/trailing/multiple spaces
    assert(summarizeIntegers("  10   -2  8  0  ") == "10,16,0,4");
    // Negative product case
    assert(summarizeIntegers("-5 2") == "-5,-3,-10,2");
    // Empty input
    assert(summarizeIntegers("   ") == "0,0,0,0");
    // Larger product (requires long long)
    assert(summarizeIntegers("1000 1000 1000") == "1000,3000,1000000000,3");
    // Duplicate values
    assert(summarizeIntegers("2 2 2") == "2,6,8,3");
    // Mixed signs with product zero
    assert(summarizeIntegers("1 -1 0 5") == "1,5,0,4");
    // Large integers (near int max)
    assert(summarizeIntegers("2147483647 -2147483648") == "2147483647,-1,-4611686016279904256,2");
    return 0;
}
