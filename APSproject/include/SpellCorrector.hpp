#ifndef SPELL_CORRECTOR_HPP
#define SPELL_CORRECTOR_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

class SpellCorrector {
public:
    // Constructor with maximum edit distance
    explicit SpellCorrector(size_t max_distance = 2) : max_distance_(max_distance) {}
    
    // Add a word to the dictionary
    void addWord(const std::string& word);
    
    // Get suggestions for a potentially misspelled word
    std::vector<std::string> getSuggestions(const std::string& word) const;
    
    // Clear the dictionary
    void clear() { dictionary_.clear(); }
    
private:
    size_t max_distance_;
    std::unordered_set<std::string> dictionary_;
    
    // Calculate Levenshtein distance between two strings
    size_t levenshteinDistance(const std::string& s1, const std::string& s2) const;
    
    // Generate possible corrections within max_distance_
    std::vector<std::string> generateCandidates(const std::string& word) const;
};

#endif // SPELL_CORRECTOR_HPP 