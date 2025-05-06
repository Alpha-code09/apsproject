#include "SpellCorrector.hpp"
#include <algorithm>

void SpellCorrector::addWord(const std::string& word) {
    dictionary_.insert(word);
}

size_t SpellCorrector::levenshteinDistance(const std::string& s1, const std::string& s2) const {
    const size_t m = s1.length();
    const size_t n = s2.length();
    
    std::vector<std::vector<size_t>> dp(m + 1, std::vector<size_t>(n + 1));
    
    // Initialize first row and column
    for (size_t i = 0; i <= m; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= n; ++j) dp[0][j] = j;
    
    // Fill the matrix
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = 1 + std::min({dp[i-1][j],      // deletion
                                       dp[i][j-1],      // insertion
                                       dp[i-1][j-1]});  // substitution
            }
        }
    }
    
    return dp[m][n];
}

std::vector<std::string> SpellCorrector::generateCandidates(const std::string& word) const {
    std::vector<std::string> candidates;
    
    // Check dictionary words within max_distance_
    for (const auto& dict_word : dictionary_) {
        size_t distance = levenshteinDistance(word, dict_word);
        if (distance <= max_distance_) {
            candidates.push_back(dict_word);
        }
    }
    
    // Sort by edit distance
    std::sort(candidates.begin(), candidates.end(),
              [this, &word](const std::string& a, const std::string& b) {
                  return levenshteinDistance(word, a) < levenshteinDistance(word, b);
              });
    
    return candidates;
}

std::vector<std::string> SpellCorrector::getSuggestions(const std::string& word) const {
    // If word exists in dictionary, return empty suggestions
    if (dictionary_.find(word) != dictionary_.end()) {
        return {};
    }
    
    return generateCandidates(word);
} 