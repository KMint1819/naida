#include <cstdio>

#include <fmt/format.h>
#include <fmt/xchar.h>
#include "naida/block.hh"
#include "naida/tokenizer.hh"
#include "naida/loader.hh"
#include "naida/log.hh"
#include "nlohmann/json_fwd.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

class MyModel : public naida::Block
{
public:
    MyModel(const int d): naida::Block("")
    {

        auto gemm = std::make_unique<naida::Gemm>(d, d);
        this->register_block("gemm0", std::move(gemm));
    }
    virtual ~MyModel() {}
    virtual std::vector<naida::Tensor> forward(const std::vector<naida::Tensor>& inputs)
    {
        return blocks["gemm0"]->forward(inputs);
    }
};
void try_weight_loader() {}
void try_forward()
{
    naida::Tensor tensor(naida::Shape { 2, 3 });
    MyModel model(3);
    naida::Tensor out = model.forward({ tensor })[0];
    NAIDA_INFO("Out:{}\n", out);
}
void try_block()
{
    naida::WeightLoader loader("/workspace/models/gpt2/model.safetensors");
    naida::AttnBlock block(768, 3, "h.0");
    loader.assign_to_block(block);
}
int main()
{
    // naida::Tokenizer tokenizer("../models/gpt2/tokenizer.json", "../models/gpt2/merges.txt");
    // const std::string query = "I'm learning LLM. 臺灣123!";
    // tokenizer.tokenize(query);
    try_block();

    return 0;
}