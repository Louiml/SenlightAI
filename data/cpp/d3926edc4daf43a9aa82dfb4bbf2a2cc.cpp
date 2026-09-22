Implement a C++ function that validates whether a byte vector represents a canonical ECDSA signature encoding according to the strict DER rules used by Bitcoin's script interpreter. The function should check the overall structure (starting with 0x30, proper length fields, integer tags 0x02, correct total length), enforce that the R and S values are positive (no negative numbers, no unnecessary leading zero bytes), and verify that the signature size falls within the allowed range of 9 to 73 bytes inclusive. The function takes a `const std::vector<unsigned char>&` as input and returns `true` if the encoding is valid, `false` otherwise.

#include <cassert>
#include <vector>

// Declaration of the function under test (assumed to be provided)
bool IsValidSignatureEncoding(const std::vector<unsigned char>& sig);

int main() {
    // Valid signature with R length 1, S length 1, sighash byte 0x01
    std::vector<unsigned char> valid1 = {0x30, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(valid1) == true);

    // Valid signature with R length 32, S length 32, sighash byte 0x01 (common length)
    std::vector<unsigned char> valid2 = {0x30, 0x44, 0x02, 0x20};
    valid2.insert(valid2.end(), 32, 0x11); // R value
    valid2.push_back(0x02);
    valid2.push_back(0x20);
    valid2.insert(valid2.end(), 32, 0x22); // S value
    valid2.push_back(0x01); // sighash
    assert(valid2.size() == 9 + 32 + 32);
    assert(IsValidSignatureEncoding(valid2) == true);

    // Too short (8 bytes)
    std::vector<unsigned char> short_sig = {0x30, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01};
    assert(IsValidSignatureEncoding(short_sig) == false);

    // Too long (74 bytes)
    std::vector<unsigned char> long_sig(74, 0x00);
    long_sig[0] = 0x30;
    long_sig[1] = 0x46; // length = 73 - 3? Actually 74 total, so length field should be 71
    // This will fail both size check first
    assert(IsValidSignatureEncoding(long_sig) == false);

    // Wrong starting byte (not 0x30)
    std::vector<unsigned char> wrong_tag = {0x31, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(wrong_tag) == false);

    // Length field does not match actual size
    std::vector<unsigned char> wrong_len = {0x30, 0x05, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(wrong_len) == false);

    // R is negative (first byte has high bit set)
    std::vector<unsigned char> negative_r = {0x30, 0x06, 0x02, 0x01, 0x80, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(negative_r) == false);

    // R has unnecessary leading zero byte (zero followed by a non-negative byte)
    std::vector<unsigned char> padded_r = {0x30, 0x07, 0x02, 0x02, 0x00, 0x01, 0x02, 0x01, 0x01, 0x01};
    // Size = 10, lenR=2, lenS=1, total = 2+1+7=10, but leading zero with next byte 0x01 (not high bit set)
    assert(IsValidSignatureEncoding(padded_r) == false);

    // R has leading zero followed by high-bit byte (allowed)
    std::vector<unsigned char> allowed_padded_r = {0x30, 0x07, 0x02, 0x02, 0x00, 0x80, 0x02, 0x01, 0x01, 0x01};
    // lenR=2, first byte 0x00, second byte 0x80 (high bit set) => allowed
    // Total: 2+1+7=10, size=10, starts with 0x30, length field = 7 = 10-3
    assert(IsValidSignatureEncoding(allowed_padded_r) == true);

    // S has zero length
    std::vector<unsigned char> zero_s = {0x30, 0x05, 0x02, 0x01, 0x01, 0x02, 0x00, 0x01};
    // size=8, fails minimum size first
    assert(IsValidSignatureEncoding(zero_s) == false);

    // S is negative
    std::vector<unsigned char> negative_s = {0x30, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x81, 0x01};
    assert(IsValidSignatureEncoding(negative_s) == false);

    // R tag is not 0x02
    std::vector<unsigned char> wrong_r_tag = {0x30, 0x06, 0x03, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(wrong_r_tag) == false);

    return 0;
}

#include <vector>
#include <cstddef>

/**
 * Validate that the given byte vector is a canonical DER-encoded ECDSA signature.
 * The format is: 0x30 [total-length] 0x02 [R-length] [R] 0x02 [S-length] [S] [sighash]
 * Rules enforced:
 * - Size between 9 and 73 bytes inclusive.
 * - Starts with 0x30 (compound tag).
 * - Length byte matches signatures size minus 3.
 * - R and S are positive integers with minimal encoding (no unnecessary leading zeros).
 * - R and S lengths are non-zero.
 */
bool IsValidSignatureEncoding(const std::vector<unsigned char>& sig) {
    // Minimum and maximum size constraints.
    if (sig.size() < 9) return false;
    if (sig.size() > 73) return false;

    // A signature is of type 0x30 (compound).
    if (sig[0] != 0x30) return false;

    // Make sure the length covers the entire signature.
    if (sig[1] != sig.size() - 3) return false;

    // Extract the length of the R element.
    unsigned int lenR = sig[3];

    // Make sure the length of the S element is still inside the signature.
    if (5 + lenR >= sig.size()) return false;

    // Extract the length of the S element.
    unsigned int lenS = sig[5 + lenR];

    // Verify that the length of the signature matches the sum of the length
    // of the elements.
    if ((size_t)(lenR + lenS + 7) != sig.size()) return false;

    // Check whether the R element is an integer.
    if (sig[2] != 0x02) return false;

    // Zero-length integers are not allowed for R.
    if (lenR == 0) return false;

    // Negative numbers are not allowed for R.
    if (sig[4] & 0x80) return false;

    // Null bytes at the start of R are not allowed, unless R would
    // otherwise be interpreted as a negative number.
    if (lenR > 1 && (sig[4] == 0x00) && !(sig[5] & 0x80)) return false;

    // Check whether the S element is an integer.
    if (sig[lenR + 4] != 0x02) return false;

    // Zero-length integers are not allowed for S.
    if (lenS == 0) return false;

    // Negative numbers are not allowed for S.
    if (sig[lenR + 6] & 0x80) return false;

    // Null bytes at the start of S are not allowed, unless S would otherwise be
    // interpreted as a negative number.
    if (lenS > 1 && (sig[lenR + 6] == 0x00) && !(sig[lenR + 7] & 0x80)) return false;

    return true;
}

// The solution follows the exact validation logic from the `IsValidSignatureEncoding` function in the provided code snippet. The algorithm proceeds by first checking the overall size bounds (minimum 9 bytes, maximum 73 bytes). Then it verifies the signature starts with the compound tag 0x30 and that the length field at position 1 correctly equals `sig.size() - 3` (covering the R, S elements and sighash byte, excluding the tag and length byte itself). Next, it extracts the R length from position 3, checks that the S element is within bounds, extracts the S length, and confirms the total length matches `lenR + lenS + 7`. For each element R and S, it validates: the integer tag is 0x02, the length is non-zero, the first byte does not have its high bit set (no negative numbers), and if the length is greater than 1, a leading zero byte is only allowed when the next byte has its high bit set (necessary to avoid negative interpretation). Key edge cases include: signatures that are too short (less than 9 bytes) or too long (greater than 73), malformed length fields that don't match actual size, zero-length R or S, negative R or S (high bit set in first byte), and unnecessary null padding that would indicate non-minimal encoding. The time complexity is O(1) because the signature size is bounded, and space complexity is O(1).
