#include <string>
#include "naida/log.hh"
#include "naida/block.hh"
#include "naida/loader.hh"
#include "naida/tensor.hh"
#include <unordered_map>

namespace naida
{
Block::Block(const std::string& name): name(name) {}
std::vector<Tensor> Block::forward(const std::vector<Tensor>& inputs) {};

void Block::load_weights(const std::string& prefix, json& js,
                         std::function<std::unique_ptr<std::vector<std::byte>>(uint64_t, uint64_t)> load_buffer)
{
}
void Block::register_weight(const std::string& str, std::unique_ptr<Tensor> tensor)
{
    weights.emplace(str, std::move(tensor));
}
void Block::register_block(const std::string& str, std::unique_ptr<Block> block)
{
    blocks.emplace(str, std::move(block));
}


Identity::Identity(const std::string& name): Block(name) {}
std::vector<Tensor> Identity::forward(const std::vector<Tensor>& inputs)
{
    std::vector<Tensor> out;
    std::copy(inputs.begin(), inputs.end(), out.begin());
    return out;
}

Gemm::Gemm(size_t in_dims, size_t out_dims, DType dtype, const std::string& name): Block(name)
{
    std::vector<size_t> shape = { in_dims, out_dims };
    auto weight = std::make_unique<Tensor>(Shape { in_dims, out_dims }, dtype);
    register_weight("weight", std::move(weight));
}

std::vector<Tensor> Gemm::forward(const std::vector<Tensor>& inputs)
{
    return {};
};

AttnBlock::AttnBlock(const int dh, const int h, const std::string& name): Block(name)
{
    auto proj_in = std::make_unique<naida::Gemm>(dh * h, 3 * dh * h);
}
std::vector<naida::Tensor> AttnBlock::forward(const std::vector<naida::Tensor>& xs)
{
    return {};
}
} // namespace naida