#include "InvertedIndex.hpp"

void InvertedIndex::addDocument(const std::shared_ptr<Document>& doc) {
    // Get word frequencies from the document
    const auto& frequencies = doc->getWordFrequencies();
    
    // Add each word to the inverted index
    for (const auto& [word, frequency] : frequencies) {
        index_[word].emplace_back(doc->getId(), frequency);
    }
    
    total_documents_++;
}

const std::vector<InvertedIndex::PostingInfo>& InvertedIndex::getPostings(const std::string& term) const {
    static const std::vector<PostingInfo> empty_vector;
    auto it = index_.find(term);
    return (it != index_.end()) ? it->second : empty_vector;
}

size_t InvertedIndex::getDocumentFrequency(const std::string& term) const {
    auto it = index_.find(term);
    return (it != index_.end()) ? it->second.size() : 0;
} 