Write a C++ function `averageProcessedValues` that reads a sequence of non-negative `double` values from the user via standard input until a negative sentinel value is entered, then returns the average of all entered non-negative values. The function must repeatedly prompt the user with `"Enter a +ve or (-ve to quit): "` before each reading, process each value immediately (accumulating the sum and count), and after the loop terminates, output the count, sum, and average in the format `"You have entered {count} values.\nSum = {sum}\nAverage = {avg}\n"` to standard output, and return the average as a `double`. Assume the user enters at least one valid non-negative value before the sentinel, and that all inputs are representable as `double`. The function should be self-contained, not relying on any global state.
The algorithm uses a `while` loop with a sentinel-controlled pattern. It reads the first value before the loop, and because the loop condition checks `value >= 0`, each iteration processes the current value by adding it to `sum` and incrementing `count`, then reads the next value at the end of the loop body. This sentinel pattern ensures that a negative value terminates the loop without being processed. Edge cases: If the user immediately enters a negative number, the loop never executes, but the problem guarantees at least one valid entry, so dividing by zero is avoided. However, to be robust in a general implementation, one could guard against `count == 0`, but for this task, we can assume valid input. The average is computed as `sum / count` using floating-point division. Time complexity is O(n), where n is the number of non-negative inputs, because each value is read and processed once. Space complexity is O(1) auxiliary, as only a few scalar variables are used.
#include <iostream>

// Reads non-negative values until a negative sentinel, prints statistics,
// and returns the average of the non-negative values.
double averageProcessedValues() {
    double sum = 0.0;
    int count = 0;
    double value;

    // Prime the loop by reading the first value
    std::cout << "Enter a +ve or (-ve to quit): ";
    std::cin >> value;

    while (value >= 0) {
        sum += value;          // Process current value
        ++count;

        std::cout << "Enter a +ve or (-ve to quit): ";
        std::cin >> value;     // Read next value (sentinel check next iteration)
    }

    const double avg = sum / count;  // count >= 1 by problem assumption

    std::cout << "You have entered " << count << " values.\n";
    std::cout << "Sum = " << sum << "\n";
    std::cout << "Average = " << avg << "\n";

    return avg;
}
#include <cassert>
#include <sstream>

// Declare the solution function (normally included from header)
double averageProcessedValues();

// Helper to redirect cin and capture cout for testing
double runWithInput(const std::string& input) {
    std::streambuf* origCin = std::cin.rdbuf();
    std::streambuf* origCout = std::cout.rdbuf();
    std::istringstream iss(input);
    std::ostringstream oss;
    std::cin.rdbuf(iss.rdbuf());
    std::cout.rdbuf(oss.rdbuf());

    double result = averageProcessedValues();

    std::cin.rdbuf(origCin);
    std::cout.rdbuf(origCout);
    return result;
}

int main() {
    // Test 1: Basic positive values
    assert(runWithInput("1 2 3 -1") == 2.0);

    // Test 2: Single value
    assert(runWithInput("5 -1") == 5.0);

    // Test 3: All zeros
    assert(runWithInput("0 0 -1") == 0.0);

    // Test 4: Large values
    assert(runWithInput("1000000 2000000 -1") == 1500000.0);

    // Test 5: Decimal values
    assert(runWithInput("1.5 2.5 -1") == 2.0);

    // Test 6: Mixed positive and zero
    assert(runWithInput("0 10 20 -1") == 10.0);

    // Test 7: Values with scientific notation
    assert(runWithInput("1e3 2e3 -1") == 1500.0);

    // Test 8: Many values (verify no overflow in count for reasonable size)
    std::string manyInput = "";
    for (int i = 1; i <= 10; ++i) manyInput += std::to_string(i) + " ";
    manyInput += "-1";
    // Sum 1..10 = 55, count = 10 → avg = 5.5
    assert(runWithInput(manyInput) == 5.5);

    // Test 9: Immediate sentinel (still returns 0.0 because count=0? But assumption says at least one valid; will handle gracefully if implementation guards? Actually our solution divides by count=0 → UB, so we avoid this test as per problem constraints)

    // Test 10: Only one negative after positive
    assert(runWithInput("42 -1") == 42.0);

    return 0;
}
