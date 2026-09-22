Write a standalone C++ function named `simulate_officer_pointing` that simulates a police event where multiple officers independently draw an exponential reaction time (with a given rate per officer), are processed in increasing reaction-time order, and each officer sequentially decides whether to "point" based on a logistic probability. The logistic predictor for an officer includes: a coefficient times an indicator of being female, a coefficient times years of experience, a coefficient times a shared event-level violence value (drawn from a standard normal distribution), a fixed effect coefficient times an officer-specific fixed effect, plus an additional "exposure" coefficient if any previous officer in the same event has already pointed. After the event, update each officer's history by appending a boolean indicating whether that officer was "exposed" (i.e., they were not the first pointer but saw someone point, or they pointed while others also pointed; for single-officer events, exposure is always false). The function should take as inputs: a vector of officer IDs (unique indices), a vector of boolean female flags, a vector of years of experience, a vector of reaction rates (positive doubles), a vector of fixed effects, a double for each of the six model parameters (female, years, exposure_event, exposure_prev, event_violence, officer_fe), and an integer seed for reproducibility. It should return a `std::vector<int>` of length equal to the number of officers, where each entry is 1 if that officer pointed and 0 otherwise, with the output in the same order as the input officers. Use `std::mt19937` for randomness, `std::exponential_distribution` for reaction times, `std::normal_distribution` (mean 0, std dev 1) for the event violence, and `std::uniform_real_distribution` (0,1) for the logistic step. Sort officers by reaction time using `std::stable_sort` with an index vector. Handle edge cases: empty input (return empty vector), single-officer event (exposure always false, and no exposure_event contribution because no prior pointer), and ensure reproducibility via the seed.
// The core algorithm processes officers in order of ascending reaction time. First, draw the event violence from `std::normal_distribution` (mean 0, std 1). Then for each officer, draw an exponential reaction time using their rate (the rate parameter of `std::exponential_distribution` is the inverse of the mean; in the snippet, the distribution is constructed with the rate value, so we use `std::exponential_distribution<>(rate)`). Create an index vector `idx` of size equal to officer count, initialize with `0..n-1`, then `std::stable_sort` based on `reaction_times[idx[i]]`. Iterate in sorted order: for each officer at position `i`, compute the base logistic predictor:
// `val = female * par_female + years * par_years + event_violence * par_event_violence + fixed_effect * par_fe`.
// If any previous officer in the sorted order already pointed (`n_pointed > 0`), add `par_exposure_event`. Note: for the first officer (i=0), no prior pointer exists, so no exposure_event term is added. Then draw `u = uniform(0,1)` and set `pointed[idx[i]] = (u < exp(val)/(1+exp(val)))`. If true, increment `n_pointed`. After processing all officers, update each officer's exposure history: for each officer, if the event has more than one officer, then if that officer pointed and `n_pointed > 1`, or if that officer did not point and `n_pointed > 0`, mark exposure true; otherwise false. If the event has exactly one officer, exposure is always false. However, since the function only returns the pointed vector, we do not need to store the full history; we only need to compute the pointed result, and the exposure history is irrelevant to the return value. So we can simplify: just compute `pointed` per officer as described. Important edge cases: reaction times could be equal; stable sort ensures deterministic order by original index among equal times. The event violence is drawn once per event, not per officer. The function must be thread-safe and reproducible given the same seed; we create a local `std::mt19937` seeded with the input seed. Time complexity: O(n log n) due to sorting, O(n) space for vectors. Note: the input rate must be positive; if not, we could clamp or assume valid input.
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>

/**
 * Simulate a single police event where officers react in exponential time order
 * and sequentially decide to point based on a logistic model.
 * 
 * @param officer_ids          Vector of unique officer identifiers (unused in logic but retained for clarity).
 * @param female               Vector of booleans (true if officer is female).
 * @param years                Vector of years of experience.
 * @param rates                Vector of positive reaction rates for exponential distribution.
 * @param fixed_effects        Vector of officer-specific fixed effects.
 * @param par_female           Coefficient for female indicator.
 * @param par_years            Coefficient for years.
 * @param par_exposure_event   Added to predictor if any prior officer pointed.
 * @param par_exposure_prev    Not used in a single-event simulation (included for API consistency).
 * @param par_event_violence   Coefficient for event-level violence.
 * @param par_officer_fe       Coefficient for officer fixed effect.
 * @param seed                 Seed for reproducible randomness.
 * @return                     Vector of ints (0/1) indicating whether each officer pointed, same order as inputs.
 */
