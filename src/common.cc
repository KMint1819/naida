#include "naida/common.hh"

namespace naida
{

std::vector<std::byte> read_binary_file(const fs::path& path)
{
    printf("%d\n", fs::is_regular_file(path));
    std::ifstream inputFile(path, std::ios_base::binary);

    inputFile.seekg(0, std::ios::end);
    auto length = inputFile.tellg();
    inputFile.seekg(0);

    std::vector<std::byte> buffer(length);
    inputFile.read(reinterpret_cast<char*>(buffer.data()), length);

    return buffer;
}
} // namespace naida