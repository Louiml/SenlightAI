Write a C++ function `float computeInformationGain(const std::vector<std::vector<std::string>>& dataset, const std::vector<std::unordered_set<std::string>>& values, const std::unordered_map<std::string,int>& decisions, int attribute_num, bool use_threshold, float threshold)` that computes the information gain for a given attribute in a small classification dataset. The dataset is represented as rows of string values, with the last column being the class label. Each attribute may be either categorical (all values non-numeric) or numeric (all values parseable as floats). For numeric attributes, you must use the provided threshold to split rows: values less than the threshold go to the left partition, all others to the right. For categorical attributes, each distinct value forms its own partition. The information gain is computed as the entropy of the entire dataset minus the weighted average entropy of the partitions. The function should handle edge cases where a partition has zero size (entropy contribution is 0), and where the dataset might be empty (return 0). You may assume the `dataset` is non-empty and consistent (all rows have same length equal to `values.size()`), and that `decisions` contains the correct counts for the full dataset. Use `log2` from `<cmath>`. The function must be `const`-correct and not modify any input.
#include <cassert>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

int main() {
    // Dataset: [outlook, temperature, humidity, play]
    // outlook: Sunny, Overcast, Rain (categorical)
    // temperature: numeric (e.g., 85, 80, 83, 70, 68, 65, 64, 72, 69, 75, 75, 72, 81, 71)
    // humidity: numeric (e.g., 85, 90, 78, 96, 80, 70, 65, 95, 70, 80, 70, 90, 75, 80)
    // play: Yes/No
    std::vector<std::vector<std::string>> data = {
        {"Sunny","85","85","No"},
        {"Sunny","80","90","No"},
        {"Overcast","83","78","Yes"},
        {"Rain","70","96","Yes"},
        {"Rain","68","80","Yes"},
        {"Rain","65","70","No"},
        {"Overcast","64","65","Yes"},
        {"Sunny","72","95","No"},
        {"Sunny","69","70","Yes"},
        {"Rain","75","80","Yes"},
        {"Sunny","75","70","Yes"},
        {"Overcast","72","90","Yes"},
        {"Overcast","81","75","Yes"},
        {"Rain","71","80","No"}
    };

    std::vector<std::unordered_set<std::string>> values(4);
    std::unordered_map<std::string,int> decisions;
    for (const auto& row : data) {
        for (int i = 0; i < 4; ++i) {
            values[i].insert(row[i]);
        }
        decisions[row.back()]++;
    }

    // Test categorical attribute (outlook) - expected value based on known dataset
    float ig_outlook = computeInformationGain(data, values, decisions, 0, false, 0.0f);
    // Exact value can be computed but we assert it's between 0.2 and 0.3
    assert(ig_outlook > 0.2f && ig_outlook < 0.3f);

    // Test numeric attribute with a specific threshold (e.g., humidity <= 75 vs > 75)
    // Using threshold 75: split rows with humidity < 75 vs >= 75
    // We can compute manually: full entropy = - (9/14 log2(9/14) + 5/14 log2(5/14)) ≈ 0.9403
    // Let's verify the function does not crash and returns a plausible value
    float ig_humidity_75 = computeInformationGain(data, values, decisions, 2, true, 75.0f);
    assert(ig_humidity_75 >= 0.0f && ig_humidity_75 <= 1.0f);

    // Test edge case: threshold below all values -> all go right, entropy gain equals 0
    float ig_low = computeInformationGain(data, values, decisions, 1, true, 0.0f);
    // With threshold 0, all temperature values >= 0, so left_size=0, right_size=14 -> weighted sum = full entropy -> gain 0
    // But due to floating point, allow tolerance
    assert(fabs(ig_low) < 1e-5);

    // Test edge case: single row dataset
    std::vector<std::vector<std::string>> single_data = {{"A","1","X"}};
    std::vector<std::unordered_set<std::string>> single_val(3);
    single_val[0].insert("A");
    single_val[1].insert("1");
    single_val[2].insert("X");
    std::unordered_map<std::string,int> single_dec = {{"X",1}};
    float ig_single = computeInformationGain(single_data, single_val, single_dec, 0, false, 0.0f);
    assert(fabs(ig_single) < 1e-6); // entropy of pure class = 0, gain = 0

    // Test empty dataset returns 0
    std::vector<std::vector<std::string>> empty_data;
    std::vector<std::unordered_set<std::string>> empty_val(2);
    std::unordered_map<std::string,int> empty_dec;
    float ig_empty = computeInformationGain(empty_data, empty_val, empty_dec, 0, false, 0.0f);
    assert(fabs(ig_empty) < 1e-6);

    // Test numeric attribute with threshold that splits perfectly (e.g., temperature <= 70 vs > 70)
    float ig_temp_70 = computeInformationGain(data, values, decisions, 1, true, 70.0f);
    // Should be non-negative and at most 1
    assert(ig_temp_70 >= 0.0f && ig_temp_70 <= 1.0f);

    return 0;
}
#include <cmath>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// Helper to check if a string represents a numeric value (integer or float, optionally negative)
static bool isNumeric(const std::string& s) {
    if (s.empty()) return false;
    size_t pos = 0;
    if (s[pos] == '-') {
        if (pos + 1 == s.size()) return false;
        pos++;
    }
    bool has_dot = false;
    for (; pos < s.size(); ++pos) {
        if (s[pos] == '.') {
            if (has_dot) return false;
            has_dot = true;
        } else if (!std::isdigit(static_cast<unsigned char>(s[pos]))) {
            return false;
        }
    }
    return true;
}

