Write a C++ function named `railFenceEncrypt` that takes a non-empty string `plaintext` and an integer `rails` (where `rails > 0`) and returns the rail fence ciphertext. The encryption rule is: split the plaintext into `rails` consecutive "rails" (each rail is a contiguous block of characters, not a zigzag). More precisely, for rail index `i` (from 0 to `rails-1`), take characters at positions `i`, `i + railSize`, `i + 2*railSize`, ..., where `railSize = length(plaintext) / rails` (integer division). If the plaintext length is not divisible by `rails`, the last rail (or rails) may be shorter because the formula uses integer division, and you must handle the possibility that `railSize` could be 0 when `rails > len` (in that case, just take each character as its own rail). The function must be pure (no input/output, no side effects) and respect `const` correctness for the input string. You are also to write a companion function `railFenceDecrypt` that inverts the encryption under the same parameters, restoring the original plaintext. Assume `rails` is always positive; if `rails` is 0, you may return an empty string or treat it as 1.

// The rail fence cipher in this problem is a columnar transposition. The encryption writes the plaintext into a 2D grid with `rails` rows and `ceil(len/rails)` columns, but only fills columns sequentially left-to-right, top-to-bottom. Actually the provided snippet uses `j += railSize`, which is not the standard zigzag but a "block" transposition. The key is that `railSize = len / rails`. For rail `i`, the indices taken are `i, i+railSize, i+2*railSize, ...` as long as they are `< len`. When `len` is not a multiple of `rails`, the last row (or rows) may have fewer elements, but the pattern still works because `j < len` stops early. For decryption, we need to reverse the mapping: we know the ciphertext is read rail by rail. We can compute the lengths of each rail: for rail `i`, the number of elements is `count = (len - i + railSize - 1) / railSize`? Actually simpler: for each rail `i`, the indices are `i`, `i+railSize`, `i+2*railSize`, ... so the count is `floor((len-1-i)/railSize)+1` if `railSize > 0`. If `railSize == 0` (when `rails > len`), then each rail gets exactly one character (indices `0..rails-1` for the first `len` rails). A robust approach for decryption: create a vector of strings for each rail, split the ciphertext into contiguous chunks of those sizes, then iterate over the original plaintext positions and pick from the correct rail position. Alternatively, we can precompute the mapping: build a table `railOfPosition` for each original index (`j % railSize` gives rail index if railSize>0, but careful with last partial column). Actually better: for encryption, for original index `pos`, the rail index is `pos % railSize` when `railSize > 0`, but if `pos/railSize` is the last column and `railSize` doesn't divide evenly, the rail count might be shorter. A simpler and correct method: for decryption, we know the exact order of indices used in encryption. We can enumerate all original indices `0..len-1`, and sort them by the order they were output (rail number then by index within rail). Store the ciphertext characters accordingly. This is O(n log n) but simpler to implement correctly. Given small constraints, this is fine. Time complexity: O(n log n) for sorting, or O(n) using counting. Space O(n). Edge cases: `rails=1` returns the original string; `rails > len` should produce a ciphertext that is just the original string but the decryption must handle `railSize=0` — we can treat `railSize=1` as fallback or use the condition `if (railSize == 0) railSize = 1;` because when `railSize=0`, the loop `for (int j=i; j<len; j+=railSize)` would be infinite, so we must avoid that. In the provided snippet, this is a bug, but our solution must handle it safely. A safe way: if `railSize == 0`, then each rail gets at most one character: rail `i` gets plaintext[i] if `i < len`. So encryption is just the original string (since rails are read in order 0,1,2,... and each has one char). For decryption with `railSize==0`, the ciphertext is identical to plaintext, so we can just return ciphertext.

#include <string>
#include <vector>
#include <algorithm>
#include <cstddef>

// Encrypts a string using the rail fence (block) cipher.
// Each rail takes characters at indices i, i+railSize, i+2*railSize, ...
// where railSize = plaintext.length() / rails (integer division).
std::string railFenceEncrypt(const std::string& plaintext, int rails) {
    if (rails <= 0) return "";
    if (rails == 1 || plaintext.empty()) return plaintext;

    std::size_t len = plaintext.size();
    std::size_t railSize = len / static_cast<std::size_t>(rails);

    // If railSize is 0, each rail gets one character (or none if len < rails)
    if (railSize == 0) {
        // For each rail i, take plaintext[i] if exists.
        std::string result;
        for (std::size_t i = 0; i < len; ++i) {
            result += plaintext[i];
        }
        return result;
    }

    std::string ciphertext;
    for (std::size_t i = 0; i < static_cast<std::size_t>(rails); ++i) {
        for (std::size_t j = i; j < len; j += railSize) {
            ciphertext += plaintext[j];
        }
    }
    return ciphertext;
}

// Decrypts a string that was encrypted with railFenceEncrypt.
// Reconstructs the original plaintext by reversing the index ordering.
std::string railFenceDecrypt(const std::string& ciphertext, int rails) {
    if (rails <= 0 || ciphertext.empty()) return "";
    if (rails == 1) return ciphertext;

    std::size_t len = ciphertext.size();
    std::size_t railSize = len / static_cast<std::size_t>(rails);

    // When railSize == 0, encryption was identity (since each rail got one char).
    if (railSize == 0) {
        // The ciphertext is the same as plaintext, so just return as is.
        return ciphertext;
    }

    // Determine which rail each original position belongs to and order.
    // We'll collect pairs (order_index, original_position) where order_index
    // is the position in the ciphertext after encryption.
    std::vector<std::pair<std::size_t, std::size_t>> order;
    std::size_t orderIndex = 0;
    for (std::size_t i = 0; i < static_cast<std::size_t>(rails); ++i) {
        for (std::size_t j = i; j < len; j += railSize) {
            order.push_back({orderIndex++, j});
        }
    }

    // Build a vector of original length, fill it from ciphertext.
    std::string plaintext(len, ' ');
    for (const auto& p : order) {
        plaintext[p.second] = ciphertext[p.first];
    }
    return plaintext;
}

#include <cassert>
#include <string>

std::string railFenceEncrypt(const std::string& plaintext, int rails);
std::string railFenceDecrypt(const std::string& ciphertext, int rails);

int main() {
    // Example from snippet: plaintext "HELLO", rails=3 -> railSize=2 (ceil(5/3)=2)
    assert(railFenceEncrypt("HELLO", 3) == "HLOEL");
    assert(railFenceDecrypt("HLOEL", 3) == "HELLO");

    // Even divisibility: "ABCDEF", rails=3 -> railSize=2
    assert(railFenceEncrypt("ABCDEF", 3) == "ADBECF");
    assert(railFenceDecrypt("ADBECF", 3) == "ABCDEF");

    // rails=1 returns original
    assert(railFenceEncrypt("TEST", 1) == "TEST");
    assert(railFenceDecrypt("TEST", 1) == "TEST");

    // rails larger than string length
    assert(railFenceEncrypt("ABC", 5) == "ABC"); // each rail gets one character
    assert(railFenceDecrypt("ABC", 5) == "ABC");

    // Empty string
    assert(railFenceEncrypt("", 3) == "");
    assert(railFenceDecrypt("", 3) == "");

    // Single character
    assert(railFenceEncrypt("Z", 4) == "Z");
    assert(railFenceDecrypt("Z", 4) == "Z");

    // Check roundtrip for a longer string
    std::string original = "THEQUICKBROWNFOXJUMPS";
    for (int r = 2; r <= 10; ++r) {
        std::string enc = railFenceEncrypt(original, r);
        assert(railFenceDecrypt(enc, r) == original);
    }
}
