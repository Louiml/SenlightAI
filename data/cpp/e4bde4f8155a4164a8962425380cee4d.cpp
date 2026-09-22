Write a C++ function `int kaprekarStep(int num)` that takes a positive 4-digit integer (between 1000 and 9999 inclusive) and returns the number of iterations required to reach the Kaprekar constant 6174 using the standard routine: arrange the four digits in descending and ascending order, subtract the ascending from the descending, and repeat with the result. The function must handle leading zeros in the subtraction result (e.g., for input 1000, the first step yields 1000 - 0001 = 0999, which becomes 999 as an integer). You may assume the input is always a valid 4-digit number (no need to validate). The function must not print anything; it must only perform the calculation and return the iteration count. For example, for input 3524, the sequence is: 5432 - 2345 = 3087 (1), 8730 - 0378 = 8352 (2), 8532 - 2358 = 6174 (3), so the function returns 3. For input 6174, the function should return 0. The function must work for any valid 4-digit input, including numbers with repeated digits (e.g., 1111, but note that such inputs will never reach 6174 because they produce 0; you must handle this by returning a sentinel value like -1 if the process enters a cycle that does not include 6174). For simplicity, you may assume the input will always eventually reach 6174, but the implementation must include a safety mechanism (e.g., a maximum iteration cap) to avoid infinite loops.

#include <cassert>

int main() {
    assert(kaprekarStep(6174) == 0);
    assert(kaprekarStep(3524) == 3);
    assert(kaprekarStep(1000) == 5); // 1000->999->8991->8082->8532->6174
    assert(kaprekarStep(1111) == -1); // all identical digits never reach 6174
    assert(kaprekarStep(1234) == 3); // 4321-1234=3087, 8730-0378=8352, 8532-2358=6174
    return 0;
}

#include <array>
#include <algorithm>

// Return the number of Kaprekar iterations needed to reach 6174,
// or -1 if a cycle not containing 6174 is detected (e.g., all digits equal).
int kaprekarStep(int num) {
    const int target = 6174;
    const int maxIterations = 100; // safety cap; valid inputs converge in <=7 steps
    int iterations = 0;

    while (num != target && iterations < maxIterations) {
        std::array<int, 4> digits;
        digits[0] = num % 10;
        digits[1] = (num / 10) % 10;
        digits[2] = (num / 100) % 10;
        digits[3] = (num / 1000) % 10;

        std::sort(digits.begin(), digits.end());

        int asc = digits[0] * 1000 + digits[1] * 100 + digits[2] * 10 + digits[3];
        int desc = digits[3] * 1000 + digits[2] * 100 + digits[1] * 10 + digits[0];

        int next = desc - asc;
        if (next == num) { // cycle without change (e.g., 1111 -> 0, but 0 is not reached; this catches no-change)
            return -1;
        }
        num = next;
        ++iterations;
    }

    if (num == target) {
        return iterations;
    }
    return -1; // exceeded cap or entered a cycle not containing target
}

// The solution uses a loop that repeats the Kaprekar operation until the number equals 6174 or a maximum iteration limit (e.g., 100) is reached to prevent infinite loops (though mathematically all valid 4-digit numbers except those with all identical digits reach 6174). In each iteration, extract the four digits using integer division and modulo (num % 10, (num/10)%10, etc.), then sort them in ascending order using a simple bubble sort (as in the snippet) or any small fixed-size sort. Compute the ascending number by combining digits in sorted order with place values (thousands, hundreds, tens, ones), and the descending number by reversing the sorted order. Subtract ascending from descending; note that for numbers like 1000, the subtraction yields a 4-digit result with a leading zero, but when stored as an int it becomes 999. The iteration count increments each time. Handle the case where the result is 0 (e.g., input 1111) by returning -1 after the loop detects the value hasn't changed or the cap is reached. Time complexity is O(k * 4 log 4) where k is the number of iterations (at most ~7 for valid inputs, but capped at 100), so effectively constant time; space complexity is O(1) as only a fixed-size array and a few integers are used.
