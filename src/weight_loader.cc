#include "naida/weight_loader.hh"
#include "naida/common.hh"

#include <fstream>
#include <memory>
#include <cstring>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


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

    uint64_t len_header = 0;
    std::memcpy(&len_header, buf.data(), 8);
    printf("header size: %ld\n", len_header);

    std::vector<char> json_buf;
    json_buf.resize(len_header);

    std::memcpy(json_buf.data(), buf.data() + 8, len_header);

    auto js = json::parse(json_buf);
    printf("%s\n", js.dump(4).data());
    return buf;
}
} // namespace naida