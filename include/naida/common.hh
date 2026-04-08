#pragma once

#include <fstream>
#include <filesystem>
#include <vector>
#include <random>


namespace fs = std::filesystem;

namespace naida
{
std::vector<std::byte> read_binary_file(const fs::path&);

inline float get_random_number()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);

    return dis(gen);
}

template<typename T>
std::string format_as(const T& obj)
{
    std::stringstream ss;
    ss << obj << "\n";
    return ss.str();
}

} // namespace naida