/// Copyright © Sam Savage 2016
#include "CellularAutomaton.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <numeric>

//http://stackoverflow.com/a/17323608
#define WRAPPED_MOD(n, m) ((n % m) + m) % m

std::string gToHexString(double first, double second, double third)
{
   std::stringstream ss;
   ss << std::setfill('0') << std::hex;
   ss << std::setw(6) << (static_cast<uint32_t>(first * 0xFFFFFF) & 0xFFFFFF);
   ss << std::setw(6) << (static_cast<uint32_t>(second * 0xFFFFFF) & 0xFFFFFF);
   ss << std::setw(6) << (static_cast<uint32_t>(third * 0xFFFFFF) & 0xFFFFFF);
   return ss.str();
}

CellularAutomaton_c::CellularAutomaton_c(const Config_s &config,
   const std::vector<double> &firstRow) : mConfig(config), mLastRow(firstRow)
{
   for(size_t i = 0; i < this->mLastRow.size(); ++i)
   {
      this->mColumnCounts.push_back(ColumnCount_c(i));
   }
   return;
}

bool CellularAutomaton_c::CreateNewRow()
{
   if (this->mInInitialState && (this->mRowCount > this->mConfig.init_row_count))
   {
      this->mInInitialState = false;
      this->mRowCount = 1;
   }
   // if we are half way done and still have 0 entropy, shortcut out
   if (this->mRowCount == (this->mConfig.max_row_count / 2) && (this->CalculateAverageEntropy() == 0))
   {
      return false;
   }

   if (!this->mInInitialState)
   {
      const static double error_delta = this->mConfig.error * cos(this->mConfig.angle);
      if (this->mErrorRow.size() == 0)
      {
         this->mErrorRow.resize(this->mLastRow.size());
         for(size_t i = 0; i < this->mLastRow.size(); ++i)
         {
            this->mErrorRow[i] = this->mLastRow[i] + error_delta;
         }
      }
      else
      {
         for(size_t i = 0; i < this->mLastRow.size(); ++i)
         {
            if (this->mErrorRow[i] != this->mLastRow[i])
            {
               this->mErrorRow[i] = (this->mErrorRow[i] > this->mLastRow[i]) ?
                                     this->mLastRow[i] + error_delta :
                                     this->mLastRow[i] - error_delta;
            }
         }
      }
   }

   const size_t row_size = this->mLastRow.size();
   std::vector<double> new_row(this->mLastRow.size());
   std::vector<double> new_error(this->mErrorRow.size());
   for(size_t i = 0; i < new_row.size(); ++i)
   {
      const double previous = (i == 0) ? this->mLastRow[WRAPPED_MOD(i - 1, row_size)] : this->mLastRow[i-1];
      const double current = this->mLastRow[i];
      const double next = ((i + 1) == row_size) ? this->mLastRow[WRAPPED_MOD(i + 1, row_size)] : this->mLastRow[i+1];
      new_row[i] = this->CalculatePolynomial(previous, current, next);

      if (!this->mInInitialState)
      {
         const double previous_e = (i == 0) ? this->mErrorRow[WRAPPED_MOD(i - 1, this->mErrorRow.size())] : this->mErrorRow[i-1];
         const double current_e = this->mErrorRow[i];
         const double next_e = ((i + 1) == this->mErrorRow.size()) ? this->mErrorRow[WRAPPED_MOD(i + 1, this->mErrorRow.size())] : this->mErrorRow[i+1];
         new_error[i] = this->CalculateErrorPolynomial(previous_e, current_e, next_e);
      }
   }
   this->ChangeLastRow(new_row, new_error);

   // the caller could technically keep asking us to continue if they want
   return ++this->mRowCount < this->mConfig.max_row_count;
}

double CellularAutomaton_c::A() const
{
   return this->mConfig.a;
}

double CellularAutomaton_c::B() const
{
   return this->mConfig.b;
}

double CellularAutomaton_c::CalculateAverageEntropy() const
{
   double avg = 0;
   for(auto &column : this->mColumnCounts)
   {
      avg += column.GetAverageEntropy();
   }
   return avg / this->mColumnCounts.size();
}

double CellularAutomaton_c::CalculateEntropySigma() const
{
   return sqrt(this->GetAverageVariance());
}

double CellularAutomaton_c::GetAverageVariance() const
{
   double avg = 0;
   for(auto &column : this->mColumnCounts)
   {
      avg += column.GetEntropyVariance();
   }
   return avg / this->mColumnCounts.size();
}

double CellularAutomaton_c::GetAverageLyapunovExponent() const
{
   if (this->mErrorFactors.size() == 0) { return 0; }
   const double sum = std::accumulate(this->mErrorFactors.begin(), this->mErrorFactors.end(), 0.0);
   return sum / static_cast<double>(this->mErrorFactors.size());
}

double CellularAutomaton_c::CalculatePolynomial(const double u/*prev*/, const double x/*curr*/, const double v/*next*/) const
{
   // taken from Blair's ALife1Dim Java program
   return (0.5 - 0.5 * cos(M_PI * (this->A() + (this->A() + this->B()) * u 
      + (this->A() - this->B()) * v + this->B() * u * v - 2 * u * x * v)));
}

double CellularAutomaton_c::CalculateErrorPolynomial(const double u/*prev*/, const double x/*curr*/, const double v/*next*/) const
{
   // taken from Blair's ALife1Dim Java program
   return (this->A() + (this->A() + this->B()) * u  +
      (this->A() - this->B()) * v + this->B() * u * v - 2 * u * x * v);
}

void CellularAutomaton_c::ChangeLastRow(std::vector<double> &newRow, std::vector<double> &newError)
{
   this->mLastRow = std::move(newRow);
   if (!this->mInInitialState)
   {
      this->mErrorRow = std::move(newError);
      double error_sum = 0;
      for(size_t i = 0; i < this->mLastRow.size(); ++i)
      {
         const double previous = (i == 0) ? this->mLastRow[WRAPPED_MOD(i - 1, this->mLastRow.size())] : this->mLastRow[i-1];
         const double current = this->mLastRow[i];
         const double next = (i+1 == this->mLastRow.size()) ? this->mLastRow[WRAPPED_MOD(i + 1, this->mLastRow.size())] : this->mLastRow[i+1];
         const std::string value = gToHexString(previous, current, next);
         this->mColumnCounts[i].IncrementCount(value);
         this->mColumnCounts[i].UpdateEntropy(this->mRowCount);
         error_sum += (this->mErrorRow[i] - current) * (this->mErrorRow[i] - current);
      }
      const static double error_delta = std::abs(this->mConfig.error * cos(this->mConfig.angle));
      const double delta = sqrt(error_sum / static_cast<double>(this->mErrorRow.size()));
      const bool is_neg = (delta < 0);
      const double final_value = (delta == 0) ? 0 : log((is_neg ? (0 - delta) : delta) / error_delta);
      this->mErrorFactors.push_back(is_neg ? 0 - final_value : final_value);
   }
   return;
}
