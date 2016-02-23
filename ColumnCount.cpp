/// Copyright © Sam Savage 2016
#include "ColumnCount.hpp"
#include <cmath>
#include <numeric>

ColumnCount_c::ColumnCount_c(int column) : mColumnNumber(column)
{
   return;
}

void ColumnCount_c::IncrementCount(const std::string &value)
{
   if (this->mCount.find(value) == this->mCount.end())
   {
      this->mCount[value] = 1;
   }
   else
   {
      this->mCount[value] += 1;
   }
   return;
}

void ColumnCount_c::UpdateEntropy(const size_t rowCount)
{
   this->mEntropy.push_back(this->CalculateEntropy(rowCount));
   return;
}

double ColumnCount_c::GetAverageEntropy() const
{
   const double sum = std::accumulate(this->mEntropy.begin(), this->mEntropy.end(), 0.0);
   return sum / static_cast<double>(this->mEntropy.size());
}

double ColumnCount_c::GetEntropyVariance() const
{
   const double mean = this->GetAverageEntropy();
   double variance = 0;
   for(auto &entropy : this->mEntropy)
   {
      const double delta = entropy - mean;
      variance += (delta * delta);
   }
   return variance / static_cast<double>(this->mEntropy.size());
}

double ColumnCount_c::CalculateEntropy(const size_t rowCount) const
{
   double entropy = 0;
   for(auto &count_pair : this->mCount)
   {
      const double weight = static_cast<double>(count_pair.second) / static_cast<double>(rowCount);
      entropy += (weight * std::log2(weight));
   }
   return 0 - entropy;
}

