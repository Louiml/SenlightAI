Write a standalone C++ function `resample_mono_ratio` that implements a simplified single-channel sample-rate converter using the same windowed-sinc interpolation approach as the provided code. The function takes a `const std::vector<float>& input`, an integer `input_rate`, an integer `output_rate`, and a `double` cutoff parameter (between 0.01 and 1.0, defaulting to 0.80). It returns a `std::vector<float>` containing the resampled audio. The converter must use a fixed number of taps (e.g., 45) and a Kaiser window with a fixed beta (e.g., 16.0), and must correctly handle both upsampling and downsampling, including arbitrary integer ratios. The output length must be computed using the formula `ceil(input_length * output_rate / input_rate)` (i.e., no extra samples beyond the theoretical number). The implementation must be self-contained, avoid global state, and must produce deterministic results.
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    using std::vector;

    // Constant signal: resampling should preserve value.
    vector<float> const_input(100, 1.0f);
    auto out = resample_mono_ratio(const_input, 1000, 2000);
    assert(out.size() == 200); // ceil(100*2000/1000) = 200
    for (float v : out) assert(std::fabs(v - 1.0f) < 1e-3f);

    // Same rate: should be identity (within tolerance)
    vector<float> ramp;
    for (int i = 0; i < 10; ++i) ramp.push_back(static_cast<float>(i));
    auto same = resample_mono_ratio(ramp, 100, 100);
    assert(same.size() == 10);
    for (int i = 0; i < 10; ++i) assert(std::fabs(same[i] - ramp[i]) < 1e-2f);

    // Downsample by factor 2 with constant signal
    auto out2 = resample_mono_ratio(const_input, 2000, 1000);
    assert(out2.size() == 50);
    for (float v : out2) assert(std::fabs(v - 1.0f) < 1e-3f);

    // Empty input
    vector<float> empty;
    auto out_empty = resample_mono_ratio(empty, 44100, 48000);
    assert(out_empty.empty());

    // Impulse: single 1 at start, rest zeros. Output length should be correct.
    vector<float> impulse(20, 0.0f);
    impulse[0] = 1.0f;
    auto out_imp = resample_mono_ratio(impulse, 1000, 1000);
    assert(out_imp.size() == 20);
    assert(std::fabs(out_imp[0] - 1.0f) < 1e-2f);
    for (int i = 1; i < 20; ++i) assert(out_imp[i] < 0.5f); // tail decays

    // Upsample a short ramp
    vector<float> short_ramp = {0.0f, 1.0f, 2.0f};
    auto up = resample_mono_ratio(short_ramp, 1, 2);
    assert(up.size() == 6); // ceil(3*2/1) = 6
    // At integer positions (even indices), should be close to input
    assert(std::fabs(up[0] - 0.0f) < 0.1f);
    assert(std::fabs(up[2] - 1.0f) < 0.1f);
    assert(std::fabs(up[4] - 2.0f) < 0.1f);

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

