Write a C++ function `selectBestProposal` that takes an integer `n` (number of requirements), an integer `p` (number of proposals), and a vector of strings `proposalData` where the first `n` entries are requirement descriptions (to be ignored), followed by groups of lines for each proposal: the proposal name, then a line with the compliance count and price (space-separated, price as a double), then exactly that many requirement descriptions (to be ignored). The function must return the name of the best proposal according to the rules: maximize the number of requirements met; if ties, choose the one with the lower price; if still ties, choose the one appearing earliest in the input. The input is guaranteed to have at least one proposal and valid formatting; the function should return a `std::string` containing the winning proposal's name.

#include <cassert>
#include <string>
#include <vector>

// Function under test (included above)
std::string selectBestProposal(int n, int p, const std::vector<std::string>& proposalData);

int main() {
    // Case 1: Simple case, one proposal
    {
        std::vector<std::string> data = {"req1", "Proposal A", "3 100.0", "r1", "r2", "r3"};
        assert(selectBestProposal(1, 1, data) == "Proposal A");
    }

    // Case 2: Tie on requirements, lower price wins
    {
        std::vector<std::string> data = {
            "req1", "req2",
            "Proposal A", "2 50.0", "a1", "a2",
            "Proposal B", "2 40.0", "b1", "b2"
        };
        assert(selectBestProposal(2, 2, data) == "Proposal B");
    }

    // Case 3: Higher requirements wins regardless of price
    {
        std::vector<std::string> data = {
            "req1",
            "Cheap", "1 1.0", "c1",
            "Expensive", "2 999.0", "e1", "e2"
        };
        assert(selectBestProposal(1, 2, data) == "Expensive");
    }

    // Case 4: Tie on requirements and price, earlier wins
    {
        std::vector<std::string> data = {
            "req1",
            "First", "1 10.0", "f1",
            "Second", "1 10.0", "s1"
        };
        assert(selectBestProposal(1, 2, data) == "First");
    }

    // Case 5: Names with spaces, compliance 0
    {
        std::vector<std::string> data = {
            "req1", "req2", "req3",
            "Best Proposal", "0 20.0",
            "Other Proposal", "1 15.0", "x"
        };
        assert(selectBestProposal(3, 2, data) == "Other Proposal");
    }

    // Case 6: Mix of requirements and prices
    {
        std::vector<std::string> data = {
            "req",
            "A", "2 100.0", "a1", "a2",
            "B", "2 100.0", "b1", "b2",
            "C", "3 1.0", "c1", "c2", "c3"
        };
        assert(selectBestProposal(1, 3, data) == "C");
    }

    return 0;
}

#include <string>
#include <vector>
#include <sstream>

// Select the best proposal from parsed data.
// proposalData layout: n requirement lines (ignored), then for each proposal:
//   - name line
//   - line with "compliance_count price" (price is double)
//   - compliance_count requirement lines (ignored)
std::string selectBestProposal(int n, int p, const std::vector<std::string>& proposalData) {
    size_t idx = 0;
    // Skip the n initial requirement lines
    for (int i = 0; i < n; ++i) {
        ++idx;
    }

    std::string bestName;
    int bestReqs = -1;
    double bestPrice = 0.0;

    for (int prop = 0; prop < p; ++prop) {
        // Proposal name (may contain spaces)
        std::string name = proposalData[idx++];

        // Compliance count and price
        std::istringstream iss(proposalData[idx++]);
        int reqs;
        double price;
        iss >> reqs >> price;

        // Skip the requirement lines for this proposal
        for (int j = 0; j < reqs; ++j) {
            ++idx;
        }

        // Update best based on rules
        if (reqs > bestReqs) {
            bestName = name;
            bestReqs = reqs;
            bestPrice = price;
        } else if (reqs == bestReqs && price < bestPrice) {
            bestName = name;
            bestReqs = reqs;
            bestPrice = price;
        }
        // If reqs == bestReqs and price >= bestPrice, keep earlier best (no update)
    }

    return bestName;
}

// The problem is a classic selection with a custom comparator. We iterate through the proposal groups in order. For each proposal, parse its name, price, and compliance count. We maintain the best proposal found so far: if the current proposal meets more requirements than the current best, it becomes the new best (since it's strictly better regardless of price). If it meets the same number of requirements, it replaces the best only if its price is strictly lower. If price is equal, we keep the earlier one (since we skip updates when equal). Edge cases: proposal names may contain spaces; we read the name as the entire line before the price/count line. The price may be a double, so compare with a tolerance if needed, but since we only check strict less, a simple `<` is fine. The `n` requirement lines are skipped before the loop. Time complexity is O(total number of lines) = O(n + sum of compliance counts + p) because we read every line once. Space complexity is O(1) auxiliary besides the input vector itself, since we process iteratively.
