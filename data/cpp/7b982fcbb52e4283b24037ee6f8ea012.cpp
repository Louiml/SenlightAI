/*
Given `n` pairs of integers, where each pair represents a product's price and its category ID, write a C++ function `long long countSpecialPairs(const std::vector<std::pair<long long, long long>>& products)` that returns the number of unordered pairs of products `(i, j)` with `i < j` such that the price of product `i` is strictly less than the price of product `j`, and the category ID of product `i` is strictly greater than the category ID of product `j`. Note that each category may contain multiple products with distinct prices; if a price appears multiple times in a category, treat them as separate products. The function should handle up to `n ≤ 300000` products, with prices and category IDs up to `10^9`. The result fits in a 64-bit signed integer.
*/

#include <bits/stdc++.h>

// Return count of pairs (i,j) such that price[i] < price[j] and category[i] > category[j].
long long countSpecialPairs(const std::vector<std::pair<long long, long long>>& products) {
    int n = static_cast<int>(products.size());
    if (n < 2) return 0;

    // Coordinate compression of prices.
    std::vector<long long> allPrices;
    allPrices.reserve(n);
    for (const auto& p : products) allPrices.push_back(p.first);
    std::sort(allPrices.begin(), allPrices.end());
    allPrices.erase(std::unique(allPrices.begin(), allPrices.end()), allPrices.end());
    int m = static_cast<int>(allPrices.size());

    // Group products by category, each category holds a list of prices.
    std::map<long long, std::vector<long long>> categories;
    for (const auto& p : products) {
        categories[p.second].push_back(p.first);
    }

    // Collect category IDs and sort them in descending order.
    std::vector<long long> catIds;
    for (const auto& entry : categories) catIds.push_back(entry.first);
    std::sort(catIds.rbegin(), catIds.rend());

    // Fenwick tree for counting inserted prices.
    std::vector<long long> bit(m + 2, 0);
    auto bit_update = [&](int idx, long long delta) {
        while (idx <= m) {
            bit[idx] += delta;
            idx += idx & (-idx);
        }
    };
    auto bit_query = [&](int idx) {
        long long sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= idx & (-idx);
        }
        return sum;
    };

    long long answer = 0;
    for (long long cat : catIds) {
        const auto& prices = categories[cat];
        // Sort prices within the category.
        std::vector<long long> sortedPrices = prices; // already not necessarily sorted
        std::sort(sortedPrices.begin(), sortedPrices.end());

        // For each price, count how many inserted prices (from larger categories) are strictly smaller.
        for (long long price : sortedPrices) {
            int pos = static_cast<int>(std::lower_bound(allPrices.begin(), allPrices.end(), price) - allPrices.begin()) + 1;
            // Query count of prices < price, i.e., positions 1..pos-1.
            answer += bit_query(pos - 1);
        }

        // Now insert this category's prices into BIT.
        for (long long price : sortedPrices) {
            int pos = static_cast<int>(std::lower_bound(allPrices.begin(), allPrices.end(), price) - allPrices.begin()) + 1;
            bit_update(pos, 1);
        }
    }

    return answer;
}

#include <bits/stdc++.h>
#include <cassert>

// Include the solution function here or link it.
// For testing, we paste the function above.

