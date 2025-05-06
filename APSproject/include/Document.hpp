#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <string>
#include <unordered_map>
#include <memory>

class Document {
public:
    Document(const std::string& id, const std::string& path);
    
    // Parse and process the document content
    bool parse();
    
    // Getters
    const std::string& getId() const { return id_; }
    const std::string& getPath() const { return path_; }
    const std::unordered_map<std::string, size_t>& getWordFrequencies() const { return word_frequencies_; }
    size_t getWordCount() const { return total_words_; }
    
private:
    std::string id_;                    // Unique document identifier
    std::string path_;                  // Path to the document file
    std::unordered_map<std::string, size_t> word_frequencies_;  // Word frequency map
    size_t total_words_;               // Total number of words in document
    
    // Helper function to process text
    void processWord(const std::string& word);
};

#endif // DOCUMENT_HPP 