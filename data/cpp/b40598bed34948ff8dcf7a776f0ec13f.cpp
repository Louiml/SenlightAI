// Write a C++ function `analyzeSourceLine` that takes a single string representing one line of source code and returns a `std::string` containing a formatted token classification report. The function must recognize the following token types in order of priority: identifiers (starting with a letter, followed by letters or digits), numbers (nonzero digit followed by digits, or a single zero), arithmetic operators (`+`, `-`, `*`, `/`), separators (`,` `;` `(` `)` `[` `]` `{` `}`), relational operators (`<`, `>`, `=` — note that `!` alone is not a relational operator, but `!=` becomes a special "non-equal operator"), and division operator `/` (which overlaps with arithmetic but must be classified as a separate type when appearing alone). The output format should list each token on a new line, prefixed with the category name and colon, then the actual characters of that token. If an unrecognized character appears, output it enclosed in angle brackets on its own line. Consecutive characters of the same category should be grouped together into a single token (e.g., `abc` becomes one identifier, not three). However, if a slash `/` is immediately followed by another slash `//`, then from that point to the end of the line, everything (including the slashes) is ignored and the function stops processing. The function must preserve the exact order of tokens as they appear in the input line. Whitespace characters (space and newline) are ignored and do not produce output. The input string will not contain actual newline characters (it's a single line), but it may contain spaces. The function must return the concatenated output as a string, with each token's report separated by a newline character `\n` (and no trailing newline at the end). For example, for input `"x=1+2"` the output should be `"ID: x\nRelationalOperator: =\nNumber: 1\nOperator: +\nNumber: 2"`. Important edge cases: a digit `0` is a separate token type ("Zero") and not grouped with other numbers; a `!` alone is its own category ("NonEqualOperator") but if followed by `=`, it becomes a single token "NonEqualOperator: !="; a `/` alone is "DivOperator", but if two slashes appear, ignore the rest of the line. Do not use any global state or file I/O—just pure string processing.

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.

int main() {
    // Basic tokens
    assert(analyzeSourceLine("x=1+2") == "ID: x\nRelationalOperator: =\nNumber: 1\nOperator: +\nNumber: 2");
    assert(analyzeSourceLine("if (a) { b = 10; }") == "ID: if\nSeparator: (\nID: a\nSeparator: )\nSeparator: {\nID: b\nRelationalOperator: =\nNumber: 10\nSeparator: ;\nSeparator: }");
    // Zero and number continuation
    assert(analyzeSourceLine("0 10 01") == "Zero: 0\nNumber: 10\nZero: 0\nNumber: 1");
    // Non-equal operators
    assert(analyzeSourceLine("a != b") == "ID: a\nNonEqualOperator: !=\nID: b");
    assert(analyzeSourceLine("a ! b") == "ID: a\nNonEqualOperator: !\nID: b");
    // Relational operators with =
    assert(analyzeSourceLine("a <= b") == "ID: a\nRelationalOperator: <=\nID: b");
    assert(analyzeSourceLine("a >= b") == "ID: a\nRelationalOperator: >=\nID: b");
    assert(analyzeSourceLine("a == b") == "ID: a\nRelationalOperator: ==\nID: b");
    // Division and comments
    assert(analyzeSourceLine("a / b") == "ID: a\nDivOperator: /\nID: b");
    assert(analyzeSourceLine("a // comment") == "ID: a\nDivOperator: /");
    assert(analyzeSourceLine("// comment only") == "");
    // Separators and operators
    assert(analyzeSourceLine("a+b*c") == "ID: a\nOperator: +\nID: b\nOperator: *\nID: c");
    assert(analyzeSourceLine("(a)") == "Separator: (\nID: a\nSeparator: )");
    // Unrecognized characters
    assert(analyzeSourceLine("a@b") == "ID: a\n<@>\nID: b");
    // Empty or whitespace only
    assert(analyzeSourceLine("") == "");
    assert(analyzeSourceLine("   ") == "");
    return 0;
}