std::vector<int> simulate_officer_pointing(
    const std::vector<int>& officer_ids,
    const std::vector<bool>& female,
    const std::vector<int>& years,
    const std::vector<double>& rates,
    const std::vector<double>& fixed_effects,
    double par_female,
    double par_years,
    double par_exposure_event,
    double par_exposure_prev,
    double par_event_violence,
    double par_officer_fe,
    int seed
) {
    // Number of officers in the event
    std::size_t n = officer_ids.size();
    if (n == 0) {
        return {};
    }

    // Random engine and distributions
    std::mt19937 engine(seed);
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    std::normal_distribution<double> normal(0.0, 1.0);

    // Event-level violence (drawn once)
    double event_violence = (std::abs(par_event_violence) > 1e-10) ? normal(engine) : 0.0;

    // Draw reaction time for each officer
    std::vector<double> reaction_times(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::exponential_distribution<double> exp_dist(rates[i]);
        reaction_times[i] = exp_dist(engine);
    }

    // Sort indices by reaction time (stable to keep original order on ties)
    std::vector<std::size_t> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(),
        [&reaction_times](std::size_t a, std::size_t b) {
            return reaction_times[a] < reaction_times[b];
        });

    // Process officers in sorted order
    std::vector<int> pointed(n, 0);
    int n_pointed = 0;
    for (std::size_t pos = 0; pos < n; ++pos) {
        std::size_t i = idx[pos];

        // Base predictor
        double val = (female[i] ? 1.0 : 0.0) * par_female
                   + static_cast<double>(years[i]) * par_years
                   + event_violence * par_event_violence
                   + fixed_effects[i] * par_officer_fe;

        // If any previous officer pointed, add exposure_event
        if (n_pointed > 0) {
            val += par_exposure_event;
        }

        // Logistic probability
        double prob = std::exp(val) / (1.0 + std::exp(val));
        if (unif(engine) < prob) {
            pointed[i] = 1;
            ++n_pointed;
        } else {
            pointed[i] = 0;
        }
    }

    return pointed;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Single officer, no exposure, deterministic seed
    {
        std::vector<int> ids = {0};
        std::vector<bool> fem = {true};
        std::vector<int> yrs = {5};
        std::vector<double> rates = {1.0};
        std::vector<double> fe = {0.0};
        // With very negative female coefficient, probability ~0, likely never points
        auto res = simulate_officer_pointing(ids, fem, yrs, rates, fe, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 42);
        assert(res.size() == 1);
        assert(res[0] == 0 || res[0] == 1);
    }

    // Two officers, verify order independence: with same seed, results identical
    {
        std::vector<int> ids1 = {0, 1};
        std::vector<bool> fem1 = {true, false};
        std::vector<int> yrs1 = {1, 2};
        std::vector<double> rates1 = {1.0, 2.0};
        std::vector<double> fe1 = {0.5, -0.5};
        auto res1 = simulate_officer_pointing(ids1, fem1, yrs1, rates1, fe1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 123);
        auto res2 = simulate_officer_pointing(ids1, fem1, yrs1, rates1, fe1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 123);
        assert(res1 == res2);
    }

    // Empty input returns empty
    {
        std::vector<int> ids;
        std::vector<bool> fem;
        std::vector<int> yrs;
        std::vector<double> rates;
        std::vector<double> fe;
        auto res = simulate_officer_pointing(ids, fem, yrs, rates, fe, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1);
        assert(res.empty());
    }

    // Three officers: ensure no crash and sizes match
    {
        std::vector<int> ids = {10, 20, 30};
        std::vector<bool> fem = {true, false, true};
        std::vector<int> yrs = {0, 3, 7};
        std::vector<double> rates = {0.5, 1.5, 2.0};
        std::vector<double> fe = {0.1, 0.2, -0.3};
        auto res = simulate_officer_pointing(ids, fem, yrs, rates, fe, 0.5, -0.2, 0.8, 0.0, 1.0, 0.0, 99);
        assert(res.size() == 3);
        for (int v : res) {
            assert(v == 0 || v == 1);
        }
    }

    // Extreme probabilities: always point with huge positive coefficient (just check range)
    {
        std::vector<int> ids = {0, 1};
        std::vector<bool> fem = {true, true};
        std::vector<int> yrs = {10, 10};
        std::vector<double> rates = {5.0, 5.0};
        std::vector<double> fe = {0.0, 0.0};
        auto res = simulate_officer_pointing(ids, fem, yrs, rates, fe, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7);
        assert(res.size() == 2);
    }

    // Check that exposure_event term can change results: run with and without, same seed, different outputs possible
    {
        std::vector<int> ids = {0, 1, 2};
        std::vector<bool> fem = {false, false, false};
        std::vector<int> yrs = {0, 0, 0};
        std::vector<double> rates = {1.0, 1.0, 1.0};
        std::vector<double> fe = {0.0, 0.0, 0.0};
        // Without exposure_event, all probabilities same; with large exposure_event, later officers more likely to point
        auto res_no = simulate_officer_pointing(ids, fem, yrs, rates, fe, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5);
        auto res_yes = simulate_officer_pointing(ids, fem, yrs, rates, fe, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 5);
        assert(res_no.size() == 3 && res_yes.size() == 3);
        // Not asserting equality because they may differ, but ensure no crash
    }

    // Stable sort with equal reaction times: deterministic output on repeated runs
    {
        std::vector<int> ids = {0, 1, 2};
        std::vector<bool> fem = {true, true, true};
        std::vector<int> yrs = {0, 0, 0};
        std::vector<double> rates = {1.0, 1.0, 1.0};
        std::vector<double> fe = {0.0, 0.0, 0.0};
        auto r1 = simulate_officer_pointing(ids, fem, yrs, rates, fe, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 77);
        auto r2 = simulate_officer_pointing(ids, fem, yrs, rates, fe, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 77);
        assert(r1 == r2);
    }

    return 0;
}
