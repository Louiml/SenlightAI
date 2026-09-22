Write a C++ function `subdomainVisits` that takes a vector of strings, where each string is a "count-paired domain" in the format `"count domain"` (e.g., `"9001 discuss.leetcode.com"`), and returns a vector of strings in the same format that aggregates the visit counts for every domain and all of its parent subdomains. For each input domain, its parent domains are obtained by successively removing the leftmost label (e.g., `"discuss.leetcode.com"` yields parents `"leetcode.com"` and `"com"`). The output should contain one entry per unique domain across all inputs, with the total count summed from all sources. The order of the output vector does not matter. You may assume all counts are non-negative integers, domain labels consist of lowercase letters and dots, and the input vector is non-empty. The function should be robust to duplicate input domains and domains that share subdomains.
The solution uses a hash map (or `std::map`) to accumulate visit counts. For each input string, split it into the count and the full domain (e.g., using a string stream). Then, generate the full domain and all its parent subdomains by iterating from the end of the domain string, finding each dot, and extracting the substring after that dot. For each generated subdomain (including the full domain), add the count to the map (defaulting to 0 if not present). After processing all inputs, iterate over the map and format each entry as `"total_count domain"`. Edge cases include: a domain with no dots (e.g., `"com"` yields only itself), duplicate inputs that should be summed, and counts that may be large (use `long long` to be safe). Time complexity is \(O(N \cdot L)\), where \(N\) is the number of input strings and \(L\) is the maximum domain length (since each domain generates at most \(O(L)\) subdomains and each substring extraction is linear). Space complexity is \(O(M \cdot L)\) for the map, where \(M\) is the total number of unique subdomains.
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

// Given a list of "count domain" strings, return aggregated counts for all subdomains.
std::vector<std::string> subdomainVisits(const std::vector<std::string>& domains) {
    std::unordered_map<std::string, long long> counts;

    for (const auto& entry : domains) {
        std::istringstream iss(entry);
        long long count;
        std::string domain;
        iss >> count >> domain;

        // Start with the full domain, then add each parent subdomain.
        std::string current = domain;
        while (true) {
            counts[current] += count;
            size_t dot = current.find('.');
            if (dot == std::string::npos) {
                break;
            }
            current = current.substr(dot + 1);
        }
    }

    std::vector<std::string> result;
    result.reserve(counts.size());
    for (const auto& [domain, count] : counts) {
        result.push_back(std::to_string(count) + " " + domain);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Example 1
    std::vector<std::string> input1 = {"9001 discuss.leetcode.com"};
    auto out1 = subdomainVisits(input1);
    std::vector<std::string> expected1 = {"9001 discuss.leetcode.com", "9001 leetcode.com", "9001 com"};
    std::sort(out1.begin(), out1.end());
    std::sort(expected1.begin(), expected1.end());
    assert(out1 == expected1);

    // Example 2
    std::vector<std::string> input2 = {"900 google.mail.com", "50 yahoo.com", "1 intel.mail.com", "5 wiki.org"};
    auto out2 = subdomainVisits(input2);
    std::vector<std::string> expected2 = {"901 mail.com", "50 yahoo.com", "900 google.mail.com", "5 wiki.org",
                                          "5 org", "1 intel.mail.com", "951 com"};
    std::sort(out2.begin(), out2.end());
    std::sort(expected2.begin(), expected2.end());
    assert(out2 == expected2);

    // Single domain without dots
    std::vector<std::string> input3 = {"42 com"};
    auto out3 = subdomainVisits(input3);
    std::sort(out3.begin(), out3.end());
    std::vector<std::string> expected3 = {"42 com"};
    assert(out3 == expected3);

    // Duplicate domains summing
    std::vector<std::string> input4 = {"10 a.com", "20 a.com"};
    auto out4 = subdomainVisits(input4);
    std::sort(out4.begin(), out4.end());
    std::vector<std::string> expected4 = {"30 a.com", "30 com"};
    assert(out4 == expected4);

    // Large counts to ensure long long handling
    std::vector<std::string> input5 = {"1000000000 big.com"};
    auto out5 = subdomainVisits(input5);
    std::sort(out5.begin(), out5.end());
    std::vector<std::string> expected5 = {"1000000000 big.com", "1000000000 com"};
    assert(out5 == expected5);

    // Multiple levels
    std::vector<std::string> input6 = {"1 a.b.c"};
    auto out6 = subdomainVisits(input6);
    std::sort(out6.begin(), out6.end());
    std::vector<std::string> expected6 = {"1 a.b.c", "1 b.c", "1 c"};
    assert(out6 == expected6);

    return 0;
}
