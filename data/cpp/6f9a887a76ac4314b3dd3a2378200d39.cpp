Write a C++ function `classifyUnknown` that takes a filename string, a positive integer `k`, and a character representing an unknown data point's type label (which can be ignored in computation), and performs a k-nearest neighbors classification on the data stored in a CSV file. The CSV file has three columns: an integer attribute `r`, a float attribute `a`, and a single-character class label `t` (which can be `'a'`, `'h'`, `'f'`, or `'?'`). Rows with class label `'?'` are considered unknown points to classify, while all others form the training set. For each unknown point, compute the normalized Euclidean squared distance to every training point (normalize each attribute by dividing the difference by the range of that attribute across the training set), sort distances ascending, take the `k` nearest neighbors, and determine the majority class among those neighbors (breaking ties by alphabetical order of class labels: `'a'` < `'f'` < `'h'`). The function should return a vector of strings, one per unknown point in the order they appear in the file, where each string is formatted as `"r=<r>, a=<a>, classified as <majority_class>"` (replace `<r>` with the integer, `<a>` with the float printed with 2 decimal places, and `<majority_class>` with the winning character). If there are no unknown points, return an empty vector. If the training set is empty (no non-`'?'` rows), return an empty vector as well. The function must be robust to lines with leading/trailing whitespace and ensure that attribute ranges are computed only from training data; if a range is zero, treat normalization as zero division by setting that normalized difference to 0.0.
The solution reads the entire CSV file line by line, skipping the header if present (assume first line may be a header containing non-numeric data; but the task specifies the file format, so we can assume all data lines are valid CSV with three fields). For each row, we parse the integer, float, and character. If the character is `'?'`, we store it in a vector of unknown points; otherwise, we store it in a training vector and also track the min and max of both `r` and `a` across training points. After reading all rows, we compute the ranges as `max - min` for each attribute. If a range is zero, we set the normalized difference to 0.0 for that attribute (to avoid division by zero). For each unknown point, we compute distances to every training point as `(delta_a / a_range)^2 + (delta_r / r_range)^2`. We store pairs of `(distance, class_label)` in a vector, sort by distance ascending (if distances are equal, the order between different classes is irrelevant because we only count frequencies among the top k; but to be deterministic, we can break ties by class label using a custom comparator). After sorting, we take the first `k` entries and count occurrences of each class label. We then choose the class with the highest count; if there is a tie, we choose the alphabetically smallest label (`'a'` < `'f'` < `'h'`). We format the output string using `std::ostringstream` with `std::fixed` and `std::setprecision(2)` for the float. Time complexity is O( (U + T) log T? ) actually: for each unknown, we compute T distances and sort them, so O(U * T log T). Space complexity is O(T + U) for storage plus O(T) for the distance list per unknown. Edge cases include: no training data (return empty vector), no unknown data (return empty vector), k larger than training set size (then take all training points; the implementation should use `std::min(k, training_set.size())` for the number of neighbors considered), and zero ranges (handled as described).
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>

struct Node {
    int r;
    float a;
    char t;
};

struct DistancePair {
    float dist;
    char label;
    bool operator<(const DistancePair& other) const {
        if (dist != other.dist) return dist < other.dist;
        return label < other.label; // tie-break by label alphabetical
    }
};

