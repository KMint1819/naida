#include <fstream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace naida
{
std::vector<std::byte> read_binary_file(const fs::path&);
}