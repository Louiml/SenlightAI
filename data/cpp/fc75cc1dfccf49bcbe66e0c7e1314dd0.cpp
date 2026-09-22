/*
Write a C++ function named `lzoLikePack` that takes a non-empty `std::vector<int>` of 14-bit unsigned values (i.e., each element satisfies `0 <= value < 16384`) and returns a `std::vector<uint8_t>` containing a simple byte-oriented compressed representation. The compression must use the following dictionary-based scheme: maintain a hash table of the most recent 16384 positions (indexed by a 14-bit hash of the last three bytes seen). For each position in the input, compute a hash from the next three values (if available) using the formula `hash = ((0x21 * ((v0 & 0x1f) | ((v0 >> 5) & 0x1f) << 5 | ((v1 & 0x3f))) ) >> 5) & 0x3fff`. If the hash points to a previous position whose three following values exactly match the current three values, output a two‑byte token: first byte `0x80 | match_length_minus_3` (where `match_length_minus_3` is the number of consecutive matches from 3 up to 18, capped), second byte the low 8 bits of the distance to the previous occurrence (distance is `currentPos - prevPos`, must be ≤ 255). Advance the output position by the matched length (starting from 3 up to the cap). Otherwise, output a literal byte: first byte the value (which is always ≤ 0x3f), then advance by one. After each processed literal or match, update the hash table entry at the current position’s hash (using the three values starting at that position) to the current position index. The output vector must be exactly the concatenated tokens; no extra headers. If a match is found but the distance exceeds 255, treat it as a literal. If fewer than three values remain, output them as literals. The function must be `const`‑correct and not modify the input.
*/
#include <vector>
#include <cstdint>
#include <cstddef>