std::vector<std::string> classifyUnknown(const std::string& filename, int k) {
    std::ifstream file(filename);
    if (!file.is_open()) return {};

    std::vector<Node> training;
    std::vector<Node> unknowns;
    float r_min = std::numeric_limits<float>::max();
    float r_max = std::numeric_limits<float>::lowest();
    float a_min = std::numeric_limits<float>::max();
    float a_max = std::numeric_limits<float>::lowest();

    std::string line;
    // Skip header if present (assume first line may contain column names)
    if (std::getline(file, line)) {
        // If the first line contains non-numeric data, it's a header, ignore.
        // But since we don't know, we'll just process it as a data line and it will fail parsing.
        // Instead, we can try to parse; if r is not an integer, skip.
        // For simplicity, we assume file always starts with data, but if header exists, parsing fails.
        // To handle both, we attempt to parse; if fails, ignore.
        std::istringstream iss(line);
        int r;
        float a;
        char t;
        char comma;
        if (iss >> r >> comma >> a >> comma >> t) {
            Node n{r, a, t};
            if (t == '?') unknowns.push_back(n);
            else {
                training.push_back(n);
                r_min = std::min(r_min, (float)n.r);
                r_max = std::max(r_max, (float)n.r);
                a_min = std::min(a_min, n.a);
                a_max = std::max(a_max, n.a);
            }
        }
        // else it was a header, ignore.
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int r;
        float a;
        char t;
        char comma;
        if (iss >> r >> comma >> a >> comma >> t) {
            Node n{r, a, t};
            if (t == '?') unknowns.push_back(n);
            else {
                training.push_back(n);
                r_min = std::min(r_min, (float)n.r);
                r_max = std::max(r_max, (float)n.r);
                a_min = std::min(a_min, n.a);
                a_max = std::max(a_max, n.a);
            }
        }
    }

    if (training.empty() || unknowns.empty()) return {};

    float r_range = r_max - r_min;
    float a_range = a_max - a_min;
    if (r_range == 0.0f) r_range = 1.0f; // avoid division by zero, but we handle below by checking
    if (a_range == 0.0f) a_range = 1.0f;

    std::vector<std::string> results;
    for (const auto& unk : unknowns) {
        std::vector<DistancePair> distances;
        for (const auto& train : training) {
            float dr = (float)(unk.r - train.r) / r_range;
            float da = (unk.a - train.a) / a_range;
            if (r_range == 1.0f && (r_max == r_min)) dr = 0.0f; // actual zero range
            if (a_range == 1.0f && (a_max == a_min)) da = 0.0f;
            float d = dr*dr + da*da;
            distances.push_back({d, train.t});
        }
        std::sort(distances.begin(), distances.end());
        int num_neighbors = std::min(k, (int)distances.size());
        int count_a = 0, count_f = 0, count_h = 0;
        for (int i = 0; i < num_neighbors; ++i) {
            if (distances[i].label == 'a') count_a++;
            else if (distances[i].label == 'f') count_f++;
            else if (distances[i].label == 'h') count_h++;
        }
        char best = 'a';
        int best_count = count_a;
        if (count_f > best_count || (count_f == best_count && 'f' < best)) {
            best = 'f';
            best_count = count_f;
        }
        if (count_h > best_count || (count_h == best_count && 'h' < best)) {
            best = 'h';
            best_count = count_h;
        }
        std::ostringstream oss;
        oss << "r=" << unk.r << ", a=" << std::fixed << std::setprecision(2) << unk.a << ", classified as " << best;
        results.push_back(oss.str());
    }
    return results;
}
#include <cassert>
#include <fstream>
#include <vector>
#include <string>

// Include the solution function here (or in a separate header, but for test we assume it's above)

