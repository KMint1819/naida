#pragma once
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace naida
{
class WeightLoader
{
public:
    explicit WeightLoader(const fs::path &);
    std::vector<std::byte> load_layer(const std::string_view &);

private:
    const fs::path weight_path;
};
} // namespace naida