// Compress 14-bit values using a simple dictionary with 14-bit hash and fixed-size matches.
std::vector<std::uint8_t> lzoLikePack(const std::vector<int>& values) {
    const std::size_t n = values.size();
    std::vector<std::uint8_t> output;
    if (n == 0) return output;

    // Hash table: maps 14-bit hash to last position, or -1 if unused.
    std::vector<int> prevPos(1 << 14, -1);
    const int MASK = (1 << 14) - 1;

    auto computeHash = [&](std::size_t pos) -> int {
        int v0 = values[pos];
        int v1 = values[pos + 1];
        int v2 = values[pos + 2];
        int low0 = v0 & 0x1f;
        int high0 = (v0 >> 5) & 0x1f;
        int low1 = v1 & 0x3f;
        int combined = low0 | (high0 << 5) | (low1 << 10);
        int hash = (0x21 * combined) >> 5;
        return hash & MASK;
    };

    std::size_t i = 0;
    while (i < n) {
        bool emittedMatch = false;

        if (i + 2 < n) {
            int hash = computeHash(i);
            int prev = prevPos[hash];
            if (prev != -1) {
                int dist = static_cast<int>(i) - prev;
                if (dist >= 1 && dist <= 255) {
                    // Try to match at least 3 values, up to 18.
                    int len = 0;
                    while (len < 18 && i + len < n && values[i + len] == values[prev + len]) {
                        ++len;
                    }
                    if (len >= 3) {
                        std::uint8_t token = static_cast<std::uint8_t>(0x80 | (len - 3));
                        output.push_back(token);
                        output.push_back(static_cast<std::uint8_t>(dist));
                        // After match, update hash for the starting position of the match.
                        // (The match itself is already covered; we do not need to update for intermediate positions.)
                        for (std::size_t k = i; k < i + len; ++k) {
                            if (k + 2 < n) {
                                prevPos[computeHash(k)] = static_cast<int>(k);
                            }
                        }
                        i += static_cast<std::size_t>(len);
                        emittedMatch = true;
                    }
                }
            }
        }

        if (!emittedMatch) {
            // Literal: value fits in 7 bits (max 0x3f).
            output.push_back(static_cast<std::uint8_t>(values[i]));
            if (i + 2 < n) {
                prevPos[computeHash(i)] = static_cast<int>(i);
            }
            ++i;
        }
    }

    return output;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Declaration of the function under test (included from the solution).
// (Assume the solution code is placed above this main.)

int main() {
    // Single literal
    {
        std::vector<int> input = {5};
        auto out = lzoLikePack(input);
        assert(out.size() == 1);
        assert(out[0] == 5);
    }
    // All literals (no repeats)
    {
        std::vector<int> input = {1, 2, 3, 4, 5};
        auto out = lzoLikePack(input);
        assert(out.size() == 5);
        for (int i = 0; i < 5; ++i) assert(out[i] == input[i]);
    }
    // Simple match: three equal values at start and repeat later
    {
        std::vector<int> input = {10, 11, 12, 10, 11, 12};
        auto out = lzoLikePack(input);
        // First three are literals: [10,11,12]
        // Then a match token: first byte 0x80 | (3-3)=0x80, second byte 3
        assert(out.size() == 5);
        assert(out[0] == 10);
        assert(out[1] == 11);
        assert(out[2] == 12);
        assert(out[3] == 0x80);
        assert(out[4] == 3);
    }
    // Match length 18 cap
    {
        std::vector<int> input;
        for (int i = 0; i < 20; ++i) input.push_back(7);
        auto out = lzoLikePack(input);
        // First three literals: [7,7,7], then a match of length 18? Actually match starts at pos 3? Let's compute:
        // pos0: hash, prev=-1 -> literal, push 7, update hash
        // pos1: hash, prev might be pos0? distance=1, match starting at pos1 with prev0: compare 3 values? pos1+2=3 <20, values at pos1..3 are 7,7,7; prev0..2 are 7,7,7 -> match length 18 (since cap). So token: 0x80 | 15 = 0x8F, distance=1. Then i advances by 18 to pos19, which is last single literal.
        assert(out.size() == 3 + 2 + 1);
        assert(out[0] == 7);
        assert(out[1] == 7);
        assert(out[2] == 7);
        assert(out[3] == 0x8F);
        assert(out[4] == 1);
        assert(out[5] == 7);
    }
    // Distance greater than 255 => no match
    {
        std::vector<int> input;
        for (int i = 0; i < 300; ++i) input.push_back(1);
        input.push_back(2); // different
        input.push_back(1); // back to 1
        input.push_back(1);
        input.push_back(1); // pattern at pos 300-303 is 1,1,1? Actually let's design: positions 0..299 are all 1, position 300 is 2, positions 301,302,303 are 1,1,1. The three at 301..303 match three at 0..2 but distance 301 >255, so literals.
        auto out = lzoLikePack(input);
        // All 304 inputs are literals because distance >255 and also there are many literals of '1' that don't match due to distance limit.
        assert(out.size() == input.size());
        for (size_t k = 0; k < input.size(); ++k) assert(out[k] == static_cast<uint8_t>(input[k]));
    }
    // Non-match despite hash collision: force same hash but different values
    {
        // We don't easily force collisions, but we test a sequence with similar first bytes
        std::vector<int> input = {1, 2, 3, 1, 4, 5};
        auto out = lzoLikePack(input);
        // All literals: no three-value match (pos0..2 vs pos3..5 have 1,4,5 vs 1,2,3)
        assert(out.size() == 6);
        for (int i = 0; i < 6; ++i) assert(out[i] == input[i]);
    }
    // Empty input (though spec says non-empty, function should return empty)
    {
        std::vector<int> input;
        auto out = lzoLikePack(input);
        assert(out.empty());
    }
    return 0;
}
// The algorithm processes the input sequentially. For each position `i`, if at least three values remain (`i+2 < n`), compute a 14‑bit hash from the three values at `i, i+1, i+2` using the given formula. Maintain a `std::vector<int> prevPos(16384, -1)` initialized to -1. Look up the hash; if `prevPos[hash]` is not -1 and the distance `i - prev` is between 1 and 255 inclusive, then attempt to match: extend the match from length 3 upward while `i+len` is within bounds, the next value matches the value at `prev+len`, and `len < 18` (since the token stores `len-3` in 4 bits, max len is 18? Actually the token’s first byte has `0x80 | (len-3)` where `len-3` must fit in 5 bits? The example says `match_length_minus_3` is from 3 up to 18, so `len-3` ranges 0..15, fits in 4 bits; we safely cap at 18). If the match length is at least 3, emit a two‑byte token: first byte `0x80 | (len-3)`, second byte the distance (as uint8_t). Then advance `i += len`. If no match (hash absent, distance too large, or match length <3), emit a literal byte: the value itself (which fits in one byte since ≤0x3f). Then advance `i += 1`. After each step (whether literal or match), update the hash table for the current position: if `i+2 < n`, compute the hash from the three values at `i, i+1, i+2` and set `prevPos[hash] = i`. Edge cases: last one or two values are always literals; hash collisions are possible but handled by direct comparison of three values; the distance is limited to 255; matches are capped at length 18; if a match is found but the following bytes don’t match exactly, fall back to literal. Time complexity: O(n) because each position is processed at most once (for literals) and matches advance by at least 3; hash table lookups are O(1). Space complexity: O(16384) for the hash table plus O(n) for the output vector.