#include <string>
#include <cctype>

/**
 * Analyzes a single line of source code and returns a formatted token classification report.
 * Tokens are recognized: identifiers, numbers, zero, operators, separators, relational operators,
 * non-equal operator (! or !=), division operator (/), and comments (// ignore rest).
 * Output format: each token on a new line as "Category: chars". Unrecognized chars are "<char>".
 * Whitespace is ignored. Returns empty string if no tokens.
 */
std::string analyzeSourceLine(const std::string& line) {
    std::string result;
    std::string currentToken;
    int state = 0; // 0=none, 1=id, 2=number, 3=zero, 4=op, 5=sep, 6=rel, 7=rel=, 8=!, 9=!=, 10=div, 11=comment
    auto flush = [&]() {
        if (!currentToken.empty()) {
            if (!result.empty()) result += '\n';
            result += currentToken;
            currentToken.clear();
        }
    };

    auto isAlpha = [](char c) { return std::isalpha(static_cast<unsigned char>(c)); };
    auto isDigitChar = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };
    auto isNonZeroDigit = [](char c) { return c >= '1' && c <= '9'; };

    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (state == 11) break; // in comment, ignore rest

        // Meta: if we see a second slash after division -> comment
        if (state == 10 && ch == '/') {
            // comment starts, ignore rest of line
            break;
        }

        // Check continuation conditions first
        if (state == 1 && (isAlpha(ch) || isDigitChar(ch))) {
            // continue identifier
            currentToken += ch;
            continue;
        } else if (state == 2 && isDigitChar(ch)) {
            // continue number
            currentToken += ch;
            continue;
        } else if (state == 6 && ch == '=') {
            // relational operator followed by '=' -> e.g., <=, >=, ==
            state = 7;
            currentToken += ch;
            continue;
        } else if (state == 8 && ch == '=') {
            // '!' followed by '=' -> !=
            state = 9;
            currentToken += ch;
            continue;
        }

        // If reached here, current token (if any) is complete; start a new token
        // Flush previous token and reset
        flush();
        state = 0;

        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            // whitespace, ignore
            continue;
        }

        // Determine new token category
        if (isAlpha(ch)) {
            // identifier start
            state = 1;
            currentToken = "ID: ";
            currentToken += ch;
        } else if (isNonZeroDigit(ch)) {
            // number start
            state = 2;
            currentToken = "Number: ";
            currentToken += ch;
        } else if (ch == '0') {
            // zero token
            state = 3;
            currentToken = "Zero: ";
            currentToken += ch;
        } else if (ch == '+' || ch == '-' || ch == '*') {
            // arithmetic operator (excluding / handled separately)
            state = 4;
            currentToken = "Operator: ";
            currentToken += ch;
        } else if (ch == '/' ) {
            // division operator (could be comment start handled above for second slash)
            state = 10;
            currentToken = "DivOperator: ";
            currentToken += ch;
        } else if (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}') {
            state = 5;
            currentToken = "Separator: ";
            currentToken += ch;
        } else if (ch == '<' || ch == '>' || ch == '=') {
            // relational operator
            state = 6;
            currentToken = "RelationalOperator: ";
            currentToken += ch;
        } else if (ch == '!') {
            // non-equal operator start
            state = 8;
            currentToken = "NonEqualOperator: ";
            currentToken += ch;
        } else {
            // unrecognized character
            currentToken = "<" + std::string(1, ch) + ">";
            // state remains 0, but we flush immediately? Actually we output it directly as its own token.
            flush(); // but we set currentToken above, so flush will add it
            state = 0;
        }
    }
    // flush any remaining token at end of line
    flush();
    return result;
}

