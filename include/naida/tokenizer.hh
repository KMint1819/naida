#include <filesystem>
#include <vector>
#include <regex>

namespace fs = std::filesystem;
namespace naida
{
class Tokenizer
{
    // That's a good question.
    //    |
    //    v
    // [That, 's, a, good, ques, tion]
    //    |
    //    v
    // [37, 32, 88, 18, 17, 90]

public:
    explicit Tokenizer(const fs::path&);
    std::vector<char> tokenize(const std::string&);
    std::string detokenize(const char);

private:
    std::vector<std::string> pre_tokenize(const std::string&);

    const std::regex pattern { R"('s|'t|'re|'ve|'m|'ll|'d| ?[A-Za-z]+| ?[0-9]+| ?[^\sA-Za-z0-9]+|\s+$|\s+)" };
};
} // namespace naida