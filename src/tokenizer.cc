#include "naida/common.hh"
#include "naida/tokenizer.hh"
#include <fstream>
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
Tokenizer::Tokenizer(const fs::path& p)
{
    std::ifstream ifs(p);
    json data = json::parse(ifs);

    // construct byte to symbol:
    int counter = 0;
    for (uint32_t c = 0; c < 256; c++)
    {
        std::string s;
        if ((c >= '!' && c <= '~') || (c >= '¡' && c <= '¬') || (c >= '®' && c <= 'ÿ'))
        {
            s.push_back(static_cast<char>(c));
        }
        else
        {
            // Push back the bytes of the codepoint
            s = unicode_to_string_buffer(counter + 256);
            counter++;
        }
        byte_to_symbol[c] = s;
    }
}

std::vector<char> Tokenizer::tokenize(const std::string& input)
{
    // 1. Pre-tokenize
    // 2. For each token:
    // 2.a Parse as bytes
    // 2.b Convert to unicode
    // 3. Start BPE
    fmt::print("{}\n", input);
    auto ans = pre_tokenize(input);
    for (const std::string s : ans)
    {
        fmt::print("[{}] ", s);
    }
    fmt::println("");
    return {};
}
std::string Tokenizer::detokenize(const char) {}

std::vector<std::string> Tokenizer::pre_tokenize(const std::string& input)
{
    std::string s = input;
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
    auto words_end = std::sregex_iterator();
    size_t num_found = std::distance(words_begin, words_end);

    std::vector<std::string> result;
    result.reserve(num_found);

    for (auto it = words_begin; it != words_end; it++)
    {
        result.push_back(it->str());
    }
    return result;
}
} // namespace naida