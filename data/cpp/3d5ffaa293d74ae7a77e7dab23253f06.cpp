/*
Write a C++ function that takes a vector of 4x4 transformation matrices (double precision) representing a camera trajectory, along with a vector of timestamp strings, and writes the trajectory to a file in TUM benchmark format. For each non-empty matrix, extract the rotation submatrix (top-left 3x3), convert it to a rotation vector using Rodrigues formula, normalize it by its angle, compute the quaternion components (qx, qy, qz, qw) using half-angle formulas, and output a line with the timestamp, translation components (tx, ty, tz), and quaternion components, all separated by spaces and using fixed-point notation. The function should skip empty matrices and ensure all matrices are CV_64FC1 type.
*/
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

// Writes a TUM-format trajectory from a vector of 4x4 double matrices.
// Each non-empty matrix is written as: timestamp tx ty tz qx qy qz qw
// using fixed-point notation. Empty matrices are skipped.
void writeTumTrajectory(const std::string& filename,
                        const std::vector<std::string>& timestamps,
                        const std::vector<cv::Mat>& Rts) {
    if (timestamps.size() != Rts.size())
        return;
    
    std::ofstream file(filename.c_str());
    if (!file.is_open())
        return;
    
    file.precision(6);
    file << std::fixed;
    
    for (size_t i = 0; i < Rts.size(); ++i) {
        const cv::Mat& Rt = Rts[i];
        if (Rt.empty())
            continue;
        
        if (Rt.type() != CV_64FC1)
            return;
        
        // Extract rotation matrix (top-left 3x3)
        cv::Mat R = Rt(cv::Rect(0, 0, 3, 3)).clone();
        
        // Convert rotation matrix to rotation vector
        cv::Mat rvec;
        cv::Rodrigues(R, rvec);
        double alpha = cv::norm(rvec);
        
        // Normalize rotation vector; avoid division by zero for null rotation
        if (alpha > DBL_MIN)
            rvec = rvec / alpha;
        
        // Compute quaternion components
        double cos_alpha2 = std::cos(0.5 * alpha);
        double sin_alpha2 = std::sin(0.5 * alpha);
        cv::Mat q = rvec * sin_alpha2;  // qx, qy, qz
        
        // Translation components (fourth column)
        double tx = Rt.at<double>(0, 3);
        double ty = Rt.at<double>(1, 3);
        double tz = Rt.at<double>(2, 3);
        
        file << timestamps[i] << " "
             << tx << " " << ty << " " << tz << " "
             << q.at<double>(0) << " " << q.at<double>(1) << " "
             << q.at<double>(2) << " " << cos_alpha2 << "\n";
    }
    file.close();
}
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>

// Solution function is assumed to be declared above.

