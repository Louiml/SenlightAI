/*
Write a standalone C++ function that simulates a simplified version of a minimum clearance valid state sampler. The function should take a vector of `double` values representing distances from sampled states to the nearest obstacle, a `double` minimum clearance threshold, and a number of sampling attempts (maximum retries). The function should return a `bool` indicating whether at least one sample met the clearance requirement, and also output the index of the first sample that satisfied the requirement (or `-1` if none). The function should model the iterative rejection logic from the given OMPL code: each sample is valid only if its distance is at least the clearance threshold; sampling retries until a valid sample is found or the maximum attempts are exhausted. The function must handle empty input, negative distances (treat as invalid), and a clearance threshold that may be zero or negative. Use `const` correctly and provide clear comments.
*/

#include <vector>
#include <cstddef>

/**
 * Simulates a minimum clearance valid state sampler.
 * 
 * @param distances A vector of distances from sampled states to the nearest obstacle.
 * @param minClearance The minimum required clearance (threshold). A sample is valid if its distance >= minClearance.
 * @param maxAttempts The maximum number of samples to try (retry limit). If negative or zero, no sampling occurs.
 * @param foundIndex (output) Set to the index of the first valid sample, or -1 if none found.
 * @return True if a valid sample was found within maxAttempts, false otherwise.
 */
bool sampleWithMinimumClearance(const std::vector<double>& distances,
                                double minClearance,
                                int maxAttempts,
                                int& foundIndex) {
    foundIndex = -1;

    // No attempts allowed or no distances to sample from
    if (maxAttempts <= 0 || distances.empty()) {
        return false;
    }

    // Limit attempts to the number of available distances
    int attempts = (maxAttempts < static_cast<int>(distances.size())) ? maxAttempts : static_cast<int>(distances.size());

    for (int i = 0; i < attempts; ++i) {
        double dist = distances[i];
        // Reject negative distances (invalid physical state)
        if (dist < 0.0) {
            continue;
        }
        // Check clearance: valid only if distance >= minClearance
        if (dist >= minClearance) {
            foundIndex = i;
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic case with valid sample found
    std::vector<double> d1 = {0.5, 1.2, 0.8, 2.0};
    int idx1;
    assert(sampleWithMinimumClearance(d1, 1.0, 10, idx1) == true);
    assert(idx1 == 1); // first distance >= 1.0 is 1.2 at index 1

    // Test 2: No sample meets clearance, returns false
    std::vector<double> d2 = {0.1, 0.5, 0.9};
    int idx2;
    assert(sampleWithMinimumClearance(d2, 1.0, 5, idx2) == false);
    assert(idx2 == -1);

    // Test 3: Negative distances are invalid even if threshold is low
    std::vector<double> d3 = {-0.5, 1.0, 0.2};
    int idx3;
    assert(sampleWithMinimumClearance(d3, 0.0, 3, idx3) == true);
    assert(idx3 == 1); // index 0 is negative, index 1 is 1.0 >= 0.0

    // Test 4: maxAttempts limits the search
    std::vector<double> d4 = {0.2, 2.0, 3.0};
    int idx4;
    assert(sampleWithMinimumClearance(d4, 1.0, 2, idx4) == true);
    assert(idx4 == 1); // only first 2 attempts, index 1 is first valid

    // Test 5: Zero attempts returns false
    std::vector<double> d5 = {5.0};
    int idx5;
    assert(sampleWithMinimumClearance(d5, 1.0, 0, idx5) == false);
    assert(idx5 == -1);

    // Test 6: Empty vector returns false
    std::vector<double> d6;
    int idx6;
    assert(sampleWithMinimumClearance(d6, 1.0, 5, idx6) == false);
    assert(idx6 == -1);

    // Test 7: Negative clearance threshold accepts all non-negative distances
    std::vector<double> d7 = {0.0, 0.2};
    int idx7;
    assert(sampleWithMinimumClearance(d7, -1.0, 2, idx7) == true);
    assert(idx7 == 0); // 0.0 >= -1.0 is true

    // Test 8: All distances below threshold
    std::vector<double> d8 = {0.5, 0.7, 0.9};
    int idx8;
    assert(sampleWithMinimumClearance(d8, 1.0, 3, idx8) == false);
    assert(idx8 == -1);

    return 0;
}

// The solution iterates through the provided distance values one by one, exactly like the `sample` function in OMPL does with the `do-while` loop. For each distance value, we first check if it is non-negative (since a negative distance makes no physical sense and should be considered invalid). Then we apply the validity check: the sample is valid only if the distance is at least the clearance threshold. If valid, we immediately return `true` and record the index. If not, we continue to the next attempt until we exhaust the allowed number of attempts (which is the size of the vector or the provided max attempts, whichever is smaller). Edge cases: if the input vector is empty, no samples exist, so return `false` with index `-1`. If the clearance threshold is negative, every non-negative distance would pass the threshold check (since `dist >= negative` is true), but we still reject negative distances. The algorithm runs in O(n) time (where n is the length of the distance vector or max attempts, whichever is smaller) and uses O(1) auxiliary space. We should be careful not to exceed vector bounds when attempts limit is larger than vector size; we cap attempts to the vector size. Also, we should record the index only when a valid sample is found; otherwise return `-1`.