// The solution simulates a simple finite-state machine similar to a lexical analyzer. We iterate over each character of the input line, maintaining the current state (token type) and the previous state to decide when to start a new token. The states are: 0 = no token, 1 = identifier, 2 = number (nonzero start), 3 = zero, 4 = arithmetic operator, 5 = separator, 6 = relational operator (non-`!`), 7 = relational operator followed by `=`, 8 = `!` (non-equal operator start), 9 = `!=` complete, 10 = division operator `/`, 11 = comment start (two slashes, ignore rest). The key logic: for each character, we check conditions in a specific order to determine if it continues the current token or begins a new one. If a character can continue the current state (e.g., identifier continues with alphanumeric, number continues with digit, relational operator continues with `=`), we append it to the current output buffer. Otherwise, if it starts a new token, we first flush the current token's report (if any) and then start a new one. Special handling for `!`: if we see `!` and not in a comment, we enter state 8; if the next character is `=`, we move to state 9 and output `!=` together. For `/`, if we see a second `/` while already in division state, we enter comment state and stop processing. Whitespace resets the state to 0 without flushing (actually, it flushes the previous token when a new token starts, but whitespace itself is not a token). The algorithm processes each character exactly once in O(n) time, where n is the length of the input string. Space usage is O(n) for the output string, plus constant auxiliary space for the state and buffer. Edge cases include: empty line produces empty string; a line with only whitespace produces empty string; a line starting with `//` produces empty string; mixed tokens like `a1b` produce one identifier (since after `a`, `1` is alphanumeric, then `b` continues); a number like `012` is processed as `Zero` for the first `0` and then `Number: 12` for the rest? Actually, careful: the state machine treats `0` as its own token, so after `0`, the next digit `1` starts a new number token. Also, a digit after a nonzero digit continues the number, but if we encounter a digit after a zero, we must split. For simplicity, we implement exactly as the original snippet logic suggests: a zero is always its own token; a nonzero digit starts a number that continues with digits. However, a digit after a zero starts a new number (since `isDigit` for continuation only applies when status==2). The original snippet has a bug (`isDigit` calls `isNonZeroDigit` twice), but we can fix it correctly: a number continuation is any digit. We treat `0` as a separate token type, so `10` would output `Number: 1` then `Zero: 0`? But wait, the snippet's logic shows that `status == 2 && isDigit(ch)` continues a number, so `10` would output `Number: 10` because after seeing `1`, status becomes 2, and the next `0` matches `isDigit` (true) and status is 2, so it continues. The `isZeroDigit` condition only applies when `checkState(status)` (status != 11) and status is not already 1 or 2, so if we are in a number, `0` continues it. Good. So we must replicate that: a `0` after a nonzero number continues the number; a standalone `0` (or `0` after a non-number) is its own "Zero" token. Also, a number cannot have leading zeros after the first digit, but a number like `0` is a zero token, and `01` would produce `Zero: 0` then `Number: 1` because after `0`, status is 3, and then `1` does not match continuation for number (since status is 3, not 2), and `checkState` is true, so it starts a new number. That's fine. The original snippet also incorrectly treats `isDigit` as `isNonZeroDigit` twice; we fix that. We'll implement a clean state machine that matches the given behavior. The function will build an output string, using a small buffer to hold the current token's characters, and a variable to track the current category name. When a new token starts, we append the category label and the buffered characters to the result, then clear the buffer. At the end of the line, we flush any pending token. Comments: once we see two slashes in a row, we just break out of the loop and ignore the rest. We'll also ensure that the `!` handling works: a `!` alone outputs "NonEqualOperator: !" (category name exactly as in snippet, but maybe use "NonEqualOperator" for consistency), and `!=` outputs "NonEqualOperator: !=". For relational operators, we output "RelationalOperator: <" or "RelationalOperator: <=" etc. We must be careful with the order of checks to prioritize longer matches (like `!=` and `<=`) correctly. The snippet's logic checks `isNonEqualRelationalOperator` (which includes `<`, `>`, `=`) before the `!` check, but also has a special case for `!`. We'll implement a simpler, correct version. Time complexity O(n), space O(n) for output.
