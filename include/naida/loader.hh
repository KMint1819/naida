#pragma once
#include <filesystem>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include "naida/tensor.hh"
#include "naida/block.hh"

using json = nlohmann::json;

namespace fs = std::filesystem;

namespace naida
{
class WeightLoader
{
public:
    explicit WeightLoader(const fs::path &);
    void load_safe_tensors(const fs::path &);
    void assign_to_block(Block &block);

private:
    const fs::path weight_path;
    std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<Tensor>>> storage;
};
} // namespace naida