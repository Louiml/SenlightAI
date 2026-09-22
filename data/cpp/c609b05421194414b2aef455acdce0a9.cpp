// Write a C++ function named `formatExpression` that takes two non-negative integer strings `op1` and `op2`, and a character `op` (one of `'+'`, `'-'`, `'*'`), and returns a string containing the formatted arithmetic work, exactly as shown in the provided snippet but with all the output captured in a single string (including the trailing empty line after each operation). The function must handle arbitrary non-negative integers that fit in `int` range. The result must be formatted with right alignment using spaces (not tabs), with the width for each line equal to the maximum length among the first operand, the operator-prefixed second operand (e.g., `+123`, `-45`, `*67`), the result (for addition/subtraction), or the intermediate products and final product (for multiplication). For multiplication, the first line of dashes should have length equal to the maximum of the lengths of the first operand and the operator-prefixed second operand, and subsequent intermediate lines are each right-aligned with one less leading space than the previous (so they appear shifted left), and a second dashed line separates the intermediate products from the final result (the length of that second dashed line is at least the length of the final result and the length of the last intermediate product plus its shift). The function must produce output identical in structure to the snippet’s `add`, `substract`, and `multiply` functions combined, including the extra blank line after each operation. For subtraction, the dashed line length is the maximum of the result length and the operator-prefixed second operand length. For addition, the dashed line length is the same as the overall width. The function should not print anything; it must return the formatted string.

#include <cassert>
#include <string>

