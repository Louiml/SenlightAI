/*
Write a C++ function `analyzeTokens` that accepts a C-style string (e.g., `const char*` or `std::string`) containing a sequence of characters terminated by `#` (the `#` itself is not part of the input). The function must tokenize the input according to the lexical rules implied by the provided code snippet, specifically: identifiers (sequences of letters and digits, starting with a letter) are classified as keyword type numbers `1..6` for the reserved words `begin`, `if`, `then`, `while`, `do`, `end`, and as type `10` for any other identifier; integer constants (sequences of digits) are type `20`; single-character operators `= + - * / ( ) [ ] { } , : ;` are types `21` through `34` respectively; `>` is type `35`, `<` is type `36`, `>=` is `37`, `<=` is `38`, `==` is `39`, `!=` is `40`; any other character (except `#` which marks the end) yields type `-1` (error). The function must return a `std::vector<std::pair<int, std::string>>` where each pair contains the type number and the lexeme (the raw token string). The tokenization must stop at the `#` character (do not process anything after it). The function must correctly handle whitespace (spaces and newlines) by skipping them between tokens, and must ensure that multi-character operators (like `>=`, `==`) are greedily matched. Do not modify the input string; do not use global variables; the function should be self-contained except for standard library includes.
*/
#include <vector>
#include <string>
#include <cstring>

