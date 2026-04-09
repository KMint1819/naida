#include <cstdio>

#include "fmt/format.h"
#include "naida/weight_loader.hh"
#include "naida/layer.hh"


int main()
{
    // naida::WeightLoader weight_loader("../models/gpt2/model.safetensors");
    // weight_loader.load_layer("123");

    naida::Tensor tensor(naida::Shape { 2, 3 });

    fmt::print("Tensor:\n{}", tensor);
    return 0;
}