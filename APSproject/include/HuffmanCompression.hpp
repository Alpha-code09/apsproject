#ifndef HUFFMAN_COMPRESSION_HPP
#define HUFFMAN_COMPRESSION_HPP

#include <string>
#include <queue>
#include <unordered_map>
#include <memory>
#include <vector>

class HuffmanCompression {
private:
    struct Node {
        char character;
        size_t frequency;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        
        Node(char c, size_t freq) : character(c), frequency(freq), left(nullptr), right(nullptr) {}
        Node(size_t freq, std::shared_ptr<Node> l, std::shared_ptr<Node> r) 
            : character('\0'), frequency(freq), left(l), right(r) {}
    };
    
    struct CompareNodes {
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return a->frequency > b->frequency;
        }
    };
    
    std::shared_ptr<Node> root_;
    std::unordered_map<char, std::string> encoding_map_;
    
    void buildEncodingMap(const std::shared_ptr<Node>& node, const std::string& code);
    
public:
    // Compress a string and return compressed data
    std::vector<unsigned char> compress(const std::string& data);
    
    // Decompress data back to string
    std::string decompress(const std::vector<unsigned char>& compressed_data);
    
    // Save compressed data to file
    bool saveToFile(const std::string& filename, const std::vector<unsigned char>& compressed_data);
    
    // Load compressed data from file
    std::vector<unsigned char> loadFromFile(const std::string& filename);
    
    // Get the compression ratio
    double getCompressionRatio(const std::string& original, const std::vector<unsigned char>& compressed);
};

#endif // HUFFMAN_COMPRESSION_HPP 