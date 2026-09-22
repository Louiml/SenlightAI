/*
Write a C++ function `decodeHuffman` that takes three parameters: a string `encodedData` containing binary bits representing a Huffman-encoded message (with possible padding at the end), and two vectors: `symbols` (characters) and `codes` (binary strings, each corresponding to the symbol at the same index), representing the Huffman codebook. The function must return the original decoded message as a string. The codebook is complete and prefix-free (no code is a prefix of another). The encoded data may contain extra padding bits at the end (the function should ignore them); the actual message ends exactly when the Huffman tree traversal reaches a leaf and the remaining bits are all padding (in practice, since the codebook is prefix-free, you can decode until no further complete symbol can be formed from remaining bits—but the padding is guaranteed to be all '0's and will not form a valid code; so decode only while you can match a complete code). Ensure the function handles empty encodedData (returning an empty string) and that it works with codes of arbitrary lengths (including length 1). The function should be efficient and use a hash map for code lookup.
*/
#include <string>
#include <vector>
#include <unordered_map>

// Decode a Huffman-encoded binary string given a codebook.
// Parameters:
//   encodedData - binary string of '0' and '1' characters, possibly with padding at the end
//   symbols     - vector of characters corresponding to codes
//   codes       - vector of binary strings, each a Huffman code for symbols[i]
// Returns the original decoded message.
std::string decodeHuffman(const std::string& encodedData,
                          const std::vector<char>& symbols,
                          const std::vector<std::string>& codes) {
    // Build a hash map from code -> symbol
    std::unordered_map<std::string, char> codeToSymbol;
    for (size_t i = 0; i < symbols.size(); ++i) {
        codeToSymbol[codes[i]] = symbols[i];
    }

    std::string result;
    std::string currentPrefix;

    // Process each bit; when a complete code is matched, output the symbol
    // and reset the prefix.
    for (char bit : encodedData) {
        currentPrefix += bit;
        auto it = codeToSymbol.find(currentPrefix);
        if (it != codeToSymbol.end()) {
            result += it->second;
            currentPrefix.clear();
        }
    }

    // Any remaining bits in currentPrefix are padding and are ignored.
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above; here we test it directly.
int main() {
    // Basic example: codes "0" and "1"
    std::vector<char> sym1 = {'A', 'B'};
    std::vector<std::string> codes1 = {"0", "1"};
    assert(decodeHuffman("0101", sym1, codes1) == "ABAB");

    // Multi-bit codes, no padding
    std::vector<char> sym2 = {'a', 'b', 'c', 'd'};
    std::vector<std::string> codes2 = {"00", "01", "10", "11"};
    assert(decodeHuffman("001011", sym2, codes2) == "abcd");

    // Unequal code lengths, with padding (trailing zeros that do not form a code)
    std::vector<char> sym3 = {'X', 'Y', 'Z'};
    std::vector<std::string> codes3 = {"0", "10", "11"};
    // Encoded: X=0, Y=10, Z=11 => "011011"; add two padding zeros at end => "01101100"
    assert(decodeHuffman("01101100", sym3, codes3) == "XZYZ");

    // Single symbol, code "0", with padding
    std::vector<char> sym4 = {'Q'};
    std::vector<std::string> codes4 = {"0"};
    assert(decodeHuffman("000", sym4, codes4) == "QQQ");

    // Empty encoded data
    assert(decodeHuffman("", sym1, codes1) == "");

    // Code of length 1 and length 2 mixed
    std::vector<char> sym5 = {'E', 'F', 'G'};
    std::vector<std::string> codes5 = {"0", "10", "11"};
    // Message "EFG" => "0 10 11" => "01011"; add one padding bit '0' => "010110"
    assert(decodeHuffman("010110", sym5, codes5) == "EFG");

    // Longer message with many symbols, padding not present
    std::vector<char> sym6 = {'A', 'B', 'C'};
    std::vector<std::string> codes6 = {"00", "01", "1"};
    // "AABBCC" => "00 00 01 01 1 1" => "0000010111"
    assert(decodeHuffman("0000010111", sym6, codes6) == "AABBCC");

    // Codes where one is a single bit and others are longer, with padding
    std::vector<char> sym7 = {'a', 'b', 'c', 'd'};
    std::vector<std::string> codes7 = {"0", "10", "110", "111"};
    // "abcd" => "0 10 110 111" => "010110111"; add two zeros => "01011011100"
    assert(decodeHuffman("01011011100", sym7, codes7) == "abcd");

    // Only one code and message is just that repeated
    std::vector<char> sym8 = {'M'};
    std::vector<std::string> codes8 = {"1"};
    assert(decodeHuffman("111", sym8, codes8) == "MMM");

    // Mixed with empty symbol (should still work, but not typical; still test)
    // Actually Huffman codes should not have empty string; skip.

    // Stress: many symbols but same length
    std::vector<char> sym9 = {'0', '1', '2', '3', '4', '5', '6', '7'};
    std::vector<std::string> codes9 = {"000", "001", "010", "011", "100", "101", "110", "111"};
    // "01234567" => "000001010011100101110111"
    assert(decodeHuffman("000001010011100101110111", sym9, codes9) == "01234567");

    return 0;
}
// The main idea is to greedily read bits from the encoded string and match them against the codebook. Since Huffman codes are prefix-free, scanning bit-by-bit from left to right and checking whether the accumulated prefix is exactly one of the given codes will yield a unique symbol each time we reach a complete code. Start with an empty prefix and an empty result. For each character in `encodedData`, append it to the prefix. After each append, check if the prefix exists as a code in the hash map. If yes, append the corresponding symbol to the result and reset the prefix to an empty string. When the input ends, any remaining prefix bits are padding and should be ignored. The critical edge case is when a code is length 1 (like "0" or "1")—the algorithm handles this naturally because it checks after every bit. Another edge case: when the encoded data is empty, the return is an empty string. Time complexity is O(n) where n is the length of `encodedData`, because each bit is processed once, and hash map lookups are O(1) average. Space complexity is O(m) for the hash map plus O(k) for the result, where m is the number of symbols and k is the length of the decoded message. The approach is straightforward and does not require building a tree explicitly.
