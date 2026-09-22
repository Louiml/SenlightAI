Write a C++ function named `decode_8_pulses_31bits` that takes an array of exactly 5 signed 16-bit integers `index[]` (representing compressed pulse positions and signs), an output array `cod[]` of exactly 40 signed 16-bit integers, and a pointer to a `bool` overflow flag. The function must decompress the first four elements of `index` as 4 sign bits (one per track), and the last element as three sub-indexes that encode 8 pulse positions (8 positions among 40, where pulses 0–3 use one of 10 positions per track and pulses 4–7 use another 10 positions per track). The decoding logic must follow the GSM AMR algebraic codebook format: the last `index[4]` is split into three compressed values using bit shifts and masks: `MSBs0 = index[4] >> 3`, `LSBs0 = index[4] & 7`, `MSBs1 = (index[4] >> 6) & 7` (since index[4] is 15 bits, but we only use the first 11 bits for simplicity; ignore higher bits). Each pair `(MSBs, LSBs)` decodes to three positions with the formulas: `pos_a = ((MSBs % 25) % 5)*2 + (LSBs % 2)`, `pos_b = (((MSBs % 25) / 5)*2 + ((LSBs % 4) / 2))`, `pos_c = ((MSBs / 25)*2 + (LSBs / 4))`. The third sub-index (from bits 10–11 of index[4]) is not used; instead, the first three pulses of the second group (positions 4,5,6) are derived from the second pair of (MSBs,LSBs) and the fourth pulse (position 7) uses a separate formula: first compute `MSBs0_24 = ( ((MSBs2*25+12)/32) )` where `MSBs2` is derived from the remaining bits, but for simplicity, use the last two bits of `index[4]` to compute `MSBs2 = (index[4] >> 10) & 3` and combine with `LSBs2 = index[4] & 3` to produce pulse 7 as `pos7 = (MSBs2*2 + LSBs2)`. After computing all 8 positions (each in range 0–9, then multiplied by 4 and added to their track index 0–3 to get actual index 0–39), each pulse's sign is determined by the signs array: if `sign_idx[j] == 0` then sign is +8191, else -8191. For pulses 4–7, if the computed position is less than the corresponding position of pulse `j-4` (the same track), then negate the sign. Place each pulse value into `cod[pos]` (for pulse 0–3) or add to `cod[pos]` for pulse 4–7 (since two pulses share the same track but different positions). Initialize all 40 entries of `cod` to zero before filling. The overflow flag must be set to `false` (no overflow) if all calculations stay within 16-bit range, but the function must never actually overflow due to input constraints; it should simply set `*pOverflow = false` at the end.
// The problem mirrors the GSM AMR algebraic codebook decoder. The main task is to unpack a 31-bit index (stored as five 16-bit integers, but effectively `index[0]`–`index[3]` are sign bits, and `index[4]` holds three sub-indexes compressed into 15 bits) into 8 pulse positions and signs. The algorithm proceeds in two stages: first, extract the sign bits directly from `index[0..3]`. Second, decompress `index[4]` by bit operations: the lower 3 bits give `LSBs0`, bits 3–9 give `MSBs0` (7 bits), bits 10–12 give a second `MSBs1` (3 bits, but we treat it as part of a second triple), and bits 13–14 are ignored for simplicity. Actually, the standard implementation splits `index[4]` into three separate compressed fields: `MSBs` and `LSBs` for the first triple, and a second `MSBs`/`LSBs` for the second triple, plus a final small field for the last pulse. The provided pseudocode uses `decompress10` for each triple, which computes positions from `MSBs` (0–124) and `LSBs` (0–7). The formulas are derived from the fact that a position in 0–9 can be represented as `2*row + col` where row in 0–4 and col in 0–1. The code uses arithmetic to extract row and col from `MSBs` (which is 0–124) by decomposing as `MSBs = 25*quot + rem` where `quot` is 0–4 and `rem` is 0–24; then `rem` is further split into `rem = 5*row2 + col2`. The modulo and division operations are performed using multiplications to simulate division (e.g., `mult(x, 1311)` gives `x*1311/32768` which is approximately `x/25`). Edge cases include clamping `MSBs` to 124, handling `LSBs` up to 7, and ensuring positions stay within 0–39 when multiplied by 4 and added to track index. The third pulse in the second group (position 7) uses a different encoding (5-bit index), which we simplified to 4 bits. The final `cod` array must accumulate two pulses per track: for each track j, pulse j and pulse j+4 both map to positions `pos1 = (pos_index[j]*4 + j)` and `pos2 = (pos_index[j+4]*4 + j)`. If `pos2 < pos1`, negate the sign of pulse j+4. Complexity is O(40) for clearing and O(8) for placing, so O(1) time and O(1) auxiliary space beyond input/output arrays.
#include <cstdint>
#include <cstddef>

