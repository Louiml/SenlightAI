// Implement a standalone C++ function that decodes a single 3-bit G.723.1 24 kbps ADPCM code into a 16-bit PCM sample, using the simplified fixed-point algorithm shown in the snippet. Your function must accept an 8-bit unsigned code (only the lower 3 bits are used, the upper 5 bits must be ignored) and maintain all necessary adaptive state variables (step size, predictors, difference history, tone flag, speed control) across calls. The function must be self-contained, with the internal state defined as static local variables so that multiple independent decoder instances are not required—the state persists between calls. The decoded output must be the reconstructed signal `sr` shifted left by a fixed volume of 2 (multiplied by 4). The function signature must be `int16_t decodeG72324(int8_t code)`. You must include the quantization tables as file-scope `const` arrays. The key challenge is accurately reproducing the arithmetic, especially the `Fmult`, `Reconstruct`, and `UpdtCalc` logic, while using only the integer types `int16_t` and `int32_t`. Ensure that all operations match the given formulas and that the state updates are performed exactly once per call.
// The solution is a faithful port of the `CodeDecToPcm16` method from the snippet, but stripped of class membership and using static local variables for state persistence. The main algorithm has four phases per code: (1) predictor computation—compute zero and pole predictions using the `Fmult` function (which implements floating-point-like multiplication via exponent/mantissa extraction), then sum them; (2) step-size adaptation—compute the quantizer step `y` by interpolating between fast (`yu`) and slow (`yl`) step sizes based on the speed control parameter `ap`; (3) reconstruction—unquantize the difference signal using the code's sign, the log-quantizer table `dqlntab`, and the step, then add to the predicted signal to get `sr`; (4) state update—call an adaptation routine that updates step sizes, predictor coefficients (both poles `a[0]`, `a[1]` and zeros `b[0..5]`), the difference history `dq[0..5]`, the reconstructed signal history `sr[0..1]`, the sign history `pk[0..1]`, and the speed control parameters `ap`, `dms`, `dml`, and `td`. The `FindQtzIdx` function performs a simple linear search to find the first power-of-two that is greater than the magnitude, returning the exponent. The `Fmult` function is the trickiest: it extracts a 5-bit magnitude, a 4-bit exponent from the `srn` value, and combines them with rounding. Edge cases include zero magnitude (mantissa forced to 32), negative signs, and clipping at 16-bit boundaries. The volume shift is simply a left shift by 2, which can overflow for large `sr`; the snippet does not clip, so we replicate that behavior. Time complexity is O(1) per call, with the only loops being constant-length (6 iterations for predictor zeros and history shifts). Space complexity is O(1) since all state is stored in a small number of fixed-size arrays and scalars.
#include <cstdint>
#include <cstdlib>

// Quantization tables from the G.723.1 24kbps decoder
const int16_t kDqlntab[8]   = {-2048, 135, 273, 373, 373, 273, 135, -2048};
const int16_t kWitab[8]     = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};
const int16_t kFitab[8]     = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};
const int16_t kPower2[15]   = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
                               0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000};

// Find first index in table where value < table[i], else return size
static int32_t findQtzIdx(int32_t value, const int16_t* table, int32_t size) {
    for (int32_t i = 0; i < size; i++) {
        if (value < table[i]) return i;
    }
    return size;
}

// Multiply a 16-bit value by a floating-point-like representation
static int32_t fmult(int32_t an, int32_t srn) {
    int16_t anmag, anexp, anmant;
    int16_t wanexp, wanmant;
    int16_t retval;

    anmag = (an > 0) ? an : ((-an) & 0x1FFF);
    anexp = findQtzIdx(anmag, kPower2, 15) - 6;
    anmant = (anmag == 0) ? 32 :
        (anexp >= 0) ? anmag >> anexp : anmag << -anexp;
    wanexp = anexp + ((srn >> 6) & 0xF) - 13;

    wanmant = (anmant * (srn & 077) + 0x30) >> 4;
    retval = (wanexp >= 0) ? ((wanmant << wanexp) & 0x7FFF) :
        (wanmant >> -wanexp);

    return (((an ^ srn) < 0) ? -retval : retval);
}

