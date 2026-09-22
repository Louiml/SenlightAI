// Write a C++ function `std::vector<int> rleCompressToVector(const std::vector<int>& data)` (or better, operate on bytes: `std::vector<char> rleCompress(const std::vector<char>& data)`) that performs run-length encoding on an input array of signed bytes, following the exact encoding scheme in the provided snippet. The encoding rules are: a run of at least 3 identical consecutive bytes is encoded as `(length-1)` (stored as a signed char, so values 2..126) followed by the byte; any other sequence of up to 127 bytes is encoded as a negative count `-(length)` (stored as a signed char) followed by the literal bytes. The function must handle empty input (return empty vector). You must also implement a corresponding decoder `std::vector<char> rleUncompress(const std::vector<char>& compressed, int maxLength)` that reconstructs the original data, returning an empty vector if the decoded length would exceed `maxLength`. The encoding must be exactly compatible with the given snippet's `rleCompress` and `rleUncompress` functions, including the handling of runs at the end of the input. Do not implement the predictor or byte-reordering; only the pure RLE layer. Provide const-correct free functions with no `main`.

// The core algorithm processes the input byte array linearly. We scan from left to right, maintaining a pointer to the start of the current run and a pointer that extends the run. For each run start, we first check if the next several bytes are all identical; if the run length reaches at least 3, we emit a positive count `(len-1)` as a signed char, followed by the byte value. If the run length is less than 3, we instead treat it as an uncompressed literal sequence. For a literal sequence, we extend it while the next two bytes are not equal to each other (i.e., a potential run hasn't started) and the length is at most 127. We emit a negative count `-(len)` and then the literal bytes. After processing a run (compressed or literal), we move the start pointer to where the run ended and continue. Edge cases: a run exactly at the end must be handled correctly; when `runEnd` reaches `inEnd`, the inner loops must stop and the remaining data must be flushed. The decoder reads the count byte; if negative, it copies `-count` literal bytes; if non-negative, it copies `count+1` copies of the next byte. It must check `maxLength` to prevent overflow. Time complexity is O(n) for both encode and decode, and space is O(n) for the output. The hardest part is ensuring the encoder matches the snippet's exact behavior for runs of length 1 or 2 that are followed by a run of 3 or more; the snippet's logic handles this by treating short runs as literals and including the following bytes as needed. The decoder is straightforward.

#include <vector>
#include <cstddef>

// Encode a vector of bytes using run-length encoding as defined in the snippet.
// Returns a vector of signed char values (as char) representing the compressed data.
// Empty input yields empty output.
std::vector<char> rleCompress(const std::vector<char>& input) {
    std::vector<char> output;
    if (input.empty()) return output;

    size_t inLength = input.size();
    const char* in = input.data();
    const char* inEnd = in + inLength;
    const char* runStart = in;
    const char* runEnd = in + 1;

    while (runStart < inEnd) {
        // Check for a compressible run (at least 3 identical bytes).
        while (runEnd < inEnd &&
               *runStart == *runEnd &&
               static_cast<int>(runEnd - runStart - 1) < 127) {
            ++runEnd;
        }

        if (runEnd - runStart >= 3) {
            // Compressible run: emit (length-1) as signed char, then the byte.
            output.push_back(static_cast<char>(static_cast<signed char>(runEnd - runStart - 1)));
            output.push_back(*runStart);
            runStart = runEnd;
        } else {
            // Uncompressible run: gather up to 127 bytes that do not start a run.
            // A run starts only when three consecutive bytes are identical.
            while (runEnd < inEnd &&
                   ((runEnd + 1 >= inEnd || *runEnd != *(runEnd + 1)) ||
                    (runEnd + 2 >= inEnd || *(runEnd + 1) != *(runEnd + 2))) &&
                   static_cast<int>(runEnd - runStart) < 127) {
                ++runEnd;
            }

            // Emit negative count: -(length) as signed char.
            output.push_back(static_cast<char>(static_cast<signed char>(runStart - runEnd)));
            for (const char* p = runStart; p < runEnd; ++p) {
                output.push_back(*p);
            }
            runStart = runEnd;
        }
        ++runEnd;  // Move past the last byte of the processed run.
    }
    return output;
}