// Compute entropy of a partition given total size and frequency map of labels
static float entropyOfPartition(int partition_size, const std::unordered_map<std::string,int>& freq) {
    if (partition_size <= 0) return 0.0f;
    float entropy = 0.0f;
    for (const auto& kv : freq) {
        float p = static_cast<float>(kv.second) / partition_size;
        entropy -= p * std::log2(p);
    }
    return entropy;
}

// Compute information gain for a given attribute column.
// attribute_num is 0-based index into dataset rows (last column is the class label).
// For numeric attributes, use provided threshold (use_threshold == true) to split < and >=.
// For categorical attributes, threshold is ignored.
float computeInformationGain(
    const std::vector<std::vector<std::string>>& dataset,
    const std::vector<std::unordered_set<std::string>>& values,
    const std::unordered_map<std::string,int>& decisions,
    int attribute_num,
    bool use_threshold,
    float threshold)
{
    if (dataset.empty()) return 0.0f;

    int n = static_cast<int>(dataset.size());
    // Full dataset entropy
    float full_entropy = entropyOfPartition(n, decisions);

    // Determine if attribute is numeric
    const auto& value_set = values[attribute_num];
    bool numeric = (value_set.empty()) ? false : isNumeric(*value_set.begin());

    float weighted_sum = 0.0f;

    if (numeric && use_threshold) {
        // Split into < threshold and >= threshold
        int left_size = 0, right_size = 0;
        std::unordered_map<std::string,int> left_freq, right_freq;
        for (const auto& row : dataset) {
            float val = std::stof(row[attribute_num]);
            const std::string& label = row.back();
            if (val < threshold) {
                left_size++;
                left_freq[label]++;
            } else {
                right_size++;
                right_freq[label]++;
            }
        }
        float left_entropy = entropyOfPartition(left_size, left_freq);
        float right_entropy = entropyOfPartition(right_size, right_freq);
        float left_weight = static_cast<float>(left_size) / n;
        float right_weight = static_cast<float>(right_size) / n;
        weighted_sum = left_weight * left_entropy + right_weight * right_entropy;
    } else {
        // Categorical: split per distinct value
        for (const std::string& val : value_set) {
            int group_size = 0;
            std::unordered_map<std::string,int> group_freq;
            for (const auto& row : dataset) {
                if (row[attribute_num] == val) {
                    group_size++;
                    group_freq[row.back()]++;
                }
            }
            if (group_size == 0) continue;
            float group_entropy = entropyOfPartition(group_size, group_freq);
            weighted_sum += (static_cast<float>(group_size) / n) * group_entropy;
        }
    }

    return full_entropy - weighted_sum;
}
// The core algorithm computes information gain in three systematic steps. First, compute the entropy of the whole dataset using the `decisions` map: for each class count, probability = count / dataset_size, and entropy = –Σ(p * log2(p)). If dataset_size == 0, return 0. Second, partition the dataset based on the attribute type. To determine type, inspect `values[attribute_num]`: if all strings in that set are numeric (i.e., each string parses as a float per a helper `isNumeric` that checks for optional negative sign, digits, and a single decimal point), treat the attribute as numeric; otherwise categorical. For numeric attributes, split rows into two groups using the threshold: numeric value < threshold goes to left (call it `left`), else right. For categorical, create one group per distinct value. For each partition group, count the class labels from the last column into a temporary frequency map, and compute the entropy of that group using its size as denominator. Sum the weighted entropies: weight = group_size / dataset_size. Subtract the weighted sum from the full entropy to obtain information gain. Handle edge cases: if a group has size 0, its entropy contribution is 0 (log2(0) avoided by not calling entropy on empty groups). Time complexity is O(N * V) where N is number of rows and V is number of distinct values (for categorical) or 2 (for numeric), since we scan all rows for each value. Space complexity is O(V * C) for temporary maps, but effectively O(C) per partition, where C is number of classes.
