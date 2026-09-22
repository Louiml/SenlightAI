Implement a C++ function that simulates pitch lag decoding using the same 9-bit and 6-bit index scheme as shown in the provided ACELP decoder code. The function should take a bit index value, the number of bits used (6 or 9), the current minimum pitch lag reference, and pitch range parameters (PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX), and return the decoded fractional pitch lag (T0) and its fractional part (T0_frac), along with optionally updated T0_min and T0_max values for the 9-bit case. For the 6-bit case, the pitch is decoded relative to T0_min with quarter-sample resolution in the range [T0_min, T0_min+15.75]. For the 9-bit case, three resolution intervals apply: quarter-sample resolution in [PIT_MIN, PIT_FR2-0.25], half-sample resolution in [PIT_FR2, PIT_FR1-0.5], and integer resolution in [PIT_FR1, PIT_MAX]. The function must also update T0_min and T0_max for the 9-bit case as described in DecodePitchLag, clamping them to the [PIT_MIN, PIT_MAX] range and maintaining a width of 15. The function should return a bool indicating success (false for invalid bit counts or out-of-range values) and produce deterministic results for all valid inputs.
// The solution approach mirrors the exact algorithm from DecodePitchLag. First, validate that num_acb_idx_bits is either 6 or 9 (otherwise return false). For the 6-bit case, compute T0 = *pT0_min + acb_idx / 4 and T0_frac = acb_idx & 0x3, with no range update needed. For the 9-bit case, compute the cumulative index boundaries: the first interval covers indices 0 to (PIT_FR2-PIT_MIN)*4-1 (quarter resolution), the second interval covers the next (PIT_FR1-PIT_FR2)*2 indices (half resolution), and the third interval covers the remaining indices (integer resolution). Assign T0 and T0_frac according to which interval the index falls into, then compute T0_min = T0-8 clamped to [PIT_MIN, PIT_MAX] and T0_max = T0_min+15 clamped to [PIT_MAX, PIT_MIN+15]^. Since the integer range computation uses T0_min = max(PIT_MIN, T0-8) and T0_max = min(PIT_MAX, T0_min+15), with the constraint that T0_max cannot exceed PIT_MAX (if the initial T0_min+15 exceeds PIT_MAX, both are adjusted so T0_min = PIT_MAX-15). Time complexity is O(1) since only arithmetic and comparisons are performed; space complexity is O(1) as only a few scalar variables are needed. Edge cases include invalid bit counts, indices that fall near the interval boundaries, and PIT_MAX being smaller than PIT_FR1 (in which case the integer-only interval may be empty, but the algorithm still works because the index range naturally covers only the valid intervals).
#include <algorithm>
#include <cstdint>
#include <tuple>

// Decode pitch lag from a compressed index using the ACELP fractional pitch scheme.
// Returns {success, T0, T0_frac, T0_min, T0_max}.
// For num_acb_idx_bits==6, T0_min is the base for the quarter-resolution range.
// For num_acb_idx_bits==9, T0_min and T0_max are computed and returned.
std::tuple<bool, int, int, int, int> DecodePitchLagFromIndex(
    int acb_idx,
    int num_acb_idx_bits,
    int PIT_MIN,
    int PIT_FR2,
    int PIT_FR1,
    int PIT_MAX,
    int T0_min_in)
{
    if (num_acb_idx_bits != 6 && num_acb_idx_bits != 9) {
        return {false, 0, 0, T0_min_in, 0};
    }

    int T0 = 0;
    int T0_frac = 0;
    int T0_min = T0_min_in;
    int T0_max = 0;

    if (num_acb_idx_bits == 6) {
        // Quarter-sample resolution in range [T0_min, T0_min+15.75]
        T0 = T0_min + acb_idx / 4;
        T0_frac = acb_idx & 0x3;
    } else { // num_acb_idx_bits == 9
        // Interval 1: quarter resolution, range [PIT_MIN, PIT_FR2-0.25]
        int quarter_count = (PIT_FR2 - PIT_MIN) * 4;
        // Interval 2: half resolution, range [PIT_FR2, PIT_FR1-0.5]
        int half_count = (PIT_FR1 - PIT_FR2) * 2;
        // Interval 3: integer resolution, range [PIT_FR1, PIT_MAX]

        if (acb_idx < quarter_count) {
            T0 = PIT_MIN + acb_idx / 4;
            T0_frac = acb_idx & 0x3;
        } else if (acb_idx < quarter_count + half_count) {
            int idx = acb_idx - quarter_count;
            T0 = PIT_FR2 + idx / 2;
            T0_frac = (idx & 0x1) * 2;
        } else {
            T0 = acb_idx + PIT_FR1 - quarter_count - half_count;
            T0_frac = 0;
        }

        // Compute T0_min and T0_max for subframe 1 or 3
        T0_min = std::max(PIT_MIN, T0 - 8);
        T0_max = T0_min + 15;
        if (T0_max > PIT_MAX) {
            T0_max = PIT_MAX;
            T0_min = std::max(PIT_MIN, T0_max - 15);
        }
    }

    return {true, T0, T0_frac, T0_min, T0_max};
}
#include <cassert>
#include <tuple>

