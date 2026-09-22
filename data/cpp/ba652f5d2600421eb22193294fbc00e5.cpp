Write a C++ function that accepts a string containing a valid IPv4 address in dotted-decimal notation (e.g., "192.168.1.1") and returns a string representing its IPv6-mapped IPv4 address in the form `::FFFF:xxxx:xxxx`, where each `xxxx` is a 4-digit lowercase hexadecimal group formed by concatenating two octets. If the IPv4 address starts with octet 127 (loopback), the function must return the special IPv6 loopback address `::1`. The function must validate the input: exactly four octets separated by dots, each octet a non-negative integer between 0 and 255 (leading zeros allowed, but no extra characters, spaces, or missing octets). On invalid input, return the string `"Invalid input"`. Do not print to standard output; return the result.
#include <cassert>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Basic valid addresses
    assert(convertToIpv6("192.168.1.1") == "::FFFF:c0a8:0101");
    assert(convertToIpv6("0.0.0.0") == "::FFFF:0000:0000");
    assert(convertToIpv6("255.255.255.255") == "::FFFF:ffff:ffff");
    assert(convertToIpv6("10.20.30.40") == "::FFFF:0a14:1e28");

    // Loopback
    assert(convertToIpv6("127.0.0.1") == "::1");
    assert(convertToIpv6("127.255.255.255") == "::1");

    // Invalid inputs
    assert(convertToIpv6("") == "Invalid input");
    assert(convertToIpv6("1.2.3") == "Invalid input");
    assert(convertToIpv6("1.2.3.4.5") == "Invalid input");
    assert(convertToIpv6("256.1.1.1") == "Invalid input");
    assert(convertToIpv6("1.2.3.abc") == "Invalid input");
    assert(convertToIpv6("1..3.4") == "Invalid input");
    assert(convertToIpv6(" 1.2.3.4") == "Invalid input");
}
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>

// Converts a dotted-decimal IPv4 string to an IPv6-mapped address.
// Returns "::1" for loopback, "::FFFF:xxxx:xxxx" otherwise, or "Invalid input".
std::string convertToIpv6(const std::string& ipv4address) {
    std::stringstream ss(ipv4address);
    std::string octet;
    int octets[4];
    int count = 0;

    // Parse and validate each octet.
    while (std::getline(ss, octet, '.')) {
        if (count >= 4) {
            return "Invalid input";
        }
        // Ensure the token is non-empty and consists only of digits.
        if (octet.empty()) {
            return "Invalid input";
        }
        for (char c : octet) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                return "Invalid input";
            }
        }
        int value = std::stoi(octet);
        if (value < 0 || value > 255) {
            return "Invalid input";
        }
        octets[count++] = value;
    }

    // Must be exactly four octets.
    if (count != 4) {
        return "Invalid input";
    }

    // Loopback special case.
    if (octets[0] == 127) {
        return "::1";
    }

    // Build the IPv6-mapped address.
    std::ostringstream out;
    out << "::FFFF:";
    out << std::hex << std::nouppercase << std::setw(4) << std::setfill('0');
    out << ((octets[0] << 8) | octets[1]) << ":";
    out << std::setw(4) << std::setfill('0');
    out << ((octets[2] << 8) | octets[3]);
    return out.str();
}
// The solution parses the input string by splitting on the '.' character using a string stream and `getline`. For each extracted token, convert to an integer with `stoi`, and immediately validate it is non-negative and ≤255; also track the count of octets, rejecting if more than four tokens appear. After parsing, ensure exactly four octets were found. If the first octet is 127, return `"::1"`. Otherwise, build the result string `"::FFFF:"` and append two hex groups: first combine octets[0] and octets[1] into a 16-bit integer formed as `(octets[0] << 8) | octets[1]`, and second combine octets[2] and octets[3] similarly. Use `std::ostringstream` with `std::hex`, `std::setw(4)`, `std::setfill('0')`, and `std::nouppercase` (or manually lowercase) to format each group as four lowercase hex digits. Time complexity is O(1) since input length is bounded by a standard IPv4 address (max ~15 characters), and space complexity is O(1) for auxiliary storage.
