#include "Trie.hpp"
#include <algorithm>
#include <queue>

void Trie::insert(const std::string& word) {
    auto current = root_;
    for (char c : word) {
        if (!current->children[c]) {
            current->children[c] = std::make_shared<Node>();
        }
        current = current->children[c];
    }
    current->is_end = true;
    current->frequency++;
}

bool Trie::contains(const std::string& word) const {
    auto current = root_;
    for (char c : word) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second;
    }
    return current->is_end;
}

void Trie::incrementFrequency(const std::string& word) {
    auto current = root_;
    for (char c : word) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return;
        }
        current = it->second;
    }
    if (current->is_end) {
        current->frequency++;
    }
}

void Trie::findAllWords(const std::shared_ptr<Node>& node, const std::string& prefix,
                       std::vector<std::pair<std::string, size_t>>& result) const {
    if (!node) return;
    
    if (node->is_end) {
        result.emplace_back(prefix, node->frequency);
    }
    
    for (const auto& [c, child] : node->children) {
        findAllWords(child, prefix + c, result);
    }
}

std::vector<std::string> Trie::getSuggestions(const std::string& prefix) const {
    auto current = root_;
    
    // Find the node corresponding to the prefix
    for (char c : prefix) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return {};
        }
        current = it->second;
    }
    
    // Find all words with this prefix
    std::vector<std::pair<std::string, size_t>> words;
    findAllWords(current, prefix, words);
    
    // Sort by frequency
    std::sort(words.begin(), words.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Extract top suggestions
    std::vector<std::string> suggestions;
    suggestions.reserve(std::min(words.size(), max_suggestions_));
    
    for (size_t i = 0; i < words.size() && i < max_suggestions_; ++i) {
        suggestions.push_back(words[i].first);
    }
    
    return suggestions;
}

void Trie::clear() {
    root_ = std::make_shared<Node>();
} 