#include <cstdio>

#include "fmt/format.h"
#include "fmt/xchar.h"
#include "naida/block.hh"
#include "naida/tokenizer.hh"
#include "naida/weight_loader.hh"

class MyModel : public naida::Block
{
public:
    MyModel(int d)
    {
        auto gemm = std::make_unique<naida::Gemm>(d, d);
        this->register_block("gemm0", std::move(gemm));
    }
    virtual ~MyModel() {}
    virtual std::vector<naida::Tensor> forward(const std::vector<naida::Tensor> &inputs)
    { return blocks["gemm0"]->forward(inputs); }
};
void try_icu()
{
    boost::u32regex r =
    boost::make_u32regex(R"('s|'t|'re|'ve|'m|'ll|'d| ?\p{L}+| ?\p{N}+| ?[^\s\p{L}\p{N}]+|\s+(?!\S)|\s+)");

    boost::smatch what;
    if (boost::u32regex_match("/home/local", what, r))
    {
        // extract $1 as a std::string:
        fmt::println("{}", what.str(1));
    }
    else
    {
        throw std::runtime_error("Invalid pathname");
    }
}
int main()
{
    // naida::WeightLoader weight_loader("../models/gpt2/model.safetensors");
    // weight_loader.load_layer("123");

    // naida::Tensor tensor(naida::Shape { 2, 3 });

    // MyModel model(3);
    // naida::Tensor out = model.forward({ tensor })[0];
    // fmt::print("Out:{}\n", out);
    naida::Tokenizer tokenizer("../models/gpt2/tokenizer.json", "../models/gpt2/merges.txt");
    const std::string query = "I'm learning LLM. 臺灣123!";
    tokenizer.tokenize(query);
    // fmt::print("{}\n", query.size());
    return 0;
}