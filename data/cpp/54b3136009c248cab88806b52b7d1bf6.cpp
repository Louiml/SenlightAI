/*
Write a C++ function named `positiveDifference` that repeatedly reads pairs of integers from standard input using `scanf` until fewer than two integers can be successfully read in a single call. For each valid pair, the function should output the absolute difference of the two integers in a specific format: first print the larger number, then a minus sign, then the smaller number, an equals sign, and the absolute difference. If the two integers are equal, the output should print them in the order they were entered. The function should ignore any extra whitespace or newlines between inputs, and it must terminate cleanly when the input is exhausted or contains non-integer data (e.g., a letter). The function does not need to return a value; it only prints to standard output.
*/

#include <cstdio> // for printf, scanf

// Reads pairs of integers from stdin until fewer than two are available,
// printing the absolute difference in the format "larger - smaller = diff".
void positiveDifference() {
    int n1, n2, scanResult;
    while (true) {
        printf("# Enter two integers: ");
        scanResult = scanf("%d%d", &n1, &n2);
        if (scanResult < 2) {
            break;
        }
        int difference = n1 - n2;
        if (difference > 0) {
            printf("%d - %d = %d\n", n1, n2, difference);
        } else {
            printf("%d - %d = %d\n", n2, n1, -difference);
        }
    }
}

#include <cassert>
#include <cstdio>
#include <sstream>
#include <iostream>

// To test, we need to redirect stdin. We'll use a helper that feeds a string.
void runWithInput(const std::string& input) {
    // Save original stdin buffer
    FILE* original = stdin;
    // Create a temporary file or use fmemopen (not portable). Instead, we'll
    // use a pipe or redirect via freopen. For simplicity, we'll use sscanf
    // logic manually in a test harness. Since we must call the function
    // directly, we'll create a wrapper that reads from a string using sscanf.
    // But the task asks for a free function that uses scanf from stdin.
    // For testing, we can redirect stdin to a file. Use tmpfile.
    FILE* temp = tmpfile();
    fputs(input.c_str(), temp);
    rewind(temp);
    stdin = temp;
    // Call the function (it will read from the temp file)
    positiveDifference();
    // Restore stdin
    fclose(temp);
    stdin = original;
}

int main() {
    // Redirect stdout to a string to capture output
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    // But printf uses stdout, not cout. So we need to redirect stdout.
    // Use freopen to a temp file.
    FILE* outFile = tmpfile();
    FILE* oldStdout = stdout;
    stdout = outFile;

    // Test 1: two pairs
    runWithInput("10 3\n7 12\n");
    fflush(outFile);
    rewind(outFile);
    char output[256];
    fgets(output, sizeof(output), outFile);
    assert(std::string(output) == "# Enter two integers: 10 - 3 = 7\n");
    fgets(output, sizeof(output), outFile);
    assert(std::string(output) == "# Enter two integers: 12 - 7 = 5\n");

    // Test 2: invalid input "abc" after a valid pair
    stdout = oldStdout; // redirect back to real stdout for cleanup
    fclose(outFile);
    // Reset stdout to original for further tests? We'll just do another.
    // Simpler: use freopen on a file.
    // We'll do a second test with a file.
    FILE* out2 = fopen("test_out.txt", "w+");
    oldStdout = stdout;
    stdout = out2;
    // Input: "5 5\n" (equal numbers)
    runWithInput("5 5\n");
    fflush(out2);
    rewind(out2);
    fgets(output, sizeof(output), out2);
    assert(std::string(output) == "# Enter two integers: 5 - 5 = 0\n");
    fclose(out2);
    stdout = oldStdout;

    // Test 3: input "1 2 3" (last call has only one integer)
    FILE* out3 = fopen("test_out3.txt", "w+");
    oldStdout = stdout;
    stdout = out3;
    runWithInput("1 2 3\n");
    fflush(out3);
    rewind(out3);
    fgets(output, sizeof(output), out3);
    assert(std::string(output) == "# Enter two integers: 2 - 1 = 1\n");
    fclose(out3);
    stdout = oldStdout;

    // Test 4: no input at all
    FILE* out4 = fopen("test_out4.txt", "w+");
    oldStdout = stdout;
    stdout = out4;
    runWithInput("");  // empty input, scanf returns EOF
    fflush(out4);
    rewind(out4);
    assert(fgets(output, sizeof(output), out4) == nullptr); // no output
    fclose(out4);
    stdout = oldStdout;

    // Clean up any temp files? We'll skip.

    return 0;
}

// The core approach is to use a loop that repeatedly calls `scanf` to read exactly two integers. The return value of `scanf` tells us how many items were successfully matched. If it returns less than 2, we break out of the loop. Otherwise, we compute the absolute difference, determine which number is larger (or equal), and print the result. A key edge case is when `scanf` returns 1 (only one number read) or 0 (no numbers read), which indicates invalid or incomplete input; both cases should terminate the loop. Negative differences are handled by taking the absolute value. If the numbers are equal, the difference is zero, and either order is valid; we choose to print them in input order for determinism. The time complexity is O(n) where n is the number of pairs read, and the space complexity is O(1) because we only use a constant amount of variables.
