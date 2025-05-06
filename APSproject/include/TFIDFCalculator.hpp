#ifndef TFIDF_CALCULATOR_HPP
#define TFIDF_CALCULATOR_HPP

#include <cmath>
#include "InvertedIndex.hpp"
#include "Document.hpp"

class TFIDFCalculator {
public:
    TFIDFCalculator(const InvertedIndex& index) : index_(index) {}
    
    // Calculate TF (term frequency) for a term in a document
    double calculateTF(const std::string& term, const Document& doc) const;
    
    // Calculate IDF (inverse document frequency) for a term
    double calculateIDF(const std::string& term) const;
    
    // Calculate TF-IDF score for a term in a document
    double calculateTFIDF(const std::string& term, const Document& doc) const;
    
private:
    const InvertedIndex& index_;
};

#endif // TFIDF_CALCULATOR_HPP 