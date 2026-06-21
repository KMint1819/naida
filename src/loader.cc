#include "naida/loader.hh"
#include "naida/common.hh"
#include "naida/log.hh"
#include "naida/tensor.hh"

#include <functional>
#include <fstream>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

namespace naida
{
namespace
{
DType parse_dtype(const std::string &s)
{
    if (s == "F32")
        return DType::FLOAT32;

    throw std::runtime_error(fmt::format("Unknown dtype: {}", s));
}
} // namespace
WeightLoader::WeightLoader(const fs::path &path)
    : weight_path(path), storage(std::unordered_map<std::string, std::unique_ptr<Tensor>>())
{
    load_safe_tensors(path);
}
void WeightLoader::load_safe_tensors(const fs::path &path)
{
    std::vector<std::byte> buf = read_binary_file(path);

    uint64_t len_header = 0;
    std::memcpy(&len_header, buf.data(), 8);

    std::vector<char> json_buf(len_header);
    std::memcpy(json_buf.data(), buf.data() + 8, len_header);


    auto buffer_begin = buf.begin() + 8 + len_header;
    json js = json::parse(json_buf);
    for (auto &[k, v] : js.items())
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

        storage.insert({ k, std::move(tensor) });
    }
}
void WeightLoader::assign_to_block(Block &block, bool load_all)
{
    block.load_weights("", storage);

    // after loading, storage should be empty
    if (!storage.empty() && load_all)
    {
        throw std::runtime_error(
        fmt::format("The following keys are not found in the model: {}", fmt::join(storage
                                                                                   | std::views::transform(
                                                                                   [this](const auto &pair)
                                                                                   {
                                                                                       return pair.first;
                                                                                   }),
                                                                                   ", ")));
    }
}
} // namespace naida