int main() {
    // Test 1: Identity matrix (no rotation)
    {
        cv::Mat Rt = cv::Mat::eye(4, 4, CV_64FC1);
        std::vector<cv::Mat> Rts = {Rt};
        std::vector<std::string> timestamps = {"1.000000"};
        writeTumTrajectory("test1.txt", timestamps, Rts);
        std::ifstream file("test1.txt");
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string ts;
        double tx, ty, tz, qx, qy, qz, qw;
        iss >> ts >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        assert(ts == "1.000000");
        assert(tx == 0.0 && ty == 0.0 && tz == 0.0);
        assert(qx == 0.0 && qy == 0.0 && qz == 0.0);
        assert(std::abs(qw - 1.0) < 1e-9);
    }
    
    // Test 2: Rotation of 90 degrees around Z-axis
    {
        cv::Mat Rt = cv::Mat::eye(4, 4, CV_64FC1);
        // Set rotation matrix: Rz(90 deg)
        double angle = CV_PI / 2;
        Rt.at<double>(0,0) = cos(angle);
        Rt.at<double>(0,1) = -sin(angle);
        Rt.at<double>(1,0) = sin(angle);
        Rt.at<double>(1,1) = cos(angle);
        Rt.at<double>(0,3) = 1.0;
        Rt.at<double>(1,3) = 2.0;
        Rt.at<double>(2,3) = 3.0;
        std::vector<cv::Mat> Rts = {Rt};
        std::vector<std::string> timestamps = {"2.000000"};
        writeTumTrajectory("test2.txt", timestamps, Rts);
        std::ifstream file("test2.txt");
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string ts;
        double tx, ty, tz, qx, qy, qz, qw;
        iss >> ts >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        assert(ts == "2.000000");
        assert(tx == 1.0 && ty == 2.0 && tz == 3.0);
        // For 90 deg around Z: qz = sin(45 deg) = sqrt(2)/2, qw = cos(45 deg)
        double expected = std::sqrt(2.0) / 2.0;
        assert(std::abs(qx - 0.0) < 1e-9);
        assert(std::abs(qy - 0.0) < 1e-9);
        assert(std::abs(qz - expected) < 1e-9);
        assert(std::abs(qw - expected) < 1e-9);
    }
    
    // Test 3: Empty matrix is skipped
    {
        cv::Mat empty;
        cv::Mat Rt = cv::Mat::eye(4, 4, CV_64FC1);
        std::vector<cv::Mat> Rts = {empty, Rt};
        std::vector<std::string> timestamps = {"3.0", "3.1"};
        writeTumTrajectory("test3.txt", timestamps, Rts);
        std::ifstream file("test3.txt");
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string ts;
        double tx, ty, tz, qx, qy, qz, qw;
        iss >> ts >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        assert(ts == "3.1");  // Only the non-empty matrix is written
        assert(tx == 0.0 && ty == 0.0 && tz == 0.0);
        assert(qx == 0.0 && qy == 0.0 && qz == 0.0);
        assert(std::abs(qw - 1.0) < 1e-9);
        std::getline(file, line);  // Should be EOF
        assert(file.eof());
    }
    
    // Test 4: Translation-only (identity rotation) with nonzero translation
    {
        cv::Mat Rt = cv::Mat::eye(4, 4, CV_64FC1);
        Rt.at<double>(0,3) = 5.5;
        Rt.at<double>(1,3) = -1.25;
        Rt.at<double>(2,3) = 0.01;
        std::vector<cv::Mat> Rts = {Rt};
        std::vector<std::string> timestamps = {"4.000000"};
        writeTumTrajectory("test4.txt", timestamps, Rts);
        std::ifstream file("test4.txt");
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string ts;
        double tx, ty, tz, qx, qy, qz, qw;
        iss >> ts >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        assert(ts == "4.000000");
        assert(tx == 5.5 && ty == -1.25 && tz == 0.01);
        assert(qx == 0.0 && qy == 0.0 && qz == 0.0);
        assert(std::abs(qw - 1.0) < 1e-9);
    }
    
    // Test 5: Multiple matrices
    {
        cv::Mat Rt1 = cv::Mat::eye(4, 4, CV_64FC1);
        Rt1.at<double>(0,3) = 1.0;
        cv::Mat Rt2 = cv::Mat::eye(4, 4, CV_64FC1);
        Rt2.at<double>(1,3) = -2.0;
        std::vector<cv::Mat> Rts = {Rt1, Rt2};
        std::vector<std::string> timestamps = {"5.0", "5.1"};
        writeTumTrajectory("test5.txt", timestamps, Rts);
        std::ifstream file("test5.txt");
        std::string line;
        std::getline(file, line);
        std::istringstream iss1(line);
        std::string ts1;
        double tx1, ty1, tz1, qx1, qy1, qz1, qw1;
        iss1 >> ts1 >> tx1 >> ty1 >> tz1 >> qx1 >> qy1 >> qz1 >> qw1;
        assert(ts1 == "5.0");
        assert(tx1 == 1.0 && ty1 == 0.0 && tz1 == 0.0);
        assert(qx1 == 0.0 && qy1 == 0.0 && qz1 == 0.0);
        assert(std::abs(qw1 - 1.0) < 1e-9);

        std::getline(file, line);
        std::istringstream iss2(line);
        std::string ts2;
        double tx2, ty2, tz2, qx2, qy2, qz2, qw2;
        iss2 >> ts2 >> tx2 >> ty2 >> tz2 >> qx2 >> qy2 >> qz2 >> qw2;
        assert(ts2 == "5.1");
        assert(tx2 == 0.0 && ty2 == -2.0 && tz2 == 0.0);
        assert(qx2 == 0.0 && qy2 == 0.0 && qz2 == 0.0);
        assert(std::abs(qw2 - 1.0) < 1e-9);
    }
    
    return 0;
}
// The main algorithm processes each transformation matrix in order. For each matrix, we first validate its type and extract the rotation submatrix R (a 3x3 matrix). Using the Rodrigues function, we obtain a rotation vector rvec whose norm equals the rotation angle alpha. The quaternion is computed using half-angle relationships: qw = cos(alpha/2), and qx, qy, qz are derived by scaling the normalized rotation vector by sin(alpha/2). The translation vector is directly the fourth column (elements at (0,3), (1,3), (2,3)). The output formatting uses fixed-point with default precision (6 decimal places) for the translation and quaternion values, preceded by the timestamp string. Edge cases include empty matrices (skipped), null rotation (alpha=0, where we avoid division by zero by checking alpha > DBL_MIN), and the case where the file cannot be opened (function returns silently). The function assumes the timestamps vector matches the size of the transformation vector, but it processes only non-empty matrices. Time complexity is O(n) for n matrices, and each conversion is O(1). Space complexity is O(1) beyond the output file buffer since we process matrices one at a time without storing additional data.
