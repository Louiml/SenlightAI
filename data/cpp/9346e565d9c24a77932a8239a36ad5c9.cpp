// Given a 2D vector representing the grades of students across multiple academic periods, where each row corresponds to a student and each column to a period, and the grades are floating-point numbers between 1 and 10, write a C++ function that returns a vector of strings describing the evolution of the class average per period. Specifically, for each period (excluding the first), compare the average grade of the class in that period with the average grade in the previous period. If the average increased by more than 5% (ratio > 1.05), the period is labeled "improved"; if it decreased by more than 5% (ratio < 0.95), it is labeled "declined"; otherwise (ratio between 0.95 and 1.05 inclusive), it is labeled "stable". The function should accept a `const std::vector<std::vector<float>>&` and return a `std::vector<std::string>` where each element corresponds to a period from index 1 to n-1, in order, and contains a string in the format: "Period <index>: <label> (average <avg>)", where `<index>` is the current period number, `<label>` is one of "improved", "declined", "stable", and `<avg>` is the current period's class average formatted to two decimal places using `std::fixed` and `std::setprecision(2)`. Assume the input has at least two periods and that each student has grades for all periods.

// The solution computes the class average for each period by summing the grades of all students for that period and dividing by the number of students. These averages are stored in a vector. Then, for each period from index 1 to n-1 (where n is the number of periods), the ratio of the current period average to the previous period average is calculated. Edge cases include: when the previous average is zero (which cannot happen since grades are between 1 and 10), when the ratio is exactly 1.0 (classified as stable), and when the ratio is exactly 1.05 or 0.95 (both included in "stable"). The ratio thresholds are chosen as: ratio > 1.05 means improved, ratio < 0.95 means declined, otherwise stable. The output string uses `std::ostringstream` to format the average with two decimal places. Time complexity is O(num_students * num_periods) for computing averages, plus O(num_periods) for the classification, so overall O(S * P) where S is the number of students and P is the number of periods. Space complexity is O(P) for storing the averages and the output vector.

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

// Given a 2D vector of student grades (rows=students, cols=periods),
// return a vector of strings describing the evolution of class averages
// from period 1 to period n-1, labeling each period as improved, declined, or stable.
std::vector<std::string> classAverageEvolution(const std::vector<std::vector<float>>& grades) {
    if (grades.empty() || grades[0].size() < 2) {
        return {};
    }
    size_t numStudents = grades.size();
    size_t numPeriods = grades[0].size();
    
    // Compute class average per period
    std::vector<float> averages(numPeriods, 0.0f);
    for (size_t period = 0; period < numPeriods; ++period) {
        float sum = 0.0f;
        for (size_t student = 0; student < numStudents; ++student) {
            sum += grades[student][period];
        }
        averages[period] = sum / static_cast<float>(numStudents);
    }
    
    std::vector<std::string> result;
    result.reserve(numPeriods - 1);
    
    for (size_t period = 1; period < numPeriods; ++period) {
        float current = averages[period];
        float previous = averages[period - 1];
        float ratio = current / previous; // previous cannot be zero (grades are >= 1)
        
        std::string label;
        if (ratio > 1.05f) {
            label = "improved";
        } else if (ratio < 0.95f) {
            label = "declined";
        } else {
            label = "stable";
        }
        
        std::ostringstream oss;
        oss << "Period " << period << ": " << label << " (average " 
            << std::fixed << std::setprecision(2) << current << ")";
        result.push_back(oss.str());
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Test the solution function
int main() {
    // Case 1: Simple improvement from 5.0 to 6.0 (ratio 1.2) and decline to 4.0 (ratio 0.666)
    std::vector<std::vector<float>> grades1 = {
        {5.0f, 6.0f, 4.0f},
        {5.0f, 6.0f, 4.0f}
    };
    std::vector<std::string> result1 = classAverageEvolution(grades1);
    assert(result1.size() == 2);
    assert(result1[0] == "Period 1: improved (average 6.00)");
    assert(result1[1] == "Period 2: declined (average 4.00)");
    
    // Case 2: Stable with slight change (ratio exactly 1.0)
    std::vector<std::vector<float>> grades2 = {
        {7.0f, 7.0f, 7.0f}
    };
    std::vector<std::string> result2 = classAverageEvolution(grades2);
    assert(result2.size() == 2);
    assert(result2[0] == "Period 1: stable (average 7.00)");
    assert(result2[1] == "Period 2: stable (average 7.00)");
    
    // Case 3: Ratio exactly 1.05 (should be stable) and exactly 0.95 (should be stable)
    std::vector<std::vector<float>> grades3 = {
        {4.0f, 4.2f, 3.99f}, // 4.2/4.0 = 1.05, 3.99/4.2 = 0.95
        {4.0f, 4.2f, 3.99f}
    };
    std::vector<std::string> result3 = classAverageEvolution(grades3);
    assert(result3.size() == 2);
    assert(result3[0] == "Period 1: stable (average 4.20)");
    assert(result3[1] == "Period 2: stable (average 3.99)");
    
    // Case 4: Mixed students with multiple periods
    std::vector<std::vector<float>> grades4 = {
        {1.0f, 10.0f, 6.0f},
        {5.0f, 5.0f, 5.0f},
        {3.0f, 3.0f, 3.0f}
    };
    // Averages: period0=3.0, period1=6.0 (ratio 2.0 -> improved), period2=4.666 (ratio 0.777 -> declined)
    std::vector<std::string> result4 = classAverageEvolution(grades4);
    assert(result4.size() == 2);
    assert(result4[0] == "Period 1: improved (average 6.00)");
    assert(result4[1] == "Period 2: declined (average 4.67)");
    
    return 0;
}
