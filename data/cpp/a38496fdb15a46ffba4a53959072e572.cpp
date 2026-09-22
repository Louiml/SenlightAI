// Write a C++ function named `decodeHiddenMessage` that takes a 2D vector of integers representing pixel values and a vector of pairs of integer coordinates (row, column) indicating edge pixels. The function must extract the least significant bit (LSB) from the pixel value at each coordinate in the given order, concatenating these bits into a binary string. If the total number of bits is not divisible by 7, pad the binary string on the left (most significant side) with zeros so that its length becomes a multiple of 7. Then, group the binary string into 7-bit chunks, convert each chunk to its decimal value, and map that decimal value to a printable ASCII character as follows: if the decimal value is less than 33, add 33 to it; if it is greater than 126, clamp it to 126. Append the resulting characters to form the final decoded string, which the function returns. Ensure the function handles empty input gracefully (returning an empty string) and works with arbitrary positive pixel values.
// The solution processes the coordinates in order, extracting the LSB for each pixel using modulo 2. These bits are appended to a string. If the string length is not a multiple of 7, we compute the remainder `r = length % 7` and insert `r` zeros at the beginning (e.g., if length is 9, remainder is 2, so we insert "00" at the front to make length 14). Then we iterate over the string in steps of 7, using `substr` to get each chunk. Convert the binary substring to decimal using `std::stoi` with base 2. Apply the mapping: for values < 33, add 33; for values > 126, set to 126; otherwise keep as is. Cast the integer to a char and append. Edge cases: empty edgePixels returns an empty string; if after padding, the number of chunks is zero (which cannot happen if length > 0, but if length is 0, we return early). The time complexity is O(n) where n is the number of edge pixels, and O(n) space for the bit string and result. The mapping ensures all output characters are printable ASCII (33–126).
#include <string>
#include <vector>
#include <utility>

// Decode a hidden message from LSBs of given pixel coordinates.
// Pixel values are non-negative integers; coordinates are (row, column).
// Returns the decoded string; empty input yields an empty string.
std::string decodeHiddenMessage(const std::vector<std::vector<int>>& image,
                                const std::vector<std::pair<int, int>>& edgePixels) {
    if (edgePixels.empty()) {
        return "";
    }

    std::string lsbString;
    for (const auto& coord : edgePixels) {
        int pixelValue = image[coord.first][coord.second];
        int lsb = pixelValue % 2;
        lsbString += (lsb == 0) ? '0' : '1';
    }

    // Pad on the left to make length a multiple of 7
    size_t remainder = lsbString.size() % 7;
    if (remainder != 0) {
        lsbString.insert(0, 7 - remainder, '0');
    }

    std::string decoded;
    for (size_t i = 0; i < lsbString.size(); i += 7) {
        std::string chunk = lsbString.substr(i, 7);
        int decimalValue = std::stoi(chunk, nullptr, 2);
        if (decimalValue < 33) {
            decimalValue += 33;
        } else if (decimalValue > 126) {
            decimalValue = 126;
        }
        decoded += static_cast<char>(decimalValue);
    }

    return decoded;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// The solution function is declared above; include its definition here.

int main() {
    // Example 1: Two pixels, LSBs "10" -> pad to "0000010" (7 bits) -> value 2 -> 2<33 -> 35 -> '#'
    std::vector<std::vector<int>> img1 = {{100, 101}};
    std::vector<std::pair<int, int>> coords1 = {{0,0}, {0,1}};
    assert(decodeHiddenMessage(img1, coords1) == "#");

    // Example 2: Seven pixels all odd -> LSBs "1111111" -> value 127 -> clamp to 126 -> '~'
    std::vector<std::vector<int>> img2 = {{3,3,3,3,3,3,3}};
    std::vector<std::pair<int, int>> coords2;
    for (int i = 0; i < 7; ++i) coords2.push_back({0, i});
    assert(decodeHiddenMessage(img2, coords2) == "~");

    // Example 3: Empty input
    std::vector<std::vector<int>> img3 = {{1}};
    std::vector<std::pair<int, int>> coords3;
    assert(decodeHiddenMessage(img3, coords3) == "");

    // Example 4: Bits "0000001" (one odd pixel at end) -> value 1 -> 1+33=34 -> '"'
    std::vector<std::vector<int>> img4 = {{2, 3}};
    std::vector<std::pair<int, int>> coords4 = {{0,0}, {0,1}};
    assert(decodeHiddenMessage(img4, coords4) == "\"");

    // Example 5: Bits "1000000" (first odd, rest even) -> pad? length=7 no pad -> value 64 -> 64 is in range -> '@'
    std::vector<std::vector<int>> img5 = {{5, 4, 4, 4, 4, 4, 4}};
    std::vector<std::pair<int, int>> coords5;
    for (int i = 0; i < 7; ++i) coords5.push_back({0, i});
    assert(decodeHiddenMessage(img5, coords5) == "@");

    // Example 6: 14 bits "01010101010101" -> two chunks "0101010" (42) and "1010101" (85) -> no clamp -> '*' and 'U'
    std::vector<std::vector<int>> img6 = {{0,1,0,1,0,1,0,1,0,1,0,1,0,1}};
    std::vector<std::pair<int, int>> coords6;
    for (int i = 0; i < 14; ++i) coords6.push_back({0, i});
    assert(decodeHiddenMessage(img6, coords6) == "*U");
}
