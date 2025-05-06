#include "HuffmanCompression.hpp"
#include <fstream>
#include <bitset>
#include <cstring>  // for memcpy

void HuffmanCompression::buildEncodingMap(const std::shared_ptr<Node>& node, const std::string& code) {
    if (!node) return;
    
    if (!node->left && !node->right) {
        encoding_map_[node->character] = code;
        return;
    }
    
    buildEncodingMap(node->left, code + "0");
    buildEncodingMap(node->right, code + "1");
}

std::vector<unsigned char> HuffmanCompression::compress(const std::string& data) {
    if (data.empty()) return {};
    
    // Count character frequencies
    std::unordered_map<char, size_t> frequencies;
    for (char c : data) {
        frequencies[c]++;
    }
    
    // Create priority queue for Huffman tree construction
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> pq;
    for (const auto& [c, freq] : frequencies) {
        pq.push(std::make_shared<Node>(c, freq));
    }
    
    // Build Huffman tree
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        pq.push(std::make_shared<Node>(left->frequency + right->frequency, left, right));
    }
    
    root_ = pq.top();
    encoding_map_.clear();
    buildEncodingMap(root_, "");
    
    // Compress data
    std::string binary_string;
    for (char c : data) {
        binary_string += encoding_map_[c];
    }
    
    // Convert binary string to bytes
    std::vector<unsigned char> compressed;
    compressed.reserve((binary_string.length() + 7) / 8 + sizeof(size_t));
    
    // Store original size for decompression
    size_t original_size = data.length();
    const unsigned char* size_bytes = reinterpret_cast<const unsigned char*>(&original_size);
    compressed.insert(compressed.end(), size_bytes, size_bytes + sizeof(size_t));
    
    // Convert binary string to bytes
    for (size_t i = 0; i < binary_string.length(); i += 8) {
        std::string byte = binary_string.substr(i, 8);
        byte.append(8 - byte.length(), '0');  // Pad with zeros if needed
        compressed.push_back(static_cast<unsigned char>(std::bitset<8>(byte).to_ulong()));
    }
    
    return compressed;
}

std::string HuffmanCompression::decompress(const std::vector<unsigned char>& compressed_data) {
    if (compressed_data.size() <= sizeof(size_t)) return "";
    
    // Extract original size
    size_t original_size;
    std::memcpy(&original_size, compressed_data.data(), sizeof(size_t));
    
    // Convert bytes back to binary string
    std::string binary_string;
    for (size_t i = sizeof(size_t); i < compressed_data.size(); ++i) {
        binary_string += std::bitset<8>(compressed_data[i]).to_string();
    }
    
    // Decompress using Huffman tree
    std::string decompressed;
    decompressed.reserve(original_size);
    
    std::shared_ptr<Node> current = root_;
    for (char bit : binary_string) {
        if (decompressed.length() >= original_size) break;
        
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (!current->left && !current->right) {
            decompressed += current->character;
            current = root_;
        }
    }
    
    return decompressed;
}

bool HuffmanCompression::saveToFile(const std::string& filename, const std::vector<unsigned char>& compressed_data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;
    
    file.write(reinterpret_cast<const char*>(compressed_data.data()), compressed_data.size());
    return true;
}

std::vector<unsigned char> HuffmanCompression::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) return {};
    
    size_t size = file.tellg();
    file.seekg(0);
    
    std::vector<unsigned char> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return data;
}

double HuffmanCompression::getCompressionRatio(const std::string& original, const std::vector<unsigned char>& compressed) {
    if (original.empty()) return 0.0;
    return static_cast<double>(compressed.size()) / original.size();
} 