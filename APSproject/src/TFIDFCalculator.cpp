#include "TFIDFCalculator.hpp"

double TFIDFCalculator::calculateTF(const std::string& term, const Document& doc) const {
    const auto& frequencies = doc.getWordFrequencies();
    auto it = frequencies.find(term);
    if (it == frequencies.end()) {
        return 0.0;
    }
    
    // Using logarithmic TF to prevent bias towards longer documents
    return 1.0 + std::log(static_cast<double>(it->second));
}

double TFIDFCalculator::calculateIDF(const std::string& term) const {
    size_t doc_freq = index_.getDocumentFrequency(term);
    if (doc_freq == 0) {
        return 0.0;
    }
    
    // Calculate IDF using the standard formula: log(N/df)
    double total_docs = static_cast<double>(index_.getTotalDocuments());
    return std::log(total_docs / static_cast<double>(doc_freq));
}

double TFIDFCalculator::calculateTFIDF(const std::string& term, const Document& doc) const {
    double tf = calculateTF(term, doc);
    double idf = calculateIDF(term);
    return tf * idf;
} 