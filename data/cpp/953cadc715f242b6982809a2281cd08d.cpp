// Write a C++ function `size_t count_envelope_configurations(const std::string& formula, size_t molecules, double precision)` that, given a chemical formula string such as `"C10000H1000O1000N1000"`, a number of molecules, and a probability precision threshold, returns the number of isotopic configurations that collectively account for at least the specified fraction of the total probability mass when considering the stochastic isotope distribution. The function must simulate the enumeration of isotopic configurations in descending order of probability, accumulate their probabilities, and stop once the cumulative probability reaches or exceeds the given precision threshold (which must be between 0 and 1). The chemical formula parser must handle elements with one or two letters (C, H, O, N, S, etc.) and an optional integer count (default 1 if absent). You may assume only common elements with atomic masses and natural isotopic abundances are used, and that the input formula is well-formed. The function should return the number of configurations visited (i.e., the number of distinct isotopic compositions whose probabilities were summed) before and including the first configuration that causes the cumulative probability to exceed the threshold. The function must be deterministic and use a priority queue (max-heap) to always expand the most probable configuration first. For simplicity, you may assume that each element has exactly two isotopes: the most abundant one and one minor isotope with a fixed abundance, and that the mass of each isotope is an integer mass (e.g., C: 12 and 13, H: 1 and 2, O: 16 and 18, N: 14 and 15). You may also assume that the maximum number of atoms per element is small enough that the total number of configurations is huge (so you cannot enumerate all), and that the precision is high enough that the algorithm will terminate in reasonable time. The function should use double precision for probabilities. Provide a complete, self-contained implementation with necessary headers and a descriptively named free function. Do not include a `main` function in the solution section.
The approach is similar to generating the most probable isotopic configurations for a molecule. We treat each element independently: for an element with `n` atoms and two isotopes (mass `m0` with abundance `p0`, mass `m1` with abundance `p1`), the possible counts of the heavier isotope range from 0 to `n`. The probability of having `k` heavier atoms is given by the binomial distribution: `C(n,k) * p0^(n-k) * p1^k`. The mass offset from the all-light configuration is `k * (m1 - m0)`. For a molecule with multiple elements, each configuration is a tuple of counts `(k1, k2, ..., ke)` for each element, with total probability equal to the product of the individual element probabilities, and total mass equal to the sum of base masses plus the sum of mass offsets. We need to enumerate configurations in descending order of total probability until the cumulative probability reaches the threshold.

Since the product of probabilities is monotonic in each `ki` (increasing `ki` decreases the probability if `p1 < p0`, which is true), we can use a max-heap initialized with the all-zero configuration (all light isotopes). Each configuration is represented by a vector of counts. When we pop a configuration, we add its probability to the cumulative sum and increment the count. Then we generate "neighbors" by incrementing exactly one element's count by 1, provided the count is less than that element's atom count, and that we haven't visited this neighbor before. To avoid duplicates, we can enforce a rule: when incrementing element `i`, we only allow it if all previous elements `j < i` have counts equal to 0? That is not correct because configurations can have multiple non-zero counts. A better rule: when incrementing element `i`, we only push it if the current configuration has zeros for all elements with index greater than `i`? That would restrict the search space incorrectly. The standard approach for enumerating multivariate distributions in descending probability is to use a priority queue with a "frontier" and a visited set, but that can blow up. Instead, we can use a recursive generation based on the fact that probabilities are multiplicative and each factor is monotonically decreasing. One known method is to treat each element as a "digit" in a mixed-radix system and use a min-heap of "next increment" events, but with only two isotopes per element, the number of configurations that meet the threshold is usually manageable for typical formulas (e.g., up to a few thousand). We can use a simple BFS with a max-heap and a set of visited states encoded as a tuple of counts. Since the total number of visited configurations is bounded by the threshold (likely small, e.g., < 10000 for precision 0.999), this is acceptable. For each popped configuration, we generate all possible single increments (for each element where the count is less than max), compute the new probability, and if not visited, push into the heap. This is O(V * E * log V) where V is number of visited configurations and E is number of elements, which is fine.

