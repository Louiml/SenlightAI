Write a C++ function named `classifyAndSummarize` that takes a single integer parameter `n`. The function must return a `std::string` containing the following information in this exact order, each on a new line: first, whether the number is "POSITIVO", "NEGATIVO", or "CERO" (exactly those uppercase words); second, either "El numero es mayor a 10" if `n > 10` or "El numero no es mayor a 10" if `n <= 10`; third, the sum of all integers from 0 to 9 (which is 45) if `n` is positive, or the count of integers from 0 to 9 that are strictly less than `n` if `n` is negative or zero (note: for negative `n`, the count is 0 because no numbers in [0,9] are less than a negative number; for `n=0`, the count is also 0 since 0 is not strictly less than 0). The function should handle boundary cases including very large positive and negative integers. The output must be formatted with each line terminated by a newline character (`\n`). The function must be `const`-correct (i.e., take the parameter by value or const reference, and not modify it) and return a `std::string` built using a `std::ostringstream` or string concatenation. Do not read from standard input or write to standard output inside the function.
// The solution approach is straightforward: check the sign of the input to determine the first line. Then evaluate the condition `n > 10` to determine the second line. For the third line, if `n > 0`, the sum of integers 0 through 9 is a constant 45, so we can return that directly. If `n <= 0`, we need to count how many integers in the range [0, 9] are strictly less than `n`. Since `n` is non-positive, no non-negative integer can be less than it, so the count is always 0. Therefore, the third line is always either "45" or "0". Edge cases include `n = 10` (second line says "not greater"), `n = -100` (count is 0), `n = 0` (first line "CERO", third line "0"), and `n` being very large (no overflow issues because we only output constants). Time complexity is O(1) because all operations are constant-time conditionals and string construction. Space complexity is O(1) auxiliary, aside from the returned string itself (which is of fixed small size).
#include <string>
#include <sstream>

// Classifies an integer and produces a three-line summary string.
// Line 1: POSITIVO, NEGATIVO, or CERO
// Line 2: "El numero es mayor a 10" or "El numero no es mayor a 10"
// Line 3: sum of 0..9 (45) if positive, else count of 0..9 less than n (0)
std::string classifyAndSummarize(const int n) {
    std::ostringstream out;
    
    // First line: sign classification
    if (n > 0) {
        out << "POSITIVO\n";
    } else if (n < 0) {
        out << "NEGATIVO\n";
    } else {
        out << "CERO\n";
    }
    
    // Second line: comparison with 10
    if (n > 10) {
        out << "El numero es mayor a 10\n";
    } else {
        out << "El numero no es mayor a 10\n";
    }
    
    // Third line: constant result based on sign
    if (n > 0) {
        out << "45\n";  // sum of 0 through 9 is always 45
    } else {
        out << "0\n";   // no non-negative number is strictly less than n <= 0
    }
    
    return out.str();
}
#include <cassert>
#include <string>

// (The solution function is assumed to be available above.)
int main() {
    // Test positive number > 10
    assert(classifyAndSummarize(11) == "POSITIVO\nEl numero es mayor a 10\n45\n");
    
    // Test positive number exactly 10 (not > 10)
    assert(classifyAndSummarize(10) == "POSITIVO\nEl numero no es mayor a 10\n45\n");
    
    // Test negative number
    assert(classifyAndSummarize(-5) == "NEGATIVO\nEl numero no es mayor a 10\n0\n");
    
    // Test zero
    assert(classifyAndSummarize(0) == "CERO\nEl numero no es mayor a 10\n0\n");
    
    // Test very large negative
    assert(classifyAndSummarize(-123456789) == "NEGATIVO\nEl numero no es mayor a 10\n0\n");
    
    // Test very large positive
    assert(classifyAndSummarize(999999999) == "POSITIVO\nEl numero es mayor a 10\n45\n");
    
    // Test small positive
    assert(classifyAndSummarize(1) == "POSITIVO\nEl numero no es mayor a 10\n45\n");
    
    // Test exact boundary negative near zero
    assert(classifyAndSummarize(-1) == "NEGATIVO\nEl numero no es mayor a 10\n0\n");
    
    // Test positive equal to 1 (small)
    assert(classifyAndSummarize(2) == "POSITIVO\nEl numero no es mayor a 10\n45\n");
    
    return 0;
}
