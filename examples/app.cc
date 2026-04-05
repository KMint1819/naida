#include <cstdio>

#include "naida/weight_loader.hh"

int main()
{
    naida::WeightLoader weight_loader("../models/gpt2/model.safetensors");
    weight_loader.load_layer("123");
    return 0;
}