Edge cases: If the formula has a single element and one atom, there are only two configurations, and the threshold may require both. The function must handle formulas with counts that are large but the number of configurations visited is small due to rapidly decreasing probabilities. The precision must be between 0 and 1 (exclusive? we can allow 1.0, but it might require all configurations; we'll clamp). The function should return at least 1. The implementation must parse the formula correctly: after splitting into elements, for each element, look up atomic mass and abundances from a hardcoded table. For the test, we can use simple cases: "H" with 1 atom, precision 0.9999 will need to visit maybe 2 configurations depending on abundance. Since we control abundances, we can choose values to make tests simple. For example, use element with p0=0.5, p1=0.5, then for one atom, both configurations have equal probability 0.5, so to reach 0.999, we need to visit both (since 0.5 < 0.999). For two atoms of the same element, probabilities are 0.25 (k=0), 0.5 (k=1), 0.25 (k=2). To reach 0.999, visit all three. The function must return the number of configurations whose cumulative probability first exceeds the threshold.

Time complexity: O(V * E log V), space O(V). V depends on the threshold and the abundances; for typical use, V is small (thousands). We'll include a hardcoded table for C, H, O, N with two isotopes each (e.g., C: 12 (0.989), 13 (0.011); H: 1 (0.99985), 2 (0.00015); O: 16 (0.9976), 18 (0.0020); N: 14 (0.9963), 15 (0.0037)). The mass numbers are integer. For simplicity, we can use approximate abundances and masses.
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <sstream>
#include <cctype>
#include <functional>
#include <algorithm>

// Helper struct to hold element info
struct ElementInfo {
    int atoms;
    double mass0;
    double mass1;
    double prob0;
    double prob1;
};

// Parse a chemical formula like "C10000H1000O1000N1000" into a vector of ElementInfo
static std::vector<ElementInfo> parseFormula(const std::string& formula) {
    std::vector<ElementInfo> result;
    size_t i = 0;
    while (i < formula.size()) {
        // Read element symbol (1 or 2 letters)
        std::string symbol;
        symbol += formula[i++];
        if (i < formula.size() && islower(formula[i])) {
            symbol += formula[i++];
        }
        // Read count (digits)
        int count = 0;
        while (i < formula.size() && isdigit(formula[i])) {
            count = count * 10 + (formula[i] - '0');
            i++;
        }
        if (count == 0) count = 1;

        // Hardcoded data for C,H,O,N only; extend if needed
        double mass0, mass1, prob0, prob1;
        if (symbol == "C") {
            mass0 = 12.0; mass1 = 13.0; prob0 = 0.989; prob1 = 0.011;
        } else if (symbol == "H") {
            mass0 = 1.0; mass1 = 2.0; prob0 = 0.99985; prob1 = 0.00015;
        } else if (symbol == "O") {
            mass0 = 16.0; mass1 = 18.0; prob0 = 0.9976; prob1 = 0.0020;
        } else if (symbol == "N") {
            mass0 = 14.0; mass1 = 15.0; prob0 = 0.9963; prob1 = 0.0037;
        } else {
            // Unknown element: treat as having no heavy isotope (all light)
            mass0 = 1.0; mass1 = mass0; prob0 = 1.0; prob1 = 0.0;
        }

        result.push_back({count, mass0, mass1, prob0, prob1});
    }
    return result;
}

// Count the number of configurations needed to reach a given cumulative probability
size_t count_envelope_configurations(const std::string& formula, size_t molecules, double precision) {
    // Note: molecules parameter is unused in this simplified version, but kept for signature compatibility
    // In a more complete version, it might represent the number of molecules; here we ignore it.
    (void)molecules;

    // Clamp precision to (0, 1]
    if (precision <= 0.0) precision = 1e-9;
    if (precision > 1.0) precision = 1.0;

    auto elements = parseFormula(formula);
    size_t numElements = elements.size();
    if (numElements == 0) return 0;

    // Precompute binomial probabilities for each element
    // For element with n atoms, we have n+1 possible counts of heavy isotope (0..n)
    // Precompute probability for each count
    std::vector<std::vector<double>> probTable(numElements);
    std::vector<int> maxCounts(numElements);
    for (size_t e = 0; e < numElements; ++e) {
        int n = elements[e].atoms;
        maxCounts[e] = n;
        probTable[e].resize(n+1);
        // Compute binomial coefficients and probabilities
        // Use dynamic programming to avoid overflows: C(n,k) = C(n,k-1) * (n-k+1)/k
        std::vector<double> binom(n+1, 0.0);
        binom[0] = 1.0;
        for (int k = 1; k <= n; ++k) {
            binom[k] = binom[k-1] * (n - k + 1) / k;
        }
        for (int k = 0; k <= n; ++k) {
            probTable[e][k] = binom[k] * std::pow(elements[e].prob0, n - k) * std::pow(elements[e].prob1, k);
        }
    }

    // A configuration is represented by a vector<int> of counts of heavy isotopes for each element
    // Probability is the product of probTable[e][count[e]]
    // We need to enumerate in descending probability order.

    // Use a max-heap with a custom comparator. We'll store (probability, vector<int>)
    using Config = std::pair<double, std::vector<int>>;
    auto cmp = [](const Config& a, const Config& b) { return a.first < b.first; }; // max-heap
    std::priority_queue<Config, std::vector<Config>, decltype(cmp)> pq(cmp);

    // Start with all-zero counts
    std::vector<int> initial(numElements, 0);
    double initialProb = 1.0;
    for (size_t e = 0; e < numElements; ++e) {
        initialProb *= probTable[e][0];
    }
    pq.push({initialProb, initial});

    // Set to avoid duplicates
    struct VectorHash {
        size_t operator()(const std::vector<int>& v) const {
            size_t h = 0;
            for (int x : v) {
                h = h * 31 + std::hash<int>()(x);
            }
            return h;
        }
    };
    std::unordered_set<std::vector<int>, VectorHash> visited;
    visited.insert(initial);

    size_t count = 0;
    double cumulative = 0.0;

    while (!pq.empty()) {
        Config current = pq.top();
        pq.pop();
        double prob = current.first;
        std::vector<int> conf = std::move(current.second);

        cumulative += prob;
        count++;
        if (cumulative >= precision) {
            break;
        }

        // Generate neighbors: increment one element's heavy count by 1
        for (size_t e = 0; e < numElements; ++e) {
            if (conf[e] < maxCounts[e]) {
                std::vector<int> next = conf;
                next[e] += 1;
                // Compute probability of next
                double nextProb = prob;
                // Remove old factor and apply new factor
                nextProb /= probTable[e][conf[e]];
                nextProb *= probTable[e][next[e]];
                if (visited.find(next) == visited.end()) {
                    visited.insert(next);
                    pq.push({nextProb, std::move(next)});
                }
            }
        }
    }

    return count;
}
#include <cassert>
#include <string>

// Declaration of the function under test (assumed to be in the solution)
size_t count_envelope_configurations(const std::string& formula, size_t molecules, double precision);

int main() {
    // Test 1: Single carbon atom, p1=0.011, probability of light=0.989
    // To reach 0.999, we need both configurations because 0.989 < 0.999
    assert(count_envelope_configurations("C", 1, 0.999) == 2);

    // Test 2: Single hydrogen atom, p1=0.00015, probability of light=0.99985
    // To reach 0.999, only the light configuration is enough because 0.99985 > 0.999
    assert(count_envelope_configurations("H", 1, 0.999) == 1);

    // Test 3: Two hydrogen atoms, p1=0.00015, probabilities: k=0: 0.99970, k=1: 0.000299, k=2: 0.0000000225
    // To reach 0.999, only k=0 is enough (0.99970 > 0.999)
    assert(count_envelope_configurations("H2", 1, 0.999) == 1);

    // Test 4: Two carbon atoms, p1=0.011, probabilities: k=0: 0.9781, k=1: 0.02156, k=2: 0.000121
    // To reach 0.999, need k=0 and k=1 because 0.9781 < 0.999, and 0.9781+0.02156=0.99966 >= 0.999
    assert(count_envelope_configurations("C2", 1, 0.999) == 2);

    // Test 5: One nitrogen, p1=0.0037, probability of light=0.9963 > 0.999? Actually 0.9963 < 0.999, so need both
    assert(count_envelope_configurations("N", 1, 0.999) == 2);

    // Test 6: One oxygen, p1=0.002, probability of light=0.9976 < 0.999, need both
    assert(count_envelope_configurations("O", 1, 0.999) == 2);

    // Test 7: A more complex formula: "CO2" with 1 carbon and 2 oxygens
    // Compute expected: C light prob 0.989, O2: probabilities: k=0: 0.9952, k=1: 0.00398, k=2: 4e-6
    // For threshold 0.999, need C light + O2 k=0: 0.989*0.9952=0.9848 <0.999, so add C heavy + O2 k=0: 0.011*0.9952=0.01095, cumulative=0.99575 <0.999
    // Next: C light + O2 k=1: 0.989*0.00398=0.00394, cumulative=0.99969 >=0.999, so visited 3 configs.
    // Let's test that.
    // We don't have exact expected from code, but we can compute manually: 
    // Config0: C0 O0: prob=0.989*0.9952=0.9848
    // Config1: C1 O0: prob=0.011*0.9952=0.01095
    // Cumulative after Config1: 0.99575 <0.999
    // Config2: C0 O1: prob=0.989*0.00398=0.00394
    // Cumulative after Config2: 0.99969 >=0.999 => count=3
    assert(count_envelope_configurations("CO2", 1, 0.999) == 3);

    // Test 8: Precision 1.0 with a single atom should return all configurations (2 for C)
    assert(count_envelope_configurations("C", 1, 1.0) == 2);

    // Test 9: Very low precision, only light configuration needed
    assert(count_envelope_configurations("C", 1, 0.1) == 1);

    // Test 10: Empty formula? That's invalid; we don't test that.

    return 0;
}
