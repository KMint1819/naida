#pragma once
#include <filesystem>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include "naida/tensor.hh"

using json = nlohmann::json;

namespace fs = std::filesystem;

namespace naida
{
class WeightLoader
{
public:
    explicit WeightLoader(const fs::path &);
    void load_safe_tensors(const fs::path &);

public:
    static std::vector<std::byte> load_buffer(std::basic_ifstream<std::byte> &in, const uint64_t start,
                                              const uint64_t end);

private:
    const fs::path weight_path;
    json storage;
};

std::unordered_map<std::string, Tensor> load_safe_tensors(const fs::path &);
} // namespace naida