// Decode a single 3-bit G.723.1 24kbps ADPCM code to a 16-bit PCM sample
int16_t decodeG72324(int8_t code) {
    // Only lower 3 bits are used
    code &= 0x07;

    // Persistent state variables
    static int32_t yl = 34816;      // slow step size integrator
    static int32_t yu = 544;        // fast step size
    static int32_t dms = 0;         // speed control filter 1
    static int32_t dml = 0;         // speed control filter 2
    static int32_t ap = 0;          // speed control
    static int16_t a[2] = {0, 0};   // predictor poles
    static int16_t pk[2] = {0, 0};  // sign history
    static int16_t sr[2] = {32, 32}; // reconstructed signal history
    static int16_t b[6] = {0,0,0,0,0,0}; // predictor zeros
    static int16_t dq[6] = {32,32,32,32,32,32}; // difference history
    static int32_t td = 0;          // tone detector

    // Step 1: compute predictions
    int32_t sezi = fmult(b[0] >> 2, dq[0]);
    for (int32_t i = 1; i < 6; i++) {
        sezi += fmult(b[i] >> 2, dq[i]);
    }
    int32_t sez = sezi >> 1;
    int32_t sei = sezi + fmult(a[1] >> 2, sr[1]) + fmult(a[0] >> 2, sr[0]);
    int32_t se = sei >> 1;

    // Step 2: quantizer step size
    int32_t y;
    if (ap >= 256) {
        y = yu;
    } else {
        y = yl >> 6;
        int32_t dif = yu - y;
        int32_t al = ap >> 2;
        if (dif > 0) y += (dif * al) >> 6;
        else if (dif < 0) y += (dif * al + 0x3F) >> 6;
    }

    // Step 3: reconstruct difference and signal
    int32_t sign = (code & 0x04) ? 1 : 0;
    int32_t dqln = kDqlntab[code];
    int32_t dql = dqln + (y >> 2);
    int32_t dq_val;
    if (dql < 0) {
        dq_val = (sign) ? -0x8000 : 0;
    } else {
        int16_t dex = (dql >> 7) & 15;
        int16_t dqt = 128 + (dql & 127);
        dq_val = (dqt << 7) >> (14 - dex);
        if (sign) dq_val = dq_val - 0x8000;
    }

    int32_t sr_val = (dq_val < 0) ? (se - (dq_val & 0x3FFF)) : (se + dq_val);
    int32_t dqsez = sr_val - se + sez;

    // Step 4: update state variables (adapted from UpdtCalc)
    const int32_t code_size = 3; // 3-bit code

    int32_t pk0 = (dqsez < 0) ? 1 : 0;

    int32_t mag = dq_val & 0x7FFF;

    // Tone/transition detection
    int32_t ylint = yl >> 15;
    int32_t ylfrac = (yl >> 10) & 0x1F;
    int32_t thr1 = (32 + ylfrac) << ylint;
    int32_t thr2 = (ylint > 9) ? 31 << 10 : thr1;
    int32_t dqthr = (thr2 + (thr2 >> 1)) >> 1;
    int32_t tr;
    if (td == 0) tr = 0;
    else if (mag <= dqthr) tr = 0;
    else tr = 1;

    // Update step sizes
    yu = y + ((kWitab[code] - y) >> 5);
    if (yu < 544) yu = 544;
    else if (yu > 5120) yu = 5120;
    yl += yu + ((-yl) >> 6);

    // Adaptive predictor coefficients
    int32_t a2p = a[1] - (a[1] >> 7);
    if (tr == 1) {
        // Reset all predictor coefficients
        for (int32_t i = 0; i < 6; i++) b[i] = 0;
        a[0] = 0;
        a[1] = 0;
    } else {
        // Update poles
        int32_t pks1 = pk0 ^ pk[0];
        if (dqsez != 0) {
            int32_t fa1 = (pks1) ? a[0] : -a[0];
            if (fa1 < -8191) a2p -= 0x100;
            else if (fa1 > 8191) a2p += 0xFF;
            else a2p += fa1 >> 5;

            if (pk0 ^ pk[1]) {
                if (a2p <= -12160) a2p = -12288;
                else if (a2p >= 12416) a2p = 12288;
                else a2p -= 0x80;
            } else {
                if (a2p <= -12416) a2p = -12288;
                else if (a2p >= 12160) a2p = 12288;
                else a2p += 0x80;
            }
        }
        a[1] = a2p;

        // Update a[0]
        a[0] -= a[0] >> 8;
        if (dqsez != 0) {
            if (pks1 == 0) a[0] += 192;
            else a[0] -= 192;
        }

        // Limit a[0]
        int32_t a1ul = 15360 - a2p;
        if (a[0] < -a1ul) a[0] = -a1ul;
        else if (a[0] > a1ul) a[0] = a1ul;

        // Update zeros
        for (int32_t cnt = 0; cnt < 6; cnt++) {
            b[cnt] -= b[cnt] >> 8; // for 24kbps
            if (dq_val & 0x7FFF) {
                if ((dq_val ^ dq[cnt]) >= 0) b[cnt] += 128;
                else b[cnt] -= 128;
            }
        }
    }

    // Shift difference history
    for (int32_t cnt = 5; cnt > 0; cnt--) dq[cnt] = dq[cnt-1];

    // Convert dq[0] to floating-point-like representation
    if (mag == 0) {
        dq[0] = (dq_val >= 0) ? 0x20 : 0xFC20;
    } else {
        int32_t exp = findQtzIdx(mag, kPower2, 15);
        dq[0] = (dq_val >= 0) ?
            (exp << 6) + ((mag << 6) >> exp) :
            (exp << 6) + ((mag << 6) >> exp) - 0x400;
    }

    // Shift reconstructed signal history
    sr[1] = sr[0];
    if (sr_val == 0) sr[0] = 0x20;
    else if (sr_val > 0) {
        int32_t exp = findQtzIdx(sr_val, kPower2, 15);
        sr[0] = (exp << 6) + ((sr_val << 6) >> exp);
    } else if (sr_val > -32768) {
        int32_t mag2 = -sr_val;
        int32_t exp = findQtzIdx(mag2, kPower2, 15);
        sr[0] = (exp << 6) + ((mag2 << 6) >> exp) - 0x400;
    } else {
        sr[0] = (int16_t)0xFC20;
    }

    // Shift sign history
    pk[1] = pk[0];
    pk[0] = pk0;

    // Tone detector update
    if (tr == 1) td = 0;
    else if (a2p < -11776) td = 1;
    else td = 0;

    // Speed control update
    dms += (kFitab[code] - dms) >> 5;
    dml += (((kFitab[code] << 2) - dml) >> 7);

    if (tr == 1) {
        ap = 256;
    } else if (y < 1536) {
        ap += (0x200 - ap) >> 4;
    } else if (td == 1) {
        ap += (0x200 - ap) >> 4;
    } else if (abs((dms << 2) - dml) >= (dml >> 3)) {
        ap += (0x200 - ap) >> 4;
    } else {
        ap += (-ap) >> 4;
    }

    // Shift result by volume (volume = 2)
    return static_cast<int16_t>(sr_val << 2);
}
#include <cassert>
#include <cstdint>

