Implement a C++ function `playfair_transform` that takes four parameters: a keyword string `key`, a plaintext/ciphertext string `text`, a boolean `use_i` indicating whether the letter J should be merged into I (true) or Q should be omitted (false), and a boolean `encrypt` indicating whether to encrypt (true) or decrypt (false). The function must build a 5x5 Playfair cipher table from the keyword, process the input according to the Playfair rules (using I/J merging or Q omission as specified), and return the transformed text as a string (uppercase, without spaces or non-letters). For encryption, insert 'X' between duplicate letters in a digraph and append 'X' if the processed text has odd length; for decryption, take the input as pre-processed (digraphs ready) and output the raw decrypted pairs without removing any 'X' padding. The function should not read/write files and must be fully self-contained.
The solution begins by building the 5x5 table: start with the keyword (converted to uppercase, filtering non-letters, handling J→I or omitting Q based on `use_i`), then append remaining alphabet letters in order, skipping duplicates and the same letter exclusions. Store the table as a 2D array of char. Next, preprocess the input text: convert to uppercase, remove non-letters, apply J→I or Q-removal filtering. If encrypting, iterate in pairs, inserting 'X' between duplicate letters within a pair (e.g., "HELLO" → "HE LX LO"), then if the final string length is odd, append 'X'. If decrypting, no extra preprocessing is needed beyond the initial filtering (assume input is properly paired). Then process digraphs: for each pair, find their positions in the table. If same row, shift columns right (+1) for encryption or left (-1) for decryption, wrapping around. If same column, shift rows similarly. Otherwise, form a rectangle: the first character takes the row of the first and column of the second, the second takes the row of the second and column of the first. Concatenate results and return. Edge cases: empty key (use "KEYWORD"), empty text (return empty), strings with odd length after filtering (handled only in encryption; decryption assumes even length, but we can still process all full pairs and ignore any leftover single character for robustness). Time complexity: O(n) for preprocessing and O(n) for table lookups, where n is the input length. Space: O(1) for the table plus the output string.
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

// Build the 5x5 Playfair matrix from a keyword
void buildMatrix(const std::string& key, bool use_i, char matrix[5][5]) {
    std::string s;
    std::string full = key;
    if (full.empty()) full = "KEYWORD";
    full += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char ch : full) {
        ch = std::toupper(static_cast<unsigned char>(ch));
        if (ch < 'A' || ch > 'Z') continue;
        if (use_i && ch == 'J') continue;
        if (!use_i && ch == 'Q') continue;
        if (s.find(ch) == std::string::npos) s += ch;
    }
    // Fill the matrix row-major
    for (int i = 0; i < 25; ++i) {
        matrix[i / 5][i % 5] = s[i];
    }
}

// Find position of a character in the matrix, returns true if found
bool findPosition(const char matrix[5][5], char c, int& row, int& col) {
    for (int r = 0; r < 5; ++r) {
        for (int cc = 0; cc < 5; ++cc) {
            if (matrix[r][cc] == c) {
                row = r;
                col = cc;
                return true;
            }
        }
    }
    return false;
}

// Main transform function
std::string playfair_transform(const std::string& key, const std::string& text,
                               bool use_i, bool encrypt) {
    char matrix[5][5];
    buildMatrix(key, use_i, matrix);

    // Preprocess text: clean and filter
    std::string cleaned;
    for (char ch : text) {
        ch = std::toupper(static_cast<unsigned char>(ch));
        if (ch < 'A' || ch > 'Z') continue;
        if (use_i && ch == 'J') ch = 'I';
        else if (!use_i && ch == 'Q') continue;
        cleaned += ch;
    }

    // If encrypting, prepare digraphs
    if (encrypt) {
        std::string ready;
        for (size_t i = 0; i < cleaned.length(); ++i) {
            ready += cleaned[i];
            if (i + 1 < cleaned.length() && cleaned[i] == cleaned[i + 1]) {
                ready += 'X';
            } else if (i + 1 == cleaned.length()) {
                // last character, will add X if odd later
                break;
            }
            // continue normally, but we need to handle pairs differently
        }
        // Better approach: build pairs properly
        ready.clear();
        for (size_t i = 0; i < cleaned.length();) {
            ready += cleaned[i];
            if (i + 1 < cleaned.length()) {
                if (cleaned[i] == cleaned[i + 1]) {
                    ready += 'X';
                    ++i; // consume only first char, keep second for next pair
                } else {
                    ready += cleaned[i + 1];
                    i += 2;
                }
            } else {
                ready += 'X';
                ++i;
            }
        }
        if (ready.length() % 2 != 0) ready += 'X';
        cleaned = ready;
    }

    // Process digraphs
    std::string result;
    for (size_t i = 0; i + 1 < cleaned.length(); i += 2) {
        char a = cleaned[i];
        char b = cleaned[i + 1];
        int r1, c1, r2, c2;
        if (!findPosition(matrix, a, r1, c1)) continue;
        if (!findPosition(matrix, b, r2, c2)) continue;
        int dir = encrypt ? 1 : -1;
        if (r1 == r2) {
            result += matrix[r1][(c1 + dir + 5) % 5];
            result += matrix[r2][(c2 + dir + 5) % 5];
        } else if (c1 == c2) {
            result += matrix[(r1 + dir + 5) % 5][c1];
            result += matrix[(r2 + dir + 5) % 5][c2];
        } else {
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above (in actual source, include it)

int main() {
    // Basic encryption with default keyword
    assert(playfair_transform("KEYWORD", "HELLO", true, true) == "HELXLO");

    // Decryption roundtrip
    std::string enc = playfair_transform("MONARCHY", "WEAREDISCOVEREDSAVEYOURSELF", true, true);
    assert(playfair_transform("MONARCHY", enc, true, false) == "WEAREDISCOVEREDSAVEYOURSELF");

    // J to I merging
    assert(playfair_transform("PLAYFAIR", "JAZZ", true, true) == "IAZXIZ");

    // Q omitted when use_i is false
    assert(playfair_transform("TEST", "QUEEN", false, true).find('Q') == std::string::npos);

    // Empty key uses default
    assert(playfair_transform("", "HI", true, true) == "BM");

    // Single letter gets padded
    assert(playfair_transform("KEY", "A", true, true).length() == 2);

    // Non-letters are filtered
    assert(playfair_transform("KEY", "A!B C", true, true).length() == 4);

    // Decryption of empty string
    assert(playfair_transform("KEY", "", true, false) == "");

    // Decryption with odd length ignores trailing char (robust)
    assert(playfair_transform("KEY", "AB", true, false) == "AB");

    // Same row wrap-around
    assert(playfair_transform("ABCDEFGHIKLMNOPQRSTUVWXYZ", "AE", true, true) == "BF");

    // Same column wrap-around
    assert(playfair_transform("ABCDEFGHIKLMNOPQRSTUVWXYZ", "AG", true, true) == "BH");

    // Rectangle rule
    assert(playfair_transform("ABCDEFGHIKLMNOPQRSTUVWXYZ", "AC", true, true) == "BD");

    return 0;
}
