#include "naida/common.hh"
#include <fstream>

namespace naida
{

std::vector<std::byte> read_binary_file(const fs::path& path, const size_t offset, const size_t length)
{
    std::ifstream inputFile(path, std::ios_base::binary);

    inputFile.seekg(offset, std::ios::end);
    std::vector<std::byte> buffer(length);
    inputFile.read(reinterpret_cast<char*>(buffer.data()), length);

    return buffer;
}

std::vector<std::byte> read_binary_file(const fs::path& path)
{
    std::ifstream inputFile(path, std::ios_base::binary);

    inputFile.seekg(0, std::ios::end);
    auto length = inputFile.tellg();
    inputFile.seekg(0);

    std::vector<std::byte> buffer(length);
    inputFile.read(reinterpret_cast<char*>(buffer.data()), length);

    return buffer;
}
} // namespace naida