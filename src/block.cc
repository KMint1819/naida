#include <string>
#include "naida/block.hh"
#include "naida/weight_loader.hh"
#include "naida/tensor.hh"
#include <unordered_map>

namespace naida
{
void Block::register_weight(const std::string& str, std::unique_ptr<Tensor> tensor)
{ weights.emplace(str, std::move(tensor)); }
void Block::register_block(const std::string& str, std::unique_ptr<Block> block)
{ blocks.emplace(str, std::move(block)); }


Gemm::Gemm(size_t in_dims, size_t out_dims, DType dtype)
{
    std::vector<size_t> shape = { in_dims, out_dims };
    auto weight = std::make_unique<Tensor>(Shape { in_dims, out_dims }, dtype);
    register_weight("weight", std::move(weight));
}

std::vector<Tensor> Gemm::forward(const std::vector<Tensor>& inputs)
{ return inputs; };

} // namespace naida