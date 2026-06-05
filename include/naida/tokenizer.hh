#include <filesystem>
#include <vector>
#include "boost/regex/icu.hpp"
#include "boost/functional/hash.hpp"

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
    using SymbolToID = std::unordered_map<std::string, uint32_t>;
    using IDToSymbol = std::vector<std::string>;
    using SymbolPair = std::pair<std::string, std::string>;
    using PairRankMap = std::unordered_map<SymbolPair, uint32_t, boost::hash<SymbolPair>>;

public:
    explicit Tokenizer(const fs::path &tokenizer_json_path, const fs::path &merges_path);
    std::vector<uint32_t> tokenize(const std::string &);
    std::string detokenize(const char);

private:
    void parse_tokenizer_json(const fs::path &);
    void parse_merges(const fs::path &);
    void build_symbols_bytes();
    std::vector<std::string> pre_tokenize(const std::string &);
    std::vector<std::string> bpe(std::vector<std::string>);
    ByteToSymbol byte_to_symbol;
    SymbolToID symbol_to_id;
    IDToSymbol id_to_symbol;
    PairRankMap pair_ranks;

    const boost::u32regex pattern;
};
} // namespace naida