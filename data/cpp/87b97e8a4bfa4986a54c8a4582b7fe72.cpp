Given a digital signature represented as a vector of bytes in DER format (as used in Bitcoin scripts), write a C++ function that determines whether the signature encoding is valid according to the consensus-critical BIP66 rules. The function should validate the overall structure: the signature must start with `0x30`, have a total-length byte that matches the remaining bytes, contain exactly two integer elements (`0x02` type tags) for R and S, where each element has a valid length, is non-negative, and uses minimal positive big-endian encoding (no leading zero bytes unless required to avoid a negative number). The signature must also include a final sighash byte, and its total size must be between 9 and 73 bytes inclusive. The function should return `true` for valid encodings and `false` otherwise, without throwing exceptions or modifying the input.

// The solution follows the exact validation steps from the Bitcoin Core implementation. First, check the size constraints: a valid DER signature with a sighash byte must be at least 9 bytes (minimum structure: header, length, R-tag, R-len, 1-byte R, S-tag, S-len, 1-byte S, sighash) and at most 73 bytes (maximum typical ECDSA signature size). Then verify the first byte is `0x30`. The second byte must equal `sig.size() - 3` because the total-length field counts everything after itself except the final sighash byte. Extract the R length from byte index 3, and ensure `5 + lenR < sig.size()` so that there is space for the S tag, S length, S data, and sighash. Extract S length from `5 + lenR`. Verify that `lenR + lenS + 7 == sig.size()` (accounting for bytes 0,1,2,3, the R data, bytes at positions `lenR+4` and `lenR+5`, the S data, and the sighash). Check that the R element tag at index 2 is `0x02`, that `lenR > 0`, that the first R byte (index 4) does not have its high bit set (no negative), and that if `lenR > 1`, the first R byte is not `0x00` unless the second byte has its high bit set (to allow a necessary zero pad for otherwise-negative numbers). Apply symmetric checks for S: its tag must be at index `lenR + 4`, `lenS > 0`, the first S byte at index `lenR + 6` must not be negative, and if `lenS > 1`, a leading zero byte is only allowed if the next byte has its high bit set. All comparisons must be done using `unsigned int` for lengths and careful index arithmetic to avoid underflow. The algorithm simply reads through the vector with constant-time checks, so it runs in `O(1)` time relative to signature size (which is bounded by 73) and uses `O(1)` auxiliary space.

#include <vector>
#include <cstddef>

// Determine whether a signature's DER encoding is valid according to BIP66 rules.
// The input signature includes a trailing sighash byte and must be between 9 and 73 bytes.
bool IsValidSignatureEncoding(const std::vector<unsigned char>& sig) {
    // Minimum and maximum size constraints.
    if (sig.size() < 9) return false;
    if (sig.size() > 73) return false;

    // A signature is of type 0x30 (compound).
    if (sig[0] != 0x30) return false;

    // The length field covers everything after it, excluding the sighash byte.
    if (sig[1] != sig.size() - 3) return false;

    // Extract length of the R element.
    unsigned int lenR = sig[3];

    // Ensure space for S-length byte (at index 5+lenR) and at least one S byte plus sighash.
    if (5 + lenR >= sig.size()) return false;

    // Extract length of the S element.
    unsigned int lenS = sig[5 + lenR];

    // Verify that total length = lenR + lenS + 7 (header 4 bytes + S tag/len 2 bytes + sighash).
    if (static_cast<size_t>(lenR + lenS + 7) != sig.size()) return false;

    // Check R element is an integer type.
    if (sig[2] != 0x02) return false;

    // R must be non-empty.
    if (lenR == 0) return false;

    // R must be non-negative (highest bit of first byte must be clear).
    if (sig[4] & 0x80) return false;

    // R must use minimal encoding: a leading zero is allowed only if the next byte is negative.
    if (lenR > 1 && sig[4] == 0x00 && !(sig[5] & 0x80)) return false;

    // Check S element is an integer type.
    if (sig[lenR + 4] != 0x02) return false;

    // S must be non-empty.
    if (lenS == 0) return false;

    // S must be non-negative.
    if (sig[lenR + 6] & 0x80) return false;

    // S must use minimal encoding: a leading zero is allowed only if the next byte is negative.
    if (lenS > 1 && sig[lenR + 6] == 0x00 && !(sig[lenR + 7] & 0x80)) return false;

    return true;
}

#include <cassert>
#include <vector>

// Declare the function under test (in a real test, this comes from the header).
bool IsValidSignatureEncoding(const std::vector<unsigned char>& sig);

int main() {
    // Valid minimal signature: 0x30 0x06 0x02 0x01 0x01 0x02 0x01 0x01 0x01
    std::vector<unsigned char> valid = {0x30, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(valid) == true);

    // Too short (8 bytes)
    std::vector<unsigned char> tooShort = {0x30, 0x05, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01};
    assert(IsValidSignatureEncoding(tooShort) == false);

    // Too long (74 bytes)
    std::vector<unsigned char> tooLong(74, 0x00);
    tooLong[0] = 0x30;
    assert(IsValidSignatureEncoding(tooLong) == false);

    // Wrong first byte
    std::vector<unsigned char> wrongType = {0x31, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(wrongType) == false);

    // Length field mismatch
    std::vector<unsigned char> badLength = {0x30, 0x07, 0x02, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(badLength) == false);

    // R not an integer (tag 0x03)
    std::vector<unsigned char> badRTag = {0x30, 0x06, 0x03, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(badRTag) == false);

    // Negative R (first byte high bit set)
    std::vector<unsigned char> negativeR = {0x30, 0x06, 0x02, 0x01, 0x80, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(negativeR) == false);

    // Leading zero in R without necessary negative next byte
    std::vector<unsigned char> paddedR = {0x30, 0x07, 0x02, 0x02, 0x00, 0x01, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(paddedR) == false);

    // Valid with necessary leading zero in R (next byte negative)
    std::vector<unsigned char> validPaddedR = {0x30, 0x07, 0x02, 0x02, 0x00, 0x80, 0x02, 0x01, 0x01, 0x01};
    assert(IsValidSignatureEncoding(validPaddedR) == true);

    // S empty (zero length)
    std::vector<unsigned char> emptyS = {0x30, 0x05, 0x02, 0x01, 0x01, 0x02, 0x00, 0x01};
    assert(IsValidSignatureEncoding(emptyS) == false);

    return 0;
}
