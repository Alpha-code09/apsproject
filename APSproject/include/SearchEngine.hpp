#ifndef SEARCH_ENGINE_HPP
#define SEARCH_ENGINE_HPP

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "Document.hpp"
#include "InvertedIndex.hpp"
#include "TFIDFCalculator.hpp"
#include "HuffmanCompression.hpp"
#include "Trie.hpp"
#include "SpellCorrector.hpp"

class SearchEngine {
public:
    SearchEngine();
    
    // Add a document to the search engine
    void addDocument(const std::string& id, const std::string& path);
    
    // Search for documents matching the query
    std::vector<std::pair<std::string, double>> search(const std::string& query, size_t num_results = 10) const;
    
    // Get autocomplete suggestions
    std::vector<std::string> getAutocompleteSuggestions(const std::string& prefix) const;
    
    // Get spell correction suggestions
    std::vector<std::string> getSpellingSuggestions(const std::string& word) const;
    
    // Compress and save index to file
    bool saveIndex(const std::string& filename) const;
    
    // Load compressed index from file
    bool loadIndex(const std::string& filename);
    
    // Get total number of documents
    size_t getDocumentCount() const { return index_.getTotalDocuments(); }
    
private:
    InvertedIndex index_;
    std::unique_ptr<TFIDFCalculator> tfidf_calculator_;
    std::vector<std::shared_ptr<Document>> documents_;
    std::unique_ptr<HuffmanCompression> compressor_;
    std::unique_ptr<Trie> autocomplete_trie_;
    std::unique_ptr<SpellCorrector> spell_corrector_;
    
    // Helper function to tokenize query
    std::vector<std::string> tokenizeQuery(const std::string& query) const;
    
    // Helper function to update autocomplete and spell correction data
    void updateSearchHelpers(const std::string& word);
};

#endif // SEARCH_ENGINE_HPP 