// Decode a compressed byte vector produced by rleCompress.
// Returns the original data, or an empty vector if the decoded length would exceed maxLength.
std::vector<char> rleUncompress(const std::vector<char>& compressed, int maxLength) {
    std::vector<char> output;
    if (compressed.empty()) return output;

    const signed char* in = reinterpret_cast<const signed char*>(compressed.data());
    size_t inLength = compressed.size();
    size_t inPos = 0;
    int outSize = 0;

    while (inPos < inLength) {
        signed char countByte = in[inPos++];
        if (countByte < 0) {
            int count = -static_cast<int>(countByte);
            if (inPos + count > inLength) return {};  // malformed input
            if (outSize + count > maxLength) return {};
            for (int i = 0; i < count; ++i) {
                output.push_back(in[inPos++]);
            }
            outSize += count;
        } else {
            int count = static_cast<int>(countByte) + 1;
            if (inPos >= inLength) return {};  // malformed: missing byte value
            if (outSize + count > maxLength) return {};
            char val = in[inPos++];
            for (int i = 0; i < count; ++i) {
                output.push_back(val);
            }
            outSize += count;
        }
    }
    return output;
}

#include <cassert>
#include <vector>

// Assume the solution functions are declared above.

int main() {
    // Empty input
    std::vector<char> empty;
    auto compEmpty = rleCompress(empty);
    assert(compEmpty.empty());
    auto decompEmpty = rleUncompress(compEmpty, 10);
    assert(decompEmpty.empty());

    // Single byte
    std::vector<char> single = {'A'};
    auto compSingle = rleCompress(single);
    assert(compSingle.size() == 2);
    auto decompSingle = rleUncompress(compSingle, 10);
    assert(decompSingle == single);

    // Run of 3 identical bytes
    std::vector<char> run3 = {'x', 'x', 'x'};
    auto compRun3 = rleCompress(run3);
    assert(compRun3.size() == 2);
    assert(compRun3[0] == static_cast<char>(2)); // length-1
    assert(compRun3[1] == 'x');
    auto decompRun3 = rleUncompress(compRun3, 10);
    assert(decompRun3 == run3);

    // Literal run of 2 bytes (no compression)
    std::vector<char> lit2 = {'a', 'b'};
    auto compLit2 = rleCompress(lit2);
    assert(compLit2.size() == 3);
    assert(static_cast<signed char>(compLit2[0]) == -2);
    assert(compLit2[1] == 'a' && compLit2[2] == 'b');
    auto decompLit2 = rleUncompress(compLit2, 10);
    assert(decompLit2 == lit2);

    // Mixed: literal then run then literal
    std::vector<char> mixed = {'a', 'b', 'c', 'c', 'c', 'd'};
    auto compMixed = rleCompress(mixed);
    auto decompMixed = rleUncompress(compMixed, 100);
    assert(decompMixed == mixed);

    // Run at the very end
    std::vector<char> endRun = {'p', 'q', 'q', 'q', 'q'};
    auto compEnd = rleCompress(endRun);
    auto decompEnd = rleUncompress(compEnd, 100);
    assert(decompEnd == endRun);

    // Long run > 127 bytes
    std::vector<char> longRun(200, 'z');
    auto compLong = rleCompress(longRun);
    // 200 = 127+73 -> first run len 127 (count 126), then 73 (count 72)
    auto decompLong = rleUncompress(compLong, 200);
    assert(decompLong == longRun);

    // MaxLength exceeded
    std::vector<char> exceedData = {'a', 'b', 'c'};
    auto compExceed = rleCompress(exceedData);
    auto decompExceed = rleUncompress(compExceed, 2);
    assert(decompExceed.empty());

    // Random pattern (deterministic)
    std::vector<char> pattern = {1, 1, 1, 2, 3, 3, 3, 3, 4};
    auto compPat = rleCompress(pattern);
    auto decompPat = rleUncompress(compPat, 100);
    assert(decompPat == pattern);

    return 0;
}
