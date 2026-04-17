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
    const std::string name;

    // virtual void load_weights(const WeightLoader& weight_loader);
    // virtual std::vector<std::byte> dump_weigths();
    virtual ~Block() {}
    virtual std::vector<Tensor> forward(const std::vector<Tensor>& inputs) = 0;

protected:
    void register_weight(const std::string& str, std::unique_ptr<Tensor> tensor);
    void register_block(const std::string& str, std::unique_ptr<Block> block);
    std::unordered_map<std::string, std::unique_ptr<Tensor>> weights;
    std::unordered_map<std::string, std::unique_ptr<Block>> blocks;
};

class Identity final : public Block
{
public:
    explicit Identity();
    ~Identity() override {};

    std::vector<Tensor> forward(const std::vector<Tensor>& inputs) override;
};

class Gemm final : public Block
{
public:
    explicit Gemm(size_t in_dims, size_t out_dims, DType dtype = DType::FLOAT32);
    ~Gemm() override {};

    std::vector<Tensor> forward(const std::vector<Tensor>& inputs) override;
};

} // namespace naida