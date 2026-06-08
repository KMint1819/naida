#include "naida/weight_loader.hh"
#include "naida/common.hh"
#include "naida/log.hh"

#include <fstream>
#include <memory>
#include <cstring>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace naida
{
WeightLoader::WeightLoader(const fs::path &path): weight_path(path)
{
    load_safe_tensors(path);
}
void WeightLoader::load_safe_tensors(const fs::path &path)
{
    std::vector<std::byte> buf = read_binary_file(weight_path);

    uint64_t len_header = 0;
    std::memcpy(&len_header, buf.data(), 8);

    std::vector<char> json_buf(len_header);

    std::memcpy(json_buf.data(), buf.data() + 8, len_header);

    auto js = json::parse(json_buf);
    NAIDA_TRACE("naida {}", js.dump());
}

// .safetensors:
// 8 bytes: uint64: size of header
// N bytes: json header
// rest of file: raw data
std::vector<std::byte> WeightLoader::load_layer(const std::string_view &name)
{
    std::vector<std::byte> buf = read_binary_file(weight_path);

    uint64_t len_header = 0;
    std::memcpy(&len_header, buf.data(), 8);

    std::vector<char> json_buf(len_header);

    std::memcpy(json_buf.data(), buf.data() + 8, len_header);

    auto js = json::parse(json_buf);
    return buf;
}
} // namespace naida