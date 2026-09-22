// Write a C++ function `cv::Mat getGaborKernel(Size ksize, double sigma, double theta, double lambd, double gamma, double psi, int ktype)` that generates a 2D Gabor filter kernel as a single-channel `cv::Mat` of type `CV_32F` or `CV_64F`. The kernel size is determined by `ksize`; if either dimension is 0 or negative, compute the corresponding half-size as `round(3 * max(|sigma_x * cos(theta)|, |sigma_y * sin(theta)|))` for width and `round(3 * max(|sigma_x * sin(theta)|, |sigma_y * cos(theta)|))` for height, where `sigma_x = sigma` and `sigma_y = sigma / gamma`. The kernel values are computed over coordinates `(x, y)` ranging from `-half` to `+half` (inclusive), rotated by `theta` to get `xr = x*cos(theta) + y*sin(theta)` and `yr = -x*sin(theta) + y*cos(theta)`, then assigned as `v = exp(-0.5 * (xr^2 / sigma_x^2 + yr^2 / sigma_y^2)) * cos(2*pi*xr/lambd + psi)`. Store the value at position `(ymax - y, xmax - x)` so that the kernel is centered with the origin at the middle. Handle both `CV_32F` and `CV_64F` output types correctly.
The function must first validate the output type and determine the kernel dimensions. For explicit positive `ksize`, compute `xmax = ksize.width/2` and `ymax = ksize.height/2` (integer division). For non-positive dimensions, compute `xmax` and `ymax` using the formula with `nstds = 3`. Then allocate a `cv::Mat` of size `(ymax - ymin + 1, xmax - xmin + 1)` where `ymin = -ymax` and `xmin = -xmax`. The main algorithm iterates over all integer coordinates from `ymin` to `ymax` and `xmin` to `xmax`, applies rotation, computes the Gaussian envelope and cosine modulation, and stores the result at the mirrored index. Edge cases: if `gamma` is zero, division by zero occurs; the original code does not guard this, so we can assume `gamma` is non-zero. If `sigma` is zero, the exponent becomes undefined; correctly, this would produce a kernel of zeros or ones depending on the limit, but we can assume positive `sigma`. Time complexity is `O(ksize.width * ksize.height)` (or `O((2*xmax+1)*(2*ymax+1))` for auto-size), space complexity is the same for the output matrix. The rotation ensures the kernel is oriented along angle `theta`. The `cscale = 2*pi/lambd` factor normalizes the wavelength.
#include <opencv2/core.hpp>
#include <cmath>
#include <algorithm>

/**
 * @brief Generate a 2D Gabor filter kernel.
 * @param ksize Kernel size. If width or height <= 0, that dimension is auto-computed.
 * @param sigma Standard deviation of the Gaussian envelope.
 * @param theta Orientation angle in radians.
 * @param lambd Wavelength of the sinusoidal factor.
 * @param gamma Spatial aspect ratio.
 * @param psi Phase offset.
 * @param ktype Must be CV_32F or CV_64F.
 * @return A single-channel Mat containing the Gabor kernel.
 */
cv::Mat getGaborKernel(cv::Size ksize, double sigma, double theta,
                       double lambd, double gamma, double psi, int ktype)
{
    double sigma_x = sigma;
    double sigma_y = sigma / gamma;
    const int nstds = 3;
    int xmin, xmax, ymin, ymax;
    double c = std::cos(theta);
    double s = std::sin(theta);

    if (ksize.width > 0)
        xmax = ksize.width / 2;
    else
        xmax = cvRound(std::max(std::fabs(nstds * sigma_x * c),
                                std::fabs(nstds * sigma_y * s)));

    if (ksize.height > 0)
        ymax = ksize.height / 2;
    else
        ymax = cvRound(std::max(std::fabs(nstds * sigma_x * s),
                                std::fabs(nstds * sigma_y * c)));

    xmin = -xmax;
    ymin = -ymax;

    CV_Assert(ktype == CV_32F || ktype == CV_64F);

    cv::Mat kernel(ymax - ymin + 1, xmax - xmin + 1, ktype);
    double ex = -0.5 / (sigma_x * sigma_x);
    double ey = -0.5 / (sigma_y * sigma_y);
    double cscale = CV_PI * 2.0 / lambd;

    for (int y = ymin; y <= ymax; ++y)
    {
        for (int x = xmin; x <= xmax; ++x)
        {
            double xr = x * c + y * s;
            double yr = -x * s + y * c;

            double v = std::exp(ex * xr * xr + ey * yr * yr) *
                       std::cos(cscale * xr + psi);

            if (ktype == CV_32F)
                kernel.at<float>(ymax - y, xmax - x) = static_cast<float>(v);
            else
                kernel.at<double>(ymax - y, xmax - x) = v;
        }
    }

    return kernel;
}
#include <cassert>
#include <cmath>
#include <opencv2/core.hpp>

