// Write a C++ function named `transformCamelCase` that takes a string `operation` in the form `"S;M;someMethodName()"` or `"C;V;some variable name"` (or `"S;C;"`, `"S;V;"`, `"C;M;"`, `"C;C;"` as appropriate), where the first character is `'S'` for split or `'C'` for combine, the second character after the semicolon is `'M'` for method, `'C'` for class, or `'V'` for variable, and the remaining part is the name. For split operations, convert the name to space-separated lowercase words (removing trailing `()` for methods and capitalizing first word as needed for classes — actually for split, class names should start with lowercase after splitting). For combine operations, convert a space-separated phrase into a camelCase name: variables start lowercase, classes start uppercase, methods start lowercase and end with `()` (no spaces). You may assume input is correctly formatted, non-empty, and contains only letters and spaces (except for the method parentheses in split).
The main algorithm is straightforward: parse the first character to decide split or combine, parse the second character to know the type (method, class, variable). For split: if it's a method, remove the trailing `()`; if it's a class, the first character is changed to lowercase; then iterate through the string, whenever an uppercase letter is found, insert a space before it and make it lowercase. For combine: iterate through the string, remove spaces, and capitalize the letter following each removed space; then apply the type rules: for class, uppercase the first letter; for method, append `()`; for variable, ensure the first letter is lowercase (already is from the input, but can force it). Edge cases: empty name (shouldn't happen), method split must remove exactly two characters `()` at the end, and combine should handle multiple spaces (though input likely has single spaces). Time complexity is O(n) per operation, where n is the length of the name part; space complexity is O(n) because we modify and return a string.
#include <string>
#include <cctype>

// Transform a Camel Case operation string according to the rules.
// operation format: "S;M;someMethod()" or "C;V;some variable", etc.
std::string transformCamelCase(const std::string& operation) {
    char action = operation[0];
    char type = operation[2];
    std::string str = operation.substr(4); // skip "X;Y;"

    if (action == 'S') { // Split
        if (type == 'M') {
            // Remove trailing "()"
            if (str.size() >= 2 && str.compare(str.size() - 2, 2, "()") == 0) {
                str.erase(str.size() - 2);
            }
        }
        if (type == 'C') {
            // Class names start uppercase, but after splitting we want lowercase first word
            if (!str.empty() && std::isupper(str[0])) {
                str[0] = static_cast<char>(std::tolower(str[0]));
            }
        }
        // Insert space before each uppercase letter and lower it
        for (size_t i = 0; i < str.size(); ++i) {
            if (std::isupper(str[i])) {
                str.insert(i, 1, ' ');
                str[i + 1] = static_cast<char>(std::tolower(str[i + 1]));
                ++i; // skip the newly inserted space
            }
        }
    } else { // Combine (action == 'C')
        // Remove spaces and capitalize following letters
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == ' ') {
                str.erase(i, 1);
                if (i < str.size()) {
                    str[i] = static_cast<char>(std::toupper(str[i]));
                }
                --i; // check the same index again (since we erased)
            }
        }
        if (type == 'C') {
            if (!str.empty() && std::islower(str[0])) {
                str[0] = static_cast<char>(std::toupper(str[0]));
            }
        }
        if (type == 'M') {
            str += "()";
        }
        // For 'V' variable, the first letter should be lowercase, which is already the case from input.
    }

    return str;
}
#include <cassert>

int main() {
    // Split tests
    assert(transformCamelCase("S;M;someMethod()") == "some method");
    assert(transformCamelCase("S;C;BlueCar") == "blue car");
    assert(transformCamelCase("S;V;someVariable") == "some variable");
    assert(transformCamelCase("S;M;startThread()") == "start thread");
    assert(transformCamelCase("S;C;MyClassName") == "my class name");

    // Combine tests
    assert(transformCamelCase("C;V;some variable") == "someVariable");
    assert(transformCamelCase("C;C;blue car") == "BlueCar");
    assert(transformCamelCase("C;M;start thread") == "startThread()");
    assert(transformCamelCase("C;V;alreadyCamel") == "alreadyCamel");
    assert(transformCamelCase("C;M;do work") == "doWork()");

    return 0;
}
