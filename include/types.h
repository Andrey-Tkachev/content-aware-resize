#pragma once

#include <memory>

typedef long long WeightData;
typedef uchar EnergyData;

typedef std::vector<int> seam; // Path to add/remove along

typedef std::vector<std::shared_ptr<seam>> Seams;
typedef std::vector<std::vector<int>> SeamsSorted;
