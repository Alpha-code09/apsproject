# Enhanced Search Engine

A powerful C++ search engine implementation with features including full-text search, autocomplete suggestions, and spell checking.

## Features

- Full-text document search using TF-IDF ranking
- Autocomplete suggestions using Trie data structure
- Spell checking and correction
- Document indexing and compression
- Save/load functionality for search indices

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Google Test (automatically downloaded during build)

## Building the Project

```bash
# Create build directory
mkdir build && cd build

# Configure CMake
cmake ..

# Build the project
cmake --build .

# Run tests
ctest
```

## Usage

After building, you can run the search engine using:

```bash
./search_engine
```

Available commands:
- `add <path>` - Add a document to the search engine
- `search <query>` - Search for documents
- `complete <prefix>` - Get autocomplete suggestions
- `spell <word>` - Get spelling suggestions
- `save <file>` - Save index to file
- `load <file>` - Load index from file
- `quit` - Exit the program

### Example Usage

```bash
# Add a document
add documents/sample.txt

# Search for documents
search machine learning

# Get autocomplete suggestions
complete mach

# Get spelling suggestions
spell learningg
```

## Project Structure

```
.
├── include/             # Header files
├── src/                # Source files
├── test/               # Test files
├── docs/               # Documentation
├── CMakeLists.txt     # Build configuration
└── README.md          # This file
```

## Implementation Details

The search engine uses several key components:
- InvertedIndex: For efficient document searching
- TFIDFCalculator: For ranking search results
- Trie: For autocomplete functionality
- SpellCorrector: For spelling suggestions
- HuffmanCompression: For index compression

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details. #   a p s p r o j e c t 4  
 