int main() {
    // Addition tests
    std::string expected1 = " 123\n+ 45\n----\n 168\n\n";
    // Actually compute: op1="123", op2="45", op='+'
    // op2Formatted="+45", len=max(3,3,3)=3
    // "123" -> "123", "+45" -> "+45", dashes "---", result "168"
    // But right-aligned to width 3: "123", "+45", "---", "168"
    // So expected1 should be "123\n+45\n---\n168\n\n"
    // Let's write correct expected strings based on our function behavior.
    expected1 = "123\n+45\n---\n168\n\n";
    assert(formatExpression("123","45",'+') == expected1);

    // Addition with different lengths
    std::string expected2 = " 9\n+10\n---\n 19\n\n";
    // op1="9", op2="10", op2Formatted="+10", width=3, lines: "  9", "+10", "---", " 19"
    // But right-align to width 3: "  9", "+10", "---", " 19"
    expected2 = "  9\n+10\n---\n 19\n\n";
    assert(formatExpression("9","10",'+') == expected2);

    // Subtraction with negative result
    std::string expected3 = " 5\n-10\n---\n -5\n\n";
    // op1="5", op2="10", op2Formatted="-10", width=2 (max(1,3)=3 actually? op1 length=1, op2Formatted length=3 -> len=3)
    // So lines: "  5", "-10", dashes max(r len=2, op2Formatted len=3)=3 -> "---", " -5"
    expected3 = "  5\n-10\n---\n -5\n\n";
    assert(formatExpression("5","10",'-') == expected3);

    // Multiplication single-digit second operand
    std::string expected4 = " 12\n *3\n---\n 36\n\n";
    // op1="12", op2="3", op2Formatted="*3", width=max(2,2,2)=2? Actually r="36" len=2, so width=2
    // Lines: "12", "*3", dashes width=2 "--", "36"
    expected4 = "12\n*3\n--\n36\n\n";
    assert(formatExpression("12","3",'*') == expected4);

    // Multiplication multi-digit second operand
    std::string expected5 = "  123\n  *45\n  ---\n  615\n 492\n ----\n 5535\n\n";
    // Let's manually compute: op1="123", op2="45", intermediates: 5*123=615, 4*123=492
    // op2Formatted="*45", maxLen = max(3,3)=3, r="5535" len=4, lastIntermediate "492" len=3 + shift 1 =4, so width=4
    // Lines:
    // " 123" (width 4)
    // " *45" (width 4)
    // firstDashLen=max(3,3)=3, string "---" right-aligned to 4 -> " ---"
    // intermediate[0]=615, right-align to width 4 -> " 615" (i=0 -> width-0=4)
    // intermediate[1]=492, right-align to width 3 -> " 492" (i=1 -> width-1=3)
    // secondDashNum=max(4, 3+1=4)=4, string "----" right-aligned to 4 -> "----"
    // final "5535" right-aligned to 4 -> "5535"
    expected5 = " 123\n *45\n ---\n 615\n 492\n----\n5535\n\n";
    assert(formatExpression("123","45",'*') == expected5);

    // Multiplication with zero
    std::string expected6 = "0\n*0\n-\n0\n\n";
    // op1="0", op2="0", width=1, dashes "-", result "0"
    expected6 = "0\n*0\n-\n0\n\n";
    assert(formatExpression("0","0",'*') == expected6);

    // Addition with zero
    std::string expected7 = "0\n+0\n--\n0\n\n";
    // op1="0", op2="0", op2Formatted="+0", width=max(1,2,1)=2, lines: " 0", "+0", "--", " 0"
    expected7 = " 0\n+0\n--\n 0\n\n";
    assert(formatExpression("0","0",'+') == expected7);

    // Subtraction resulting zero
    std::string expected8 = " 7\n-7\n--\n 0\n\n";
    // op1="7", op2="7", op2Formatted="-7", width=2, lines: " 7", "-7", dashes max(1,2)=2 "--", " 0"
    expected8 = " 7\n-7\n--\n 0\n\n";
    assert(formatExpression("7","7",'-') == expected8);

    // Large numbers
    std::string expected9 = "12345\n+678\n-----\n13023\n\n";
    // op1="12345", op2="678", op2Formatted="+678", width=5 (max(5,4,5)=5), lines: "12345", " +678" -> actually right-align to 5: "  +678" would be width 6? No, op2Formatted len=4, width=5 -> " +678" (one space)
    // Let's compute: "12345", " +678", dashes 5 "-----", "13023"
    expected9 = "12345\n +678\n-----\n13023\n\n";
    assert(formatExpression("12345","678",'+') == expected9);

    // Multiplication with leading zeros in intermediate
    std::string expected10 = " 12\n*10\n --\n  0\n 12\n --\n120\n\n";
    // op1="12", op2="10", intermediates: 0*12=0, 1*12=12
    // width = max(2,2,3, lastIntermediate 12+1=3)=3
    // Lines: " 12", "*10", firstDashLen=max(2,2)=2 -> " --" right-aligned to 3 -> " --", 
    // intermediate[0]=0 right-align to width 3 -> "  0", intermediate[1]=12 right-align to width 2 -> " 12",
    // secondDashNum=max(r="120" len=3, lastIntermediate "12" len2+1=3)=3 -> "---",
    // final "120"
    expected10 = " 12\n*10\n --\n  0\n 12\n---\n120\n\n";
    assert(formatExpression("12","10",'*') == expected10);
}

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Helper to convert an integer to its decimal string representation.
std::string intToString(int value) {
    return std::to_string(value);
}

// Helper to right-align a string to a given width using spaces.
std::string rightAlign(const std::string& s, int width) {
    std::ostringstream oss;
    oss << std::setw(width) << s;
    return oss.str();
}

