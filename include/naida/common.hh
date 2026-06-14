#pragma once

#include <filesystem>
#include <vector>
#include <random>
#include <fmt/core.h>
#include <ostream>

#include <nlohmann/json.hpp>


using nlohmann::json;
namespace fs = std::filesystem;

namespace naida
{
std::vector<std::byte> read_binary_file(const fs::path& path);
std::vector<std::byte> read_binary_file(const fs::path& path, const size_t offset, const size_t length);

inline float get_random_number()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);

    return dis(gen);
}
class Formattable
{
public:
    virtual std::string to_string() const = 0;
};
} // namespace naida

// to allow fmtlib with custom class
template<typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<naida::Formattable, T>, char>> : fmt::formatter<std::string>
{
    auto format(const naida::Formattable& formattable, format_context& ctx) const
    {
        return formatter<std::string>::format(formattable.to_string(), ctx);
    }
};