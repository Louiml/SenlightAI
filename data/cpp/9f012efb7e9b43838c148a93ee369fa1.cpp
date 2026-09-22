// Write a C++ function named `identifySafeIngredients` that, given a vector of paired recipe lines where each line contains a list of ingredients followed by the word "contains" and a list of allergens, returns a map mapping each allergen (as a string) to a sorted set of possible ingredient names (as strings) that could be that allergen. The input is represented as a `std::vector<std::string>`, where each element is one recipe line in the format: `"ingredient1 ingredient2 ... (contains allergen1, allergen2, ...)"`. The function should parse each line, extract ingredients and allergens (ignoring the parentheses and comma), and for each allergen, compute the intersection of ingredient lists across all recipes that mention that allergen. The output map should contain every allergen that appears in at least one recipe, with the set of ingredients that are consistent with all occurrences. If an allergen appears in only one recipe, all its ingredients are possible. The order of allergens in the map does not matter, but each set must be sorted alphabetically. The function must handle lowercase letters only, and ingredient/allergen names consist solely of alphabetic characters.
// The main algorithm involves parsing each recipe line to separate ingredients and allergens. For each line, we split by whitespace, and we can identify the keyword "contains" to switch from ingredient collection to allergen collection. We must strip the leading `(` from the first allergen and the trailing `)` from the last allergen, and remove commas from allergen names. After parsing, we create a map from allergen to a set of recipe indices that mention it. Then for each allergen, we perform a set intersection of the ingredient lists from all those recipes. The intersection can be computed incrementally: initialize the candidate set with the first recipe's ingredients, then for each subsequent recipe, keep only elements that also appear in that recipe's ingredients. Edge cases include: an allergen appearing in only one recipe (the result is the full ingredient list of that recipe), an allergen appearing in multiple recipes with no common ingredients (the result is an empty set), and allergens that appear in all recipes (the result is the intersection of all). We also need to ensure that if a recipe mentions an allergen multiple times, we do not process it multiple times; using a set of recipe indices per allergen avoids redundancy. Time complexity: let `R` be number of recipes, `A` be number of distinct allergens, `I` be maximum number of ingredients per recipe, and `M` be the total number of allergen mentions across recipes. Parsing takes O(total characters). Building the allergen-to-recipe index takes O(M) if we use sets. For each allergen, intersecting `k` ingredient lists each of size at most `I` costs O(k*I) using an unordered_set for fast lookup, so total cost is O(R*I) per allergen worst-case, but overall it's O(sum over allergens of k*I) ≤ O(R*I*A) worst-case. Space complexity is O(R*I + A*I) for storing parsed data and output.
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

// Given recipe lines of the form "ingredient1 ingredient2 ... (contains allergen1, allergen2)"
// return a map from each allergen to a sorted set of possible ingredients that could be that allergen.
std::map<std::string, std::set<std::string>> identifySafeIngredients(const std::vector<std::string>& recipeLines) {
    // Parse recipes into per-recipe ingredient and allergen lists.
    std::vector<std::vector<std::string>> ingredientsPerRecipe;
    std::vector<std::vector<std::string>> allergensPerRecipe;

    for (const std::string& line : recipeLines) {
        std::istringstream stream(line);
        std::string word;
        bool inAllergens = false;
        std::vector<std::string> ingredients;
        std::vector<std::string> allergens;

        while (stream >> word) {
            if (word == "contains") {
                inAllergens = true;
                continue;
            }
            // Clean the word: remove parentheses and commas.
            std::string cleaned;
            for (char c : word) {
                if (std::isalpha(c)) {
                    cleaned.push_back(c);
                }
            }
            if (cleaned.empty()) {
                continue;
            }
            if (inAllergens) {
                allergens.push_back(cleaned);
            } else {
                ingredients.push_back(cleaned);
            }
        }
        ingredientsPerRecipe.push_back(ingredients);
        allergensPerRecipe.push_back(allergens);
    }

    // Map each allergen to a set of recipe indices that mention it.
    std::map<std::string, std::set<int>> allergenToRecipeIndices;
    for (size_t i = 0; i < allergensPerRecipe.size(); ++i) {
        for (const std::string& alerg : allergensPerRecipe[i]) {
            allergenToRecipeIndices[alerg].insert(static_cast<int>(i));
        }
    }

    // For each allergen, intersect ingredient lists from all relevant recipes.
    std::map<std::string, std::set<std::string>> result;

    for (const auto& entry : allergenToRecipeIndices) {
        const std::string& allergen = entry.first;
        const std::set<int>& recipeIds = entry.second;

        // Start with ingredients from the first recipe that mentions this allergen.
        int firstId = *recipeIds.begin();
        std::set<std::string> possible(ingredientsPerRecipe[firstId].begin(),
                                       ingredientsPerRecipe[firstId].end());

        // Intersect with each subsequent recipe.
        bool first = true;
        for (int rid : recipeIds) {
            if (first) {
                first = false;
                continue;
            }
            const std::vector<std::string>& ingList = ingredientsPerRecipe[rid];
            std::set<std::string> current(ingList.begin(), ingList.end());
            std::set<std::string> intersection;
            std::set_intersection(possible.begin(), possible.end(),
                                  current.begin(), current.end(),
                                  std::inserter(intersection, intersection.begin()));
            possible = intersection;
        }

        result[allergen] = possible;
    }

    return result;
}
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>

// The function to test is declared elsewhere; here we provide the main test harness.
std::map<std::string, std::set<std::string>> identifySafeIngredients(const std::vector<std::string>& recipeLines);

int main() {
    // Test case 1: Single recipe, allergen appears once, all ingredients possible.
    std::vector<std::string> recipes1 = {"mxmxvkd kfcds sqjhc nhms (contains dairy, fish)"};
    auto res1 = identifySafeIngredients(recipes1);
    assert(res1.size() == 2);
    assert(res1["dairy"] == std::set<std::string>({"kfcds", "mxmxvkd", "nhms", "sqjhc"}));
    assert(res1["fish"] == std::set<std::string>({"kfcds", "mxmxvkd", "nhms", "sqjhc"}));

    // Test case 2: Two recipes sharing an allergen, intersection of ingredients.
    std::vector<std::string> recipes2 = {
        "a b c (contains x)",
        "b c d (contains x)"
    };
    auto res2 = identifySafeIngredients(recipes2);
    assert(res2.size() == 1);
    assert(res2["x"] == std::set<std::string>({"b", "c"}));

    // Test case 3: Allergen with no common ingredients across three recipes.
    std::vector<std::string> recipes3 = {
        "a b (contains y)",
        "c d (contains y)",
        "e f (contains y)"
    };
    auto res3 = identifySafeIngredients(recipes3);
    assert(res3.size() == 1);
    assert(res3["y"].empty());

    // Test case 4: Multiple allergens, one appears in all recipes, one in only one.
    std::vector<std::string> recipes4 = {
        "apple banana (contains fruit, peel)",
        "apple cherry (contains fruit)",
        "banana date (contains peel)"
    };
    auto res4 = identifySafeIngredients(recipes4);
    assert(res4.size() == 2);
    assert(res4["fruit"] == std::set<std::string>({"apple"}));
    assert(res4["peel"] == std::set<std::string>({"apple", "banana"}));

    // Test case 5: Empty input.
    std::vector<std::string> recipes5;
    auto res5 = identifySafeIngredients(recipes5);
    assert(res5.empty());

    // Test case 6: Recipe with no allergens.
    std::vector<std::string> recipes6 = {"a b c"};
    auto res6 = identifySafeIngredients(recipes6);
    assert(res6.empty());

    return 0;
}