// Tokenize a C-string according to specified lexical rules. Stops at '#'.
std::vector<std::pair<int, std::string>> analyzeTokens(const std::string& input) {
    static const std::vector<std::string> keywords = {
        "begin", "if", "then", "while", "do", "end"
    };

    std::vector<std::pair<int, std::string>> tokens;
    size_t pos = 0;
    const size_t n = input.size();

    auto is_letter = [](char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    };
    auto is_digit = [](char c) {
        return (c >= '0' && c <= '9');
    };
    auto is_whitespace = [](char c) {
        return (c == ' ' || c == '\n');
    };

    while (pos < n && input[pos] != '#') {
        // Skip whitespace
        while (pos < n && is_whitespace(input[pos])) {
            ++pos;
        }
        if (pos >= n || input[pos] == '#') break;

        char ch = input[pos];
        std::string lexeme;

        // Identifiers or keywords
        if (is_letter(ch)) {
            lexeme.push_back(ch);
            ++pos;
            while (pos < n && (is_letter(input[pos]) || is_digit(input[pos]))) {
                lexeme.push_back(input[pos]);
                ++pos;
            }
            int type = 10; // default identifier
            for (size_t i = 0; i < keywords.size(); ++i) {
                if (lexeme == keywords[i]) {
                    type = static_cast<int>(i + 1);
                    break;
                }
            }
            tokens.emplace_back(type, lexeme);
        }
        // Integer constants
        else if (is_digit(ch)) {
            lexeme.push_back(ch);
            ++pos;
            while (pos < n && is_digit(input[pos])) {
                lexeme.push_back(input[pos]);
                ++pos;
            }
            tokens.emplace_back(20, lexeme);
        }
        // Operators (possibly multi-character)
        else {
            bool matched = false;
            // Multi-character operators
            if (pos + 1 < n) {
                char next = input[pos + 1];
                if (ch == '=' && next == '=') {
                    tokens.emplace_back(39, "==");
                    pos += 2;
                    matched = true;
                } else if (ch == '>' && next == '=') {
                    tokens.emplace_back(37, ">=");
                    pos += 2;
                    matched = true;
                } else if (ch == '<' && next == '=') {
                    tokens.emplace_back(38, "<=");
                    pos += 2;
                    matched = true;
                } else if (ch == '!' && next == '=') {
                    tokens.emplace_back(40, "!=");
                    pos += 2;
                    matched = true;
                }
                else if (ch == '!') {
                    // '!' alone is an error
                    tokens.emplace_back(-1, "!");
                    ++pos;
                    matched = true;
                }
            }
            if (!matched) {
                // Single-character operators
                switch (ch) {
                    case '=': tokens.emplace_back(21, "="); ++pos; break;
                    case '+': tokens.emplace_back(22, "+"); ++pos; break;
                    case '-': tokens.emplace_back(23, "-"); ++pos; break;
                    case '*': tokens.emplace_back(24, "*"); ++pos; break;
                    case '/': tokens.emplace_back(25, "/"); ++pos; break;
                    case '(': tokens.emplace_back(26, "("); ++pos; break;
                    case ')': tokens.emplace_back(27, ")"); ++pos; break;
                    case '[': tokens.emplace_back(28, "["); ++pos; break;
                    case ']': tokens.emplace_back(29, "]"); ++pos; break;
                    case '{': tokens.emplace_back(30, "{"); ++pos; break;
                    case '}': tokens.emplace_back(31, "}"); ++pos; break;
                    case ',': tokens.emplace_back(32, ","); ++pos; break;
                    case ':': tokens.emplace_back(33, ":"); ++pos; break;
                    case ';': tokens.emplace_back(34, ";"); ++pos; break;
                    case '>': tokens.emplace_back(35, ">"); ++pos; break;
                    case '<': tokens.emplace_back(36, "<"); ++pos; break;
                    default:
                        // Unknown character: error
                        lexeme.push_back(ch);
                        tokens.emplace_back(-1, lexeme);
                        ++pos;
                        break;
                }
            }
        }
    }

    return tokens;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// (Function definition from solution goes here, or assume it's included)

int main() {
    // Basic tokens with reserved words and identifiers
    std::vector<std::pair<int, std::string>> t1 = analyzeTokens("begin if then while do end variable123 #");
    assert(t1.size() == 7);
    assert(t1[0] == std::make_pair(1, std::string("begin")));
    assert(t1[1] == std::make_pair(2, std::string("if")));
    assert(t1[2] == std::make_pair(3, std::string("then")));
    assert(t1[3] == std::make_pair(4, std::string("while")));
    assert(t1[4] == std::make_pair(5, std::string("do")));
    assert(t1[5] == std::make_pair(6, std::string("end")));
    assert(t1[6] == std::make_pair(10, std::string("variable123")));

    // Integers and operators
    std::vector<std::pair<int, std::string>> t2 = analyzeTokens("123 + - * / = == >= <= != > <");
    assert(t2.size() == 12);
    assert(t2[0] == std::make_pair(20, std::string("123")));
    assert(t2[1] == std::make_pair(22, std::string("+")));
    assert(t2[2] == std::make_pair(23, std::string("-")));
    assert(t2[3] == std::make_pair(24, std::string("*")));
    assert(t2[4] == std::make_pair(25, std::string("/")));
    assert(t2[5] == std::make_pair(21, std::string("=")));
    assert(t2[6] == std::make_pair(39, std::string("==")));
    assert(t2[7] == std::make_pair(37, std::string(">=")));
    assert(t2[8] == std::make_pair(38, std::string("<=")));
    assert(t2[9] == std::make_pair(40, std::string("!=")));
    assert(t2[10] == std::make_pair(35, std::string(">")));
    assert(t2[11] == std::make_pair(36, std::string("<")));

    // Punctuation and special characters
    std::vector<std::pair<int, std::string>> t3 = analyzeTokens("( ) [ ] { } , : ;");
    assert(t3.size() == 8);
    assert(t3[0].first == 26 && t3[0].second == "(");
    assert(t3[1].first == 27 && t3[1].second == ")");
    assert(t3[2].first == 28 && t3[2].second == "[");
    assert(t3[3].first == 29 && t3[3].second == "]");
    assert(t3[4].first == 30 && t3[4].second == "{");
    assert(t3[5].first == 31 && t3[5].second == "}");
    assert(t3[6].first == 32 && t3[6].second == ",");
    assert(t3[7].first == 33 && t3[7].second == ":");
    // There is no ';' in the input, so we add a separate check below

    // Whitespace handling and multiple tokens with spaces/newlines
    std::vector<std::pair<int, std::string>> t4 = analyzeTokens("  \n begin 42 \n end #");
    assert(t4.size() == 3);
    assert(t4[0] == std::make_pair(1, std::string("begin")));
    assert(t4[1] == std::make_pair(20, std::string("42")));
    assert(t4[2] == std::make_pair(6, std::string("end")));

    // Error cases: '!' alone, unknown character '@', and '#' at start
    std::vector<std::pair<int, std::string>> t5 = analyzeTokens("! @ #");
    assert(t5.size() == 2);
    assert(t5[0] == std::make_pair(-1, std::string("!")));
    assert(t5[1] == std::make_pair(-1, std::string("@")));

    std::vector<std::pair<int, std::string>> t6 = analyzeTokens("#");
    assert(t6.empty());

    // Empty input (only spaces)
    std::vector<std::pair<int, std::string>> t7 = analyzeTokens("   \n  ");
    assert(t7.empty());

    // Multi-character operator at end of input (no #)
    std::vector<std::pair<int, std::string>> t8 = analyzeTokens(">=");
    assert(t8.size() == 1);
    assert(t8[0] == std::make_pair(37, std::string(">=")));

    // Semicolon as punctuation (type 34)
    std::vector<std::pair<int, std::string>> t9 = analyzeTokens(";");
    assert(t9.size() == 1);
    assert(t9[0] == std::make_pair(34, std::string(";")));

    // Identifier that starts with a digit is not valid, so it's a number followed by identifier
    std::vector<std::pair<int, std::string>> t10 = analyzeTokens("123abc");
    assert(t10.size() == 2);
    assert(t10[0] == std::make_pair(20, std::string("123")));
    assert(t10[1] == std::make_pair(10, std::string("abc")));

    return 0;
}
// The solution simulates a simple scanner similar to the provided code. The main algorithm: iterate over the input string from left to right, skipping spaces and newlines. At each non-whitespace character, determine the token type by peeking at the current character and optionally one character ahead for multi-character operators. For identifiers, consume all consecutive letters and digits (starting with a letter), then check the resulting lexeme against the reserved words list; if found, assign type `index+1` (where index starts at 0), else type `10`. For numbers, consume all consecutive digits and assign type `20`. For operators, use a switch/if-else structure: for `=`, `>`, `<`, `!` we need to peek at the next character to see if it forms `==`, `>=`, `<=`, `!=`; if yes, consume both and assign the appropriate type; otherwise, consume only the single character and assign the base type. For all other single-character operators, assign type as per the mapping. If the character is none of the above and not whitespace, produce type `-1` and the single character as lexeme, then continue scanning (do not treat as fatal). The loop ends when the null terminator of the input string is reached or when we encounter `#` (which should stop processing, not produce a token). Time complexity is O(n) where n is the length of the input string, because each character is processed at most a constant number of times (peeking ahead is still O(1) per token). Space complexity is O(m) for the returned vector where m is the number of tokens, plus O(k) for the temporary token buffer, but overall O(m + max_token_length). Edge cases: empty input (leading to an empty vector), input that is only whitespace (empty vector), identifiers that are longer than the reserved words, multi-character operators at the very end (e.g., `=` at the end of string should produce `=` not an error), `#` appearing immediately without preceding tokens (should terminate and return empty). Also, note that the original code treats `!` alone as an error and `!=` as `40`; we must replicate that. The function should be `const`-correct and avoid modifications to the input string.