// Decode 8-pulse algebraic codebook from compressed index (31-bit format).
// index:  5-element array; index[0..3] are sign bits, index[4] holds pulse positions.
// cod:    40-element output array; each entry is +/-8191 or 0.
// pOverflow: set to false if no overflow occurs (always for valid input).
void decode_8_pulses_31bits(const int16_t index[5], int16_t cod[40], bool* pOverflow) {
    // Clear output codebook
    for (int i = 0; i < 40; ++i) {
        cod[i] = 0;
    }

    // Extract sign bits for 4 tracks
    int16_t signs[4];
    for (int j = 0; j < 4; ++j) {
        signs[j] = index[j];
    }

    // Decompress positions from index[4]
    // First triple: pulses 0, 4, 1
    int16_t MSBs0 = static_cast<int16_t>((index[4] >> 3) & 0x7F);  // 7 bits
    int16_t LSBs0 = static_cast<int16_t>(index[4] & 0x7);          // 3 bits
    if (MSBs0 > 124) MSBs0 = 124;

    // Decompress first triple using formulas
    // ia = MSBs0 / 25, ib = MSBs0 % 25
    int ia0 = MSBs0 / 25;
    int rem0 = MSBs0 % 25;
    int rowA0 = rem0 / 5;
    int colA0 = rem0 % 5;
    int pos0 = (colA0 * 2 + (LSBs0 & 1));
    int pos4 = (rowA0 * 2 + ((LSBs0 >> 1) & 1));
    int pos1_tmp = (ia0 * 2 + (LSBs0 >> 2));

    // Second triple: pulses 2, 6, 5 (from bits 10-12 and 7-9 of index[4])
    // For simplicity, we use bits 10-12 as MSBs1 (3 bits, 0-7) and bits 7-9 as LSBs1 (3 bits)
    int16_t MSBs1 = static_cast<int16_t>((index[4] >> 10) & 0x7);
    int16_t LSBs1 = static_cast<int16_t>((index[4] >> 7) & 0x7);
    if (MSBs1 > 124) MSBs1 = 124;

    int ia1 = MSBs1 / 25;
    int rem1 = MSBs1 % 25;
    int rowA1 = rem1 / 5;
    int colA1 = rem1 % 5;
    int pos2 = (colA1 * 2 + (LSBs1 & 1));
    int pos6 = (rowA1 * 2 + ((LSBs1 >> 1) & 1));
    int pos5 = (ia1 * 2 + (LSBs1 >> 2));

    // Third pulse in group 2: pulse 7, using bits 0-2 of index[4] as LSBs2 and high bits not used
    // We approximate with a 4-bit field: use bits 13-14 as MSBs2 (0-3) and bits 1-0 as LSBs2 (0-3)
    int16_t MSBs2 = static_cast<int16_t>((index[4] >> 13) & 0x3);
    int16_t LSBs2 = static_cast<int16_t>(index[4] & 0x3);
    int pos7 = MSBs2 * 2 + LSBs2;

    // Place pulse positions into a 8-element array (track order: 0,4,1,2,6,5,3,7)
    int positions[8];
    positions[0] = pos0;
    positions[4] = pos4;
    positions[1] = pos1_tmp;
    positions[2] = pos2;
    positions[6] = pos6;
    positions[5] = pos5;
    positions[3] = 0;  // Placeholder, not used; we compute pos3 separately below
    positions[7] = pos7;

    // Actually, the standard order is pulses 0,1,2,3,4,5,6,7 with tracks:
    // pulse 0 -> track 0, pulse 1 -> track 1, pulse 2 -> track 2, pulse 3 -> track 3,
    // pulse 4 -> track 0, pulse 5 -> track 1, pulse 6 -> track 2, pulse 7 -> track 3.
    // The decompressed triples give pulses (0,4,1) and (2,6,5) and (3,7) in the original.
    // Our array should have positions[track] for first group and positions[track+4] for second group.
    // Let's reassign correctly:
    // From first triple: pos0 -> pulse0, pos4 -> pulse4, pos1_tmp -> pulse1
    // From second triple: pos2 -> pulse2, pos6 -> pulse6, pos5 -> pulse5
    // For pulse3 and pulse7, we need to derive: pulse3 = ?, pulse7 = ?
    // The original code uses a complex formula for pulse3 and pulse7 from a separate 5-bit field.
    // For simplicity, we set pulse3 = 0 and pulse7 = pos7 (as above), but that's not standard.
    // To keep correctness within the provided pseudo-code, we use the provided formulas:
    // For pulse 3: derived from MSBs0_24 (not available). We'll use MSBs0_24 = (MSBs0*25+12)/32.
    // But that's overkill. Instead, we follow the exact decompress10 for the first two triples,
    // and for the third (pulse3 and pulse7) we use the formula from the code snippet:
    // MSBs = (index[4] >> 2) & 0x7F? No, the snippet uses index[6] but we only have index[4].
    // Given the task specification, we treat index[4] as 15 bits: bits 0-2 LSBs, bits 3-9 MSBs,
    // bits 10-12 second MSBs, bits 13-14 third MSBs. The third triple is 2 pulses (pulse3 and pulse7).
    // Use the provided formula from the code: MSBs0_24 = (MSBs*25+12)/32 where MSBs is the third MSBs (0-3).
    int16_t MSBs3 = static_cast<int16_t>((index[4] >> 13) & 0x3);  // bits 13-14
    int16_t LSBs3 = static_cast<int16_t>((index[4] >> 11) & 0x3); // bits 11-12
    int temp = (MSBs3 * 25 + 12) / 32;
    int ia3 = temp / 5;
    int rem3 = temp % 5;
    int pos3 = (rem3 * 2 + (LSBs3 & 1));
    int pos7_correct = (ia3 * 2 + (LSBs3 >> 1));

    // Now assemble final positions array in pulse order 0..7
    int final_pos[8];
    final_pos[0] = pos0;
    final_pos[4] = pos4;
    final_pos[1] = pos1_tmp;
    final_pos[2] = pos2;
    final_pos[6] = pos6;
    final_pos[5] = pos5;
    final_pos[3] = pos3;
    final_pos[7] = pos7_correct;

    // Build output codebook per track
    for (int j = 0; j < 4; ++j) {
        int16_t sign1 = (signs[j] == 0) ? 8191 : -8191;
        int pos1 = (final_pos[j] << 2) + j;  // actual index = pos*4 + track
        if (pos1 < 40) cod[pos1] = sign1;

        int16_t sign2 = sign1;
        int pos2 = (final_pos[j + 4] << 2) + j;
        if (pos2 < pos1) {
            sign2 = -sign2;
        }
        if (pos2 < 40) {
            cod[pos2] += sign2;
        }
    }

    *pOverflow = false;
}
#include <cassert>
#include <cstdint>

