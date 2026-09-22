// Write a C++ function named `greetSpecialCourse` that takes no arguments and returns a `std::string` containing the exact message `"Hello PTIT."` (including the period, no trailing newline). The function must be const-correct and self-contained, using only standard library facilities. The purpose is to practice defining a simple function with proper return type and string handling, matching the behavior of the provided snippet's core output. The function should not print to the console; it should only construct and return the string so that it can be tested programmatically.
#include <cassert>
#include <string>

// Declare the function to test (assumed to be provided in the solution section)
std::string greetSpecialCourse();

int main() {
    // Exact match test
    assert(greetSpecialCourse() == "Hello PTIT.");
    
    // Test that it is not empty and contains the required substring
    std::string result = greetSpecialCourse();
    assert(!result.empty());
    assert(result.find("Hello") == 0);
    assert(result.find("PTIT") != std::string::npos);
    
    // Ensure it ends with a period and has no extra whitespace
    assert(result.back() == '.');
    assert(result.find('\n') == std::string::npos);
    assert(result.find(' ') != std::string::npos);
    
    // Verify the exact length (12 characters: "Hello PTIT." has 11 letters + 1 period? Actually count)
    // Count: H e l l o (5) + space (1) = 6, P T I T (4) = 10, period (1) = 11 total.
    assert(result.size() == 11);
    
    return 0;
}
#include <string>

// Returns the fixed greeting message "Hello PTIT."
std::string greetSpecialCourse() {
    return "Hello PTIT.";
}
// The task is straightforward: the required output is a fixed constant string. The function should simply create a `std::string` with the exact characters `"Hello PTIT."` and return it. There are no edge cases because the output is deterministic and independent of any input. The main challenge is ensuring the string literal is exactly correct (uppercase `H`, space, `PTIT` with capital letters, and a final period). Time complexity is O(1) because we are returning a constant string; space complexity is also O(1) as no additional dynamic data structures are used beyond the returned string's internal buffer. Const-correctness is achieved by declaring the function as returning `std::string` by value (which is efficient with move semantics) and taking no mutable parameters. The function has no side effects, making it pure.
