Write a C++ function `void buildConsensus(int samples, int haplotypes, int markers, const std::vector<std::string>& inputHaplotypes, std::vector<std::string>& consensusHaplotypes)` that takes a set of sampled haplotypes (each sample is a set of `haplotypes` binary strings of length `markers`, where `'0'` and `'1'` represent alleles), and builds a consensus haplotype pair for each consecutive pair of haplotypes (indices `0-1`, `2-3`, etc.). For each pair, the function must determine the correct phase (ordering) for each sample relative to the consensus, then at each marker position, count the four possible genotype combinations (allele1, allele2) across samples, select the most frequent genotype, assign it to the consensus pair, and update phase for samples that disagree with the chosen heterozygous genotype. The final consensus strings must be stored in `consensusHaplotypes` (size `haplotypes`, each string of length `markers`). The function should handle up to 10 samples, 4 haplotypes, and 100 markers. Assume the number of haplotypes is even. If `samples == 0`, leave consensus strings empty (all `'0'`). Time and memory should be efficient for the given bounds.

// The solution processes each pair of haplotypes independently. For each pair (h, h+1), we first determine an initial phase for each sample by scanning markers left-to-right and finding the first heterozygous position (where the two haplotypes for that sample differ). At that position, if the first haplotype has allele `'1'`, phase = 0; otherwise phase = 1. If no heterozygous position exists, phase remains 0. Then we iterate over each marker position. For each marker, we count how many samples have each of the four possible ordered genotype pairs (00, 01, 10, 11) based on the current phase. We select the genotype with the highest count (ties broken by smallest index). The consensus alleles for this marker are assigned as `best/2` and `best%2`. We then update an error counter with the number of samples that do not match the selected genotype (for homozygous selected genotypes, count all non-matching; for heterozygous, count homozygotes). Additionally, if the selected genotype is heterozygous, we check if there are samples with the opposite heterozygous genotype (best XOR 3). For each such sample, we flip its phase at this marker and all subsequent markers (since flipping phase changes the interpretation of all later heterozygous positions), and increment a flip counter. Finally, we divide both counters by the number of samples to get average quality scores (though these are not returned, they illustrate correctness). The algorithm runs in O(samples * markers * haplotypes) time and uses O(samples) extra space for phase tracking. Edge cases: no samples (output all '0'), monomorphic positions (all same allele), and ties in counts (choose lowest index).

#include <vector>
#include <string>
#include <algorithm>

// Build consensus haplotypes from sampled haplotypes.
// Each input sample is a vector of strings (haplotypes), each string length markers.
// Output consensusHaplotypes has same dimensions, filled with '0'/'1'.
void buildConsensus(int samples, int haplotypes, int markers,
                    const std::vector<std::vector<std::string>>& inputHaplotypes,
                    std::vector<std::string>& consensusHaplotypes) {
    // Initialize output with all '0' strings
    consensusHaplotypes.assign(haplotypes, std::string(markers, '0'));
    if (samples == 0) return;

    // Process each pair of haplotypes
    for (int h = 0; h < haplotypes; h += 2) {
        // Phase for each sample: 0 means read as (h, h+1), 1 means swapped
        std::vector<int> phase(samples, 0);

        // Determine initial phase by first heterozygous position
        for (int i = 0; i < samples; i++) {
            for (int j = 0; j < markers; j++) {
                if (inputHaplotypes[i][h][j] != inputHaplotypes[i][h + 1][j]) {
                    phase[i] = (inputHaplotypes[i][h][j] == '1') ? 0 : 1;
                    break;
                }
            }
        }

        // Build consensus position by position
        for (int j = 0; j < markers; j++) {
            int counts[4] = {0, 0, 0, 0}; // 00, 01, 10, 11

            // Count genotypes with current phases
            for (int i = 0; i < samples; i++) {
                int a1 = inputHaplotypes[i][h + phase[i]][j] - '0';
                int a2 = inputHaplotypes[i][h + (phase[i] ^ 1)][j] - '0';
                counts[a1 * 2 + a2]++;
            }

            // Select most common genotype (ties -> smaller index)
            int best = 0;
            for (int g = 1; g < 4; g++) {
                if (counts[g] > counts[best]) best = g;
            }

            // Assign consensus
            consensusHaplotypes[h][j] = (best / 2) ? '1' : '0';
            consensusHaplotypes[h + 1][j] = (best % 2) ? '1' : '0';

            // If heterozygous selected, flip phases for samples with opposite heterozygote
            if (best != 0 && best != 3) {
                int opposite = best ^ 3;
                if (counts[opposite] > 0) {
                    for (int i = 0; i < samples; i++) {
                        int a1 = inputHaplotypes[i][h + phase[i]][j] - '0';
                        int a2 = inputHaplotypes[i][h + (phase[i] ^ 1)][j] - '0';
                        if (a1 * 2 + a2 == opposite) {
                            phase[i] ^= 1;
                        }
                    }
                }
            }
        }
    }
}

