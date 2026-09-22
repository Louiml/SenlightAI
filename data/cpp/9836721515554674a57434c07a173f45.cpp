Write a standalone C++ function that computes the velocity-velocity autocorrelation function (VVACF) from a time series of per-molecule velocity projections and selection flags, applying a windowing function and normalization. The function should take as input: a vector of velocity projection values (v_proj), a vector of velocity z-components (vz), a vector of boolean selection flags indicating which molecules contribute to the correlation, the number of molecules N per timestep, the total number of timesteps Traj_len, the correlation length tcfl, a time offset T_w, the timestep dt, and a reference mean_global (the average number of contributing molecules). The function should compute for each lag t_ from 0 to tcfl-1: sum over all selected molecules i and all start times t from 0 to Traj_len-tcfl-T_w-1 of vz[N*(t+T_w)+i] * v_proj[N*(t+t_+T_w)+i], divided by mean_global, then multiply by a cosine-squared window: pow(cos(PI * t_ * dt / (tcfl * dt * 2.0)), 2). The function must return a vector<double> of length tcfl containing the windowed, normalized VVACF values. Handle edge cases such as tcfl=0, Traj_len=0, N=0, or mean_global=0 gracefully (return empty vector or zeros). The function must be const-correct and use only standard library headers.
// The core algorithm iterates over each lag t_ from 0 to tcfl-1. For each lag, it accumulates a sum over all start times t from 0 to (Traj_len - tcfl - T_w - 1) inclusive, and over all molecules i from 0 to N-1. The condition for including a molecule is based on a selection flag (provided as a vector<bool> selection of size N*Traj_len, where selection[N*t+i] indicates whether molecule i at time t contributes). For each selected (t,i) pair, it adds the product vz[N*(t+T_w)+i] * v_proj[N*(t+t_+T_w)+i]. After accumulating the sum for lag t_, it divides by mean_global (if mean_global is nonzero, else leaves as 0), and multiplies by the windowing function cos²(π * t_ * dt / (2 * tcfl * dt)) = cos²(π * t_ / (2 * tcfl)). The time complexity is O(tcfl * (Traj_len - tcfl - T_w) * N), which could be large but is inherent to the problem. Space complexity is O(tcfl) for the result, plus O(N*Traj_len) for the input vectors (not counted as extra). Edge cases: if tcfl=0 or Traj_len=0 or N=0, return empty vector; if mean_global <= 0, return zeros (since division by zero or negative normalization is invalid); if Traj_len < tcfl + T_w + 1, then the start times loop has no iterations, so all lags are zero after normalization. Use size_t for indices to avoid overflow and ensure proper unsigned arithmetic. The selection flag vector should be a const vector<char> or vector<bool>; prefer vector<char> for clarity and to avoid potential bool vector specialization issues.
#include <vector>
#include <cmath>
#include <cstddef>

