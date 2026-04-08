#pragma once
#include <string>
#include "naida/weight_loader.hh"
#include "naida/tensor.hh"
#include <unordered_map>

namespace naida
{
class Block
{
public:
    virtual void load_weights(const WeightLoader& weight_loader);
    virtual std::vector<std::byte> dump_weigths();
    virtual ~Block() {}

    const std::string name;

    virtual std::vector<Tensor> forward(const std::vector<Tensor>& inputs) = 0;

protected:
    void register_weight(const std::string& str, std::unique_ptr<Tensor> tensor)
    { weights.emplace(str, std::move(tensor)); }
    void register_block(const std::string& str, std::unique_ptr<Block> block)
    { blocks.emplace(str, std::move(block)); }

private:
    std::unordered_map<std::string, std::unique_ptr<Tensor>> weights;
    std::unordered_map<std::string, std::unique_ptr<Block>> blocks;
};

class Gemm : public Block
{
public:
    Gemm(size_t in_dims, size_t out_dims, DType dtype = DType::FLOAT32)
    {
        std::vector<size_t> shape = { in_dims, out_dims };
        auto weight = std::make_unique<Tensor>(Shape { in_dims, out_dims }, dtype);
        register_weight("weight", std::move(weight));
    }

    std::vector<Tensor> forward(const std::vector<Tensor>& inputs) override
    { return inputs; };
};

} // namespace naida