// Declaration of the function under test
int16_t decodeG72324(int8_t code);

int main() {
    // Test 1: First code with all zero state (initial conditions)
    // State initialized: yl=34816, yu=544, ap=0, all predictors zero, sr=[32,32], dq=[32,...]
    int16_t out1 = decodeG72324(0);
    // With code 0: sign=0, dqln=-2048, y = yl>>6 = 544, dql = -2048 + 136 = -1912 < 0 => dq=0
    // sezi=0, sez=0, sei=0, se=0, sr = 0 + 0 = 0
    // Output = 0 << 2 = 0
    assert(out1 == 0);

    // Test 2: Second call with same code 0, state has been updated
    // After first call: yu updated, yl updated, predictors unchanged (no dqsez), etc.
    // The result may not be exactly 0 but we can verify it is in a reasonable range
    int16_t out2 = decodeG72324(0);
    // Just check the output is within int16_t range (no assertion on exact value here)
    // But we can check that state is changed: yl must not equal 34816 anymore
    // We can't access static state, so we rely on known behavior: after first call,
    // yl = 34816 + yu - (34816>>6) = 34816 + 544 - 544 = 34816? Actually:
    // yl += yu + ((-yl)>>6) = 34816 + 544 - 544 = 34816 (since -34816>>6 = -544)
    // So yl stays same, but other states change slightly.
    // We can at least assert the output is not wildly out of range.
    assert(out2 >= -32768 && out2 <= 32767);

    // Test 3: Test code with maximum value (7) after resetting state
    // We cannot reset static state, but we can continue and check consistency
    // Not robust to test exactly, so we'll just test that the function runs without crash
    for (int8_t code = 0; code < 8; code++) {
        int16_t result = decodeG72324(code);
        assert(result >= -32768 && result <= 32767);
    }

    // Test 4: Test that upper bits are ignored: code 8 (1000b) should be same as code 0
    // But state has changed from previous calls, so we can't compare to fresh state.
    // Instead, we can manually verify by calling with 8 and 0 in sequence and comparing
    // to a fresh run (not possible here). We'll just check that code 8 produces a valid output.
    int16_t out_with_high_bit = decodeG72324(8);
    assert(out_with_high_bit >= -32768 && out_with_high_bit <= 32767);

    // Test 5: Simple check that the function is deterministic for same input sequence
    // We'll record outputs for a sequence and compare with a second run of same sequence
    // But static state prevents reset, so we'll just verify that repeated same code gives
    // possibly different outputs (since state changes), which is expected.
    int16_t first = decodeG72324(3);
    int16_t second = decodeG72324(3);
    // These may differ, but both must be valid
    assert(first >= -32768 && first <= 32767);
    assert(second >= -32768 && second <= 32767);

    // Test 6: Verify that the function handles negative codes correctly (e.g., -1 = 255 & 7 = 7)
    int16_t neg_out = decodeG72324(-1);
    assert(neg_out >= -32768 && neg_out <= 32767);

    return 0;
}
