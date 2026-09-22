Write a standalone C++ function that implements the Black-Scholes formula for pricing European call and put options without dividends. The function must take the spot price, strike price, risk-free interest rate, volatility, time to maturity (in years), and an option type indicator (0 for call, 1 for put) as inputs, and return the calculated option price as a double. The implementation must use the cumulative normal distribution approximation described by Hull (using the polynomial approximation with constants 0.319381530, -0.356563782, 1.781477937, -1.821255978, and 1.330274429, and the factor 0.2316419), handling negative inputs to the CDF correctly by symmetry. The function must be self-contained, use only standard C++ headers, and be suitable for use in a pricing engine or as a standalone library function.
The Black-Scholes formula for a European option without dividends computes the theoretical price using the standard closed-form solution:  
For a call: \( C = S \cdot N(d_1) - K \cdot e^{-rT} \cdot N(d_2) \)  
For a put: \( P = K \cdot e^{-rT} \cdot N(-d_2) - S \cdot N(-d_1) \)  
where \( d_1 = \frac{\ln(S/K) + (r + \frac{1}{2}\sigma^2)T}{\sigma \sqrt{T}} \) and \( d_2 = d_1 - \sigma \sqrt{T} \).  
The key component is the cumulative normal distribution function \( N(x) \), which is approximated using the rational polynomial approximation from Hull’s textbook. This approximation first computes \( N'(x) = \frac{1}{\sqrt{2\pi}} e^{-x^2/2} \) and then uses the series \( N(x) \approx 1 - N'(x) \cdot (a_1 k + a_2 k^2 + a_3 k^3 + a_4 k^4 + a_5 k^5) \) where \( k = \frac{1}{1 + 0.2316419|x|} \) and the constants \( a_1=0.319381530, a_2=-0.356563782, a_3=1.781477937, a_4=-1.821255978, a_5=1.330274429 \). For negative \( x \), use the symmetry \( N(-x) = 1 - N(x) \).  

Edge cases to consider:  
- Time to maturity \( T \) must be non-negative (zero is allowed; then the option price equals intrinsic value).  
- Volatility must be non-negative; zero volatility leads to degenerate cases where \( d_1 \) and \( d_2 \) can be undefined (division by zero). In practice, the test will avoid such cases, but the function should handle them gracefully by returning intrinsic value when \( T=0 \) or volatility is zero.  
- The spot price and strike price should be positive to avoid log of non-positive numbers.  
The time complexity is \( O(1) \) for a single option, and the space complexity is \( O(1) \) as only a constant number of intermediate variables are used.
#include <cmath>

// Approximate the cumulative normal distribution function N(x) using the Hull polynomial method.
double cumulativeNormalDistribution(double x) {
    const double inv_sqrt_2xPI = 0.39894228040143270286;
    
    int sign = 0;
    if (x < 0.0) {
        x = -x;
        sign = 1;
    }
    
    double xInput = x;
    double expValues = std::exp(-0.5 * xInput * xInput);
    double xNPrimeofX = expValues * inv_sqrt_2xPI;
    
    double xK2 = 0.2316419 * xInput;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    double xK2_2 = xK2 * xK2;
    double xK2_3 = xK2_2 * xK2;
    double xK2_4 = xK2_3 * xK2;
    double xK2_5 = xK2_4 * xK2;
    
    double xLocal_1 = xK2 * 0.319381530;
    double xLocal_2 = xK2_2 * (-0.356563782);
    double xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_1 = xLocal_2 + xLocal_1;
    double xLocal = xLocal_1 * xNPrimeofX;
    xLocal = 1.0 - xLocal;
    
    double outputX = xLocal;
    if (sign) {
        outputX = 1.0 - outputX;
    }
    return outputX;
}

