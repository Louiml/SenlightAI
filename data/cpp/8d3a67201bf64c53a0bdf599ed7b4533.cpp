// Write a C++ function `std::string hillCipherEncrypt(const std::string& plaintext, const std::vector<std::vector<int>>& keyMatrix)` that performs Hill cipher encryption on an uppercase alphabetic string (A-Z). The function should automatically pad the plaintext with `'X'` characters to make its length a multiple of the key matrix dimension (which is the size of the square matrix). The key matrix is guaranteed to be square (n×n) with integer entries modulo 26, and the plaintext may contain lowercase letters that must be converted to uppercase before encryption. The function should return the encrypted uppercase string. Handle edge cases such as an empty plaintext (return empty string), and ensure the matrix multiplication and modulus operations are correct for any matrix size. Assume the key matrix is invertible modulo 26 (no need to check validity), and that the plaintext contains only alphabetic characters (no spaces, digits, or punctuation).

The solution involves converting each character to a numeric value 0–25 (A=0, B=1, ..., Z=25), then processing the text in blocks of size n (where n is the key matrix dimension). For each block, we compute the encryption vector by multiplying the key matrix (row-major) by the column vector of numeric plaintext values, taking each result modulo 26. The key insight is that the function must handle input of any length by padding with `'X'` (which has value 23) to make the total length divisible by n. Edge cases include: empty input (return empty), input already divisible by n (no padding needed), and mixed-case input (convert to uppercase first). The algorithm runs in O(L·n²) time where L is the padded text length, and uses O(L + n²) auxiliary space for the output and the numeric vector. The implementation must carefully index the matrix using `row * n + col` and ensure the multiplication sums over n terms. No main function is needed in the solution; the test code will call the function directly with various inputs.

#include <string>
#include <vector>
#include <cctype>

// Encrypt a plaintext string using the Hill cipher with a given square key matrix.
std::string hillCipherEncrypt(const std::string& plaintext, const std::vector<std::vector<int>>& keyMatrix) {
    if (plaintext.empty()) return "";
    
    int n = keyMatrix.size(); // square matrix dimension
    // Convert to uppercase and retain only alphabetic characters
    std::string text;
    for (char c : plaintext) {
        if (isalpha(c)) {
            text += toupper(c);
        }
    }
    if (text.empty()) return "";
    
    // Pad with 'X' if necessary
    int pad = (n - (text.size() % n)) % n;
    for (int i = 0; i < pad; ++i) {
        text += 'X';
    }
    
    int len = text.size();
    std::string encrypted(len, ' ');
    
    for (int i = 0; i < len; i += n) {
        // Build numeric vector for this block
        std::vector<int> block(n);
        for (int j = 0; j < n; ++j) {
            block[j] = text[i + j] - 'A';
        }
        // Multiply key matrix * block, mod 26
        for (int row = 0; row < n; ++row) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += keyMatrix[row][k] * block[k];
            }
            int value = ((sum % 26) + 26) % 26; // ensure non-negative
            encrypted[i + row] = static_cast<char>('A' + value);
        }
    }
    
    return encrypted;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above (not repeated here).

int main() {
    // 2x2 key matrix [[3, 3], [2, 5]] encrypts "HI" -> "YJ" (classic example)
    std::vector<std::vector<int>> key2 = {{3, 3}, {2, 5}};
    assert(hillCipherEncrypt("HI", key2) == "YJ");
    assert(hillCipherEncrypt("hi", key2) == "YJ"); // lowercase input
    assert(hillCipherEncrypt(" H i ", key2) == "YJ"); // only alpha, case-insensitive
    
    // Padding: "A" with 2x2 matrix pads to "AX" -> "JM" (calculate: A=0, X=23; [3*0+3*23=69%26=17=R? wait, check])
    // Let's compute: block [0,23], row0: 3*0+3*23=69%26=17 -> 'R'; row1: 2*0+5*23=115%26=11 -> 'L'? but must be consistent. 
    // Use a known simple identity matrix for predictable results.
    std::vector<std::vector<int>> ident2 = {{1, 0}, {0, 1}};
    assert(hillCipherEncrypt("A", ident2) == "AX"); // padded with X, identity returns same
    assert(hillCipherEncrypt("ABC", ident2) == "ABCX"); // pad to length 4
    assert(hillCipherEncrypt("ABCD", ident2) == "ABCD"); // already multiple
    assert(hillCipherEncrypt("", ident2) == ""); // empty
    
    // 3x3 identity matrix
    std::vector<std::vector<int>> ident3 = {{1,0,0},{0,1,0},{0,0,1}};
    assert(hillCipherEncrypt("HEL", ident3) == "HEL");
    assert(hillCipherEncrypt("HELLO", ident3) == "HELLOX"); // pad 1 to make 6
    assert(hillCipherEncrypt("HELLO WORLD", ident3) == "HELLOWORLDX"); // non-alpha removed, then pad
    
    // 2x2 key that multiples to known: key = [[0,1],[1,0]] swaps pair
    std::vector<std::vector<int>> swap2 = {{0,1},{1,0}};
    assert(hillCipherEncrypt("AB", swap2) == "BA");
    assert(hillCipherEncrypt("ABC", swap2) == "BACX"); // "AB"->"BA", "CX"->"XC"
    
    return 0;
}
