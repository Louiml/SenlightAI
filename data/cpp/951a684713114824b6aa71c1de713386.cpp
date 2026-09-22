Write a C++ function named `sumOfDivisors` that takes a single positive integer `n` and returns the sum of all its positive divisors (including 1 and `n` itself). Additionally, the function must print each divisor followed by `" e delitel."` to the standard output in increasing order, one per line. If `n` is less than 1, the function should return 0 and print nothing. The input value will be read from the user in a separate program; your function only needs to perform the computation and printing. The function must be `const`-correct, meaning it does not modify any external state and takes its parameter by value.

The main algorithm iterates from 1 to `n` and checks if `i` divides `n` exactly (i.e., `n % i == 0`). For each divisor found, it prints that divisor and adds it to a running sum. After the loop, the function returns the total sum. Edge cases: if `n` is 0 or negative, the loop condition naturally fails, but to be safe we explicitly return 0 and skip printing for invalid inputs. For `n = 1`, the only divisor is 1, so the sum is 1 and it prints one line. The time complexity is O(n) because we check every integer from 1 to `n`. The space complexity is O(1) since we only use a few integer variables and no additional data structures grow with input size. The output printing is a side effect, but the primary return value is the sum.

#include <iostream>

// Return the sum of all positive divisors of n, and print each divisor to std::cout.
int sumOfDivisors(int n) {
    if (n < 1) {
        return 0;
    }
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) {
            std::cout << i << " e delitel." << std::endl;
            sum += i;
        }
    }
    return sum;
}

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the solution function.
int sumOfDivisors(int n);

int main() {
    // Capture output to verify printing behavior without polluting test output.
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Test 1: n = 6, divisors 1,2,3,6; sum = 12
    int result1 = sumOfDivisors(6);
    std::cout.rdbuf(old);
    assert(result1 == 12);
    assert(buffer.str() == "1 e delitel.\n2 e delitel.\n3 e delitel.\n6 e delitel.\n");
    buffer.str("");

    // Test 2: n = 1, only divisor 1; sum = 1
    std::cout.rdbuf(buffer.rdbuf());
    int result2 = sumOfDivisors(1);
    std::cout.rdbuf(old);
    assert(result2 == 1);
    assert(buffer.str() == "1 e delitel.\n");
    buffer.str("");

    // Test 3: n = 10, divisors 1,2,5,10; sum = 18
    std::cout.rdbuf(buffer.rdbuf());
    int result3 = sumOfDivisors(10);
    std::cout.rdbuf(old);
    assert(result3 == 18);
    assert(buffer.str() == "1 e delitel.\n2 e delitel.\n5 e delitel.\n10 e delitel.\n");
    buffer.str("");

    // Test 4: n = 0 (invalid), sum = 0, no output
    std::cout.rdbuf(buffer.rdbuf());
    int result4 = sumOfDivisors(0);
    std::cout.rdbuf(old);
    assert(result4 == 0);
    assert(buffer.str().empty());
    buffer.str("");

    // Test 5: n = -3 (invalid), sum = 0, no output
    std::cout.rdbuf(buffer.rdbuf());
    int result5 = sumOfDivisors(-3);
    std::cout.rdbuf(old);
    assert(result5 == 0);
    assert(buffer.str().empty());

    return 0;
}
