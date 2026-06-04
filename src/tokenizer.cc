#include "naida/common.hh"
#include "naida/tokenizer.hh"
#include <fstream>
#include <fmt/ranges.h>
#include <fmt/format.h>

namespace
{
std::string unicode_to_string_buffer(const uint32_t code_point)
{
    // U+0000   ~ U+007F  : 0yyyzzzz
    // U+0080   ~ U+07FF  : 110xxxyy	10yyzzzz
    // U+0800   ~ U+FFFF  : 1110wwww	10xxxxyy	10yyzzzz
    // U+010000 ~ U+10FFFF: 11110uvv	10vvwwww	10xxxxyy	10yyzzzz
    std::string s;
    if (code_point <= 0x7F)
    {
        s.push_back(static_cast<char>(code_point));
    }
    else if (code_point <= 0x7FF)
    {
        unsigned char first = 0x6 << 5 | (code_point >> 6);
        unsigned char second = 0x2 << 6 | (code_point & 0x3F);
        s.push_back(first);
        s.push_back(second);
    }
    else if (code_point <= 0xFFFF)
    {
        unsigned char first = 0xE << 4 | (code_point >> 12);
        unsigned char second = 0x2 << 6 | (code_point >> 6 & 0x3F);
        unsigned char third = 0x2 << 6 | (code_point & 0x3F);
        s.push_back(first);
        s.push_back(second);
        s.push_back(third);
    }
    else if (code_point <= 0x10FFFF)
    {
        unsigned char first = 0x1E << 3 | (code_point >> 18 & 0x7);
        unsigned char second = 0x2 << 6 | (code_point >> 12 & 0x3F);
        unsigned char third = 0x2 << 6 | (code_point >> 6 & 0x3F);
        unsigned char fourth = 0x2 << 6 | (code_point & 0x3F);
        s.push_back(first);
        s.push_back(second);
        s.push_back(third);
        s.push_back(fourth);
    }
    return s;
}
} // namespace

namespace naida
{
Tokenizer::Tokenizer(const fs::path& tokenizer_json_path, const fs::path& merges_path)
    : pattern { boost::make_u32regex(
      R"('s|'t|'re|'ve|'m|'ll|'d| ?[[:L*:]]+| ?[[:N*:]]+| ?[^\s[:L*:][:N*:]]+|\s+(?!\S)|\s+)") }
{
    // the linter error is because we're using pairs as a key for the merges. we need to provide a hash function for
    // pair<char,char>. maybe use https://www.boost.org/doc/libs/latest/libs/container_hash/doc/html/hash.html
    parse_tokenizer_json(tokenizer_json_path);
    parse_merges(merges_path);
    build_symbols_bytes();
}

void Tokenizer::build_symbols_bytes()
{
    // !: 33
    // ~: 126
    // ¡: 161
    // ¬: 172
    // ®: 174
    // ÿ: 255
    int counter = 0;
    for (int i = 0; i < 256; i++)
    {
        std::string s;
        if ((i >= 33 && i <= 126) || (i >= 161 && i <= 172) || (i >= 174 && i <= 255))
        {
            s = unicode_to_string_buffer(i);
        }
        else
        {
            s = unicode_to_string_buffer(256 + counter);
            counter++;
        }
        byte_to_symbol[i] = s;
        symbol_to_id[s] = i;
    }
}

void Tokenizer::parse_tokenizer_json(const fs::path& path)
{
    std::ifstream ifs(path);
    json data = json::parse(ifs);
}

void Tokenizer::parse_merges(const fs::path& path)
{
    // merges.txt structure:
    // #version: x.y
    // a b
    // c d
    //...
    std::ifstream ifs(path);
    std::string a, b;

    // get rid of the first version line
    ifs >> a >> b;
    uint32_t rank_count = 1;
    while (ifs >> a >> b)
    {
        pair_ranks.insert({ SymbolPair { a, b }, rank_count });
        rank_count++;
    }
}

std::vector<char> Tokenizer::tokenize(const std::string& input)
{
    // 1. Pre-tokenize
    // 2. For each token:
    // 2.a Parse as bytes
    // 2.b Convert to unicode
    // 3. Start BPE
    fmt::print("Tokenizing <{}>\n", input);
    std::vector<char> ans;
    auto pre_tokenized = pre_tokenize(input);
    for (const std::string s : pre_tokenized)
    {
        fmt::print("Tokens for '{}': ", s);
        std::vector<std::string> symbols;
        for (const char ch : s)
        {
            std::string symbol = byte_to_symbol[ch];
            symbols.push_back(symbol);
        }
        fmt::println("Symbols: <{}>", symbols);

        std::vector<std::string> tokens = bpe(symbols);
        for (const std::string token : tokens)
        {
            ans.push_back(static_cast<char>(symbol_to_id[token]));
        }
    }
    return ans;
}
std::vector<std::string> Tokenizer::bpe(const std::vector<std::string>& symbols)
{
    auto get_pairs = [](const std::vector<std::string>& x)
    {
        std::vector<SymbolPair> pairs;
        std::string prev_symbol = x[0];
        const int len = x.size();
        for (int i = 1; i < len; i++)
        {
            pairs.push_back({ prev_symbol, x[i] });
            prev_symbol = x[i];
        }
        return pairs;
    };
    std::vector<SymbolPair> pairs = get_pairs(symbols);
    for (const auto& pair : pairs)
    {
        fmt::print("pair: <<{}> <{}>>, {} {}, ", pair.first, pair.second, symbol_to_id[pair.first],
                   symbol_to_id[pair.second]);
        if (pair_ranks.contains(pair))
            fmt::println("rank {}", pair_ranks[pair]);
        else
            fmt::println("no rank");
    }

    return {};
}
std::string Tokenizer::detokenize(const char) {}

std::vector<std::string> Tokenizer::pre_tokenize(const std::string& input)
{
    std::string s = input;
    auto it = boost::make_u32regex_iterator(input, pattern);
    auto end = boost::u32regex_iterator<std::string::const_iterator>();

    size_t num_found = std::distance(it, {});

    std::vector<std::string> result;
    result.reserve(num_found);

    for (; it != end; it++)
    {
        fmt::println("pushing {}", it->str());
        result.push_back(it->str());
    }
    return result;
}
} // namespace naida