// Black-Scholes price for a European option without dividends.
// Option type: 0 = call, 1 = put.
double blackScholesPrice(double spotPrice, double strikePrice, double riskFreeRate,
                         double volatility, double timeToMaturity, int optionType) {
    // Handle degenerate cases to avoid division by zero or log of non-positive.
    if (timeToMaturity <= 0.0) {
        // Option is at expiration; intrinsic value.
        if (optionType == 0) {
            return std::max(0.0, spotPrice - strikePrice);
        } else {
            return std::max(0.0, strikePrice - spotPrice);
        }
    }
    if (spotPrice <= 0.0 || strikePrice <= 0.0) {
        // Non-positive spot or strike leads to undefined log; return 0 as a safe fallback.
        return 0.0;
    }
    
    double sqrtTime = std::sqrt(timeToMaturity);
    double d1 = (std::log(spotPrice / strikePrice) + 
                 (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * sqrtTime);
    double d2 = d1 - volatility * sqrtTime;
    
    double NOfd1 = cumulativeNormalDistribution(d1);
    double NOfd2 = cumulativeNormalDistribution(d2);
    double futureValue = strikePrice * std::exp(-riskFreeRate * timeToMaturity);
    
    double optionPrice;
    if (optionType == 0) {
        // Call option
        optionPrice = spotPrice * NOfd1 - futureValue * NOfd2;
    } else {
        // Put option
        double negNOfd1 = 1.0 - NOfd1;
        double negNOfd2 = 1.0 - NOfd2;
        optionPrice = futureValue * negNOfd2 - spotPrice * negNOfd1;
    }
    return optionPrice;
}
#include <cassert>
#include <cmath>

// Forward declaration of the function being tested.
double blackScholesPrice(double spotPrice, double strikePrice, double riskFreeRate,
                         double volatility, double timeToMaturity, int optionType);

int main() {
    // Test known values: 
    // From standard Black-Scholes examples, for S=100, K=100, r=0.05, sigma=0.2, T=1, 
    // call ~ 10.4506, put ~ 5.5735 (values may vary slightly due to approximation).
    double callPrice = blackScholesPrice(100.0, 100.0, 0.05, 0.2, 1.0, 0);
    assert(std::abs(callPrice - 10.4506) < 0.01);
    
    double putPrice = blackScholesPrice(100.0, 100.0, 0.05, 0.2, 1.0, 1);
    assert(std::abs(putPrice - 5.5735) < 0.01);
    
    // Put-call parity: Call - Put = S - K*exp(-rT)
    double parityDifference = callPrice - putPrice;
    double expectedParity = 100.0 - 100.0 * std::exp(-0.05 * 1.0);
    assert(std::abs(parityDifference - expectedParity) < 0.01);
    
    // At expiration (T=0), intrinsic value
    double expiredCall = blackScholesPrice(120.0, 100.0, 0.05, 0.2, 0.0, 0);
    assert(std::abs(expiredCall - 20.0) < 1e-9);
    double expiredPut = blackScholesPrice(80.0, 100.0, 0.05, 0.2, 0.0, 1);
    assert(std::abs(expiredPut - 20.0) < 1e-9);
    
    // Deep in-the-money call should be close to intrinsic value discounted
    double deepITMCall = blackScholesPrice(1000.0, 10.0, 0.02, 0.1, 0.5, 0);
    assert(deepITMCall > 900.0);
    
    // Deep out-of-the-money put should be near zero
    double deepOTMPut = blackScholesPrice(1.0, 1000.0, 0.02, 0.3, 0.5, 1);
    assert(deepOTMPut < 0.001);
    
    // Zero volatility case: option value reduces to discounted intrinsic or zero
    double zeroVolCall = blackScholesPrice(150.0, 100.0, 0.05, 0.0, 1.0, 0);
    double expectedZeroVolCall = 150.0 - 100.0 * std::exp(-0.05 * 1.0);
    assert(std::abs(zeroVolCall - expectedZeroVolCall) < 1e-6);
    
    return 0;
}