// The solution function is assumed to be included above
int main() {
    // 6-bit case: base T0_min = 20 (arbitrary)
    int T0_min_6 = 20;
    // Index 0 -> T0=20, frac=0
    auto [ok1, t1, f1, mn1, mx1] = DecodePitchLagFromIndex(0, 6, 0, 0, 0, 0, T0_min_6);
    assert(ok1 && t1 == 20 && f1 == 0 && mn1 == 20 && mx1 == 0);
    // Index 1 -> T0=20, frac=1
    auto [ok2, t2, f2, mn2, mx2] = DecodePitchLagFromIndex(1, 6, 0, 0, 0, 0, T0_min_6);
    assert(ok2 && t2 == 20 && f2 == 1);
    // Index 4 -> T0=21, frac=0
    auto [ok3, t3, f3, mn3, mx3] = DecodePitchLagFromIndex(4, 6, 0, 0, 0, 0, T0_min_6);
    assert(ok3 && t3 == 21 && f3 == 0);
    // Index 7 -> T0=21, frac=3
    auto [ok4, t4, f4, mn4, mx4] = DecodePitchLagFromIndex(7, 6, 0, 0, 0, 0, T0_min_6);
    assert(ok4 && t4 == 21 && f4 == 3);

    // 9-bit case with realistic parameters: PIT_MIN=34, PIT_FR2=128, PIT_FR1=160, PIT_MAX=231
    const int PIT_MIN = 34;
    const int PIT_FR2 = 128;
    const int PIT_FR1 = 160;
    const int PIT_MAX = 231;
    int dummy_min = 0;

    // Index 0: first quarter-resolution value -> T0=34, frac=0
    auto [ok5, t5, f5, mn5, mx5] = DecodePitchLagFromIndex(0, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok5 && t5 == 34 && f5 == 0);
    assert(mn5 == 34 && mx5 == 49); // T0_min = max(34,34-8)=34, T0_max=49

    // Index 3: still quarter-resolution -> T0=34, frac=3
    auto [ok6, t6, f6, mn6, mx6] = DecodePitchLagFromIndex(3, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok6 && t6 == 34 && f6 == 3);

    // Index 4: -> T0=35, frac=0
    auto [ok7, t7, f7, mn7, mx7] = DecodePitchLagFromIndex(4, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok7 && t7 == 35 && f7 == 0);

    // Boundary: last quarter-resolution index = (128-34)*4-1 = 375 -> T0=127, frac=3
    int last_quarter_idx = (PIT_FR2 - PIT_MIN) * 4 - 1;
    auto [ok8, t8, f8, mn8, mx8] = DecodePitchLagFromIndex(last_quarter_idx, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok8 && t8 == 127 && f8 == 3);

    // First half-resolution index: 376 -> T0=128, frac=0
    int first_half_idx = (PIT_FR2 - PIT_MIN) * 4;
    auto [ok9, t9, f9, mn9, mx9] = DecodePitchLagFromIndex(first_half_idx, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok9 && t9 == 128 && f9 == 0);

    // Last half-resolution index: 376 + (160-128)*2-1 = 375+63 = 439 -> T0=159, frac=0
    int last_half_idx = (PIT_FR2 - PIT_MIN) * 4 + (PIT_FR1 - PIT_FR2) * 2 - 1;
    auto [ok10, t10, f10, mn10, mx10] = DecodePitchLagFromIndex(last_half_idx, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok10 && t10 == 159 && f10 == 0);

    // First integer index: 440 -> T0=160, frac=0, T0_min=152, T0_max=167
    int first_int_idx = (PIT_FR2 - PIT_MIN) * 4 + (PIT_FR1 - PIT_FR2) * 2;
    auto [ok11, t11, f11, mn11, mx11] = DecodePitchLagFromIndex(first_int_idx, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok11 && t11 == 160 && f11 == 0);
    assert(mn11 == 152 && mx11 == 167);

    // Max index: (160-34)*4 + (160-128)*2 + (231-160) = 504 + 64 + 71 = 639 -> T0=231, frac=0
    int max_idx = (PIT_FR1 - PIT_MIN) * 4 + (PIT_FR1 - PIT_FR2) * 2 + (PIT_MAX - PIT_FR1);
    auto [ok12, t12, f12, mn12, mx12] = DecodePitchLagFromIndex(max_idx, 9, PIT_MIN, PIT_FR2, PIT_FR1, PIT_MAX, dummy_min);
    assert(ok12 && t12 == 231 && f12 == 0);
    // T0_min = max(34, 231-8)=223, T0_max=238, but clamped to PIT_MAX=231 -> T0_max=231, T0_min=216
    assert(mn12 == 216 && mx12 == 231);

    // Invalid bit count
    auto [ok13, t13, f13, mn13, mx13] = DecodePitchLagFromIndex(0, 5, 0, 0, 0, 0, 0);
    assert(!ok13);

    return 0;
}
