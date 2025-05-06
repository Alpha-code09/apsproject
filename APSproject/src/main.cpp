#include <iostream>
#include <string>
#include <filesystem>
#include "SearchEngine.hpp"

void printUsage() {
    std::cout << "Available commands:\n"
              << "  add <path>     - Add a document to the search engine\n"
              << "  search <query> - Search for documents\n"
              << "  complete <prefix> - Get autocomplete suggestions\n"
              << "  spell <word>   - Get spelling suggestions\n"
              << "  save <file>    - Save index to file\n"
              << "  load <file>    - Load index from file\n"
              << "  quit          - Exit the program\n";
}

int main() {
    SearchEngine engine;
    std::string command;
    
    std::cout << "Enhanced Search Engine\n"
              << "--------------------\n";
    printUsage();
    
    while (true) {
        std::cout << "\nEnter command: ";
        std::getline(std::cin, command);
        
        if (command.empty()) continue;
        
        if (command == "quit") {
            break;
        }
        else if (command.substr(0, 4) == "add ") {
            std::string path = command.substr(4);
            try {
                std::filesystem::path fs_path(path);
                if (!std::filesystem::exists(fs_path)) {
                    std::cout << "Error: File does not exist\n";
                    continue;
                }
                engine.addDocument(fs_path.filename().string(), path);
                std::cout << "Document added successfully\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error adding document: " << e.what() << "\n";
            }
        }
        else if (command.substr(0, 7) == "search ") {
            std::string query = command.substr(7);
            try {
                auto results = engine.search(query);
                if (results.empty()) {
                    std::cout << "No results found.\n";
                    
                    // Show spelling suggestions for each word
                    std::istringstream iss(query);
                    std::string word;
                    while (iss >> word) {
                        auto suggestions = engine.getSpellingSuggestions(word);
                        if (!suggestions.empty()) {
                            std::cout << "Did you mean '" << word << "'? Suggestions:\n";
                            for (const auto& suggestion : suggestions) {
                                std::cout << "  - " << suggestion << "\n";
                            }
                        }
                    }
                }
                else {
                    std::cout << "\nSearch results:\n";
                    for (const auto& [doc_id, score] : results) {
                        std::cout << doc_id << " (score: " << score << ")\n";
                    }
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error during search: " << e.what() << "\n";
            }
        }
        else if (command.substr(0, 9) == "complete ") {
            std::string prefix = command.substr(9);
            try {
                auto suggestions = engine.getAutocompleteSuggestions(prefix);
                if (suggestions.empty()) {
                    std::cout << "No suggestions found.\n";
                }
                else {
                    std::cout << "Autocomplete suggestions:\n";
                    for (const auto& suggestion : suggestions) {
                        std::cout << "  - " << suggestion << "\n";
                    }
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error getting suggestions: " << e.what() << "\n";
            }
        }
        else if (command.substr(0, 6) == "spell ") {
            std::string word = command.substr(6);
            try {
                auto suggestions = engine.getSpellingSuggestions(word);
                if (suggestions.empty()) {
                    std::cout << "No spelling suggestions found.\n";
                }
                else {
                    std::cout << "Spelling suggestions:\n";
                    for (const auto& suggestion : suggestions) {
                        std::cout << "  - " << suggestion << "\n";
                    }
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error getting spelling suggestions: " << e.what() << "\n";
            }
        }
        else if (command.substr(0, 5) == "save ") {
            std::string filename = command.substr(5);
            if (engine.saveIndex(filename)) {
                std::cout << "Index saved successfully.\n";
            }
            else {
                std::cout << "Error saving index.\n";
            }
        }
        else if (command.substr(0, 5) == "load ") {
            std::string filename = command.substr(5);
            if (engine.loadIndex(filename)) {
                std::cout << "Index loaded successfully.\n";
            }
            else {
                std::cout << "Error loading index.\n";
            }
        }
        else {
            std::cout << "Unknown command\n";
            printUsage();
        }
    }
    
    return 0;
} 