#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Document.hpp"

class InvertedIndex {
public:
    // Structure to store document frequency information
    struct PostingInfo {
        std::string doc_id;
        size_t term_frequency;
        
        PostingInfo(const std::string& id, size_t freq) 
            : doc_id(id), term_frequency(freq) {}
    };
    
    // Add a document to the index
    void addDocument(const std::shared_ptr<Document>& doc);
    
    // Get posting list for a term
    const std::vector<PostingInfo>& getPostings(const std::string& term) const;
    
    // Get document frequency (number of documents containing the term)
    size_t getDocumentFrequency(const std::string& term) const;
    
    // Get total number of documents in the index
    size_t getTotalDocuments() const { return total_documents_; }
    
private:
    // Map from term to its posting list
    std::unordered_map<std::string, std::vector<PostingInfo>> index_;
    size_t total_documents_{0};
};

#endif // INVERTED_INDEX_HPP 