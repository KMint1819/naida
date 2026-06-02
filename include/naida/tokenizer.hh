#include <filesystem>
#include <regex>
#include <vector>
#include "boost/regex/icu.hpp"

namespace fs = std::filesystem;
namespace naida
{

// Implementation of https://github.com/openai/gpt-2/blob/master/src/encoder.py
class Tokenizer
{
    // That's a good question.
    //    |
    //    v
    // [That, 's, a, good, ques, tion]
    //    |
    //    v
    // [37, 32, 88, 18, 17, 90]
    using ByteToSymbol = std::array<std::string, 256>;
    using SymbolToByte = std::unordered_map<std::string, uint8_t>;

public:
    explicit Tokenizer(const fs::path &);
    std::vector<char> tokenize(const std::string &);
    std::string detokenize(const char);

private:
    std::vector<std::string> pre_tokenize(const std::string &);
    ByteToSymbol byte_to_symbol;
    SymbolToByte symbol_to_byte;

    const boost::u32regex pattern;
};
} // namespace naida