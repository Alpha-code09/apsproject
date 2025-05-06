#include <gtest/gtest.h>
#include "SearchEngine.hpp"
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

class SearchEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files with different content
        createTestFile("test_doc1.txt", 
            "This is a test document about machine learning and artificial intelligence.");
        
        createTestFile("test_doc2.txt", 
            "Deep learning models have transformed the field of AI. Neural networks are powerful.");
        
        createTestFile("test_doc3.txt", 
            "Natural language processing helps computers understand human language.");
        
        // Create a large test file
        std::ofstream large_file("large_doc.txt");
        for(int i = 0; i < 1000; i++) {
            large_file << "This is line " << i << " containing some repeated text about machine learning ";
            large_file << "and artificial intelligence to test performance with larger documents.\n";
        }
        large_file.close();
    }

    void TearDown() override {
        // Clean up test files
        std::remove("test_doc1.txt");
        std::remove("test_doc2.txt");
        std::remove("test_doc3.txt");
        std::remove("large_doc.txt");
        std::remove("test_index.bin");
    }

    void createTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        file << content;
        file.close();
    }

    SearchEngine engine;
};

// Basic Functionality Tests
TEST_F(SearchEngineTest, AddDocument) {
    EXPECT_NO_THROW(engine.addDocument("doc1", "test_doc1.txt"));
    EXPECT_EQ(engine.getDocumentCount(), 1);
}

TEST_F(SearchEngineTest, AddMultipleDocuments) {
    EXPECT_NO_THROW({
        engine.addDocument("doc1", "test_doc1.txt");
        engine.addDocument("doc2", "test_doc2.txt");
        engine.addDocument("doc3", "test_doc3.txt");
    });
    EXPECT_EQ(engine.getDocumentCount(), 3);
}

TEST_F(SearchEngineTest, DuplicateDocumentId) {
    engine.addDocument("doc1", "test_doc1.txt");
    EXPECT_THROW(engine.addDocument("doc1", "test_doc2.txt"), std::exception);
}

// Search Functionality Tests
TEST_F(SearchEngineTest, BasicSearch) {
    engine.addDocument("doc1", "test_doc1.txt");
    auto results = engine.search("machine learning");
    EXPECT_FALSE(results.empty());
    EXPECT_EQ(results[0].first, "doc1");
}

TEST_F(SearchEngineTest, MultiDocumentSearch) {
    engine.addDocument("doc1", "test_doc1.txt");
    engine.addDocument("doc2", "test_doc2.txt");
    auto results = engine.search("neural networks");
    EXPECT_FALSE(results.empty());
    EXPECT_EQ(results[0].first, "doc2");
}

TEST_F(SearchEngineTest, SearchRelevanceOrder) {
    engine.addDocument("doc1", "test_doc1.txt");
    engine.addDocument("doc2", "test_doc2.txt");
    engine.addDocument("doc3", "test_doc3.txt");
    
    auto results = engine.search("artificial intelligence");
    EXPECT_GE(results.size(), 2);
    // Doc1 should be more relevant as it contains both terms
    EXPECT_EQ(results[0].first, "doc1");
}

// Autocomplete Tests
TEST_F(SearchEngineTest, BasicAutocomplete) {
    engine.addDocument("doc1", "test_doc1.txt");
    auto suggestions = engine.getAutocompleteSuggestions("mach");
    EXPECT_FALSE(suggestions.empty());
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "machine") != suggestions.end());
}

TEST_F(SearchEngineTest, AutocompleteMultipleMatches) {
    engine.addDocument("doc1", "test_doc1.txt");
    engine.addDocument("doc2", "test_doc2.txt");
    auto suggestions = engine.getAutocompleteSuggestions("ne");
    EXPECT_GE(suggestions.size(), 2);
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "neural") != suggestions.end());
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "networks") != suggestions.end());
}

// Spell Checking Tests
TEST_F(SearchEngineTest, BasicSpellCheck) {
    engine.addDocument("doc1", "test_doc1.txt");
    auto suggestions = engine.getSpellingSuggestions("machne");
    EXPECT_FALSE(suggestions.empty());
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "machine") != suggestions.end());
}

TEST_F(SearchEngineTest, SpellCheckMultipleSuggestions) {
    engine.addDocument("doc2", "test_doc2.txt");
    auto suggestions = engine.getSpellingSuggestions("neurall");
    EXPECT_FALSE(suggestions.empty());
    EXPECT_TRUE(std::find(suggestions.begin(), suggestions.end(), "neural") != suggestions.end());
}

// Performance Tests
TEST_F(SearchEngineTest, LargeDocumentHandling) {
    EXPECT_NO_THROW(engine.addDocument("large", "large_doc.txt"));
    auto start = std::chrono::high_resolution_clock::now();
    auto results = engine.search("machine learning");
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_FALSE(results.empty());
    EXPECT_LT(duration.count(), 1000); // Search should complete within 1 second
}

// Edge Cases
TEST_F(SearchEngineTest, EmptyQuery) {
    engine.addDocument("doc1", "test_doc1.txt");
    auto results = engine.search("");
    EXPECT_TRUE(results.empty());
}

TEST_F(SearchEngineTest, NonExistentDocument) {
    EXPECT_THROW(engine.addDocument("invalid", "nonexistent.txt"), std::exception);
}

TEST_F(SearchEngineTest, EmptyDocument) {
    createTestFile("empty.txt", "");
    EXPECT_NO_THROW(engine.addDocument("empty", "empty.txt"));
    auto results = engine.search("any query");
    EXPECT_TRUE(results.empty());
    std::remove("empty.txt");
}

// Save/Load Tests
TEST_F(SearchEngineTest, SaveAndLoadIndex) {
    engine.addDocument("doc1", "test_doc1.txt");
    engine.addDocument("doc2", "test_doc2.txt");
    
    EXPECT_TRUE(engine.saveIndex("test_index.bin"));
    
    SearchEngine new_engine;
    EXPECT_TRUE(new_engine.loadIndex("test_index.bin"));
    EXPECT_EQ(new_engine.getDocumentCount(), 2);
    
    auto results1 = engine.search("machine learning");
    auto results2 = new_engine.search("machine learning");
    EXPECT_EQ(results1.size(), results2.size());
}

TEST_F(SearchEngineTest, LoadNonexistentIndex) {
    EXPECT_FALSE(engine.loadIndex("nonexistent_index.bin"));
}

// Concurrent Access Tests
TEST_F(SearchEngineTest, ConcurrentSearches) {
    engine.addDocument("doc1", "test_doc1.txt");
    engine.addDocument("doc2", "test_doc2.txt");
    
    auto search_func = [&]() {
        for(int i = 0; i < 100; i++) {
            auto results = engine.search("machine learning");
            EXPECT_FALSE(results.empty());
        }
    };
    
    std::thread t1(search_func);
    std::thread t2(search_func);
    
    t1.join();
    t2.join();
} 