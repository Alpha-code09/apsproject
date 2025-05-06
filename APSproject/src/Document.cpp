#include "Document.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

Document::Document(const std::string& id, const std::string& path)
    : id_(id), path_(path), total_words_(0) {}

bool Document::parse() {
    std::ifstream file(path_);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        
        while (iss >> word) {
            processWord(word);
        }
    }
    
    return true;
}

void Document::processWord(const std::string& word) {
    // Convert word to lowercase and remove punctuation
    std::string processed;
    processed.reserve(word.length());
    
    for (char c : word) {
        if (std::isalnum(c)) {
            processed += std::tolower(c);
        }
    }
    
    if (!processed.empty()) {
        word_frequencies_[processed]++;
        total_words_++;
    }
} 