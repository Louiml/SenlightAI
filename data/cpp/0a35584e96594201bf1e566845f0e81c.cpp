// Write a C++ function named `classifyNumbers` that takes a `std::vector<int>` as input (by const reference) and returns a `std::string` summarizing the classification of each element. For each integer, the function must append a line to the output string in the exact format: `"Número evaluado: <value> -> <result>\n"`, where `<result>` is `"El número es positivo."`, `"El número es negativo."`, or `"El número es cero."` depending on whether the number is greater than 0, less than 0, or equal to 0, respectively. The function must preserve the original order of input values, handle empty vectors (return an empty string), and work with negative, zero, and positive integers, including duplicate values.
#include <cassert>
#include <string>
#include <vector>

// (The solution function is assumed to be declared above.)

int main() {
    // Basic mixed values
    assert(classifyNumbers({5, -3, 0, 12, -7}) ==
           "Número evaluado: 5 -> El número es positivo.\n"
           "Número evaluado: -3 -> El número es negativo.\n"
           "Número evaluado: 0 -> El número es cero.\n"
           "Número evaluado: 12 -> El número es positivo.\n"
           "Número evaluado: -7 -> El número es negativo.\n");

    // Empty vector
    assert(classifyNumbers({}) == "");

    // Only zeros
    assert(classifyNumbers({0, 0}) ==
           "Número evaluado: 0 -> El número es cero.\n"
           "Número evaluado: 0 -> El número es cero.\n");

    // Only negatives
    assert(classifyNumbers({-1, -2}) ==
           "Número evaluado: -1 -> El número es negativo.\n"
           "Número evaluado: -2 -> El número es negativo.\n");

    // Single positive
    assert(classifyNumbers({42}) ==
           "Número evaluado: 42 -> El número es positivo.\n");

    // Duplicate values
    assert(classifyNumbers({7, 7, -7}) ==
           "Número evaluado: 7 -> El número es positivo.\n"
           "Número evaluado: 7 -> El número es positivo.\n"
           "Número evaluado: -7 -> El número es negativo.\n");

    return 0;
}
#include <string>
#include <vector>

// Returns a multi-line string that classifies each integer in the input vector.
// For each number, the output line has the form:
// "Número evaluado: <value> -> <positive/negative/zero message>\n"
std::string classifyNumbers(const std::vector<int>& numbers) {
    std::string result;
    for (const int number : numbers) {
        result += "Número evaluado: " + std::to_string(number) + " -> ";
        if (number > 0) {
            result += "El número es positivo.\n";
        } else if (number < 0) {
            result += "El número es negativo.\n";
        } else {
            result += "El número es cero.\n";
        }
    }
    return result;
}
// The solution iterates over each element of the input vector in order using a range-based for loop. For each element, a conditional chain (`if`, `else if`, `else`) determines whether the value is positive, negative, or zero, and the corresponding message is appended to the result string using `std::to_string` for the numeric value and the newline character for line separation. The main edge cases are: an empty vector (the loop doesn't execute, resulting in an empty string), and values that are exactly zero (must be handled in the final `else` branch). No modifiers to the input are needed, so the vector is passed by `const` reference to avoid copying and to enforce read-only access. Time complexity is O(n) where n is the number of elements, because each element is processed once. Space complexity is O(1) auxiliary, plus the space needed for the resulting string, which grows linearly with the total number of digits and output characters.