namespace resample_detail {

double sinc(double x) {
    if (std::abs(x) < 1e-12) return 1.0;
    double pix = M_PI * x;
    return std::sin(pix) / pix;
}

double bessel_i0(double x) {
    double sum = 1.0;
    double term = 1.0;
    double x2 = x * x;
    for (int n = 1; n <= 20; ++n) {
        term *= x2 / (4.0 * n * n);
        sum += term;
        if (term < 1e-15) break;
    }
    return sum;
}

int gcd_int(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

std::vector<double> make_kaiser_window(int taps, double beta) {
    std::vector<double> win(taps);
    int mid = taps / 2;
    double i0_beta = bessel_i0(beta);
    for (int i = 0; i < taps; ++i) {
        double x = (i - mid) / static_cast<double>(mid);
        if (mid == 0) x = 0.0;
        win[i] = bessel_i0(beta * std::sqrt(1.0 - x * x)) / i0_beta;
    }
    return win;
}

} // namespace resample_detail

// Resample a single-channel audio signal from input_rate to output_rate
// using windowed-sinc interpolation (Kaiser window, 45 taps).
// cutoff is a fraction of the lower Nyquist frequency (0.01 to 1.0).
std::vector<float> resample_mono_ratio(
    const std::vector<float>& input,
    int input_rate,
    int output_rate,
    double cutoff = 0.80)
{
    if (input_rate <= 0 || output_rate <= 0) return {};
    if (input.empty()) return {};

    // Reduce ratio to lowest terms
    int g = resample_detail::gcd_int(input_rate, output_rate);
    int A = input_rate / g;  // reduced input rate
    int B = output_rate / g; // reduced output rate

    // Effective cutoff frequency (cycles per input sample, 1 = Nyquist)
    double min_ratio = std::min(1.0, static_cast<double>(B) / A);
    double f_c = cutoff * min_ratio * 0.5;

    const int taps = 45; // must be odd
    int mid = taps / 2;
    auto window = resample_detail::make_kaiser_window(taps, 16.0);

    // Output length: ceiling(input_length * output_rate / input_rate)
    size_t out_len = (input.size() * static_cast<size_t>(output_rate) + input_rate - 1) / input_rate;
    std::vector<float> output(out_len);

    for (size_t o = 0; o < out_len; ++o) {
        // Position in input samples
        double pos = static_cast<double>(o) * A / B;
        int n = static_cast<int>(std::floor(pos));
        double frac = pos - n;

        double acc = 0.0;
        for (int t = 0; t < taps; ++t) {
            int k = t - mid;
            long idx = static_cast<long>(n) + k;
            double sample = 0.0;
            if (idx >= 0 && idx < static_cast<long>(input.size())) {
                sample = input[static_cast<size_t>(idx)];
            }
            // Coefficient for this tap: window[t] * 2*f_c * sinc(2*f_c * (k - frac))
            double coeff = window[t] * 2.0 * f_c * resample_detail::sinc(2.0 * f_c * (k - frac));
            acc += sample * coeff;
        }
        output[o] = static_cast<float>(acc);
    }
    return output;
}
// The core algorithm follows the provided `res_init` and `push` logic but simplified to a single channel and a fixed-tap finite impulse response (FIR) filter. First, reduce the input/output rate ratio by dividing both by their greatest common divisor (GCD) to avoid oversampling unnecessarily. The filter table is generated as a set of polyphase subfilters: for each output phase index `p` from 0 to `outfreq-1` (where `outfreq` is the reduced output rate), create a filter of `taps` coefficients. Each coefficient at position `t` (0 to taps-1) corresponds to a sinc function sampled at `(t - taps/2 + p/infreq) * cutoff * input_rate / output_rate`? Wait – more precisely, from the original code, `filt_sinc` fills a table of `outfreq * taps` coefficients, where the coefficient for phase `p` and tap `t` is `sin(pi * x * s) / (pi * x) * step` with `x = t - taps/2 + p/infreq`, `s = cutoff / step`? Actually `step` is `outfreq` in the original call, and `s = cutoff / outfreq`? Let’s derive: The original `filt_sinc(state->table, outfreq*taps, outfreq, cutoff, gain, taps)` – here `N = outfreq*taps`, `step = outfreq`, `fc = cutoff`, `width = taps`. The coefficient sequence is generated with `x` starting at `-mid` where `mid = (N-1)/2`? Since `N` may be odd or even, the original handles even `N` by adding a zero. For our simplified version, we can choose `taps` odd (e.g., 45) and `outfreq * taps` may be even/odd. To keep it simple, we will directly compute the coefficients for each phase and tap using a formula consistent with the original: For phase `p` (0 to outfreq-1) and tap index `t` (0 to taps-1), define `x = t - (taps-1)/2.0 + p / (double)infreq`? Actually the original uses `x = -mid` to `mid` stepping by 1, and for each output sample, the source index moves in steps of `infreq/outfreq`. A common approach: coefficient for phase `p` and tap `t` is `sinc((t - center) * infreq/outfreq * cutoff)`? Let’s examine the original `sum` function: it multiplies `*scale` (from table starting at `state->table + offset*taps`) with `*source` (source pointer moving backward by `srcstep` each tap). The offset advances by `infreq` each output sample, and when offset >= outfreq, subtract outfreq and advance source. So effectively, each output sample uses a filter where the phase offset (in input-sample fractional units) is `offset/outfreq`? Actually offset is integer, and each tap corresponds to source index `source - t` (since srcstep=1 and we move backward). So the filter coefficient for tap t and phase offset (integer) is `table[offset*taps + t]`. The sinc function in `filt_sinc` uses `x = -mid, -mid+1, ...` and for each phase p, the coefficient is `sin(x * pi * s)/(x * pi) * step` with `x` being an integer. But note `s = fc / step` where `step` is outfreq, so `s = cutoff / outfreq`. For a given phase p (0..outfreq-1) and tap t (0..taps-1), the index into the table is `p*taps + t`, and `x` in `filt_sinc` is `(-mid + (p*taps + t))`? Because the loop increments dest by `width` each iteration and increments x by 1, so for linear index `n` (0..outfreq*taps-1), `x = -mid + n`. Then `dest` for that n is placed at `state->table + (n % outfreq)*taps + (n / outfreq)`? Actually the modulo pattern is complex. To avoid duplication, we can generate the filter table more directly. A standard polyphase filter for resampling by ratio `infreq/outfreq` (after GCD reduction) uses a low-pass filter with cutoff = min(1, outfreq/infreq) * cutoff_user. The filter length is `taps * outfreq`? Actually the number of coefficients per phase is `taps` (odd). For each phase `p` (0..outfreq-1) and tap index `t` (0..taps-1), the continuous-time impulse response is `h(t) = sinc( (t - center) * f_c )` where `center = (taps-1)/2` and `f_c` is the normalized cutoff relative to the input sample rate. Then the coefficient for phase p is `h( (t - center) + p/infreq )`? Let’s use the known formula from the original: In `filt_sinc`, `x` is an integer ranging from `-mid` to `mid`, and `s = fc/step` where `step` is outfreq. So the coefficient is `sin(x * pi * fc/outfreq) / (x * pi) * outfreq`. Because `step` multiplies. Then `x` increments by 1 for each table entry, but the table is interleaved by phase. For a given phase p and tap t, the table entry index is `p*taps + t`? Actually in the original, after call, `state->table` is generated such that `state->table + offset*taps` points to the starting coefficient for that phase (offset is 0..outfreq-1). So `table[offset*taps + t]` is coefficient for phase offset and tap t. How does `filt_sinc` produce that? It fills `N = outfreq*taps` coefficients, with `dest` starting at table[0] and stepping by `width` (which is taps) after each coefficient. So the linear order is: table[0], table[taps], table[2*taps], ... i.e., for phase 0, tap 0; then phase 1, tap 0; ... That means the linear index `n` corresponds to `phase = n % outfreq`? Actually stepping by `taps` each time, so index `n` maps to `phase = n / taps`? Wait: n=0 -> dest[0] (phase 0, tap 0), n=1 -> dest[taps] (phase 1, tap 0), n=2 -> dest[2*taps] (phase 2, tap 0), ... up to n=outfreq-1 -> dest[(outfreq-1)*taps] (phase outfreq-1, tap 0). Then n=outfreq -> dest[1] (phase 0, tap 1)? Because `dest` is incremented by `width` each time, but also there is a wrap when dest reaches endpoint: `if (dest >= endpoint) dest = ++base;` So after writing to dest[(outfreq-1)*taps], next dest is `base+1` (i.e., table[1]). So the pattern is: for each tap index t from 0, fill all phases for that tap, then move to next tap. So the linear index `n` maps to `tap = n / outfreq`, `phase = n % outfreq`. And `x` is computed as `-mid + n`? But `mid = N/2` when N odd, and when N even they adjust. This is messy. To simplify, we can generate the filter coefficients directly using the standard formula: For each phase p (0..outfreq-1) and tap t (0..taps-1), compute the delay in input-sample units: `delay = (t - (taps-1)/2.0) + p / (double)infreq`? Wait, the original uses `x` integer and `s = fc/outfreq`. In the interpolation, when we produce an output sample at time `source_index + offset/outfreq` (where offset is an integer from 0 to outfreq-1), we want the filter to be centered at that fractional position. The input sample spacing is 1 unit. The FIR filter should sample the impulse response at positions `(t - center) + offset/outfreq`? Actually a common formulation: For ratio `infreq/outfreq`, the filter taps for phase p are `h[ t * infreq/outfreq - p/outfreq ]`? Let's derive from the original `sum` function. It takes `source` pointer and moves backward by `srcstep` (which is 1) each tap, so tap t corresponds to input sample `source - t`. To produce an output sample at time `T_out`, the corresponding input time is `T_in = T_out * infreq/outfreq`. The filter sum is `sum_{t=0}^{taps-1} table[offset*taps + t] * input[floor(T_in) - t]` (assuming source points to floor(T_in)). The table coefficient should approximate `h( (floor(T_in) - (floor(T_in)-t)) - T_in )` = `h( t - (T_in - floor(T_in)) )` = `h( t - frac )` where `frac = T_in - floor(T_in)`. Here `offset` advances by `infreq` each output sample, and when it exceeds `outfreq`, we subtract `outfreq` and increment source by 1. So `offset` is an integer that represents the numerator of the fractional part: `frac = offset / outfreq`? Actually after each output sample, `offset += infreq`, then while `offset >= outfreq`, subtract outfreq and advance source. So at the moment of computing an output sample, `offset` is in range [0, outfreq-1] and `source` points to the input sample floor(T_in). The fractional part is `offset/outfreq`? That would be if `T_in = source_index + offset/outfreq`. Since offset increments by infreq each output sample, and source increments when offset wraps, that matches: after one output sample, T_in increases by infreq/outfreq. So yes, `frac = offset / (double)outfreq`. Then the ideal coefficient for tap t is `h( t - center + frac )`? Actually if we define filter impulse response `h(x)` centered at 0, the contribution of input sample at index `source - t` to output at fractional position `source + frac` (where `frac` is between 0 and 1) is `h( (source - t) - (source + frac) ) = h( -t - frac )` = `h( t + frac )` if symmetric. So coefficient = `h( t + frac )` where `h` is a low-pass sinc. But note in the original code, the filter table is generated with `x` being an integer from -mid to mid, and `s = fc/outfreq`? That seems to suggest the filter is scaled differently. Actually let's look at `filt_sinc`: `*dest = (x ? sin(x * M_PI * s) / (x * M_PI) * step : fc) * gain;` where `s = fc / step` (step = outfreq). So coefficient = `sin(x * pi * (fc/outfreq)) / (x * pi) * outfreq`. For large x, this approximates `outfreq * fc * sinc(x * fc / outfreq)`? Since `sin(a)/ (pi * x)` = `sinc(a) * a / (pi x)`? Better to derive: `sin(x * pi * fc/outfreq) / (x * pi) * outfreq = outfreq * (sin(x * pi * fc/outfreq) / (x * pi * fc/outfreq)) * (fc/outfreq) = fc * sinc(x * fc/outfreq)`. So the coefficient is `fc * sinc(x * fc/outfreq)`. So the filter has a bandwidth `fc` (in normalized frequency relative to input sample rate?) Actually the cutoff frequency parameter `fc` is a fraction of the output rate? In `res_init`, after reducing rates, if outfreq < infreq (downsampling), they multiply cutoff by outfreq/infreq. So the effective cutoff is `cutoff * min(outfreq/infreq, 1)` relative to the input sample rate. But in `filt_sinc`, `fc = cutoff` (adjusted) and `step = outfreq`, so the sinc argument is `x * fc/outfreq`. This is essentially the same as using a cutoff of `fc/outfreq` cycles per input sample? Hmm.
//
// Given the complexity, for a standalone task, we can simplify and present a standard polyphase resampler with explicit formula: precompute a low-pass filter of length `taps` (odd) using a windowed sinc with cutoff = min(1, output_rate/input_rate) * user_cutoff. Then for each output sample, compute the fractional input position and interpolate by summing `taps` consecutive input samples weighted by the appropriate phase. To handle fractional positions efficiently, we can either compute coefficients on the fly (simpler, but O(taps) per sample) or build a phase table. For the task, on-the-fly is fine, but we can also build a table for clarity. The task says "using the same windowed-sinc interpolation approach", so we must include a Kaiser window. We'll define a helper `sinc` and `kaiser` functions.
//
// Algorithm steps:
// 1. Compute GCD of input_rate and output_rate, reduce both: `reduced_in = input_rate/gcd`, `reduced_out = output_rate/gcd`.
// 2. Determine the cutoff frequency relative to the input rate: If downsampling (reduced_out < reduced_in), effective_cutoff = cutoff * (double)reduced_out / reduced_in; else effective_cutoff = cutoff. Also, the filter cutoff should not exceed 0.5 (Nyquist) but given cutoff <= 1.0, it's fine.
// 3. Precompute a windowed-sinc filter of length `taps` (odd, e.g., 45) with center at (taps-1)/2 and cutoff = effective_cutoff. For each tap index `i` (0..taps-1), compute `x = i - center`; coefficient = `sinc(x * effective_cutoff) * kaiser_window(i)`? Actually standard windowed sinc: `h[i] = sinc(2 * effective_cutoff * (i - center)) * window[i]`? Need to be careful with normalization. The original uses `sin(x * pi * s)/(x*pi) * step` where s = fc/outfreq, so that's essentially `fc * sinc(x * fc/outfreq)`. So if we let `cutoff_norm = effective_cutoff` (relative to input sample rate? Actually it's relative to output rate in the original? Let's not overcomplicate. For a resampler, a common approach: design a low-pass filter with cutoff frequency `min(1, output_rate/input_rate) * cutoff` relative to the input sample rate (where 1 corresponds to Nyquist? Usually digital filter cutoff is normalized to [0,1] where 1 is Nyquist). But the original uses `fc` directly and later multiplies by `step` which is outfreq. Since the filter is applied to input samples at unit spacing, the filter's frequency response should have cutoff at `min(output_rate, input_rate) * cutoff` in absolute frequency. To avoid confusion, we can derive the coefficient from the original formula directly: For a given fractional offset `frac` (0<=frac<1) where frac = offset/outfreq, the coefficient for tap t is `h( (t - center) + frac )` where `h(u) = fc * sinc(u * fc / outfreq)`? Actually the original uses `x` integer and `s = fc / outfreq`, so `x` is an integer shift. For a given phase p, the set of x values are `-mid + n` for n = p, p+outfreq, p+2*outfreq, ... up to taps-1? Let's compute an example: For outfreq=2, taps=3, N=6. mid=3? Actually if N even (6), they add a zero and reduce N to 5, mid=2. So x runs from -2 to 2. The table indices: n=0 -> phase 0 tap 0? Actually n=0 -> dest[0] (phase 0, tap 0), x=-2. n=1 -> dest[taps]=dest[3] (phase 1, tap 0), x=-1. n=2 -> dest[2*taps]=dest[6]? but N=6, so dest[6] is out of range? Hmm. This is messy.
//
// Given the task is for teaching, I will define a clear and correct implementation that matches the spirit of the original but is simpler to understand. I will use the standard polyphase filter design:
// - Let `ratio = output_rate / input_rate` (after GCD reduction? Actually we can keep original rates, but using reduced rates is fine because the impulse positions repeat with period output_rate/gcd).
// - The output sample `o` corresponds to input position `o * input_rate / output_rate`.
// - We will precompute a phase table of size `output_rate_reduced` arrays, each of length `taps`. For phase `p`, the coefficient for tap `t` is `sinc( (t - center) - (p / output_rate_reduced) )`? Wait: For output sample o, the fractional input position is `frac = (o * input_rate) % output_rate` (if using reduced rates, it's cleaner). Then we want to compute `sum_{k=0}^{taps-1} h( (k - center) + frac / input_rate_reduced? )`? Let me derive properly.
//
// Let reduced_in = A, reduced_out = B, with gcd(A,B)=1. Output sample index `out_idx` corresponds to input time `T = out_idx * A / B`. The integer part is `n = floor(T)`, fractional part is `frac = T - n = (out_idx * A) % B / B`. We want to compute the value as a weighted sum of input samples from `n - (taps-1)/2` to `n + (taps-1)/2` (or centered at n). The ideal interpolation filter is a low-pass sinc with cutoff frequency `f_c` (normalized to input sample rate, 1 = Nyquist). The impulse response is `h(t) = 2 f_c sinc(2 f_c t)` for t real. For a delay of `frac`, the coefficient for input sample at index `n - d` (where d is offset from n) is `h( d + frac )`? Actually the sample at index `n - d` has time `n - d`, and we want value at time `n + frac`. The distance is `(n - d) - (n + frac) = -d - frac`, so due to symmetry `h(d + frac)`. So for tap index `t` from 0 to taps-1, let `d = t - center` (center = (taps-1)/2). Then coefficient = `h( d + frac )` where `h(u) = 2 f_c sinc(2 f_c u)`.
//
// Now for the original code, they used `s = fc/outfreq` and coefficient = `sin(x * pi * s)/(x*pi) * outfreq`. Let's see if that matches: If we set `f_c = fc * outfreq?` Not exactly. But for our implementation, we can directly compute using a standard formula. To be consistent with the original, we can use the same formula: coefficient = `(x == 0 ? fc : sin(x * pi * s) / (x * pi) * outfreq)` with `x = t - center + frac` and `s = cutoff / outfreq`? But then for each phase, `frac = p / outfreq` (if we use reduced outfreq as B). Actually in the original, `x` is integer, not fractional, because the filter table is only for integer offsets. The fractional part is handled by choosing different phase filters, each with a different integer shift. So for phase p, we use the filter coefficients evaluated at integer points `x = -mid, -mid+1, ...` but the actual delay for that phase is `p/outfreq`. The relationship is that the filter coefficient for phase p and tap t is `h( (t - center) + p/outfreq )`? That would require non-integer x. But the original uses integer x only because they shift the filter by integer amounts? Actually if you consider a filter with continuous impulse response, sampling it at non-integer positions gives a different set of coefficients for each phase. The original generates the table by having `x` be an integer that increments by 1 across all phases. For a given tap t, the x values for phases 0..outfreq-1 are consecutive integers. That means the filter is effectively sampled at positions `base + p` for each phase p, where base depends on tap. That is equivalent to having the filter shifted by `p` samples for phase p, but the fractional delay is `p/outfreq`? That seems off by a factor of outfreq.
//
// I think the cleanest approach for the solution is to compute coefficients on the fly for each output sample using the formula `h(d + frac)` where `d` is integer offset and `frac` is fractional part from 0 to 1. That is a standard and correct method that matches the intent of the original (windowed-sinc interpolation). The original code is an optimization that precomputes a phase table; but for a teaching task, on-the-fly is fine and simpler. However, the task says "resample_mono_ratio" and mentions using "the same windowed-sinc interpolation approach" – we can implement a function that builds a phase table for efficiency, but we can also do on-the-fly. To be safe and match the performance-conscious original, I'll build a phase table: after GCD reduction, we have A (input rate reduced) and B (output rate reduced). We'll precompute a table of B phases, each with `taps` coefficients. For phase p (0..B-1), the coefficient for tap t is computed using `frac = p / B` (because the fractional input position modulo 1 is `p/B`? Actually output sample `out_idx` has `out_idx % B` as phase? Since `T = out_idx * A / B`, the fractional part is `(out_idx * A) % B / B`. Since A and B are coprime, as out_idx increments, the phase cycles through all residues modulo B. So we can index phase by `p = (out_idx * A) % B`. Then the fractional part is `p / B`. So for each phase p, we compute coefficients for tap t as `h( d + p/B )` where `d = t - center`. To avoid recomputing sinc for each output sample, we precompute table[p][t].
//
// For the sinc function, use `sinc(x) = sin(pi x)/(pi x)` for x != 0, else 1. The ideal low-pass filter with cutoff frequency `fc` (in cycles per input sample, where 1 = Nyquist = 0.5 cycles/sample? Actually Nyquist is 0.5 cycles/sample) has impulse response `2 fc sinc(2 fc t)`. But the original uses `fc` directly without the factor 2? In the original, `s = fc/outfreq` and coefficient = `sin(x * pi * s)/(x*pi) * outfreq`. For large x, this is approximately `outfreq * fc * sinc(x * fc / outfreq)`. Since `sinc(x * fc/outfreq)` has first zero at `x = outfreq/fc`, which corresponds to a time of `outfreq/fc` input samples. The cutoff frequency in cycles per input sample is `fc / (2*outfreq)`? This is getting complicated.
//
// Given the ambiguity, I will use a standard definition: The filter is designed with a normalized cutoff frequency `fc_normalized = min(1.0, (double)output_rate / input_rate) * cutoff` where cutoff is user-specified (0.01 to 1.0). Here `1.0` corresponds to the Nyquist frequency (0.5 cycles per sample). So the impulse response is `2 * fc_normalized * sinc(2 * fc_normalized * t)`. However, when resampling, we also need to account for the fact that the output sample spacing is `input_rate/output_rate` in input-sample units. A simpler approach used in many textbooks: For a fractional delay `frac`, the interpolated value is `sum_{k} x[k] * sinc( (k - (n + frac)) * L )` where L is the interpolation factor. For a general ratio, we can use a windowed sinc with a cutoff at `min(output_rate, input_rate)/2` in absolute frequency, then when sampling the impulse response, we evaluate at times `(t - center) * input_rate/output_rate + frac`? Hmm.
//
// Let's look at the original code again: In `push`, source pointer moves backward by 1 each tap, and offset advances by `infreq` per output sample. So the filter tap spacing in input samples is 1. The coefficient for phase `p` is stored in table at `p*taps + t`. In `filt_sinc`, `s = fc/step` where step = outfreq. For the first tap (t=0), is x = -mid? That means the filter is centered at mid taps from the source pointer. For a given output sample, source pointer points to the input sample at fractional position `offset/outfreq`? Actually offset is integer, and the fractional position is `offset/outfreq`? But offset starts at 0, and after each output sample, offset += infreq; when it wraps, source advances by 1. So the output sample is at time `source_index + offset/outfreq`. So the filter center is at `source_index + offset/outfreq`. The input sample at index `source_index - t` has distance `-t - offset/outfreq` from center. The ideal coefficient should be `h( -t - offset/outfreq )` = `h( t + offset/outfreq )`. In the original, `x` is an integer from -mid to mid, and the coefficient for tap t is stored such that when we sum over t from 0 to taps-1, we use `table[offset*taps + t]`. But the generation of table using `x` integer suggests that for a given phase offset, the set of x values are offset, offset+outfreq, offset+2*outfreq, ... up to taps? That would give x = offset + t*outfreq? That can't be right because x should be an integer around -mid.
//
// After careful reading, I realize the original `filt_sinc` fills the table in a "scattered" order: it iterates `x` from -mid to mid, and writes to `dest` which increments by `taps` each iteration, wrapping around when it reaches the end. So for a given phase p (0..outfreq-1), the coefficients for taps 0,1,2,... are stored at table[p], table[p+taps], table[p+2*taps], ...? Actually table is a flat array of size outfreq*taps. When we do `sum(state->table + *offset * state->taps, state->taps, ...)`, it reads consecutive taps from that offset. So for phase offset, tap t index is `offset*taps + t`. That means the generation must write coefficients such that `table[offset*taps + t]` gets the coefficient for tap t for phase offset. But `filt_sinc` writes with `dest` incrementing by `taps` each iteration, so it writes table[0], table[taps], table[2*taps], ... for x = -mid, -mid+1, ... So `table[offset*taps + t]` = coefficient for x = -mid + (t*outfreq + offset). Because after writing `offset` many times for different phases, the pattern emerges: For each x, it writes to the position `(x+mid)*outfreq`? Let's simulate with outfreq=3, taps=5, N=15, mid=7 (since odd, mid=7). x runs from -7 to 7. dest starts at table[0]. For x=-7: write table[0]. Then dest += 5 -> table[5]. x=-6: write table[5]. dest+=5 -> table[10]. x=-5: write table[10]. dest+=5 -> table[15] is endpoint, so dest = ++base = table[1]. Then x=-4: write table[1]. dest+=5 -> table[6]. x=-3: write table[6]. dest+=5 -> table[11]. x=-2: write table[11]. dest+=5 -> table[16] -> wrap to table[2]. x=-1: write table[2]. dest+=5 -> table[7]. x=0: write table[7]. dest+=5 -> table[12]. x=1: write table[12]. dest+=5 -> table[17] -> wrap to table[3]. x=2: write table[3]. dest+=5 -> table[8]. x=3: write table[8]. dest+=5 -> table[13]. x=4: write table[13]. dest+=5 -> table[18] -> wrap to table[4]. x=5: write table[4]. dest+=5 -> table[9]. x=6: write table[9]. dest+=5 -> table[14]. x=7: write table[14]. dest+=5 -> table[19] -> wrap to table[5]? but N=15 so table has indices 0..14. Actually after writing table[14], dest becomes table[15] which is endpoint, so dest = ++base = table[1]? But we already have table[1] filled. There's an assert that dest equals origdest+width at the end, but that's only if N is odd? For even N, they add extra zero. This is getting too detailed.
//
// Given the complexity, for the solution I will implement a simpler and mathematically correct version: For each output sample, compute the fractional input position `pos = out_idx * (double)input_rate / output_rate`. Let `n = floor(pos)`, `frac = pos - n`. Then compute the interpolated value as `sum_{k=0}^{taps-1} input[n + k - center] * windowed_sinc( (k - center - frac) * cutoff_norm )` where `cutoff_norm` is the normalized cutoff frequency (e.g., `min(1, output_rate/input_rate)` times user cutoff) but we must be careful with scaling. Actually to avoid aliasing, the low-pass filter cutoff should be `min(output_rate, input_rate) / 2` in absolute frequency (Hz). In our digital domain, we sample at input rate, so normalized cutoff (relative to input rate) is `min(1, output_rate/input_rate) * 0.5` in cycles/sample. But the user provides `cutoff` from 0.01 to 1.0, likely meaning a fraction of Nyquist. So we set `f_c = cutoff * min(1, (double)output_rate / input_rate) * 0.5`? Actually the original multiplies cutoff by outfreq/infreq when downsampling. Then uses `s = fc/outfreq` in sinc. Let's derive from the original: For downsampling (outfreq < infreq), they set `cutoff = cutoff * outfreq / infreq`. Then in `filt_sinc`, `s = cutoff / outfreq` = `(cutoff_orig * outfreq/infreq) / outfreq` = `cutoff_orig / infreq`. So `s = cutoff_orig / infreq`. Then coefficient = `sin(x * pi * s)/(x*pi) * outfreq` = `outfreq * sin(x * pi * cutoff_orig/infreq)/(x*pi)`. This is approximately `outfreq * cutoff_orig/infreq * sinc(x * cutoff_orig/infreq)`. So the effective impulse response has amplitude scaled by `outfreq * cutoff_orig/infreq` and zero-crossing spacing `infreq/cutoff_orig`. The cutoff frequency in cycles per input sample is `cutoff_orig / (2*infreq)`? That seems very small. I think the original code is using `cutoff` as a fraction of the output Nyquist frequency? Actually `cutoff` default 0.8, and they multiply by outfreq/infreq for downsampling. So `cutoff` is likely meant as a fraction of the output sample rate? But they also use `step` in the sum. It's confusing.
//
// For a teaching task, it's better to use a clear and standard formulation. I will define the function to use a low-pass filter with cutoff `cutoff * min(output_rate, input_rate) / output_rate`? No. Let me just state in the solution that the cutoff is a fraction of the Nyquist frequency of the lower of the two rates, and the filter is designed with a sinc function scaled accordingly. I will implement the filter using `double cutoff_eff = cutoff * std::min(1.0, (double)output_rate / input_rate);` where 1.0 corresponds to Nyquist (0.5 cycles per sample). Then the impulse response is `2 * cutoff_eff * sinc(2 * cutoff_eff * t)`. For the window, use Kaiser with beta=16.0.
//
// So algorithm:
// - Compute gcd of input_rate, output_rate. Let A = input_rate/gcd, B = output_rate/gcd.
// - Let f_c = cutoff * min(1.0, (double)B / A) * 0.5? Actually min(1, B/A) is a factor, and if we treat 1.0 as Nyquist (0.5 cycles/sample), then f_c in cycles per sample is `cutoff * min(1.0, B/A) * 0.5`? But if B/A > 1 (upsampling), we don't want to reduce cutoff; we keep cutoff as user-specified fraction of input Nyquist. So f_c = cutoff * 0.5 cycles/sample. For downsampling (B/A < 1), we reduce cutoff to `cutoff * B/A * 0.5`? Actually to avoid aliasing after downsampling, the cutoff should be at most the output Nyquist, which is `B/A * input_Nyquist` = `B/A * 0.5` cycles per input sample. So f_c = cutoff * min(1, B/A) * 0.5. Good.
// - Precompute Kaiser window coefficients of length `taps` (odd). For tap index i, compute `window[i] = I0(beta * sqrt(1 - ((i-center)/center)^2)) / I0(beta)` where center = (taps-1)/2.
// - For each output sample, we could compute coefficients on the fly, but we can also precompute a phase table for performance. Given that the task is simple, I'll compute on the fly for clarity, but mention that a phase table could be used.
// - For output sample index `out_idx`, compute `pos = out_idx * (double)input_rate / output_rate` (use original rates, not reduced; but using reduced rates is fine as long as we handle the position correctly). Let `n = floor(pos)`, `frac = pos - n`. We need to gather input samples from `n - center` to `n + center` (since taps is odd, total taps = 2*center+1). For each tap offset `k` from -center to center, let `t = k + center` (0..taps-1). The input index is `n + k`. The coefficient is `window[t] * 2*f_c * sinc(2*f_c * (k - frac))`? Actually the delay from the output time `pos` to the input sample at index `n+k` is `(n+k) - pos = k - frac`. So coefficient = `window[t] * 2*f_c * sinc(2*f_c * (k - frac))`. However, note that for upsampling, the filter should also include a gain factor equal to the ratio `output_rate/input_rate`? In standard resampling, the filter should have a gain of `output_rate/input_rate` for upsampling to preserve amplitude (since the sinc is scaled by interpolation factor). The original multiplies by `step` which is outpout rate, but after GCD reduction, step is reduced outfreq. Actually in the original, the sum is `total += *source * *scale` and there's no additional multiplication by ratio. The scaling is embedded in the table coefficients. For upsampling, the table coefficients should be scaled by `outfreq` (the reduced output rate) to get correct amplitude. In our formula, the ideal low-pass filter for interpolation with factor L should have gain L. Here the factor is `output_rate/input_rate`. After GCD reduction, the ratio is `B/A`. So we should multiply the coefficients by `(double)B / A`? For upsampling (B > A), that would be >1, correct. For downsampling, B/A <1, and the filter should have gain 1 (since we are just low-pass filtering, not interpolating). Actually the standard formula: For resampling by factor L/M, the ideal filter is a low-pass with cutoff `min(1, L/M)/2` and gain `L/M`? No, the gain should be `max(1, L/M)`? Let's derive: If we upsample by L (insert zeros) then low-pass filter with gain L. If we downsample by M, we low-pass filter with gain 1 then decimate. For arbitrary L/M, we can write as upsample by L, filter with cutoff min(1, L/M)/2 and gain L, then downsample by M. So the overall gain is L. In our case, the "L" is the numerator of the ratio after reduction? Actually the ratio is `output_rate/input_rate = B/A` after dividing by gcd. So for upsampling (B > A), gain = B/A? But B and A are reduced integers, so B/A is rational. The standard would be to upsample by B and downsample by A, so filter gain = B. But then after downsampling by A, the net gain is B/A. To simplify, we can just scale the filter coefficients by `(double)output_rate / input_rate`? Let's test with a simple case: input_rate=1000, output_rate=2000 (2x upsampling). The interpolated value at integer input positions should equal the input sample. When frac=0, coefficient for k=0 should be 1, and for other k should be 0 (ideal sinc). Our formula: 2*f_c * sinc(2*f_c * k) with f_c = cutoff*0.5, if cutoff=1, f_c=0.5, then 2*0.5*sinc(1*k) = sinc(k). At k=0, sinc(0)=1. So coefficient = 1, good. But for other k, sinc(k) is 0? Actually sinc(k) = sin(pi*k)/(pi*k) = 0 for integer k != 0. So the filter gives exactly the input sample, no scaling needed. For non-integer frac, it interpolates. So no extra gain needed when cutoff = 1 and we use the formula `2*f_c * sinc(2*f_c * t)`? But for a bandlimited interpolation with cutoff = 0.5 (Nyquist), the filter is exactly the sinc with zero-crossings at integers, which is the ideal reconstruction for sampling at rate 1. So for upsampling, the filter should have a gain of 1 at DC? The frequency response at DC for the sinc with cutoff 0.5 is `sum_{t} sinc(t) = 1`? Actually the ideal low-pass with cutoff 0.5 and passband gain 1 has impulse response sinc(t). At DC, the sum is 1. So if we use that, the output amplitude will be correct without extra gain. For downsampling, we need a low-pass filter with cutoff less than 0.5 (to avoid aliasing) and gain 1 to preserve DC. So the formula `2*f_c * sinc(2*f_c * t)` gives a filter with DC gain 1 (since integral of impulse response equals 2*f_c * (1/(2*f_c)) = 1). So no extra gain needed. However, when we resample, the output sample spacing is different, but since we are evaluating the filter at times scaled appropriately, the interpolation is correct. So I'll just use that formula, with f_c = cutoff * min(1, B/A) * 0.5, and windowed by Kaiser.
//
// Edge cases:
// - Input length 0: return empty vector.
// - If input_rate or output_rate <=0, return empty (or handle gracefully).
// - Need to handle boundary: when output sample index requires input indices beyond the input length, we should clamp to the first/last input sample? The original uses a pool with zero-padding via `calloc` for tail. In our standalone function, we can treat out-of-range indices as 0 (zero-padding) or clamp. The original uses zero-padding for tail (drain) and for initial fill, it holds partial data until enough samples. For a synchronous function that takes the entire input at once, we can simply zero-pad both ends. That is acceptable.
// - The output length is `(input_length * output_rate + input_rate - 1) / input_rate` (ceiling). That matches the original `res_push_check` formula.
// - For each output sample, we compute `pos = out_idx * input_rate / output_rate` (double), `n = (int)floor(pos)`, `frac = pos - n`. We then iterate from `k = -center` to `+center`, input index = `n + k`. If index <0 or >= input_length, use 0.0.
//
// Time complexity: O(output_length * taps) where output_length is ceiling(input_length * output_rate / input_rate). Since taps is constant (45), it's linear in the number of output samples. Space: O(taps) for window, plus O(1) extra.
//
// Now we implement the function `std::vector<float> resample_mono_ratio(const std::vector<float>& input, int input_rate, int output_rate, double cutoff=0.80)`. Include needed headers: vector, cmath, cstdint, algorithm.
//
// We'll define helper functions `sinc`, `bessel_i0`, `kaiser_window`. Use `double` for coefficients, then cast to float.
//
// We'll also implement a small gcd function.
//
// Test: Use simple sine wave or known cases. For example, a constant signal: all ones, resample from 1000 to 2000, output should be all ones (approximately). Also test with an impulse: a single 1 at index 0, rest zeros; resampling should produce a sinc-like output. But for assert, we can test with constant input.
//
// Now write the solution.
