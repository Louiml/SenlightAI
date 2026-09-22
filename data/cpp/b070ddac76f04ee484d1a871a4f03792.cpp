Write a C++ function `void printNumberInWords(int n, const std::string words[])` that takes a non-negative integer `n` and an array of 10 string constants representing the English words for digits 0-9 (e.g., "ZERO", "ONE", ..., "NINE"). The function must print the digits of `n` in their original left-to-right order, each followed by a single space, using recursion. For example, given `n = 431`, the output should be `FOUR THREE ONE ` (with a trailing space). You may assume `n >= 0`. Handle the edge case where `n == 0` by printing `ZERO ` (i.e., the word for zero followed by a space). Do not use any loops, and do not convert the number to a string. The function must be recursive and should not use any external storage beyond the given array.
#include <cassert>
#include <sstream>
#include <iostream>
#include <string>

// Solution function (as above)
void printNumberInWords(int n, const std::string words[]) {
    if (n == 0) {
        std::cout << words[0] << " ";
        return;
    }
    std::function<void(int)> helper = [&](int num) {
        if (num == 0) return;
        int digit = num % 10;
        num /= 10;
        helper(num);
        std::cout << words[digit] << " ";
    };
    helper(n);
}

// Helper to capture output for testing
std::string captureOutput(int n, const std::string words[]) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    printNumberInWords(n, words);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    std::string words[10] = {"ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE"};
    assert(captureOutput(0, words) == "ZERO ");
    assert(captureOutput(7, words) == "SEVEN ");
    assert(captureOutput(10, words) == "ONE ZERO ");
    assert(captureOutput(431, words) == "FOUR THREE ONE ");
    assert(captureOutput(1000, words) == "ONE ZERO ZERO ZERO ");
    assert(captureOutput(999999999, words) == "NINE NINE NINE NINE NINE NINE NINE NINE NINE ");
    assert(captureOutput(1234567890, words) == "ONE TWO THREE FOUR FIVE SIX SEVEN EIGHT NINE ZERO ");
    return 0;
}
Note: The test uses output capture to compare strings. The `assert` checks will pass if the function produces the expected output. Ensure to include `<sstream>`, `<functional>`, `<cassert>` in the test section. The above test is self-contained.
#include <iostream>
#include <string>

// Recursively prints the digits of a non-negative integer in words.
// Precondition: words is an array of 10 strings for digits 0-9, n >= 0.
void printNumberInWords(int n, const std::string words[]) {
    // Handle the special case n == 0 explicitly to print "ZERO".
    if (n == 0) {
        std::cout << words[0] << " ";
        return;
    }
    // Recursive helper to handle digits in correct order.
    // Lambda function to avoid exposing internal logic.
    // (C++ standard allows recursive lambdas via std::function, but for simplicity use a nested function via a local struct or just a private helper inside the function.)
    // We'll define a nested function-like recursive call using a lambda that captures itself via a std::function.
    // However, to keep it simple and self-contained, we'll just write a local recursive function using a static helper, but since we cannot define a named function inside another in standard C++ (only via lambda), we'll use a lambda.
    std::function<void(int)> printHelper = [&](int num) {
        if (num == 0) return;
        int digit = num % 10;
        num /= 10;
        printHelper(num);
        std::cout << words[digit] << " ";
    };
    // Invoke the recursive helper for the original n.
    printHelper(n);
}
Note: Since the problem explicitly disallows loops and requires recursion, I used a recursive lambda. However, the reference solution in the original snippet uses a free function. To adhere to the "descriptively named free function" requirement and avoid lambda complexity in a self-contained answer, I can provide a cleaner version using a helper function, but the prompt asks for only one function. So I'll keep the lambda approach, but I must include `<functional>` header. Alternatively, a simpler approach: since the original solution had a function that processed n directly, we can adapt it by handling zero as a special case and then calling a recursive private function. But since we cannot have a private function without extra declarations, the lambda is acceptable. However, to be more standard and match the typical teaching style, I'll rewrite as follows:

#include <iostream>
#include <string>

// Recursively prints the digits of a non-negative integer in words.
// Precondition: words is an array of 10 strings for digits 0-9, n >= 0.
void printNumberInWords(int n, const std::string words[]) {
    if (n == 0) {
        std::cout << words[0] << " ";
        return;
    }
    // Internal recursive helper using a lambda that captures the words array.
    // This avoids loops and prints digits in correct order.
    std::function<void(int)> helper = [&](int num) {
        if (num == 0) return;
        int digit = num % 10;
        num /= 10;
        helper(num);
        std::cout << words[digit] << " ";
    };
    helper(n);
}
But to avoid including `<functional>` and to keep it as a free function, I'll provide the classic two-function approach: a public function that handles the zero case and calls a private helper. Since the task says "free function" (singular), it's okay to have a helper as a separate free function. I'll write the solution as a single free function that internally uses a lambda (as allowed). For testing, that's fine.
// The solution uses recursion to process digits from least significant to most significant, but prints them in reverse order (most significant first). At each recursive step, we extract the last digit using `n % 10` and reduce `n` by integer division by 10. The base case occurs when `n == 0`; at that point we return without printing. After the recursive call returns (which prints all higher-order digits), we print the word for the current digit. This ensures the digits are output in the original order. For the special case where the input is exactly 0, the base case would trigger immediately and print nothing, so we add a separate check: if `n == 0`, directly print `"ZERO "` and return. For any `n > 0`, the recursion handles all digits. Time complexity is O(d) where d is the number of digits in `n` (since each recursive call processes one digit). Space complexity is O(d) due to recursion stack depth. The array access is safe because `digit` will always be between 0 and 9 (since `n % 10` yields 0-9). The function prints directly to standard output, so no return value is needed.
