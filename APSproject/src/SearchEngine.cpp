#include "SearchEngine.hpp"
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <fstream>

SearchEngine::SearchEngine() 
    : tfidf_calculator_(std::make_unique<TFIDFCalculator>(index_)),
      compressor_(std::make_unique<HuffmanCompression>()),
      autocomplete_trie_(std::make_unique<Trie>()),
      spell_corrector_(std::make_unique<SpellCorrector>()) {}

void SearchEngine::addDocument(const std::string& id, const std::string& path) {
    auto doc = std::make_shared<Document>(id, path);
    if (doc->parse()) {
        documents_.push_back(doc);
        index_.addDocument(doc);
        
        // Update autocomplete and spell correction with document words
        const auto& frequencies = doc->getWordFrequencies();
        for (const auto& [word, _] : frequencies) {
            updateSearchHelpers(word);
        }
    }
    else {
        throw std::runtime_error("Failed to parse document: " + path);
    }
}

void SearchEngine::updateSearchHelpers(const std::string& word) {
    autocomplete_trie_->insert(word);
    spell_corrector_->addWord(word);
}

std::vector<std::pair<std::string, double>> SearchEngine::search(const std::string& query, size_t num_results) const {
    std::vector<std::string> query_terms = tokenizeQuery(query);
    std::unordered_map<std::string, double> doc_scores;
    
    // Calculate scores for each document
    for (const auto& doc : documents_) {
        double score = 0.0;
        for (const auto& term : query_terms) {
            score += tfidf_calculator_->calculateTFIDF(term, *doc);
        }
        if (score > 0.0) {
            doc_scores[doc->getId()] = score;
        }
    }
    
    // Convert to vector and sort by score
    std::vector<std::pair<std::string, double>> results;
    results.reserve(doc_scores.size());
    for (const auto& [doc_id, score] : doc_scores) {
        results.emplace_back(doc_id, score);
    }
    
    std::sort(results.begin(), results.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Return top N results
    if (results.size() > num_results) {
        results.resize(num_results);
    }
    
    // Update word frequencies in trie for better suggestions
    for (const auto& term : query_terms) {
        autocomplete_trie_->incrementFrequency(term);
    }
    
    return results;
}

std::vector<std::string> SearchEngine::getAutocompleteSuggestions(const std::string& prefix) const {
    return autocomplete_trie_->getSuggestions(prefix);
}

std::vector<std::string> SearchEngine::getSpellingSuggestions(const std::string& word) const {
    return spell_corrector_->getSuggestions(word);
}

bool SearchEngine::saveIndex(const std::string& filename) const {
    // Serialize index data
    std::stringstream ss;
    
    // Save document count
    size_t doc_count = documents_.size();
    ss.write(reinterpret_cast<const char*>(&doc_count), sizeof(doc_count));
    
    // Save documents
    for (const auto& doc : documents_) {
        const auto& id = doc->getId();
        const auto& path = doc->getPath();
        
        size_t id_length = id.length();
        size_t path_length = path.length();
        
        ss.write(reinterpret_cast<const char*>(&id_length), sizeof(id_length));
        ss.write(id.c_str(), id_length);
        ss.write(reinterpret_cast<const char*>(&path_length), sizeof(path_length));
        ss.write(path.c_str(), path_length);
    }
    
    // Compress and save
    std::string data = ss.str();
    auto compressed = compressor_->compress(data);
    return compressor_->saveToFile(filename, compressed);
}

bool SearchEngine::loadIndex(const std::string& filename) {
    auto compressed = compressor_->loadFromFile(filename);
    if (compressed.empty()) return false;
    
    std::string data = compressor_->decompress(compressed);
    std::stringstream ss(data);
    
    // Clear existing data
    documents_.clear();
    index_ = InvertedIndex();
    autocomplete_trie_->clear();
    spell_corrector_->clear();
    
    // Load document count
    size_t doc_count;
    ss.read(reinterpret_cast<char*>(&doc_count), sizeof(doc_count));
    
    // Load documents
    for (size_t i = 0; i < doc_count; ++i) {
        size_t id_length, path_length;
        ss.read(reinterpret_cast<char*>(&id_length), sizeof(id_length));
        
        std::string id(id_length, '\0');
        ss.read(&id[0], id_length);
        
        ss.read(reinterpret_cast<char*>(&path_length), sizeof(path_length));
        std::string path(path_length, '\0');
        ss.read(&path[0], path_length);
        
        // Add document
        try {
            addDocument(id, path);
        }
        catch (const std::exception&) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> SearchEngine::tokenizeQuery(const std::string& query) const {
    std::vector<std::string> tokens;
    std::istringstream iss(query);
    std::string token;
    
    while (iss >> token) {
        // Convert to lowercase and remove punctuation
        std::string processed;
        processed.reserve(token.length());
        
        for (char c : token) {
            if (std::isalnum(c)) {
                processed += std::tolower(c);
            }
        }
        
        if (!processed.empty()) {
            tokens.push_back(processed);
        }
    }
    
    return tokens;
} 