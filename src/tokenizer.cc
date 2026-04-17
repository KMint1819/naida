#include "naida/common.hh"
#include "naida/tokenizer.hh"
#include <fstream>
#include <fmt/format.h>

namespace naida
{
Tokenizer::Tokenizer(const fs::path& p)
{
    std::ifstream ifs(p);
    json data = json::parse(ifs);
}

std::vector<char> Tokenizer::tokenize(const std::string& input)
{
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