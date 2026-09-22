/*
Write a C++ function that implements a Hamming(7,4) error correction encoder/decoder for a single 4-bit data block. The function should take a 4-element integer array representing the original data bits (each 0 or 1), compute the three parity bits using even parity as described in the snippet, and return the complete 7-bit encoded codeword as a `std::array<int,7>`. Then, given a received 7-bit array (which may contain at most one flipped bit), the function should detect and correct any single-bit error, returning the corrected 7-bit codeword. The function must validate that all input bits are 0 or 1, and if not, throw an `std::invalid_argument` exception. The function should be named `hamming_encode_and_correct` and follow the exact parity bit positions and calculation formulas from the code snippet (parity bits at indices 6, 5, 3 as computed using XOR of specific data bits, and syndrome calculation `c = c3*4 + c2*2 + c1` to find the error position, with the correction at index `7-c`).
*/

#include <array>
#include <stdexcept>

// Encodes and corrects a 4-bit data block using Hamming(7,4) even parity.
// Throws std::invalid_argument if any bit is not 0 or 1.
std::array<int, 7> hamming_encode_and_correct(
    const std::array<int, 4>& data,
    const std::array<int, 7>& received) {

    // Validate all bits in both arrays
    for (int bit : data) {
        if (bit != 0 && bit != 1)
            throw std::invalid_argument("Data bits must be 0 or 1");
    }
    for (int bit : received) {
        if (bit != 0 && bit != 1)
            throw std::invalid_argument("Received bits must be 0 or 1");
    }

    // Encode the original data into a 7-bit codeword
    std::array<int, 7> encoded = {0, 0, 0, 0, 0, 0, 0};
    encoded[0] = data[0];
    encoded[1] = data[1];
    encoded[2] = data[2];
    encoded[4] = data[3];
    encoded[6] = encoded[0] ^ encoded[2] ^ encoded[4];
    encoded[5] = encoded[0] ^ encoded[1] ^ encoded[4];
    encoded[3] = encoded[0] ^ encoded[1] ^ encoded[2];

    // Compute syndrome from received word
    int c1 = received[6] ^ received[4] ^ received[2] ^ received[0];
    int c2 = received[5] ^ received[4] ^ received[1] ^ received[0];
    int c3 = received[3] ^ received[2] ^ received[1] ^ received[0];
    int c = c3 * 4 + c2 * 2 + c1;

    // Correct the received word if a single error is detected
    std::array<int, 7> corrected = received;
    if (c != 0) {
        int position = 7 - c; // 0-based index
        corrected[position] = corrected[position] ^ 1;
    }

    return corrected;
}

#include <cassert>
#include <array>

int main() {
    // Test 1: No error case
    std::array<int,4> data1 = {1,0,1,1};
    std::array<int,7> recv1 = {1,0,1,0,1,1,1}; // correct encoding
    std::array<int,7> result1 = hamming_encode_and_correct(data1, recv1);
    assert(result1 == recv1);

    // Test 2: Single error at position 0 (flip first bit)
    std::array<int,4> data2 = {1,0,1,0};
    std::array<int,7> recv2 = {0,0,1,1,0,1,1}; // flipped bit 0
    std::array<int,7> expected2 = {1,0,1,1,0,1,1};
    assert(hamming_encode_and_correct(data2, recv2) == expected2);

    // Test 3: Single error at position 6 (one parity bit)
    std::array<int,4> data3 = {0,1,1,1};
    std::array<int,7> recv3 = {0,1,1,1,1,0,0}; // flipped last parity bit
    std::array<int,7> expected3 = {0,1,1,1,1,0,1};
    assert(hamming_encode_and_correct(data3, recv3) == expected3);

    // Test 4: All zeros
    std::array<int,4> data4 = {0,0,0,0};
    std::array<int,7> recv4 = {0,0,0,0,0,0,0};
    assert(hamming_encode_and_correct(data4, recv4) == recv4);

    // Test 5: All ones (even parity gives all parity bits 1 as well)
    std::array<int,4> data5 = {1,1,1,1};
    std::array<int,7> recv5 = {1,1,1,1,1,1,1};
    assert(hamming_encode_and_correct(data5, recv5) == recv5);

    // Test 6: Error in a data bit (position 3)
    std::array<int,4> data6 = {1,1,0,0};
    std::array<int,7> recv6 = {1,1,0,0,0,0,0}; // correct is 1,1,0,1,0,0,0? Actually compute: data[0]=1,data[1]=1,data[2]=0,data[4]=0 => parity6=1^0^0=1, parity5=1^1^0=0, parity3=1^1^0=0 => codeword 1,1,0,0,0,0,1? Let's recompute: index 6=1, index5=0, index3=0, so codeword = [1,1,0,0,0,0,1]. Received has bit at index3 flipped: [1,1,0,1,0,0,1]? Actually recv6 given is [1,1,0,0,0,0,0] which has two errors (index5 should be 0, index6 should be 1). This may not be single error. Better use correct codeword and flip one bit.
    // Let's compute properly:
    std::array<int,4> data6b = {1,1,0,0};
    std::array<int,7> correct6 = {1,1,0,0,0,0,1};
    std::array<int,7> recv6b = correct6;
    recv6b[3] = 1; // flip bit at index 3
    std::array<int,7> expected6 = correct6;
    assert(hamming_encode_and_correct(data6b, recv6b) == expected6);

    // Test 7: Invalid input throws
    std::array<int,4> bad_data = {1,2,0,1};
    std::array<int,7> good_recv = {1,0,1,1,0,1,1};
    bool threw = false;
    try {
        hamming_encode_and_correct(bad_data, good_recv);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

// The solution must first verify that every element of the input arrays is either 0 or 1; if any is out of range, throw `std::invalid_argument`. For encoding, place the four data bits into positions 0, 1, 2, and 4 of a 7-element array (indices 3, 5, 6 are reserved for parity). Compute the parity bits using XOR (even parity) exactly as in the snippet: `data[6] = data[0]^data[2]^data[4]`, `data[5] = data[0]^data[1]^data[4]`, `data[3] = data[0]^data[1]^data[2]`. For the received array, compute the three syndrome bits `c1`, `c2`, `c3` using XOR combinations as given: `c1 = r[6]^r[4]^r[2]^r[0]`, `c2 = r[5]^r[4]^r[1]^r[0]`, `c3 = r[3]^r[2]^r[1]^r[0]`, then combine into `c = c3*4 + c2*2 + c1`. If `c == 0`, no error. Otherwise, flip the bit at position `7-c` (since the parity calculation makes the syndrome equal to the 1-based position of the error). The function must return the corrected codeword (or the original received word if no error). Time complexity is O(1) (constant number of operations) and space complexity is O(1) (only fixed-size arrays are used). Edge cases: input bits that are not 0/1 (throws), all zeros, all ones, multiple bit errors (not handled by this code—only single-bit correction is attempted, but the function will still flip one bit based on the syndrome, which is acceptable for the task specification).