// Declaration of the solution function (should match the one above)
cv::Mat getGaborKernel(cv::Size ksize, double sigma, double theta,
                       double lambd, double gamma, double psi, int ktype);

int main()
{
    // Test 1: Check size for explicit kernel size
    cv::Mat k1 = getGaborKernel(cv::Size(5, 7), 2.0, 0.0, 4.0, 0.5, 0.0, CV_64F);
    assert(k1.rows == 7 && k1.cols == 5);
    assert(k1.type() == CV_64F);

    // Test 2: Auto-size for both dimensions zero
    cv::Mat k2 = getGaborKernel(cv::Size(0, 0), 1.0, 0.0, 3.0, 1.0, 0.0, CV_32F);
    // xmax = round(3*max(|1*1|,|1*0|)) = 3, so cols = 2*3+1 = 7
    // ymax = round(3*max(|1*0|,|1*1|)) = 3, so rows = 7
    assert(k2.rows == 7 && k2.cols == 7);
    assert(k2.type() == CV_32F);

    // Test 3: Center value for theta=0, psi=0, gamma=1, lambd=4, sigma=2
    // center at (x=0,y=0) => xr=0, yr=0 => v = exp(0)*cos(0) = 1
    cv::Mat k3 = getGaborKernel(cv::Size(5, 5), 2.0, 0.0, 4.0, 1.0, 0.0, CV_64F);
    int cx = k3.cols / 2;
    int cy = k3.rows / 2;
    assert(std::fabs(k3.at<double>(cy, cx) - 1.0) < 1e-9);

    // Test 4: Symmetry for theta=0, psi=0, gamma=1 (real part is symmetric about center)
    cv::Mat k4 = getGaborKernel(cv::Size(7, 7), 1.5, 0.0, 2.0, 1.0, 0.0, CV_64F);
    int c = 3;
    for (int dy = -3; dy <= 3; ++dy)
    {
        for (int dx = -3; dx <= 3; ++dx)
        {
            double v = k4.at<double>(c + dy, c + dx);
            double v_mirror = k4.at<double>(c - dy, c - dx);
            assert(std::fabs(v - v_mirror) < 1e-9);
        }
    }

    // Test 5: All values finite and within expected range [-1, 1] for cosine modulation
    cv::Mat k5 = getGaborKernel(cv::Size(9, 9), 3.0, 0.5, 5.0, 0.7, 1.2, CV_64F);
    for (int i = 0; i < k5.rows; ++i)
    {
        for (int j = 0; j < k5.cols; ++j)
        {
            double val = k5.at<double>(i, j);
            assert(std::isfinite(val));
            assert(val >= -1.0 - 1e-12 && val <= 1.0 + 1e-12);
        }
    }

    // Test 6: Type check for CV_32F output
    cv::Mat k6 = getGaborKernel(cv::Size(4, 4), 1.0, 0.0, 1.0, 1.0, 0.0, CV_32F);
    assert(k6.type() == CV_32F);
    assert(k6.at<float>(2, 1) == static_cast<float>(k6.at<double>(0,0))); // just check type consistency

    return 0;
}
