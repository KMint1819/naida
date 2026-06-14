#include "naida/loader.hh"
#include "naida/common.hh"
#include "naida/log.hh"
#include "naida/tensor.hh"

#include <functional>
#include <fstream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace naida
{
WeightLoader::WeightLoader(const fs::path &path): weight_path(path)
{
    load_safe_tensors(path);
}
void WeightLoader::load_safe_tensors(const fs::path &path) {}

// .safetensors:
// 8 bytes: uint64: size of header
// N bytes: json header
// rest of file: raw data
std::vector<std::byte> WeightLoader::load_buffer(std::basic_ifstream<std::byte> &in, const uint64_t start,
                                                 const uint64_t end)
{
    uint64_t num_bytes = end - start;
    in.seekg(start);
    std::vector<std::byte> res(std::istreambuf_iterator<std::byte>(in), {});
    return res;
}

static DType parse_dtype(const std::string &s)
{
    if (s == "F32")
        return DType::FLOAT32;

    throw std::runtime_error(fmt::format("Unknown dtype: {}", s));
}

std::unordered_map<std::string, Tensor> load_safe_tensors(const fs::path &path)
{
    std::vector<std::byte> buf = read_binary_file(path);

    uint64_t len_header = 0;
    std::memcpy(&len_header, buf.data(), 8);

    std::vector<char> json_buf(len_header);
    std::memcpy(json_buf.data(), buf.data() + 8, len_header);


    auto buffer_begin = buf.begin() + 8 + len_header;
    json storage = json::parse(json_buf);
    std::unordered_map<std::string, std::unique_ptr<Tensor>> map;
    for (auto &[k, v] : storage.items())
    {
        if (k == "__metadata__")
            continue;
        uint64_t data_start = v["data_offsets"][0];
        uint64_t data_end = v["data_offsets"][1];
        DType dtype = parse_dtype(v["dtype"]);

        std::vector<size_t> sha(v["shape"]);
        Shape shape(sha.begin(), sha.end());


        auto tensor_buffer = std::make_unique<std::vector<std::byte>>(data_end - data_start);
        std::copy(buffer_begin + data_start, buffer_begin + data_end, tensor_buffer->begin());

        auto tensor = std::make_unique<Tensor>(std::move(tensor_buffer), shape, dtype);

        map.insert({ k, std::move(tensor) });
    }

    for (const auto &[k, v] : map)
    {
        NAIDA_INFO("{}: ", k);
    }
}
// void WeightLoader::traverse_assign(Block &model)
// {
//     std::basic_ifstream<std::byte> in(weight_path);
//     model.load_weights("", storage,
//                        [this, &in](uint64_t start, uint64_t end)
//                        {
//                            std::vector<std::byte> buf = WeightLoader::load_buffer(in, start, end);
//                            return std::make_unique<std::vector<std::byte>>(buf);
//                        });
// }
} // namespace naida