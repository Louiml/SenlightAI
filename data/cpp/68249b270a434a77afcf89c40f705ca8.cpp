Write a C++ function `applyConstantPropagation` that accepts a vector of strings representing lines of C-like code (already stripped of all whitespace) and an unordered_map<char, int> that initially maps variable names (single lowercase or uppercase letters) to constant integer values. The function must process each line as follows: if the line exactly matches the pattern `X=digits;` (one letter, `=`, one or more digits, `;`), then update the value map for that variable with the parsed integer and do **not** include the line in the output. For all other lines, replace every occurrence of a standalone single-letter variable (a letter that is not part of a larger alphanumeric identifier, i.e., its immediate neighbors are not alphanumeric) with its current numeric value from the map if it exists; otherwise, leave the letter unchanged. Return a new vector<string> containing the processed non-assignment lines in their original relative order. Input lines are guaranteed to be non-empty and contain only letters, digits, `=`, `;`, and `+` (no spaces), and digit sequences are valid non-negative integers. The original code snippet reads from a file and applies similar logic but with a bug where it misses the last digit due to `substr` length; your implementation must correctly parse the full integer on the assignment line. Important: The map is passed by value, so assignments within the function affect only the local copy, meaning earlier assignments are visible to later lines in the same call.

// The solution processes the lines in one pass. For each line, first check if it matches the regular expression `^[a-zA-Z]=[0-9]+;$` (in the original snippet, the regex misses multi‑digit numbers because it uses `*`, but we require at least one digit). If it matches, extract the variable letter (character at index 0) and the digits starting at index 2 up to but not including the trailing `;`, convert to integer with `std::stoi`, and store in the map. Do not add this line to the output. Otherwise, build a new string by iterating over each character `c` at index `j`. Determine if `c` is a standalone identifier character: it must be a letter, and either it is at the beginning of the line and the next character is not alphanumeric, or at the end and the previous is not alphanumeric, or in the middle with both neighbors non‑alphanumeric. If it is a standalone letter and it exists in the map, append its integer value as a string; otherwise, append the character unchanged. Edge cases: assignments like `a=0;` map to zero; `a=12;` must parse the full number (the original snippet erroneously uses `substr(2, n-1)` where `n` is the number of lines, bug); letters that appear inside multi‑character identifiers (e.g., `ab` or `a1`) are not standalone and are left untouched; lines that are not assignments and may include letters unknown to the map are unchanged. Time complexity is O(total number of characters across all lines) because each character is examined once and concatenation is amortized linear; space complexity is O(total output characters) for the result vector plus the map size, which is O(number of variables).

#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <regex>

// Process lines: handle constant assignments and propagate known constants.
std::vector<std::string> applyConstantPropagation(
    const std::vector<std::string>& lines,
    std::unordered_map<char, int> values) {

    std::vector<std::string> result;
    const std::regex assignment_pattern("^[a-zA-Z]=[0-9]+;$");

    for (const std::string& line : lines) {
        if (std::regex_match(line, assignment_pattern)) {
            // Parse variable and integer value.
            char variable = line[0];
            std::string digits = line.substr(2, line.size() - 3); // skip "X=" and trailing ';'
            int val = std::stoi(digits);
            values[variable] = val;
        } else {
            std::string appended;
            const int len = static_cast<int>(line.size());
            for (int j = 0; j < len; ++j) {
                char c = line[j];
                if (std::isalpha(static_cast<unsigned char>(c))) {
                    // Check if this is a standalone identifier (not part of alnum sequence).
                    bool standalone = false;
                    if (j == 0) {
                        standalone = (len == 1) || !std::isalnum(static_cast<unsigned char>(line[1]));
                    } else if (j == len - 1) {
                        standalone = !std::isalnum(static_cast<unsigned char>(line[j - 1]));
                    } else {
                        standalone = !std::isalnum(static_cast<unsigned char>(line[j - 1])) &&
                                     !std::isalnum(static_cast<unsigned char>(line[j + 1]));
                    }
                    if (standalone) {
                        auto it = values.find(c);
                        if (it != values.end()) {
                            appended += std::to_string(it->second);
                            continue;
                        }
                    }
                }
                appended += c;
            }
            result.push_back(appended);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <unordered_map>

// Solution function declaration (or include the implementation above)

int main() {
    std::unordered_map<char, int> init;
    
    // Test 1: Basic assignment and propagation with addition signs.
    std::vector<std::string> lines1 = {"a=5;", "b=a+1;"};
    std::vector<std::string> res1 = applyConstantPropagation(lines1, init);
    assert(res1.size() == 1);
    assert(res1[0] == "5+1");

    // Test 2: Multi-digit assignment is fully captured.
    std::vector<std::string> lines2 = {"x=123;", "y=x+x;"};
    std::vector<std::string> res2 = applyConstantPropagation(lines2, init);
    assert(res2.size() == 1);
    assert(res2[0] == "123+123");

    // Test 3: Unknown variable remains unchanged.
    std::vector<std::string> lines3 = {"z=7;", "w=z+q;"};
    std::vector<std::string> res3 = applyConstantPropagation(lines3, init);
    assert(res3.size() == 1);
    assert(res3[0] == "7+q");

    // Test 4: Non-assignment line with no variables.
    std::vector<std::string> lines4 = {"1+2;"};
    std::vector<std::string> res4 = applyConstantPropagation(lines4, init);
    assert(res4.size() == 1);
    assert(res4[0] == "1+2;");

    // Test 5: Letters inside larger identifiers are not replaced.
    std::vector<std::string> lines5 = {"a=3;", "ab+a;"};
    std::vector<std::string> res5 = applyConstantPropagation(lines5, init);
    assert(res5.size() == 1);
    assert(res5[0] == "ab+3");

    // Test 6: Assignment to zero.
    std::vector<std::string> lines6 = {"p=0;", "p;"};
    std::vector<std::string> res6 = applyConstantPropagation(lines6, init);
    assert(res6.size() == 1);
    assert(res6[0] == "0");

    // Test 7: Lines consisting only of an assignment are removed entirely.
    std::vector<std::string> lines7 = {"a=1;", "b=2;", "a;"};
    std::vector<std::string> res7 = applyConstantPropagation(lines7, init);
    assert(res7.size() == 1);
    assert(res7[0] == "1");

    // Test 8: Variable letters at boundaries.
    std::vector<std::string> lines8 = {"a=9;", "+a+", "a+"};
    std::vector<std::string> res8 = applyConstantPropagation(lines8, init);
    assert(res8.size() == 2);
    assert(res8[0] == "+9+");
    assert(res8[1] == "9+");

    // Test 9: Map is local; multiple assignments update sequentially.
    std::vector<std::string> lines9 = {"m=1;", "n=2;", "m=3;", "m+n;"};
    std::vector<std::string> res9 = applyConstantPropagation(lines9, init);
    assert(res9.size() == 1);
    assert(res9[0] == "3+2");

    // Test 10: No lines at all.
    std::vector<std::string> lines10;
    std::vector<std::string> res10 = applyConstantPropagation(lines10, init);
    assert(res10.empty());

    return 0;
}
