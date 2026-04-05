#include "naida/weight_loader.hh"
#include "naida/common.hh"

#include <fstream>

namespace naida
{
// .safetensors:
// 8 bytes: uint64: size of header
// N bytes: json header
// rest of file: raw data

WeightLoader::WeightLoader(const fs::path &path): weight_path(path) {}
std::vector<std::byte> WeightLoader::load_layer(const std::string_view &name)
{
    std::vector<std::byte> buf = read_binary_file(weight_path);
    printf("%d\n", buf.size());
    return buf;
}
} // namespace naida