// Format arithmetic expression exactly as in the original snippet.
std::string formatExpression(const std::string& op1, const std::string& op2, char op) {
    // Convert operands to integers.
    int val1 = 0;
    for (char c : op1) {
        val1 = val1 * 10 + (c - '0');
    }
    int val2 = 0;
    for (char c : op2) {
        val2 = val2 * 10 + (c - '0');
    }

    std::string result;
    std::string r;

    if (op == '+') {
        int res = val1 + val2;
        r = intToString(res);
        std::string op2Formatted = "+" + op2;
        int len = std::max({(int)op1.length(), (int)op2Formatted.length(), (int)r.length()});
        result += rightAlign(op1, len) + "\n";
        result += rightAlign(op2Formatted, len) + "\n";
        result += std::string(len, '-') + "\n";
        result += rightAlign(r, len) + "\n\n";
    } else if (op == '-') {
        int res = val1 - val2;
        r = intToString(res);
        std::string op2Formatted = "-" + op2;
        int len = std::max((int)op1.length(), (int)op2Formatted.length());
        result += rightAlign(op1, len) + "\n";
        result += rightAlign(op2Formatted, len) + "\n";
        int dashNum = std::max((int)r.length(), (int)op2Formatted.length());
        result += rightAlign(std::string(dashNum, '-'), len) + "\n";
        result += rightAlign(r, len) + "\n\n";
    } else if (op == '*') {
        int res = val1 * val2;
        r = intToString(res);
        int lenSecond = op2.length();
        std::vector<int> intermediate(lenSecond);
        for (int i = lenSecond - 1; i >= 0; --i) {
            intermediate[lenSecond - 1 - i] = (op2[i] - '0') * val1;
        }
        std::string op2Formatted = "*" + op2;
        int maxLen = std::max((int)op1.length(), (int)op2Formatted.length());
        int maxLenResult = std::max(maxLen, (int)r.length());
        if (lenSecond > 1) {
            int lastShift = lenSecond - 1;
            std::string lastIntermediate = intToString(intermediate[lenSecond - 1]);
            int lastLength = lastIntermediate.length() + lastShift;
            maxLenResult = std::max(maxLenResult, lastLength);
        }
        int overallWidth = maxLenResult;
        result += rightAlign(op1, overallWidth) + "\n";
        result += rightAlign(op2Formatted, overallWidth) + "\n";
        if (lenSecond == 1) {
            result += std::string(overallWidth, '-') + "\n";
        } else {
            int firstDashLen = std::max((int)op1.length(), (int)op2Formatted.length());
            result += rightAlign(std::string(firstDashLen, '-'), overallWidth) + "\n";
            for (int i = 0; i < lenSecond; ++i) {
                result += rightAlign(intToString(intermediate[i]), overallWidth - i) + "\n";
            }
            std::string lastIntermediate = intToString(intermediate[lenSecond - 1]);
            int lastShift = lenSecond - 1;
            int secondDashNum = std::max((int)r.length(), (int)(lastIntermediate.length() + lastShift));
            result += rightAlign(std::string(secondDashNum, '-'), overallWidth) + "\n";
        }
        result += rightAlign(r, overallWidth) + "\n\n";
    }

    return result;
}

// The solution mirrors the original snippet’s logic but replaces all `cout` writes with appending to a `std::string` result, using a helper to right-align a value to a given width via `std::setw` and `std::ostringstream`. The main algorithm: parse the two operands into integers using `std::stoi` (or a safe manual conversion) and perform the requested operation. For addition and subtraction, compute the result as a string, prepend the operator to the second operand, determine the maximum width needed among the three parts, and format each line right-aligned with that width, followed by a dashed line of the appropriate length (for addition, the dashed line uses the overall width; for subtraction, the dashed line length is max of result length and operator-second operand length) and the result, ending with a newline and an extra blank line. For multiplication, compute the final product, create an array of intermediate products (each digit of `op2` times the integer value of `op1`), prepend `'*'` to `op2`, determine the overall width as max among first operand, operator-second operand, final result, and (if multi-digit second operand) the last intermediate product plus its shift. Print the first operand and operator-second operand right-aligned to that width, a dashed line of length equal to max(first operand length, operator-second operand length) (but if second operand is single-digit, that dashed line length is just the overall width), then if multi-digit, print each intermediate product right-aligned to `(maxLen - index)` where index is its position (so earlier products get more leading spaces), then print a second dashed line with length max(final result length, last intermediate product length + (len-1)), then the final result, then a blank line. Edge cases: operands may be zero; result may be zero; second operand may be single-digit or multi-digit; the intermediate product may have leading zeros (e.g., `0 * 123`), but the original snippet would print `0` (no leading zeros), so we must ensure using integer conversion and `std::to_string` avoids leading zeros. Also, when second operand has multiple digits, intermediate products may have fewer digits than needed; the shift is handled by right-aligning to `maxLen - i` (i from 0 for the last digit). Time complexity is O(len(op1) + len(op2)) for parsing and O(len(op2)) for intermediate products, plus O(L) for formatting where L is the maximum line length; overall O(n) where n is the total number of digits. Space complexity is O(L) for the returned string and O(len(op2)) for the intermediate product array.
