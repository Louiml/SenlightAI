/*
Write a C++ function that reads exactly four floating-point values from standard input, stores them in a global vector (or a fixed-size array) with indices 0 through 3, accumulates their sum into a separate global variable (or index 4 of the array), and returns the minimum of the four values. The function must use recursion to process the values: it should read each value, update the running sum, then recursively process the next index, and on the way back up, compare the current value with the minimum returned from the recursive call to determine the overall minimum. The function should handle exactly four inputs, and if fewer or more than four values are provided, behavior is undefined (but you may assume valid input for the task). After the function returns, the program should print the minimum value (with two decimal places) and the total sum (with two decimal places). The main logic must be encapsulated in the recursive function; a global array and global sum variable are acceptable. Ensure the code compiles and runs correctly with standard C++ headers.
*/
#include <cstdio>

// Global array to hold up to 4 input values plus a sum slot.
float values[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // indices 0-3 for data, index 4 for sum
float sum = 0.0f; // Alternative: could use values[4] as sum, but separate variable is clearer

// Recursively read values from index i, accumulate sum, and return minimum from index i onward.
float readAndFindMinRecursive(int index) {
    // Read the value at the current index
    scanf("%f", &values[index]);
    sum += values[index];
    
    float currentMin = values[index];
    
    // If not the last element, get the minimum of the rest recursively
    if (index < 3) {
        float restMin = readAndFindMinRecursive(index + 1);
        if (restMin < currentMin) {
            currentMin = restMin;
        }
    }
    
    return currentMin;
}

// Public function that resets and runs the recursive reader, then returns the minimum.
float readFourValuesAndGetMinimum() {
    sum = 0.0f;
    return readAndFindMinRecursive(0);
}
#include <cassert>
#include <sstream>
#include <iostream>

// Assume the function is declared as above.
float readFourValuesAndGetMinimum();

// Helper to redirect stdin for testing
void setInput(const std::string& input) {
    static std::string data;
    data = input;
    freopen(NULL, "r", stdin); // Not portable; better to use stringstream redirection
    // For simplicity in test, we'll use a temporary file or just test manually.
    // But to keep it simple, we'll directly call the function after setting global sum.
    // Since the function reads from stdin, we'll use a global stringstream:
    extern std::istringstream testInput;
}

// We can't easily mock stdin in standard C++ without OS-specific hacks.
// So we'll just provide a manual test harness that calls the function with known input.
// For a real test, we would redirect stdin. Here we provide asserts that would work if
// stdin were redirected correctly. Since this is a self-contained test section, we
// provide a simple main that demonstrates correctness by testing the logic via a helper.

int main() {
    // We can't easily capture stdin in a portable way, so we'll simulate by
    // writing a separate function that takes a vector. But the task requires reading from stdin.
    // Instead, we provide a test that uses freopen on a temp file.
    // For illustration, we'll use a static buffer approach.
    
    // We'll define a replacement that reads from a string stream.
    // Since we cannot change the function signature, we'll use a file.
    // For brevity, we'll test with manual input from a file.
    // Create a temporary file with "1 2 3 4"
    FILE* f = tmpfile();
    fputs("1 2 3 4", f);
    rewind(f);
    fseek(f, 0, SEEK_SET);
    // Redirect stdin to this file
    FILE* old = stdin;
    stdin = f;
    
    float min = readFourValuesAndGetMinimum();
    assert(min == 1.0f);
    // Check sum (we need to expose sum, but it's global)
    extern float sum;
    assert(sum == 10.0f);
    
    // Test 2: negatives and duplicates
    f = tmpfile();
    fputs("-2.5 -2.5 3.0 0.0", f);
    rewind(f);
    fseek(f, 0, SEEK_SET);
    stdin = f;
    min = readFourValuesAndGetMinimum();
    assert(min == -2.5f);
    assert(sum == -2.0f); // -2.5 -2.5 +3.0 +0.0 = -2.0
    
    // Test 3: all same
    f = tmpfile();
    fputs("7 7 7 7", f);
    rewind(f);
    fseek(f, 0, SEEK_SET);
    stdin = f;
    min = readFourValuesAndGetMinimum();
    assert(min == 7.0f);
    assert(sum == 28.0f);
    
    // Restore stdin
    stdin = old;
    printf("All tests passed.\n");
    return 0;
}
// The solution uses a recursive function `readAndFindMin(int index, float values[])` that expects `index` to be 0 initially. At each call, it reads a float into `values[index]`, adds that value to a global sum variable `totalSum`, and if `index < 3`, it recursively calls itself with `index+1` to get the minimum of the remaining elements. After the recursive call returns (or immediately for the last element), the function returns the smaller of the current `values[index]` and the recursively obtained minimum. This way, the minimum propagates upward through the recursion. The base case is when `index == 3`, where it simply returns `values[3]` (no recursive call). Edge cases include negative values and duplicate values; both are handled naturally by the comparison. The algorithm reads exactly four floats and performs one comparison per element after the first, so time complexity is O(n) where n=4, and space complexity is O(n) for the recursion stack (maximum depth 4) plus O(1) for the global sum.
