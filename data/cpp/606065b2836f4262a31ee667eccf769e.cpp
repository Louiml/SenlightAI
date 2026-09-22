Write a C++ function `std::string convertSpreadsheetReference(const std::string& ref)` that converts between two common spreadsheet cell reference formats. The input is a non-empty string representing a cell address in one of two formats: (1) Excel-style like `"BC23"` (column letters followed by row number, where columns are A, B, ..., Z, AA, AB, ...) or (2) R1C1-style like `"R23C55"` (row number immediately after 'R', column number immediately after 'C'). If the input matches the R1C1 pattern (starts with 'R', second character is a digit, and contains a 'C' later), convert it to Excel-style (e.g., `"R23C55"` → `"BC23"`). Otherwise, assume it is Excel-style and convert it to R1C1-style (e.g., `"BC23"` → `"R23C55"`). The input is guaranteed to be valid and well‑formed for one of these formats; row and column numbers are positive integers fitting in a 32‑bit signed integer. Return the converted string.
#include <cassert>

int main() {
    // Basic conversions.
    assert(convertSpreadsheetReference("A1") == "R1C1");
    assert(convertSpreadsheetReference("R1C1") == "A1");
    assert(convertSpreadsheetReference("Z1") == "R1C26");
    assert(convertSpreadsheetReference("R1C26") == "Z1");

    // Multi-letter columns and larger rows.
    assert(convertSpreadsheetReference("AA1") == "R1C27");
    assert(convertSpreadsheetReference("R1C27") == "AA1");
    assert(convertSpreadsheetReference("BC23") == "R23C55");
    assert(convertSpreadsheetReference("R23C55") == "BC23");

    // Larger numbers.
    assert(convertSpreadsheetReference("XFD1048576") == "R1048576C16384");
    assert(convertSpreadsheetReference("R1048576C16384") == "XFD1048576");

    // Edge case with row and column both equal 1.
    assert(convertSpreadsheetReference("R1C1") == "A1");
    assert(convertSpreadsheetReference("A1") == "R1C1");

    return 0;
}
#include <string>
#include <algorithm>
#include <cctype>

// Convert between Excel-style (e.g., "BC23") and R1C1-style (e.g., "R23C55") spreadsheet references.
std::string convertSpreadsheetReference(const std::string& ref) {
    // Detect R1C1 format: starts with 'R', second char is digit, and a 'C' appears later.
    bool isR1C1 = false;
    if (ref.size() >= 2 && ref[0] == 'R' && std::isdigit(static_cast<unsigned char>(ref[1]))) {
        if (ref.find('C', 2) != std::string::npos) {
            isR1C1 = true;
        }
    }

    if (isR1C1) {
        // Parse row and column from "R<row>C<col>".
        size_t cPos = ref.find('C', 2);
        int row = 0;
        for (size_t i = 1; i < cPos; ++i) {
            row = row * 10 + (ref[i] - '0');
        }
        int col = 0;
        for (size_t i = cPos + 1; i < ref.size(); ++i) {
            col = col * 10 + (ref[i] - '0');
        }

        // Convert column number to Excel letters.
        std::string letters;
        while (col > 0) {
            char ch = static_cast<char>((col - 1) % 26 + 'A');
            letters.push_back(ch);
            col = (col - 1) / 26;
        }
        std::reverse(letters.begin(), letters.end());

        // Append row number.
        return letters + std::to_string(row);
    } else {
        // Parse Excel-style: leading letters column, then row number.
        size_t i = 0;
        int col = 0;
        while (i < ref.size() && std::isalpha(static_cast<unsigned char>(ref[i]))) {
            col = col * 26 + (ref[i] - 'A' + 1);
            ++i;
        }
        int row = 0;
        for (; i < ref.size(); ++i) {
            row = row * 10 + (ref[i] - '0');
        }
        return "R" + std::to_string(row) + "C" + std::to_string(col);
    }
}
// The solution must detect the input format first. The detection rule from the given code is: if the string starts with `'R'`, the second character (if exists) is a digit, and there is a `'C'` at position ≥ 2, then it is R1C1 format; otherwise it is Excel format. For R1C1 conversion: parse the number after `'R'` as the row (an integer) and the number after `'C'` as the column (an integer). Then convert the column integer to Excel column letters by repeatedly applying `(col-1) % 26` to get the last letter and `(col-1)/26` to reduce the column; build the string by prepending each letter. Finally, append the row number. For Excel conversion: scan the leading alphabetic characters as column letters, converting each to a running integer with `column = column*26 + (letter - 'A' + 1)`. The remaining characters form the row number. Then output `"R" + row + "C" + column`. Edge cases: single-letter columns (e.g., `"A1"` → `"R1C1"`), multi-letter columns (e.g., `"Z1"` → `"R1C26"`, `"AA1"` → `"R1C27"`), and large columns up to the max int. The algorithm is O(L) where L is the length of the input string, and O(L) auxiliary space for the output string. No special handling is needed for valid inputs.
