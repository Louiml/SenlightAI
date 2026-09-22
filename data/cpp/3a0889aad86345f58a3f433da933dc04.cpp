Write a standalone C++ function that analyzes a hash table’s bucket size distribution and returns a formatted report string. The function should accept the number of buckets, a vector of bucket sizes (where each element is the number of items in that bucket), and the total number of items (which may be computed from the vector). It must compute and report: total items, number of buckets, number of non-empty buckets, maximum bucket size, expected search time (1 + total_items / num_buckets), actual search time (1 + total_items / non_empty_buckets), and a distribution table of bucket sizes with actual counts and theoretical Poisson-based probabilities under uniform random hashing. The table should list sizes from 0 up to the largest bucket size, and continue listing theoretical probabilities while they exceed 0.1, even if no bucket has that size. The output must be a single string with lines separated by '\n' exactly as shown in the example format.
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test with 3 buckets, sizes [2,0,1]
    std::vector<int> buckets1 = {2,0,1};
    std::string r1 = hashTableAnalysis(buckets1, 3);
    // Check header lines
    assert(r1.find("table size: 3") != std::string::npos);
    assert(r1.find("number of buckets: 3") != std::string::npos);
    assert(r1.find("nonempty buckets: 2") != std::string::npos);
    assert(r1.find("max bucket size: 2") != std::string::npos);
    // Check table includes size 0,1,2
    assert(r1.find("0\t") != std::string::npos);
    assert(r1.find("1\t") != std::string::npos);
    assert(r1.find("2\t") != std::string::npos);
    // Actual counts: size0=1, size1=1, size2=1
    assert(r1.find("0\t1\t") != std::string::npos);
    assert(r1.find("1\t1\t") != std::string::npos);
    assert(r1.find("2\t1\t") != std::string::npos);
    // Test empty table
    std::vector<int> buckets2 = {0,0};
    std::string r2 = hashTableAnalysis(buckets2, 2);
    assert(r2.find("table size: 0") != std::string::npos);
    assert(r2.find("nonempty buckets: 0") != std::string::npos);
    assert(r2.find("max bucket size: 0") != std::string::npos);
    // Test with one large bucket
    std::vector<int> buckets3 = {5};
    std::string r3 = hashTableAnalysis(buckets3, 1);
    assert(r3.find("table size: 5") != std::string::npos);
    assert(r3.find("nonempty buckets: 1") != std::string::npos);
    assert(r3.find("max bucket size: 5") != std::string::npos);
    assert(r3.find("5\t1\t") != std::string::npos);
    return 0;
}
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <ostream>

// Compute the hash table analysis report as a string.
std::string hashTableAnalysis(const std::vector<int>& bucketSizes, size_t numBuckets) {
    size_t totalItems = 0;
    size_t nonEmpty = 0;
    size_t maxSize = 0;
    for (size_t s : bucketSizes) {
        totalItems += s;
        if (s > 0) nonEmpty++;
        if (s > maxSize) maxSize = s;
    }

    std::vector<int> actualCounts(maxSize + 1, 0);
    for (size_t s : bucketSizes) {
        if (s <= maxSize) actualCounts[s]++;
    }

    std::ostringstream os;
    os << "\ntable size: " << totalItems << "\nnumber of buckets: " << numBuckets
       << "\nnonempty buckets: " << nonEmpty << "\nmax bucket size: " << maxSize
       << "\nexpected search time: " << (float)(1 + (totalItems * 1.0) / (numBuckets * 1.0))
       << "\nactual search time: " << (float)(1 + (totalItems * 1.0) / (nonEmpty * 1.0)) << '\n';

    os << "\nbucket size distributions\n-------------------------\nsize \tactual \ttheory (uniform random distribution) \n----\t------\t------\n";

    double check = numBuckets * std::pow((numBuckets * 1.0 - 1) / (numBuckets * 1.0), totalItems);
    size_t i = 0;
    while (check > 0.1 || i < actualCounts.size()) {
        os << i << '\t';
        if (i < actualCounts.size()) {
            os << actualCounts[i] << '\t' << check << '\n';
        } else {
            os << "\t" << check << '\n';
        }
        i++;
        if (totalItems >= i) {
            check = ((totalItems - i + 1.0) / i) * (1.0 / (numBuckets - 1.0)) * check;
        } else {
            check = 0;
        }
    }
    return os.str();
}
// The solution must first determine the maximum bucket size by scanning the vector. Then compute the total number of items by summing the bucket sizes, and count non-empty buckets. Using the total items `N` and number of buckets `B`, the theoretical probability of a bucket having size `k` under uniform random placement is given by the Poisson approximation: `P(k) = ( (B-1)/B )^N * ( N! / (N-k)! ) / ( (B-1)^k * k! )`? Actually, the code uses a recurrence: start with `check = B * ((B-1)/B)^N` for size 0, then each next `check` is multiplied by `(N - k + 1)/k * 1/(B-1)`. This computes the expected number of buckets of size `k`. We must replicate this exact recurrence to match the snippet. Edge cases: if `N=0`, the initial `check = B` (since any number to power 0 is 1), but actual bucket sizes may all be zero; output should handle that. Also, if a bucket size exceeds the current `check` range, we extend the vector holding counts. For sizes beyond the largest observed bucket, we output only the theoretical value. The report string is built exactly as the snippet: header lines then a table with columns "size", "actual", and "theory". Ensure proper formatting with tabs and newlines. Time complexity is O(max_bucket_size + num_buckets) for scanning and O(max_bucket_size) for the table loop, so O(num_buckets + max_bucket). Space is O(max_bucket) for the counts vector.