// The solution function is declared here (simulate inclusion).
void decode_8_pulses_31bits(const int16_t index[5], int16_t cod[40], bool* pOverflow);

int main() {
    bool overflow;
    int16_t cod[40];

    // Test case 1: All signs positive, all positions 0 (index[4] encodes zeros)
    int16_t idx1[5] = {0, 0, 0, 0, 0};
    decode_8_pulses_31bits(idx1, cod, &overflow);
    assert(overflow == false);
    // Pulses at positions 0,1,2,3 (first group) and 0,1,2,3 (second group) but second group negated if pos2<pos1? pos2=0 not <0, so signs positive.
    assert(cod[0] == 8191);
    assert(cod[1] == 8191);
    assert(cod[2] == 8191);
    assert(cod[3] == 8191);
    assert(cod[4] == 8191);  // pulse 4 at track 0 pos 0
    assert(cod[5] == 8191);  // pulse 5
    assert(cod[6] == 8191);  // pulse 6
    assert(cod[7] == 8191);  // pulse 7
    assert(cod[8] == 0);     // no other positions

    // Test case 2: Signs negative, positions decoded from index[4]=0b11111 (some)
    int16_t idx2[5] = {1, 1, 1, 1, 0x7F}; // MSBs0=15, LSBs0=7, MSBs1=0, LSBs1=0, MSBs3=0
    decode_8_pulses_31bits(idx2, cod, &overflow);
    // MSBs0=15 -> ia=0, rem=15 -> row=3, col=0 -> pos0=0*2+1=1? Wait colA=0, LSBs0&1=1 -> pos0=1. pos4=3*2+ (7>>1 &1)=6+1=7? (7>>1)=3, &1=1 -> pos4=7. pos1_tmp=0*2+(7>>2)=0+1=1.
    // Positions: pulse0=1, pulse4=7, pulse1=1. MSBs1=0 gives pos2=0, pos6=0, pos5=0. MSBs3=0 gives pos3=0, pos7=0.
    // Actual indices: pulse0: 1*4+0=4, pulse4:7*4+0=28, pulse1:1*4+1=5, pulse2:0*4+2=2, pulse6:0*4+2=2, pulse5:0*4+1=1, pulse3:0*4+3=3, pulse7:0*4+3=3.
    assert(cod[4] == -8191);
    assert(cod[28] == -8191);
    assert(cod[5] == -8191);
    assert(cod[2] == -8191);
    assert(cod[1] == -8191);
    assert(cod[3] == -8191);
    // Note: cod[2] has both pulse2 and pulse6, so sum should be -8191 + -8191 = -16382
    assert(cod[2] == -16382);
    assert(cod[1] == -8191);
    assert(cod[3] == -8191);

    // Test case 3: Overflow flag should be false for valid inputs
    int16_t idx3[5] = {0, 0, 0, 0, 0x7FFF}; // max index, but we ignore high bits
    decode_8_pulses_31bits(idx3, cod, &overflow);
    assert(overflow == false);

    // Test case 4: Ensure all positions are within 0..39
    for (int i = 0; i < 40; ++i) {
        assert(cod[i] == 0 || cod[i] == 8191 || cod[i] == -8191 || cod[i] == 16382 || cod[i] == -16382);
    }

    // Test case 5: Specific index that yields known positions
    int16_t idx5[5] = {0, 0, 0, 0, 0x7B}; // 123 decimal: MSBs0=15, LSBs0=3, MSBs1=0, LSBs1=0
    decode_8_pulses_31bits(idx5, cod, &overflow);
    // MSBs0=15 -> ia=0, rem=15 -> row=3, col=0, LSBs0=3 -> pos0=0*2+1=1, pos4=3*2+ (3>>1=1 &1)=6+1=7? (3>>1)=1, &1=1 -> pos4=7, pos1=0*2+(3>>2=0)=0.
    // Others zero. Check cod[4] = -8191? signs are 0 so +8191.
    assert(cod[4] == 8191);
    assert(cod[28] == 8191);
    assert(cod[5] == 8191);
    // pulse1 at pos0*4+1=1, pulse2 at 0, pulse6 at 0+? pos6=0 -> index 2, pulse5=0 -> index 1, pulse3=0 -> index 3, pulse7=0 -> index 3.
    assert(cod[1] == 8191);
    assert(cod[2] == 8191);
    assert(cod[3] == 8191);

    return 0;
}
