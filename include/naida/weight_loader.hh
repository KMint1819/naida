#pragma once
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

namespace naida
{
class WeightLoader
{
public:
    explicit WeightLoader(const fs::path &);
    void load_safe_tensors(const fs::path &);
    std::vector<std::byte> load_layer(const std::string_view &);

private:
    const fs::path weight_path;
    std::unordered_map<std::string, std::vector<std::byte>> storage;
};
} // namespace naida