int main() {
    // Create a temporary CSV file for testing
    const char* filename = "test_knn.csv";
    {
        std::ofstream f(filename);
        f << "r,a,t\n";
        f << "1,1.0,a\n";
        f << "2,2.0,a\n";
        f << "10,10.0,h\n";
        f << "11,11.0,h\n";
        f << "5,5.0,f\n";
        f << "6,6.0,f\n";
        f << "3,3.0,?\n";
    }
    // Test 1: k=1, nearest is (1,1.0,a) or (2,2.0,a) because both distance 0? Actually unknown (3,3.0)
    // Training ranges: r:1-11 (range 10), a:1-11 (range 10)
    // Distances: to (1,1): dr=2/10=0.2, da=2/10=0.2, d=0.08
    // to (2,2): dr=1/10=0.1, da=1/10=0.1, d=0.02
    // to (10,10): dr=7/10=0.7, da=7/10=0.7, d=0.98
    // to (11,11): dr=8/10=0.8, da=8/10=0.8, d=1.28
    // to (5,5): dr=2/10=0.2, da=2/10=0.2, d=0.08
    // to (6,6): dr=3/10=0.3, da=3/10=0.3, d=0.18
    // Sorted: (0.02,'a'), (0.08,'a'), (0.08,'f'),... so k=1 -> 'a'
    std::vector<std::string> res = classifyUnknown(filename, 1);
    assert(res.size() == 1);
    assert(res[0] == "r=3, a=3.00, classified as a");

    // Test 2: k=3 -> neighbors are 'a', 'a', 'f' -> majority 'a'
    res = classifyUnknown(filename, 3);
    assert(res.size() == 1);
    assert(res[0] == "r=3, a=3.00, classified as a");

    // Test 3: k=5 -> neighbors: 0.02(a),0.08(a),0.08(f),0.18(f),0.98(h) -> counts a:2, f:2, h:1 -> tie a and f -> pick 'a'
    res = classifyUnknown(filename, 5);
    assert(res.size() == 1);
    assert(res[0] == "r=3, a=3.00, classified as a");

    // Test 4: k=6 -> all training: a:2, f:2, h:2 -> tie between a,f,h -> pick 'a'
    res = classifyUnknown(filename, 6);
    assert(res.size() == 1);
    assert(res[0] == "r=3, a=3.00, classified as a");

    // Test 5: No unknowns -> file with no '?' rows
    {
        std::ofstream f2("test_no_unknown.csv");
        f2 << "1,1.0,a\n2,2.0,h\n";
    }
    res = classifyUnknown("test_no_unknown.csv", 2);
    assert(res.empty());

    // Test 6: No training (all '?')
    {
        std::ofstream f3("test_all_unknown.csv");
        f3 << "1,1.0,?\n2,2.0,?\n";
    }
    res = classifyUnknown("test_all_unknown.csv", 1);
    assert(res.empty());

    // Test 7: Zero range attributes (all training have same r and a)
    {
        std::ofstream f4("test_zero_range.csv");
        f4 << "5,2.0,a\n5,2.0,h\n5,2.0,f\n5,2.0,?\n";
    }
    // All distances are 0, tie by label alphabetical after sorting => labels: a,h,f with all distance 0.
    // Sorted by distance then label: a, f, h (because 'a'<'f'<'h').
    // For k=1 -> 'a', k=2 -> 'a','f' majority a? counts a:1, f:1 -> tie -> pick 'a'
    res = classifyUnknown("test_zero_range.csv", 1);
    assert(res.size() == 1);
    assert(res[0] == "r=5, a=2.00, classified as a");

    res = classifyUnknown("test_zero_range.csv", 2);
    assert(res.size() == 1);
    assert(res[0] == "r=5, a=2.00, classified as a");

    // Test 8: Multiple unknowns
    {
        std::ofstream f5("test_multi_unknown.csv");
        f5 << "1,1.0,a\n2,2.0,a\n10,10.0,h\n11,11.0,h\n3,3.0,?\n12,12.0,?\n";
    }
    // For unknown (3,3) with k=2 -> 'a'
    // For unknown (12,12) with k=2 -> distances: to (10,10): dr=2/10=0.2, da=2/10=0.2, d=0.08 -> h
    // to (11,11): dr=1/10=0.1, da=1/10=0.1, d=0.02 -> h
    // to (1,1): dr=11/10=1.1, da=1.1, d=2.42 -> a
    // to (2,2): dr=1.0, da=1.0, d=2.0 -> a
    // So k=2 -> 'h'
    res = classifyUnknown("test_multi_unknown.csv", 2);
    assert(res.size() == 2);
    assert(res[0] == "r=3, a=3.00, classified as a");
    assert(res[1] == "r=12, a=12.00, classified as h");

    // Cleanup not necessary for test, but we can remove files if desired (optional)

    return 0;
}