#include <cassert>
#include <vector>
#include <string>

// Assume buildConsensus is declared above

int main() {
    // Test 1: Simple case, two samples, two haplotypes, three markers
    {
        std::vector<std::vector<std::string>> input = {
            {"010", "101"},  // sample 0
            {"010", "101"}   // sample 1
        };
        std::vector<std::string> consensus;
        buildConsensus(2, 2, 3, input, consensus);
        // Both samples agree: consensus is 010/101
        assert(consensus[0] == "010");
        assert(consensus[1] == "101");
    }

    // Test 2: Phase disagreement, needs flipping
    {
        std::vector<std::vector<std::string>> input = {
            {"010", "101"},  // sample 0: initial phase 0
            {"101", "010"}   // sample 1: initial phase 1 (since first diff at 0: h=1)
        };
        // At marker 1 both are heterozygous: sample0 has 10, sample1 has 01 (after phase flip)
        // After phase flip for sample1, both become 10, consensus = 10 at marker 1
        std::vector<std::string> consensus;
        buildConsensus(2, 2, 3, input, consensus);
        // Marker 0: both have 0/1 -> consensus 0/1 (best=1)
        // Marker 1: after flips, both have 1/0 -> consensus 1/0 (best=2)
        // Marker 2: both have 0/1 -> consensus 0/1
        assert(consensus[0] == "010");
        assert(consensus[1] == "101");
    }

    // Test 3: Zero samples
    {
        std::vector<std::vector<std::string>> input;
        std::vector<std::string> consensus;
        buildConsensus(0, 2, 4, input, consensus);
        assert(consensus.size() == 2);
        assert(consensus[0] == "0000");
        assert(consensus[1] == "0000");
    }

    // Test 4: Ties in genotype counts, choose lower index
    {
        std::vector<std::vector<std::string>> input = {
            {"00", "11"},  // sample 0: hetero 01
            {"00", "11"}   // sample 1: hetero 01
        };
        // At marker 0: counts: 00=0, 01=2, 10=0, 11=0 -> best=1 -> consensus 0/1
        // At marker 1: same -> consensus 0/1
        std::vector<std::string> consensus;
        buildConsensus(2, 2, 2, input, consensus);
        assert(consensus[0] == "00");
        assert(consensus[1] == "11");
    }

    // Test 5: Majority homozygous
    {
        std::vector<std::vector<std::string>> input = {
            {"0", "0"},  // sample 0: 00
            {"1", "1"},  // sample 1: 11
            {"0", "1"}   // sample 2: 01
        };
        // Counts: 00=1, 01=1, 11=1 -> tie among all, choose 0 (00)
        std::vector<std::string> consensus;
        buildConsensus(3, 2, 1, input, consensus);
        assert(consensus[0] == "0");
        assert(consensus[1] == "0");
    }

    // Test 6: Four haplotypes, two pairs
    {
        std::vector<std::vector<std::string>> input = {
            {"000", "111", "010", "101"},
            {"000", "111", "010", "101"}
        };
        std::vector<std::string> consensus;
        buildConsensus(2, 4, 3, input, consensus);
        assert(consensus[0] == "000");
        assert(consensus[1] == "111");
        assert(consensus[2] == "010");
        assert(consensus[3] == "101");
    }

    return 0;
}
