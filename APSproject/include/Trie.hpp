#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Trie {
private:
    struct Node {
        bool is_end;
        std::unordered_map<char, std::shared_ptr<Node>> children;
        size_t frequency;  // Track word frequency for better suggestions
        
        Node() : is_end(false), frequency(0) {}
    };
    
    std::shared_ptr<Node> root_;
    size_t max_suggestions_;
    
    void findAllWords(const std::shared_ptr<Node>& node, const std::string& prefix,
                     std::vector<std::pair<std::string, size_t>>& result) const;
    
public:
    explicit Trie(size_t max_suggestions = 5) : root_(std::make_shared<Node>()), max_suggestions_(max_suggestions) {}
    
    // Insert a word into the trie
    void insert(const std::string& word);
    
    // Get autocomplete suggestions for a prefix
    std::vector<std::string> getSuggestions(const std::string& prefix) const;
    
    // Check if a word exists in the trie
    bool contains(const std::string& word) const;
    
    // Increment frequency of a word
    void incrementFrequency(const std::string& word);
    
    // Clear all entries
    void clear();
};

#endif // TRIE_HPP 