#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "naida/tensor.hh"


namespace naida
{
class Block
{
public:
    Block(const std::string& name);
    virtual ~Block() {}
    virtual std::vector<Tensor> forward(const std::vector<Tensor>& inputs);
    void load_weights(const std::string& prefix, std::unordered_map<std::string, std::unique_ptr<Tensor>>& map);


protected:
    std::string name;
    void register_weight(const std::string& str, std::unique_ptr<Tensor> tensor);
    void register_block(const std::string& str, std::unique_ptr<Block> block);
    std::unordered_map<std::string, std::unique_ptr<Tensor>> weights;
    std::unordered_map<std::string, std::unique_ptr<Block>> blocks;
};

class Identity final : public Block
{
public:
    explicit Identity(const std::string& name = "identity");
    ~Identity() override {};

    std::vector<Tensor> forward(const std::vector<Tensor>& inputs) override;
};

class Embedding final : public Block
{
public:
    explicit Embedding(const std::string& name = "emb");
    ~Embedding() override {};

    std::vector<Tensor> forward(const std::vector<Tensor>& inputs) override;
};

class Gemm final : public Block
{
public:
    explicit Gemm(size_t in_dims, size_t out_dims, DType dtype = DType::FLOAT32, const std::string& name = "gemm");
    ~Gemm() override {};

    std::vector<Tensor> forward(const std::vector<Tensor>& inputs) override;
};
class AttnBlock final : public Block
{
public:
    AttnBlock(const int dh, const int h, const std::string& name = "attn");
    std::vector<naida::Tensor> forward(const std::vector<naida::Tensor>&);
};
} // namespace naida