// Compute the windowed, normalized velocity-velocity autocorrelation function (VVACF).
// Parameters:
//   vz       - vector of velocity z-components for all molecules at all times (size N * Traj_len)
//   v_proj   - vector of velocity projections for all molecules at all times (size N * Traj_len)
//   selection- vector of char flags (0/1) indicating whether molecule i at time t contributes (size N * Traj_len)
//   N        - number of molecules per timestep
//   Traj_len - total number of timesteps
//   tcfl     - length of the correlation function (number of lags)
//   T_w      - time offset for the start of the correlation
//   dt       - timestep
//   mean_global - normalization factor (average number of contributing molecules)
// Returns:
//   a vector<double> of length tcfl containing the windowed VVACF values.
std::vector<double> compute_vvacf(
    const std::vector<double>& vz,
    const std::vector<double>& v_proj,
    const std::vector<char>& selection,
    std::size_t N,
    std::size_t Traj_len,
    std::size_t tcfl,
    std::size_t T_w,
    double dt,
    double mean_global)
{
    // Edge cases: return empty vector if no lags requested or insufficient data.
    if (tcfl == 0 || Traj_len == 0 || N == 0) {
        return std::vector<double>{};
    }
    // If normalization factor is invalid, return zeros (avoid division by zero).
    if (mean_global <= 0.0) {
        return std::vector<double>(tcfl, 0.0);
    }
    // If start times range is empty, all lags are zero.
    if (Traj_len < tcfl + T_w + 1) {
        return std::vector<double>(tcfl, 0.0);
    }

    // Result vector.
    std::vector<double> vvacf(tcfl, 0.0);
    const std::size_t start_time_count = Traj_len - tcfl - T_w; // number of valid start times t (0 .. start_time_count-1)
    const double inv_mean = 1.0 / mean_global;
    const double pi = 3.14159265358979323846;

    // Loop over each lag.
    for (std::size_t t_ = 0; t_ < tcfl; ++t_) {
        double sum = 0.0;
        // Sum over all start times.
        for (std::size_t t = 0; t < start_time_count; ++t) {
            // Offset for the base time (start time + T_w).
            const std::size_t base_index = N * (t + T_w);
            // Offset for the time at lag t_ (start time + t_ + T_w).
            const std::size_t shifted_index = N * (t + t_ + T_w);
            // Sum over selected molecules.
            for (std::size_t i = 0; i < N; ++i) {
                if (selection[base_index + i] != 0) {
                    sum += vz[base_index + i] * v_proj[shifted_index + i];
                }
            }
        }
        // Apply normalization and windowing.
        double window = std::cos(pi * static_cast<double>(t_) / (2.0 * static_cast<double>(tcfl)));
        window = window * window; // cos^2
        vvacf[t_] = sum * inv_mean * window;
    }
    return vvacf;
}
#include <cassert>
#include <cmath>
#include <vector>

// Function declaration (for testing, include the solution above or duplicate here).
std::vector<double> compute_vvacf(
    const std::vector<double>& vz,
    const std::vector<double>& v_proj,
    const std::vector<char>& selection,
    std::size_t N,
    std::size_t Traj_len,
    std::size_t tcfl,
    std::size_t T_w,
    double dt,
    double mean_global);

// Helper to compare doubles with tolerance.
bool close(double a, double b, double tol = 1e-9) {
    return std::abs(a - b) < tol;
}

