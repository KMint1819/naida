#include <cstdio>

#include "fmt/format.h"
#include "naida/weight_loader.hh"
#include "naida/block.hh"


class MyModel : public naida::Block
{
public:
    MyModel(int d)
    {
        auto gemm = std::make_unique<naida::Gemm>(d, d);
        this->register_block("gemm0", std::move(gemm));
    }
    virtual ~MyModel() {}
    virtual std::vector<naida::Tensor> forward(const std::vector<naida::Tensor>& inputs)
    { return blocks["gemm0"]->forward(inputs); }
};
int main()
{
    // naida::WeightLoader weight_loader("../models/gpt2/model.safetensors");
    // weight_loader.load_layer("123");

    naida::Tensor tensor(naida::Shape { 2, 3 });

    MyModel model(3);
    naida::Tensor out = model.forward({ tensor })[0];
    fmt::print("Out:{}\n", out);

    return 0;
}