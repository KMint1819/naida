#include <cstdio>

#include "naida/weight_loader.hh"

int main()
{
    naida::WeightLoader weight_loader("../models/my.json");
    weight_loader.load_layer("123");
    return 0;
}