int main() {
    // Test 1: Simple case with one molecule, one start time, tcfl=2.
    // N=1, Traj_len=3, tcfl=2, T_w=0, mean_global=1.
    // vz = [1, 2, 3], v_proj = [4, 5, 6], selection = [1,1,1].
    // Start time t=0 only (since start_time_count = Traj_len - tcfl - T_w = 1).
    // For t_=0: sum = vz[0]*v_proj[0] + vz[1]*v_proj[1]? Wait, t_=0: shifted index uses t+0+T_w, so base_index = t+T_w, shifted_index = t+0+T_w = same. For t=0: base_index=0, shifted=0: vz[0]*v_proj[0] = 1*4=4. But also t_=0 loops over t up to start_time_count-1=0, so only t=0. Sum=4. window = cos^2(π*0/(2*2))=1. vvacf[0]=4*1*1=4.
    // For t_=1: t=0: base_index=0, shifted=1: vz[0]*v_proj[1] = 1*5=5 (since T_w=0, t_=1). Actually shifted index = N*(t+t_+T_w)=1*(0+1+0)=1, so vz[0]*v_proj[1]=1*5=5. Sum=5. window = cos^2(π*1/(2*2))=cos^2(π/4)=0.5. vvacf[1]=5*1*0.5=2.5.
    {
        std::vector<double> vz = {1.0, 2.0, 3.0};
        std::vector<double> v_proj = {4.0, 5.0, 6.0};
        std::vector<char> selection = {1, 1, 1};
        auto result = compute_vvacf(vz, v_proj, selection, 1, 3, 2, 0, 1.0, 1.0);
        assert(result.size() == 2);
        assert(close(result[0], 4.0));
        assert(close(result[1], 2.5));
    }

    // Test 2: Edge case tcfl=0 should return empty.
    {
        std::vector<double> vz = {1.0};
        std::vector<double> v_proj = {1.0};
        std::vector<char> selection = {1};
        auto result = compute_vvacf(vz, v_proj, selection, 1, 1, 0, 0, 1.0, 1.0);
        assert(result.empty());
    }

    // Test 3: mean_global=0 returns zeros.
    {
        std::vector<double> vz = {1.0, 2.0};
        std::vector<double> v_proj = {3.0, 4.0};
        std::vector<char> selection = {1, 1};
        auto result = compute_vvacf(vz, v_proj, selection, 1, 2, 1, 0, 1.0, 0.0);
        assert(result.size() == 1);
        assert(result[0] == 0.0);
    }

    // Test 4: Selection flag filters molecules.
    // N=2, Traj_len=2, tcfl=1, T_w=0, mean_global=1.
    // vz for two molecules at t=0: [1,2], at t=1: [3,4]; v_proj: [5,6] and [7,8].
    // selection: only molecule 0 at t=0 and t=1 (flags: [1,0,1,0]).
    // start_time_count = Traj_len - tcfl - T_w = 1 (only t=0).
    // For t_=0: base_index = 0, shifted_index = 0. Sum over i=0..1: i=0 selection[0]=1 => vz[0]*v_proj[0]=1*5=5. i=1 selection[1]=0 => skip. Sum=5. window=cos^2(0)=1. vvacf[0]=5*1=5.
    {
        std::vector<double> vz = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> v_proj = {5.0, 6.0, 7.0, 8.0};
        std::vector<char> selection = {1, 0, 1, 0};
        auto result = compute_vvacf(vz, v_proj, selection, 2, 2, 1, 0, 1.0, 1.0);
        assert(result.size() == 1);
        assert(close(result[0], 5.0));
    }

    // Test 5: T_w offset changes which times are used.
    // N=1, Traj_len=4, tcfl=1, T_w=1, mean_global=1.
    // vz = [1,2,3,4], v_proj = [5,6,7,8], selection = all 1.
    // start_time_count = Traj_len - tcfl - T_w = 4-1-1=2 (t=0,1).
    // For t_=0:
    //   t=0: base_index = N*(0+1)=1 -> vz[1]=2, shifted_index = N*(0+0+1)=1 -> v_proj[1]=6 => 2*6=12.
    //   t=1: base_index = N*(1+1)=2 -> vz[2]=3, shifted_index = N*(1+0+1)=2 -> v_proj[2]=7 => 3*7=21.
    // Sum=33, window=1, vvacf[0]=33.
    {
        std::vector<double> vz = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> v_proj = {5.0, 6.0, 7.0, 8.0};
        std::vector<char> selection = {1, 1, 1, 1};
        auto result = compute_vvacf(vz, v_proj, selection, 1, 4, 1, 1, 1.0, 1.0);
        assert(result.size() == 1);
        assert(close(result[0], 33.0));
    }

    // Test 6: Insufficient Traj_len for given tcfl and T_w returns zeros.
    {
        std::vector<double> vz = {1.0};
        std::vector<double> v_proj = {1.0};
        std::vector<char> selection = {1};
        auto result = compute_vvacf(vz, v_proj, selection, 1, 1, 2, 0, 1.0, 1.0); // Traj_len < tcfl+T_w+1 (1 < 3)
        assert(result.size() == 2);
        assert(result[0] == 0.0 && result[1] == 0.0);
    }

    // Test 7: Normalization divides by mean_global.
    // Use Test 1 but with mean_global=2.0, values should be halved.
    {
        std::vector<double> vz = {1.0, 2.0, 3.0};
        std::vector<double> v_proj = {4.0, 5.0, 6.0};
        std::vector<char> selection = {1, 1, 1};
        auto result = compute_vvacf(vz, v_proj, selection, 1, 3, 2, 0, 1.0, 2.0);
        assert(result.size() == 2);
        assert(close(result[0], 2.0));  // 4/2
        assert(close(result[1], 1.25)); // 2.5/2
    }

    return 0;
}