int main() {
    // Test 1: Basic example
    std::vector<std::pair<long long, long long>> products1 = {{1,2}, {3,1}};
    assert(countSpecialPairs(products1) == 1);

    // Test 2: Reverse order
    std::vector<std::pair<long long, long long>> products2 = {{2,1}, {1,2}};
    assert(countSpecialPairs(products2) == 1);

    // Test 3: Duplicate prices in same category
    std::vector<std::pair<long long, long long>> products3 = {{1,2}, {1,2}, {3,1}};
    assert(countSpecialPairs(products3) == 2);

    // Test 4: No valid pairs (all categories same)
    std::vector<std::pair<long long, long long>> products4 = {{1,1}, {2,1}, {3,1}};
    assert(countSpecialPairs(products4) == 0);

    // Test 5: Larger random set
    std::vector<std::pair<long long, long long>> products5 = {{5,3}, {2,2}, {8,4}, {1,1}, {4,2}};
    // Let's manually compute: categories: 4 has 8; 3 has 5; 2 has 2,4; 1 has 1.
    // Process descending: cat4: price8, query empty =>0, insert 8.
    // cat3: price5, query bit_query(idx of 5) -1: prices sorted [1,2,4,5,8] so idx for 5=4, bit_query(3) = count <5 = 0? Actually after inserting 8 only, count <5 is 0. so 0, insert 5.
    // cat2: prices 2 and 4. Sort: [2,4]. For price2: query count <2 = 0, insert? wait we are still processing cat2, we haven't inserted cat2 yet. So current BIT has {8,5}. For price2: count <2 = 0. For price4: count <4 = 0 (since 2 not inserted yet). So sum=0. Then insert 2 and 4.
    // cat1: price1: query count <1 = 0. total answer=0. But wait, are there valid pairs? Let's check manually: Need price[i] < price[j] and category[i] > category[j]. For (price 2, cat2) and (price 4, cat2) same category not allowed. Between cat4 (price8) and cat2 (price2) => price8 >2, but we need price[i] < price[j], so 2<8 but category 2 <4? Actually category for price8 is 4, for price2 is 2, so category[8]=4 > category[2]=2, price[8]=8 >2, so price condition fails (need price[largerCat] < price[smallerCat]). So none. Similarly all others. So answer 0. Correct.

    assert(countSpecialPairs(products5) == 0);

    // Test 6: Example with valid pairs
    std::vector<std::pair<long long, long long>> products6 = {{2,2}, {3,3}, {1,1}};
    // Categories: 3 has 3; 2 has 2; 1 has 1.
    // Process cat3: price3, query empty =>0, insert 3.
    // cat2: price2, query count <2 = 0, insert 2.
    // cat1: price1, query count <1 = 0. answer 0. But actually pair (1,3) and (2,3) etc? Check: (price1 cat1) and (price2 cat2): price1<2 and cat1<2? Need cat[i] > cat[j], so if i is cat2 (price2) and j is cat1 (price1), price2>1 but need price[i]<price[j] -> 2<1 false. So none. So 0. Correct.

    assert(countSpecialPairs(products6) == 0);

    // Test 7: More complex with valid pairs
    std::vector<std::pair<long long, long long>> products7 = {{1,3}, {2,2}, {3,1}};
    // Categories: 3 has 1; 2 has 2; 1 has 3.
    // Process cat3: price1, query empty =>0, insert 1.
    // cat2: price2, query count <2 = 1 (the 1), answer +=1, insert 2.
    // cat1: price3, query count <3 = 2 (1 and 2), answer +=2 => total 3.
    // Manual pairs: (cat3 price1, cat2 price2): valid? price1<2, cat3>cat2 => yes. (cat3 price1, cat1 price3): valid. (cat2 price2, cat1 price3): valid. That's 3. Correct.

    assert(countSpecialPairs(products7) == 3);

    // Test 8: Duplicate prices across categories
    std::vector<std::pair<long long, long long>> products8 = {{5,2}, {1,2}, {5,1}, {1,1}};
    // Categories: 2 has prices 5,1; 1 has prices 5,1.
    // Process cat2: sort [1,5]; query count <1 =0, count <5 =0 (empty); insert 1,5.
    // cat1: sort [1,5]; for price1: query count <1 =0; for price5: query count <5 =1 (the 1 from cat2) => answer +=1. Total 1.
    // Manual: pairs where price[i]<price[j] and cat[i]>cat[j]. Consider (cat2 price1, cat1 price5): valid. (cat2 price1, cat1 price1): price equal not valid. (cat2 price5, cat1 price5): equal not valid. (cat2 price5, cat1 price1): price5>1 not valid. (cat2 price5, cat1 price5) no. So only 1. Correct.

    assert(countSpecialPairs(products8) == 1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// This problem is a classic "inversion count" variant but with a twist: we need to count pairs where `price[i] < price[j]` and `category[i] > category[j]` for `i < j`. A direct O(n²) comparison is too slow.
//
// The key observation: If we sort products by category in descending order and process categories from largest to smallest, then when processing a category `c`, any previously processed categories have IDs larger than `c`. So for a product `p` in category `c`, any previously added product `q` (from a larger category) will satisfy `category[q] > category[p]`. We now need to count among those how many have `price[q] < price[p]`.
//
// To do this efficiently, we use a Fenwick (Binary Indexed) Tree over compressed price values. We process categories in descending order. For each category, we first insert all its product prices into the BIT (so that they become available for later (smaller) categories), then for each product in the current category except the first (after sorting the category's prices ascending), we query the BIT to count how many inserted prices are strictly less than the current price and strictly greater than the previous price in the same category. This counts valid pairs where the earlier product is from a larger category and has a price between the two consecutive prices in the current category. Summing these over all categories gives the answer.
//
// **Edge cases:**  
// - Duplicate prices within a category: They are treated as separate products, but when counting pairs, if two products have the same price, they cannot form a pair because `price[i] < price[j]` would be false. Sorting the category's prices and using distinct indices handles this because we only count between consecutive distinct or same values? Actually the formula counts pairs where the earlier product's price is between previous and current price, but if there are duplicates, the range may be empty. We must be careful: For each product in a category (except the first after sorting), we count inserted products with price in `(prevPrice, currentPrice]`? Let’s align with the original code: It uses `getSum(fin, ind) - getSum(fin, prev)` which counts prices with index in `(prev, ind]` (since BIT uses 0-indexed and `getSum` returns sum up to index inclusive). Actually `ind` and `prev` are lower_bound indices in the sorted unique price array. The range `(prev, ind]` includes prices strictly greater than previous and less than or equal to current. Since prices are distinct within a category (they are stored as given, but two products could have same price? The original code inserts all prices and then uses `prev` and `ind` as positions of consecutive sorted prices in the category. If they are equal, `ind == prev` and the difference is zero, so no contribution. So duplicates within a category don't produce pairs because the interval is empty. For pairs between categories, duplicates across categories are fine because we count all inserted prices in that range.
//
// **Complexity:**  
// - Sorting products by category and within each category by price: O(n log n)  
// - Coordinate compression of all distinct prices: O(n log n)  
// - Processing each product once: each insert and each query O(log n)  
// - Total time: O(n log n)  
// - Space: O(n)
//
// The solution uses a Fenwick tree of size `m` (number of distinct prices) to store counts.
